/******************************************************************************
Copyright (c) 1999 Daniel Stenberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "fcpp.h"

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>


#ifdef EMACS

/* Use the Emacs config file to find out what type of machine */

#define NO_SHORTNAMES

/* Convert Emacs's conventions for BIG_ENDIAN to cpp's convention.  */
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#define BIG_ENDIAN TRUE
#else /* not BIG_ENDIAN */
#define BIG_ENDIAN FALSE
#endif /* BIG_ENDIAN */

/* Emacs uses the names index and rindex and defines them as str(r)chr if nec;
   cpp uses the opposite convention.  Here we flush the macro definitions for
   Emacs and add the ones cpp wants.  */

#ifdef index
#undef index
#undef rindex
#else /* index is not defined as a macro */
#define strchr index
#define strrchr rindex
#endif /* index is not defined as a macro */

#define NBUFF 2048
#define NWORK 2048

#endif /* EMACS */

/*
 *		   S y s t e m	 D e p e n d e n t
 *		D e f i n i t i o n s	 f o r	 C P P
 *
 * Definitions in this file may be edited to configure CPP for particular
 * host operating systems and target configurations.
 *
 * NOTE: cpp assumes it is compiled by a compiler that supports macros
 * with arguments.  If this is not the case (as for Decus C), #define
 * nomacarg -- and provide function equivalents for all macros.
 *
 * cpp also assumes the host and target implement the Ascii character set.
 * If this is not the case, you will have to do some editing here and there.
 */

/*
 * This redundant definition of TRUE and FALSE works around
 * a limitation of Decus C.
 */
#ifndef TRUE
#define TRUE			1
#define FALSE			0
#endif

/*
 * Define the HOST operating system.  This is needed so that
 * cpp can use appropriate filename conventions.
 */
#define SYS_UNKNOWN		0
#define SYS_UNIX		1
#define SYS_VMS 		2
#define SYS_RSX 		3
#define SYS_RT11		4
#define SYS_LATTICE		5
#define SYS_ONYX		6
#define SYS_68000		7
#define SYS_AMIGADOS		8

#ifndef HOST
#ifdef	unix
#define HOST			SYS_UNIX
#else
#ifdef	amiga
#define HOST			SYS_AMIGADOS
#endif
#endif
#endif

/*
 * We assume that the target is the same as the host system
 */
#ifndef TARGET
#define TARGET			HOST
#endif

/*
 * In order to predefine machine-dependent constants,
 * several strings are defined here:
 *
 * MACHINE	defines the target cpu (by name)
 * SYSTEM	defines the target operating system
 * COMPILER	defines the target compiler
 *
 *	The above may be #defined as "" if they are not wanted.
 *	They should not be #defined as NULL.
 *
 * LINE_PREFIX	defines the # output line prefix, if not "line"
 *		This should be defined as "" if cpp is to replace
 *		the "standard" C pre-processor.
 */
#define LINE_PREFIX		"line"
/*
 * FILE_LOCAL	marks functions which are referenced only in the
 *		file they reside.  Some C compilers allow these
 *		to be marked "static" even though they are referenced
 *		by "extern" statements elsewhere.
 *
 * OK_DOLLAR	Should be set TRUE if $ is a valid alphabetic character
 *		in identifiers (default), or zero if $ is invalid.
 *		Default is TRUE.
 *
 * OK_CONCAT	Should be set TRUE if # may be used to concatenate
 *		tokens in macros (per the Ansi Draft Standard) or
 *		FALSE for old-style # processing (needed if cpp is
 *		to process assembler source code).
 */
#define OK_CONCAT TRUE
/*
 * OK_DATE	Predefines the compilation date if set TRUE.
 *		Not permitted by the Nov. 12, 1984 Draft Standard.
 */
#define OK_DATE TRUE
/*
 *
 * OK_SIZEOF	Permits sizeof in #if preprocessor expressions.
 *		According to K&R V2 (page 232), this is not allowed.
 */
#define OK_SIZEOF TRUE
/*
 * S_CHAR etc.	Define the sizeof the basic TARGET machine word types.
 *		By default, sizes are set to the values for the HOST
 *		computer.  If this is inappropriate, see the code in
 *		cpp3.c for details on what to change.  Also, if you
 *		have a machine where sizeof (signed int) differs from
 *		sizeof (unsigned int), you will have to edit code and
 *		tables in cpp3.c (and extend the -S option definition.)
 *
 * CPP_LIBRARY	May be defined if you have a site-specific include directory
 *		which is to be searched *before* the operating-system
 *		specific directories.
 */

#define MACHINE 		"amiga", "m68000"
#define SYSTEM			"amigados"


/*
 * defaults
 */

#ifndef MSG_PREFIX
#define MSG_PREFIX		"cpp: "
#endif

/*
 * OLD_PREPROCESSOR forces the definition of OK_DOLLAR, OK_CONCAT,
 * COMMENT_INVISIBLE to values appropriate for
 * an old-style preprocessor.
 */

#if	OLD_PREPROCESSOR
#define OK_DOLLAR		FALSE
#define OK_CONCAT		FALSE
#define COMMENT_INVISIBLE	TRUE
#endif

/*
 * RECURSION_LIMIT may be set to -1 to disable the macro recursion test.
 */
#ifndef RECURSION_LIMIT
#define RECURSION_LIMIT 1000
#endif

/*
 * BITS_CHAR may be defined to set the number of bits per character.
 * it is needed only for multi-byte character constants.
 */
#ifndef BITS_CHAR
#define BITS_CHAR		8
#endif

/*
 * BIG_ENDIAN is set TRUE on machines (such as the IBM 360 series)
 * where 'ab' stores 'a' in the high-bits and 'b' in the low-bits.
 * It is set FALSE on machines (such as the PDP-11 and Vax-11)
 * where 'ab' stores 'a' in the low-bits and 'b' in the high-bits.
 * (Or is it the other way around?) -- Warning: BIG_ENDIAN code is untested.
 * [I *seems* to be the other way around, according to the code /OIS]
 */
#ifndef BIG_ENDIAN
#define BIG_ENDIAN		FALSE
#endif

/*
 * COMMENT_INVISIBLE may be defined to allow "old-style" comment
 * processing, whereby the comment becomes a zero-length token
 * delimiter.  This permitted tokens to be concatenated in macro
 * expansions.	This was removed from the Draft Ansi Standard.
 */
#ifndef COMMENT_INVISIBLE
#define COMMENT_INVISIBLE	FALSE
#endif

/*
 * OK_DOLLAR enables use of $ as a valid "letter" in identifiers.
 * This is a permitted extension to the Ansi Standard and is required
 * for e.g., VMS, RSX-11M, etc.   It should be set FALSE if cpp is
 * used to preprocess assembler source on Unix systems.  OLD_PREPROCESSOR
 * sets OK_DOLLAR FALSE for that reason.
 */
#ifndef OK_DOLLAR
#define OK_DOLLAR		TRUE
#endif

/*
 * OK_CONCAT enables (one possible implementation of) token concatenation.
 * If cpp is used to preprocess Unix assembler source, this should be
 * set FALSE as the concatenation character, #, is used by the assembler.
 */
#ifndef OK_CONCAT
#define OK_CONCAT		TRUE
#endif

/*
 * OK_DATE may be enabled to predefine today's date as a string
 * at the start of each compilation.  This is apparently not permitted
 * by the Draft Ansi Standard.
 */
#ifndef OK_DATE
#define OK_DATE 	TRUE
#endif

/*
 * OK_SIZEOF may be defined to allow sizeof(type) in #if expressions.
 * Actually, it is none of the preprocessors business how large these
 * things are, as they might be different with different compiler
 * options. Also, according to K&R V2, page 232, it is nonstandard.
 * This option was added in the PDC process, under no. *OIS*0.92*.
 */
#ifndef OK_SIZEOF
#define OK_SIZEOF	FALSE
#endif

/*
 * Some common definitions.
 */

#ifndef DEBUG
#define DEBUG		FALSE
#endif

/*
 * The following definitions are used to allocate memory for
 * work buffers.  In general, they should not be modified
 * by implementors.
 *
 * PAR_MAC	The maximum number of #define parameters (31 per Standard)
 *		Note: we need another one for strings.
 * NBUFF	Input buffer size
 * NWORK	Work buffer size -- the longest macro
 *		must fit here after expansion.
 * NEXP 	The nesting depth of #if expressions
 * NINCLUDE	The number of directories that may be specified
 *		on a per-system basis, or by the -I option.
 * BLK_NEST	The number of nested #if's permitted.
 */

#ifndef PAR_MAC
#define PAR_MAC 	   (31 + 1)
#endif

#ifndef NBUFF
#define NBUFF			512
#endif

#ifndef NWORK
#define NWORK			512
#endif

#ifndef NEXP
#define NEXP			128
#endif

#ifndef NINCLUDE
#define NINCLUDE		  20
#endif

#ifndef NPARMWORK
#define NPARMWORK		(NWORK * 2)
#endif

#ifndef BLK_NEST
#define BLK_NEST		32
#endif


/*
 * Some special constants.  These may need to be changed if cpp
 * is ported to a wierd machine.
 *
 * NOTE: if cpp is run on a non-ascii machine, ALERT and VT may
 * need to be changed.	They are used to implement the proposed
 * ANSI standard C control characters '\a' and '\v' only.
 * DEL is used to tag macro tokens to prevent #define foo foo
 * from looping.  Note that we don't try to prevent more elaborate
 * #define loops from occurring.
 */

#ifndef ALERT
#define ALERT			'\007'          /* '\a' is "Bell"       */
#endif

#ifndef VT
#define VT			'\013'          /* Vertical Tab CTRL/K  */
#endif

/*
 * Functions can be declared/defined static to only become in the
 * scope for functions of the same source file.
 */

#ifndef FILE_LOCAL
#define FILE_LOCAL		static		/* file-unique globals */
#endif

/*
 * For compilers supporting inlining, the INLINE macro has been added to
 * functions called from only one place. There might still be some
 * functions that should have this macro.
 */
#ifdef AMIGA
#define INLINE __inline /* Amiga compiler SAS/C 6.x supports this! */
#else
#define INLINE /* don't support that kind of stuff */
#endif

#if defined(AMIGA) && defined(SHARED)
#define PREFIX __asm __saveds
#define REG(x) register __ ## x
#else
#define PREFIX
#define REG(x)
#endif

/*
 * SBSIZE defines the number of hash-table slots for the symbol table.
 */
#ifndef SBSIZE
#define SBSIZE  64
#endif

#define VERSION_TEXT "Frexx C Preprocessor v1.5.1 " \
"Copyright (C) by FrexxWare 1993 - 2002.\n" \
"Compiled " __DATE__ "\n"

/*
 *	I n t e r n a l   D e f i n i t i o n s    f o r   C P P
 *
 * In general, definitions in this file should not be changed.
 */

#include <stdlib.h>
#include <string.h>
#ifndef toupper
#define toupper(c) ((c) + ('A' - 'a'))
#endif /* no toupper */
#ifndef tolower
#define tolower(c) ((c) + ('a' - 'A'))
#endif /* no tolower */

#ifndef TRUE
#define TRUE		1
#define FALSE		0
#endif
#ifndef EOS
/*
 * This is predefined in Decus C
 */
#define EOS		'\0'            /* End of string                */
#endif
#define EOF_CHAR	0		/* Returned by get() on eof     */
#define NULLST		((char *) NULL) /* Pointer to nowhere (linted)  */
#define DEF_NOARGS	(-1)            /* #define foo vs #define foo() */

/*
 * The following may need to change if the host system doesn't use ASCII.
 */
#define QUOTE_PARM	0x1C		/* Magic quoting operator	*/
#define DEF_MAGIC	0x1D		/* Magic for #defines		*/
#define TOK_SEP 	0x1E		/* Token concatenation delim.	*/
#define COM_SEP 	0x1F		/* Magic comment separator	*/

/*
 * Note -- in Ascii, the following will map macro formals onto DEL + the
 * C1 control character region (decimal 128 .. (128 + PAR_MAC)) which will
 * be ok as long as PAR_MAC is less than 33).  Note that the last PAR_MAC
 * value is reserved for string substitution.
 */

#define MAC_PARM	0x7F		/* Macro formals start here	*/
#ifndef OS9
#if (PAR_MAC >= 33)
#error	"assertion fails -- PAR_MAC isn't less than 33"

#endif
#endif
#define LASTPARM	(PAR_MAC - 1)

/*
 * Character type codes.
 */

#define INV		0		/* Invalid, must be zero	*/
#define OP_EOE		INV		/* End of expression		*/
#define DIG		1		/* Digit			*/
#define LET		2		/* Identifier start		*/
#define FIRST_BINOP	OP_ADD
#define OP_ADD		3
#define OP_SUB		4
#define OP_MUL		5
#define OP_DIV		6
#define OP_MOD		7
#define OP_ASL		8
#define OP_ASR		9
#define OP_AND		10		/* &, not &&			*/
#define OP_OR		11		/* |, not ||			*/
#define OP_XOR		12
#define OP_EQ		13
#define OP_NE		14
#define OP_LT		15
#define OP_LE		16
#define OP_GE		17
#define OP_GT		18
#define OP_ANA		19		/* &&				*/
#define OP_ORO		20		/* ||				*/
#define OP_QUE		21		/* ?				*/
#define OP_COL		22		/* :				*/
#define OP_CMA		23		/* , (relevant?)                */
#define LAST_BINOP	OP_CMA		/* Last binary operand		*/
/*
 * The following are unary.
 */
#define FIRST_UNOP	OP_PLU		/* First Unary operand		*/
#define OP_PLU		24		/* + (draft ANSI standard)      */
#define OP_NEG		25		/* -				*/
#define OP_COM		26		/* ~				*/
#define OP_NOT		27		/* !				*/
#define LAST_UNOP	OP_NOT
#define OP_LPA		28		/* (                            */
#define OP_RPA		29		/* )				*/
#define OP_END		30		/* End of expression marker	*/
#define OP_MAX		(OP_END + 1)    /* Number of operators          */
#define OP_FAIL 	(OP_END + 1)    /* For error returns            */

/*
 * The following are for lexical scanning only.
 */

#define QUO		65		/* Both flavors of quotation	*/
#define DOT		66		/* . might start a number	*/
#define SPA		67		/* Space and tab		*/
#define BSH		68		/* Just a backslash		*/
#define END		69		/* EOF				*/

/*
 * These bits are set in ifstack[]
 */
#define WAS_COMPILING	1		/* TRUE if compile set at entry */
#define ELSE_SEEN	2		/* TRUE when #else processed	*/
#define TRUE_SEEN	4		/* TRUE when #if TRUE processed */

/*
 * Define bits for the basic types and their adjectives
 */

#define T_CHAR		  1
#define T_INT		  2
#define T_FLOAT 	  4
#define T_DOUBLE	  8
#define T_SHORT 	 16
#define T_LONG		 32
#define T_SIGNED	 64
#define T_UNSIGNED	128
#define T_PTR		256		/* Pointer			*/
#define T_FPTR		512		/* Pointer to functions 	*/

/*
 * The DEFBUF structure stores information about #defined
 * macros.  Note that the defbuf->repl information is always
 * in malloc storage.
 */

typedef struct defbuf {
	struct defbuf	*link;		/* Next define in chain */
	char		*repl;		/* -> replacement	*/
	int		hash;		/* Symbol table hash	*/
	int		nargs;		/* For define(args)     */
	char		name[1];	/* #define name 	*/
} DEFBUF;

/*
 * The FILEINFO structure stores information about open files
 * and macros being expanded.
 */

typedef struct fileinfo {
	char		*bptr;		/* Buffer pointer	*/
	int		line;		/* for include or macro */
	FILE		*fp;		/* File if non-null	*/
	struct fileinfo *parent;	/* Link to includer	*/
	char		*filename;	/* File/macro name	*/
	char		*progname;	/* From #line statement */
	unsigned int	unrecur;	/* For macro recursion	*/
	char		buffer[1];	/* current input line	*/
} FILEINFO;

/*
 * The SIZES structure is used to store the values for #if sizeof
 */

typedef struct sizes {
    short	bits;			/* If this bit is set,		*/
    short	size;			/* this is the datum size value */
    short	psize;			/* this is the pointer size	*/
} SIZES;
/*
 * nomacarg is a built-in #define on Decus C.
 */

#ifdef	nomacarg
#define cput		output		/* cput concatenates tokens	*/
#else
#if COMMENT_INVISIBLE
#define cput(c)         { if (c != TOK_SEP && c != COM_SEP) putchar(c); }
#else
#define cput(c)         { if (c != TOK_SEP) putchar(c); }
#endif
#endif

#ifndef nomacarg
#define streq(s1, s2)   (strcmp(s1, s2) == 0)
#endif

/*
 * Note: IO_NORMAL and IO_ERROR are defined in the Decus C stdio.h file
 */
#ifndef IO_NORMAL
#define IO_NORMAL	0
#endif
#ifndef IO_ERROR
#define IO_ERROR	1
#endif

/*
 * Externs
 */

// @donw
#ifdef AMIGA
#include <dos.h>
extern int _OSERR;
#endif

extern char	type[]; 		/* Character classifier 	*/

#define compiling global->ifstack[0]
#if	DEBUG
extern int	debug;			/* Debug level			*/
#endif
extern SIZES	size_table[];		/* For #if sizeof sizes 	*/

#define MAX_SPACE_SIZE 512 /* maximum number of whitespaces possible
                              to remember */

struct Global {

  /*
   * Commonly used global variables:
   * line       is the current input line number.
   * wrongline  is set in many places when the actual output
   *            line is out of sync with the numbering, e.g,
   *            when expanding a macro with an embedded newline.
   *
   * tokenbuf   holds the last identifier scanned (which might
   *            be a candidate for macro expansion).
   * errors     is the running cpp error counter.
   * infile     is the head of a linked list of input files (extended by
   *            #include and macros being expanded).  infile always points
   *            to the current file/macro.  infile->parent to the includer,
   *            etc.  infile->fd is NULL if this input stream is a macro.
   */
  int           line;                   /* Current line number          */
  int           wrongline;              /* Force #line to compiler      */
  char          *tokenbuf;              /* Buffer for current input token */
  char      *functionname;  /* Buffer for current function */
  int       funcline;       /* Line number of current function */
  int           tokenbsize;             /* Allocated size of tokenbuf,  */
                                        /* not counting zero at end.    */
  int           errors;                 /* cpp error counter            */
  FILEINFO      *infile;                /* Current input file           */
#if DEBUG
  int           debug;                  /* TRUE if debugging now        */
#endif
  /*
   * This counter is incremented when a macro expansion is initiated.
   * If it exceeds a built-in value, the expansion stops -- this tests
   * for a runaway condition:
   *    #define X Y
   *    #define Y X
   *    X
   * This can be disabled by falsifying rec_recover.  (Nothing does this
   * currently: it is a hook for an eventual invocation flag.)
   */
  int           recursion;              /* Infinite recursion counter   */
  int           rec_recover;            /* Unwind recursive macros      */

  /*
   * instring is set TRUE when a string is scanned.  It modifies the
   * behavior of the "get next character" routine, causing all characters
   * to be passed to the caller (except <DEF_MAGIC>).  Note especially that
   * comments and \<newline> are not removed from the source.  (This
   * prevents cpp output lines from being arbitrarily long).
   *
   * inmacro is set by #define -- it absorbs comments and converts
   * form-feed and vertical-tab to space, but returns \<newline>
   * to the caller.  Strictly speaking, this is a bug as \<newline>
   * shouldn't delimit tokens, but we'll worry about that some other
   * time -- it is more important to prevent infinitly long output lines.
   *
   * instring and inmarcor are parameters to the get() routine which
   * were made global for speed.
   */
  int           instring;       /* TRUE if scanning string      */
  int           inmacro;        /* TRUE if #defining a macro    */
  
  /*
   * work[] and workp are used to store one piece of text in a temporay
   * buffer.  To initialize storage, set workp = work.  To store one
   * character, call save(c);  (This will fatally exit if there isn't
   * room.)  To terminate the string, call save(EOS).  Note that
   * the work buffer is used by several subroutines -- be sure your
   * data won't be overwritten.  The extra byte in the allocation is
   * needed for string formal replacement.
   */
  char          work[NWORK + 1];        /* Work buffer                  */
  char          *workp;                 /* Work buffer pointer          */

