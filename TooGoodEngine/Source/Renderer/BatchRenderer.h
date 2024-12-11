#pragma once

#include "Math/Camera.h"

#include "API/OpenGL/VertexArray.h"
#include "API/OpenGL/Framebuffer.h"



#include <filesystem>


namespace TooGoodEngine {

	struct QuadVertex
	{
		glm::mat4 Transform;
	};

	struct Quad
	{
		QuadVertex Vertices[4];
	};

	struct BatchRendererData
	{
		std::filesystem::path ShaderDirectory;

		Camera* SceneCamera = nullptr;

		OpenGL::VertexArray QuadVertexArray;
		OpenGL::Buffer QuadVertexBuffer;
		OpenGL::Buffer QuadIndexBuffer;

		Quad* QuadVertexPointer = nullptr;
		uint32_t QuadVertexOffset = 0;

		uint32_t QuadIndexOffset = 0;
		uint32_t QuadIndexCount = 6 * 100; //default to 100 quads

		OpenGL::Program ShaderProgram;

		OpenGL::Framebuffer FinalImageFramebuffer;
		Ref<OpenGL::Texture2D> FinalImage;

		uint32_t ViewportWidth  = 1280;
		uint32_t ViewportHeight = 720;
	};

	class BatchRenderer
	{
	public:
		 BatchRenderer() = delete;
		~BatchRenderer() = delete;

		static void Init();

		static void Begin(Camera* camera);
		static void End();
		
		static void DrawQuad(const glm::mat4& transform);


		static void OnWindowResize(uint32_t newWidth, uint32_t newHeight);

	private:
		static void _CreateBuffers();
		static void _CreatePrograms();
		static void _CreateTextures();
		static void _CreateFramebuffers();
	};

}