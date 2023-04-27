
cmake_minimum_required(VERSION 3.16)
project(2DLevelEditor LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/opengl/openglwindow")

find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets Core Gui OpenGL)

qt_standard_project_setup()

include_directories("C:\\Users\\Dathan\\Code Projects\\box2d\\include\\box2d")
link_directories("C:\\Users\\Dathan\\Code Projects\\box2d\\build\\bin\\Debug")

set(SOURCE_FILES main.cpp mainwindow.h mainwindow.cpp
openglscene.h openglscene.cpp
box2dworld.h box2dworld.cpp
geometryengine.h geometryengine.cpp
shadermanager.h shadermanager.cpp
drawable.h drawable.cpp
mesh.h tilemesh.h tilemesh.cpp
)
set(UI_FILES mainwindow.ui)
set(RESOURCE_FILES shaders.qrc textures.qrc)

QT6_ADD_RESOURCES(PROJECT_RESOURCES ${RESOURCE_FILES})

qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES} ${PROJECT_RESOURCES})


set_target_properties(2DLevelEditor PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(2DLevelEditor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    # Link Box2D library
    box2d
)

install(TARGETS 2DLevelEditor
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
