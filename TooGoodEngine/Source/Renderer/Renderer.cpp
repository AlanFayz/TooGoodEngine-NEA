#include "Renderer.h"

#include "API/OpenGL/Command.h"
#include "Utils/Statistics.h"

namespace TooGoodEngine {

	Renderer::Renderer(const RenderSettings& settings)
		: m_Settings(settings), m_Data()
	{
		if (settings.RuntimeShaderDirectory.empty()) //if not set then we are in the editor
		{
			m_Data.ShaderDirectory = __FILE__;
			m_Data.ShaderDirectory = m_Data.ShaderDirectory.parent_path() / "Shaders";
		}
		else
		{
			m_Data.ShaderDirectory = settings.RuntimeShaderDirectory;
		}

		ApplySettings();
		_CreatePrograms();
		_CreateBuffers();
		_CreateDefaultMaterialsAndMeshes();
		_CreateTextures();
		_CreateFramebuffers();
	}

	Renderer::~Renderer()
	{
	}

	MaterialID Renderer::CreateMaterial(const MaterialInfo& material)
	{
		return m_Data.MaterialStorage.Create(material);
	}

	MaterialID Renderer::CreateMaterial()
	{
		return m_Data.MaterialStorage.Create();
	}

	GeometryID Renderer::AddGeometry(const Geometry& data)
	{
		GeometryInstanceBufferInfo info{};
		info.Program = &m_Data.GeometryShaderProgram;
		
		info.Indices = data.Indices.data();
		info.IndexDataSize = data.Indices.size();

		info.VertexData = data.Vertices.data();
		info.VertexDataSize = data.Vertices.size();

		info.DefaultMaterialIndex = CreateMaterial(data.Material); 

		return m_Data.GeometryStorage.Create(info);
	}

	void Renderer::RemoveGeometry(GeometryID id)
	{
		m_Data.GeometryStorage.Remove(id);
	}

	ModelInfo Renderer::AddModel(const Ref<Model>& model)
	{
		auto& data = model->GetData();

		TGE_VERIFY(data.size() > 0, "cannot have a size of 0");

		if (m_Data.ModelInstanceCache.contains(model->GetAssetID()))
			return m_Data.ModelInstanceCache[model->GetAssetID()];

		ModelInfo info{};
		
		info.ID = AddGeometry(data[0]);
		info.Size = 1;

		for (size_t i = 1; i < data.size(); i++)
		{
			AddGeometry(data[i]);
			info.Size++;
		}

		m_Data.ModelInstanceCache[model->GetAssetID()] = info;
		return info;
	}

	void Renderer::RemoveModel(const Ref<Model>& model)
	{
		if (m_Data.ModelInstanceCache.contains(model->GetAssetID()))
			RemoveModel(m_Data.ModelInstanceCache[model->GetAssetID()]);
	}

	void Renderer::RemoveModel(const ModelInfo& info)
	{
		for (size_t i = 0; i < info.Size; i++)
		{
			RemoveGeometry(info.ID + i);
		}
	}

	void Renderer::ChangeSettings(const RenderSettings& settings)
	{
		m_Settings = settings;
		ApplySettings();
	}

	void Renderer::OnWindowResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_Settings.ViewportWidth = newWidth;
		m_Settings.ViewportHeight = newHeight;

