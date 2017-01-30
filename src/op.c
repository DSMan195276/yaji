
#include "ast.h"

#include "compare_op.h"
#include "logical_op.h"
#include "math_op.h"
#include "console_op.h"
#include "control_flow_op.h"
#include "stack_op.h"
#include "subroutine_op.h"

#include "op.h"

jaz_op_callback_t jaz_op_lookup_table[] = {
    ['='] = jaz_op_equal,
    ['<'] = jaz_op_less_than,
    ['>'] = jaz_op_greater_than,
    [TOK_NOT_EQUAL] = jaz_op_not_equal,
    [TOK_LESS_THAN_EQUAL] = jaz_op_less_than_equal,
    [TOK_GREATER_THAN_EQUAL] = jaz_op_greater_than_equal,

    [TOK_SHOW] = jaz_op_show,
    [TOK_PRINT] = jaz_op_print,

    [TOK_GOTO] = jaz_op_goto,
    [TOK_GOFALSE] = jaz_op_gofalse,
    [TOK_GOTRUE] = jaz_op_gotrue,
    [TOK_HALT] = jaz_op_halt,
    [TOK_LABEL] = jaz_op_label,

    ['&'] = jaz_op_logic_and,
    ['|'] = jaz_op_logic_or,
    ['!'] = jaz_op_logic_not,

    ['-'] = jaz_op_minus,
    ['+'] = jaz_op_plus,
    ['*'] = jaz_op_mult,
    ['/'] = jaz_op_div,
    [TOK_MOD] = jaz_op_mod,

    [TOK_PUSH] = jaz_op_push,
    [TOK_POP] = jaz_op_pop,
    [TOK_RVALUE] = jaz_op_rvalue,
    [TOK_LVALUE] = jaz_op_lvalue,
    [TOK_ASSIGN] = jaz_op_assign,
    [TOK_COPY] = jaz_op_copy,

    [TOK_BEGIN] = jaz_op_begin,
    [TOK_END] = jaz_op_end,
    [TOK_RETURN] = jaz_op_return,
    [TOK_CALL] = jaz_op_call,
};

