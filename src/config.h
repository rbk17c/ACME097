// ACME - a crossassembler for producing 6502/65c02/65816/65ce02 code.
// Copyright (C) 1998-2024 Marco Baye
// Have a look at "acme.c" for further info
//
// Configuration
#ifndef config_H
#define config_H

#include <stdint.h>


// make sure the enum below works with strange compilers, too:
#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif
// types
typedef enum { FALSE = 0, TRUE }	boolean;	// yes, I could include <stdbool.h>, but this source should work with ancient compilers as well...
typedef unsigned int	bits;
typedef unsigned int	scope_t;
typedef intptr_t 	intval_t;	// at least 32 bits
typedef unsigned int 	uintval_t;	// at least 32 bits (only used for logical shift right)
#define OUTBUF_MAXSIZE	0x1000000	// 16 MiB ought to be enough for anybody

// struct to remember where macros/symbols were defined
struct location {
	const char	*plat_filename;	// filename in platform style
	int		line_number;
};
// struct for code blocks (loop conditions, loop bodies and macro bodies)
struct block {
	int	line_number;	// start of block
	char	*body;
};


// stuff for results from expression parser:

enum numtype {
	NUMTYPE_UNDEFINED,
	NUMTYPE_INT,
	NUMTYPE_FLOAT,
};

// structure for ints/floats
struct number {
	enum numtype	ntype;
	bits		flags;	// FITS_IN_BYTE etc. (see alu.h)
	union {
		intval_t	intval;	// integer value
		double		fpval;	// floating point value
	} val;
	int	addr_refs;	// address reference count (only look at this if value is DEFINED)
};

struct type;
struct string;
struct listitem;
// structure for ints/floats/lists/strings (anything that can be assigned to symbol)
struct object {
	struct type	*type;
	union {
		struct number	number;
		struct string	*string;
		struct listitem	*listhead;
	} u;
};
struct string {
	int	length;
	int	refs;	// FIXME - either use correctly or remove altogether!
	char	payload[1];	// real structs are malloc'd to correct size
};
struct listitem {
	struct listitem	*next;
	struct listitem	*prev;
	union {
		struct {
			int	length;	// this does not include the head element
			int	refs;	// FIXME - either use correctly or remove altogether!
		} listinfo;			// if item is list head
		struct object	payload;	// if item is not list head
	} u;
};

// debugging flag, should be undefined in release version
// #define FDEBUG

// maximum nesting depth of "!src" and macro calls, also max number of passes
// is not actually a limitation, but a means of finding recursions
#define SANITY_LIMIT	64
// default value for "!fill"
#define FILLVALUE_FILL		0

// Nullpointer definition
#ifndef NULL
#define NULL	((void *) 0)
#endif


#endif
