#include "Scene.h"

#include "Project/Project.h"

namespace TooGoodEngine {

	Scene::Scene()
		: m_CameraController(nullptr)
	{
		//TODO: add a way to get the window dimensions (isn't a pressing issue as this is fixed by resizing the window)
		RenderSettings settings{};
		settings.ViewportWidth  = 1200;  
		settings.ViewportHeight = 800;
		settings.ClearColor     = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_SceneRenderer = CreateRef<Renderer>(settings);

		PerspectiveCameraData cameraData;
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

	
		// ---- Render Meshes ----
		{
			m_SceneRenderer->Begin(m_SceneCamera.get());

			m_Registry.ForEach<MeshComponent>(
				[this](auto& component, const auto entityID) 
				{
					if (!m_Registry.HasComponent<TransformComponent>(entityID))
						return;

					TransformComponent& transform = m_Registry.GetComponent<TransformComponent>(entityID);
					
					if(!m_Registry.HasComponent<MaterialComponent>(entityID))
						m_SceneRenderer->Draw(component.ID, transform.GetTransform());
					else
					{
						//these are added externally either by loading it from the project or from the editor.
						MaterialComponent& material = m_Registry.GetComponent<MaterialComponent>(entityID);
						m_SceneRenderer->Draw(component.ID, transform.GetTransform(), material.ID);
					}
				});
			
			m_SceneRenderer->AddDirectionaLight({ 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 2.0f);
			m_SceneRenderer->End();
		}
		
	}

	void Scene::OnEvent(Event* event)
	{
		if (event->GetType() == EventType::ViewportResize)
		{
			ViewportResizeEvent* windowResizeEvent = (ViewportResizeEvent*)event;
			m_SceneRenderer->OnWindowResize(windowResizeEvent->GetWidth(), windowResizeEvent->GetHeight());
			m_SceneCamera->OnWindowResize((float)windowResizeEvent->GetWidth(), (float)windowResizeEvent->GetHeight());
		}
	}
}