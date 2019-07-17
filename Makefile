CC=clang++
CFLAGS=-g
LDFLAGS=-c 

all:	lpmatrix.o Token.o Tokenizer.o SymbolTabel.o StraffeLP.o StandardLP.o LP.o
	$(CC) $(CFLAGS) Token.o Tokenizer.o SymbolTabel.o lpmatrix.o StraffeLP.o StandardLP.o LP.o test.cpp 
LP.o:	LP.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) LP.cpp
StandardLP.o:	StandardLP.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) StandardLP.cpp
StraffeLP.o:	StraffeLP.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) StraffeLP.cpp
SymbolTabel.o:	SymbolTabel.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) SymbolTabel.cpp

Tokenizer.o:	Tokenizer.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) Tokenizer.cpp

Token.o:	Token.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) Token.cpp

lpmatrix.o:	lpmatrix.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) lpmatrix.cpp
clean:
	-rm -rf *.o


