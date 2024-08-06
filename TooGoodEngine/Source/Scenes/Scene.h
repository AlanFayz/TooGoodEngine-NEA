#pragma once

#include "Events/Event.h"
#include "Assets/Asset.h"
#include "ECS/EntityTree.h"
#include "Renderer/Renderer.h"
#include "ECS/Components/Components.h"
#include "Math/PerspectiveCameraController.h"

namespace TooGoodEngine {

	//scene loading will be inside project 
	//and handled by the project

	class Scene : public EventHandler 
	{
	public:
		Scene();
		~Scene() = default;

		void Play() {}; //TODO: render scene in context of chosen camera from registry
		void Update(double delta);

		virtual void OnEvent(Event* event) override;

		inline EntityTree& GetRegistry() { return m_Registry; }
		inline Ref<Renderer> GetSceneRenderer() { return m_SceneRenderer; }

		inline const std::string& GetName() const { return m_SceneName; }
		inline void SetName(const std::string& name) { m_SceneName = name; }

	private:
		EntityTree m_Registry;
		Ref<Renderer> m_SceneRenderer;
		std::string m_SceneName;
		Ref<PerspectiveCamera> m_SceneCamera;
		PerspectiveCameraController m_CameraController;
	};

}