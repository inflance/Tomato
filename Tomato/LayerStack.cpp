#include "LayerStack.h"

namespace Tomato {

	LayerStack::LayerStack()
	{

	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
			delete layer;
		}
			
	}
	
	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin(), layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverLayer(Layer* overLay)
	{
		m_layers.emplace_back(overLay);
		overLay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin()+m_layerInsert, layer);
		if (it != m_layers.begin() + m_layerInsert)
		{
			layer->OnDetach();
			m_layers.erase(it);
			m_layerInsert--;
		}
	}

	void LayerStack::PopOverLayer(Layer* overLay)
	{
		auto it = std::find(m_layers.begin() + m_layerInsert, m_layers.end(), overLay);
		if (it != m_layers.end())
		{
			overLay->OnDetach();
			m_layers.erase(it);
		}
	}

}