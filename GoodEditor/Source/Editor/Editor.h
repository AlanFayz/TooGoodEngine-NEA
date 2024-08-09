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

	private:
		bool m_ProjectLoader = true;
		char m_File[256];
		std::filesystem::path m_ProjectPath;
		ImVec2 m_PreviousWindowSize = ImVec2(0, 0);
	};
}