ARCHIVE=$(OBJDIR)/liblp.a
BUILDDIR=build
CC=clang++
CFLAGS=-Iinclude -g -Wall
OBJDIR:=bin
OBJS=$(addprefix $(OBJDIR)/, LPMatrix.o Token.o Tokenizer.o SymbolTabel.o StraffeLP.o StandardLP.o LP.o)
VPATH=src include

all:	$(BUILDDIR)/test
all:	$(OBJDIR)/lpmatrix

$(OBJS): | $(OBJDIR) $(BUILDDIR) 					# order-only prerequisite

$(OBJDIR)/%.o: %.cpp %.h Matrix.h
	$(CC) -c $(CFLAGS) $< -o $@
$(OBJDIR):
	-mkdir $(OBJDIR)
$(BUILDDIR):
	-mkdir $(BUILDDIR)
$(BUILDDIR)/test: test.cpp $(ARCHIVE)
	$(CC) $(CFLAGS) $(ARCHIVE) $^ -o $@
$(OBJDIR)/lpmatrix: test.cpp $(OBJS) 
	$(CC) $(CFLAGS) $^ -o $(OBJDIR)/lpmatrix
clean:
	-rm -rf $(OBJDIR) test test.dSYM
	-rm -rf $(BUILDDIR)
$(ARCHIVE):	$(OBJS)
	ar -r $(ARCHIVE) $(OBJS)
 


