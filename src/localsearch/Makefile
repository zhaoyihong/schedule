.PHONY:clean test
CC=g++
CC_FLAGS=-Wall -g -std=c++11 
#CC_FLAGS=-Wall -g -std=c++11 -DRANDOM_START
BIN=localsearch
SRC=deny.cpp

$(BIN):$(SRC)
	$(CC) $(SRC) $(CC_FLAGS)  -o $(BIN) 
test4:
	./$(BIN) ../../shell/data/matrix4-1-2 4 31
test8:
	./$(BIN) ../../shell/data/matrix8-9-3 8 25
test16:
	./$(BIN) ../../shell/data/matrix16-1-1 16 31
clean:
	rm $(BIN)
