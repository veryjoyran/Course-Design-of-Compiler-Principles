CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iinclude -Iimgui -Iimgui/backends

SRC_DIR = src
OBJ_DIR = output
IMGUI_DIR = imgui

IMGUI_SRC = $(IMGUI_DIR)/imgui.cpp \
            $(IMGUI_DIR)/imgui_draw.cpp \
            $(IMGUI_DIR)/imgui_widgets.cpp \
            $(IMGUI_DIR)/imgui_demo.cpp \
            $(IMGUI_DIR)/imgui_tables.cpp \
            $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
            $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
IMGUI_OBJS = $(IMGUI_SRC:.cpp=.o)

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS) $(IMGUI_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lGL -lGLEW -lglfw

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(IMGUI_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(IMGUI_DIR)/*.o $(TARGET)