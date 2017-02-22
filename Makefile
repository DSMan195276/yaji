include ./config.mk

# Program wide settings
EXE       := yaji
EXEC      := YAJI
PROG      := $(BIN)/$(EXE)
YAJI_VERSION   := 0
YAJI_SUBLEVEL  := 8
YAJI_PATCH     := 0
YAJI_VERSION_N := $(YAJI_VERSION).$(YAJI_SUBLEVEL).$(YAJI_PATCH)

LIBFLAGS :=

CFLAGS  += -std=gnu99 -I'./include' \
				-DYAJI_VERSION_N="$(YAJI_VERSION_N)"

# 'tree' references the current directory later-on
tree := .

# This is our default target - The default is the first target in the file so
# we need to define this fairly high-up.
all: real-all

.PHONY: all install clean dist real-all configure

# Predefine this variable. It contains a list of extra files to clean. Ex.
CLEAN_LIST :=

# List of files that dependency files should be generated for
DEPS :=

# Current project being compiled - Blank for core
PROJ :=
EXES :=
SRC_OBJS :=

# Set configuration options
Q := @
ifdef V
	Q :=
endif

define create_link_rule
$(1): $(2)
	@echo " LD      $$@"
	$$(Q)$$(LD) -r $(2) -o $$@
endef

# Traverse into tree
#
# For those confused, this does a few things:
#   1. Initalizes some variables for the current directory
#   2. 'Include's the current directory's Makefile, to get it's object information
#   3. Updates the clean-list, dependency list
#   4. Creates a rule that links all the objects in that directory into a
#      single .o file (Including sub-directory objects)
#   5. Recurses into subdirectories listed in the included Makefile

define subdir_inc
tree := $$(tree)/$(1)

subdir-y :=
objs-y :=
clean-list-y :=

include $$(tree)/Makefile

# Prepend 'tree' to everything in the lists - the lists are relative to their directory
CLEAN_LIST += $$(patsubst %,$$(tree)/%,$$(objs-y)) $$(patsubst %,$$(tree)/%,$$(clean-list-y)) $$(tree).o
DEPS += $$(patsubst %,$$(tree)/%,$$(objs-y))

objs := $$(patsubst %,$$(tree)/%,$$(objs-y)) $$(patsubst %,$$(tree)/%.o,$$(subdir-y))

$$(eval $$(call create_link_rule,$$(tree).o,$$(objs)))

$$(foreach subdir,$$(subdir-y),$$(eval $$(call subdir_inc,$$(subdir))))

# Reduce 'tree' by one directory level
tree := $$(patsubst %/$(1),%,$$(tree))
endef

ifeq ($(CONFIG_DEBUG),y)
	CFLAGS += -g -DCONFIG_DEBUG
	ASFLAGS += -g
	LDFLAGS += -g
endif

ifeq ($(CONFIG_PROF),y)
	CFLAGS += -pg
	LDFLAGS += -pg
endif

$(eval $(call subdir_inc,src))

CLEAN_LIST += $(BIN)
CLEAN_LIST += $(PROG)

# Actual entry
real-all: $(PROG)

$(PROG): ./src.o | $(BIN)
	@echo " CCLD    $@"
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $< $(LIBFLAGS)

install:
	$(Q)mkdir -p $(BINDIR)
	@echo " INSTALL $(PROG)"
	$(Q)install -m 775 ./bin/$(PROG) $(BINDIR)
	@echo " INSTALL doc"
	$(Q)install -m 775 ./doc/$(PROG).1 $(MAN1DIR)
	@echo " $(PROG) Installation done"

dist: clean
	$(Q)mkdir -p $(EXE)-$(MY_LISP_VERSION_N)
	$(Q)cp -R Makefile README.md config.mk LICENSE ./default_yabrrc ./include ./src $(EXE)-$(MY_LISP_VERSION_N)
	$(Q)tar -cf $(EXE)-$(MY_LISP_VERSION_N).tar $(EXE)-$(MY_LISP_VERSION_N)
	$(Q)gzip $(EXE)-$(MY_LISP_VERSION_N).tar
	$(Q)rm -fr $(EXE)-$(MY_LISP_VERSION_N)
	@echo " Created $(EXE)-$(MY_LISP_VERSION_N).tar.gz"

clean:
	$(Q)for file in $(CLEAN_LIST); do \
		if [ -e $$file ]; then \
		    echo " RM      $$file"; \
			rm -rf $$file; \
		fi \
	done

$(BIN):
	@echo " MKDIR   $@"
	$(Q)$(MKDIR) $@

%.o: %.c
	@echo " CC      $@"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

.%.d: %.c
	@echo " CCDEP   $@"
	$(Q)$(CC) -MM -MP -MF $@ $(CFLAGS) $< -MT ./$*.o -MT $@

%.c: %.l
	@echo " LEX     $@"
	$(Q)$(LEX) $(LFLAGS) -o $@ $<

src/%.tab.c include/%.tab.h: src/%.y
	@echo " YACC    src/$*.tab.c include/$*.tab.h"
	$(Q)$(YACC) $(YFLAGS) -d -b ./src/$* $<
	$(Q)mv ./src/$*.tab.h ./include/$*.tab.h

DEP_LIST := $(foreach dep,$(DEPS),$(dir $(dep)).$(notdir $(dep)))
DEP_LIST := $(DEP_LIST:.o=.d)

real-all: $(DEP_LIST)

ifeq (,$(filter $(MAKECMDGOALS),clean dist))
-include $(DEP_LIST)
endif
CLEAN_LIST += $(DEP_LIST)