  /*
   * keepcomments is set TRUE by the -C option.  If TRUE, comments
   * are written directly to the output stream.  This is needed if
   * the output from cpp is to be passed to lint (which uses commands
   * embedded in comments).  cflag contains the permanent state of the
   * -C flag.  keepcomments is always falsified when processing #control
   * commands and when compilation is supressed by a false #if
   *
   * If eflag is set, CPP returns "success" even if non-fatal errors
   * were detected.
   *
   * If nflag is non-zero, no symbols are predefined except __LINE__.
   * __FILE__, and __DATE__.  If nflag > 1, absolutely no symbols
   * are predefined.
   */
  char          keepcomments;   /* Write out comments flag      */
  char          cflag;          /* -C option (keep comments)    */
  char          eflag;          /* -E option (never fail)       */
  char          nflag;          /* -N option (no predefines)    */
  char          wflag;          /* -W option (write #defines)   */

  /*
   * ifstack[] holds information about nested #if's.  It is always
   * accessed via *ifptr.  The information is as follows:
   *    WAS_COMPILING   state of compiling flag at outer level.
   *    ELSE_SEEN       set TRUE when #else seen to prevent 2nd #else.
   *    TRUE_SEEN       set TRUE when #if or #elif succeeds
   * ifstack[0] holds the compiling flag.  It is TRUE if compilation
   * is currently enabled.  Note that this must be initialized TRUE.
   */
  char          ifstack[BLK_NEST];      /* #if information      */
  char          *ifptr;                 /* -> current ifstack[] */

  /*
   * incdir[] stores the -i directories (and the system-specific
   * #include <...> directories.
   */
  char  *incdir[NINCLUDE];              /* -i directories               */
  char  **incend;                       /* -> free space in incdir[]    */

  /*
   * include[] stores the -X and -x files.
   */
  char  *include[NINCLUDE];
  char  includeshow[NINCLUDE]; /* show it or not! */
  int  included;

  /*
   * This is the table used to predefine target machine and operating
   * system designators.        It may need hacking for specific circumstances.
   * Note: it is not clear that this is part of the Ansi Standard.
   * The -B option supresses preset definitions.
   */
  char  *preset[5];                     /* names defined at cpp start   */

  /*
   * The value of these predefined symbols must be recomputed whenever
   * they are evaluated.        The order must not be changed.
   */
  char  *magic[5];                      /* Note: order is important     */

  /*
   * This is the variable saying if Cpp should remove C++ style comments from
   * the output. Default is... TRUE, yes, pronto, do it!!!
   */
  
  char cplusplus;

  char *sharpfilename;


  /*
   * parm[], parmp, and parlist[] are used to store #define() argument
   * lists.  nargs contains the actual number of parameters stored.
   */
  char  parm[NPARMWORK + 1];    /* define param work buffer     */
  char  *parmp;                 /* Free space in parm           */
  char  *parlist[LASTPARM];     /* -> start of each parameter   */
  int   nargs;                  /* Parameters for this macro    */

  DEFBUF *macro;                /* Catches start of infinite macro      */

  DEFBUF *symtab[SBSIZE];       /* Symbol table queue headers   */

  int evalue;                   /* Current value from evallex() */

  char *(*input)(char *, int, void *);  /* Input function */

  char *first_file;             /* Preprocessed file. */

  void *userdata;               /* Data sent to input function */

  void (*output)(int, void *);  /* output function */

  void (*error)(void *, char *, va_list);       /* error function */

  char linelines;

  char warnillegalcpp; /* warn for illegal preprocessor instructions? */

  char outputLINE;   /* output 'line' in #line instructions */

  char showversion;  /* display version */

  char showincluded; /* display included files */

  char showbalance; /* display paren balance */

  char showspace;   /* display all whitespaces as they are */

  char comment;     /* TRUE if a comment just has been written to output */

  char *spacebuf;    /* Buffer to store whitespaces in if -H */

  long chpos;       /* Number of whitespaces in buffer */

  char nestcomments; /* Allow nested comments */

  char warnnestcomments; /* Warn at nested comments */

  char warnnoinclude; /* Warn at missing include file */

  char outputfile; /* output the main file */

  char out; /* should we output anything now? */

  char rightconcat; /* should the right part of a concatenation be avaluated
					   before the concat (TRUE) or after (FALSE) */
  char *initialfunc; /* file to include first in all functions */

  char *excludedinit[20]; /* functions (names) excluded from the initfunc */
  int excluded;

  char outputfunctions;  /* output all discovered functions to stderr! */

  char webmode; /* WWW process mode */

  char allowincludelocal;
};

typedef enum {
  ERROR_STRING_MUST_BE_IF,
  ERROR_STRING_MAY_NOT_FOLLOW_ELSE,
  ERROR_ERROR,
  ERROR_PREPROC_FAILURE,
  ERROR_MISSING_ARGUMENT,
  ERROR_INCLUDE_SYNTAX,
  ERROR_DEFINE_SYNTAX,
  ERROR_REDEFINE,
  ERROR_ILLEGAL_UNDEF,
  ERROR_RECURSIVE_MACRO,
  ERROR_EOF_IN_ARGUMENT,
  ERROR_MISPLACED_CONSTANT,
  ERROR_IF_OVERFLOW,
  ERROR_ILLEGAL_IF_LINE,
  ERROR_OPERATOR,
  ERROR_EXPR_OVERFLOW,
  ERROR_UNBALANCED_PARENS,
  ERROR_MISPLACED,
  ERROR_STRING_IN_IF,
  ERROR_DEFINED_SYNTAX,
  ERROR_ILLEGAL_ASSIGN,
  ERROR_ILLEGAL_BACKSLASH,
  ERROR_SIZEOF_SYNTAX,
  ERROR_SIZEOF_UNKNOWN,
  ERROR_SIZEOF_ILLEGAL_TYPE,
  ERROR_SIZEOF_NO_TYPE,
  ERROR_UNTERMINATED_STRING,
  ERROR_EOF_IN_COMMENT,
  ERROR_IFDEF_DEPTH,
  ERROR_ILLEGAL_CHARACTER,
  ERROR_ILLEGAL_CHARACTER2,
  ERROR_SIZEOF_BUG,
  ERROR_IF_OPERAND,
  ERROR_STRANG_CHARACTER,
  ERROR_STRANG_CHARACTER2,

  BORDER_ERROR_WARN, /* below this number: errors, above: warnings */

  WARN_CONTROL_LINE_IN_MACRO,
  WARN_ILLEGAL_COMMAND,
  WARN_UNEXPECTED_TEXT_IGNORED,
  WARN_TOO_FEW_VALUES_TO_SIZEOF,
  WARN_TOO_MANY_VALUES_TO_SIZEOF,
  WARN_NOT_DEFINED,
  WARN_INTERNAL_ERROR,
  WARN_MACRO_NEEDS_ARGUMENTS,
  WARN_WRONG_NUMBER_ARGUMENTS,
  WARN_DIVISION_BY_ZERO,
  WARN_ILLEGAL_OCTAL,
  WARN_MULTIBYTE_NOT_PORTABLE,
  WARN_CANNOT_OPEN_INCLUDE,
  WARN_BRACKET_DEPTH,
  WARN_PAREN_DEPTH,
  WARN_BRACE_DEPTH,
  WARN_NESTED_COMMENT,

  BORDER_WARN_FATAL, /* below this number: warnings, above: fatals */

  FATAL_TOO_MANY_NESTINGS,
  FATAL_FILENAME_BUFFER_OVERFLOW,
  FATAL_TOO_MANY_INCLUDE_DIRS,
  FATAL_TOO_MANY_INCLUDE_FILES,
  FATAL_TOO_MANY_ARGUMENTS_MACRO,
  FATAL_MACRO_AREA_OVERFLOW,
  FATAL_ILLEGAL_MACRO,
  FATAL_TOO_MANY_ARGUMENTS_EXPANSION,
  FATAL_OUT_OF_SPACE_IN_ARGUMENT,
  FATAL_WORK_AREA_OVERFLOW,
  FATAL_WORK_BUFFER_OVERFLOW,
  FATAL_OUT_OF_MEMORY,
  FATAL_TOO_MUCH_PUSHBACK


  } ErrorCode;

/**********************************************************************
 * RETURN CODES:
 *********************************************************************/

typedef enum {
  FPP_OK,
  FPP_OUT_OF_MEMORY,
  FPP_TOO_MANY_NESTED_STATEMENTS,
  FPP_FILENAME_BUFFER_OVERFLOW,
  FPP_NO_INCLUDE,
  FPP_OPEN_ERROR,
  FPP_TOO_MANY_ARGUMENTS,
  FPP_WORK_AREA_OVERFLOW,
  FPP_ILLEGAL_MACRO,
  FPP_EOF_IN_MACRO,
  FPP_OUT_OF_SPACE_IN_MACRO_EXPANSION,
  FPP_ILLEGAL_CHARACTER,
  FPP_CANT_USE_STRING_IN_IF,
  FPP_BAD_IF_DEFINED_SYNTAX,
  FPP_IF_ERROR,
  FPP_SIZEOF_ERROR,
  FPP_UNTERMINATED_STRING,
  FPP_TOO_MANY_INCLUDE_DIRS,
  FPP_TOO_MANY_INCLUDE_FILES,
  FPP_INTERNAL_ERROR,

  FPP_LAST_ERROR
} ReturnCode;

/* Nasty defines to make them appear as three different functions! */
#define cwarn cerror
#define cfatal cerror 


/**********************************************************************
 * PROTOTYPES:
 *********************************************************************/
struct fppTag;
int fppPreProcess(struct fppTag *);
void Freemem(void *);
void Error(struct Global *, char *, ...);
void Putchar(struct Global *, int);
void Putstring(struct Global *, char *);
void Putint(struct Global *, int);
char *savestring(struct Global *, char *);
ReturnCode addfile(struct Global *, FILE *, char *);
int catenate(struct Global *, int lhs_number, ReturnCode *);
void cerror(struct Global *, ErrorCode, ...);
ReturnCode control(struct Global *, int *);
ReturnCode dodefine(struct Global *);
int dooptions(struct Global *, struct fppTag *);
void doundef(struct Global *);
void dumpparm(char *);
ReturnCode expand(struct Global *, DEFBUF *);
int get(struct Global *);
ReturnCode initdefines(struct Global *);
void outdefines(struct Global *);
ReturnCode save(struct Global *, int);
void scanid(struct Global *, int);
ReturnCode scannumber(struct Global *, int, ReturnCode(*)(struct Global *, int));
ReturnCode scanstring(struct Global *, int, ReturnCode(*)(struct Global *, int));
void unget(struct Global *);
ReturnCode ungetstring(struct Global *, char *);
ReturnCode eval(struct Global *, int *);
#ifdef  DEBUG_EVAL
void dumpstack(OPTAB[NEXP], register OPTAB *, int [NEXP], register int *);
#endif
void skipnl(struct Global *);
int skipws(struct Global *);
ReturnCode macroid(struct Global *, int *);
ReturnCode getfile(struct Global *, int, char *, FILEINFO **);
DEFBUF *lookid(struct Global *, int );
DEFBUF *defendel(struct Global *, char *, int);
#if DEBUG
void dumpdef(char *);
void dumpadef(char *, register DEFBUF *);
#endif
ReturnCode openfile(struct Global *,char *);
int cget(struct Global *);
void deldefines(struct Global *);
char *Getmem(struct Global *, int);
ReturnCode openinclude(struct Global *, char *, int);
ReturnCode expstuff(struct Global *, char *, char *);

#if defined(AMIGA)
#include        <dos.h>
#if defined(SHARED)
int _OSERR=0;
char *_ProgramName="junk";
void __stdargs _XCEXIT(long a) { return; }
#endif
#endif

FILE_LOCAL ReturnCode output(struct Global *, int); /* Output one character */
FILE_LOCAL void sharp(struct Global *);
INLINE FILE_LOCAL ReturnCode cppmain(struct Global *);

int fppPreProcess(struct fppTag *tags)
{
  int i=0;
  ReturnCode ret;       /* cpp return code */
  struct Global *global;

  global=(struct Global *)malloc(sizeof(struct Global));
  if(!global)
    return(FPP_OUT_OF_MEMORY);

  memset(global, 0, sizeof(struct Global));

  global->infile=NULL;
  global->line=0;
  global->wrongline=0;
  global->errors=0;
  global->recursion=0;
  global->rec_recover=TRUE;
  global->instring=FALSE;
  global->inmacro=FALSE;
  global->workp=NULL;
  global->keepcomments = FALSE;  /* Write out comments flag     */
  global->cflag = FALSE;          /* -C option (keep comments)    */
  global->eflag = FALSE;          /* -E option (never fail)       */
  global->nflag = 0;              /* -N option (no predefines)    */
  global->wflag = FALSE;          /* -W option (write #defines)   */

  global->ifstack[0]=TRUE;       /* #if information     */
  global->ifptr = global->ifstack;
  global->incend = global->incdir;

  /* names defined at cpp start */
  global->preset[0]="frexxcpp"; /* This is the Frexx cpp program */
#if defined( amiga )
  global->preset[1]="amiga";
  global->preset[2]="m68000";
  global->preset[3]="amigados";
  global->preset[4]= NULL;              /* Must be last         */
#elif defined( unix )
  global->preset[1]="unix";
  global->preset[2]= NULL;
#endif

  /* Note: order is important   */
  global->magic[0] = "__LINE__";
  global->magic[1] = "__FILE__";
  global->magic[2] = "__FUNCTION__";
  global->magic[3] = "__FUNC_LINE__";
  global->magic[4] = NULL;                        /* Must be last       */

  global->funcline = 0;

  global->cplusplus=1;
  global->sharpfilename=NULL;

  global->parmp=NULL;
  global->nargs=0;

  global->macro=NULL;
  global->evalue=0;

  global->input=NULL;
  global->output=NULL;
  global->error=NULL;
  global->first_file=NULL;
  global->userdata=NULL;

  global->linelines=TRUE;
  global->warnillegalcpp = FALSE;
  global->outputLINE = TRUE;
  global->warnnoinclude = TRUE;
  global->showversion = TRUE;
  global->showincluded = FALSE;
  global->showspace = FALSE;
  global->nestcomments = FALSE;
  global->warnnestcomments = FALSE;
  global->outputfile = TRUE;
  global->included = 0;

  global->comment = FALSE;
  global->rightconcat = FALSE;
  global->work[0] = '\0';
  global->initialfunc = NULL;

  global->allowincludelocal = TRUE;

  memset(global->symtab, 0, SBSIZE * sizeof(DEFBUF *));

  ret=initdefines(global);  /* O.S. specific def's  */
  if(ret)
    return(ret);
  dooptions(global, tags);  /* Command line -flags  */
  ret=addfile(global, stdin, global->work); /* "open" main input file       */

  global->out = global->outputfile;

  if(!ret)
    ret=cppmain(global);             /* Process main file            */
  if ((i = (global->ifptr - global->ifstack)) != 0) {
#if OLD_PREPROCESSOR
    cwarn(global, ERROR_IFDEF_DEPTH, i);
#else
    cerror(global, ERROR_IFDEF_DEPTH, i);
#endif
  }
  fflush(stdout);

  if (global->errors > 0 && !global->eflag)
    return(IO_ERROR);
  return(IO_NORMAL);       /* No errors or -E option set   */
}

INLINE FILE_LOCAL
ReturnCode cppmain(struct Global *global)
{
  /*
   * Main process for cpp -- copies tokens from the current input
   * stream (main file, include file, or a macro) to the output
   * file.
   */

  int c;        /* Current character    */
  int counter;  /* newlines and spaces  */
  ReturnCode ret; /* return code variable type */

  long bracelevel = 0;
  long parenlevel = 0;
  long bracketlevel = 0;
  int fake = 0;

#define MAX_FUNC_LENGTH 50

  char tempfunc[MAX_FUNC_LENGTH + 1];
  char tempfunc2[MAX_FUNC_LENGTH + 1];
  char define = 0; /* probability of a function define phase in the program */
  char prev = 0; /* previous type */
  char go = 0;
  int include = 0;
  char initfunc = 0;

  /* Initialize for reading tokens */
  global->tokenbsize = 50;
  global->tokenbuf = malloc(global->tokenbsize + 1);
  if(!global->tokenbuf)
    return(FPP_OUT_OF_MEMORY);

  global->functionname = malloc(global->tokenbsize + 1);
  if(!global->functionname)
    return(FPP_OUT_OF_MEMORY);
  global->functionname[0] = '\0';

  if(global->showspace) {
    global->spacebuf = (char *)malloc(MAX_SPACE_SIZE);
    if(!global->spacebuf)
      return(FPP_OUT_OF_MEMORY);
  }

  if(global->showversion)
      Error(global, VERSION_TEXT);

  /*
   * Explicitly output a #line at the start of cpp output so
   * that lint (etc.) knows the name of the original source
   * file.  If we don't do this explicitly, we may get
   * the name of the first #include file instead.
   */
  if(global->linelines) /* if #line lines are wanted! */
    sharp(global);
  /*
   * This loop is started "from the top" at the beginning of each line
   * wrongline is set TRUE in many places if it is necessary to write
   * a #line record.  (But we don't write them when expanding macros.)
   *
   * The counter variable has two different uses:  at
   * the start of a line, it counts the number of blank lines that
   * have been skipped over. These are then either output via
   * #line records or by outputting explicit blank lines.
   * When expanding tokens within a line, the counter remembers
   * whether a blank/tab has been output.  These are dropped
   * at the end of the line, and replaced by a single blank
   * within lines.
   */

  include = global->included;

  while(include--) {
    openinclude(global, global->include[include], TRUE);
  }
  
  for (;;) {
    counter = 0;                        /* Count empty lines    */
    for (;;) {                          /* For each line, ...   */
      global->comment = FALSE;          /* No comment yet!      */
      global->chpos = 0;                /* Count whitespaces    */
      while (type[(c = get(global))] == SPA)  /* Skip leading blanks */
	if(global->showspace) {
	  if(global->chpos<MAX_SPACE_SIZE-1)
	    /* we still have buffer to store this! */
	    global->spacebuf[global->chpos++]=(char)c;
	}
      if (c == '\n') {                  /* If line's all blank, */
	if(global->comment) {
	  /* A comment was output! */
	  Putchar(global, '\n');
	}
	else
	  ++counter;                    /* Do nothing now       */
      }
      else if (c == '#') {              /* Is 1st non-space '#' */
	global->keepcomments = FALSE;   /* Don't pass comments  */
	ret = control(global, &counter); /* Yes, do a #command   */
	if(ret)
	  return(ret);
	global->keepcomments = (global->cflag && compiling);
      }
      else if (c == EOF_CHAR)           /* At end of file?      */
	break;
      else if (!compiling) {            /* #ifdef false?        */
	skipnl(global);                 /* Skip to newline      */
	counter++;                      /* Count it, too.       */
      } else {
	break;                          /* Actual token         */
      }
    }
    if (c == EOF_CHAR)                  /* Exit process at      */
      break;                            /* End of file          */
    /*
     * If the loop didn't terminate because of end of file, we
     * know there is a token to compile.  First, clean up after
     * absorbing newlines.  counter has the number we skipped.
     */
    if(global->linelines) { /* if #line lines are wanted! */
      if ((global->wrongline && global->infile->fp != NULL) || counter > 4)
        sharp(global);                    /* Output # line number */
      else {                              /* If just a few, stuff */
        while (--counter >= 0)            /* them out ourselves   */
	  Putchar(global, (int)'\n');
      }
    }
    if(global->showspace) {
      /* Show all whitespaces! */
      global->spacebuf[global->chpos] = '\0';
      Putstring(global, global->spacebuf);
    }
    
    /*
     * Process each token on this line.
     */
    unget(global);                      /* Reread the char.     */
    for (;;) {                          /* For the whole line,  */
      do {                              /* Token concat. loop   */
	for (global->chpos = counter = 0; type[(c = get(global))] == SPA;) {
#if COMMENT_INVISIBLE
	  if (c != COM_SEP)
	    counter++;
#else
	  if(global->showspace && global->chpos < MAX_SPACE_SIZE-1) {
	    global->spacebuf[global->chpos++]=(char)c;
	  }
	  counter++;            /* Skip over blanks     */
#endif
	}
	if (c == EOF_CHAR || c == '\n')
	  break;                      /* Exit line loop       */
	else if (counter > 0) {       /* If we got any spaces */
	  if(!global->showspace)      /* We don't output all spaces */
	    Putchar(global, (int)' ');/* Output one space     */
	  else {
	    global->spacebuf[global->chpos] = '\0';
	    Putstring(global, global->spacebuf); /* Output all whitespaces */
	  }
	}
	if((ret=macroid(global, &c)))   /* Grab the token       */
	  return(ret);
      } while (type[c] == LET && catenate(global, 0, &ret) && !ret);
      if(ret)
	/* If the loop was broken because of a fatal error! */
	return(ret);
      if (c == EOF_CHAR || c == '\n') /* From macro exp error */
	break;                        /* Exit line loop       */
      go++;
      switch (type[c]) {
      case LET:
	go =0;
	/* Quite ordinary token */
	Putstring(global, global->tokenbuf);
	
	if(!define) {
	  /* Copy the name */
	  strncpy(tempfunc, global->tokenbuf, MAX_FUNC_LENGTH);
          tempfunc[MAX_FUNC_LENGTH]=0;
	}
	/* fputs(global->tokenbuf, stdout); */
	break;
      case DIG:                 /* Output a number      */
      case DOT:                 /* Dot may begin floats */
	go = 0;
	ret=scannumber(global, c, (ReturnCode(*)(struct Global *, int))output);
	if(ret)
	  return(ret);
	catenate(gloabl, 1, &ret);  /* Check to see if the number is the lhs of a macro concat */
	if (ret)
	  return(ret);
	break;
      case QUO:                 /* char or string const */
	go = 0;
	/* Copy it to output */
        if(!global->webmode) {
          ret=scanstring(global, c,
                         (ReturnCode(*)(struct Global *, int))output);
          if(ret)
            return(ret);
          break;
        }
        /* FALLTHROUGH */
      default:                  /* Some other character */
	
	define++;
	switch(c) {
	case '{':
	  if(! bracelevel++ && define > 2) {
	    /*
	     * This is a starting brace. If there is a probability of a
	     * function defining, we copy the `tempfunc' function name to
	     * `global->functionname'.
	     */
	    strcpy(global->functionname, tempfunc2);
	    global->funcline = global->line;

		if(global->outputfunctions) {
			/*
			 * Output the discovered function name to stderr!
			 */
			Error(global, "#> Function defined at line %d: %s <#\n",
				  global->line,
				  global->functionname);
		}

	    if(global->initialfunc) {
		int a;
		for(a=0; a<global->excluded; a++) {
		    /* check for excluded functions */
		    if(!strcmp(global->functionname,
			       global->excludedinit[a]))
			break;
		}
		if(a==global->excluded) {
		    expstuff(global, "__brace__", "{");
		    expstuff(global, "__init_func__", global->initialfunc);
		    initfunc = TRUE;
		}
	    }

	  }
	  break;
	case '}':
	  go = 0;
	  if( (--bracelevel == initfunc) &&
	     strcmp(global->infile->filename, "__init_func__") ) {
	    /* we just stepped out of the function! */
	    global->functionname[0] = '\0';
	    global->funcline = 0;
	    define = 1;

	    if(initfunc) {
	      Putchar(global, '}');
	      bracelevel--;
	      initfunc=0;
	    }
	  }
	  fake = 0;
	  break;
	  
	case ';':
	case ',':
	  if(go == 2) {
	    define = 1;
	    fake = 0;
	    go--;
	    break;
	  }
	  break;
	case '(':
	  if(! parenlevel++ && !bracelevel) {
	    if(go == 2) {
	      /* foobar(text) -> "(" is found. This can't be a
		 function */
	      go--;
	      define = 1;
	      break;
	    }
	    if( define < 2 && prev == LET) {
	      /* This is the first parenthesis on the ground brace
		 level, and we did previously not have a probable
		 function name */
	      strncpy(tempfunc2, global->tokenbuf, MAX_FUNC_LENGTH);
              tempfunc2[MAX_FUNC_LENGTH]=0;
	      define++;
	    }
	    else {
	      /* we have a fake start */
	      fake++;
	    }
	  }
	  break;
	case ')':
	  if(! --parenlevel && !bracelevel && define>1 && !fake) {
	    /*
	     * The starting parentheses level and
	     * the starting brace level.
	     * This might be the start of a function defining coming
	     * up!
	     */
	    define++; /* increase probability */
	    fake = 0;
	    go = 1;
	  }
	  break;
	case '[':
	  bracketlevel++;
	  break;
	case ']':
	  bracketlevel--;
	  break;
	}
	define--; /* decrease function probability */
	
	Putchar(global, c);     /* Just output it       */
	break;
      }                         /* Switch ends          */
      prev = type[c];
    }                           /* Line for loop        */
    
    if (c == '\n') {  /* Compiling at EOL?    */
      Putchar(global, '\n');              /* Output newline, if   */
      if (global->infile->fp == NULL)     /* Expanding a macro,   */
	global->wrongline = TRUE; /* Output # line later        */
    }
  }                             /* Continue until EOF   */

  if(global->showbalance) {
    if(bracketlevel) {
      cwarn(global, WARN_BRACKET_DEPTH, bracketlevel);
    }
    if(parenlevel) {
      cwarn(global, WARN_PAREN_DEPTH, parenlevel);
    }
    if(bracelevel) {
      cwarn(global, WARN_BRACE_DEPTH, bracelevel);
    }
  }
  if (global->wflag) {
    global->out = TRUE;         /* enable output */
    outdefines(global);         /* Write out #defines   */
  }
  return(FPP_OK);
}

