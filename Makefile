CC=g++
CFLAGS=-std=c++17 -Ilib/ -Iinc/
LFLAGS=-lglfw3
SRC=src
INC=inc

SRC_FILES=\
	${SRC}/main.cpp \
	${SRC}/shader.cpp \
	${SRC}/model.cpp \
	${SRC}/mnist.cpp \
	${SRC}/texture.cpp \

INC_FILES=\
	${INC}/shader.h \
	${INC}/model.h \
	${INC}/mnist.h \
	${INC}/texture.h \

build:
	${CC} ${CFLAGS} ${SRC_FILES} ${INC_FILES} lib/glad/glad.c ${LFLAGS} -o ug
.PHONY: build

run: ug
	./ug

ohio:
	zig c++ -Ilib/ -Iinc/ test.cpp lib/glad/glad.c -lglfw3 -o ug

heb: src/hebbian5.cpp
	g++ -g3 -Ilib/ -Iinc/ src/tensor.cpp src/container.cpp src/hebbian5.cpp -o hebbian
	./hebbian

tens: tens.cpp
	g++ -Ilib/ -Iinc/ src/tensor.cpp src/container.cpp tens.cpp -o tens
	./tens
