.PHONY:clean
BIN=hungarian
CC=g++
MUNKRES_DIR=/home/yihong/schedule/src/munkres-cpp-master/src
CC_FLAGS=-g -Wall --std=c++11 -L$(MUNKRES_DIR) -lmunkres -I$(MUNKRES_DIR)
PROG=hungarian.cpp
$(BIN):$(PROG)
	$(CC) $(PROG) $(CC_FLAGS) -o $(BIN)
