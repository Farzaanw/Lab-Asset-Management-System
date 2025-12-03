CXX = g++
CXXFLAGS = -std=c++17 -I.. -I../library -I../users -O2

SRC = main.cpp ../SystemController.cpp ../users/User.cpp
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
