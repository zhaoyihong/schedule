.PHONY:clean test
CC=g++
CC_FLAGS=-Wall -g --std=c++11 -DRANDOM_START
BIN=localsearch
SRC=localsearch.cpp

$(BIN):$(SRC)
	$(CC) $(SRC) $(CC_FLAGS)  -o $(BIN) 
test4:
	./$(BIN) ../../shell/data/matrix4-4-2 4 31
test8:
	./$(BIN) ../../shell/data/matrix8-4-1 8 31
test16:
	./$(BIN) ../../shell/data/matrix16-4-1 16 31
clean:
	rm $(BIN)
