#include "Scene.h"

namespace TooGoodEngine {

	Scene::Scene()
	{
		RenderSettings settings{};
		settings.ViewportWidth  = 600;
		settings.ViewportHeight = 600;
		settings.ClearColor     = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_SceneRenderer = CreateRef<Renderer>(settings);

		PerspectiveCameraData cameraData{};
		cameraData.AspectRatio = 600.0f / 600.0f;

		m_SceneCamera.SetData(cameraData);
	}

	void Scene::Update(double delta)
	{
		// ---- Run Scripts ----
		{
			auto scriptComponents = m_Registry.View<ScriptComponent>();

			for (auto& script : scriptComponents)
				script.OnUpdate(delta);
		}

		// ---- Render Meshes ----
		{
			m_SceneRenderer->Begin(&m_SceneCamera);

			m_Registry.ForEach<MeshComponent>(
				[this](auto& component, const auto entityID) 
				{
					if (!m_Registry.HasComponent<TransformComponent>(entityID))
						return;

					auto& transform = m_Registry.GetComponent<TransformComponent>(entityID);
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
	
			m_SceneCamera.OnWindowResize((float)windowResizeEvent->GetWidth(), (float)windowResizeEvent->GetHeight());
		}
	}
}