FILE_LOCAL
ReturnCode output(struct Global *global, int c)
{
  /*
   * Output one character to stdout -- output() is passed as an
   * argument to scanstring()
   */
#if COMMENT_INVISIBLE
  if (c != TOK_SEP && c != COM_SEP)
#else
  if (c != TOK_SEP)
#endif
    Putchar(global, c);
  return(FPP_OK);
}

void Putchar(struct Global *global, int c)
{
  /*
   * Output one character to stdout or to output function!
   */
  if(!global->out)
    return;
#if defined(UNIX)
  if(global->output)
    global->output(c, global->userdata);
  else
    putchar(c);
#else /* amiga */
  global->output(c, global->userdata);
#endif
}

void Putstring(struct Global *global, char *string)
{
  /*
   * Output a string! One letter at a time to the Putchar routine!
   */

  if(!string)
    return;

  while(*string)
    Putchar(global, *string++);
}

void Putint(struct Global *global, int number)
{
  /*
   * Output the number as a string.
   */

  char buffer[16]; /* an integer can't be that big! */
  char *point=buffer;

  sprintf(buffer, "%d", number);

  while(*point)
    Putchar(global, *point++);
}


FILE_LOCAL
void sharp(struct Global *global)
{
  /*
   * Output a line number line.
   */

  char *name;
  if (global->keepcomments)                     /* Make sure # comes on */
    Putchar(global, '\n');                      /* a fresh, new line.   */
  /*  printf("#%s %d", LINE_PREFIX, global->line); */

  Putchar(global, '#');
  if(global->outputLINE)
          Putstring(global, LINE_PREFIX);
  Putchar(global, ' ');
  Putint(global, global->line);

  if (global->infile->fp != NULL) {
    name = (global->infile->progname != NULL)
      ? global->infile->progname : global->infile->filename;
    if (global->sharpfilename == NULL
        || (global->sharpfilename != NULL && !streq(name, global->sharpfilename))) {
      if (global->sharpfilename != NULL)
        free(global->sharpfilename);
      global->sharpfilename = savestring(global, name);
      /* printf(" \"%s\"", name); */
      Putstring(global, " \"");
      Putstring(global, name);
      Putchar(global, '\"');
    }
  }
  Putchar(global, '\n');
  global->wrongline = FALSE;
  return;
}

#ifdef _AMIGA
#include <proto/dos.h>
#endif

FILE_LOCAL void dump_line(struct Global *, int *);
FILE_LOCAL ReturnCode doif(struct Global *, int);
INLINE FILE_LOCAL ReturnCode doinclude(struct Global *);
INLINE FILE_LOCAL int hasdirectory(char *, char *);


/*
 * Generate (by hand-inspection) a set of unique values for each control
 * operator.  Note that this is not guaranteed to work for non-Ascii
 * machines.  CPP won't compile if there are hash conflicts.
 */

#define L_assert    ('a' + ('s' << 1))
#define L_define    ('d' + ('f' << 1))
#define L_elif      ('e' + ('i' << 1))
#define L_else      ('e' + ('s' << 1))
#define L_endif     ('e' + ('d' << 1))
#define L_error     ('e' + ('r' << 1))
#define L_if        ('i' + (EOS << 1))
#define L_ifdef     ('i' + ('d' << 1))
#define L_ifndef    ('i' + ('n' << 1))
#define L_include   ('i' + ('c' << 1))
#define L_line      ('l' + ('n' << 1))
#define L_nogood    (EOS + (EOS << 1))      /* To catch #i          */
#define L_pragma    ('p' + ('a' << 1))
#define L_undef     ('u' + ('d' << 1))

ReturnCode control( struct Global *global,
    int *counter )  /* Pending newline counter */
{
    /*
     * Process #control lines.  Simple commands are processed inline,
     * while complex commands have their own subroutines.
     *
     * The counter is used to force out a newline before #line, and
     * #pragma commands.  This prevents these commands from ending up at
     * the end of the previous line if cpp is invoked with the -C option.
     */

    int c;
    char *tp;
    int hash;
    char *ep;
    ReturnCode ret;

    c = skipws( global );

    if( c == '\n' || c == EOF_CHAR )
        {
        (*counter)++;

        return(FPP_OK);
        }

    if( !isdigit(c) )
        scanid( global, c );                  /* Get #word to tokenbuf        */
    else
        {
        unget( global );                    /* Hack -- allow #123 as a      */

        strcpy( global->tokenbuf, "line" );   /* synonym for #line 123        */
        }

    hash = (global->tokenbuf[1] == EOS) ? L_nogood : (global->tokenbuf[0] + (global->tokenbuf[2] << 1));

    switch( hash )
        {
        case L_assert:
            tp = "assert";
            break;
        case L_define:
            tp = "define";
            break;
        case L_elif:
            tp = "elif";
            break;
        case L_else:
            tp = "else";
            break;
        case L_endif:
            tp = "endif";
            break;
        case L_error:
            tp = "error";
            break;
        case L_if:
            tp = "if";
            break;
        case L_ifdef:
            tp = "ifdef";
            break;
        case L_ifndef:
            tp = "ifndef";
            break;
        case L_include:
            tp = "include";
            break;
        case L_line:
            tp = "line";
            break;
        case L_pragma:
            tp = "pragma";
            break;
        case L_undef:
            tp = "undef";
            break;
        default:
            hash = L_nogood;
        case L_nogood:
            tp = "";
            break;
        }

    if( !streq( tp, global->tokenbuf ) )
        hash = L_nogood;

    /*
     * hash is set to a unique value corresponding to the
     * control keyword (or L_nogood if we think it's nonsense).
     */
    if( global->infile->fp == NULL )
        cwarn( global, WARN_CONTROL_LINE_IN_MACRO, global->tokenbuf );

    if( !compiling )
        {                       /* Not compiling now    */
        switch( hash )
            {
            case L_if:              /* These can't turn     */
            case L_ifdef:           /*  compilation on, but */
            case L_ifndef:          /*   we must nest #if's */
                if( ++global->ifptr >= &global->ifstack[BLK_NEST] )
                    {
                    cfatal( global, FATAL_TOO_MANY_NESTINGS, global->tokenbuf );

                    return( FPP_TOO_MANY_NESTED_STATEMENTS );
                    }

                *global->ifptr = 0;       /* !WAS_COMPILING   */

            case L_line:            /* Many         */
                /*
                 * Are pragma's always processed?
                 */
            case L_pragma:          /*  options     */
            case L_include:         /*   are uninteresting  */
            case L_define:          /*    if we     */
            case L_undef:           /*     aren't           */
            case L_assert:          /*  compiling.  */
            case L_error:
                dump_line( global, counter );       /* Ignore rest of line  */
                return(FPP_OK);
            }
        }
    /*
     * Make sure that #line and #pragma are output on a fresh line.
     */
    if( *counter > 0 && (hash == L_line || hash == L_pragma) )
        {
        Putchar( global, '\n' );

        (*counter)--;
        }

    switch( hash )
        {
        case L_line:
            /*
             * Parse the line to update the line number and "progname"
             * field and line number for the next input line.
             * Set wrongline to force it out later.
             */
            c = skipws( global );

            global->workp = global->work;       /* Save name in work    */

            while( c != '\n' && c != EOF_CHAR )
                {
                if( (ret = save( global, c )) )
                    return(ret);

                c = get( global );
                }

            unget( global );

            if( (ret = save( global, EOS )) )
                return(ret);

            /*
             * Split #line argument into <line-number> and <name>
             * We subtract 1 as we want the number of the next line.
             */
            global->line = atoi(global->work) - 1;     /* Reset line number    */

            for( tp = global->work; isdigit(*tp) || type[(int)*tp] == SPA; tp++)
                ;             /* Skip over digits */

            if( *tp != EOS )
                {
                /* Got a filename, so:  */

                if( *tp == '"' && (ep = strrchr(tp + 1, '"')) != NULL )
                    {
                    tp++;           /* Skip over left quote */

                    *ep = EOS;      /* And ignore right one */
                    }

                if( global->infile->progname != NULL )
                    /* Give up the old name if it's allocated.   */
                    free( global->infile->progname );

                global->infile->progname = savestring( global, tp );
                }

            global->wrongline = TRUE;           /* Force output later   */
            break;

        case L_include:
            ret = doinclude( global );
            if( ret )
                return(ret);
            break;

        case L_define:
            ret = dodefine( global );
            if( ret )
                return(ret);
            break;

        case L_undef:
            doundef( global );
            break;

        case L_else:
            if( global->ifptr == &global->ifstack[0] )
                {
                cerror( global, ERROR_STRING_MUST_BE_IF, global->tokenbuf );

                dump_line( global, counter );

                return( FPP_OK );
                }
            else if( (*global->ifptr & ELSE_SEEN) != 0 )
                {
                cerror( global, ERROR_STRING_MAY_NOT_FOLLOW_ELSE, global->tokenbuf );

                dump_line( global, counter );

                return( FPP_OK );
                }

            *global->ifptr |= ELSE_SEEN;

            if( (*global->ifptr & WAS_COMPILING) != 0 )
                {
                if( compiling || (*global->ifptr & TRUE_SEEN) != 0 )
                    compiling = FALSE;
                else
                    {
                    compiling = TRUE;
                    }
                }
            break;

        case L_elif:
            if( global->ifptr == &global->ifstack[0] )
                {
                cerror( global, ERROR_STRING_MUST_BE_IF, global->tokenbuf );

                dump_line( global, counter );

                return( FPP_OK );
                }
            else if( (*global->ifptr & ELSE_SEEN) != 0 )
                {
                cerror( global, ERROR_STRING_MAY_NOT_FOLLOW_ELSE, global->tokenbuf );

                dump_line( global, counter );

                return( FPP_OK );
                }

            if( (*global->ifptr & (WAS_COMPILING | TRUE_SEEN)) != WAS_COMPILING )
                {
                compiling = FALSE;        /* Done compiling stuff */

                dump_line( global, counter );   /* Skip this clause */

                return( FPP_OK );
                }

            ret = doif( global, L_if );

            if( ret )
                return(ret);

            break;

        case L_error:
            cerror(global, ERROR_ERROR);
            break;

        case L_if:
        case L_ifdef:
        case L_ifndef:
            if( ++global->ifptr < &global->ifstack[BLK_NEST] )
                {
                *global->ifptr = WAS_COMPILING;

                ret = doif( global, hash );

                if( ret )
                    return(ret);

                break;
                }

            cfatal( global, FATAL_TOO_MANY_NESTINGS, global->tokenbuf );

            return( FPP_TOO_MANY_NESTED_STATEMENTS );

        case L_endif:
            if( global->ifptr == &global->ifstack[0] )
                {
                cerror( global, ERROR_STRING_MUST_BE_IF, global->tokenbuf );

                dump_line( global, counter );

                return(FPP_OK);
                }

            if( !compiling && (*global->ifptr & WAS_COMPILING) != 0 )
                global->wrongline = TRUE;

            compiling = ((*global->ifptr & WAS_COMPILING) != 0);

            --global->ifptr;

            break;

        case L_assert:
            {
            int result;

            ret = eval( global, &result );

            if(ret)
                return(ret);

            if( result == 0 )
                cerror( global, ERROR_PREPROC_FAILURE );
            }
            break;

        case L_pragma:
            /*
             * #pragma is provided to pass "options" to later
             * passes of the compiler.  cpp doesn't have any yet.
             */
            Putstring( global, "#pragma " );

            while( (c = get( global ) ) != '\n' && c != EOF_CHAR )
                Putchar( global, c );

            unget( global );

            Putchar( global, '\n' );

            break;

        default:
            /*
             * Undefined #control keyword.
             * Note: the correct behavior may be to warn and
             * pass the line to a subsequent compiler pass.
             * This would allow #asm or similar extensions.
             */
            if( global->warnillegalcpp )
                cwarn( global, WARN_ILLEGAL_COMMAND, global->tokenbuf );

            Putchar( global, '#' );
            Putstring( global, global->tokenbuf );
            Putchar( global, ' ' );

            while( (c = get( global ) ) != '\n' && c != EOF_CHAR )
                Putchar( global, c );

            unget( global );

            Putchar( global, '\n' );

            break;
        }

    if( hash != L_include )
        {
        #if OLD_PREPROCESSOR
        /*
         * Ignore the rest of the #control line so you can write
         *      #if foo
         *      #endif  foo
         */
        dump_line( global, counter );         /* Take common exit */

        return( FPP_OK );
        #else
        if( skipws( global ) != '\n' )
            {
            cwarn( global, WARN_UNEXPECTED_TEXT_IGNORED );

            skipnl( global );
            }
        #endif
        }

    (*counter)++;

    return( FPP_OK );
}

FILE_LOCAL
void dump_line(struct Global *global, int *counter)
{
    skipnl( global );         /* Ignore rest of line  */

    (*counter)++;
}

FILE_LOCAL
ReturnCode doif(struct Global *global, int hash)
{
    /*
     * Process an #if, #ifdef, or #ifndef. The latter two are straightforward,
     * while #if needs a subroutine of its own to evaluate the expression.
     *
     * doif() is called only if compiling is TRUE.  If false, compilation
     * is always supressed, so we don't need to evaluate anything.  This
     * supresses unnecessary warnings.
     */

    int c;
    int found;
    ReturnCode ret;

    if( (c = skipws( global ) ) == '\n' || c == EOF_CHAR )
        {
        unget( global );

        cerror( global, ERROR_MISSING_ARGUMENT );

        #if !OLD_PREPROCESSOR
        skipnl( global );               /* Prevent an extra     */

        unget( global );                /* Error message        */
        #endif

        return(FPP_OK);
        }

    if( hash == L_if )
        {
        unget( global );

        ret = eval( global, &found );

        if( ret )
            return( ret );

        found = (found != 0);     /* Evaluate expr, != 0 is  TRUE */

        hash = L_ifdef;       /* #if is now like #ifdef */
        }
    else
        {
        if( type[c] != LET )
            {         /* Next non-blank isn't letter  */
                          /* ... is an error          */
            cerror( global, ERROR_MISSING_ARGUMENT );

            #if !OLD_PREPROCESSOR
            skipnl( global );             /* Prevent an extra     */

            unget( global );              /* Error message        */
            #endif

            return(FPP_OK);
            }

        found = ( lookid( global, c ) != NULL ); /* Look for it in symbol table */
        }

    if( found == (hash == L_ifdef) )
        {
        compiling = TRUE;

        *global->ifptr |= TRUE_SEEN;
        }
    else
        compiling = FALSE;

    return(FPP_OK);
}

INLINE FILE_LOCAL
ReturnCode doinclude( struct Global *global )
{
    /*
     *  Process the #include control line.
     *  There are three variations:
     *
     *      #include "file" search somewhere relative to the
     *                      current source file, if not found,
     *                      treat as #include <file>.
     *
     *      #include <file> Search in an implementation-dependent
     *                      list of places.
     *
     *      #include token  Expand the token, it must be one of
     *                      "file" or <file>, process as such.
     *
     *  Note:   the November 12 draft forbids '>' in the #include <file> format.
     *          This restriction is unnecessary and not implemented.
     */

    int c;
    int delim;
    ReturnCode ret;

    delim = skipws( global );

    if( (ret = macroid( global, &delim )) )
        return(ret);

    if( delim != '<' && delim != '"' )
        {
        cerror( global, ERROR_INCLUDE_SYNTAX );

        return( FPP_OK );
        }

    if( delim == '<' )
        delim = '>';

    global->workp = global->work;

    while( (c = get(global)) != '\n' && c != EOF_CHAR )
        if( (ret = save( global, c )) )       /* Put it away.                */
            return( ret );

    unget( global );                        /* Force nl after include      */

    /*
     * The draft is unclear if the following should be done.
     */
    while( --global->workp >= global->work &&
        (*global->workp == ' ' || *global->workp == '\t') )
        ;               /* Trim blanks from filename    */

    if( *global->workp != delim )
        {
        cerror( global, ERROR_INCLUDE_SYNTAX );

        return(FPP_OK);
        }

    *global->workp = EOS;         /* Terminate filename       */

    ret = openinclude( global, global->work, (delim == '"') );

    if( ret && global->warnnoinclude )
        {
        /*
         * Warn if #include file isn't there.
         */
        cwarn( global, WARN_CANNOT_OPEN_INCLUDE, global->work );
        }

    return( FPP_OK );
}

