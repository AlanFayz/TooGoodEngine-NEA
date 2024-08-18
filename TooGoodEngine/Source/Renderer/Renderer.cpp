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

		_ApplySettings();
		_CreatePrograms();
		_CreateBuffers();
		_CreateDefaultMaterialsAndMeshes();
		_CreateTextures();
		_CreateFramebuffers();

	}

	Renderer::~Renderer()
	{
		for (size_t i = 0; i <= m_Data.Materials.Size; i++)
		{
			m_Data.Materials.MappedData[i].MakeHandlesNonResident();
			m_Data.Materials.MappedData[i].~Material();
		}
	}

	MaterialID Renderer::AddMaterial(const Material& material)
	{
		MaterialID id = m_Data.Materials.Size;

		if (m_Data.Materials.Size * sizeof(Material) >= m_Data.Materials.Buffer.GetCapacity())
		{
			m_Data.Materials.Buffer.Unmap();
			m_Data.Materials.Buffer.Resize(m_Data.Materials.Size * sizeof(Material) * 2);
			m_Data.Materials.MappedData = (Material*)m_Data.Materials.Buffer.MapRange(m_Data.Materials.MapFlags);

			size_t sizeToInit = m_Data.Materials.Buffer.GetCapacity() - (m_Data.Materials.Size * sizeof(Material));
			memset(m_Data.Materials.MappedData + m_Data.Materials.Size, 0, sizeToInit);
		}

		m_Data.Materials.MappedData[m_Data.Materials.Size++] = material;
		
		material.MakeHandlesResident();
		return id;
	}

	GeometryID Renderer::AddGeometry(const Geometry& data)
	{
		GeometryInstanceBufferInfo info{};
		info.Program = &m_Data.GeometryShaderProgram;
		
		info.Indices = data.Indices.data();
		info.IndexDataSize = data.Indices.size();

		info.VertexData = data.Vertices.data();
		info.VertexDataSize = data.Vertices.size();

		info.DefaultMaterialIndex = AddMaterial(CreateMaterial(data.Material)); 

		m_Data.GeometryList.emplace_back(info);

		return m_Data.GeometryList.size() - 1;
	}

	ModelInfo Renderer::AddModel(const Ref<Model>& model)
	{
		auto& data = model->GetData();

		TGE_VERIFY(data.size() > 0, "cannot have a size of 0");

		ModelInfo info{};
		
		info.ID = AddGeometry(data[0]);
		info.Size = 1;

		for (size_t i = 1; i < data.size(); i++)
		{
			AddGeometry(data[i]);
			info.Size++;
		}

		return info;
	}

	void Renderer::ChangeSettings(const RenderSettings& settings)
	{
		m_Settings = settings;
		_ApplySettings();
	}

	void Renderer::OnWindowResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_Settings.ViewportWidth = newWidth;
		m_Settings.ViewportHeight = newHeight;

		_CreateTextures();
		_CreateFramebuffers();
	}

	void Renderer::ChangeMaterialData(MaterialID id, const Material& material)
	{
		TGE_VERIFY(id < m_Data.Materials.Size, "not a valid material id");

		m_Data.Materials.MappedData[id].MakeHandlesNonResident(); 
		m_Data.Materials.MappedData[id] = material;
		m_Data.Materials.MappedData[id].MakeHandlesResident();

		m_Data.Materials.Buffer.FlushMapRange();
	}

	void Renderer::Begin(Camera* camera)
	{
		TGE_VERIFY(!m_Data.IsDrawing, "already in a rendering pass");
		m_Data.CurrentCamera = camera;
		m_Data.IsDrawing = true;
	}

	void Renderer::Draw(GeometryID id, const glm::mat4& transform, uint32_t materialIndex)
	{
		TGE_VERIFY(id < m_Data.GeometryList.size(), "index out of range");
		TGE_VERIFY(materialIndex < m_Data.Materials.Size, "index out of range");

		Instance info{};
		info.MaterialIndex = materialIndex == 0 ? (uint32_t)m_Data.GeometryList[id].GetDefaultMaterialIndex() : materialIndex;
		info.Transform = transform;

		m_Data.GeometryList[id].Push(info);
	}

	void Renderer::DrawModel(const ModelInfo& info, const glm::mat4& transform)
	{
		for (size_t i = 0; i < info.Size; i++)
		{
			if (info.CustomMaterials.size() > i)
				Draw(info.ID + i, transform, (uint32_t)info.CustomMaterials[i]);
			else
				Draw(info.ID + i, transform);
		}
	}

	void Renderer::PlacePointLight(const glm::vec3& position, const glm::vec4& color, float radius, float intensity)
	{
		PointLight light{};
		light.Color = color;
		light.Position = glm::vec4(position, 1.0f);
		light.Radius = radius;
		light.Intensity = intensity;

		auto& currentBuffer = m_Data.PointLights.Buffers[m_Data.PointLights.BufferIndex];

		if (m_Data.PointLights.Size * sizeof(PointLight) >= currentBuffer.GetCapacity())
		{
			currentBuffer.Unmap();
			currentBuffer.Resize(m_Data.PointLights.Size * sizeof(PointLight) * 2);
			m_Data.PointLights.MappedData[m_Data.PointLights.BufferIndex] = (PointLight*)currentBuffer.MapRange(m_Data.PointLights.MapFlags);
		}

		m_Data.PointLights.MappedData[m_Data.PointLights.BufferIndex][m_Data.PointLights.Size++] = light;
	}

	void Renderer::AddDirectionaLight(const glm::vec3& direction, const glm::vec4& color, float intensity)
	{
		DirectionalLight light{};
		light.Color = color;
		light.Direction = glm::vec4(direction, 1.0f);
		light.Intensity = intensity;

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
		_RenderFinalPass();

		m_Data.PointLights.Size = 0;
		m_Data.DirectionalLights.Size = 0;

		m_Data.PointLights.BufferIndex = (m_Data.PointLights.BufferIndex + 1) % 3;
		m_Data.DirectionalLights.BufferIndex = (m_Data.DirectionalLights.BufferIndex + 1) % 3;

		m_Data.FinalImageFramebuffer.Unbind();
	}

	void Renderer::_RenderGeometry()
	{
		m_Data.GeometryShaderProgram.Use();

		PerFrameData data{};
		data.ViewProjection = m_Data.CurrentCamera->GetProjection() * m_Data.CurrentCamera->GetView();

		m_Data.GeometryShaderProgram.SetUniform("u_ViewProjection", data.ViewProjection);
		m_Data.GeometryShaderProgram.SetUniform("u_PointLightSize", (int)m_Data.PointLights.Size);
		m_Data.GeometryShaderProgram.SetUniform("u_DirectionalLightSize", (int)m_Data.DirectionalLights.Size);
		m_Data.GeometryShaderProgram.SetUniform("u_CameraPosition", m_Data.CurrentCamera->GetCameraPosition());
		m_Data.GeometryShaderProgram.SetUniform("u_NumberOfMipMaps", (float)g_NumberOfMipMaps);
		
		if (m_Settings.CurrentEnvironmentMap)
		{
			m_Data.GeometryShaderProgram.SetUniform("u_HasCubeMap", 1);
			m_Data.GeometryShaderProgram.SetUniform("u_CubeMap", 0);
			m_Settings.CurrentEnvironmentMap->GetTexture().Bind(0);
		}
		else
		{
			m_Data.GeometryShaderProgram.SetUniform("u_HasCubeMap", 0);
		}
		

		for (auto& instanceBuffer : m_Data.GeometryList)
		{
			instanceBuffer.BeginBatch(0);
			m_Data.Materials.Buffer.BindBase(1, OpenGL::BufferTypeShaderStorage);
			m_Data.PointLights.Buffers[m_Data.PointLights.BufferIndex].BindBase(2, OpenGL::BufferTypeShaderStorage);
			m_Data.DirectionalLights.Buffers[m_Data.DirectionalLights.BufferIndex].BindBase(3, OpenGL::BufferTypeShaderStorage);


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

			Draw(m_Data.CubeGeometryIndex, glm::identity<glm::mat4>());

			m_Data.GeometryList[m_Data.CubeGeometryIndex].BeginBatch(0); 

			OpenGL::Command::DrawElementsInstanced(
				&m_Data.SkyBoxShaderProgram,
				m_Data.GeometryList[m_Data.CubeGeometryIndex].GetVertexArrayPointer(),
				OpenGL::DrawMode::Triangle, m_Data.GeometryList[m_Data.CubeGeometryIndex].GetIndexCount(),
				m_Data.GeometryList[m_Data.CubeGeometryIndex].GetInstanceCount());

			m_Data.GeometryList[m_Data.CubeGeometryIndex].EndBatch();

			glDepthMask(GL_TRUE);
			_ApplySettings();
		}
	}

	void Renderer::_RenderFinalPass()
	{
		m_Data.FinalPass.Use();
		m_Data.FinalImageTexture->BindImage(0, 0, 0, false);
		m_Data.FinalPass.SetUniform("u_FinalImage", 0);
		m_Data.FinalPass.SetUniform("u_Gradient", m_Settings.Gradient);

		glDispatchCompute((GLuint)std::ceil((float)m_Settings.ViewportWidth  / 8.0f), 
						  (GLuint)std::ceil((float)m_Settings.ViewportHeight / 8.0f), 
						  1);
		glMemoryBarrier(GL_ALL_SHADER_BITS);
	}

	void Renderer::_ApplySettings() 
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
		// ---- Materials ----
		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = 10 * sizeof(Material); 
			bufferInfo.Data = nullptr;
			bufferInfo.Masks = OpenGL::BufferOptionMapCoherient |
				OpenGL::BufferOptionMapPersistent |
				OpenGL::BufferOptionMapWrite;


			m_Data.Materials.MapFlags = OpenGL::BufferOptionMapCoherient | 
										OpenGL::BufferOptionMapPersistent | 
										OpenGL::BufferOptionMapWrite | 
										OpenGL::BufferOptionMapFlushExplicit;

			m_Data.Materials.Buffer = OpenGL::Buffer(bufferInfo);
			m_Data.Materials.MappedData = (Material*)m_Data.Materials.Buffer.MapRange(m_Data.Materials.MapFlags);
			m_Data.Materials.Size = 0;

			memset(m_Data.Materials.MappedData, 0, m_Data.Materials.Buffer.GetCapacity());
		}

		//
		// ---- Point Lights ----
		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = 10 * sizeof(PointLight);
			bufferInfo.Data = nullptr;
			bufferInfo.Masks = OpenGL::BufferOptionMapCoherient |
							   OpenGL::BufferOptionMapPersistent |
							   OpenGL::BufferOptionMapWrite;

			m_Data.PointLights.MapFlags = OpenGL::BufferOptionMapCoherient | OpenGL::BufferOptionMapPersistent | OpenGL::BufferOptionMapWrite;
			
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

			m_Data.DirectionalLights.MapFlags = OpenGL::BufferOptionMapCoherient | OpenGL::BufferOptionMapPersistent | OpenGL::BufferOptionMapWrite;

			for (size_t i = 0; i < 3; i++)
			{
				m_Data.DirectionalLights.Buffers[i] = OpenGL::Buffer(bufferInfo);
				m_Data.DirectionalLights.MappedData[i] = (DirectionalLight*)m_Data.DirectionalLights.Buffers[i].MapRange(m_Data.DirectionalLights.MapFlags);
			}

			m_Data.DirectionalLights.BufferIndex = 0;
			m_Data.DirectionalLights.Size = 0;
		}
	}

	void Renderer::_CreateDefaultMaterialsAndMeshes()
	{
		{
			Geometry square;

			std::vector<Vertex> squareData = {
				{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
				{ glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(0.5f,  0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) }
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
			info.AlbedoFactor = 1.0f;

			info.Metallic = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			info.MetallicFactor = 1.0f;

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
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec2(1.0f, 1.0f) },

				// Back face
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f,  -1.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f,  -1.0f), glm::vec2(1.0f, 1.0f) },

				// Left face
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },

				// Right face
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },

				// Top face
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },

				// Bottom face
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) }
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
			info.AlbedoFactor = 1.0f;

			info.Metallic = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			info.MetallicFactor = 1.0f;

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
			info.MipMapLevels = RenderData::BloomMipLevelCount;

			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;

			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToBorder;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToBorder;

			m_Data.FinalImageTexture.reset();
			m_Data.FinalImageTexture = CreateRef<OpenGL::Texture2D>(info);
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