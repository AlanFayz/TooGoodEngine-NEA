#pragma once

#include <Core/Core.h>

namespace GoodEditor {

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
	};
}