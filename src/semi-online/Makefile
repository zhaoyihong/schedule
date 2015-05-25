.PHONY:clean test istall
CC=g++
CC_FLAGS=-Wall -g -std=c++11 -DRANDOM_START
#CC_FLAGS=-Wall -g -std=c++11 -DRANDOM_START
BIN=semi-online
SRC=deny.cpp

$(BIN):$(SRC)
	$(CC) $(SRC) $(CC_FLAGS)  -o $(BIN) 
test4:
	./$(BIN) ../../shell/data/matrix4-1-3 4 25
test8:
	./$(BIN) ../../shell/data/matrix8-8-1 8 5 0  1  2  3  4  5  6  7
test16:
	./$(BIN) ../../shell/data/matrix16-1-1 16 25
install:
	cp $(BIN) ../../bin
clean:
	rm $(BIN)
