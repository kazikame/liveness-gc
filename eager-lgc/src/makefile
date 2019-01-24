CC = g++
CFLAGS = -g -Wall -fPIC -std=c++11 -fpermissive
OBJ_FLAGS=-c
DBG_FLAGS=-pg -O0
PROD_FLAGS=-O0

TARGET = Simulator

all : $(TARGET)




$(TARGET): $(TARGET).cpp gc.o  parser.o lexer.o SchemeDriver.o SchemeAST_Print.o SchemeAST_ANFs.o SchemeAST_Demands.o SchemeAST_Defs.o output.o demands.o
	$(CC) -o $(TARGET) $(CFLAGS) $(TARGET).cpp parser.o lexer.o SchemeDriver.o SchemeAST_Print.o SchemeAST_ANFs.o SchemeAST_Demands.o SchemeAST_Defs.o output.o demands.o gc.o


gc.o : gc.cpp 
	$(CC) $(CFLAGS) -c gc.cpp

parser.o: scheme.y SchemeAST.h SchemeDriver.h SchemeScanner.hpp
	bison -dv $<
	$(CC) -o parser.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) scheme.tab.c

lexer.o: scheme.l parser.o SchemeScanner.hpp
	flex --outfile=scheme_lexer.cc $<
	$(CC) -o lexer.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) scheme_lexer.cc
output.o: Output.h Output.cpp
	$(CC) -o output.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) Output.cpp

demands.o: Demands.h Demands.cpp
	$(CC) -o demands.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) Demands.cpp

SchemeAST_Defs.o: Utils.hpp SchemeAST.h SchemeAST_Defs.cpp gc.cpp
	$(CC) -o SchemeAST_Defs.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeAST_Defs.cpp 

SchemeAST_ANFs.o: SchemeAST.h SchemeAST_ANFs.cpp
	$(CC) -o SchemeAST_ANFs.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeAST_ANFs.cpp

SchemeAST_Print.o: SchemeAST.h SchemeAST_Print.cpp
	$(CC) -o SchemeAST_Print.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeAST_Print.cpp

SchemeAST_Demands.o: Demands.h SchemeAST.h SchemeAST_Demands.cpp
	$(CC) -o SchemeAST_Demands.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeAST_Demands.cpp

SchemeDriver.o: parser.o Output.h SchemeScanner.hpp SchemeDriver.h SchemeDriver.cpp
	$(CC) -o SchemeDriver.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeDriver.cpp

format:
	astyle -A14Ck2M56nNSQW2xC96 *.cpp *.h *.hpp

clean: remove
	rm -rf $(TARGET) 
remove:
	rm -rf *~ *.ghc
	rm -rf *.o scheme_lexer.cc
	rm -rf location.hh position.hh stack.hh
	rm -rf scheme.tab.h scheme.tab.c scheme.output
