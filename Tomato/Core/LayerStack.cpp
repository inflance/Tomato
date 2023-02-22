#include "LayerStack.h"

namespace Tomato
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
		{
			layer->OnDestroy();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin(), layer);
		layer->OnCreate();
	}

	void LayerStack::PushOverLayer(Layer* over_layer)
	{
		m_layers.emplace_back(over_layer);
		over_layer->OnCreate();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_layer_insert, layer);
		if (it != m_layers.begin() + m_layer_insert)
		{
			layer->OnDestroy();
			m_layers.erase(it);
			--m_layer_insert;
		}
	}

	void LayerStack::PopOverLayer(Layer* over_layer)
	{
		auto it = std::find(m_layers.begin() + m_layer_insert, m_layers.end(), over_layer);
		if (it != m_layers.end())
		{
			over_layer->OnDestroy();
			m_layers.erase(it);
		}
	}
}
