#pragma once

#include <Core/Core.h>


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

	private:
		bool m_ProjectLoader = true;
		char m_File[256];
		std::filesystem::path m_ProjectPath;
	};
}