#include "BatchRenderer.h"

namespace TooGoodEngine {

	static BatchRendererData s_Data;

	void BatchRenderer::Init()
	{
		s_Data.ShaderDirectory = __FILE__;
		s_Data.ShaderDirectory = s_Data.ShaderDirectory.parent_path() / "Shaders";

		_CreatePrograms();
		_CreateBuffers();
		_CreateTextures();
		_CreateFramebuffers();
	}

	void BatchRenderer::Begin(Camera* camera)
	{
		TGE_VERIFY(camera, "not a valid camera");

		s_Data.SceneCamera = camera;	

		s_Data.QuadVertexOffset = 0;
		s_Data.QuadIndexCount   = 0;
	}

	void BatchRenderer::End()
	{
	}

	void BatchRenderer::_CreateBuffers()
	{
		{
			OpenGL::BufferInfo info{};
			info.Capacity = s_Data.QuadIndexCount * sizeof(uint32_t);
			info.Masks = OpenGL::BufferOptionMapCoherient  |
						 OpenGL::BufferOptionMapPersistent |
						 OpenGL::BufferOptionMapWrite;

			s_Data.QuadIndexBuffer = OpenGL::Buffer(info);
			s_Data.QuadVertexArray.AttachIndexBuffer(&s_Data.QuadIndexBuffer);
		}

		{
			OpenGL::BufferInfo info{};
			info.Capacity = 10 * sizeof(Quad);
			info.Masks = OpenGL::BufferOptionMapCoherient |
						 OpenGL::BufferOptionMapPersistent |
						 OpenGL::BufferOptionMapWrite;
			
			s_Data.QuadVertexBuffer  = OpenGL::Buffer(info);

			OpenGL::VertexInputMap map = 
			{
				{"Transform",  {OpenGL::VertexType::Matrix4x4, false}}
			};

			s_Data.QuadVertexArray.AttachVertexInput(&s_Data.QuadVertexBuffer, &s_Data.ShaderProgram, map);
			s_Data.QuadVertexPointer = (Quad*)s_Data.QuadVertexBuffer.MapRange(info.Masks);
		}
	}

	void BatchRenderer::_CreatePrograms()
	{
		{
			OpenGL::ShaderMap map
			{ {OpenGL::ShaderType::FragmentShader, s_Data.ShaderDirectory / "Batch.frag"},
			  {OpenGL::ShaderType::VertexShader,   s_Data.ShaderDirectory / "Batch.vert"} };

			s_Data.ShaderProgram = OpenGL::Program(map);
		}
	}

	void BatchRenderer::_CreateTextures()
	{
		{
			OpenGL::Texture2DInfo info{};
			info.Type    = OpenGL::Texture2DType::Texture;
			info.Format  = OpenGL::Texture2DFormat::RGBA32F;
			info.Width   = s_Data.ViewportWidth;
			info.Height  = s_Data.ViewportHeight;

			info.MipMapLevels = 1;
			
			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToEdge;

			s_Data.FinalImage.reset();
			s_Data.FinalImage = CreateRef<OpenGL::Texture2D>(info);
		}

	}

	void BatchRenderer::_CreateFramebuffers()
	{
		{
			OpenGL::FramebufferInfo info{};
			info.ColorAttachments.push_back(s_Data.FinalImage.get());
			s_Data.FinalImageFramebuffer = OpenGL::Framebuffer(info);
		}
	}

}