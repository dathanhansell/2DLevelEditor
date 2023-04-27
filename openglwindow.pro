include(openglwindow.pri)

SOURCES += \
    main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/opengl/openglwindow
INSTALLS += target
RESOURCES += \
    shaders.qrc \
    textures.qrc
SOURCES += \
    openglscene.cpp \
    geometryengine.cpp

HEADERS += \
    openglscene.h \
    geometryengine.h
