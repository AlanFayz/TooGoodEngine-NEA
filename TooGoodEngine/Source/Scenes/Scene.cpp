#include "Scene.h"

#include "Project/Project.h"
#include "Utils/Statistics.h"

#include "Renderer/Common.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


namespace TooGoodEngine {

	Scene::Scene()
		: m_CameraController(nullptr), m_CameraController2D(nullptr)
	{
		RenderSettings settings{};
		settings.ViewportWidth  = 1200;  
		settings.ViewportHeight = 800;
		settings.ClearColor     = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_SceneRenderer = CreateRef<Renderer>(settings);

		PerspectiveCameraData cameraData;
		cameraData.AspectRatio = 1200.0f / 800.0f;
		cameraData.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		m_SceneCamera = CreateRef<PerspectiveCamera>(cameraData);

		m_CameraController.SetCamera(m_SceneCamera);

		OrthographicCameraData orthoCameraData{};
		orthoCameraData.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		orthoCameraData.Left = -3.0f;
		orthoCameraData.Right = 3.0f;
		orthoCameraData.Bottom = -4.0f;
		orthoCameraData.Top = 4.0f;

		m_SceneCamera2D = CreateRef<OrthographicCamera>(orthoCameraData);
		m_CameraController2D.SetCamera(m_SceneCamera2D);

		MaterialInfo info;
		info.EmissionFactor = 20.0f;

		info.Emission = { 0.0f, 1.0f, 0.0f, 1.0f };
		m_Green = m_SceneRenderer->CreateMaterial(info);

		info.Emission = { 0.0f, 0.0f, 1.0f, 1.0f };
		m_Blue = m_SceneRenderer->CreateMaterial(info);

		info.Emission = { 1.0f, 0.0f, 0.0f, 1.0f };
		m_Red = m_SceneRenderer->CreateMaterial(info);
	}

