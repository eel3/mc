/* ********************************************************************** */
/**
 * @brief   Mini calculator (Test project for Lex & Yacc).
 * @author  eel3
 * @date    2015/05/08
 */
/* ********************************************************************** */


#ifndef MINICALC_H_INCLUDED
#define MINICALC_H_INCLUDED


/* ---------------------------------------------------------------------- */
/* Variable */
/* ---------------------------------------------------------------------- */

/** Line number counter */
extern long line_number;


/* ---------------------------------------------------------------------- */
/* Function */
/* ---------------------------------------------------------------------- */

/* ********************************************************************** */
/**
 * @brief  Show warning message.
 *
 * @param[in] *s  Message.
 * @param[in] *t  Additional message.
 */
/* ********************************************************************** */
extern void
warning(const char * const s, const char * const t);

/* ********************************************************************** */
/**
 * @brief  Show runtime error message.
 *
 * @param[in] *s  Message.
 * @param[in] *t  Additional message.
 */
/* ********************************************************************** */
extern void
execerror(const char * const s, const char * const t);

/* ********************************************************************** */
/**
 * @brief  Definition of yyerror.
 *
 * @param[in] *s  Message.
 */
/* ********************************************************************** */
extern void
yyerror(const char * const s);


#endif /* ndef MINICALC_H_INCLUDED */
