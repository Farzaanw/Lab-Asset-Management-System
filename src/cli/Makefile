CXX = g++
# Use repo-relative include path from src/cli -> library is ../library
CXXFLAGS = -std=c++17 -I../library -O2

# Only compile sources that actually exist for the CLI
SRC = main.cpp ../SystemController.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = TestExecutable.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean run

run: $(TARGET)
	./$(TARGET)