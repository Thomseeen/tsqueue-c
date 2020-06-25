########################################
# Settings
########################################
# Compiler settings
CC = gcc
CXXFLAGS = -Wall
LDFLAGS = -lpthread

# Makefile settings
APPNAME = cqueue_test
EXT = .c
SRCDIR = src
OBJDIR = obj


########################################
# Dynamically set
########################################
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)

# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)

########################################
# Targets
########################################
all: $(APPNAME)
	@printf "### Finished Make-Build ###\n\n"

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME)
	@printf "### Cleaned ###\n\n"

# Make symlinks to libraries
.PHONY: symlib
symlib:
	cd src/; ln -s ../lib/log.c/src/log.h log.h
	cd src/; ln -s ../lib/log.c/src/log.c log.c
