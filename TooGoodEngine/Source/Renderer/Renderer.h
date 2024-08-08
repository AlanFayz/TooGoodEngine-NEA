#pragma once

#include "Core/Base.h"
#include "GeometryInstanceBuffer.h"
#include "Math/Camera.h"
#include "Assets/Model.h"

namespace TooGoodEngine {

	enum class DepthTestOption
	{
		None = 0, Less, LessOrEqual, Equal
	};

	enum class CullingOption
	{
		None = 0, Back, Front, FrontAndBack
	};

	//order to process triangle vertices
	enum class WindingOrderOption
	{
		None = 0, ClockWise, CounterClockWise
	};

	struct RenderSettings
	{
		uint32_t ViewportWidth, ViewportHeight;
		glm::vec4 ClearColor;
		std::filesystem::path RuntimeShaderDirectory = ""; 
		
		DepthTestOption DepthTesting    = DepthTestOption::Less;
		CullingOption Culling           = CullingOption::Back;
		WindingOrderOption WindingOrder = WindingOrderOption::CounterClockWise;
		bool Blending = false;
	};

	struct RenderData
	{
		std::filesystem::path ShaderDirectory;
		bool IsDrawing = false;

		OpenGL::Program ColorShaderProgram;

		std::vector<GeometryInstanceBuffer> GeometryList;

		//no need for triple buffering as materials are unlikely to change per frame
		//so we do not need to restart every frame.
		//a user can edit a material simply by its ID (index)
		struct MaterialBuffer
		{
			OpenGL::Buffer Buffer;
			Material* MappedData;
			size_t Size;
			uint32_t MapFlags;
		};

		//do i want triple buffers for these?
		struct PointLightBuffer
		{
			OpenGL::Buffer Buffers[3];
			PointLight* MappedData[3];
			size_t BufferIndex;
			size_t Size;
			uint32_t MapFlags;
		};

		struct DirectionalLightBuffer
		{
			OpenGL::Buffer Buffers[3];
			DirectionalLight* MappedData[3];
			size_t BufferIndex;
			size_t Size;
			uint32_t MapFlags;
		};

		MaterialBuffer Materials;
		PointLightBuffer PointLights;
		DirectionalLightBuffer DirectionalLights;

		Camera* CurrentCamera;

		size_t SquareGeometryIndex;
	};

	using GeometryID = size_t;
	using MaterialID = size_t;

	inline constexpr size_t g_NullID = std::numeric_limits<size_t>::max();

	struct ModelInfo
	{
		GeometryID ID;
		size_t Size;
		std::vector<MaterialID> CustomMaterials; //can leave blank and will use default. 
	};

	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(const RenderSettings& settings);
		~Renderer();

		MaterialID AddMaterial(const Material& material);
		GeometryID AddGeometry(const Geometry& data);
		ModelInfo  AddModel(const Ref<Model>& model);

		void ChangeSettings(const RenderSettings& settings);
		void OnWindowResize(uint32_t newWidth, uint32_t newHeight);
		void ChangeMaterialData(MaterialID id, const Material& material);

		void Begin(Camera* camera);
		void Draw(GeometryID id, const glm::mat4& transform, uint32_t materialIndex = 0);
		void DrawModel(const ModelInfo& info, const glm::mat4& transform); 

		void PlacePointLight(const glm::vec3& position, const glm::vec4& color, float radius, float intensity);
		void AddDirectionaLight(const glm::vec3& direction, const glm::vec4& color, float intensity);
		void End();

	private:
		void _RenderInstances();
		void _ApplySettings();

		void _CreateBuffers();
		void _CreateDefaultMaterialsAndMeshes();

	private:
		RenderSettings m_Settings;
		RenderData m_Data;
	};

}