	void Scene::Play(double delta)
	{   
		Camera* sceneCamera = nullptr;

		m_SceneRenderer->Reset();

		{
			//first search perspective cameras

			auto perspectiveView = m_Registry.View<PerspectiveCameraComponent>();

			for (auto& [camera, entityID] : perspectiveView)
			{
				if (camera.InUse)
				{
					sceneCamera = camera.Camera.get();
					break;
				}
			}

			//then search Orthographic cameras if one is not found

			if (!sceneCamera)
			{
				auto OrthographicView = m_Registry.View<OrthographicCameraComponent>();

				for (auto& [camera, entityID] : OrthographicView)
				{
					if (camera.InUse)
					{
						sceneCamera = camera.Camera.get();
						break;
					}
				}
			}

		}

		if (!sceneCamera)
			sceneCamera = m_SceneCamera.get();


		{
			auto scripts = m_Registry.View<ScriptComponent>();

			//if its the time playing then call
			//script init code. OnCreate() should really
			//be called OnReady() here or OnPlay() TODO

			if (m_FirstPlay)
			{
				for (const auto& [script, entityID] : scripts)
				{
					if(script)
						script.OnCreate();
				}

				m_FirstPlay = false;
			}

			for (const auto& [script, entityID] : scripts)
			{
				if(script)
					script.OnUpdate(delta);
			}
		}

		m_SceneRenderer->Begin(sceneCamera);

		{
			auto pointLights = m_Registry.View<PointLightComponent>();

			for (const auto& [pointLight, entityID] : pointLights)
				m_SceneRenderer->SubmitPointLight(pointLight.Position, pointLight.Color, pointLight.Radius, pointLight.Intensity);
		}


		{
			auto directionalLights = m_Registry.View<DirectionalLightComponent>();

			for (const auto& [light, entityID] : directionalLights)
				m_SceneRenderer->SubmitDirectionaLight(light.Direction, light.Color, light.Intensity);
		}

		{
			auto& meshView        = m_Registry.View<MeshComponent>();
			auto  transformBucket = m_Registry.GetBucket<TransformComponent>();
			auto  materialBucket  = m_Registry.GetBucket<MaterialComponent>();

			for (const auto& [component, entityID] : meshView)
			{
				if (!transformBucket->Contains(entityID))
					continue;

				auto& transform = transformBucket->Get(entityID);

				if (!materialBucket->Contains(entityID))
				{
					m_SceneRenderer->Submit(component.ID, transform.GetTransform());
				}
				else
				{
					auto& material = materialBucket->Get(entityID);
					m_SceneRenderer->Submit(component.ID, transform.GetTransform(), (uint32_t)material.ID);
				}
			}

			auto& modelView = m_Registry.View<ModelComponent>();

			for (const auto& [component, entityID] : modelView)
			{
				if (!transformBucket->Contains(entityID))
					continue;

				auto& transform = transformBucket->Get(entityID);
				m_SceneRenderer->SubmitModel(component.Info, transform.GetTransform());
			}

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

		m_SceneRenderer->Reset();

		DrawAxis();


		if (m_SceneView == SceneView::View3D)
			m_SceneRenderer->Begin(m_SceneCamera.get());
		else
			m_SceneRenderer->Begin(m_SceneCamera2D.get());


		{
			auto pointLights = m_Registry.View<PointLightComponent>();

			for (auto& [pointLight, entityID] : pointLights)
				m_SceneRenderer->SubmitPointLight(pointLight.Position, pointLight.Color, pointLight.Radius, pointLight.Intensity);
		}


		{
			auto directionalLights = m_Registry.View<DirectionalLightComponent>();

			for (auto& [light, entityID] : directionalLights)
				m_SceneRenderer->SubmitDirectionaLight(light.Direction, light.Color, light.Intensity);
		}

		{
			TGE_PROFILE_SCOPE(RenderGeometry);

			auto& meshView		  = m_Registry.View<MeshComponent>();
			auto  transformBucket = m_Registry.GetBucket<TransformComponent>();
			auto  materialBucket  = m_Registry.GetBucket<MaterialComponent>();

			for (const auto& [component, entityID] : meshView)
			{
				if (!transformBucket->Contains(entityID))
					continue;

				auto& transform = transformBucket->Get(entityID);

				if (!materialBucket->Contains(entityID))
				{
					m_SceneRenderer->Submit(component.ID, transform.GetTransform());
				}
				else
				{
					auto& material = materialBucket->Get(entityID);
					m_SceneRenderer->Submit(component.ID, transform.GetTransform(), (uint32_t)material.ID);
				}
			}

			auto& modelView = m_Registry.View<ModelComponent>();

			for (const auto& [component, entityID] : modelView)
			{
				if (!transformBucket->Contains(entityID))
					continue;

				auto& transform = transformBucket->Get(entityID);
				m_SceneRenderer->SubmitModel(component.Info, transform.GetTransform());
			}
			
		}

		m_SceneRenderer->End();

	}

	void Scene::OnEvent(Event* event)
	{
		if (event->GetType() == EventType::ViewportResize)
		{
			ViewportResizeEvent* windowResizeEvent = (ViewportResizeEvent*)event;
			Common::OnWindowResize(windowResizeEvent->GetWidth(), windowResizeEvent->GetHeight());
			
			m_SceneRenderer->OnWindowResize(windowResizeEvent->GetWidth(), windowResizeEvent->GetHeight());
			m_SceneCamera->OnWindowResize((float)windowResizeEvent->GetWidth(), (float)windowResizeEvent->GetHeight());
			m_SceneCamera2D->OnWindowResize((float)windowResizeEvent->GetWidth(), (float)windowResizeEvent->GetHeight());
		}
	}
	void Scene::SetSceneView(SceneView view)
	{
		m_SceneView = view;
	}
	void Scene::DrawAxis()
	{
		if (m_SceneView == SceneView::View2D)
			return;

		glm::mat4 orientation = m_SceneCamera->GetOrientation();

		OrthographicCameraData data;
		OrthographicCamera ortho(data);

		m_SceneRenderer->Begin(&ortho);

		glm::mat3 rotationMatrix = glm::mat3(orientation);

		glm::vec3 eulerAngles = glm::eulerAngles(glm::quat_cast(rotationMatrix));

		glm::vec3 screenOffset = { 0.8f, 0.8f, 0.0f }; 

		constexpr glm::mat4 identity = glm::identity<glm::mat4>();

		glm::mat4 transformX = glm::translate(identity, screenOffset) * glm::rotate(identity, eulerAngles[0], {1.0f, 0.0f, 0.0f}) *
			glm::scale(identity, { 0.3f, 0.02f, 0.02f });
		m_SceneRenderer->Submit(0, transformX, m_Red);

		glm::mat4 transformY = glm::translate(identity, screenOffset) * glm::rotate(identity, eulerAngles[1], { 0.0f, 1.0f, 0.0f }) *
			glm::scale(identity, { 0.3f, 0.02f, 0.02f });
		m_SceneRenderer->Submit(0, transformY, m_Green);

		glm::mat4 transformZ = glm::translate(identity, screenOffset) * glm::rotate(identity, eulerAngles[2], { 0.0f, 0.0f, 1.0f }) *
			glm::scale(identity, { 0.3f, 0.02f, 0.02f });
		m_SceneRenderer->Submit(0, transformZ, m_Blue);

		m_SceneRenderer->End();

	}
	Entity Scene::Add(const std::string& name)
	{
		return m_Registry.Add(name);
	}
}