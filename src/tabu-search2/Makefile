.PHONY:clean test
CC=g++
CC_FLAGS=-Wall -g -std=c++11 -DRANDOM_START
BIN=tabu-search
SRC=deny.cpp

$(BIN):$(SRC)
	$(CC) $(SRC) $(CC_FLAGS)  -o $(BIN) 
test4:
	./$(BIN) ../../shell/data/matrix4-1-3 4 25
test8:
	./$(BIN) ../../shell/data/matrix8-8-1 8 25
test16:
	./$(BIN) ../../shell/data/matrix16-1-1 16 25
test64:
	./$(BIN) ../../shell/data/matrix64-9-1 64 50
clean:
	rm $(BIN)