#ifdef _AMIGA
ReturnCode MultiAssignLoad( struct Global *global, char *incptr, char *filename, char *tmpname );
#endif

ReturnCode openinclude( struct Global *global,
    char *filename,     /* Input file name         */
    int searchlocal )   /* TRUE if #include "file" */
{
    /*
     * Actually open an include file.  This routine is only called from
     * doinclude() above, but was written as a separate subroutine for
     * programmer convenience.  It searches the list of directories
     * and actually opens the file, linking it into the list of
     * active files.  Returns ReturnCode. No error message is printed.
     */

    char **incptr;
    char tmpname[NWORK]; /* Filename work area    */
    int len;

    #if HOST == SYS_AMIGADOS
    if( strchr (filename, ':') != NULL )
        {
        if( ! openfile( global, filename ) )
            return(FPP_OK);
        }
    #else
    if( filename[0] == '/' )
        {
        if( ! openfile( global, filename ) )
            return(FPP_OK);
        }
    #endif

    if( searchlocal && global->allowincludelocal )
        {
        /*
         * Look in local directory first.
         * Try to open filename relative to the directory of the current
         * source file (as opposed to the current directory). (ARF, SCK).
         * Note that the fully qualified pathname is always built by
         * discarding the last pathname component of the source file
         * name then tacking on the #include argument.
         */

        if( hasdirectory( global->infile->filename, tmpname ) )
            strcat( tmpname, filename );
        else
            strcpy( tmpname, filename );

        if( ! openfile( global, tmpname ) )
            return(FPP_OK);
        }

    /*
     * Look in any directories specified by -I command line
     * arguments, then in the builtin search list.
     */
    for( incptr = global->incdir; incptr < global->incend; incptr++ )
        {
        len = strlen(*incptr);

        if( len + strlen(filename) >= sizeof(tmpname) )
            {
            cfatal( global, FATAL_FILENAME_BUFFER_OVERFLOW );

            return( FPP_FILENAME_BUFFER_OVERFLOW );
            }
        else
            {
            #if HOST == SYS_AMIGADOS
            if( (*incptr)[len-1] != '/' && (*incptr)[len-1] != ':' )
	            sprintf( tmpname, "%s/%s", *incptr, filename );
            #else
            if( (*incptr)[len-1] != '/' )
                sprintf( tmpname, "%s/%s", *incptr, filename );
            #endif
            else
                sprintf( tmpname, "%s%s", *incptr, filename );

            #if HOST == SYS_AMIGADOS
            //
            //  amp July 9, 1997
            //
            //  OK, hack in multiassign support for the buitin
            //  search directories...
            //
            if( (*incptr)[len-1] == ':' )
                {
                if( ! MultiAssignLoad( global, *incptr, filename, tmpname ) )
                    return(FPP_OK);
                }
            else
            #endif
            if( !openfile( global, tmpname ) )
                return(FPP_OK);
            }
        }

    return( FPP_NO_INCLUDE );
}

INLINE FILE_LOCAL
int hasdirectory( char *source,   /* Directory to examine         */
    char *result )  /* Put directory stuff here     */
{
    /*
     * If a device or directory is found in the source filename string, the
     * node/device/directory part of the string is copied to result and
     * hasdirectory returns TRUE.  Else, nothing is copied and it returns FALSE.
     */

    char *tp2;

    #if HOST == SYS_AMIGADOS
    char *tp1;

    if( (tp1 = strrchr( source, ':' ) ) == NULL )
        tp1 = source;

    if( (tp2 = strrchr( tp1, '/' ) ) == NULL )
        tp2 = tp1;

    if( tp2 == source )
        return (FALSE);

    #else
    if( (tp2 = strrchr( source, '/' ) ) == NULL )
        return(FALSE);
    #endif

    strncpy( result, source, tp2 - source + 1 );

    result[tp2 - source + 1] = EOS;

    return( TRUE );
}

#ifdef _AMIGA
//
//  amp July 9, 1997
//
//  Use the OS Luke...
//
//  We do the sneaky version and let the OS do all
//  the hard work so we don't have to mess around
//  a lot ;)
//
ReturnCode MultiAssignLoad( struct Global *global, char *incptr, char *filename, char *tmpname )

{ /* MultiAssignLoad */

    struct MsgPort  *FSTask;
    struct DevProc  *DevProc = NULL;
    LONG            RtnCode = FPP_NO_INCLUDE;

    FSTask = GetFileSysTask();

    do
        {
        //
        //  This should not bring up a requester.
        //  check to see if cpp does in fact tweek
        //  the process WindowPtr.
        //
        DevProc = GetDeviceProc( incptr, DevProc );

        if( DevProc )
            {
            SetFileSysTask( DevProc->dvp_Port );

            //
            //  Normally we would pass the lock and filename
            //  to the Load() routine, which would CD to the
            //  directory and Open(filename), but in order to
            //  satisfy the exisiting openfile() function, we
            //  bite the bullet and build the complete pathspec
            //  rather than add the standard Load() routine.
            //
            if( NameFromLock( DevProc->dvp_Lock, tmpname, NWORK ) )
                {
                AddPart( tmpname, filename, NWORK );

                RtnCode = openfile( global, tmpname );

                if( ! RtnCode )
                    break;
                }
            }

        } while ( RtnCode &&
            DevProc &&
            (DevProc->dvp_Flags & DVPF_ASSIGN) &&
            IoErr() == ERROR_OBJECT_NOT_FOUND); /* repeat if multi-assign */

    SetFileSysTask( FSTask );

    if( DevProc )
        FreeDeviceProc( DevProc );

    return RtnCode;

} /* MultiAssignLoad */
#endif  //_AMIGA


ReturnCode openfile(struct Global *global, char *filename)
{
  /*
   * Open a file, add it to the linked list of open files.
   * This is called only from openfile() in cpp2.c.
   */

  FILE *fp;
  ReturnCode ret;

  if ((fp = fopen(filename, "r")) == NULL)
    ret=FPP_OPEN_ERROR;
  else
    ret=addfile(global, fp, filename);

  if(!ret && global->showincluded) {
          /* no error occured! */
          Error(global, "cpp: included \"");
          Error(global, filename);
          Error(global, "\"\n");
  }
  return(ret);
}

ReturnCode addfile(struct Global *global,
                   FILE *fp,            /* Open file pointer */
                   char *filename)      /* Name of the file  */
{
  /*
   * Initialize tables for this open file.  This is called from openfile()
   * above (for #include files), and from the entry to cpp to open the main
   * input file. It calls a common routine, getfile() to build the FILEINFO
   * structure which is used to read characters. (getfile() is also called
   * to setup a macro replacement.)
   */

  FILEINFO *file;
  ReturnCode ret;

  ret = getfile(global, NBUFF, filename, &file);
  if(ret)
    return(ret);
  file->fp = fp;                        /* Better remember FILE *       */
  file->buffer[0] = EOS;                /* Initialize for first read    */
  global->line = 1;                     /* Working on line 1 now        */
  global->wrongline = TRUE;             /* Force out initial #line      */
  return(FPP_OK);
}

int dooptions(struct Global *global, struct fppTag *tags)
{
  /*
   * dooptions is called to process command line arguments (-Detc).
   * It is called only at cpp startup.
   */
  DEFBUF *dp;
  char end=FALSE; /* end of taglist */

  while(tags && !end) {
    switch(tags->tag) {
    case FPPTAG_END:
      end=TRUE;
      break;
    case FPPTAG_INITFUNC:
      global->initialfunc = (char *) tags->data;
      break;
    case FPPTAG_DISPLAYFUNCTIONS:
      global->outputfunctions = tags->data?1:0;
      break;
    case FPPTAG_RIGHTCONCAT:
      global->rightconcat = tags->data?1:0;
      break;
    case FPPTAG_OUTPUTMAIN:
      global->outputfile = tags->data?1:0;
      break;
    case FPPTAG_NESTED_COMMENTS:
      global->nestcomments = tags->data?1:0;
      break;
    case FPPTAG_WARNMISSINCLUDE:
      global->warnnoinclude = tags->data?1:0;
      break;
    case FPPTAG_WARN_NESTED_COMMENTS:
      global->warnnestcomments =  tags->data?1:0;
      break;
    case FPPTAG_OUTPUTSPACE:
      global->showspace = tags->data?1:0;
      break;
    case FPPTAG_OUTPUTBALANCE:
      global->showbalance = tags->data?1:0;
      break;
    case FPPTAG_OUTPUTINCLUDES:
      global->showincluded = tags->data?1:0;
      break;
    case FPPTAG_SHOWVERSION:
      global->showversion = tags->data?1:0;
      break;
    case FPPTAG_WARNILLEGALCPP:
      global->warnillegalcpp = tags->data?1:0;
      break;
    case FPPTAG_OUTPUTLINE:
      global->outputLINE = tags->data?1:0;
      break;
    case FPPTAG_KEEPCOMMENTS:
      if(tags->data) {
        global->cflag = TRUE;
        global->keepcomments = TRUE;
      }
      break;
    case FPPTAG_DEFINE:
      /*
       * If the option is just "-Dfoo", make it -Dfoo=1
       */
      {
        char *symbol=(char *)tags->data;
        char *text=symbol;
        while (*text != EOS && *text != '=')
          text++;
        if (*text == EOS)
          text = "1";
        else
          *text++ = EOS;
        /*
         * Now, save the word and its definition.
         */
        dp = defendel(global, symbol, FALSE);
        if(!dp)
          return(FPP_OUT_OF_MEMORY);
        dp->repl = savestring(global, text);
        dp->nargs = DEF_NOARGS;
      }
      break;
    case FPPTAG_IGNORE_NONFATAL:
      global->eflag = TRUE;
      break;
    case FPPTAG_INCLUDE_DIR:
      if (global->incend >= &global->incdir[NINCLUDE]) {
          cfatal(global, FATAL_TOO_MANY_INCLUDE_DIRS);
          return(FPP_TOO_MANY_INCLUDE_DIRS);
      }
      *global->incend++ = (char *)tags->data;
      break;
    case FPPTAG_INCLUDE_FILE:
    case FPPTAG_INCLUDE_MACRO_FILE:
      if (global->included >= NINCLUDE) {
          cfatal(global, FATAL_TOO_MANY_INCLUDE_FILES);
          return(FPP_TOO_MANY_INCLUDE_FILES);
      }
      global->include[global->included] = (char *)tags->data;

      global->includeshow[global->included] =
          (tags->tag == FPPTAG_INCLUDE_FILE);

      global->included++;
      break;
    case FPPTAG_BUILTINS:
      global->nflag|=(tags->data?NFLAG_BUILTIN:0);
      break;
    case FPPTAG_PREDEFINES:
      global->nflag|=(tags->data?NFLAG_PREDEFINE:0);
      break;
    case FPPTAG_IGNORE_CPLUSPLUS:
      global->cplusplus=!tags->data;
      break;
    case FPPTAG_SIZEOF_TABLE:
      {
        SIZES *sizp;    /* For -S               */
        int size;       /* For -S               */
        int isdatum;    /* FALSE for -S*        */
        int endtest;    /* For -S               */

        char *text=(char *)tags->data;

        sizp = size_table;
        if ((isdatum = (*text != '*'))) /* If it's just -S,     */
          endtest = T_FPTR;     /* Stop here            */
        else {                  /* But if it's -S*      */
          text++;               /* Step over '*'        */
          endtest = 0;          /* Stop at end marker   */
        }
        while (sizp->bits != endtest && *text != EOS) {
          if (!isdigit(*text)) {    /* Skip to next digit   */
            text++;
            continue;
          }
          size = 0;             /* Compile the value    */
          while (isdigit(*text)) {
            size *= 10;
            size += (*text++ - '0');
          }
          if (isdatum)
            sizp->size = size;  /* Datum size           */
          else
            sizp->psize = size; /* Pointer size         */
          sizp++;
        }
        if (sizp->bits != endtest)
          cwarn(global, WARN_TOO_FEW_VALUES_TO_SIZEOF, NULL);
        else if (*text != EOS)
          cwarn(global, WARN_TOO_MANY_VALUES_TO_SIZEOF, NULL);
      }
      break;
    case FPPTAG_UNDEFINE:
      if (defendel(global, (char *)tags->data, TRUE) == NULL)
        cwarn(global, WARN_NOT_DEFINED, tags->data);
      break;
    case FPPTAG_OUTPUT_DEFINES:
      global->wflag++;
      break;
    case FPPTAG_INPUT_NAME:
      strcpy(global->work, tags->data);    /* Remember input filename */
      global->first_file=tags->data;
      break;
    case FPPTAG_INPUT:
      global->input=(char *(*)(char *, int, void *))tags->data;
      break;
    case FPPTAG_OUTPUT:
      global->output=(void (*)(int, void *))tags->data;
      break;
    case FPPTAG_ERROR:
      global->error=(void (*)(void *, char *, va_list))tags->data;
      break;
    case FPPTAG_USERDATA:
      global->userdata=tags->data;
      break;
    case FPPTAG_LINE:
      global->linelines= tags->data?1:0;
      break;
    case FPPTAG_EXCLFUNC:
      global->excludedinit[ global->excluded++ ] = (char *)tags->data;
      break;
    case FPPTAG_WEBMODE:
      global->webmode=(tags->data?1:0);
      break;
    case FPPTAG_ALLOW_INCLUDE_LOCAL:
      global->allowincludelocal=(tags->data?1:0);
      break;
    default:
      cwarn(global, WARN_INTERNAL_ERROR, NULL);
      break;
    }
    tags++;
  }
  return(0);
}

