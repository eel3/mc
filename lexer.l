%{
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "y.tab.h"
#include "mc.h"

#define CONV_NUM(text, base) \
	if (to_i32(text, base, &yylval)) { \
		return NUMBER; \
	} else { \
		yyerror("invalid number format"); \
		return ERROR; \
	}

static bool
to_i32(const char *s, int base, int32_t *retval)
{
	long n;
	char *endp;

	assert((s != NULL) && (s[0] != '\0')
	       && ((base == 0) || ((base >= 2) && (base <= 36)))
	       && (retval != NULL));

	errno = 0;
	n = strtol(s, &endp, base);
	if ((n == 0) && ((const char *) endp == s) && (errno == ERANGE)) {
		return false;
	}
	if (((n == LONG_MAX) || (n == LONG_MIN)) && (errno == ERANGE)) {
		return false;
	}
	if (*endp != '\0') {
		return false;
	}

#if INT32_MAX < LONG_MAX || INT32_MIN > LONG_MIN
	/* check if int32_t < long */
	if ((n < INT32_MIN) || (n > INT32_MAX)) {
		return false;
	}
#endif

	*retval = (int32_t) n;

	return true;
}
%}

%%
0[bB][0-1]+         CONV_NUM(&yytext[2], 2)
0[0-7]+             CONV_NUM(yytext, 8)
0|[1-9][0-9]*       CONV_NUM(yytext, 10)
0[xX][0-9a-fA-F]+   CONV_NUM(yytext, 16)

"%"             return MOD;
"*"             return MUL;
"**"            return POW;
"+"             return ADD;
"-"             return SUB;
"/"             return DIV;

"&"             return BITAND;
"<<"            return LSHIFT;
">>"            return RSHIFT;
"^"             return BITXOR;
"|"             return BITOR;
"~"             return BITNOT;

"!"             return NOT;
"&&"            return AND;
"||"            return OR;

"!="            return NE;
"<"             return LT;
"<="            return LE;
"=="            return EQ;
">"             return GT;
">="            return GE;

"("             return LPAR;
")"             return RPAR;

#[^\n]*         /* comment */
[ \t]*          /* white space */

"\n"            line_number++; return EOS;

.               yyerror("illegal character"); return ERROR;
%%
int yywrap(void)
{
	return 1;
}
