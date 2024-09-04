CC=clang++
CFLAGS=-Iinclude -g
LD=ld
LDFLAGS=-lstdc++
OBJDIR:=bin
OBJS=$(addprefix $(OBJDIR)/, lpmatrix.o Token.o Tokenizer.o SymbolTabel.o StraffeLP.o StandardLP.o LP.o)
VPATH=src

all:	test

$(OBJS): | $(OBJDIR)					# order-only prerequisite

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
$(OBJDIR):
	-mkdir $(OBJDIR)
test: test.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(OBJDIR)/test
clean:
	-rm -rf bin/



