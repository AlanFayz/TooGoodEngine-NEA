#include "EnviormentMap.h"

#include "API/OpenGL/Program.h"
#include "API/OpenGL/Framebuffer.h"
#include "API/OpenGL/Command.h"

#include "External/stb/stb_image.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TooGoodEngine {

	static Ref<OpenGL::Program> s_EnviormentProgram;

	static const char* s_EnviormentMapSourceVertex =
	R"(
		#version 460 core
		
		layout(location = 0) in vec3 a_Position;

		out vec3 o_WorldPosition;
		
		uniform mat4 u_ViewProjection;
		
		void main()
		{
			o_WorldPosition = a_Position;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		}
	)";

	static const char* s_EnviromentMapSourceFragment = 
	R"(
		#version 460 core
		
		layout(location = 0) out vec4 OutColor;

		uniform sampler2D EquirectangularMap;

		in vec3 o_WorldPosition;
		
		//this function converts from 3D world pos to 2D uv coords
		const vec2 inverseATan = vec2(0.1591, 0.3183);
		vec2 SampleSphericalMap(in vec3 position)
		{
			vec2 uv = vec2(atan(position.z, position.x), asin(position.y));
			uv *= inverseATan;
			uv += 0.5;
			return uv;
		}
		
		void main()
		{
			vec2 uv = SampleSphericalMap(normalize(o_WorldPosition));
			OutColor = texture(EquirectangularMap, uv);
			OutColor.a = 1.0;
		}
	)";

	Ref<EnviormentMap> EnviormentMap::LoadEnviromentMapAssetFromFile(const std::filesystem::path& path)
	{
		if (!s_EnviormentProgram)
		{
			OpenGL::ShaderSourceMap shaderMap;
			shaderMap[OpenGL::ShaderType::FragmentShader] = s_EnviromentMapSourceFragment;
			shaderMap[OpenGL::ShaderType::VertexShader] = s_EnviormentMapSourceVertex;

			s_EnviormentProgram = CreateRef<OpenGL::Program>(shaderMap);
		}

		Ref<EnviormentMap> enviormentMap = CreateRef<EnviormentMap>();

		const int desiredChannels = 4;
		int channels = 0;
		stbi_set_flip_vertically_on_load(true);

		float* imageData = nullptr;
		int width = 0, height = 0;

		imageData = stbi_loadf(path.string().c_str(), &width, &height, &channels, desiredChannels);

		const char* failure = stbi_failure_reason();
		if (failure)
		{
			TGE_LOG_ERROR("failed to load image ", path, " because ", failure);
			return nullptr;
		}

		//first load into normal texture
		Ref<OpenGL::Texture2D> hdrTexture;

		{
			OpenGL::Texture2DInfo info{};
			info.Type = OpenGL::Texture2DType::Texture;
			info.Format = OpenGL::Texture2DFormat::RGBA32F;
			info.Width = (uint32_t)width;
			info.Height = (uint32_t)height;
			info.Data = imageData;

			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;



			hdrTexture = CreateRef<OpenGL::Texture2D>(info);
		}

		if (imageData)
			stbi_image_free(imageData);

		//create our cubemap
		{
			OpenGL::Texture2DInfo info{};
			info.Type = OpenGL::Texture2DType::CubeMap;
			info.Format = OpenGL::Texture2DFormat::RGBA32F;
			info.Width = g_EnviormentMapWidth; //May make variable in the future.
			info.Height = g_EnviormentMapHeight;
			info.MipMapLevels = g_NumberOfMipMaps;

			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::WrapModeR] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::MipMapLinear;
			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;

			enviormentMap->m_Texture = OpenGL::Texture2D(info);
		}

		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
		glm::mat4 viewMatrices[6] = 
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),   glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec3(0.0f, 0.0f, 1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),   glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),  glm::vec3(0.0f, -1.0f, 0.0f))
		};

		OpenGL::FramebufferInfo info;
		info.ColorAttachments.push_back(&enviormentMap->m_Texture);
		info.DepthAttachment = nullptr;
		OpenGL::Framebuffer framebuffer(info);

		OpenGL::Command::SetViewport(g_EnviormentMapWidth, g_EnviormentMapHeight);

		framebuffer.Bind();

		OpenGL::Command::ClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		OpenGL::Command::ClearDepth();

		for (uint32_t i = 0; i < 6; i++)
		{
			//TODO: add as a part of the framebuffer api
			glNamedFramebufferTextureLayer(framebuffer.GetHandle(), GL_COLOR_ATTACHMENT0, enviormentMap->m_Texture.GetHandle(), 0, i);

			s_EnviormentProgram->Use();

			hdrTexture->Bind(0);
			s_EnviormentProgram->SetUniform("u_ViewProjection", projection* viewMatrices[i]);
			s_EnviormentProgram->SetUniform("EquirectangularMap", 0);

			_RenderCube(s_EnviormentProgram.get());
		}

		framebuffer.Unbind();
		

		enviormentMap->m_Texture.GenerateMipmaps();

		return enviormentMap; 
	}
	void EnviormentMap::_RenderCube(OpenGL::Program* program)
	{
		static float vertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		//i could initalize static buffers and vertex array
		//but they are small and its not something thats
		//going to affect frame rate, may change in the future
		OpenGL::BufferInfo info{};
		info.Capacity = sizeof(float) * 108;
		info.Data = vertices;
		
		OpenGL::Buffer buffer(info);
		
		OpenGL::VertexInputMap map
		{ {"a_Position",    {OpenGL::VertexType::Vector3, false}} };

		OpenGL::VertexArray vertexArray;
		vertexArray.AttachVertexInput(&buffer, program, map);

		OpenGL::Command::DrawArrays(program, &vertexArray, OpenGL::DrawMode::Triangle, 0, 36);
	}
}