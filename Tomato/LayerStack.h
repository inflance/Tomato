#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace Tomato {

	class LayerStack 
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLay);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* overLay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
		
	private:
		std::vector<Layer*> m_layers;
		unsigned int  m_layerInsert = 0;
	};
}
