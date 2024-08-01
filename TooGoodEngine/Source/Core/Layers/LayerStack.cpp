#include "LayerStack.h"

namespace TooGoodEngine {
	
	void LayerStack::Push(const Ref<Layer>& layer)
	{
		layer->OnCreate();
		m_Stack.push_back(layer);
	}

	void LayerStack::Remove(const Ref<Layer>& layer)
	{
		auto it = std::find(m_Stack.begin(), m_Stack.end(), layer);
		if (it != m_Stack.end())
		{
			(*it)->OnDestroy();
			m_Stack.erase(it);
		}
	}

	void LayerStack::Pop()
	{
		m_Stack.back()->OnDestroy();
		m_Stack.pop_back();
	}

	void LayerStack::OnUpdateLayers(double delta)
	{
		for (auto& layer : m_Stack)
			layer->OnUpdate(delta);
	}

	void LayerStack::OnGuiUpdateLayers(double delta)
	{
		for (auto& layer : m_Stack)
			layer->OnGuiUpdate(delta);
	}

	void LayerStack::OnEvent(Event* event)
	{
		for (auto& layer : m_Stack)
			layer->OnEvent(event);
	}

	LayerStack::~LayerStack()
	{
		for (auto& layer : m_Stack)
			layer->OnDestroy();
	}
}
