CPP= g++
export CPATH=/usr/inlcude/c++/4.8.4
EXTRA_FLAGS= -Wno-sign-compare -Wno-reorder -Wno-unused -Werror
OBJ_FLAGS=-c
DBG_FLAGS= -g
PROD_FLAGS= -O4

CPPFLAGS = -g -Wall $(EXTRA_FLAGS) -fPIC -std=c++11 -fpermissive 

TARGET = Simulator

all : 	depend scheme.tab.h $(TARGET)

OBJS = gc.o  parser.o lexer.o \
	SchemeDriver.o SchemeAST_Print.o SchemeAST_ANFs.o \
	SchemeAST_Demands.o SchemeAST_Defs.o Output.o Demands.o OptDemands.o

$(TARGET): $(OBJS) $(TARGET).o 
	$(CPP) -o $(TARGET) $(CPPFLAGS) $(TARGET).o $(OBJS)

scheme.tab.h: 
parser.cpp: scheme.y 
	bison -dv $<
	mv scheme.tab.c $@

lexer.cpp: scheme.l parser.o SchemeScanner.hpp
	flex --outfile=lexer.cpp $<

format:
	astyle -A14Ck2M56nNSQW2xC96 *.cpp *.h *.hpp

.PHONY: clean remove
clean: remove
	rm -rf $(TARGET) $(TARGET)_Stats

remove:
	rm -rf *~ *.ghc 
	rm -rf *.o scheme_lexer.cc parser.cpp
	rm -rf location.hh position.hh stack.hh
	rm -rf scheme.tab.h scheme.tab.c scheme.output
	rm -rf PostGC*.txt GC*.txt PreGC*.txt gc_messages.txt pre_gc_addr.txt

depend: __depends

__depends: *.cpp *.h
	touch __depends
	makedepend -f __depends *.cpp *.h 2> /dev/null

sinclude __depends
