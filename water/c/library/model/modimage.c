/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modimage.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 9 May 1998
 * Last Update	: 17 May 1998
 *****************************************************************
 * Description	: Model image bank functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <text/text.h>
#include <parse/parse.h>
#include <tga/tga.h>
#include <linked/linked.h>
#include <memory/memory.h>

#include "model.h"
#include "model_int.h"

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

/*
 * variables
 */

/* 
 * globals variables...
 */

private LnkLinkedList	*ibank;
private Bool			 bank_initialised = FALSE;

/*
 * functions
 */


/******************************************************************************
 * Function:
 * modImageLoad -- create and initialise a model structure
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	*modImageLoad ( 
	Text		 name,
	int			*width,
	int			*height
	)
{
	TgaFile			*pic;
	LnkLinkedList	*list;
	ModImage		*image;
	Text			 errstr;

	/* validate */
	if ( name == NULL || width == NULL || height == NULL ) 
		return( NULL );

	/* init */
	*width  = 0;
	*height = 0;

	/* create image bank if necessary */
	if ( ! bank_initialised ) {
		ibank = lnkCreate();
		bank_initialised = TRUE;
	}

	/* find image */
	list = lnkNext(ibank);
	while( list != NULL ) {
		image = (ModImage *)list->data;
		if ( txtMatch( image->name, name ) ) {
			image->refcnt++;
			*width  = image->width;
			*height = image->height;
			return( image->idata );
		}
		list = lnkNext(list);
	}

	/* assume tga file */
	pic = tgaOpen( name, &errstr );
	if ( pic == NULL ) {
		modSetErrorString( errstr );
		return( NULL );
	}

	/* create new image */
	image = memNew(ModImage);
	if ( image == NULL ) {
		memFree(pic->data);
		memFree(pic);
		return( NULL );
	}

	/* save */
	image->idata  = pic->data;
	image->refcnt = 1;
	image->name   = txtDupl( name );
	image->width  = pic->width;
	image->height = pic->height;

	/* add to linked list */
	lnkAdd(ibank,image);

	/* done with pic data */
	memFree(pic);

	/* return image data */
	*width  = image->width;
	*height = image->height;
	return( image->idata );
}


/******************************************************************************
 * Function:
 * modImageLoadFromPak -- load an image from a PAK file (internet capable)
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	*modImageLoadFromPak ( 
	Byte		*data,
	Text		 name,
	int			*width,
	int			*height
	)
{
	MibFile			*pic;
	LnkLinkedList	*list;
	ModImage		*image;
	Text			 errstr;

	/* validate */
	if ( name == NULL || width == NULL || height == NULL ) 
		return( NULL );

	/* init */
	*width  = 0;
	*height = 0;

	/* create image bank if necessary */
	if ( ! bank_initialised ) {
		ibank = lnkCreate();
		bank_initialised = TRUE;
	}

	/* find image */
	list = lnkNext(ibank);
	while( list != NULL ) {
		image = (ModImage *)list->data;
		if ( txtMatch( image->name, name ) ) {
			image->refcnt++;
			*width  = image->width;
			*height = image->height;
			return( image->idata );
		}
		list = lnkNext(list);
	}

	/* assume mib file */
	pic = modLoadMibFromPak( data, name, &errstr );
	if ( pic == NULL ) {
		//modSetErrorString( errstr );
		return( NULL );
	}

	/* create new image */
	image = memNew(ModImage);
	if ( image == NULL ) {
		memFree(pic->data);
		memFree(pic);
		return( NULL );
	}

	/* save */
	image->idata  = pic->data;
	image->refcnt = 1;
	image->name   = txtDupl( name );
	image->width  = pic->width;
	image->height = pic->height;

	/* done with pic data */
	memFree(pic);

	/* add to linked list */
	lnkAdd(ibank,image);

	/* return image data */
	*width  = image->width;
	*height = image->height;
	return( image->idata );
}


/******************************************************************************
 * Function:
 * modImageDestroy -- clean image memory for a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modImageDestroy ( 
	Text		 name
	)
{
	LnkLinkedList	*list;
	ModImage		*image;

	/* validate */
	if ( name == NULL || (! bank_initialised) )
		return( FALSE );

	/* find image */
	list = lnkNext(ibank);
	while( list != NULL ) {
		image = (ModImage *)list->data;
		if ( txtMatch( image->name, name ) ) {
			image->refcnt--;
			if ( image->refcnt == 0 ) {
				/* ok kill it */
				memFree(image->name);
				memFree(image->idata);
				lnkDelete(list);
			}
			return( TRUE );
		}
		list = lnkNext(list);
	}

	/* could not find image */
	return( FALSE );
}

