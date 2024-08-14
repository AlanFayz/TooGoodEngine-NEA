#pragma once

#include "Core/Base.h"
#include "GeometryInstanceBuffer.h"
#include "Math/Camera.h"
#include "Assets/Model.h"
#include "API/OpenGL/Framebuffer.h"
#include "Assets/EnvironmentMap.h"

namespace TooGoodEngine {

	//TODO: more to add (such as blending operations, polygone mode etc...)
	enum class DepthTestOption
	{
		None = 0, Less, LessOrEqual, Equal, Greater, Count
	};

	enum class CullingOption
	{
		None = 0, Back, Front, FrontAndBack, Count
	};

	//order to process triangle vertices
	enum class WindingOrderOption
	{
		None = 0, ClockWise, CounterClockWise, Count
	};

	enum class BlendingFactor
	{
		None = 0, Zero, One, SrcColor, DstColor, OneMinusSrcColor, OneMinusDstColor, SrcAlpha,
		OneMinusSrcAlpha, DstAlpha, OneMinusDstAlpha, Count
	};

	enum class FillMode
	{
		None = 0, Point, Line, Fill, Count
	};

	struct RenderSettings
	{
		uint32_t ViewportWidth, ViewportHeight;
		glm::vec4 ClearColor;
		std::filesystem::path RuntimeShaderDirectory = ""; 
		
		DepthTestOption DepthTesting    = DepthTestOption::Less;
		CullingOption Culling           = CullingOption::None;
		WindingOrderOption WindingOrder = WindingOrderOption::CounterClockWise;
		
		//Note, if either is none then there will be no blending
		BlendingFactor Source      = BlendingFactor::None;
		BlendingFactor Destination = BlendingFactor::None;
		FillMode FillingMode       = FillMode::Fill;

		Ref<EnvironmentMap> CurrentEnvironmentMap = nullptr;

		float Gradient = 1.0f;
		float LevelOfDetail = 2.0f;
	};

	struct RenderData
	{
		std::filesystem::path ShaderDirectory;
		bool IsDrawing = false;

		OpenGL::Program GeometryShaderProgram;
		OpenGL::Program SkyBoxShaderProgram;
		OpenGL::Program FinalPass;

		std::vector<GeometryInstanceBuffer> GeometryList;

		//no need for triple buffering as materials are unlikely to change per frame
		//so we do not need to restart every frame.
		//a user can edit a material simply by its ID 
		struct MaterialBuffer
		{
			OpenGL::Buffer Buffer;
			Material* MappedData;
			size_t Size;
			uint32_t MapFlags;
		};

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
		GeometryID SquareGeometryIndex;
		GeometryID CubeGeometryIndex;

		OpenGL::Framebuffer FinalImageFramebuffer;
		Ref<OpenGL::Texture2D> FinalImageTexture;
		Ref<OpenGL::Texture2D> DepthTexture;

	};

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

		inline const Ref<OpenGL::Texture2D>& GetImage() const { return m_Data.FinalImageTexture; }
		inline const RenderSettings& GetSettings() const { return m_Settings; }

	private:
		void _RenderGeometry();
		void _RenderSkyBox();
		void _RenderFinalPass();

		void _ApplySettings();

		void _CreateBuffers();
		void _CreateDefaultMaterialsAndMeshes();

		void _CreateTextures();
		void _CreateFramebuffers();

		void _CreatePrograms();
		
		GLenum GetBlendFactor(BlendingFactor factor);
	private:
		RenderSettings m_Settings;
		RenderData m_Data;
	};

}