		_CreateTextures();
		_CreateFramebuffers();
	}

	void Renderer::ModifyMaterial(MaterialID id, const MaterialInfo& material)
	{
		m_Data.MaterialStorage.Modify(id, material);
	}

	void Renderer::RemoveMaterial(MaterialID id)
	{
		m_Data.MaterialStorage.Remove(id);
	}

	MaterialInfo& Renderer::GetMaterialInfo(MaterialID id)
	{
		return m_Data.MaterialStorage.GetInfo(id);
	}

	void Renderer::Begin(Camera* camera)
	{
		TGE_VERIFY(!m_Data.IsDrawing, "already in a rendering pass");
		m_Data.CurrentCamera = camera;
		m_Data.IsDrawing = true;
	}

	void Renderer::Submit(GeometryID id, const glm::mat4& transform, uint32_t materialIndex)
	{
		uint32_t sparseIndex = materialIndex == 0 ? (uint32_t)m_Data.GeometryStorage.Get(id).GetDefaultMaterialIndex() : materialIndex;

		uint32_t index = (uint32_t)m_Data.MaterialStorage.GetIndex((size_t)sparseIndex);

		Instance info{};
		info.MaterialIndex = index;
		info.Transform = transform;

		m_Data.GeometryStorage.PushInstance(id, info);
	}

	void Renderer::SubmitModel(const ModelInfo& info, const glm::mat4& transform)
	{
		for (size_t i = 0; i < info.Size; i++)
		{
			if (info.CustomMaterials.size() > i)
				Submit(info.ID + i, transform, (uint32_t)info.CustomMaterials[i]);
			else
				Submit(info.ID + i, transform);
		}
	}

	void Renderer::SubmitPointLight(const glm::vec3& position, const glm::vec4& color, float radius, float intensity)
	{
		PointLight light{};
		light.ColorAndRadius = glm::vec4(color.r, color.g, color.b, radius);
		light.PositionAndIntensity = glm::vec4(position, intensity);

		auto& currentBuffer = m_Data.PointLights.Buffers[m_Data.PointLights.BufferIndex];

		if (m_Data.PointLights.Size * sizeof(PointLight) >= currentBuffer.GetCapacity())
		{
			currentBuffer.Unmap();
			currentBuffer.Resize(m_Data.PointLights.Size * sizeof(PointLight) * 2);
			m_Data.PointLights.MappedData[m_Data.PointLights.BufferIndex] = (PointLight*)currentBuffer.MapRange(m_Data.PointLights.MapFlags);
		}

		m_Data.PointLights.MappedData[m_Data.PointLights.BufferIndex][m_Data.PointLights.Size++] = light;
	}

	void Renderer::SubmitDirectionaLight(const glm::vec3& direction, const glm::vec4& color, float intensity)
	{
		DirectionalLight light{};
		light.Color = color;
		light.DirectionAndIntensity = glm::vec4(direction, intensity);

		auto& currentBuffer = m_Data.DirectionalLights.Buffers[m_Data.DirectionalLights.BufferIndex];

		if (m_Data.DirectionalLights.Size * sizeof(DirectionalLight) >= currentBuffer.GetCapacity())
		{
			currentBuffer.Unmap();
			currentBuffer.Resize(m_Data.PointLights.Size * sizeof(DirectionalLight) * 2);
			m_Data.DirectionalLights.MappedData[m_Data.DirectionalLights.BufferIndex] = (DirectionalLight*)currentBuffer.MapRange(m_Data.DirectionalLights.MapFlags);
		}

		m_Data.DirectionalLights.MappedData[m_Data.DirectionalLights.BufferIndex][m_Data.DirectionalLights.Size++] = light;
	}

	void Renderer::End()
	{
		TGE_VERIFY(m_Data.IsDrawing, "haven't drawn anything");
		TGE_PROFILE_SCOPE(RendererSubmit);
		m_Data.IsDrawing = false;

		m_Data.FinalImageFramebuffer.Bind();

		OpenGL::Command::SetViewport(m_Settings.ViewportWidth, m_Settings.ViewportHeight);
		OpenGL::Command::ClearColor(m_Settings.ClearColor);
		OpenGL::Command::ClearDepth();
		
		_RenderGeometry();
		_RenderSkyBox();
		_RenderBloom();
		_RenderFinalPass();

		m_Data.PointLights.Size = 0;
		m_Data.DirectionalLights.Size = 0;

		m_Data.PointLights.BufferIndex = (m_Data.PointLights.BufferIndex + 1) % 3;
		m_Data.DirectionalLights.BufferIndex = (m_Data.DirectionalLights.BufferIndex + 1) % 3;

		m_Data.FinalImageFramebuffer.Unbind();
	}

	void Renderer::RenderImageToScreen(uint32_t width, uint32_t height)
	{
		OpenGL::Command::SetViewport(width, height);

		m_Data.DisplayProgram.Use();
		glDisable(GL_DEPTH_TEST);

		m_Data.FinalImageTexture->Bind(0);
		m_Data.DisplayProgram.SetUniform("u_Image", 0);

		//vertex array not used so just pass a dummy to make opengl happy.
		OpenGL::Command::DrawArrays(&m_Data.DisplayProgram, &m_Data.Dummy, OpenGL::DrawMode::Triangle, 0, 3);

		if (m_Settings.DepthTesting != DepthTestOption::None)
			ApplySettings();
	}

	void Renderer::_RenderGeometry()
	{
		m_Data.GeometryShaderProgram.Use();

		GeometryUniformBuffer data{};
		data.PointLightSize		  = (int)m_Data.PointLights.Size;
		data.DirectionalLightSize = (int)m_Data.DirectionalLights.Size;
		data.CameraPosition		  = m_Data.CurrentCamera->GetCameraPosition();
		data.NumberOfMipMaps      = (float)g_NumberOfMipMaps;
		data.ViewProjection       = m_Data.CurrentCamera->GetProjection() * m_Data.CurrentCamera->GetView();
		data.HasCubeMap = 0;

		if (m_Settings.CurrentEnvironmentMap)
		{
			data.HasCubeMap = 1;
			m_Data.GeometryShaderProgram.SetUniform("u_CubeMap", 0);
			m_Settings.CurrentEnvironmentMap->GetTexture().Bind(0);
		}

		m_Data.GeometryPassBuffer.SetData(sizeof(GeometryUniformBuffer), &data);
		

		for (auto& [instanceBuffer,  index]:m_Data.GeometryStorage)
		{
			if (instanceBuffer.GetInstanceCount() == 0)
				continue;

			instanceBuffer.BeginBatch(0);
			m_Data.MaterialStorage.SubmitBuffer(1);
			m_Data.PointLights.Buffers[m_Data.PointLights.BufferIndex].BindBase(2, OpenGL::BufferTypeShaderStorage);
			m_Data.DirectionalLights.Buffers[m_Data.DirectionalLights.BufferIndex].BindBase(3, OpenGL::BufferTypeShaderStorage);
			m_Data.GeometryPassBuffer.BindBase(4, OpenGL::BufferTypeUniform);

			OpenGL::Command::DrawElementsInstanced(
				&m_Data.GeometryShaderProgram,
				instanceBuffer.GetVertexArrayPointer(),
				OpenGL::DrawMode::Triangle, instanceBuffer.GetIndexCount(),
				instanceBuffer.GetInstanceCount());

			instanceBuffer.EndBatch();
		}
	}

	void Renderer::_RenderSkyBox()
	{
		if (m_Settings.CurrentEnvironmentMap)
		{
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_LEQUAL);
			glDisable(GL_CULL_FACE);

			m_Data.SkyBoxShaderProgram.Use();
			m_Settings.CurrentEnvironmentMap->GetTexture().Bind(0);
												
			glm::mat4 viewProjection = m_Data.CurrentCamera->GetProjection() *
									   glm::mat4(glm::mat3(m_Data.CurrentCamera->GetView())); //(removes translation as translation is in the last column)

			m_Data.SkyBoxShaderProgram.SetUniform("u_ViewProjection", viewProjection);
			m_Data.SkyBoxShaderProgram.SetUniform("u_EnvironmentMap", 0);
			m_Data.SkyBoxShaderProgram.SetUniform("u_LevelOfDetail", m_Settings.LevelOfDetail);

			Submit(m_Data.CubeGeometryIndex, glm::identity<glm::mat4>());

			auto& cubeBuffer = m_Data.GeometryStorage.Get(m_Data.CubeGeometryIndex);
			cubeBuffer.BeginBatch(0);

			OpenGL::Command::DrawElementsInstanced(
				&m_Data.SkyBoxShaderProgram,
				cubeBuffer.GetVertexArrayPointer(),
				OpenGL::DrawMode::Triangle, cubeBuffer.GetIndexCount(),
				cubeBuffer.GetInstanceCount());

			cubeBuffer.EndBatch();

			glDepthMask(GL_TRUE);
			ApplySettings();
		}
	}

	void Renderer::_RenderBloom()
	{
		if (!m_Settings.Bloom)
			return;

		{
			OpenGL::Framebuffer::BlitInfo info{};
			info.Source = &m_Data.FinalImageFramebuffer;
			info.Destination = &m_Data.BloomFramebuffer;
			info.SourceTexture = m_Data.FinalImageTexture.get();
			info.DestinationTexture = m_Data.BloomTexture.get();
			info.SourceWidth = m_Settings.ViewportWidth;	
			info.SourceHeight = m_Settings.ViewportHeight;
			info.DestinationWidth = m_Settings.ViewportWidth;
			info.DestinationHeight = m_Settings.ViewportHeight;
			info.SourceIndex = 0;
			info.DestinationIndex = 0;
			info.SourceLayer = 0;
			info.DestinationLayer = 0;
			info.SourceMipLevel = 0;
			info.DestinationMipLevel = 0;

			OpenGL::Framebuffer::BlitColorAttachment(info);
		}


		m_Data.BloomPass.Use();

		uint32_t width  = m_Settings.ViewportWidth;
		uint32_t height = m_Settings.ViewportHeight;

		static constexpr int DownSample = 0;
		static constexpr int UpSample   = 1;

		//
		// ---- down sampling ----
		//

		for (uint32_t i = 1; i < RenderData::BloomMipLevelCount; i++)
		{
			uint32_t source = i - 1;
			uint32_t destination = i;

			uint32_t sourceWidth = width >> source;
			uint32_t sourceHeight = height >> source;

			uint32_t destinationWidth = width >> destination;
			uint32_t destinationHeight = height >> destination;

			float groupX = std::ceil((float)destinationWidth / 8.0f);
			float groupY = std::ceil((float)destinationHeight / 8.0f);

			BloomUniformBuffer data{};
			data.SampleOption = DownSample;
			data.SourceMip = (int)source;
			data.Threshold = m_Settings.Threshold;
			data.Intensity = m_Settings.Intensity;
			data.FilterRadius = m_Settings.FilterRadius;
			
			m_Data.BloomPassBuffer.SetData(sizeof(BloomUniformBuffer), &data);
			m_Data.BloomPassBuffer.BindBase(0, OpenGL::BufferTypeUniform);
			
			m_Data.BloomPass.SetUniform("u_Source", 0);
			m_Data.BloomPass.SetUniform("u_Destination", 1);

			m_Data.BloomTexture->Bind(0);
			m_Data.BloomTexture->BindImage(1, destination, 0, false);

			glDispatchCompute((GLuint)groupX, (GLuint)groupY, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}

		//
		// ---- up sampling ----
		//

		m_Data.BloomPass.Use();

		for (uint32_t i = RenderData::BloomMipLevelCount - 1; i > 0; i--)
		{
			uint32_t source = i;
			uint32_t destination = i - 1;

			uint32_t sourceWidth = width >> source;
			uint32_t sourceHeight = height >> source;

			uint32_t destinationWidth = width >> destination;
			uint32_t destinationHeight = height >> destination;

			float groupX = std::ceil((float)destinationWidth / 8.0f);
			float groupY = std::ceil((float)destinationHeight / 8.0f);

			
			BloomUniformBuffer data{};
			data.SampleOption = UpSample;
			data.SourceMip    = (int)source;
			data.Threshold = m_Settings.Threshold;
			data.Intensity = m_Settings.Intensity;
			data.FilterRadius = m_Settings.FilterRadius;


			m_Data.BloomPassBuffer.SetData(sizeof(BloomUniformBuffer), &data);
			m_Data.BloomPassBuffer.BindBase(0, OpenGL::BufferTypeUniform);

			m_Data.BloomPass.SetUniform("u_Source", 0);
			m_Data.BloomPass.SetUniform("u_Destination", 1);
			
			m_Data.BloomTexture->Bind(0);
			m_Data.BloomTexture->BindImage(1, destination, 0, false);

			glDispatchCompute((GLuint)groupX, (GLuint)groupY, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}


		{
			OpenGL::Framebuffer::BlitInfo info{};
			info.Source = &m_Data.BloomFramebuffer;
			info.Destination = &m_Data.FinalImageFramebuffer;
			info.SourceTexture = m_Data.BloomTexture.get();
			info.DestinationTexture = m_Data.FinalImageTexture.get();
			info.SourceWidth = m_Settings.ViewportWidth;
			info.SourceHeight = m_Settings.ViewportHeight;
			info.DestinationWidth = m_Settings.ViewportWidth;
			info.DestinationHeight = m_Settings.ViewportHeight;
			info.SourceIndex = 0;
			info.DestinationIndex = 0;
			info.SourceLayer = 0;
			info.DestinationLayer = 0;
			info.SourceMipLevel = 0;
			info.DestinationMipLevel = 0;

			OpenGL::Framebuffer::BlitColorAttachment(info);
		}

	}

	void Renderer::_RenderFinalPass()
	{
		m_Data.FinalPass.Use();
		m_Data.FinalImageTexture->BindImage(0, 0, 0, false);
		m_Data.FinalPass.SetUniform("u_Gradient", m_Settings.Gradient);

		glDispatchCompute((GLuint)std::ceil((float)m_Settings.ViewportWidth  / 8.0f), 
						  (GLuint)std::ceil((float)m_Settings.ViewportHeight / 8.0f), 
						  1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	void Renderer::ApplySettings() 
	{
		glEnable(GL_DEPTH_TEST);

		switch (m_Settings.DepthTesting)
		{
			case DepthTestOption::Greater:	   glDepthFunc(GL_GREATER); break;
			case DepthTestOption::Equal:	   glDepthFunc(GL_EQUAL);   break;
			case DepthTestOption::LessOrEqual: glDepthFunc(GL_LEQUAL);  break;
			case DepthTestOption::Less:        glDepthFunc(GL_LESS);    break;
			case DepthTestOption::None:
			default:
				glDisable(GL_DEPTH_TEST);
				break;
		}

		glEnable(GL_CULL_FACE);

		switch (m_Settings.Culling)
		{
			case CullingOption::FrontAndBack: glCullFace(GL_FRONT_AND_BACK); break;
			case CullingOption::Front:		  glCullFace(GL_FRONT);			 break;
			case CullingOption::Back:		  glCullFace(GL_BACK);			 break;
			case CullingOption::None:
			default:
				glDisable(GL_CULL_FACE);
				break;
		}

		switch (m_Settings.WindingOrder)
		{
			case WindingOrderOption::ClockWise: glFrontFace(GL_CW); break;
			case WindingOrderOption::CounterClockWise:
			case WindingOrderOption::None:
			default:
				glFrontFace(GL_CCW);
				break;
		}

		switch (m_Settings.FillingMode)
		{
			case FillMode::Line:	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  break;
			case FillMode::Point:	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
			case FillMode::Fill:
			case FillMode::None:
			default:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
		}

		GLenum sourceBlend = GetBlendFactor(m_Settings.Source);
		GLenum dstBlend = GetBlendFactor(m_Settings.Destination);

		if (sourceBlend == GL_INVALID_ENUM || dstBlend == GL_INVALID_ENUM)
			glDisable(GL_BLEND);
		else
		{
			glEnable(GL_BLEND);
			glBlendFunc(sourceBlend, dstBlend);
		}
		
	}

	void Renderer::_CreateBuffers()
	{
		//
		// ---- Point Lights ----
		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = 10 * sizeof(PointLight);
			bufferInfo.Data = nullptr;
			bufferInfo.Masks = OpenGL::BufferOptionMapCoherient |
							   OpenGL::BufferOptionMapPersistent |
							   OpenGL::BufferOptionMapWrite;

			m_Data.PointLights.MapFlags = OpenGL::BufferOptionMapCoherient |
				OpenGL::BufferOptionMapPersistent | OpenGL::BufferOptionMapWrite;
			
			for (size_t i = 0; i < 3; i++)
			{
				m_Data.PointLights.Buffers[i] = OpenGL::Buffer(bufferInfo);
				m_Data.PointLights.MappedData[i] = (PointLight*)m_Data.PointLights.Buffers[i].MapRange(m_Data.PointLights.MapFlags);
			}

			m_Data.PointLights.BufferIndex = 0;
			m_Data.PointLights.Size = 0;
		}

		//
		// ---- Directional Lights ----
		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = 10 * sizeof(DirectionalLight);
			bufferInfo.Data = nullptr;
			bufferInfo.Masks = OpenGL::BufferOptionMapCoherient |
							   OpenGL::BufferOptionMapPersistent |
							   OpenGL::BufferOptionMapWrite;

			m_Data.DirectionalLights.MapFlags = OpenGL::BufferOptionMapCoherient |
				OpenGL::BufferOptionMapPersistent | OpenGL::BufferOptionMapWrite;

			for (size_t i = 0; i < 3; i++)
			{
				m_Data.DirectionalLights.Buffers[i] = OpenGL::Buffer(bufferInfo);
				m_Data.DirectionalLights.MappedData[i] = (DirectionalLight*)m_Data.DirectionalLights.Buffers[i].MapRange(m_Data.DirectionalLights.MapFlags);
			}

			m_Data.DirectionalLights.BufferIndex = 0;
			m_Data.DirectionalLights.Size = 0;
		}

		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = sizeof(GeometryUniformBuffer);
			bufferInfo.Data = nullptr;
			bufferInfo.Masks = OpenGL::BufferOptionDynamicStorage;

			m_Data.GeometryPassBuffer = OpenGL::Buffer(bufferInfo);
		}

		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = sizeof(BloomUniformBuffer);
			bufferInfo.Data = nullptr;
			bufferInfo.Masks = OpenGL::BufferOptionDynamicStorage;

			m_Data.BloomPassBuffer = OpenGL::Buffer(bufferInfo);
		}
	}

	void Renderer::_CreateDefaultMaterialsAndMeshes()
	{
		{
			Geometry square;

			std::vector<Vertex> squareData = {
				{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
				{ glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(0.5f,  0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) }
			};

			std::vector<uint32_t> squareIndices = {
				2, 3, 1,
				2, 1, 0
			};

			square.Vertices = squareData;
			square.Indices = squareIndices;

			MaterialInfo info;

			info.Ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			info.Albedo = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			info.Metallic = 0.0f;

			info.Emission = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			info.EmissionFactor = 0.0f;

			info.Roughness = 1.0f;

			square.Material = info;
			m_Data.SquareGeometryIndex = AddGeometry(square);
		}
	
		{
			Geometry cube;

			cube.Vertices = {
				// Front face
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec2(0.0f, 0.0f) },

				// Back face
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f,  -1.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f,  -1.0f), glm::vec2(0.0f, 0.0f) },

				// Left face
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },

				// Right face
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },

				// Top face
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f) },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },

				// Bottom face
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f),glm::vec2(1.0f, 1.0f) },
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f),glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }
			};

			cube.Indices = {
				// Front face
				0, 2, 1,
				2, 3, 1,

				// Back face
				4, 6, 5,
				6, 7, 5,

				// Left face
				8, 10, 9,
				10, 11, 9,

				// Right face
				12, 14, 13,
				14, 15, 13,

				// Top face
				16, 18, 17,
				18, 19, 17,

				// Bottom face
				20, 22, 21,
				22, 23, 21
			};

			MaterialInfo info;

			info.Ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			info.Albedo = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			info.Metallic = 0.0f;

			info.Emission = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			info.EmissionFactor = 0.0f;

			info.Roughness = 1.0f;

			cube.Material = info;
			m_Data.SquareGeometryIndex = AddGeometry(cube);

			m_Data.CubeGeometryIndex = 1;
		}

	}

	void Renderer::_CreateTextures()
	{
		{
			OpenGL::Texture2DInfo info{};
			info.Type = OpenGL::Texture2DType::Texture;
			info.Format = OpenGL::Texture2DFormat::RGBA32F;
			info.Width = m_Settings.ViewportWidth;
			info.Height = m_Settings.ViewportHeight;
			
			info.MipMapLevels = 1;
			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::Linear;
			
			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToEdge;

			m_Data.FinalImageTexture.reset();
			m_Data.FinalImageTexture = CreateRef<OpenGL::Texture2D>(info);
		}

		{

			OpenGL::Texture2DInfo info{};
			info.Type = OpenGL::Texture2DType::Texture;
			info.Format = OpenGL::Texture2DFormat::RGBA32F;
			info.Width = m_Settings.ViewportWidth;
			info.Height = m_Settings.ViewportHeight;

			info.MipMapLevels = RenderData::BloomMipLevelCount;
			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::MipMapLinear;

			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToEdge;

			m_Data.BloomTexture.reset();
			m_Data.BloomTexture = CreateRef<OpenGL::Texture2D>(info);
		} 

		{
			OpenGL::Texture2DInfo info{};
			info.Type = OpenGL::Texture2DType::DepthTexture;
			info.Format = OpenGL::Texture2DFormat::DEPTH_32F;
			info.Width = m_Settings.ViewportWidth;
			info.Height = m_Settings.ViewportHeight;

			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToBorder;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToBorder;

			m_Data.DepthTexture.reset();
			m_Data.DepthTexture = CreateRef<OpenGL::Texture2D>(info);
		}
	}

	void Renderer::_CreateFramebuffers()
	{
		{
			OpenGL::FramebufferInfo info{};
			info.ColorAttachments.push_back(m_Data.FinalImageTexture.get());
			info.DepthAttachment = m_Data.DepthTexture.get();

			m_Data.FinalImageFramebuffer.~Framebuffer();
			m_Data.FinalImageFramebuffer = OpenGL::Framebuffer(info);
		}

		{
			OpenGL::FramebufferInfo info{};
			info.ColorAttachments.push_back(m_Data.BloomTexture.get());

			m_Data.BloomFramebuffer.~Framebuffer();
			m_Data.BloomFramebuffer = OpenGL::Framebuffer(info);
		}
	}

	void Renderer::_CreatePrograms()
	{
		{
			OpenGL::ShaderMap map
			{ {OpenGL::ShaderType::FragmentShader, m_Data.ShaderDirectory / "Color.frag"},
			  {OpenGL::ShaderType::VertexShader,   m_Data.ShaderDirectory / "Color.vert"} };

			m_Data.GeometryShaderProgram = OpenGL::Program(map);
		}

		{
			OpenGL::ShaderMap map
			{ {OpenGL::ShaderType::FragmentShader, m_Data.ShaderDirectory / "SkyBox.frag"},
			  {OpenGL::ShaderType::VertexShader, m_Data.ShaderDirectory / "SkyBox.vert"} };

			m_Data.SkyBoxShaderProgram = OpenGL::Program(map);
		}

		{
			OpenGL::ShaderMap map
			{ {OpenGL::ShaderType::ComputeShader, m_Data.ShaderDirectory / "Bloom.comp"} };
			
			m_Data.BloomPass = OpenGL::Program(map);
		}

		{
			OpenGL::ShaderMap map
			{ {OpenGL::ShaderType::ComputeShader, m_Data.ShaderDirectory / "FinalPass.comp"} };

			m_Data.FinalPass = OpenGL::Program(map);
		}

		{
			OpenGL::ShaderMap shaderMap
			{ {OpenGL::ShaderType::FragmentShader, m_Data.ShaderDirectory / "Display.frag"},
			  {OpenGL::ShaderType::VertexShader,   m_Data.ShaderDirectory / "Display.vert"} };

			m_Data.DisplayProgram = OpenGL::Program(shaderMap);
		}

	}

	GLenum Renderer::GetBlendFactor(BlendingFactor factor)
	{
		switch (m_Settings.Source)
		{
			case BlendingFactor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
			case BlendingFactor::DstAlpha:		   return GL_DST_ALPHA;
			case BlendingFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
			case BlendingFactor::SrcAlpha:		   return GL_SRC_ALPHA;
			case BlendingFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
			case BlendingFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
			case BlendingFactor::DstColor:		   return GL_DST_COLOR;
			case BlendingFactor::SrcColor:		   return GL_SRC_COLOR;
			case BlendingFactor::One:			   return GL_ONE;
			case BlendingFactor::Zero:			   return GL_ZERO;
			case BlendingFactor::None:
			default: return GL_INVALID_ENUM;
		}
	}

}