ReturnCode initdefines(struct Global *global)
{
  /*
   * Initialize the built-in #define's.  There are two flavors:
   *    #define decus   1               (static definitions)
   *    #define __FILE__ ??             (dynamic, evaluated by magic)
   * Called only on cpp startup.
   *
   * Note: the built-in static definitions are supressed by the -N option.
   * __LINE__, __FILE__, __TIME__ and __DATE__ are always present.
   */

  char **pp;
  char *tp;
  DEFBUF *dp;
  struct tm *tm;

  int i;
  time_t tvec;

  static char months[12][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

  /*
   * Predefine the built-in symbols.  Allow the
   * implementor to pre-define a symbol as "" to
   * eliminate it.
   */
  if (!(global->nflag & NFLAG_BUILTIN)) {
    for (pp = global->preset; *pp != NULL; pp++) {
      if (*pp[0] != EOS) {
        dp = defendel(global, *pp, FALSE);
        if(!dp)
          return(FPP_OUT_OF_MEMORY);
        dp->repl = savestring(global, "1");
        dp->nargs = DEF_NOARGS;
      }
    }
  }
  /*
   * The magic pre-defines (__FILE__ and __LINE__ are
   * initialized with negative argument counts.  expand()
   * notices this and calls the appropriate routine.
   * DEF_NOARGS is one greater than the first "magic" definition.
   */
  if (!(global->nflag & NFLAG_PREDEFINE)) {
    for (pp = global->magic, i = DEF_NOARGS; *pp != NULL; pp++) {
      dp = defendel(global, *pp, FALSE);
      if(!dp)
        return(FPP_OUT_OF_MEMORY);
      dp->nargs = --i;
    }
#if OK_DATE
    /*
     * Define __DATE__ as today's date.
     */
    dp = defendel(global, "__DATE__", FALSE);
    tp = malloc(14);
    if(!tp || !dp)
      return(FPP_OUT_OF_MEMORY);
    dp->repl = tp;
    dp->nargs = DEF_NOARGS;
    time(&tvec);
    tm = localtime(&tvec);
    sprintf(tp, "\"%3s %2d %4d\"",      /* "Aug 20 1988" */
            months[tm->tm_mon],
            tm->tm_mday,
            tm->tm_year + 1900);

    /*
     * Define __TIME__ as this moment's time.
     */
    dp = defendel(global, "__TIME__", FALSE);
    tp = malloc(11);
    if(!tp || !dp)
      return(FPP_OUT_OF_MEMORY);
    dp->repl = tp;
    dp->nargs = DEF_NOARGS;
    sprintf(tp, "\"%2d:%02d:%02d\"",    /* "20:42:31" */
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);
#endif
  }
  return(FPP_OK);
}

void deldefines(struct Global *global)
{
  /*
   * Delete the built-in #define's.
   */
  char **pp;
  int i;


  /*
   * Delete the built-in symbols, unless -WW.
   */
  if (global->wflag < 2) {
    for (pp = global->preset; *pp != NULL; pp++) {
      defendel(global, *pp, TRUE);
    }
  }
  /*
   * The magic pre-defines __FILE__ and __LINE__
   */
  for (pp = global->magic, i = DEF_NOARGS; *pp != NULL; pp++) {
    defendel(global, *pp, TRUE);
  }
#if OK_DATE
  /*
   * Undefine __DATE__.
   */
  defendel(global, "__DATE__", TRUE);

  /*
   * Undefine __TIME__.
   */
  defendel(global, "__TIME__", TRUE);
#endif
  return;
}

INLINE FILE_LOCAL ReturnCode checkparm(struct Global *, int, DEFBUF *, int);
INLINE FILE_LOCAL ReturnCode stparmscan(struct Global *, int);
INLINE FILE_LOCAL ReturnCode textput(struct Global *, char *);
FILE_LOCAL ReturnCode charput(struct Global *, int);
INLINE FILE_LOCAL ReturnCode expcollect(struct Global *);
INLINE FILE_LOCAL char *doquoting(char *, char *);


ReturnCode dodefine(struct Global *global)
{
  /*
   * Called from control when a #define is scanned.  This module
   * parses formal parameters and the replacement string.  When
   * the formal parameter name is encountered in the replacement
   * string, it is replaced by a character in the range 128 to
   * 128+NPARAM (this allows up to 32 parameters within the
   * Dec Multinational range).  If cpp is ported to an EBCDIC
   * machine, you will have to make other arrangements.
   *
   * There is some special case code to distinguish
   *	#define foo	bar
   * from #define foo()   bar
   *
   * Also, we make sure that
   *	#define foo	foo
   * expands to "foo" but doesn't put cpp into an infinite loop.
   *
   * A warning message is printed if you redefine a symbol to a
   * different text.  I.e,
   *	#define foo	123
   *	#define foo	123
   * is ok, but
   *	#define foo	123
   *	#define foo	+123
   * is not.
   *
   * The following subroutines are called from define():
   * checkparm	called when a token is scanned.  It checks through the
   *		array of formal parameters.  If a match is found, the
   *		token is replaced by a control byte which will be used
   *		to locate the parameter when the macro is expanded.
   * textput	puts a string in the macro work area (parm[]), updating
   *		parmp to point to the first free byte in parm[].
   *		textput() tests for work buffer overflow.
   * charput	puts a single character in the macro work area (parm[])
   *		in a manner analogous to textput().
   */
  int c;
  DEFBUF *dp;	/* -> new definition	*/
  int isredefine;	/* TRUE if redefined	*/
  char *old;		/* Remember redefined	*/
  ReturnCode ret;
#if OK_CONCAT
  int quoting;	/* Remember we saw a #	*/
#endif
  
  if (type[(c = skipws(global))] != LET) {
    cerror(global, ERROR_DEFINE_SYNTAX);
    global->inmacro = FALSE;		/* Stop <newline> hack	*/
    return(FPP_OK);
  }
  isredefine = FALSE;			/* Set if redefining	*/
  if ((dp = lookid(global, c)) == NULL) { /* If not known now     */
    dp = defendel(global, global->tokenbuf, FALSE); /* Save the name  */
    if(!dp)
      return(FPP_OUT_OF_MEMORY);
  } else {				/* It's known:          */
    isredefine = TRUE;			/* Remember this fact	*/
    old = dp->repl;			/* Remember replacement */
    dp->repl = NULL;			/* No replacement now	*/
  }
  global->parlist[0] = global->parmp = global->parm; /* Setup parm buffer */
  if ((c = get(global)) == '(') {       /* With arguments?      */
    global->nargs = 0;			/* Init formals counter */
    do {				/* Collect formal parms */
      if (global->nargs >= LASTPARM) {
	cfatal(global, FATAL_TOO_MANY_ARGUMENTS_MACRO);
	return(FPP_TOO_MANY_ARGUMENTS);
      } else if ((c = skipws(global)) == ')')
	break;			/* Got them all 	*/
      else if (type[c] != LET) {         /* Bad formal syntax    */
	cerror(global, ERROR_DEFINE_SYNTAX);
	global->inmacro = FALSE;		/* Stop <newline> hack	*/
	return(FPP_OK);
      }
      scanid(global, c);                        /* Get the formal param */
      global->parlist[global->nargs++] = global->parmp; /* Save its start */
      ret=textput(global, global->tokenbuf); /* Save text in parm[]  */
      if(ret)
	return(ret);
    } while ((c = skipws(global)) == ',');    /* Get another argument */
    if (c != ')') {                     /* Must end at )        */
      cerror(global, ERROR_DEFINE_SYNTAX);
      global->inmacro = FALSE;		/* Stop <newline> hack	*/
      return(FPP_OK);
    }
    c = ' ';                            /* Will skip to body    */
  }
  else {
    /*
     * DEF_NOARGS is needed to distinguish between
     * "#define foo" and "#define foo()".
     */
    global->nargs = DEF_NOARGS;		/* No () parameters     */
  }
  if (type[c] == SPA)                   /* At whitespace?       */
    c = skipws(global);                 /* Not any more.        */
  global->workp = global->work;		/* Replacement put here */
  global->inmacro = TRUE;		/* Keep \<newline> now	*/
  quoting = 0;				/* No # seen yet.	*/
  while (c != EOF_CHAR && c != '\n') {  /* Compile macro body   */
#if OK_CONCAT
    if (c == '#') {                     /* Token concatenation? */
      if ((c = get(global)) != '#') {   /* No, not really       */
	quoting = 1;		        /* Maybe quoting op.	*/
	continue;
      }
      while (global->workp > global->work && type[(int)global->workp[-1]] == SPA)
	--global->workp;		/* Erase leading spaces */
      if((ret=save(global, TOK_SEP)))     /* Stuff a delimiter    */
	return(ret);
      c = skipws(global);               /* Eat whitespace       */
      continue;
    }
#endif
    switch (type[c]) {
    case LET:
#if OK_CONCAT
      ret=checkparm(global, c, dp, quoting);      /* Might be a formal    */
#else
      ret=checkparm(c, dp);               /* Might be a formal    */
#endif
      if(ret)
	return(ret);
      break;
	
    case DIG:				/* Number in mac. body	*/
    case DOT:				/* Maybe a float number */
      ret=scannumber(global, c, save);  /* Scan it off          */
      if(ret)
	return(ret);
      break;
	
    case QUO:				/* String in mac. body	*/
      ret=stparmscan(global, c);
      if(ret)
	return(ret);
      break;
	
    case BSH:				/* Backslash		*/
      ret=save(global, '\\');
      if(ret)
	return(ret);
      if ((c = get(global)) == '\n')
	global->wrongline = TRUE;
      ret=save(global, c);
      if(ret)
	return(ret);
      break;
      
    case SPA:				/* Absorb whitespace	*/
      /*
       * Note: the "end of comment" marker is passed on
       * to allow comments to separate tokens.
       */
      if (global->workp[-1] == ' ')   /* Absorb multiple      */
	break;			/* spaces		*/
      else if (c == '\t')
	c = ' ';                      /* Normalize tabs       */
      /* Fall through to store character			*/
    default:				/* Other character	*/
      ret=save(global, c);
      if(ret)
	return(ret);
      break;
    }
    c = get(global);
    quoting = 0;			/* Only when immediately*/
    /* preceding a formal	*/
  }
  global->inmacro = FALSE;		/* Stop newline hack	*/
  unget(global);                            /* For control check    */
  if (global->workp > global->work && global->workp[-1] == ' ') /* Drop trailing blank  */
    global->workp--;
  *global->workp = EOS;		/* Terminate work	*/
  dp->repl = savestring(global, global->work); /* Save the string      */
  dp->nargs = global->nargs;			/* Save arg count	*/
  if (isredefine) {                   /* Error if redefined   */
    if ((old != NULL && dp->repl != NULL && !streq(old, dp->repl))
	|| (old == NULL && dp->repl != NULL)
	|| (old != NULL && dp->repl == NULL)) {
      cerror(global, ERROR_REDEFINE, dp->name);
    }
    if (old != NULL)                  /* We don't need the    */
      free(old);                      /* old definition now.  */
  }
  return(FPP_OK);
}

INLINE FILE_LOCAL
ReturnCode checkparm(struct Global *global,
		     int c,
		     DEFBUF *dp,
		     int quoting)	/* Preceded by a # ?	*/
{
  /*
   * Replace this param if it's defined.  Note that the macro name is a
   * possible replacement token. We stuff DEF_MAGIC in front of the token
   * which is treated as a LETTER by the token scanner and eaten by
   * the output routine. This prevents the macro expander from
   * looping if someone writes "#define foo foo".
   */
  
  int i;
  char *cp;
  ReturnCode ret=FPP_OK;
      
  scanid(global, c);                /* Get parm to tokenbuf */
  for (i = 0; i < global->nargs; i++) {     /* For each argument    */
    if (streq(global->parlist[i], global->tokenbuf)) {  /* If it's known */
#if OK_CONCAT
      if (quoting) {                    /* Special handling of  */
	ret=save(global, QUOTE_PARM);     /* #formal inside defn  */
	if(ret)
	  return(ret);
      }
#endif
      ret=save(global, i + MAC_PARM);     /* Save a magic cookie  */
      return(ret);		      /* And exit the search	*/
    }
  }
  if (streq(dp->name, global->tokenbuf))    /* Macro name in body?  */
    ret=save(global, DEF_MAGIC);            /* Save magic marker    */
  for (cp = global->tokenbuf; *cp != EOS;)  /* And save             */
    ret=save(global, *cp++);                /* The token itself     */
  return(ret);
}

INLINE FILE_LOCAL
ReturnCode stparmscan(struct Global *global, int delim)
{
  /*
   * Normal string parameter scan.
   */
  
  unsigned char	*wp;
  int i;
  ReturnCode ret;
      
  wp = (unsigned char *)global->workp;	/* Here's where it starts       */
  ret=scanstring(global, delim, save);
  if(ret)
    return(ret);		/* Exit on scanstring error	*/
  global->workp[-1] = EOS;		/* Erase trailing quote 	*/
  wp++;				/* -> first string content byte */
  for (i = 0; i < global->nargs; i++) {
    if (streq(global->parlist[i], (char *)wp)) {
      *wp++ = MAC_PARM + PAR_MAC;	/* Stuff a magic marker */
      *wp++ = (i + MAC_PARM);         /* Make a formal marker */
      *wp = wp[-3];			/* Add on closing quote */
      global->workp = (char *)wp + 1; 	/* Reset string end	*/
      return(FPP_OK);
    }
  }
  global->workp[-1] = wp[-1];	/* Nope, reset end quote.	*/
  return(FPP_OK);
}
  
void doundef(struct Global *global)
  /*
   * Remove the symbol from the defined list.
   * Called from the #control processor.
   */
{
  int c;
  if (type[(c = skipws(global))] != LET)
    cerror(global, ERROR_ILLEGAL_UNDEF);
  else {
    scanid(global, c);                         /* Get name to tokenbuf */
    (void) defendel(global, global->tokenbuf, TRUE);
  }
}

INLINE FILE_LOCAL  
ReturnCode textput(struct Global *global, char *text)
{
  /*
   * Put the string in the parm[] buffer.
   */

  int size;
  
  size = strlen(text) + 1;
  if ((global->parmp + size) >= &global->parm[NPARMWORK]) {
    cfatal(global, FATAL_MACRO_AREA_OVERFLOW);
    return(FPP_WORK_AREA_OVERFLOW);
  } else {
    strcpy(global->parmp, text);
    global->parmp += size;
  }
  return(FPP_OK);
}

FILE_LOCAL
ReturnCode charput(struct Global *global, int c)
{
  /*
   * Put the byte in the parm[] buffer.
   */
  
  if (global->parmp >= &global->parm[NPARMWORK]) {
    cfatal(global, FATAL_MACRO_AREA_OVERFLOW);
    return(FPP_WORK_AREA_OVERFLOW);
  }
  *global->parmp++ = c;
  return(FPP_OK);
}

/*
 *		M a c r o   E x p a n s i o n
 */

ReturnCode expand(struct Global *global, DEFBUF *tokenp)
{
  /*
   * Expand a macro.  Called from the cpp mainline routine (via subroutine
   * macroid()) when a token is found in the symbol table.  It calls
   * expcollect() to parse actual parameters, checking for the correct number.
   * It then creates a "file" containing a single line containing the
   * macro with actual parameters inserted appropriately.  This is
   * "pushed back" onto the input stream.  (When the get() routine runs
   * off the end of the macro line, it will dismiss the macro itself.)
   */
  int c;
  FILEINFO *file;
  ReturnCode ret=FPP_OK;
      
  /*
   * If no macro is pending, save the name of this macro
   * for an eventual error message.
   */
  if (global->recursion++ == 0)
    global->macro = tokenp;
  else if (global->recursion == RECURSION_LIMIT) {
    cerror(global, ERROR_RECURSIVE_MACRO, tokenp->name, global->macro->name);
    if (global->rec_recover) {
      do {
	c = get(global);
      } while (global->infile != NULL && global->infile->fp == NULL);
      unget(global);
      global->recursion = 0;
      return(FPP_OK);
    }
  }
  /*
   * Here's a macro to expand.
   */
  global->nargs = 0;			/* Formals counter	*/
  global->parmp = global->parm;		/* Setup parm buffer	*/
  switch (tokenp->nargs) {
  case (-2):                              /* __LINE__             */
      if(global->infile->fp)
	  /* This is a file */
	  sprintf(global->work, "%d", global->line);
      else
	  /* This is a macro! Find out the file line number! */
	  for (file = global->infile; file != NULL; file = file->parent) {
	      if (file->fp != NULL) {
		  sprintf(global->work, "%d", file->line);
		  break;
	      }
	  }
      ret=ungetstring(global, global->work);
      if(ret)
	  return(ret);
      break;
    
  case (-3):                              /* __FILE__             */
    for (file = global->infile; file != NULL; file = file->parent) {
      if (file->fp != NULL) {
	sprintf(global->work, "\"%s\"", (file->progname != NULL)
		? file->progname : file->filename);
	ret=ungetstring(global, global->work);
	if(ret)
	  return(ret);
	break;
      }
    }
    break;

  case (-4):				/* __FUNC__ */
    sprintf(global->work, "\"%s\"", global->functionname[0]?
	    global->functionname : "<unknown function>");
    ret=ungetstring(global, global->work);
    if(ret)
	return(ret);
    break;

  case (-5):                              /* __FUNC_LINE__ */
    sprintf(global->work, "%d", global->funcline);
    ret=ungetstring(global, global->work);
    if(ret)
      return(ret);
    break;

  default:
    /*
     * Nothing funny about this macro.
     */
    if (tokenp->nargs < 0) {
      cfatal(global, FATAL_ILLEGAL_MACRO, tokenp->name);
      return(FPP_ILLEGAL_MACRO);
    }
    while ((c = skipws(global)) == '\n')      /* Look for (, skipping */
      global->wrongline = TRUE;		/* spaces and newlines	*/
    if (c != '(') {
      /*
       * If the programmer writes
       *	#define foo() ...
       *	...
       *	foo [no ()]
       * just write foo to the output stream.
       */
      unget(global);
      cwarn(global, WARN_MACRO_NEEDS_ARGUMENTS, tokenp->name);

      /* fputs(tokenp->name, stdout); */
      Putstring(global, tokenp->name);
      return(FPP_OK);
    } else if (!(ret=expcollect(global))) {     /* Collect arguments    */
      if (tokenp->nargs != global->nargs) {     /* Should be an error?  */
	cwarn(global, WARN_WRONG_NUMBER_ARGUMENTS, tokenp->name);
      }
    } else {				/* Collect arguments		*/
      return(ret); /* We failed in argument colleting! */
    }
  case DEF_NOARGS:			/* No parameters just stuffs	*/
    ret=expstuff(global, tokenp->name, tokenp->repl); /* expand macro   */
  }					/* nargs switch 		*/
  return(ret);
}

INLINE FILE_LOCAL
ReturnCode expcollect(struct Global *global)
{
  /*
   * Collect the actual parameters for this macro.
   */

  int c;
  int paren;		    /* For embedded ()'s    */
  ReturnCode ret;
      
  for (;;) {
    paren = 0;			    /* Collect next arg.    */
    while ((c = skipws(global)) == '\n')/* Skip over whitespace */
      global->wrongline = TRUE;		/* and newlines.	*/
    if (c == ')') {                     /* At end of all args?  */
      /*
       * Note that there is a guard byte in parm[]
       * so we don't have to check for overflow here.
       */
      *global->parmp = EOS;	    /* Make sure terminated */
      break;			    /* Exit collection loop */
    }
    else if (global->nargs >= LASTPARM) {
      cfatal(global, FATAL_TOO_MANY_ARGUMENTS_EXPANSION);
      return(FPP_TOO_MANY_ARGUMENTS);
    }
    global->parlist[global->nargs++] = global->parmp; /* At start of new arg */
    for (;; c = cget(global)) {               /* Collect arg's bytes  */
      if (c == EOF_CHAR) {
	cerror(global, ERROR_EOF_IN_ARGUMENT);
	return(FPP_EOF_IN_MACRO); /* Sorry.               */
      }
      else if (c == '\\') {             /* Quote next character */
	charput(global, c);             /* Save the \ for later */
	charput(global, cget(global));  /* Save the next char.  */
	continue;			/* And go get another   */
      }
      else if (type[c] == QUO) {        /* Start of string?     */
	ret=scanstring(global, c, (ReturnCode (*)(struct Global *, int))charput); /* Scan it off    */
	if(ret)
	  return(ret);
	continue;			    /* Go get next char     */
      }
      else if (c == '(')                /* Worry about balance  */
	paren++;			/* To know about commas */
      else if (c == ')') {              /* Other side too       */
	if (paren == 0) {               /* At the end?          */
	  unget(global);                /* Look at it later     */
	  break;			/* Exit arg getter.     */
	}
	paren--;			/* More to come.        */
      }
      else if (c == ',' && paren == 0)  /* Comma delimits args  */
	break;
      else if (c == '\n')               /* Newline inside arg?  */
	global->wrongline = TRUE;	/* We'll need a #line   */
      charput(global, c);               /* Store this one       */
    }				        /* Collect an argument  */
    charput(global, EOS);               /* Terminate argument   */
  }				        /* Collect all args.    */
  return(FPP_OK);                       /* Normal return        */
}
  
  
#if OK_CONCAT
  
INLINE FILE_LOCAL
char *doquoting(char *to, char *from)
{
  *to++ = '"';
  while (*from) {
    if (*from == '\\' || *from == '"')
      *to++ = '\\';
    *to++ = *from++;
  }
  *to++ = '"';
      
  return to;
}
  
#endif
  
ReturnCode expstuff(struct Global *global,
		    char *MacroName,
		    char *MacroReplace)
{
  /*
   * Stuff the macro body, replacing formal parameters by actual parameters.
   */
  int c;		/* Current character	*/
  char *inp;		/* -> repl string	*/
  char *defp;		/* -> macro output buff */
  int size;		/* Actual parm. size	*/
  char *defend;		/* -> output buff end	*/
  int string_magic;	/* String formal hack	*/
  FILEINFO *file;	/* Funny #include	*/
  ReturnCode ret;
#if OK_CONCAT
  char quoting;	/* Quote macro argument */
#endif
      
  ret = getfile(global, NBUFF, MacroName, &file);
  if(ret)
    return(ret);
  inp = MacroReplace;			/* -> macro replacement */
  defp = file->buffer;			/* -> output buffer	*/
  defend = defp + (NBUFF - 1);              /* Note its end         */
  if (inp != NULL) {
    quoting = 0;
    while ((c = (*inp++ & 0xFF)) != EOS) {
#if OK_CONCAT
      if (c == QUOTE_PARM) {                /* Special token for #  */
	quoting = 1;			/* set flag, for later	*/
	continue;				/* Get next character	*/
      }
#endif
      if (c >= MAC_PARM && c <= (MAC_PARM + PAR_MAC)) {
	string_magic = (c == (MAC_PARM + PAR_MAC));
	if (string_magic)
	  c = (*inp++ & 0xFF);
	/*
	 * Replace formal parameter by actual parameter string.
	 */
	if ((c -= MAC_PARM) < global->nargs) {
	  size = strlen(global->parlist[c]);
#if OK_CONCAT
	  if (quoting) {
	    size++;
	    size *= 2;		/* worst case condition */
	  }
#endif
	  if ((defp + size) >= defend) {
	    cfatal(global, FATAL_OUT_OF_SPACE_IN_ARGUMENT, MacroName);
	    return(FPP_OUT_OF_SPACE_IN_MACRO_EXPANSION);
	  }
	  /*
	   * Erase the extra set of quotes.
	   */
	  if (string_magic && defp[-1] == global->parlist[c][0]) {
	    strcpy(defp-1, global->parlist[c]);
	    defp += (size - 2);
	  }
#if OK_CONCAT
else if (quoting)
  defp = doquoting(defp, global->parlist[c]);
#endif
else {
  strcpy(defp, global->parlist[c]);
  defp += size;
}
	}
      }
      else if (defp >= defend) {
	cfatal(global, FATAL_OUT_OF_SPACE_IN_ARGUMENT, MacroName);
	return(FPP_OUT_OF_SPACE_IN_MACRO_EXPANSION);
      } else
	*defp++ = c;
      quoting = 0;
    }
  }
  *defp = EOS;
  return(FPP_OK);
}

INLINE FILE_LOCAL ReturnCode evallex(struct Global *, int, int *);
INLINE FILE_LOCAL ReturnCode dosizeof(struct Global *, int *);
INLINE FILE_LOCAL int bittest(int);
INLINE FILE_LOCAL int evalnum(struct Global *, int);
INLINE FILE_LOCAL int evalchar(struct Global *, int);
INLINE FILE_LOCAL int *evaleval(struct Global *, int *, int, int);

/*
 * Evaluate an #if expression.
 */

static char *opname[] = {		/* For debug and error messages */
  "end of expression", "val", "id",
  "+",   "-",  "*",  "/",  "%",
  "<<", ">>",  "&",  "|",  "^",
  "==", "!=",  "<", "<=", ">=",  ">",
  "&&", "||",  "?",  ":",  ",",
  "unary +", "unary -", "~", "!",  "(",  ")", "(none)",
};

/*
 * opdope[] has the operator precedence:
 *     Bits
 *	  7	Unused (so the value is always positive)
 *	6-2	Precedence (000x .. 017x)
 *	1-0	Binary op. flags:
 *	    01	The binop flag should be set/cleared when this op is seen.
 *	    10	The new value of the binop flag.
 * Note:  Expected, New binop
 * constant	0	1	Binop, end, or ) should follow constants
 * End of line	1	0	End may not be preceeded by an operator
 * binary	1	0	Binary op follows a value, value follows.
 * unary	0	0	Unary op doesn't follow a value, value follows
 *   (          0       0       Doesn't follow value, value or unop follows
 *   )		1	1	Follows value.	Op follows.
 */

static char opdope[OP_MAX] = {
  0001, 				/* End of expression		*/
  0002, 				/* Digit			*/
  0000, 				/* Letter (identifier)          */
  0141, 0141, 0151, 0151, 0151, 	/* ADD, SUB, MUL, DIV, MOD	*/
  0131, 0131, 0101, 0071, 0071, 	/* ASL, ASR, AND,  OR, XOR	*/
  0111, 0111, 0121, 0121, 0121, 0121,	/*  EQ,  NE,  LT,  LE,	GE,  GT */
  0061, 0051, 0041, 0041, 0031, 	/* ANA, ORO, QUE, COL, CMA	*/
  /*
   * Unary op's follow
   */
  0160, 0160, 0160, 0160,		/* NEG, PLU, COM, NOT		*/
  0170, 0013, 0023,			/* LPA, RPA, END		*/
};
/*
 * OP_QUE and OP_RPA have alternate precedences:
 */
#define OP_RPA_PREC	0013
#define OP_QUE_PREC	0034

/*
 * S_ANDOR and S_QUEST signal "short-circuit" boolean evaluation, so that
 *	#if FOO != 0 && 10 / FOO ...
 * doesn't generate an error message.  They are stored in optab.skip.
 */
#define S_ANDOR 	2
#define S_QUEST 	1

typedef struct optab {
  int	op;			/* Operator			*/
  char	prec;			/* Its precedence		*/
  char	skip;			/* Short-circuit: TRUE to skip	*/
} OPTAB;
     
#ifdef	nomacargs
     FILE_LOCAL int
       isbinary(op)
     int op;
{
  return (op >= FIRST_BINOP && op <= LAST_BINOP);
}

FILE_LOCAL int
  isunary(op)
int op;
{
  return (op >= FIRST_UNOP && op <= LAST_UNOP);
}
#else
#define isbinary(op)    (op >= FIRST_BINOP && op <= LAST_BINOP)
#define isunary(op)     (op >= FIRST_UNOP  && op <= LAST_UNOP)
#endif

/*
 * The following definitions are used to specify basic variable sizes.
 */

#if OK_SIZEOF

#ifndef S_CHAR
#define S_CHAR		(sizeof (char))
#endif
#ifndef S_SINT
#ifdef manx		/* Aztec/Manx C does not like "short int" */
#define S_SINT		(sizeof (short))
#else
#define S_SINT		(sizeof (short int))
#endif
#endif
#ifndef S_INT
#define S_INT		(sizeof (int))
#endif
#ifndef S_LINT
#define S_LINT		(sizeof (long int))
#endif
#ifndef S_FLOAT
#define S_FLOAT 	(sizeof (float))
#endif
#ifndef S_DOUBLE
#define S_DOUBLE	(sizeof (double))
#endif
#ifndef S_PCHAR
#define S_PCHAR 	(sizeof (char *))
#endif
#ifndef S_PSINT
#ifdef manx		/* Aztec/Manx C does not like "short int" */
#define S_PSINT 	(sizeof (short *))
#else
#define S_PSINT 	(sizeof (short int *))
#endif
#endif
#ifndef S_PINT
#define S_PINT		(sizeof (int *))
#endif
#ifndef S_PLINT
#define S_PLINT 	(sizeof (long int *))
#endif
#ifndef S_PFLOAT
#define S_PFLOAT	(sizeof (float *))
#endif
#ifndef S_PDOUBLE
#define S_PDOUBLE	(sizeof (double *))
#endif
#ifndef S_PFPTR
#define S_PFPTR 	(sizeof (int (*)()))
#endif


typedef struct types {
  short	type;			/* This is the bit if		*/
  char	*name;			/* this is the token word	*/
} TYPES;

static TYPES basic_types[] = {
  { T_CHAR,	"char",         },
  { T_INT,	"int",          },
  { T_FLOAT,	"float",        },
  { T_DOUBLE,	"double",       },
  { T_SHORT,	"short",        },
  { T_LONG,	"long",         },
  { T_SIGNED,	"signed",       },
  { T_UNSIGNED,	"unsigned",     },
  { 0,		NULL,		},	/* Signal end		*/
};

/*
 * Test_table[] is used to test for illegal combinations.
 */
static short test_table[] = {
  T_FLOAT | T_DOUBLE | T_LONG | T_SHORT,
  T_FLOAT | T_DOUBLE | T_CHAR | T_INT,
  T_FLOAT | T_DOUBLE | T_SIGNED | T_UNSIGNED,
  T_LONG  | T_SHORT  | T_CHAR,
  0						/* end marker	*/
  };

/*
 * The order of this table is important -- it is also referenced by
 * the command line processor to allow run-time overriding of the
 * built-in size values.  The order must not be changed:
 *	char, short, int, long, float, double (func pointer)
 */
SIZES size_table[] = {
  { T_CHAR,	S_CHAR, 	S_PCHAR 	},	/* char 	*/
  { T_SHORT,	S_SINT, 	S_PSINT 	},	/* short int	*/
  { T_INT,	S_INT,		S_PINT		},	/* int		*/
  { T_LONG,	S_LINT, 	S_PLINT 	},	/* long 	*/
  { T_FLOAT,	S_FLOAT,	S_PFLOAT	},	/* float	*/
  { T_DOUBLE,	S_DOUBLE,	S_PDOUBLE	},	/* double	*/
  { T_FPTR,	0,		S_PFPTR 	},	/* int (*())    */
  { 0,		0,		0		},	/* End of table */
};

#endif /* OK_SIZEOF */

ReturnCode eval(struct Global *global, int *eval)
{
  /*
   * Evaluate an expression.  Straight-forward operator precedence.
   * This is called from control() on encountering an #if statement.
   * It calls the following routines:
   * evallex	Lexical analyser -- returns the type and value of
   *		the next input token.
   * evaleval	Evaluate the current operator, given the values on
   *		the value stack.  Returns a pointer to the (new)
   *		value stack.
   * For compatiblity with older cpp's, this return returns 1 (TRUE)
   * if a syntax error is detected.
   */
  int op;		/* Current operator		*/
  int *valp;		/* -> value vector		*/
  OPTAB *opp;		/* Operator stack		*/
  int prec;		/* Op precedence		*/
  int binop;		/* Set if binary op. needed	*/
  int op1;		/* Operand from stack		*/
  int skip;		/* For short-circuit testing	*/
  int value[NEXP];	/* Value stack			*/
  OPTAB opstack[NEXP];	/* Operand stack		*/
  ReturnCode ret;
  char again=TRUE;
  
  valp = value;
  opp = opstack;
  opp->op = OP_END;		/* Mark bottom of stack 	*/
  opp->prec = opdope[OP_END];	/* And its precedence		*/
  opp->skip = 0;		/* Not skipping now		*/
  binop = 0;

  while(again) {
    ret=evallex(global, opp->skip, &op);
    if(ret)
      return(ret);
    if (op == OP_SUB && binop == 0)
      op = OP_NEG;			/* Unary minus		*/
    else if (op == OP_ADD && binop == 0)
      op = OP_PLU;			/* Unary plus		*/
    else if (op == OP_FAIL) {
      *eval=1;                    	/* Error in evallex     */
      return(FPP_OK);
    }
    if (op == DIG) {                      /* Value?               */
      if (binop != 0) {
	cerror(global, ERROR_MISPLACED_CONSTANT);
	*eval=1;
	return(FPP_OK);
      } else if (valp >= &value[NEXP-1]) {
	cerror(global, ERROR_IF_OVERFLOW);
	*eval=1;
	return(FPP_OK);
      } else {
	*valp++ = global->evalue;
	binop = 1;
      }
      again=TRUE;
      continue;
    } else if (op > OP_END) {
      cerror(global, ERROR_ILLEGAL_IF_LINE);
      *eval=1;
      return(FPP_OK);
    }
    prec = opdope[op];
    if (binop != (prec & 1)) {
      cerror(global, ERROR_OPERATOR, opname[op]);
      *eval=1;
      return(FPP_OK);
    }
    binop = (prec & 2) >> 1;
    do {
      if (prec > opp->prec) {
	if (op == OP_LPA)
	  prec = OP_RPA_PREC;
	else if (op == OP_QUE)
	  prec = OP_QUE_PREC;
	op1 = opp->skip;		/* Save skip for test	*/
	/*
	 * Push operator onto op. stack.
	 */
	opp++;
	if (opp >= &opstack[NEXP]) {
	  cerror(global, ERROR_EXPR_OVERFLOW, opname[op]);
	  *eval=1;
	  return(FPP_OK);
	}
	opp->op = op;
	opp->prec = prec;
	skip = (valp[-1] != 0);         /* Short-circuit tester */
	/*
	 * Do the short-circuit stuff here.  Short-circuiting
	 * stops automagically when operators are evaluated.
	 */
	if ((op == OP_ANA && !skip)
	    || (op == OP_ORO && skip))
	  opp->skip = S_ANDOR;	/* And/or skip starts	*/
	else if (op == OP_QUE)          /* Start of ?: operator */
	  opp->skip = (op1 & S_ANDOR) | ((!skip) ? S_QUEST : 0);
	else if (op == OP_COL) {        /* : inverts S_QUEST    */
	  opp->skip = (op1 & S_ANDOR)
	    | (((op1 & S_QUEST) != 0) ? 0 : S_QUEST);
	}
	else {				/* Other ops leave	*/
	  opp->skip = op1;		/*  skipping unchanged. */
	}
	again=TRUE;
	continue;
      }
      /*
       * Pop operator from op. stack and evaluate it.
       * End of stack and '(' are specials.
       */
      skip = opp->skip;			/* Remember skip value	*/
      switch ((op1 = opp->op)) {          /* Look at stacked op   */
      case OP_END:			/* Stack end marker	*/
	if (op == OP_EOE) {
	  *eval=valp[-1];     		/* Finished ok.         */
	  return(FPP_OK);
	}
	/* Read another op.	*/
	again=TRUE;
	continue;
      case OP_LPA:			/* ( on stack           */
	if (op != OP_RPA) {             /* Matches ) on input   */
	  cerror(global, ERROR_UNBALANCED_PARENS, opname[op]);
	  *eval=1;
	  return(FPP_OK);
	}
	opp--;				/* Unstack it		*/
	/* -- Fall through 	*/
      case OP_QUE:
	/* Evaluate true expr.	*/
	again=TRUE;
	continue;
      case OP_COL:			/* : on stack.		*/
	opp--;				/* Unstack :		*/
	if (opp->op != OP_QUE) {        /* Matches ? on stack?  */
	  cerror(global, ERROR_MISPLACED, opname[opp->op]);
	  *eval=1;
	  return(FPP_OK);
	}
	/*
	 * Evaluate op1.
	 */
      default:				/* Others:		*/
	opp--;				/* Unstack the operator */
	valp = evaleval(global, valp, op1, skip);
	again=FALSE;
      }					/* op1 switch end	*/
    } while (!again);			/* Stack unwind loop	*/
  }
  return(FPP_OK);
}

INLINE FILE_LOCAL
ReturnCode evallex(struct Global *global,
		   int skip,	/* TRUE if short-circuit evaluation */
		   int *op)
{
  /*
   * Set *op to next eval operator or value. Called from eval(). It
   * calls a special-purpose routines for 'char' strings and
   * numeric values:
   * evalchar	called to evaluate 'x'
   * evalnum	called to evaluate numbers.
   */

  int c, c1, t;
  ReturnCode ret;
  char loop;
  
  do { /* while(loop); */
  /* again: */
    loop=FALSE;
    do {					/* Collect the token	*/
      c = skipws(global);
      if((ret=macroid(global, &c)))
      return(ret);
      if (c == EOF_CHAR || c == '\n') {
	unget(global);
	*op=OP_EOE;           /* End of expression    */
	return(FPP_OK);
      }
    } while ((t = type[c]) == LET && catenate(global, 0, &ret) && !ret);
    if(ret)
      /* If the loop was broken because of a fatal error! */
      return(ret);
    if (t == INV) {                         /* Total nonsense       */
      if (!skip) {
	if (isascii(c) && isprint(c))
	  cerror(global, ERROR_ILLEGAL_CHARACTER, c);
	else
	  cerror(global, ERROR_ILLEGAL_CHARACTER2, c);
      }
      return(FPP_ILLEGAL_CHARACTER);
    } else if (t == QUO) {                  /* ' or "               */
      if (c == '\'') {                    /* Character constant   */
	global->evalue = evalchar(global, skip);  /* Somewhat messy       */
	*op=DIG;                          /* Return a value       */
	return(FPP_OK);
      }
      cerror(global, ERROR_STRING_IN_IF);
      return(FPP_CANT_USE_STRING_IN_IF);
    } else if (t == LET) {                  /* ID must be a macro   */
      if (streq(global->tokenbuf, "defined")) {   /* Or defined name      */
	c1 = c = skipws(global);
	if (c == '(')                     /* Allow defined(name)  */
	  c = skipws(global);
	if (type[c] == LET) {
	  global->evalue = (lookid(global, c) != NULL);
	  if (c1 != '('                   /* Need to balance      */
	      || skipws(global) == ')') { /* Did we balance?      */
	    *op=DIG;
	    return(FPP_OK);               /* Parsed ok            */
	  }
	}
	cerror(global, ERROR_DEFINED_SYNTAX);
	return(FPP_BAD_IF_DEFINED_SYNTAX);
      }
#if OK_SIZEOF
else if (streq(global->tokenbuf, "sizeof")) { /* New sizeof hackery   */
  ret=dosizeof(global, op);             /* Gets own routine     */
  return(ret);
}
#endif
      global->evalue = 0;
      *op=DIG;
      return(FPP_OK);
    }
    else if (t == DIG) {                  /* Numbers are harder   */
      global->evalue = evalnum(global, c);
    }
    else if (strchr("!=<>&|\\", c) != NULL) {
      /*
       * Process a possible multi-byte lexeme.
       */
      c1 = cget(global);                        /* Peek at next char    */
      switch (c) {
      case '!':
	if (c1 == '=') {
	  *op=OP_NE;
	  return(FPP_OK);
	}
	break;
	
      case '=':
	if (c1 != '=') {                  /* Can't say a=b in #if */
	  unget(global);
	  cerror(global, ERROR_ILLEGAL_ASSIGN);
	  return (FPP_IF_ERROR);
	}
	*op=OP_EQ;
	return(FPP_OK);
	
      case '>':
      case '<':
	if (c1 == c) {
	  *op= ((c == '<') ? OP_ASL : OP_ASR);
	  return(FPP_OK);
	} else if (c1 == '=') {
	  *op= ((c == '<') ? OP_LE  : OP_GE);
	  return(FPP_OK);
	}
	break;
	
      case '|':
      case '&':
	if (c1 == c) {
	  *op= ((c == '|') ? OP_ORO : OP_ANA);
	  return(FPP_OK);
	}
	break;
      
      case '\\':
	if (c1 == '\n') {                  /* Multi-line if        */
	  loop=TRUE;
	  break;
	}
	cerror(global, ERROR_ILLEGAL_BACKSLASH);
	return(FPP_IF_ERROR);
      }
      if(!loop)
	unget(global);
    }
  } while(loop);
  *op=t;
  return(FPP_OK);
}

#if OK_SIZEOF

INLINE FILE_LOCAL
ReturnCode dosizeof(struct Global *global, int *result)
{
  /*
   * Process the sizeof (basic type) operation in an #if string.
   * Sets evalue to the size and returns
   *	DIG		success
   *	OP_FAIL 	bad parse or something.
   */
  int c;
  TYPES *tp;
  SIZES *sizp;
  short *testp;
  short typecode;
  ReturnCode ret;
  
  if ((c = skipws(global)) != '(') {
    unget(global);
    cerror(global, ERROR_SIZEOF_SYNTAX);
    return(FPP_SIZEOF_ERROR);
  }
  /*
   * Scan off the tokens.
   */
  typecode = 0;
  while ((c = skipws(global))) {
    if((ret=macroid(global, &c)))
      return(ret);
    /* (I) return on fail! */
    if (c  == EOF_CHAR || c == '\n') {
      /* End of line is a bug */
      unget(global);
      cerror(global, ERROR_SIZEOF_SYNTAX);
      return(FPP_SIZEOF_ERROR);
    } else if (c == '(') {                /* thing (*)() func ptr */
      if (skipws(global) == '*'
	  && skipws(global) == ')') {         /* We found (*)         */
	if (skipws(global) != '(')            /* Let () be optional   */
	  unget(global);
	else if (skipws(global) != ')') {
	  unget(global);
	  cerror(global, ERROR_SIZEOF_SYNTAX);
	  return(FPP_SIZEOF_ERROR);
	}
	typecode |= T_FPTR; 		/* Function pointer	*/
      } else {				/* Junk is a bug	*/
	unget(global);
	cerror(global, ERROR_SIZEOF_SYNTAX);
	return(FPP_SIZEOF_ERROR);
      }
    }
    else if (type[c] != LET)            /* Exit if not a type   */
      break;
    else if (!catenate(global, 0, &ret) && !ret) { /* Maybe combine tokens */
      /*
       * Look for this unexpandable token in basic_types.
       * The code accepts "int long" as well as "long int"
       * which is a minor bug as bugs go (and one shared with
       * a lot of C compilers).
       */
      for (tp = basic_types; tp->name != NULLST; tp++) {
	if (streq(global->tokenbuf, tp->name))
	  break;
      }
      if (tp->name == NULLST) {
	cerror(global, ERROR_SIZEOF_UNKNOWN, global->tokenbuf);
	return(FPP_SIZEOF_ERROR);
      }
      typecode |= tp->type;		/* Or in the type bit	*/
    } else if(ret)
      return(ret);
  }
  /*
   * We are at the end of the type scan.	Chew off '*' if necessary.
   */
  if (c == '*') {
    typecode |= T_PTR;
    c = skipws(global);
  }
  if (c == ')') {                         /* Last syntax check    */
    for (testp = test_table; *testp != 0; testp++) {
      if (!bittest(typecode & *testp)) {
	cerror(global, ERROR_SIZEOF_ILLEGAL_TYPE);
	return(FPP_SIZEOF_ERROR);
      }
    }
    /*
     * We assume that all function pointers are the same size:
     *		sizeof (int (*)()) == sizeof (float (*)())
     * We assume that signed and unsigned don't change the size:
     *		sizeof (signed int) == (sizeof unsigned int)
     */
    if ((typecode & T_FPTR) != 0)       /* Function pointer     */
      typecode = T_FPTR | T_PTR;
    else {				/* Var or var * datum	*/
      typecode &= ~(T_SIGNED | T_UNSIGNED);
      if ((typecode & (T_SHORT | T_LONG)) != 0)
	typecode &= ~T_INT;
    }
    if ((typecode & ~T_PTR) == 0) {
      cerror(global, ERROR_SIZEOF_NO_TYPE);
      return(FPP_SIZEOF_ERROR);
    }
    /*
     * Exactly one bit (and possibly T_PTR) may be set.
     */
    for (sizp = size_table; sizp->bits != 0; sizp++) {
      if ((typecode & ~T_PTR) == sizp->bits) {
	global->evalue = ((typecode & T_PTR) != 0)
	  ? sizp->psize : sizp->size;
	*result=DIG;
	return(FPP_OK);
      }
    }					/* We shouldn't fail    */
    cerror(global, ERROR_SIZEOF_BUG, typecode);
    return(FPP_SIZEOF_ERROR);
  }
  unget(global);
  cerror(global, ERROR_SIZEOF_SYNTAX);
  return(FPP_SIZEOF_ERROR);
}

INLINE FILE_LOCAL
int bittest(int value)
{
  /*
   * TRUE if value is zero or exactly one bit is set in value.
   */

#if (4096 & ~(-4096)) == 0
  return ((value & ~(-value)) == 0);
#else
  /*
   * Do it the hard way (for non 2's complement machines)
   */
  return (value == 0 || value ^ (value - 1) == (value * 2 - 1));
#endif
}

#endif /* OK_SIZEOF */

INLINE FILE_LOCAL
int evalnum(struct Global *global, int c)
{
  /*
   * Expand number for #if lexical analysis.  Note: evalnum recognizes
   * the unsigned suffix, but only returns a signed int value.
   */

  int value;
  int base;
  int c1;
  
  if (c != '0')
    base = 10;
  else if ((c = cget(global)) == 'x' || c == 'X') {
    base = 16;
    c = cget(global);
  }
  else base = 8;
  value = 0;
  for (;;) {
    c1 = c;
    if (isascii(c) && isupper(c1))
      c1 = tolower(c1);
    if (c1 >= 'a')
      c1 -= ('a' - 10);
    else c1 -= '0';
    if (c1 < 0 || c1 >= base)
      break;
    value *= base;
    value += c1;
    c = cget(global);
  }
  if (c == 'u' || c == 'U')       /* Unsigned nonsense            */
    c = cget(global);
  unget(global);
  return (value);
}

INLINE FILE_LOCAL
int evalchar(struct Global *global,
	     int skip)		/* TRUE if short-circuit evaluation	*/
     /*
      * Get a character constant
      */
{
  int c;
  int value;
  int count;
  
  global->instring = TRUE;
  if ((c = cget(global)) == '\\') {
    switch ((c = cget(global))) {
    case 'a':                           /* New in Standard      */
#if ('a' == '\a' || '\a' == ALERT)
      value = ALERT;			/* Use predefined value */
#else
      value = '\a';                   /* Use compiler's value */
#endif
      break;
      
    case 'b':
      value = '\b';
      break;
      
    case 'f':
      value = '\f';
      break;
      
    case 'n':
      value = '\n';
      break;
      
    case 'r':
      value = '\r';
      break;
      
    case 't':
      value = '\t';
      break;
      
    case 'v':                           /* New in Standard      */
#if ('v' == '\v' || '\v' == VT)
      value = VT;			/* Use predefined value */
#else
      value = '\v';                   /* Use compiler's value */
#endif
      break;
      
    case 'x':                           /* '\xFF'               */
      count = 3;
      value = 0;
      while ((((c = get(global)) >= '0' && c <= '9')
	      || (c >= 'a' && c <= 'f')
	      || (c >= 'A' && c <= 'F'))
	     && (--count >= 0)) {
	value *= 16;
	value += (c <= '9') ? (c - '0') : ((c & 0xF) + 9);
      }
      unget(global);
      break;
      
    default:
      if (c >= '0' && c <= '7') {
	count = 3;
	value = 0;
	while (c >= '0' && c <= '7' && --count >= 0) {
	  value *= 8;
	  value += (c - '0');
	  c = get(global);
	}
	unget(global);
      } else
	value = c;
      break;
    }
  } else if (c == '\'')
    value = 0;
  else value = c;
  /*
   * We warn on multi-byte constants and try to hack
   * (big|little)endian machines.
   */
#if BIG_ENDIAN
  count = 0;
#endif
  while ((c = get(global)) != '\'' && c != EOF_CHAR && c != '\n') {
    if (!skip)
      cwarn(global, WARN_MULTIBYTE_NOT_PORTABLE, c);
#if BIG_ENDIAN
    count += BITS_CHAR;
    value += (c << count);
#else
    value <<= BITS_CHAR;
    value += c;
#endif
  }
  global->instring = FALSE;
  return (value);
}

INLINE FILE_LOCAL
int *evaleval(struct Global *global,
	      int *valp,
	      int op,
	      int skip)		/* TRUE if short-circuit evaluation	*/
{
  /*
   * Apply the argument operator to the data on the value stack.
   * One or two values are popped from the value stack and the result
   * is pushed onto the value stack.
   *
   * OP_COL is a special case.
   *
   * evaleval() returns the new pointer to the top of the value stack.
   */
  int v1, v2;
  
  if (isbinary(op))
    v2 = *--valp;
  v1 = *--valp;
  switch (op) {
  case OP_EOE:
    break;
  case OP_ADD:
    v1 += v2;
    break;
  case OP_SUB:
    v1 -= v2;
    break;
  case OP_MUL:
    v1 *= v2;
    break;
  case OP_DIV:
  case OP_MOD:
    if (v2 == 0) {
      if (!skip) {
	cwarn(global, WARN_DIVISION_BY_ZERO,
	      (op == OP_DIV) ? "divide" : "mod");
      }
      v1 = 0;
    }
    else if (op == OP_DIV)
      v1 /= v2;
    else
      v1 %= v2;
    break;
  case OP_ASL:
    v1 <<= v2;
    break;
  case OP_ASR:
    v1 >>= v2;
    break;
  case OP_AND:
    v1 &= v2;
    break;
  case OP_OR:
    v1 |= v2;
    break;
  case OP_XOR:
    v1 ^= v2;
    break;
  case OP_EQ:
    v1 = (v1 == v2);
    break;
  case OP_NE:
    v1 = (v1 != v2);
    break;
  case OP_LT:
    v1 = (v1 < v2);
    break;
  case OP_LE:
    v1 = (v1 <= v2);
    break;
  case OP_GE:
    v1 = (v1 >= v2);
    break;
  case OP_GT:
    v1 = (v1 > v2);
    break;
  case OP_ANA:
    v1 = (v1 && v2);
    break;
  case OP_ORO:
    v1 = (v1 || v2);
    break;
  case OP_COL:
    /*
     * v1 has the "true" value, v2 the "false" value.
     * The top of the value stack has the test.
     */
    v1 = (*--valp) ? v1 : v2;
    break;
  case OP_NEG:
    v1 = (-v1);
    break;
  case OP_PLU:
    break;
  case OP_COM:
    v1 = ~v1;
    break;
  case OP_NOT:
    v1 = !v1;
    break;
  default:
    cerror(global, ERROR_IF_OPERAND, op);
    v1 = 0;
  }
  *valp++ = v1;
  return (valp);
}


INLINE FILE_LOCAL void outadefine(struct Global *, DEFBUF *);
INLINE FILE_LOCAL void domsg(struct Global *, ErrorCode, va_list);

/*
 * skipnl()     skips over input text to the end of the line.
 * skipws()     skips over "whitespace" (spaces or tabs), but
 *              not skip over the end of the line.  It skips over
 *              TOK_SEP, however (though that shouldn't happen).
 * scanid()     reads the next token (C identifier) into tokenbuf.
 *              The caller has already read the first character of
 *              the identifier.  Unlike macroid(), the token is
 *              never expanded.
 * macroid()    reads the next token (C identifier) into tokenbuf.
 *              If it is a #defined macro, it is expanded, and
 *              macroid() returns TRUE, otherwise, FALSE.
 * catenate()   Does the dirty work of token concatenation, TRUE if it did.
 * scanstring() Reads a string from the input stream, calling
 *              a user-supplied function for each character.
 *              This function may be output() to write the
 *              string to the output file, or save() to save
 *              the string in the work buffer.
 * scannumber() Reads a C numeric constant from the input stream,
 *              calling the user-supplied function for each
 *              character.  (output() or save() as noted above.)
 * save()       Save one character in the work[] buffer.
 * savestring() Saves a string in malloc() memory.
 * getfile()    Initialize a new FILEINFO structure, called when
 *              #include opens a new file, or a macro is to be
 *              expanded.
 * Getmem()     Get a specified number of bytes from malloc memory.
 * output()     Write one character to stdout (calling Putchar) --
 *              implemented as a function so its address may be
 *              passed to scanstring() and scannumber().
 * lookid()     Scans the next token (identifier) from the input
 *              stream.  Looks for it in the #defined symbol table.
 *              Returns a pointer to the definition, if found, or NULL
 *              if not present.  The identifier is stored in tokenbuf.
 * defnedel()   Define enter/delete subroutine.  Updates the
 *              symbol table.
 * get()        Read the next byte from the current input stream,
 *              handling end of (macro/file) input and embedded
 *              comments appropriately.  Note that the global
 *              instring is -- essentially -- a parameter to get().
 * cget()       Like get(), but skip over TOK_SEP.
 * unget()      Push last gotten character back on the input stream.
 * cerror()     This routine format an print messages to the user.
 */

/*
 * This table must be rewritten for a non-Ascii machine.
 *
 * Note that several "non-visible" characters have special meaning:
 * Hex 1C QUOTE_PARM --a flag for # stringifying
 * Hex 1D DEF_MAGIC -- a flag to prevent #define recursion.
 * Hex 1E TOK_SEP   -- a delimiter for ## token concatenation
 * Hex 1F COM_SEP   -- a zero-width whitespace for comment concatenation
 */

#ifndef OS9
#if (TOK_SEP != 0x1E || COM_SEP != 0x1F || DEF_MAGIC != 0x1D)
#error "<< error type table isn't correct >>"
#endif
#endif

#if OK_DOLLAR
#define DOL     LET
#else
#define DOL     000
#endif

char type[256] = {              /* Character type codes    Hex          */
  END,   000,    000,   000,   000,   000,   000,   000, /* 00          */
  000,   SPA,    000,   000,   000,   000,   000,   000, /* 08          */
  000,   000,    000,   000,   000,   000,   000,   000, /* 10          */
  000,   000,    000,   000,   000,   LET,   000,   SPA, /* 18          */
  SPA,   OP_NOT, QUO,   000,   DOL,   OP_MOD,OP_AND,QUO, /* 20  !"#$%&' */
  OP_LPA,OP_RPA,OP_MUL,OP_ADD, 000,OP_SUB,   DOT,OP_DIV, /* 28 ()*+,-./ */
  DIG,   DIG,    DIG,   DIG,   DIG,   DIG,   DIG,   DIG, /* 30 01234567 */
  DIG,   DIG,OP_COL,    000, OP_LT, OP_EQ, OP_GT,OP_QUE, /* 38 89:;<=>? */
  000,   LET,    LET,   LET,   LET,   LET,   LET,   LET, /* 40 @ABCDEFG */
  LET,   LET,    LET,   LET,   LET,   LET,   LET,   LET, /* 48 HIJKLMNO */
  LET,   LET,    LET,   LET,   LET,   LET,   LET,   LET, /* 50 PQRSTUVW */
  LET,   LET,    LET,   000,   BSH,   000,OP_XOR,   LET, /* 58 XYZ[\]^_ */
  000,   LET,    LET,   LET,   LET,   LET,   LET,   LET, /* 60 `abcdefg */
  LET,   LET,    LET,   LET,   LET,   LET,   LET,   LET, /* 68 hijklmno */
  LET,   LET,    LET,   LET,   LET,   LET,   LET,   LET, /* 70 pqrstuvw */
  LET,   LET,    LET,   000, OP_OR,   000,OP_NOT,   000, /* 78 xyz{|}~  */
  000,   000,    000,   000,   000,   000,   000,   000, /*   80 .. FF  */
  000,   000,    000,   000,   000,   000,   000,   000, /*   80 .. FF  */
  000,   000,    000,   000,   000,   000,   000,   000, /*   80 .. FF  */
  000,   000,    000,   000,   000,   000,   000,   000, /*   80 .. FF  */
  000,   000,    000,   000,   000,   000,   000,   000, /*   80 .. FF  */
  000,   000,    000,   000,   000,   000,   000,   000, /*   80 .. FF  */
  000,   000,    000,   000,   000,   000,   000,   000, /*   80 .. FF  */
  000,   000,    000,   000,   000,   000,   000,   000, /*   80 .. FF  */
};

void skipnl(struct Global *global)
{
  /*
   * Skip to the end of the current input line.
   */
  int c;

  do {                          /* Skip to newline      */
    c = get(global);
  } while (c != '\n' && c != EOF_CHAR);
  return;
}

int skipws(struct Global *global)
{
  /*
   * Skip over whitespace
   */
  int c;

  do {                          /* Skip whitespace      */
    c = get(global);
#if COMMENT_INVISIBLE
  } while (type[c] == SPA || c == COM_SEP);
#else
} while (type[c] == SPA);
#endif
 return(c);
}

