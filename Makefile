TARGET = ./a.out
COMPILER = g++
FLAGS = -Wall

PREF_SRC = ./source/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))

all: $(TARGET)

$(TARGET) : $(OBJ)
	$(COMPILER) $(OBJ) -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

clean :
	rm $(TARGET) $(PREF_OBJ)*.o