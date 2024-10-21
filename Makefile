CC=g++
LFLAGS=-lglfw3
G_FLAGS=-std=c++17 -Ilib/ -Iinc/
X_FLAGS=-lstdc++ -Ilib/ -Iinc/
SRC=src
INC=inc
OBJ=obj
RTL=rtl

CPP_FILES=\
	shader.cpp \
	model.cpp \
	mnist.cpp \
	texture.cpp \
	run.cpp \

SRC_FILES=$(foreach file, ${CPP_FILES}, ${SRC}/${file})
OBJ_FILES=$(patsubst %.cpp, ${OBJ}/%.o, $(CPP_FILES))
INC_FILES=$(patsubst %.cpp, ${INC}/%.h, $(CPP_FILES))

SV_FILES=\
	tb_vga.sv \
	top.sv \
	vga.sv \
	vga_known_good.sv \

RTL_FILES=$(foreach file, ${SV_FILES}, ${RTL}/${file})

XSC_FLAGS=$(foreach flag, ${X_FLAGS}, --gcc_compile_options ${flag})

build:
	${CC} ${CFLAGS} ${SRC_FILES} ${INC_FILES} lib/glad/glad.c ${LFLAGS} -o ug
.PHONY: build

files:
	@printf "RTL:\n%s\n" "${RTL_FILES}"
	@printf "SRC:\n%s\n" "${SRC_FILES}"
	@printf "OBJ:\n%s\n" "${OBJ_FILES}"
	@printf "INC:\n%s\n" "${INC_FILES}"
	@printf "FLAGS:\n%s\n" "${XSC_FLAGS}"

#${OBJ}/%.o: ${SRC}/%.cpp:
#	g++ -c ${FLAGS} -o $@ $< $(CFLAGS)

xsc:
	@#g++ -c -fPIC -std=c++17 -Ilib/ -Iinc/ ${SRC_FILES} lib/glad/glad.c -lglfw3 -o ${SRC}
	@#$(foreach f, ${SRC_FILES}, g++ -c $f -o $(patsubst %.cpp, %.o, $f) ${CFLAGS})
	@#g++ -c ${FLAGS} ${SRC_FILES} -lglfw3
	@#for f in ${CPP_FILES}; do echo $$f; done
	for f in $(patsubst %.cpp, %, ${CPP_FILES}); do g++ -c -fPIC ${G_FLAGS} -o ${OBJ}/$$f.o ${SRC}/$$f.cpp; done
	g++ -c -fPIC ${G_FLAGS} -o ${OBJ}/glad.o lib/glad/glad.c
	g++ -shared -Ilib/ -Iinc/ -o lib/libapp.so ${OBJ_FILES} ${OBJ}/glad.o
	xvlog -sv ${RTL_FILES}
	xelab tb_vga -dpiheader dpi.h

test:
	g++ ${SRC}/test.cpp -o test -L "/home/phaedrus/Documents/project/fpga/opengl-vga/lib" -l "app" -I "inc" -I "lib" -lglfw3

xsc2:
	xsc ${SRC}/bridge.cpp ${XSC_FLAGS} --gcc_compile_options "-B/usr/bin" --gcc_compile_options "-lapp" --gcc_compile_options "-Llib" --gcc_link_options "-Wl,-rpath=/home/phaedrus/Documents/project/fpga/opengl-vga/lib" --gcc_link_option "-lglfw3"
	xelab tb_vga -sv_lib dpi -R

cross:
	xvlog -sv rtl/tb_cross.sv rtl/cross.sv
	xelab tb_cross -R

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