void scanid(struct Global *global,
  int c)                                /* First char of id     */
{
  /*
   * Get the next token (an id) into the token buffer.
   * Note: this code is duplicated in lookid().
   * Change one, change both.
   */

  int ct;

  if (c == DEF_MAGIC)                     /* Eat the magic token  */
    c = get(global);                      /* undefiner.           */
  ct = 0;
  do
    {
      if (ct == global->tokenbsize)
        global->tokenbuf = realloc(global->tokenbuf, 1 +
                                   (global->tokenbsize *= 2));
      global->tokenbuf[ct++] = c;
      c = get(global);
    }
  while (type[c] == LET || type[c] == DIG);
  unget(global);
  global->tokenbuf[ct] = EOS;
}

ReturnCode macroid(struct Global *global, int *c)
{
  /*
   * If c is a letter, scan the id.  if it's #defined, expand it and scan
   * the next character and try again.
   *
   * Else, return the character. If type[c] is a LET, the token is in tokenbuf.
   */
  DEFBUF *dp;
  ReturnCode ret=FPP_OK;

  if (global->infile != NULL && global->infile->fp != NULL)
    global->recursion = 0;
  while (type[*c] == LET && (dp = lookid(global, *c)) != NULL) {
    if((ret=expand(global, dp)))
      return(ret);
    *c = get(global);
  }
  return(FPP_OK);
}

