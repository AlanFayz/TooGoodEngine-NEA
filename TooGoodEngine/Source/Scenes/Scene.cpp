#include "Scene.h"

#include "Project/Project.h"
#include "Utils/Statistics.h"

namespace TooGoodEngine {

	Scene::Scene()
		: m_CameraController(nullptr), m_CameraController2D(nullptr)
	{
		//TODO: add a way to get the window dimensions (isn't a pressing issue as this is fixed by resizing the window)
		RenderSettings settings{};
		settings.ViewportWidth  = 1200;  
		settings.ViewportHeight = 800;
		settings.ClearColor     = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_SceneRenderer = CreateRef<Renderer>(settings);

		PerspectiveCameraData cameraData;
		cameraData.AspectRatio = 1200.0f / 800.0f;
		cameraData.Up = glm::vec3(0.0f, -1.0f, 0.0f);

		m_SceneCamera = CreateRef<PerspectiveCamera>(cameraData);

		m_CameraController.SetCamera(m_SceneCamera);

		OrthoGraphicCameraData orthoCameraData{};
		orthoCameraData.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		orthoCameraData.Front = glm::vec3(0.0f, 0.0f, 1.0f);
		orthoCameraData.Up = glm::vec3(0.0f, 1.0f, 0.0f);
		orthoCameraData.Left = -3.0f;
		orthoCameraData.Right = 3.0f;
		orthoCameraData.Bottom = -4.0f;
		orthoCameraData.Top = 4.0f;

		m_SceneCamera2D = CreateRef<OrthoGraphicCamera>(orthoCameraData);
		m_CameraController2D.SetCamera(m_SceneCamera2D);
	}

	void Scene::Play(double delta)
	{
		//TODO: this should look for a camera from the camera components.
		if (m_SceneView == SceneView::View3D)
			m_CameraController.Update(delta);
		else
			m_CameraController2D.Update(delta);

		// ---- call scripts ----
		{
			auto scripts = m_Registry.View<ScriptComponent>();

			if (m_FirstPlay)
			{
				for (const auto& script : scripts)
					script.OnCreate();

				m_FirstPlay = false;
			}

			for (const auto& script : scripts)
				script.OnUpdate(delta);
		}

		// ---- renderer begin ----
		if (m_SceneView == SceneView::View3D)
			m_SceneRenderer->Begin(m_SceneCamera.get());
		else
			m_SceneRenderer->Begin(m_SceneCamera2D.get());

		// ---- add point lights ----

		{
			auto pointLights = m_Registry.View<PointLightComponent>();

			for (const auto& pointLight : pointLights)
				m_SceneRenderer->PlacePointLight(pointLight.Position, pointLight.Color, pointLight.Radius, pointLight.Intensity);
		}

		// ---- add directional lights ----

		{
			auto directionalLights = m_Registry.View<DirectionalLightComponent>();

			for (const auto& light : directionalLights)
				m_SceneRenderer->AddDirectionaLight(light.Direction, light.Color, light.Intensity);
		}

		// ---- Render Meshes ----
		{
			m_Registry.ForEach<MeshComponent>(
				[this](auto& component, const auto entityID)
				{
					if (!m_Registry.HasComponent<TransformComponent>(entityID))
						return;

					TransformComponent& transform = m_Registry.GetComponent<TransformComponent>(entityID);

					if (!m_Registry.HasComponent<MaterialComponent>(entityID))
						m_SceneRenderer->Draw(component.ID, transform.GetTransform());
					else
					{
						MaterialComponent& material = m_Registry.GetComponent<MaterialComponent>(entityID);
						m_SceneRenderer->Draw(component.ID, transform.GetTransform(), (uint32_t)material.ID);
					}
				});

			m_Registry.ForEach<ModelComponent>(
				[this](ModelComponent& component, const auto entityID)
				{
					if (!m_Registry.HasComponent<TransformComponent>(entityID))
						return;

					TransformComponent& transform = m_Registry.GetComponent<TransformComponent>(entityID);

					//TODO: add a way to change materials dynamically from the editor. currently will have to 
					//edit it directly from native application.
					m_SceneRenderer->DrawModel(component.Info, transform.GetTransform());
				});

		}

		m_SceneRenderer->End();
	}

	void Scene::Update(double delta)
	{
		TGE_PROFILE_SCOPE(SceneUpdate);

		m_FirstPlay = true;


		if (m_SceneView == SceneView::View3D)
			m_CameraController.Update(delta);
		else
			m_CameraController2D.Update(delta);

		// ---- renderer begin ----
		if (m_SceneView == SceneView::View3D)
			m_SceneRenderer->Begin(m_SceneCamera.get());
		else
			m_SceneRenderer->Begin(m_SceneCamera2D.get());

		// ---- add point lights ----

		{
			auto pointLights = m_Registry.View<PointLightComponent>();

			for (auto& pointLight : pointLights)
				m_SceneRenderer->PlacePointLight(pointLight.Position, pointLight.Color, pointLight.Radius, pointLight.Intensity);
		}

		// ---- add directional lights ----

		{
			auto directionalLights = m_Registry.View<DirectionalLightComponent>();

			for (auto& light : directionalLights)
				m_SceneRenderer->AddDirectionaLight(light.Direction, light.Color, light.Intensity);
		}

		// ---- Render Meshes ----
		{
			TGE_PROFILE_SCOPE(RenderGeometry);

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
						MaterialComponent& material = m_Registry.GetComponent<MaterialComponent>(entityID);
						m_SceneRenderer->Draw(component.ID, transform.GetTransform(), (uint32_t)material.ID);
					}
				});

			m_Registry.ForEach<ModelComponent>(
				[this](ModelComponent& component, const auto entityID)
				{
					if (!m_Registry.HasComponent<TransformComponent>(entityID))
						return;

					TransformComponent& transform = m_Registry.GetComponent<TransformComponent>(entityID);

					//TODO: add a way to change materials dynamically from the editor. currently will have to 
					//edit it directly from native application.
					m_SceneRenderer->DrawModel(component.Info, transform.GetTransform()); 
				});
			
		}

		m_SceneRenderer->End();

	}

	void Scene::OnEvent(Event* event)
	{
		if (event->GetType() == EventType::ViewportResize)
		{
			ViewportResizeEvent* windowResizeEvent = (ViewportResizeEvent*)event;
			m_SceneRenderer->OnWindowResize(windowResizeEvent->GetWidth(), windowResizeEvent->GetHeight());
			m_SceneCamera->OnWindowResize((float)windowResizeEvent->GetWidth(), (float)windowResizeEvent->GetHeight());
			m_SceneCamera2D->OnWindowResize((float)windowResizeEvent->GetWidth(), (float)windowResizeEvent->GetHeight());
		}
	}
	void Scene::SetSceneView(SceneView view)
	{
		m_SceneView = view;
	}
	Entity Scene::Add(const std::string& name)
	{
		return m_Registry.Add(name);
	}
}