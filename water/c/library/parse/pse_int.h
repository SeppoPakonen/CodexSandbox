
#ifndef THEYER_PARSE_INT_H
#define THEYER_PARSE_INT_H

/*
 * Description:
 * 
 */

/*
 * includes
 */

#include <file/file.h>
#include <text/text.h>

/*
 * macros
 */

#define MAXARGS	   			200
#define PSE_ARG_MAXLEN		256

/*
 * typedefs
 */

typedef enum {
	PSE_DATATYPE_INT,
	PSE_DATATYPE_FLOAT,
	PSE_DATATYPE_STRING
} PseDataType;

/* parse info */
typedef struct {
    File   		 file;
	Text		 filename;
    char   		 line[PSE_MAXLINE];
	int			 lineno;
    int    		 max;
	int			 nargs;
	char		 args[PSE_ARG_MAXLEN][MAXARGS];
	int			 offsets[MAXARGS];
	int			 argtype[MAXARGS];
	int			 intval[MAXARGS]; 
	float		 floatval[MAXARGS]; 
	Bool		 abort; 
	Text		 ichars;
	int			 nchars;
	Byte		*data;
	int			 dcnt;
} PseInfo;

/*
 * prototypes
 */

/*
 * variables
 */

#endif
