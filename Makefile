ARCHIVE=$(OBJDIR)/liblp.a
BUILDDIR=build
CC=clang++
CFLAGS=-Iinclude -g -Wall -std=c++11
OBJDIR:=bin
OBJS=$(addprefix $(OBJDIR)/, LPMatrix.o Token.o Tokenizer.o SymbolTabel.o StraffeLP.o StandardLP.o SimplexVisualizer.o LP.o)
VPATH=src include

# Default build target
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

help:
	@echo "Linear Programming Solver - Build Targets"
	@echo "==========================================="
	@echo ""
	@echo "Default targets:"
	@echo "  make              - Build the main lpmatrix executable"
	@echo "  make clean        - Remove all build artifacts"
	@echo ""
	@echo "Additional targets:"
	@echo "  make build/test   - Build the test executable"
	@echo "  make -B bin/lpmatrix  - Force rebuild of lpmatrix"
	@echo ""
	@echo "Usage:"
	@echo "  echo 'tests/problems/mathhx.dk.txt' | ./bin/lpmatrix"
	@echo ""
	@echo "For more information, see README.md or INSTRUCTIONS.md"

.PHONY: help clean


