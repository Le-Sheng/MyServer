  
DIR_BIN = .
DIR_OBJ = ./obj
DIR_SRC = ./

SRC = $(wildcard $(DIR_SRC)/*.cpp)
OBJ = $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(notdir $(SRC)))


CXX_FLAG = -g  -std=c++17 -pthread -O3
CC = g++

TARGET = echoserver

$(DIR_BIN)/$(TARGET) : $(OBJ)
	$(CC) $(CXX_FLAG) -o $@ $^

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.cpp
	if [ ! -d $(DIR_OBJ) ];	then mkdir -p $(DIR_OBJ); fi;
	$(CC) $(CXX_FLAG) -c $< -o $@

.PHONY : clean
clean : 
	-rm -rf $(DIR_OBJ)