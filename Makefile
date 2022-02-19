# @brief   mc Makefile for Unix like environment.
# @author  eel3
# @date    2022-02-11
#
# @note
# - GCC 4.2.1 (Xcode 4.6.1) on Mac OS X 10.8.4
# - GCC 4.6.3 on Ubuntu 12.04 LTS 32bit

app        := mc
objs       := $(app).o lexer.o parser.o

CFLAGS     += -Wall -std=c99 -pedantic
CPPFLAGS   += -D YYDEBUG=0 -D YYERROR_VERBOSE # -D YY_NO_UNISTD_H
LDLIBS     := -lm

LEX        := flex
YACC       := bison
YFLAGS     := -d -y

.PHONY: all
all: $(app)

$(app): $(objs)
$(app).o: $(app).c $(app).h parser.c y.tab.h
lexer.o: CFLAGS += -Wno-unused-function -Wno-implicit-function-declaration
lexer.o: lexer.c $(app).h parser.c y.tab.h
parser.o: parser.c $(app).h
y.tab.h: parser.y

.PHONY: clean
clean:
	$(RM) $(app) $(objs) lexer.c parser.c y.tab.h
