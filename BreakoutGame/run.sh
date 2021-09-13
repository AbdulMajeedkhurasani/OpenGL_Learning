#!/bin/bash
g++ -Wall -o $1 $2 Game.cpp includes/Utiliy/texture.cpp \
    includes/Utiliy/Shader.cpp includes/Utiliy/stb_image.cpp \
    includes/Utiliy/resource_manager.cpp glad.c \
    SpriteRenderer.cpp GameLevel.cpp GameObject.cpp\
    BallObject.cpp particleGenerator.cpp\
    -lassimp -lGL -lglut  -lglfw3  -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm \
    -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
./$1