int catenate(struct Global *global, int lhs_number, ReturnCode *ret)
{
  /*
   * A token was just read (via macroid).
   * If the next character is TOK_SEP, concatenate the next token
   * return TRUE -- which should recall macroid after refreshing
   * macroid's argument.  If it is not TOK_SEP, unget() the character
   * and return FALSE.
   */

#if OK_CONCAT
  int c;
  char *token1 = "";
#endif

#if OK_CONCAT
  if (get(global) != TOK_SEP) {                 /* Token concatenation  */
    unget(global);
    return (FALSE);
  }
  else {
    if (lhs_number == 0)                             /* The lhs number has already been emit */
      token1 = savestring(global, global->tokenbuf); /* Save first token     */
    c=get(global);
    if(global->rightconcat) {
      *ret=macroid(global, &c);           /* Scan next token      */
      if(*ret)
        return(FALSE);
    } else
      lookid(global, c);
    switch(type[c]) {                   /* What was it?         */
    case LET:                           /* An identifier, ...   */
      if ((int)strlen(token1) + (int)strlen(global->tokenbuf) >= NWORK) {
        cfatal(global, FATAL_WORK_AREA_OVERFLOW, token1);
        *ret=FPP_WORK_AREA_OVERFLOW;
        return(FALSE);
      }
      sprintf(global->work, "%s%s", token1, global->tokenbuf);
      break;
    case DIG:                           /* A number             */
    case DOT:                           /* Or maybe a float     */
      strcpy(global->work, token1);
      global->workp = global->work + strlen(global->work);
      *ret=scannumber(global, c, save);
      if(*ret)
        return(FALSE);
      *ret=save(global, EOS);
      if(*ret)
        return(FALSE);
      break;
    default:                            /* An error, ...        */
      if (isprint(c))
        cerror(global, ERROR_STRANG_CHARACTER, c);
      else
        cerror(global, ERROR_STRANG_CHARACTER2, c);
      strcpy(global->work, token1);
      unget(global);
      break;
    }
    /*
     * work has the concatenated token and token1 has
     * the first token (no longer needed).  Unget the
     * new (concatenated) token after freeing token1.
     * Finally, setup to read the new token.
     */
    if (lhs_number == 0)
      free(token1);                            /* Free up memory       */
    *ret=ungetstring(global, global->work);  /* Unget the new thing, */
    if(*ret)
      return(FALSE);
    return(TRUE);
  }
#else
  return(FALSE);                    /* Not supported        */
#endif
}

ReturnCode scanstring(struct Global *global,
                      int delim, /* ' or " */
                      /* Output function: */
                      ReturnCode (*outfun)(struct Global *, int))
{
  /*
   * Scan off a string.  Warning if terminated by newline or EOF.
   * outfun() outputs the character -- to a buffer if in a macro.
   * TRUE if ok, FALSE if error.
   */

  int c;
  ReturnCode ret;

  global->instring = TRUE;              /* Don't strip comments         */
  ret=(*outfun)(global, delim);
  if(ret)
    return(ret);
  while ((c = get(global)) != delim
         && c != '\n'
         && c != EOF_CHAR) {
    ret=(*outfun)(global, c);
    if(ret)
      return(ret);
    if (c == '\\') {
      ret=(*outfun)(global, get(global));
      if(ret)
        return(ret);
    }
  }
  global->instring = FALSE;
  if (c == delim) {
    ret=(*outfun)(global, c);
    return(ret);
  } else {
    cerror(global, ERROR_UNTERMINATED_STRING);
    unget(global);
    return(FPP_UNTERMINATED_STRING);
  }
}

ReturnCode scannumber(struct Global *global,
                      int c,            /* First char of number */
                      /* Output/store func: */
                      ReturnCode (*outfun)(struct Global *, int))
{
  /*
   * Process a number.  We know that c is from 0 to 9 or dot.
   * Algorithm from Dave Conroy's Decus C.
   */

  int radix;            /* 8, 10, or 16         */
  int expseen;          /* 'e' seen in floater  */
  int signseen;         /* '+' or '-' seen      */
  int octal89;          /* For bad octal test   */
  int dotflag;          /* TRUE if '.' was seen */
  ReturnCode ret;
  char done=FALSE;

  expseen = FALSE;                      /* No exponent seen yet */
  signseen = TRUE;                      /* No +/- allowed yet   */
  octal89 = FALSE;                      /* No bad octal yet     */
  radix = 10;                           /* Assume decimal       */
  if ((dotflag = (c == '.')) != FALSE) {/* . something?         */
    ret=(*outfun)(global, '.');         /* Always out the dot   */
    if(ret)
      return(ret);
    if (type[(c = get(global))] != DIG) { /* If not a float numb, */
      unget(global);                    /* Rescan strange char  */
      return(FPP_OK);                   /* All done for now     */
    }
  }                                     /* End of float test    */
  else if (c == '0') {                  /* Octal or hex?        */
    ret=(*outfun)(global, c);           /* Stuff initial zero   */
    if(ret)
      return(ret);
    radix = 8;                          /* Assume it's octal    */
    c = get(global);                    /* Look for an 'x'      */
    if (c == 'x' || c == 'X') {         /* Did we get one?      */
      radix = 16;                       /* Remember new radix   */
      ret=(*outfun)(global, c);         /* Stuff the 'x'        */
      if(ret)
        return(ret);
      c = get(global);                  /* Get next character   */
    }
  }
  while (!done) {                       /* Process curr. char.  */
    /*
     * Note that this algorithm accepts "012e4" and "03.4"
     * as legitimate floating-point numbers.
     */
    if (radix != 16 && (c == 'e' || c == 'E')) {
      if (expseen)                      /* Already saw 'E'?     */
        break;                          /* Exit loop, bad nbr.  */
      expseen = TRUE;                   /* Set exponent seen    */
      signseen = FALSE;                 /* We can read '+' now  */
      radix = 10;                       /* Decimal exponent     */
    }
    else if (radix != 16 && c == '.') {
      if (dotflag)                      /* Saw dot already?     */
        break;                          /* Exit loop, two dots  */
      dotflag = TRUE;                   /* Remember the dot     */
      radix = 10;                       /* Decimal fraction     */
    }
    else if (c == '+' || c == '-') {    /* 1.0e+10              */
      if (signseen)                     /* Sign in wrong place? */
        break;                          /* Exit loop, not nbr.  */
      /* signseen = TRUE; */            /* Remember we saw it   */
    } else {                            /* Check the digit      */
      switch (c) {
      case '8': case '9':               /* Sometimes wrong      */
        octal89 = TRUE;                 /* Do check later       */
      case '0': case '1': case '2': case '3':
      case '4': case '5': case '6': case '7':
        break;                          /* Always ok            */

      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        if (radix == 16)                /* Alpha's are ok only  */
          break;                        /* if reading hex.      */
      default:                          /* At number end        */
        done=TRUE;                      /* Break from for loop  */
        continue;
      }                                 /* End of switch        */
    }                                   /* End general case     */
    ret=(*outfun)(global, c);           /* Accept the character */
    if(ret)
      return(ret);
    signseen = TRUE;                    /* Don't read sign now  */
    c = get(global);                    /* Read another char    */
  }                                     /* End of scan loop     */
  /*
   * When we break out of the scan loop, c contains the first
   * character (maybe) not in the number.  If the number is an
   * integer, allow a trailing 'L' for long and/or a trailing 'U'
   * for unsigned.  If not those, push the trailing character back
   * on the input stream.  Floating point numbers accept a trailing
   * 'L' for "long double".
   */

  if (dotflag || expseen) {               /* Floating point?      */
    if (c == 'l' || c == 'L') {
      ret=(*outfun)(global, c);
      if(ret)
        return(ret);
      c = get(global);                   /* Ungotten later       */
    }
  } else {                                      /* Else it's an integer */
    /*
     * We know that dotflag and expseen are both zero, now:
     * dotflag signals "saw 'L'", and
     * expseen signals "saw 'U'".
     */
    char done=TRUE;
    while(done) {
      switch (c) {
      case 'l':
      case 'L':
        if (dotflag) {
          done=FALSE;
          continue;
        }
        dotflag = TRUE;
        break;
      case 'u':
      case 'U':
        if (expseen) {
          done=FALSE;
          continue;
        }
        expseen = TRUE;
        break;
      default:
        done=FALSE;
        continue;
      }
      ret=(*outfun)(global, c);       /* Got 'L' or 'U'.      */
      if(ret)
        return(ret);
      c = get(global);                /* Look at next, too.   */
    }
  }
  unget(global);                         /* Not part of a number */
  if(!(global->webmode)) {
    if (octal89 && radix == 8)
      cwarn(global, WARN_ILLEGAL_OCTAL);
  }
  return(FPP_OK);
}

ReturnCode save(struct Global *global, int c)
{
  if (global->workp >= &global->work[NWORK]) {
    cfatal(global, FATAL_WORK_BUFFER_OVERFLOW);
    return(FPP_WORK_AREA_OVERFLOW);
  } else
    *global->workp++ = c;
  return(FPP_OK);
}

