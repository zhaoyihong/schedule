.PHONY:clean test
CC=g++
#CC_FLAGS=-Wall -g -std=c++11 -DRANDOM_START
CC_FLAGS=-Wall -g -std=c++11
BIN=random_one
SRC=random.cpp

$(BIN):$(SRC)
	$(CC) $(SRC) $(CC_FLAGS)  -o $(BIN) 
test4:
	./$(BIN) ../../shell/data/matrix4-1-2 4 31
test8:
	./$(BIN) ../../shell/data/matrix8-1-1 8 31
test16:
	./$(BIN) ../../shell/data/matrix16-1-1 16 31
clean:
	rm $(BIN)
