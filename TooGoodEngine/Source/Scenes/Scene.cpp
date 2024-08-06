#include "Scene.h"

namespace TooGoodEngine {

	Scene::Scene()
		: m_CameraController(nullptr)
	{
		RenderSettings settings{};
		settings.ViewportWidth  = 1200; //TODO: add a way to get the window dimensions
		settings.ViewportHeight = 800;
		settings.ClearColor     = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_SceneRenderer = CreateRef<Renderer>(settings);

		PerspectiveCameraData cameraData{};
		cameraData.AspectRatio = 1200.0f / 800.0f;

		m_SceneCamera = CreateRef<PerspectiveCamera>(cameraData);

		m_CameraController.SetCamera(m_SceneCamera);
	}

	void Scene::Update(double delta)
	{
		m_CameraController.Update(delta);

		// ---- Run Scripts ----
		{
			auto scriptComponents = m_Registry.View<ScriptComponent>();

			for (auto& script : scriptComponents)
				script.OnUpdate(delta);
		}

		//TODO: physics goes here

		// ---- Render Meshes ----
		{
			m_SceneRenderer->Begin(m_SceneCamera.get());

			m_Registry.ForEach<MeshComponent>(
				[this](auto& component, const auto entityID) 
				{
					if (!m_Registry.HasComponent<TransformComponent>(entityID))
						return;

					TransformComponent& transform = m_Registry.GetComponent<TransformComponent>(entityID);
					//TODO: add materials

					m_SceneRenderer->Draw(component.ID, transform.GetTransform());
				});

			m_SceneRenderer->End();
		}
		
	}

	void Scene::OnEvent(Event* event)
	{
		if (event->GetType() == EventType::WindowResize)
		{
			WindowResizeEvent* windowResizeEvent = (WindowResizeEvent*)event;
			m_SceneRenderer->OnWindowResize(windowResizeEvent->GetWidth(), windowResizeEvent->GetHeight());
	
			m_SceneCamera->OnWindowResize((float)windowResizeEvent->GetWidth(), (float)windowResizeEvent->GetHeight());
		}
	}
}