#pragma once

#include <Core/Core.h>

#include "AssetPanel.h"

namespace GoodEditor {

	using namespace TooGoodEngine;

	class Editor : public TooGoodEngine::Layer
	{
	public:
		Editor() = default;
		~Editor() = default;

		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate(double delta) override;
		virtual void OnGuiUpdate(double delta) override;
		virtual void OnEvent(TooGoodEngine::Event* event) override;

	private:
		void _TryGetProjectName();
		void _RenderViewport(const Ref<OpenGL::Texture2D>& image);
		void _RenderViewportMenuBar();

	private:
		bool m_ProjectLoader = true;
		bool m_Playing = false;
		char m_File[256];
		std::filesystem::path m_ProjectPath;
		ImVec2 m_PreviousWindowSize = ImVec2(0, 0);

		//These images are not loaded into project asset manager simply because
		//they are external to the engine. They are for the editor
		std::map<std::filesystem::path, Ref<Image>> m_ExtensionMap; 

		uint32_t m_CurrentWidth = 1200;
		uint32_t m_CurrentHeight = 800;
	};
}