
objs-y += main.o
objs-y += ast.o
objs-y += vm.o
objs-y += scope.o
objs-y += label_table.o

objs-y += op.o

objs-y += hashtable.o

clean-list-y += parser.tab.c
CLEAN_LIST += $(tree-include)/parser.tab.h

# Add extra dependency information
#
# This forces make to generate the necessary parser headers before attempting
# to create the dependecy files.

$(tree)/parser.tab.c: $(tree)/parser.y
$(tree)/parser.tab.h: $(tree)/parser.y
include/ast.h: $(tree)/parser.y
include/ast.h: $(tree-include)/parser.tab.h

$(tree)/.main.d: $(tree-include)/parser.tab.h
$(tree)/.lexer.d: $(tree-include)/parser.tab.h

objs-y += parser.tab.o
objs-y += lexer.o
clean-list-y += lexer.c

# Tell make not to delete the temporary parser files.
.SECONDARY: $(patsubst %,$(tree)/%,$(clean-list-y))

