CC=clang++
CFLAGS=-Iinclude -g
LD=ld
LDFLAGS=-lstdc++
OBJDIR:=bin
OBJS=$(addprefix $(OBJDIR)/, lpmatrix.o Token.o Tokenizer.o SymbolTabel.o StraffeLP.o StandardLP.o LP.o)
VPATH=src

all:	lpmatrix

$(OBJS): | $(OBJDIR)					# order-only prerequisite

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
$(OBJDIR):
	-mkdir $(OBJDIR)
lpmatrix: test.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(OBJDIR)/lpmatrix
clean:
	-rm -rf bin/



