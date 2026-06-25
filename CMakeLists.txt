cmake_minimum_required(VERSION 3.16)
project(CanvasEditor VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---------------------------------------------------------------------------
# Biblioteca central (reutilizada por todas as interfaces)
# ---------------------------------------------------------------------------
add_library(canvas_core STATIC
    src/core/Canvas.cpp
    src/core/Drawing.cpp
    src/core/History.cpp
    src/core/CanvasIO.cpp
    src/core/CanvasManager.cpp
)
target_include_directories(canvas_core PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_compile_options(canvas_core PRIVATE -Wall -Wextra)

# ---------------------------------------------------------------------------
# Interface de terminal (sempre compilada)
# ---------------------------------------------------------------------------
add_executable(canvas_cli src/cli/main_cli.cpp)
target_link_libraries(canvas_cli PRIVATE canvas_core)

# ---------------------------------------------------------------------------
# Interface grafica Qt6 (opcional: so compila se o Qt estiver instalado)
# ---------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    qt_standard_project_setup()
    qt_add_executable(canvas_gui
        src/gui/main_gui.cpp
        src/gui/MainWindow.cpp
        src/gui/CanvasView.cpp
    )
    target_link_libraries(canvas_gui PRIVATE canvas_core Qt6::Widgets)
    message(STATUS "Qt6 encontrado: a interface grafica (canvas_gui) sera compilada.")
else()
    message(STATUS "Qt6 nao encontrado: apenas a CLI (canvas_cli) sera compilada.")
endif()
