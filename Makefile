.PHONY: help build run clean

help:
	@echo "Available options:"
	@echo "  make build   - Compile the program"
	@echo "  make run     - Run the program"
	@echo "  make clean   - Remove the executable"
	@echo "  make help    - Show this help message"

build: pingpong.cpp
	@g++ pingpong.cpp -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -o pingpong.exe

run: pingpong.exe
	@.\pingpong.exe

clean:
	@if exist pingpong.exe del /F /Q pingpong.exe
