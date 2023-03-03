cmake_minimum_required (VERSION 3.12.4)

add_subdirectory(glfw)
add_subdirectory(spdlog)
add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(entt)
add_subdirectory(json)
add_subdirectory(ImGuizmo)
add_subdirectory(VulkanMemoryAllocator)
add_subdirectory(vulkan-hpp)
add_subdirectory(tinyobjloader)
add_subdirectory(box2d)


file(GLOB assimpdll "${CMAKE_CURRENT_SOURCE_DIR}/assimp/bin/Debug/assimp-vc142-mtd.dll")
file(COPY ${assimpdll} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

