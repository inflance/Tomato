#pragma once

namespace Tomato::VKConfig
{
	//Version Config
	inline static const std::string ApplicationName = {"Tomato Editor"};
	inline static const std::string EngineName = {"Tomato Engine"};
	inline static constexpr uint32_t ApiVersion = VK_API_VERSION_1_2;

	//Layer Config
	inline constexpr bool StandardValidationLayer = false;
	inline constexpr bool AssistanceLayer = false;
	inline constexpr bool EnableValidationLayers = true;
}
