CXX := clang++
CXXFLAGS := -std=c++23 -Wall -Wextra -g
CPPFLAGS := -Isrc
TARGET := main
SRC := main.cpp
OBJ := $(SRC:.cpp=.o)
DEPS := $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

.INTERMEDIATE: $(OBJ) $(DEPS)

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ) $(DEPS)
