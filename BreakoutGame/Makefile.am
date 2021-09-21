bin_PROGRAMS = myprogram
myprogram_SOURCES = main.cpp Game.cpp includes/Utiliy/texture.cpp \
    includes/Utiliy/Shader.cpp includes/Utiliy/stb_image.cpp \
    includes/Utiliy/resource_manager.cpp glad.c \
    includes/Utiliy/SpriteRenderer.cpp GameLevel.cpp GameObject.cpp\
    BallObject.cpp particleGenerator.cpp PostProcessor.cpp
AM_CXXFLAGS = @gllibs_CFLAGS@
AM_LDFLAGS = @gllibs_LIBS@ -lGL -lglut -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm \
    -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor