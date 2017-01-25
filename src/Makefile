
objs-y += main.o
objs-y += ast.o

clean-list-y += parser.tab.c
clean-list-y += parser.tab.h

# Add extra dependency information
#
# This forces make to generate the necessary parser headers before attempting
# to create the dependecy files.

$(tree)/parser.tab.c: $(tree)/parser.y
$(tree)/parser.tab.h: $(tree)/parser.y

$(tree)/.main.d: $(tree)/parser.tab.h
$(tree)/.lexer.d: $(tree)/parser.tab.h

objs-y += parser.tab.o
objs-y += lexer.o
clean-list-y += lexer.c

# Tell make not to delete the temporary parser files.
.SECONDARY: $(patsubst %,$(tree)/%,$(clean-list-y))


