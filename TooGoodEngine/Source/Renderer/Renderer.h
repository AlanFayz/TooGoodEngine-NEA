#pragma once

#include "Core/Base.h"
#include "GeometryInstanceBuffer.h"
#include "Math/Camera.h"

namespace TooGoodEngine {

	struct RenderSettings
	{
		uint32_t ViewportWidth, ViewportHeight;
		glm::vec4 ClearColor;
		std::filesystem::path RuntimeShaderDirectory = ""; //for the runtime
	};

	struct RenderData
	{
		std::filesystem::path ShaderDirectory;
		bool IsDrawing = false;

		OpenGL::Program ColorShaderProgram;

		//no need for triple buffering as materials are unlikely to change per frame
		//so we do not need to restart every frame.
		//a user can edit a material simply by its ID (index)
		OpenGL::Buffer MaterialBuffer;
		Material* MaterialMappedData;
		size_t CurrentMaterialBufferIndex;

		std::vector<GeometryInstanceBuffer> GeometryList;

		Camera* CurrentCamera;

		size_t SquareGeometryIndex;
	};

	using GeometryID = size_t;
	using MaterialID = size_t;

	inline constexpr size_t g_NullID = std::numeric_limits<size_t>::max();

	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(const RenderSettings& settings);
		~Renderer();

		MaterialID AddMaterial(const Material& material);
		GeometryID AddGeometry(const Geometry& data);

		void OnWindowResize(uint32_t newWidth, uint32_t newHeight);
		void ChangeMaterialData(MaterialID id, const Material& material);

		void Begin(Camera* camera);
		void Draw(GeometryID id, const glm::mat4& transform, uint32_t materialIndex = 0);
		void DrawQuad(const glm::mat4& transform, uint32_t materialIndex = 0);
		void End();

	private:
		void RenderInstances();

	private:
		RenderSettings m_Settings;
		RenderData m_Data;
	};

}