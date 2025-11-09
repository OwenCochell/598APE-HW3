FUNC := g++
copt := -c
OBJ_DIR := ./bin/
FLAGS := -O3 -lm -g -std=c++20 -Werror -fopenmp -flto -ffast-math

C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix $(OBJ_DIR),$(notdir $(C_FILES:.c=.obj)))

all:
	$(FUNC) ./main.cpp -o ./main.exe $(FLAGS)

clean:
	rm -f ./*.exe