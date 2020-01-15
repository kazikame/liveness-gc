CC = g++
CFLAGS = -Wall -fPIC -std=c++11 -fpermissive
OBJ_FLAGS=-c
DBG_FLAGS=-pg -O4
PROD_FLAGS=-O4
OBJS = gc_diff.o parser.o lexer.o SchemeDriver.o SchemeAST_ANFs.o SchemeAST_Demands.o SchemeAST_Defs.o SchemeAST_Print.o Output.o DemandStructure_diff.o
TARGET = Simulator

all : $(TARGET)_diff

$(TARGET)_diff: $(TARGET).cpp gc_diff.o parser.o lexer.o SchemeDriver.o SchemeAST_ANFs.o SchemeAST_Demands.o SchemeAST_Defs.o SchemeAST_Print.o Output.o DemandStructure_diff.o
	$(CC) -o $(TARGET)_diff $(CFLAGS) $(PROD_FLAGS) $(TARGET).cpp gc_diff.o parser.o lexer.o SchemeDriver.o SchemeAST_ANFs.o SchemeAST_Demands.o SchemeAST_Defs.o SchemeAST_Print.o Output.o DemandStructure_diff.o

$(TARGET)_simple: $(TARGET).cpp $(OBJS) DemandStructure_simple.o
	$(CC) -o $(TARGET)_simple $(CFLAGS) $(PROD_FLAGS) $(TARGET).cpp $(OBJS) DemandStructure_simple.o

$(TARGET): $(TARGET).cpp $(OBJS) DemandStructure.o
	$(CC) -o $(TARGET) $(CFLAGS) $(PROD_FLAGS) $(TARGET).cpp $(OBJS) DemandStructure.o

gc.o : gc.cpp gc.h
	$(CC) $(CFLAGS) $(PROD_FLAGS) $(OBJ_FLAGS) gc.cpp
gc_diff.o : gc_diff.cpp gc.h
	$(CC) $(CFLAGS) $(PROD_FLAGS) $(OBJ_FLAGS) gc_diff.cpp

parser.o: scheme.y SchemeAST.h SchemeDriver.h SchemeScanner.hpp
	bison -dv $<
	$(CC) -o parser.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) scheme.tab.c

lexer.o: scheme.l parser.o SchemeScanner.hpp
	flex --outfile=scheme_lexer.cc $<
	$(CC) -o lexer.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) scheme_lexer.cc

Output.o: Output.h Output.cpp
	$(CC) -o Output.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) Output.cpp

SchemeAST_Print.o: SchemeAST.h SchemeAST_Print.cpp DemandStructure.h
	$(CC) -o SchemeAST_Print.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeAST_Print.cpp 

SchemeAST_Defs.o: Utils.hpp SchemeAST.h SchemeAST_Defs.cpp gc.cpp DemandStructure.h
	$(CC) -o SchemeAST_Defs.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeAST_Defs.cpp 

SchemeAST_ANFs.o: SchemeAST.h SchemeAST_ANFs.cpp DemandStructure.h
	$(CC) -o SchemeAST_ANFs.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeAST_ANFs.cpp

SchemeAST_Demands.o: SchemeAST.h SchemeAST_Demands.cpp DemandStructure.h
	$(CC) -o SchemeAST_Demands.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeAST_Demands.cpp

SchemeDriver.o: parser.o SchemeScanner.hpp SchemeDriver.h SchemeDriver.cpp
	$(CC) -o SchemeDriver.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) SchemeDriver.cpp

DemandStructure.o: DemandStructure.cpp DemandStructure.h
	$(CC) -o DemandStructure.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) DemandStructure.cpp

DemandStructure_simple.o: DemandStructure_simple.cpp DemandStructure.h
	$(CC) -o DemandStructure_simple.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) DemandStructure_simple.cpp
DemandStructure_diff.o: DemandStructure_diff.cpp DemandStructure.h
	$(CC) -o DemandStructure_diff.o $(CFLAGS) $(OBJ_FLAGS) $(PROD_FLAGS) DemandStructure_diff.cpp

format:
	astyle -A14Ck2M56nNSQW2xC96 *.cpp *.h *.hpp

clean: remove
	rm -rf $(TARGET)
	rm -rf $(TARGET)_simple
remove:
	rm -rf *~ *.ghc
	rm -rf *.o scheme_lexer.cc
	rm -rf location.hh position.hh stack.hh
	rm -rf scheme.tab.h scheme.tab.c scheme.output
