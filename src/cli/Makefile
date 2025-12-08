# OS Detection
ifeq ($(OS),Windows_NT)
	RM = del /Q
	FIXPATH = $(subst /,\\,$1)
else
	RM = rm -f
	FIXPATH = $1
endif

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -I.. -I../library -I../users -O2

# Source files
SRC = main.cpp ../SystemController.cpp ../users/User.cpp ../users/ResearchStudent.cpp ../users/LabManager.cpp ../resources/Reservations.cpp ../users/FacultyResearcher.cpp ../users/LabAssetManager.cpp ../resources/Documents.cpp  ../resources/Assets.cpp ../resources/Notifications.cpp ../resources/Dashboard.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = MicroCat.exe

# Phony targets to always run
.PHONY: all clean rebuild run

# Default: rebuild everything
all: rebuild

# Full rebuild: clean then build
rebuild: clean $(TARGET)

# Build executable from object files
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Compile object files from source
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove all compiled files
clean:
	$(RM) $(call FIXPATH,$(TARGET)) $(call FIXPATH,$(OBJ))

# Run the program
run: $(TARGET)
	$(call FIXPATH,./$(TARGET))