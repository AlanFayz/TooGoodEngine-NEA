#pragma once

#include "Core/Base.h"
#include "Math/Camera.h"
#include "Assets/Model.h"
#include "API/OpenGL/Framebuffer.h"
#include "Assets/EnvironmentMap.h"

#include "Storage/GeometryStorage.h"
#include "Storage/MaterialStorage.h"

namespace TooGoodEngine {

	enum class DepthTestOption
	{
		None = 0, Less, LessOrEqual, Equal, Greater, Count
	};

	enum class CullingOption
	{
		None = 0, Back, Front, FrontAndBack, Count
	};

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

		bool Bloom = true;
		float Threshold = 1.0f; 
		float Intensity = 1.0f;
		float FilterRadius = 0.005f;
	};

	struct RenderData
	{
		static const uint32_t BloomMipLevelCount = 8;

		std::unordered_map<uint64_t, ModelInfo> ModelInstanceCache;

		std::filesystem::path ShaderDirectory;
		bool IsDrawing = false;

		OpenGL::Program GeometryShaderProgram;
		OpenGL::Program SkyBoxShaderProgram;
		OpenGL::Program BloomPass;
		OpenGL::Program FinalPass;

		GeometryStorage GeometryStorage;
		MaterialStorage MaterialStorage;

		struct PointLightBuffer
		{
			OpenGL::Buffer Buffers[3];
			PointLight* MappedData[3]{};
			size_t BufferIndex = 0;
			size_t Size = 0;
			uint32_t MapFlags = 0;
		};

		struct DirectionalLightBuffer
		{
			OpenGL::Buffer Buffers[3];
			DirectionalLight* MappedData[3]{};
			size_t BufferIndex = 0;
			size_t Size = 0;
			uint32_t MapFlags = 0;
		};

		PointLightBuffer PointLights;
		DirectionalLightBuffer DirectionalLights;

		OpenGL::Buffer GeometryPassBuffer;
		OpenGL::Buffer BloomPassBuffer;

		Camera* CurrentCamera;
		GeometryID SquareGeometryIndex;
		GeometryID CubeGeometryIndex;

		OpenGL::Framebuffer FinalImageFramebuffer;
		OpenGL::Framebuffer BloomFramebuffer;

		Ref<OpenGL::Texture2D> FinalImageTexture;
		Ref<OpenGL::Texture2D> DepthTexture;
		Ref<OpenGL::Texture2D> BloomTexture;

		OpenGL::Program DisplayProgram;
		OpenGL::VertexArray Dummy;
	};

	inline constexpr size_t g_NullID = std::numeric_limits<size_t>::max();

	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(const RenderSettings& settings);
		~Renderer();

		GeometryID AddGeometry(const Geometry& data);
		void RemoveGeometry(GeometryID id);

		ModelInfo  AddModel(const Ref<Model>& model);
		void RemoveModel(const Ref<Model>& model); //should use this if possible
		void RemoveModel(const ModelInfo& info);

		MaterialID CreateMaterial(const MaterialInfo& material);
		MaterialID CreateMaterial();
		void ModifyMaterial(MaterialID id, const MaterialInfo& material);
		void RemoveMaterial(MaterialID id);
		MaterialInfo& GetMaterialInfo(MaterialID id);


		void ChangeSettings(const RenderSettings& settings);
		void OnWindowResize(uint32_t newWidth, uint32_t newHeight);
		void ApplySettings();

		void Begin(Camera* camera);
		void Draw(GeometryID id, const glm::mat4& transform, uint32_t materialIndex = 0);
		void DrawModel(const ModelInfo& info, const glm::mat4& transform); 

		void PlacePointLight(const glm::vec3& position, const glm::vec4& color, float radius, float intensity);
		void AddDirectionaLight(const glm::vec3& direction, const glm::vec4& color, float intensity);
		void End();

		void RenderImageToScreen(uint32_t width, uint32_t height);

		inline const Ref<OpenGL::Texture2D>& GetImage() const { return m_Data.FinalImageTexture; }
		inline const RenderSettings& GetSettings() const { return m_Settings; }


	private:
		void _RenderGeometry();
		void _RenderSkyBox();
		void _RenderBloom();
		void _RenderFinalPass();

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