char *savestring(struct Global *global, char *text)
{
  /*
   * Store a string into free memory.
   */

  char *result;
  result = malloc(strlen(text) + 1);
  strcpy(result, text);
  return (result);
}

ReturnCode getfile(struct Global *global,
                   int bufsize, /* Line or define buffer size   */
                   char *name,
                   FILEINFO **file) /* File or macro name string        */
{
  /*
   * Common FILEINFO buffer initialization for a new file or macro.
   */

  int size;

  size = strlen(name);                          /* File/macro name      */

  if(!size) {
      name = "[stdin]";
      size = strlen(name);
  }

  *file = (FILEINFO *) malloc((int)(sizeof (FILEINFO) + bufsize + size));
  if(!*file)
    return(FPP_OUT_OF_MEMORY);
  (*file)->parent = global->infile;             /* Chain files together */
  (*file)->fp = NULL;                           /* No file yet          */
  (*file)->filename = savestring(global, name); /* Save file/macro name */
  (*file)->progname = NULL;                     /* No #line seen yet    */
  (*file)->unrecur = 0;                         /* No macro fixup       */
  (*file)->bptr = (*file)->buffer;              /* Initialize line ptr  */
  (*file)->buffer[0] = EOS;                     /* Force first read     */
  (*file)->line = 0;                            /* (Not used just yet)  */
  if (global->infile != NULL)                   /* If #include file     */
    global->infile->line = global->line;        /* Save current line    */
  global->infile = (*file);                     /* New current file     */
  global->line = 1;                             /* Note first line      */
  return(FPP_OK);                               /* All done.            */
}

/*
 *                      C P P   S y m b o l   T a b l e s
 */

DEFBUF *lookid(struct Global *global,
               int c)           /* First character of token     */
{
  /*
   * Look for the next token in the symbol table. Returns token in tokenbuf.
   * If found, returns the table pointer;  Else returns NULL.
   */

  int nhash;
  DEFBUF *dp;
  int ct;
  int temp = 0;
  int isrecurse;        /* For #define foo foo  */

  nhash = 0;
  if ((isrecurse = (c == DEF_MAGIC)))   /* If recursive macro   */
    c = get(global);                    /* hack, skip DEF_MAGIC */
  ct = 0;
  do {
    if (ct == global->tokenbsize)
      global->tokenbuf = realloc(global->tokenbuf, 1 + (global->tokenbsize *= 2));
    global->tokenbuf[ct++] = c;         /* Store token byte     */
    nhash += c;                         /* Update hash value    */
    c = get(global);
  }  while (type[c] == LET || type[c] == DIG);
  unget(global);                        /* Rescan terminator    */
  global->tokenbuf[ct] = EOS;           /* Terminate token      */
  if (isrecurse)                        /* Recursive definition */
    return(NULL);                       /* undefined just now   */
  nhash += ct;                          /* Fix hash value       */
  dp = global->symtab[nhash % SBSIZE];  /* Starting bucket      */
  while (dp != (DEFBUF *) NULL) {       /* Search symbol table  */
    if (dp->hash == nhash               /* Fast precheck        */
        && (temp = strcmp(dp->name, global->tokenbuf)) >= 0)
      break;
    dp = dp->link;                      /* Nope, try next one   */
  }
  return((temp == 0) ? dp : NULL);
}

DEFBUF *defendel(struct Global *global,
                 char *name,
                 int delete)            /* TRUE to delete a symbol */
{
  /*
   * Enter this name in the lookup table (delete = FALSE)
   * or delete this name (delete = TRUE).
   * Returns a pointer to the define block (delete = FALSE)
   * Returns NULL if the symbol wasn't defined (delete = TRUE).
   */

  DEFBUF *dp;
  DEFBUF **prevp;
  char *np;
  int nhash;
  int temp;
  int size;

  for (nhash = 0, np = name; *np != EOS;)
    nhash += *np++;
  size = (np - name);
  nhash += size;
  prevp = &global->symtab[nhash % SBSIZE];
  while ((dp = *prevp) != (DEFBUF *) NULL) {
    if (dp->hash == nhash
        && (temp = strcmp(dp->name, name)) >= 0) {
      if (temp > 0)
        dp = NULL;                      /* Not found            */
      else {
        *prevp = dp->link;              /* Found, unlink and    */
        if (dp->repl != NULL)           /* Free the replacement */
          free(dp->repl);               /* if any, and then     */
        free((char *) dp);              /* Free the symbol      */
      }
      break;
    }
    prevp = &dp->link;
  }
  if (!delete) {
    dp = (DEFBUF *) malloc((int) (sizeof (DEFBUF) + size));
    dp->link = *prevp;
    *prevp = dp;
    dp->hash = nhash;
    dp->repl = NULL;
    dp->nargs = 0;
    strcpy(dp->name, name);
  }
  return(dp);
}


void outdefines(struct Global *global)
{
  DEFBUF *dp;
  DEFBUF **syp;

  deldefines(global);                   /* Delete built-in #defines     */
  for (syp = global->symtab; syp < &global->symtab[SBSIZE]; syp++) {
    if ((dp = *syp) != (DEFBUF *) NULL) {
      do {
        outadefine(global, dp);
      } while ((dp = dp->link) != (DEFBUF *) NULL);
    }
  }
}

INLINE FILE_LOCAL
void outadefine(struct Global *global, DEFBUF *dp)
{
  char *cp;
  int c;

  /* printf("#define %s", dp->name); */
  Putstring(global, "#define ");
  Putstring(global, dp->name);

  if (dp->nargs > 0) {
    int i;
    Putchar(global, '(');
    for (i = 1; i < dp->nargs; i++) {
      /* printf("__%d,", i); */
      Putstring(global, "__");
      Putint(global, i);
      Putchar(global, ',');
    }
    /* printf("__%d)", i); */
    Putstring(global, "__");
    Putint(global, i);
    Putchar(global, ')');

  } else if (dp->nargs == 0) {
    Putstring(global, "()");
  }
  if (dp->repl != NULL) {
    Putchar(global, '\t');
    for (cp = dp->repl; (c = *cp++ & 0xFF) != EOS;) {
      if (c >= MAC_PARM && c < (MAC_PARM + PAR_MAC)) {
        /* printf("__%d", c - MAC_PARM + 1); */
        Putstring(global, "__");
        Putint(global, c - MAC_PARM + 1);
      } else if (isprint(c) || c == '\t' || c == '\n')
        Putchar(global, c);
      else switch (c) {
      case QUOTE_PARM:
        Putchar(global, '#');
        break;
      case DEF_MAGIC:       /* Special anti-recursion */
      case MAC_PARM + PAR_MAC:    /* Special "arg" marker */
        break;
      case COM_SEP:
#if COMMENT_INVISIBLE
        Putstring(global, "/**/");
#else
        Putchar(global, ' ');
#endif
        break;
      case TOK_SEP:
        Putstring(global, "##");
        break;
      default:
        {
          /* Octal output! */
          char buffer[32];
          sprintf(buffer, "\\0%o", c);
          Putstring(global, buffer);
        }
      }
    }
  }
  Putchar(global, '\n');
}

/*
 *                      G E T
 */

int get(struct Global *global)
{
  /*
   * Return the next character from a macro or the current file.
   * Handle end of file from #include files.
   */

  int c;
  FILEINFO *file;
  int popped;   /* Recursion fixup      */
  long comments=0;

  popped = 0;
 get_from_file:
  if ((file = global->infile) == NULL)
    return (EOF_CHAR);
 newline:
  /*
   * Read a character from the current input line or macro.
   * At EOS, either finish the current macro (freeing temp.
   * storage) or read another line from the current input file.
   * At EOF, exit the current file (#include) or, at EOF from
   * the cpp input file, return EOF_CHAR to finish processing.
   */
  if ((c = *file->bptr++ & 0xFF) == EOS) {
    /*
     * Nothing in current line or macro.  Get next line (if
     * input from a file), or do end of file/macro processing.
     * In the latter case, jump back to restart from the top.
     */
    if (file->fp == NULL) {             /* NULL if macro        */
      popped++;
      global->recursion -= file->unrecur;
      if (global->recursion < 0)
        global->recursion = 0;
      global->infile = file->parent;            /* Unwind file chain    */
    } else {                            /* Else get from a file */
      /*
       * If a input routine has been specified in the initial taglist,
       * we should get the next line from that function IF we're reading
       * from that certain file!
       */

      if(global->input && global->first_file && !strcmp(global->first_file, file->filename))
        file->bptr = global->input(file->buffer, NBUFF, global->userdata);
      else
        file->bptr = fgets(file->buffer, NBUFF, file->fp);
      if(file->bptr != NULL) {
        goto newline;           /* process the line     */
      } else {
        if(!(global->input && global->first_file && !strcmp(global->first_file, file->filename)))
          /* If the input function isn't user supplied, close the file! */
          fclose(file->fp);           /* Close finished file  */
        if ((global->infile = file->parent) != NULL) {
          /*
           * There is an "ungotten" newline in the current
           * infile buffer (set there by doinclude() in
           * cpp1.c).  Thus, we know that the mainline code
           * is skipping over blank lines and will do a
           * #line at its convenience.
           */
          global->wrongline = TRUE;     /* Need a #line now     */
        }
      }
    }
    /*
     * Free up space used by the (finished) file or macro and
     * restart input from the parent file/macro, if any.
     */
    free(file->filename);               /* Free name and        */
    if (file->progname != NULL)         /* if a #line was seen, */
      free(file->progname);             /* free it, too.        */
    free(file);                         /* Free file space      */
    if (global->infile == NULL)         /* If at end of file    */
      return (EOF_CHAR);                /* Return end of file   */
    global->line = global->infile->line; /* Reset line number   */
    goto get_from_file;                 /* Get from the top.    */
  }
  /*
   * Common processing for the new character.
   */
  if (c == DEF_MAGIC && file->fp != NULL) /* Don't allow delete   */
    goto newline;                       /* from a file          */
  if (file->parent != NULL) {           /* Macro or #include    */
    if (popped != 0)
      file->parent->unrecur += popped;
    else {
      global->recursion -= file->parent->unrecur;
      if (global->recursion < 0)
        global->recursion = 0;
      file->parent->unrecur = 0;
    }
  }
  if (c == '\n')                        /* Maintain current     */
    ++global->line;                     /* line counter         */
  if (global->instring)                 /* Strings just return  */
    return (c);                         /* the character.       */
  else if (c == '/') {                  /* Comment?             */
    global->instring = TRUE;            /* So get() won't loop  */

    /* Check next byte for '*' and if(cplusplus) also '/' */
    if ( (c = get(global)) != '*' )
      if(!global->cplusplus || (global->cplusplus && c!='/')) {
        global->instring = FALSE;       /* Nope, no comment     */
        unget(global);                  /* Push the char. back  */
        return ('/');                   /* Return the slash     */
      }

    comments = 1;

    if (global->keepcomments) {         /* If writing comments   */

      global->comment = TRUE; /* information that a comment has been output */
      if(global->showspace) {
        /* Show all whitespaces! */
        global->spacebuf[global->chpos] = '\0';
        Putstring(global, global->spacebuf);
      }

      if(c=='*') {
        Putchar(global, '/');           /* Write out the         */
        Putchar(global, '*');           /*   initializer         */
      } else {
        /* C++ style comment */
        Putchar(global, '/');           /* Write out the         */
        Putchar(global, '/');           /*   initializer         */
      }
    }

    if(global->cplusplus && c=='/') {   /* Eat C++ comment!      */
      do {
        c=get(global);
        if(global->keepcomments)
          Putchar(global, c);
      } while(c!='\n' && c!=EOF_CHAR);  /* eat all to EOL or EOF */
      global->instring = FALSE;         /* End of comment        */
      return(c);                        /* Return the end char   */
    }

    for (;;) {                          /* Eat a comment         */
      c = get(global);
    test:
      if (global->keepcomments && c != EOF_CHAR)
        Putchar(global, c);
      switch (c) {
      case EOF_CHAR:
        cerror(global, ERROR_EOF_IN_COMMENT);
        return (EOF_CHAR);

      case '/':
        if(global->nestcomments || global->warnnestcomments) {
          if((c = get(global)) != '*')
            goto test;
          if(global->warnnestcomments) {
            cwarn(global, WARN_NESTED_COMMENT);
          }
          if(global->nestcomments)
            comments++;
        }
        break;

      case '*':
        if ((c = get(global)) != '/')           /* If comment doesn't   */
          goto test;                    /* end, look at next    */
        if (global->keepcomments) {     /* Put out the comment  */
          Putchar(global, c);           /* terminator, too      */
        }
        if(--comments)
          /* nested comment, continue! */
          break;

        global->instring = FALSE;       /* End of comment,      */
        /*
         * A comment is syntactically "whitespace" --
         * however, there are certain strange sequences
         * such as
         *              #define foo(x)  (something)
         *                      foo|* comment *|(123)
         *           these are '/' ^           ^
         * where just returning space (or COM_SEP) will cause
         * problems.  This can be "fixed" by overwriting the
         * '/' in the input line buffer with ' ' (or COM_SEP)
         * but that may mess up an error message.
         * So, we peek ahead -- if the next character is
         * "whitespace" we just get another character, if not,
         * we modify the buffer.  All in the name of purity.
         */
        if (*file->bptr == '\n'
            || type[*file->bptr & 0xFF] == SPA)
          goto newline;
#if COMMENT_INVISIBLE
        /*
         * Return magic (old-fashioned) syntactic space.
         */
        return ((file->bptr[-1] = COM_SEP));
#else
        return ((file->bptr[-1] = ' '));
#endif

      case '\n':                        /* we'll need a #line   */
        if (!global->keepcomments)
          global->wrongline = TRUE;     /* later...             */
      default:                          /* Anything else is     */
        break;                          /* Just a character     */
      }                                 /* End switch           */
    }                                   /* End comment loop     */
  }                                     /* End if in comment    */
  else if (!global->inmacro && c == '\\') { /* If backslash, peek   */
    if ((c = get(global)) == '\n') {    /* for a <nl>.  If so,  */
      global->wrongline = TRUE;
      goto newline;
    } else {                            /* Backslash anything   */
      unget(global);                    /* Get it later         */
      return ('\\');                    /* Return the backslash */
    }
  } else if (c == '\f' || c == VT)      /* Form Feed, Vertical  */
    c = ' ';                            /* Tab are whitespace   */
  return (c);                           /* Just return the char */
}

void unget(struct Global *global)
{
  /*
   * Backup the pointer to reread the last character.  Fatal error
   * (code bug) if we backup too far.  unget() may be called,
   * without problems, at end of file.  Only one character may
   * be ungotten.  If you need to unget more, call ungetstring().
   */

  FILEINFO *file;
  if ((file = global->infile) == NULL)
    return;                     /* Unget after EOF            */
  if (--file->bptr < file->buffer) {
    cfatal(global, FATAL_TOO_MUCH_PUSHBACK);
    /* This happens only if used the wrong way! */
    return;
  }
  if (*file->bptr == '\n')              /* Ungetting a newline?       */
    --global->line;                     /* Unget the line number, too */
}

ReturnCode ungetstring(struct Global *global, char *text)
{
  /*
   * Push a string back on the input stream.  This is done by treating
   * the text as if it were a macro.
   */

  FILEINFO *file;
  ReturnCode ret;

  ret = getfile(global, strlen(text) + 1, "", &file);
  if(!ret)
    strcpy(file->buffer, text);
  return(ret);
}

int cget(struct Global *global)
{
  /*
   * Get one character, absorb "funny space" after comments or
   * token concatenation
   */

  int c;
  do {
    c = get(global);
#if COMMENT_INVISIBLE
  } while (c == TOK_SEP || c == COM_SEP);
#else
  } while (c == TOK_SEP);
#endif
  return (c);
}

/*
 * Error messages and other hacks.
 */

INLINE FILE_LOCAL
void domsg(struct Global *global,
  ErrorCode error,  /* error message number */
  va_list arg)      /* Something for the message    */
{
  /*
   * Print filenames, macro names, and line numbers for error messages.
   */

  static char *ErrorMessage[]={
    /*
     * ERRORS:
     */
    "#%s must be in an #if",
    "#%s may not follow #else",
    "#error directive encountered",
    "Preprocessor assertion failure",
    "#if, #ifdef, or #ifndef without an argument",
    "#include syntax error",
    "#define syntax error",
    "Redefining defined variable \"%s\"",
    "Illegal #undef argument",
    "Recursive macro definition of \"%s\"(Defined by \"%s\")",
    "end of file within macro argument",
    "misplaced constant in #if",
    "#if value stack overflow",
    "Illegal #if line",
    "Operator %s in incorrect context",
    "expression stack overflow at op \"%s\"",
    "unbalanced paren's, op is \"%s\"",
    "Misplaced '?' or ':', previous operator is %s",
    "Can't use a string in an #if",
    "Bad #if ... defined() syntax",
    "= not allowed in #if",
    "Unexpected \\ in #if",
    "#if ... sizeof() syntax error",
    "#if sizeof, unknown type \"%s\"",
    "#if ... sizeof: illegal type combination",
    "#if sizeof() error, no type specified",
    "Unterminated string",
    "EOF in comment",
    "Inside #ifdef block at end of input, depth = %d",
    "illegal character '%c' in #if",
    "illegal character (%d decimal) in #if",
    "#if ... sizeof: bug, unknown type code 0x%x",
    "#if bug, operand = %d.",
    "Strange character '%c' after ##",
    "Strange character (%d.) after ##",

    "", /* Dummy, to visualize the border between errors and warnings */
    /*
     * WARNINGS:
     */
    "Control line \"%s\" within macro expansion",
    "Illegal # command \"%s\"",
    "Unexpected text in #control line ignored",
    "too few values specified to sizeof",
    "too many values specified to sizeof! Not used.",
    "\"%s\" wasn't defined",
    "Internal error!",
    "Macro \"%s\" needs arguments",
    "Wrong number of macro arguments for \"%s\"",
    "%s by zero in #if, zero result assumed",
    "Illegal digit in octal number",
    "multi-byte constant '%c' isn't portable",
    "Cannot open include file \"%s\"",
    "Illegal bracket '[]' balance, depth = %d",
    "Illegal parentheses '()' balance, depth = %d",
    "Illegal brace '{}' balance, depth = %d",
    "Nested comment",

    "", /* Dummy, to visualize the border between warnings and fatals */

    /*
     * FATALS:
     */
    "Too many nested #%s statements",
    "Filename work buffer overflow",
    "Too many include directories",
    "Too many include files",
    "Too many arguments for macro",
    "Macro work area overflow",
    "Bug: Illegal __ macro \"%s\"",
    "Too many arguments in macro expansion",
    "Out of space in macro \"%s\" arg expansion",
    "work buffer overflow doing %s ##",
    "Work buffer overflow",
    "Out of memory",
    "Too much pushback", /* internal */
    };

  char *tp;
  FILEINFO *file;
  char *severity=error<BORDER_ERROR_WARN?"Error":
    error<BORDER_WARN_FATAL?"Warning":
      "Fatal";

  for (file = global->infile; file && !file->fp; file = file->parent)
    ;
  tp = file ? file->filename : 0;
  Error(global, "%s\"%s\", line %d: %s: ",
        MSG_PREFIX, tp, global->infile->fp?global->line:file->line, severity);
  if(global->error)
    global->error(global->userdata, ErrorMessage[error], arg);
#if defined(UNIX)
  else
    vfprintf(stderr, ErrorMessage[error], arg);
#elif defined(AMIGA)
  else
    return;
#endif
  Error(global, "\n");

  if (file)   /*OIS*0.92*/
    while ((file = file->parent) != NULL) { /* Print #includes, too */
      tp = file->parent ? "," : ".";
      if (file->fp == NULL)
        Error(global, " from macro %s%s\n", file->filename, tp);
      else
        Error(global, " from file %s, line %d%s\n",
              (file->progname != NULL) ? file->progname : file->filename,
              file->line, tp);
    }

  if(error<BORDER_ERROR_WARN)
    /* Error! Increase error counter! */
    global->errors++;
}

void cerror(struct Global *global,
            ErrorCode message,
            ...)        /* arguments    */
{
  /*
   * Print a normal error message, string argument.
   */
  va_list arg;
  va_start(arg, message);
  domsg(global, message, arg);
}

void Error(struct Global *global, char *format, ...)
{
  /*
   * Just get the arguments and send a decent string to the user error
   * string handler or to stderr.
   */

  va_list arg;
  va_start(arg, format);
  if(global->error)
    global->error(global->userdata, format, arg);
#if defined(UNIX)
  else
    vfprintf(stderr, format, arg);
#endif
}
