#include "Renderer.h"

#include "API/OpenGL/Command.h"

namespace TooGoodEngine {

	Renderer::Renderer(const RenderSettings& settings)
		: m_Settings(settings), m_Data()
	{
		if (settings.RuntimeShaderDirectory.empty())
		{
			m_Data.ShaderDirectory = __FILE__;
			m_Data.ShaderDirectory = m_Data.ShaderDirectory.parent_path() / "Shaders";
		}
		else
		{
			m_Data.ShaderDirectory = settings.RuntimeShaderDirectory;
		}


		// ---- init shaders ----
		{
			OpenGL::ShaderMap map
			{ {OpenGL::ShaderType::FragmentShader, m_Data.ShaderDirectory / "Color.frag"}, 
			  {OpenGL::ShaderType::VertexShader,   m_Data.ShaderDirectory / "Color.vert"} };

			m_Data.ColorShaderProgram = OpenGL::Program(map);
		}

		// ---- Add default meshes ----
		Geometry square;

		std::vector<Vertex> squareData = {
			{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) }, 
			{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) }, 
			{ glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) }, 
			{ glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) }  
		};

		std::vector<uint32_t> squareIndices = {
			0, 1, 2, 
			1, 3, 2  
		};

		square.Vertices = squareData;
		square.Indices = squareIndices;

		m_Data.SquareGeometryIndex = AddGeometry(square);
	}

	Renderer::~Renderer()
	{
	}

	GeometryID Renderer::AddGeometry(const Geometry& data)
	{
		GeometryInstanceBufferInfo info{};
		info.Program = &m_Data.ColorShaderProgram;
		
		info.Indices = data.Indices.data();
		info.IndexDataSize = data.Indices.size();

		info.VertexData = data.Vertices.data();
		info.VertexDataSize = data.Vertices.size();

		info.DefaultMaterialIndex = 0; //TODO: need to add material system

		m_Data.GeometryList.emplace_back(info);

		return m_Data.GeometryList.size() - 1;
	}

	void Renderer::OnWindowResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_Settings.ViewportWidth = newWidth;
		m_Settings.ViewportHeight = newHeight;
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

		Instance info{};
		info.MaterialIndex = materialIndex;
		info.Transform = transform;

		m_Data.GeometryList[id].Push(info);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, uint32_t materialIndex)
	{
		Instance instance{};
		instance.MaterialIndex = materialIndex;
		instance.Transform = transform;

		m_Data.GeometryList[m_Data.SquareGeometryIndex].Push(instance);
	}

	void Renderer::End()
	{
		TGE_VERIFY(m_Data.IsDrawing, "haven't drawn anything");
		m_Data.IsDrawing = false;

		OpenGL::Command::SetViewport(m_Settings.ViewportWidth, m_Settings.ViewportHeight);
		OpenGL::Command::ClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		
		RenderInstances();
	}

	void Renderer::RenderInstances()
	{
		PerFrameData data{};
		data.ViewProjection = m_Data.CurrentCamera->GetProjection() * m_Data.CurrentCamera->GetView();

		for (auto& instanceBuffer : m_Data.GeometryList)
		{
			m_Data.ColorShaderProgram.SetUniform("ViewProjection", data.ViewProjection);

			instanceBuffer.BeginBatch(0);

			OpenGL::Command::DrawElementsInstanced(
				&m_Data.ColorShaderProgram,
				instanceBuffer.GetVertexArrayPointer(),
				OpenGL::DrawMode::Triangle, instanceBuffer.GetIndexCount(),
				instanceBuffer.GetInstanceCount());

			instanceBuffer.EndBatch();
		}
	}

}