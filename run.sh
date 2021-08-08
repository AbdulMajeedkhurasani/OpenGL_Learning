#!/bin/bash
g++ -Wall -o $1 $2 glad.c -lGL -lglut  -lglfw3  -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
./$1
