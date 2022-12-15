CC=clang++
CFLAGS=-Iinclude -g
LD=ld
LDFLAGS=-lstdc++
OBJDIR:=bin
OBJS=$(addprefix $(OBJDIR)/, LPMatrix.o Token.o Tokenizer.o SymbolTabel.o StraffeLP.o StandardLP.o LP.o)
VPATH=src include

all:	$(OBJDIR)/lpmatrix

$(OBJS): | $(OBJDIR)					# order-only prerequisite

$(OBJDIR)/%.o: %.cpp %.h Matrix.h
	$(CC) -c $(CFLAGS) $< -o $@
$(OBJDIR):
	-mkdir $(OBJDIR)
$(OBJDIR)/lpmatrix: test.cpp $(OBJS) 
	$(CC) $(CFLAGS) $^ -o $(OBJDIR)/lpmatrix
clean:
	-rm -rf bin/



