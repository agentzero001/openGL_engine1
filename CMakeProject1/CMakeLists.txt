# CMakeList.txt: CMake-Projekt für "CMakeProject1". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#


include_directories("E:/Users/jensm/Desktop/OpenGLlibraries/include")
link_directories("E:/Users/jensm/Desktop/OpenGLlibraries/lib")

include_directories("E:/Users/jensm/Documents/Visual Studio 2022/stb-master")

include_directories("E:/Users/jensm/Documents/Visual Studio 2022/tinyobjloader-release")


# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (
  CMakeProject1 
  "CMakeProject1.cpp"
  "Context.cpp"
  "utils.cpp"
  "keyboard.cpp"
  "properties.cpp"
  "resource.cpp"
  "modelGen/Leaf.cpp"
  "modelGen/Sphere.cpp"
  "modelGen/Torus.cpp"
  ImGui/imgui.cpp
  ImGui/imgui_draw.cpp
  ImGui/imgui_tables.cpp
  ImGui/imgui_widgets.cpp
  ImGui/imgui_demo.cpp 

  ImGui/backends/imgui_impl_glfw.cpp
  ImGui/backends/imgui_impl_opengl3.cpp)
  

target_include_directories(CMakeProject1 PRIVATE
    ImGui
    ImGui/backends
)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(CMakeProject1 glfw3 opengl32 glew32 SOIL2-debug)

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.


target_compile_definitions(
    CMakeProject1
    PRIVATE RESOURCE_DIR="${CMAKE_SOURCE_DIR}/res"
)