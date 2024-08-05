#pragma once

#include "Core/Base.h"
#include "Layer.h"

#include <vector>

namespace TooGoodEngine {

	class LayerStack : public EventHandler
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void Push(const Ref<Layer>& layer);
		void Remove(const Ref<Layer>& layer);
		void Pop();

		void OnUpdateLayers(double delta);
		void OnGuiUpdateLayers(double delta);

		virtual void OnEvent(Event* event);

		inline const std::vector<Ref<Layer>>& Get() const { return m_Stack; }

	private:
		std::vector<Ref<Layer>> m_Stack;
	};

}