# Compiler settings
CXX 		 := g++
CXXFLAGS := -std=c++11 -pedantic -Wall -Wextra -Wshadow -Wwrite-strings -O2 -g
LDFLAGS  := -lsfml-graphics -lsfml-window -lsfml-system -lGL

# Directories and targets
INCLUDES 		:= -I./src
LIB_DIR 		:= ./lib
LIB_TARGET 	:= $(LIB_DIR)/libsfml-widgets.a
DEMO_TARGET := sfml-widgets-demo

# Library
LIB_SRC = \
  src/Gui/Layouts/FormLayout.cpp \
  src/Gui/Layouts/Layout.cpp \
  src/Gui/Layouts/VBoxLayout.cpp \
  src/Gui/Layouts/HBoxLayout.cpp \
  src/Gui/Slider.cpp \
  src/Gui/Label.cpp \
  src/Gui/TextBox.cpp \
  src/Gui/CheckBox.cpp \
  src/Gui/Utils/Arrow.cpp \
  src/Gui/Utils/Cross.cpp \
  src/Gui/Utils/Box.cpp \
  src/Gui/Widget.cpp \
  src/Gui/SpriteButton.cpp \
  src/Gui/ProgressBar.cpp \
  src/Gui/Image.cpp \
  src/Gui/Menu.cpp \
  src/Gui/Button.cpp \
  src/Gui/Theme.cpp

LIB_OBJ := $(LIB_SRC:.cpp=.o)

# Demo
DEMO_SRC := demo/demo.cpp
DEMO_OBJ := $(DEMO_SRC:.cpp=.o)

# Rules
all: $(LIB_TARGET) $(DEMO_TARGET)

# Static library
$(LIB_TARGET): $(LIB_OBJ)
	@mkdir -p $(LIB_DIR)
	@echo "\033[1;33mLinking library\033[0m $@"
	$(AR) rcs $@ $^

# Library object
src/Gui/%.o: src/Gui/%.cpp
	@echo "\033[1;33mCompiling\033[0m $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Demo executable
$(DEMO_TARGET): $(DEMO_OBJ) $(LIB_TARGET)
	@echo "\033[1;33mLinking executable\033[0m $@"
	$(CXX) $(CXXFLAGS) $(DEMO_OBJ) -L$(LIB_DIR) -lsfml-widgets $(LDFLAGS) -o $@
	@echo "\033[1;32mDone!\033[0m"

# Demo object
demo/%.o: demo/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "\033[1;33mRemoving\033[0m $(LIB_TARGET), $(DEMO_TARGET) and object files"
	-@rm -f $(LIB_OBJ) $(LIB_TARGET) $(DEMO_OBJ) $(DEMO_TARGET)

.PHONY: all clean
