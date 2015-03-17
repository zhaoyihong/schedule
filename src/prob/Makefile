.PHONY:clean test
CC=g++
CC_FLAGS=-Wall -g --std=c++11
BIN=deny
SRC=deny.cpp

$(BIN):$(SRC)
	$(CC) $(SRC) $(CC_FLAGS)  -o $(BIN) 
test4:
	./$(BIN) ../../shell/data/m4-3 4 16
test8:
	./$(BIN) ../../shell/data/m8-3 8 16
clean:
	rm $(BIN)
