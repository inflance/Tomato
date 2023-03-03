#pragma once

#include "../Camera/Camera.hpp"
#include "Tomato/Core/Macro.h"
#include "Tomato/Events/Event.h"

namespace Tomato
{

	enum class CameraMove :uint32_t
	{
		None = 0,
		Forward = BIT(0),
		Back = BIT(1),
		Left = BIT(2),
		Right = BIT(3),
		Up = BIT(4),
		Down = BIT(5),
	};

	inline CameraMove operator|(CameraMove& move1, CameraMove& move2)
	{
		return static_cast<CameraMove>(static_cast<uint32_t>(move1) | static_cast<uint32_t>(move2));
	}

	inline CameraMove& operator|=(CameraMove& move1, CameraMove move2)
	{
		return move1 = move1 | move2;
	}

	inline CameraMove operator&(CameraMove move1, CameraMove move2)
	{
		return static_cast<CameraMove>(static_cast<uint32_t>(move1) & static_cast<uint32_t>(move2));
	}

	inline CameraMove& operator&=(CameraMove& move1, CameraMove move2)
	{
		return move1 = move1 & move2;
	}


	template <typename TCamera>
	class CameraController
	{
	public:
		using CameraTickFn = std::function<void(TCamera&, float)>;
		using CameraTickEventFn = std::function<void(TCamera&)>;

		CameraController() = default;

		CameraController(TCamera camera)
			: m_camera(camera)
		{
		}

		void SetCameraTickFnc(const CameraTickFn& fn) { m_func_v.emplace_back(fn); }

		auto& GetCameraTickFnc() { return m_func_v; }

		void SetCameraEventFnc(const CameraTickEventFn& fn) { m_event_fn.emplace_back(fn); }

		auto& GetCameraEventFnc() { return m_event_fn; }

		void Tick(float deltaTime)
		{
			for (const auto& func : m_func_v)
			{
				func(m_camera, deltaTime);
			}
		}

		void Tick(CameraTickFn&& func)
		{
			func(m_camera);
		}

		void TickEvent(Event& e)
		{
			for (const auto& func : m_event_fn)
			{
				func(m_camera);
			}
		}

		void TickEvent(CameraTickEventFn&& func)
		{
			func(m_camera);
		}

	protected:
		std::vector<CameraTickFn> m_func_v;
		std::vector<CameraTickEventFn> m_event_fn;
		TCamera m_camera{};
	};
}
