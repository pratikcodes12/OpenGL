# CMakeList.txt : CMake project for ModernOpenGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ModernOpenGL")

# dependencies
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.5.1
)

FetchContent_MakeAvailable(glfw)

# ---------------- GLEW ----------------
FetchContent_Declare(
    glew
    URL https://github.com/nigels-com/glew/releases/download/glew-2.3.1/glew-2.3.1.zip
    SOURCE_SUBDIR build/cmake
)

FetchContent_MakeAvailable(glew)

# Add source to this project's executable.
add_executable (ModernOpenGL src/main.cpp)

add_custom_command(TARGET ModernOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:ModernOpenGL>/res"
)

target_include_directories(
    ModernOpenGL
    PRIVATE
        ${glew_SOURCE_DIR}/include
)

target_link_libraries(ModernOpenGL
    PRIVATE
        glfw
        glew
        opengl32
)

if (WIN32)
    add_custom_command(TARGET ModernOpenGL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:glew>
            $<TARGET_FILE_DIR:ModernOpenGL>
    )
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ModernOpenGL PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
