/* ********************************************************************** */
/**
 * @brief   Mini calculator (Test project for Lex & Yacc).
 * @author  eel3
 * @date    2015/05/08
 *
 * @par Compilers
 *   - GCC 4.6.3, Debian 4.6.3-14+rpi1 (Raspbian)
 *   - LLVM 6.0, clang-600.0.56, Xcode 6.1.1 (Mac OS X 10.9.5)
 *   - TDM-GCC 4.8.1 (Windows 7 64bit SP1)
 *   - Visual Studio 2013 Professional with Update 4, 64bit build (Windows 7 64bit SP1)
 */
/* ********************************************************************** */


#if defined(_WIN32) || defined(_WIN64)
#	if !defined(__MINGW32__) && !defined(__MINGW64__)
#		ifndef _CRT_SECURE_NO_WARNINGS
#			define _CRT_SECURE_NO_WARNINGS
#		endif /* ndef _CRT_SECURE_NO_WARNINGS */
#	endif /* !defined(__MINGW32__) && !defined(__MINGW64__) */
#	define PATH_SEP '\\'
#else /* defined(_WIN32) || defined(_WIN64) */
#	define PATH_SEP '/'
#endif /* defined(_WIN32) || defined(_WIN64) */

#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "y.tab.h"
#include "mc.h"


/* ---------------------------------------------------------------------- */
/* Variable */
/* ---------------------------------------------------------------------- */

/** Line number counter */
long line_number = 1;

/** Program name. */
static const char *progname;

/**  */
static jmp_buf jmpbuf;


/* ---------------------------------------------------------------------- */
/* Function-like macro */
/* ---------------------------------------------------------------------- */

/* ====================================================================== */
/**
 * @brief  Return true if string s1 equal to s2.
 *
 * @param[in] s1  string 1.
 * @param[in] s2  string 2.
 *
 * @retval !=0  Equal.
 * @retval   0  Not equal.
 */
/* ====================================================================== */
#define  STREQ(s1, s2)  (((s1)[0] == (s2)[0]) && (strcmp((s1), (s2)) == 0))


/* ---------------------------------------------------------------------- */
/* Function */
/* ---------------------------------------------------------------------- */

/* ********************************************************************** */
/**
 * @brief  Show warning message.
 *
 * @param[in] *s  
 * @param[in] *t  Additional message.
 */
/* ********************************************************************** */
void
warning(const char * const s, const char * const t)
{
	FILE * const out = stderr;

	assert(s != NULL);

	(void) fprintf(out, "%s: %s", progname, s);
	if (t != NULL) {
		(void) fprintf(out, " %s", t);
	}
	(void) fprintf(out, " near line %ld\n", line_number);
}

/* ********************************************************************** */
/**
 * @brief  Show runtime error message.
 *
 * @param[in] *s  
 * @param[in] *t  Additional message.
 */
/* ********************************************************************** */
void
execerror(const char * const s, const char * const t)
{
	warning(s, t);
	longjmp(jmpbuf, 0);
}

/* ********************************************************************** */
/**
 * @brief  Definition of yyerror.
 *
 * @param[in] *s  Message.
 */
/* ********************************************************************** */
void
yyerror(const char * const s)
{
	warning(s, NULL);
}

/* ====================================================================== */
/**
 * @brief  Tiny copy of basename(3).
 *
 * @param[in] *name  (See man 3 basename).
 *
 * @return  (See man 3 basename).
 */
/* ====================================================================== */
static const char *
my_basename(const char * const name)
{
	const char *bn;

	assert(name != NULL);

	bn = strrchr(name, PATH_SEP);
	return (bn == NULL) ? name : bn+1;
}

/* ====================================================================== */
/**
 * @brief  Tiny copy of basename(3).
 *
 * @param[in] signum  
 */
/* ====================================================================== */
static void
fpecatch(int signum)
{
	(void) signum;
	execerror("floating point exception", NULL);
}

/* ********************************************************************** */
/**
 * @brief  Main routine.
 *
 * @retval EXIT_SUCCESS  OK (success).
 * @retval EXIT_FAILURE  NG (failure).
 */
/* ********************************************************************** */
int
main(int argc, char *argv[])
{
	extern FILE *yyin;  /* from lexer.c (generated from lexer.l) */
#if YYDEBUG
	extern int yydebug; /* from lexer.c (generated from lexer.l) */
	yydebug = 1;
#endif /* YYDEBUG */

	progname = my_basename(argv[0]);

	if (argc >= 2) {
		FILE *in;

		if (STREQ(argv[1], "-")) {
			/*EMPTY*/
		} else if (errno = 0, (in = fopen(argv[1], "r")) == NULL) {
			perror(argv[1]);
			return EXIT_FAILURE;
		} else {
			yyin = in;
		}
	}

	setjmp(jmpbuf);
	signal(SIGFPE, fpecatch);
	yyparse();

	if (yyin != stdin) {
		(void) fclose(yyin);
	}

	return EXIT_SUCCESS;
}
