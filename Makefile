ARCHIVE=$(OBJDIR)/liblp.a
BUILDDIR=build
CC=clang++
CFLAGS=-Iinclude -g -Wall
OBJDIR:=bin
OBJS=$(addprefix $(OBJDIR)/, lpmatrix.o Token.o Tokenizer.o SymbolTabel.o StraffeLP.o StandardLP.o LP.o)
VPATH=src

all:	$(BUILDDIR)/test

$(OBJS): | $(OBJDIR) $(BUILDDIR) 					# order-only prerequisite

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
$(OBJDIR):
	-mkdir $(OBJDIR)
$(BUILDDIR):
	-mkdir $(BUILDDIR)
$(BUILDDIR)/test: test.cpp $(ARCHIVE)
	$(CC) $(CFLAGS) $(ARCHIVE) $^ -o $@
clean:
	-rm -rf $(OBJDIR) test test.dSYM
	-rm -rf $(BUILDDIR)
$(ARCHIVE):	$(OBJS)
	ar -r $(ARCHIVE) $(OBJS)
 


