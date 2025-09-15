/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	gfxtmd.c
 * Author:	Mark Theyer
 * Created:	07 Dec 1999
 **********************************************************
 * Description:	PSX Graphics Interface
 **********************************************************
 * Notes:		You cannot use 'psx' as a variable name.
 **********************************************************
 * Functions:
 *	gfxSetViewPoint()
 *				Set the eye and view points for the display.
 *	gfxBufSwap()
 *				Render graphics and swap display buffers.
 *	gfxBeginObject()
 *				Create a new graphics model object for 
 *				subsequent update.
 *	gfxEndObject()
 *				Indicate completion of object creation.
 *	gfxObjectSubDivide()
 *				Set subdivision flag for an object.
 *	gfxObjectParent()
 *				Set or remove a parent for an object.
 *	gfxAddTri()
 *				Add a triangle surface to the current 
 *				graphics object.
 *	gfxAddQuad()
 *				Add a quad (triangle pair) surface to the 
 *				current graphics object.
 *	gfxDrawObject()
 *				Draw a graphics object.
 *	gfxRotateObject()
 *				Rotate an object on an axis.
 *	gfxTranslateObject()
 *				Translate an object.
 *	gfxScaleObject()
 *				Scale an object.
 *	gfxGetObjectMatrix()
 *				Get the transformation matrix from an object.
 *	gfxSetObjectMatrix()
 *				Set the transformation matrix for an object.
 *	gfxGetObjectPoint()
 *				Get a point value from an object.
 *	gfxSetObjectPoint()
 *				Set a point value for an object.
 *	gfxSetObjectNormal()
 *				Set a point normal value for an object.
 *
 *  Private PSX Functions:
 *  ----------------------
 *
 *	psxClear()
 *				Clear the graphics ordering tables.
 *	psxClearModels()
 *				Clear all models from memory.
 *	psxGetObjectPosition()
 *				Get the position pointer from an object.
 *	psxAllocateExtraHandler()
 *				Allocate an extra handler for an object.
 *	psxScale()
 *				Perform PSX scaling.
 *	psxTranslate()
 *				Perform direct translation (non PSX).
 *	psxRotate()
 *				Perform PSX rotation.
 *	psxApplyMatrix()
 *				Multiply object matrix by matrix.
 **********************************************************
 * Revision History:
 * 07-Dec-99	Theyer	Initial creation from gfxpsx.c
 **********************************************************/

#ifndef PSX_USE_HMD_FORMAT

/*
 * includes
 */

#include <type/datatype.h>
#include <gfx/gfx.h>
#include <gfx/gfxpsx.h>
#include <triangle/triangle.h>
#include <text/text.h>
#include <parse/parse.h>
#include <memory/memory.h>

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

private void         psxClear( void );
private void        *psxAddPrimitive( GfxObject *gfxobj,  PsxObjType type );
private void         psxSetPrimitive( PsxPrimitive *prim, PsxObjType type );
private GfxObject   *psxNewObject( Text name, Bool dynamic );
private PsxObjPosition *psxGetObjectPosition( PsxObject *psxobj );
private PsxObjHandleList * psxAllocateExtraHandler( PsxObjHandleList *current );
private void         psxScale( PsxObjPosition *position, fixed x, fixed y, fixed z );
private void         psxTranslate( PsxObjPosition *position, int x, int y, int z );
private void  	     psxRotate( PsxObjPosition *position, char axis, int angle );

/*
 * variables
 */


extern PsxData ps;


/**********************************************************
 * Function:	gfxSetViewPoint
 **********************************************************
 * Description: Set the view point
 * Inputs:		eye - new eye point
 *				ref - new reference (look at) point
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxSetViewPoint (
    Point_i3d	*eye,
    Point_i3d	*ref,
	fixed		 proj
    )
{
	int			 x, y, z;
	int			 dist;

	/* set eye */
    ps.tmd.view.vpx = (long)eye->x;
    ps.tmd.view.vpy = (long)-(eye->z);
    ps.tmd.view.vpz = (long)eye->y;

    /* this stupid system needs to look past the view point */
    x = ((int)ref->x - (int)eye->x);
    y = ((int)ref->y - (int)eye->y);
    z = ((int)ref->z - (int)eye->z);
    ps.tmd.view.vrx = (long)(int)ref->x;
    ps.tmd.view.vry = (long)-((int)ref->z);
    ps.tmd.view.vrz = (long)(int)ref->y;

	/* set projection distance */
	x /= 100;
	y /= 100;
	z /= 100;
    dist = mthFastSqrt( gfxAbs(((x * x) + (y * y) + (z * z))) ) * 100;
    GsSetProjection( FIXED_TO_INT((dist * proj))  );

	/* other stuff */
    ps.tmd.view.rz    = 0;
    //ps.tmd.view.super = WORLD;	
    GsSetRefView2( (GsRVIEW2 *)&ps.tmd.view );
}
	

/**********************************************************
 * Function:	gfxSetViewParent
 **********************************************************
 * Description: Set the view point
 * Inputs:		eye - new eye point
 *				ref - new reference (look at) point
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxSetViewParent (
    GfxObject	*parent
    )
{
    PsxObject		*psxobj;
    PsxObject		*parentobj;

    /* link to parent object or divorce */
    if ( parent != NULL ) {
		parentobj = (PsxObject*)parent->data;
	    ps.tmd.view.super = (GsCOORDINATE2 *)&parentobj->current->position;
    } else {
	    ps.tmd.view.super = WORLD;
    }
}
	

/**********************************************************
 * Function:	gfxBufSwap
 **********************************************************
 * Description: Swap frame buffers
 * Inputs:		void
 * Notes:		Performs PSX drawing and frame buffer swap
 * Returns:		void
 **********************************************************/

public void gfxBufSwap (
    void
    )
{
	/* increment draw rate counter */
	ps.ndraw++;
	
    /* wait for end of drawing */
    DrawSync(0);

	/* update ticker */
	ps.tick = VSync(-1) - ps.time;
	ps.time += ps.tick;

    /* wait for V_BLANK interrupt */
#if 0
	VSync(0);
#else
	ps.vsync = 0;
	while( ! ps.vsync );
#endif

    /* swap double buffers */
    GsSwapDispBuff();

	if ( ps.split_screen.on ) {

		/* register clear-command: clear to background color */
		GsSortClear( ps.bkcolor.red, ps.bkcolor.green, ps.bkcolor.blue, &ps.screen.split.worldorder[ps.drawpage] );

		/* set clip region */
		GsSetClip( &ps.split_screen.clip[0] );
		GsSetOffset( ps.split_screen.offsetx[0], ps.split_screen.offsety[0] );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.split.worldorder[ps.drawpage] );

		/* wait for end of drawing */
		DrawSync(0);

		/* set clip region */
		GsSetClip( &ps.split_screen.clip[1] );
		GsSetOffset( ps.split_screen.offsetx[1], ps.split_screen.offsety[1] );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.split.worldorder[(ps.drawpage+2)] );

		/* wait for end of drawing */
		DrawSync(0);

		/* set clip region */
		GsSetClip( &ps.split_screen.clip[2] );
		GsSetOffset( (ps.xmax>>1), (ps.ymax>>1) );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.split.worldorder[(ps.drawpage+4)] );
	} else {
		/* register clear-command: clear to background color */
		GsSortClear( ps.bkcolor.red, ps.bkcolor.green, ps.bkcolor.blue, &ps.screen.full.worldorder[ps.drawpage] );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.full.worldorder[ps.drawpage] );
	}

    /* update drawpage and displaypage */
    if ( ps.displaypage == 0 ) {
		ps.drawpage    = 0;
		ps.displaypage = 1;
    } else {
		ps.drawpage    = 1;
		ps.displaypage = 0;
    }
     
    /* clear the new frame */
    psxClear();

#ifdef GFX_USE_PSX_ANALYSER
	pollhost();
#endif
}


/**********************************************************
 * Function:	gfxBufSwapNoClear
 **********************************************************
 * Description: Swap frame buffers (without clearing to background color)
 * Inputs:		void
 * Notes:		Performs PSX drawing and frame buffer swap
 * Returns:		void
 **********************************************************/

public void gfxBufSwapNoClear (
    void
    )
{
	/* increment draw rate counter */
	ps.ndraw++;
	
    /* wait for end of drawing */
    DrawSync(0);

	/* update ticker */
	ps.tick = VSync(-1) - ps.time;
	ps.time += ps.tick;

    /* wait for V_BLANK interrupt */
#if 0
	VSync(0);
#else
	ps.vsync = 0;
	while( ! ps.vsync );
#endif

    /* swap double buffers */
    GsSwapDispBuff();

	if ( ps.split_screen.on ) {

		/* register clear-command: clear to background color */
		//GsSortClear( ps.bkcolor.red, ps.bkcolor.green, ps.bkcolor.blue, &ps.screen.split.worldorder[ps.drawpage] );

		/* set clip region */
		GsSetClip( &ps.split_screen.clip[0] );
		GsSetOffset( ps.split_screen.offsetx[0], ps.split_screen.offsety[0] );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.split.worldorder[ps.drawpage] );

		/* wait for end of drawing */
		DrawSync(0);

		/* set clip region */
		GsSetClip( &ps.split_screen.clip[1] );
		GsSetOffset( ps.split_screen.offsetx[1], ps.split_screen.offsety[1] );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.split.worldorder[(ps.drawpage+2)] );

		/* wait for end of drawing */
		DrawSync(0);

		/* set clip region */
		GsSetClip( &ps.split_screen.clip[2] );
		GsSetOffset( (ps.xmax>>1), (ps.ymax>>1) );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.split.worldorder[(ps.drawpage+4)] );
	} else {
		/* register clear-command: clear to background color */
		//GsSortClear( ps.bkcolor.red, ps.bkcolor.green, ps.bkcolor.blue, &ps.screen.full.worldorder[ps.drawpage] );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.full.worldorder[ps.drawpage] );
	}

    /* update drawpage and displaypage */
    if ( ps.displaypage == 0 ) {
		ps.drawpage    = 0;
		ps.displaypage = 1;
    } else {
		ps.drawpage    = 1;
		ps.displaypage = 0;
    }
     
    /* clear the new frame */
    psxClear();

#ifdef GFX_USE_PSX_ANALYSER
	pollhost();
#endif
}


/**********************************************************
 * Function:	gfxBeginObject
 **********************************************************
 * Description: Begin creation of a graphics object
 * Inputs:		name - name of object
 *				parent - parent object
 *				dynamic - single instance object
 * Notes:		New object is empty, add data then call
 *				gfxEndObject to close...
 * Returns:		pointer to new object
 **********************************************************/

public GfxObject *gfxBeginObject (
	Text		 name,
    GfxObject	*parent,
    Bool		 skinned
    )
{
    GfxObject		*gfxobj;
    PsxObject		*psxobj;
    PsxObject		*parentobj;
    PsxTmdHeader	*header;
    PsxTmdObject	*object;
    PsxObjHandler	*handler;
    u_long          *l;
    int				 i;
    int				 n_vert;
    int				 n_norm;
    int				 p_size;

#if GFX_DEBUG
    printf("gfxBeginObject...");
#endif

#if GFX_VALIDATE
	/* validate */
	if ( skinned ) {
		gfxError( GFX_ERROR_SKINNED_TMD_NOT_SUPPORTED );
		gfxAbort();
	}
#endif

    /* allocate memory and init */
    gfxobj  = psxNewObject( name, 0 );
    psxobj  = (PsxObject *)gfxobj->data;
    header  = &psxobj->header;
    object  = &psxobj->object;
    handler = &psxobj->handle_list.handler;

    /* init TMD header */
    header->version  = PSX_TMD_VERSION;
    header->flags    = PSX_TMD_HAS_REAL_ADDRESSES;
    header->nobjects = 1;

	/* re-init transparency */
	ps.transparency = FALSE;

    /* init TMD object */
    object->n_vert         = 0;
    object->n_normal       = 0;
    object->n_primitive    = 0;

	n_vert = ps.tmd.n_vert;
    n_norm = ps.tmd.n_normal;
    p_size = ps.tmd.primitive_size;
#if GFX_VERTEX_LOCAL
    object->vert_top       = (u_long *)&(ps.tmd.vertex[n_vert]);
    object->normal_top     = (u_long *)&(ps.tmd.normal[n_norm]);
#else
    object->vert_top       = (u_long *)&(ps.tmd.vertex);
    object->normal_top     = (u_long *)&(ps.tmd.normal);
#endif
    object->primitive_top  = &(ps.tmd.primitive[p_size]);

    object->scale          = 0;
    psxobj->primitive_size = 0;

    /* init handler position */
    GsInitCoordinate2( WORLD, (GsCOORDINATE2 *)&psxobj->handle_list.position );
    handler->position = &psxobj->handle_list.position;
    psxobj->current = &psxobj->handle_list;
	psxobj->handle_list.linked = FALSE;
	psxobj->handle_list.next = NULL;
	psxobj->state = PSX_OBJ_NOTDRAWN;

    /* link to parent object if necessary */
    if ( parent != NULL ) {
		parentobj = (PsxObject*)parent->data;
        psxobj->current->position.parent = &parentobj->current->position;
    }
 
    /* init object handler */
    handler->attribute.unused       = 0;
    handler->attribute.lightingoff  = ps.lightingoff;
    handler->attribute.unused       = 0;
    handler->attribute.subdivision  = PSX_SUBDIV_NONE;
    handler->attribute.unused2      = 0;
    handler->attribute.transparency = 0;
    handler->attribute.displayoff   = 0;

    /* save the gfx object */
    //gfxobj->dynamic = 0;
    gfxobj->id      = 1;
    gfxobj->data    = (void *)psxobj;

#if GFX_DEBUG
    printf("done\n");
#endif

    /* OK */
    return( gfxobj );
}


/**********************************************************
 * Function:	gfxEndObject
 **********************************************************
 * Description: Indicate end of graphics object creation
 * Inputs:		nil
 * Notes:		Used by GL port, does nothing here...
 * Returns:		void
 **********************************************************/

public void gfxEndObject (	
	GfxObject	*object
	)
{
	;
}


/**********************************************************
 * Function:	gfxObjectSubDivide
 **********************************************************
 * Description: Subdivide a graphics object's polygons
 * Inputs:		object - object to set subdivision flag
 *				subdiv - subdivision flag
 * Notes:		PlayStation 1 for Z sort functionality
 * Returns:		void
 **********************************************************/

public void gfxObjectSubDivide (
    GfxObject	*object,
    int			 subdiv
    )
{
#if PSX_USE_ACTIVE_SUBDIV
	;
#else
    PsxObject		*psxobj;
    PsxObjHandler	*handler;
  
    psxobj  = (PsxObject *)object->data;
    handler = &psxobj->current->handler;

    /* set subdivision flag */
    handler->attribute.subdivision = subdiv;
#endif
}


/**********************************************************
 * Function:	gfxObjectParent
 **********************************************************
 * Description: Set the parent for an object
 * Inputs:		object - object to set parent for
 *				parent - parent object (or NULL for no parent)
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxObjectParent (
    GfxObject 	*object,
    GfxObject	*parent
    )
{
    PsxObject		*psxobj;
    PsxObject		*parentobj;

    /* init */ 
    psxobj = (PsxObject *)object->data;

    /* link to parent object or divorce */
    if ( parent != NULL ) {
		parentobj = (PsxObject*)parent->data;
        psxobj->current->position.parent = &parentobj->current->position;
    } else {
        psxobj->current->position.parent = NULL;
    }
}


/**********************************************************
 * Function:	gfxAddPivot
 **********************************************************
 * Description: Add a pivot point to an skinned object
 * Inputs:		object - object to add pivot to
 *				pt     - pivot point
 *				parent - pivot parent (GFX_PIVOT_WORLD or <n>)
 *				npts   - number of points attached to pivot pt
 * Notes:		No skinned support in TMD models
 * Returns:		void
 **********************************************************/

public int gfxAddPivot ( 
	GfxObject	*object, 
	Point_i3d	 pt, 
	int			 parent, 
	int			 npts 
	)
{
	/* not supported */
	gfxError( GFX_ERROR_SKINNED_TMD_NOT_SUPPORTED );
	//gfxAbort();
	return(0);
}


/**********************************************************
 * Function:	gfxAddTri
 **********************************************************
 * Description: Add a single triangle to an object
 * Inputs:		object - object to add triangle to
 *				pts - triangle points
 *				txy - optional texture points
 *				normals - optional normals per point
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxAddTri (
    GfxObject	*object,
    Point_i3d	 pts[],
    Point_fx2d   txy[],
    int			 normals[]
    )
{
    PsxObject	  	 	*psxobj;
    PsxTmdObject  	 	*obj;
    size_t	   	   	 	 newsize;
    PsxTriFlat  	 	*triflat;
    PsxTriFlatTex 		*triflattex;
	PsxTriGourLit		*trigourlit;
	PsxTriGourTexLit	*trigourtexlit;
    PsxTriFlatLit  	 	*triflatlit;
    PsxTriFlatTexLit 	*triflattexlit;
    PsxTexPoint  		*atxy;
    u_char	  			*c;
    short	  			*s;
    int		   			 i;
    u_long	  			*l;
    Point_fx3d	   		 normal;
	u_short				 nindex[2];
	u_short				 vindex[4];

    /* init */
    psxobj = (PsxObject *)object->data;
    obj    = &(psxobj->object);

    /* create primitive */
	if ( ps.lightingoff ) {
		/* no lighting */
    	if ( ps.textureon ) {
    		/* flat shaded, textured, no lighting */
			atxy = psxAdjustTexturePoints( txy, 3 );
    		triflattex = (PsxTriFlatTex *)psxAddPrimitive( object, PSX_TRI_FLAT_TEX );
			/* add triangle data */
    		psxSetPrimitive( &triflattex->prim, PSX_TRI_FLAT_TEX );
        	triflattex->tx0     = atxy[0].x;
        	triflattex->ty0     = atxy[0].y;
        	triflattex->tx1     = atxy[1].x;
        	triflattex->ty1     = atxy[1].y;
			triflattex->tx2     = atxy[2].x;
			triflattex->ty2     = atxy[2].y;
			triflattex->unused0 = 0;
			triflattex->red     = ps.color.red;
    		triflattex->green   = ps.color.green;
    		triflattex->blue    = ps.color.blue;
			triflattex->unused1 = 0;
        	/* add texture info */
        	psxSetTextureInfo( &triflattex->texture, &triflattex->clut );
    		/* add vertex data */
    		psxAddVertex( object, pts, 3, vindex );
    		triflattex->v0 = vindex[0];
    		triflattex->v1 = vindex[1];
    		triflattex->v2 = vindex[2];
#if GFX_DEBUG
    		l = (u_long *)triflattex;
    		printf("tri (flat tex not lit) 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3), *(l+4), *(l+5) );
#endif
			/* add double sided triangle if necessary */
        	if ( ps.doublesided ) {    	
    		    triflattex = (PsxTriFlatTex *)psxAddPrimitive( object, PSX_TRI_FLAT_TEX );
    		    psxSetPrimitive( &triflattex->prim, PSX_TRI_FLAT_TEX );
        	    triflattex->tx0    = atxy[1].x;
        	    triflattex->ty0    = atxy[1].y;
        	    triflattex->tx1    = atxy[0].x;
        	    triflattex->ty1    = atxy[0].y;
	    		triflattex->tx2    = atxy[2].x;
	    		triflattex->ty2    = atxy[2].y;
				triflattex->unused0 = 0;
				triflattex->red     = ps.color.red;
    			triflattex->green   = ps.color.green;
    			triflattex->blue    = ps.color.blue;
				triflattex->unused1 = 0;
           		/* add texture info */
  	    		psxSetTextureInfo( &triflattex->texture, &triflattex->clut );
            	/* add reversed points */
    			triflattex->v0 = vindex[1];
    			triflattex->v1 = vindex[0];
    			triflattex->v2 = vindex[2];
        	}
    	} else {
    		/* flat, no texture, no lighting */
    		triflat = (PsxTriFlat *)psxAddPrimitive( object, PSX_TRI_FLAT );
			/* add triangle data */
    		psxSetPrimitive( &triflat->prim, PSX_TRI_FLAT );
    		triflat->mode  = triflat->prim.mode;
			triflat->red   = ps.color.red;
    		triflat->green = ps.color.green;
    		triflat->blue  = ps.color.blue;
    		/* add vertex data */
    		psxAddVertex( object, pts, 3, vindex );
    		triflat->v0 = vindex[0];
    		triflat->v1 = vindex[1];
    		triflat->v2 = vindex[2];
#if GFX_DEBUG
    		l = (u_long *)triflat;
    		printf("tri (flat not lit) 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3) );
#endif
    		/* add double sided triangle if necessary */
        	if ( ps.doublesided ) {
 	    		triflat = (PsxTriFlat *)psxAddPrimitive( object, PSX_TRI_FLAT );
	    		/* add triangle data */
    		    psxSetPrimitive( &triflat->prim, PSX_TRI_FLAT );
    		    triflat->mode  = triflat->prim.mode;
	    		triflat->red   = ps.color.red;
    		    triflat->green = ps.color.green;
    		    triflat->blue  = ps.color.blue;
    		    /* add reversed vertex data */ 
    			triflat->v0 = vindex[1];
    			triflat->v1 = vindex[0];
    			triflat->v2 = vindex[2];
        	}
		}
	} else {
		/* lighting supported */
    	if ( ps.textureon ) {
			if ( normals != NULL ) {
				/* gouraud shaded, textured with lighting */
				atxy = psxAdjustTexturePoints( txy, 3 );
    			trigourtexlit = (PsxTriGourTexLit *)psxAddPrimitive( object, PSX_TRI_GOURAUD_TEX_LIT );
				/* add triangle data */
    			psxSetPrimitive( &trigourtexlit->prim, PSX_TRI_GOURAUD_TEX_LIT );
        		trigourtexlit->tx0    = atxy[0].x;
        		trigourtexlit->ty0    = atxy[0].y;
        		trigourtexlit->tx1    = atxy[1].x;
        		trigourtexlit->ty1    = atxy[1].y;
				trigourtexlit->tx2    = atxy[2].x;
				trigourtexlit->ty2    = atxy[2].y;
				trigourtexlit->unused = 0;
        		/* add texture info */
        		psxSetTextureInfo( &trigourtexlit->texture, &trigourtexlit->clut );
    			/* add vertex and normal data */
    			psxAddVertex( object, pts, 3, vindex );
    			trigourtexlit->v0 = vindex[0];
    			trigourtexlit->v1 = vindex[1];
    			trigourtexlit->v2 = vindex[2];
    			trigourtexlit->n0 = normals[0];
    			trigourtexlit->n1 = normals[1];
    			trigourtexlit->n2 = normals[2];
#if GFX_DEBUG
    			l = (u_long *)trigourtexlit;
    			printf("tri (goraud tex lit) 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3), *(l+4), *(l+5) );
#endif
				/* add double sided triangle if necessary */
        		if ( ps.doublesided ) {    	
    				trigourtexlit = (PsxTriGourTexLit *)psxAddPrimitive( object, PSX_TRI_GOURAUD_TEX_LIT );
    				psxSetPrimitive( &trigourtexlit->prim, PSX_TRI_GOURAUD_TEX_LIT );
        			trigourtexlit->tx0    = atxy[1].x;
        			trigourtexlit->ty0    = atxy[1].y;
        			trigourtexlit->tx1    = atxy[0].x;
        			trigourtexlit->ty1    = atxy[0].y;
	    			trigourtexlit->tx2    = atxy[2].x;
	    			trigourtexlit->ty2    = atxy[2].y;
	    			trigourtexlit->unused = 0;
           			/* add texture info */
  	    			psxSetTextureInfo( &trigourtexlit->texture, &trigourtexlit->clut );
            		/* add reversed points */
	    			trigourtexlit->n0 = normals[1];
					trigourtexlit->n1 = normals[0];
    				trigourtexlit->n2 = normals[2];
					trigourtexlit->v0 = vindex[1];
    				trigourtexlit->v1 = vindex[0];
    				trigourtexlit->v2 = vindex[2];
        		}		
			} else {
				/* flat shaded, textured with lighting */
				atxy = psxAdjustTexturePoints( txy, 3 );
    			triflattexlit = (PsxTriFlatTexLit *)psxAddPrimitive( object, PSX_TRI_FLAT_TEX_LIT );
				/* add triangle data */
    			psxSetPrimitive( &triflattexlit->prim, PSX_TRI_FLAT_TEX_LIT );
        		triflattexlit->tx0    = atxy[0].x;
        		triflattexlit->ty0    = atxy[0].y;
        		triflattexlit->tx1    = atxy[1].x;
        		triflattexlit->ty1    = atxy[1].y;
				triflattexlit->tx2    = atxy[2].x;
				triflattexlit->ty2    = atxy[2].y;
				triflattexlit->unused = 0;
        		/* add texture info */
        		psxSetTextureInfo( &triflattexlit->texture, &triflattexlit->clut );
        		/* add normal */
				normal = triCalcNormal( pts );
   				psxAddNormal( object, &normal, 1, nindex );
       			triflattexlit->n0 = nindex[0];
    			/* add vertex data */
    			psxAddVertex( object, pts, 3, vindex );
    			triflattexlit->v0 = vindex[0];
    			triflattexlit->v1 = vindex[1];
    			triflattexlit->v2 = vindex[2];
#if GFX_DEBUG
    			l = (u_long *)triflattexlit;
    			printf("tri (flat tex lit) 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3), *(l+4), *(l+5) );
#endif
				/* add double sided triangle if necessary */
        		if ( ps.doublesided ) {    	
    				triflattexlit = (PsxTriFlatTexLit *)psxAddPrimitive( object, PSX_TRI_FLAT_TEX_LIT );
    				psxSetPrimitive( &triflattexlit->prim, PSX_TRI_FLAT_TEX_LIT );
        			triflattexlit->tx0    = atxy[1].x;
        			triflattexlit->ty0    = atxy[1].y;
        			triflattexlit->tx1    = atxy[0].x;
        			triflattexlit->ty1    = atxy[0].y;
	    			triflattexlit->tx2    = atxy[2].x;
	    			triflattexlit->ty2    = atxy[2].y;
	    			triflattexlit->unused = 0;
           			/* add texture info */
  	    			psxSetTextureInfo( &triflattexlit->texture, &triflattexlit->clut );
            		/* add reversed points */
            		triflattexlit->n0 = nindex[0];
					triflattexlit->v0 = vindex[1];
    				triflattexlit->v1 = vindex[0];
    				triflattexlit->v2 = vindex[2];
        		}		
			}
    	} else {
			if ( normals != NULL ) {
    			/* goraud shaded, no texture with lighting */
    			trigourlit = (PsxTriGourLit *)psxAddPrimitive( object, PSX_TRI_GOURAUD_LIT );
				/* add triangle data */
    			psxSetPrimitive( &trigourlit->prim, PSX_TRI_GOURAUD_LIT );
    			trigourlit->mode  = trigourlit->prim.mode;
				trigourlit->red   = ps.color.red;
    			trigourlit->green = ps.color.green;
    			trigourlit->blue  = ps.color.blue;
    			/* add vertex and normal data */
    			psxAddVertex( object, pts, 3, vindex );
    			trigourlit->v0 = vindex[0];
    			trigourlit->v1 = vindex[1];
    			trigourlit->v2 = vindex[2];
				trigourlit->n0 = normals[0];
				trigourlit->n1 = normals[1];
				trigourlit->n2 = normals[2];
#if GFX_DEBUG
    			l = (u_long *)trigourlit;
    			printf("tri (gouraud lit) 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3) );
#endif
    			/* add double sided triangle if necessary */
				if ( ps.doublesided ) {
 	    			trigourlit = (PsxTriGourLit *)psxAddPrimitive( object, PSX_TRI_GOURAUD_LIT );
	    			/* add triangle data */
    				psxSetPrimitive( &trigourlit->prim, PSX_TRI_GOURAUD_LIT );
    				trigourlit->mode  = trigourlit->prim.mode;
	    			trigourlit->red   = ps.color.red;
    				trigourlit->green = ps.color.green;
    				trigourlit->blue  = ps.color.blue;
    				/* add reversed vertex data */ 
					trigourlit->n0 = normals[1];
					trigourlit->n1 = normals[0];
					trigourlit->n2 = normals[2];
					trigourlit->v0 = vindex[1];
    				trigourlit->v1 = vindex[0];
    				trigourlit->v2 = vindex[2];
				}
			} else {
    			/* flat, no texture with lighting */
    			triflatlit = (PsxTriFlatLit *)psxAddPrimitive( object, PSX_TRI_FLAT_LIT );
				/* add triangle data */
    			psxSetPrimitive( &triflatlit->prim, PSX_TRI_FLAT_LIT );
    			triflatlit->mode  = triflatlit->prim.mode;
				triflatlit->red   = ps.color.red;
    			triflatlit->green = ps.color.green;
    			triflatlit->blue  = ps.color.blue;
				/* add normal */
				if ( normals == NULL ) {
					normal = triCalcNormal( pts );
	    			psxAddNormal( object, &normal, 1, nindex );
					triflatlit->n0 = nindex[0];
				}
    			/* add vertex data */
    			psxAddVertex( object, pts, 3, vindex );
    			triflatlit->v0 = vindex[0];
    			triflatlit->v1 = vindex[1];
    			triflatlit->v2 = vindex[2];
#if GFX_DEBUG
    			l = (u_long *)triflatlit;
    			printf("tri (flat lit) 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3) );
#endif
    			/* add double sided triangle if necessary */
				if ( ps.doublesided ) {
 	    			triflatlit = (PsxTriFlatLit *)psxAddPrimitive( object, PSX_TRI_FLAT_LIT );
	    			/* add triangle data */
    				psxSetPrimitive( &triflatlit->prim, PSX_TRI_FLAT_LIT );
    				triflatlit->mode  = triflatlit->prim.mode;
	    			triflatlit->red   = ps.color.red;
    				triflatlit->green = ps.color.green;
    				triflatlit->blue  = ps.color.blue;
    				/* add reversed vertex data */ 
        			triflatlit->n0 = nindex[0];
					triflatlit->v0 = vindex[1];
    				triflatlit->v1 = vindex[0];
    				triflatlit->v2 = vindex[2];
				}
			}
		}
    }
}     


/**********************************************************
 * Function:	gfxAddQuad
 **********************************************************
 * Description: Add a quad to an object
 * Inputs:	object - object to add quad to
 *			pts - quad points
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxAddQuad (
    GfxObject	*object,
    Point_i3d	 pts[],
    Point_fx2d   txy[],
    int			 normals[]
    )
{
    PsxObject	    	*psxobj;
    PsxTmdObject    	*obj;
    size_t	     		 newsize;
    PsxQuadFlat		    *quadflat;
    PsxQuadFlatTex		*quadflattex;
    PsxQuadFlatLit	    *quadflatlit;
    PsxQuadFlatTexLit	*quadflattexlit;
    PsxQuadGourLit	    *quadgourlit;
    PsxQuadGourTexLit	*quadgourtexlit;
    PsxTexPoint  		*atxy;
    u_char	  			*c;
    short	  			*s;
    int		   			 i;
    u_long	  			*l;
    Point_fx3d	   		 normal;
	u_short				 nindex[2];
	u_short				 vindex[4];

    /* init */
    psxobj = (PsxObject *)object->data;
    obj = &(psxobj->object);

    /* create primitive */
	if ( ps.lightingoff ) {
		/* no lighting */
		if ( ps.textureon ) {
			/* flat shaded, textured, nolighting */
    		atxy = psxAdjustTexturePoints( txy, 4 );
			quadflattex = (PsxQuadFlatTex *)psxAddPrimitive( object, PSX_QUAD_FLAT_TEX );
    		psxSetPrimitive( &quadflattex->prim, PSX_QUAD_FLAT_TEX );
			quadflattex->tx0     = atxy[0].x;
			quadflattex->ty0     = atxy[0].y;
			quadflattex->tx1     = atxy[1].x;
			quadflattex->ty1     = atxy[1].y;
			quadflattex->tx2     = atxy[2].x;
			quadflattex->ty2     = atxy[2].y;
			quadflattex->unused0 = 0;
			quadflattex->tx3     = atxy[3].x;
			quadflattex->ty3     = atxy[3].y;
			quadflattex->unused1 = 0;			
			quadflattex->red     = ps.color.red;
    		quadflattex->green   = ps.color.green;
    		quadflattex->blue    = ps.color.blue;
			quadflattex->unused2 = 0;
			/* add texture info */
			psxSetTextureInfo( &quadflattex->texture, &quadflattex->clut );
			/* add vertex points */ 
    		psxAddVertex( object, pts, 4, vindex );
    		quadflattex->v0 = vindex[0];
    		quadflattex->v1 = vindex[1];
    		quadflattex->v2 = vindex[2];
    		quadflattex->v3 = vindex[3];
			quadflattex->unused2 = 0;
#if GFX_DEBUG
    		l = (u_long *)quadflattex;
    		printf("quad (flat tex) 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3),
    				*(l+4), *(l+5), *(l+6) );
#endif
			/* add double sided quad if necessary */
    		if ( ps.doublesided ) {
	    		quadflattex = (PsxQuadFlatTex *)psxAddPrimitive( object, PSX_QUAD_FLAT_TEX );
    			psxSetPrimitive( &quadflattex->prim, PSX_QUAD_FLAT_TEX );
				quadflattex->tx0     = atxy[0].x;
				quadflattex->ty0     = atxy[0].y;
				quadflattex->tx1     = atxy[2].x;
				quadflattex->ty1     = atxy[2].y;
	    		quadflattex->tx2     = atxy[1].x;
	    		quadflattex->ty2     = atxy[1].y;
	    		quadflattex->unused0 = 0;
	    		quadflattex->tx3     = atxy[3].x;
	    		quadflattex->ty3     = atxy[3].y;
	    		quadflattex->unused1 = 0;
				quadflattex->red     = ps.color.red;
				quadflattex->green   = ps.color.green;
    			quadflattex->blue    = ps.color.blue;
				quadflattex->unused2 = 0;
	    		/* add texture info */
				psxSetTextureInfo( &quadflattex->texture, &quadflattex->clut );
				/* add reversed vertex points */
    			quadflattex->v0 = vindex[0];
    			quadflattex->v1 = vindex[2];
    			quadflattex->v2 = vindex[1];
    			quadflattex->v3 = vindex[3];
			}
		} else {
			/* flat shaded, no texture, no lighting */
    		quadflat = (PsxQuadFlat *)psxAddPrimitive( object, PSX_QUAD_FLAT );
    		psxSetPrimitive( &quadflat->prim, PSX_QUAD_FLAT );
    		quadflat->mode  = quadflat->prim.mode;
			/* add color of quad */
    		quadflat->red   = ps.color.red;
    		quadflat->green = ps.color.green;
    		quadflat->blue  = ps.color.blue;
			/* add vertex points */ 
    		psxAddVertex( object, pts, 4, vindex );
    		quadflat->v0 = vindex[0];
    		quadflat->v1 = vindex[1];
    		quadflat->v2 = vindex[2];
    		quadflat->v3 = vindex[3];
#if GFX_DEBUG
    		l = (u_long *)quadflat;
    		printf("quad (flat) 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3) );
#endif
			/* add double sided quad if necessary */
			if ( ps.doublesided ) {
	    		/* flat shaded */
    			quadflat = (PsxQuadFlat *)psxAddPrimitive( object, PSX_QUAD_FLAT );
    			psxSetPrimitive( &quadflat->prim, PSX_QUAD_FLAT );
    			quadflat->mode  = quadflat->prim.mode;
				/* add color of quad */
    			quadflat->red   = ps.color.red;
    			quadflat->green = ps.color.green;
    			quadflat->blue  = ps.color.blue;
				/* add vertex points */
    			quadflat->v0 = vindex[0];
    			quadflat->v1 = vindex[2];
    			quadflat->v2 = vindex[1];
    			quadflat->v3 = vindex[3];
			}
		}
	} else {
		/* lighting supported */
		if ( ps.textureon ) {
			if ( normals != NULL ) {
				/* gouraud shaded, textured with lighting */
				atxy = psxAdjustTexturePoints( txy, 4 );
    			quadgourtexlit = (PsxQuadGourTexLit *)psxAddPrimitive( object, PSX_QUAD_GOURAUD_TEX_LIT );
				/* add quad data */
    			psxSetPrimitive( &quadgourtexlit->prim, PSX_QUAD_GOURAUD_TEX_LIT );
        		quadgourtexlit->tx0     = atxy[0].x;
        		quadgourtexlit->ty0     = atxy[0].y;
        		quadgourtexlit->tx1     = atxy[1].x;
        		quadgourtexlit->ty1     = atxy[1].y;
				quadgourtexlit->tx2     = atxy[2].x;
				quadgourtexlit->ty2     = atxy[2].y;
				quadgourtexlit->unused0 = 0;
				quadgourtexlit->tx3     = atxy[3].x;
				quadgourtexlit->ty3     = atxy[3].y;
				quadgourtexlit->unused1 = 0;
        		/* add texture info */
        		psxSetTextureInfo( &quadgourtexlit->texture, &quadgourtexlit->clut );
    			/* add vertex and normal data */
    			psxAddVertex( object, pts, 4, vindex );
    			quadgourtexlit->v0 = vindex[0];
    			quadgourtexlit->v1 = vindex[1];
    			quadgourtexlit->v2 = vindex[2];
    			quadgourtexlit->v3 = vindex[3];
    			quadgourtexlit->n0 = normals[0];
    			quadgourtexlit->n1 = normals[1];
    			quadgourtexlit->n2 = normals[2];
    			quadgourtexlit->n3 = normals[3];
#if GFX_DEBUG
    			l = (u_long *)quadgourtexlit;
    			printf("quad (goraud tex lit) 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3), *(l+4), *(l+5) );
#endif
				/* add double sided quad if necessary */
        		if ( ps.doublesided ) {    	
    				quadgourtexlit = (PsxQuadGourTexLit *)psxAddPrimitive( object, PSX_QUAD_GOURAUD_TEX_LIT );
    				psxSetPrimitive( &quadgourtexlit->prim, PSX_QUAD_GOURAUD_TEX_LIT );
        			quadgourtexlit->tx0     = atxy[0].x;
        			quadgourtexlit->ty0     = atxy[0].y;
        			quadgourtexlit->tx1     = atxy[2].x;
        			quadgourtexlit->ty1     = atxy[2].y;
	    			quadgourtexlit->tx2     = atxy[1].x;
	    			quadgourtexlit->ty2     = atxy[1].y;
	    			quadgourtexlit->unused0 = 0;
					quadgourtexlit->tx3     = atxy[3].x;
					quadgourtexlit->ty3     = atxy[3].y;
					quadgourtexlit->unused1 = 0;
           			/* add texture info */
  	    			psxSetTextureInfo( &quadgourtexlit->texture, &quadgourtexlit->clut );
            		/* add reversed points */
	    			quadgourtexlit->n0 = normals[0];
					quadgourtexlit->n1 = normals[2];
    				quadgourtexlit->n2 = normals[1];
    				quadgourtexlit->n3 = normals[3];
					quadgourtexlit->v0 = vindex[0];
    				quadgourtexlit->v1 = vindex[2];
    				quadgourtexlit->v2 = vindex[1];
    				quadgourtexlit->v3 = vindex[3];
        		}		
			} else {
				/* flat shaded, textured with lighting */
    			atxy = psxAdjustTexturePoints( txy, 4 );
				quadflattexlit = (PsxQuadFlatTexLit *)psxAddPrimitive( object, PSX_QUAD_FLAT_TEX_LIT );
    			psxSetPrimitive( &quadflattexlit->prim, PSX_QUAD_FLAT_TEX_LIT );
				quadflattexlit->tx0     = atxy[0].x;
				quadflattexlit->ty0     = atxy[0].y;
				quadflattexlit->tx1     = atxy[1].x;
				quadflattexlit->ty1     = atxy[1].y;
				quadflattexlit->tx2     = atxy[2].x;
				quadflattexlit->ty2     = atxy[2].y;
				quadflattexlit->unused0 = 0;
				quadflattexlit->tx3     = atxy[3].x;
				quadflattexlit->ty3     = atxy[3].y;
				quadflattexlit->unused1 = 0;
				/* add texture info */
				psxSetTextureInfo( &quadflattexlit->texture, &quadflattexlit->clut );
				/* add normal */
				normal = triCalcNormal( pts );
   				psxAddNormal( object, &normal, 1, nindex );
				quadflattexlit->n0 = nindex[0];
				/* add vertex points */ 
    			psxAddVertex( object, pts, 4, vindex );
    			quadflattexlit->v0 = vindex[0];
    			quadflattexlit->v1 = vindex[1];
    			quadflattexlit->v2 = vindex[2];
    			quadflattexlit->v3 = vindex[3];
				quadflattexlit->unused2 = 0;
#if GFX_DEBUG
    			l = (u_long *)quadflattexlit;
    			printf("quad (flat tex lit) 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3),
    					*(l+4), *(l+5), *(l+6) );
#endif
				/* add double sided quad if necessary */
    			if ( ps.doublesided ) {
	    			quadflattexlit = (PsxQuadFlatTexLit *)psxAddPrimitive( object, PSX_QUAD_FLAT_TEX_LIT );
    				psxSetPrimitive( &quadflattexlit->prim, PSX_QUAD_FLAT_TEX_LIT );
					quadflattexlit->tx0     = atxy[0].x;
					quadflattexlit->ty0     = atxy[0].y;
					quadflattexlit->tx1     = atxy[2].x;
					quadflattexlit->ty1     = atxy[2].y;
	    			quadflattexlit->tx2     = atxy[1].x;
	    			quadflattexlit->ty2     = atxy[1].y;
	    			quadflattexlit->unused0 = 0;
	    			quadflattexlit->tx3     = atxy[3].x;
	    			quadflattexlit->ty3     = atxy[3].y;
	    			quadflattexlit->unused1 = 0;
	    			/* add texture info */
					psxSetTextureInfo( &quadflattexlit->texture, &quadflattexlit->clut );
					/* add normal */
					quadflattexlit->n0 = nindex[0];
					/* add reversed vertex points */
    				quadflattexlit->v0 = vindex[0];
    				quadflattexlit->v1 = vindex[2];
    				quadflattexlit->v2 = vindex[1];
    				quadflattexlit->v3 = vindex[3];
	    			quadflattexlit->unused2 = 0;
				}
			}
		} else {
			if ( normals != NULL ) {
    			/* goraud shaded, no texture with lighting */
    			quadgourlit = (PsxQuadGourLit *)psxAddPrimitive( object, PSX_QUAD_GOURAUD_LIT );
				/* add triangle data */
    			psxSetPrimitive( &quadgourlit->prim, PSX_QUAD_GOURAUD_LIT );
    			quadgourlit->mode  = quadgourlit->prim.mode;
				quadgourlit->red   = ps.color.red;
    			quadgourlit->green = ps.color.green;
    			quadgourlit->blue  = ps.color.blue;
    			/* add vertex and normal data */
    			psxAddVertex( object, pts, 4, vindex );
    			quadgourlit->v0 = vindex[0];
    			quadgourlit->v1 = vindex[1];
    			quadgourlit->v2 = vindex[2];
				quadgourlit->v3 = vindex[3];
				quadgourlit->n0 = normals[0];
				quadgourlit->n1 = normals[1];
				quadgourlit->n2 = normals[2];
				quadgourlit->n3 = normals[3];
#if GFX_DEBUG
    			l = (u_long *)quadgourlit;
    			printf("quad (gouraud lit) 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3) );
#endif
    			/* add double sided triangle if necessary */
				if ( ps.doublesided ) {
 	    			quadgourlit = (PsxQuadGourLit *)psxAddPrimitive( object, PSX_QUAD_GOURAUD_LIT );
	    			/* add triangle data */
    				psxSetPrimitive( &quadgourlit->prim, PSX_QUAD_GOURAUD_LIT );
    				quadgourlit->mode  = quadgourlit->prim.mode;
	    			quadgourlit->red   = ps.color.red;
    				quadgourlit->green = ps.color.green;
    				quadgourlit->blue  = ps.color.blue;
    				/* add reversed vertex data */ 
					quadgourlit->n0 = normals[0];
					quadgourlit->n1 = normals[2];
					quadgourlit->n2 = normals[1];
					quadgourlit->n3 = normals[3];
					quadgourlit->v0 = vindex[0];
    				quadgourlit->v1 = vindex[2];
    				quadgourlit->v2 = vindex[1];
    				quadgourlit->v3 = vindex[3];
				}
			} else {
				/* flat shaded, no texture with lighting */
    			quadflatlit = (PsxQuadFlatLit *)psxAddPrimitive( object, PSX_QUAD_FLAT_LIT );
    			psxSetPrimitive( &quadflatlit->prim, PSX_QUAD_FLAT_LIT );
    			quadflatlit->mode  = quadflatlit->prim.mode;
				/* add color of quad */
    			quadflatlit->red   = ps.color.red;
    			quadflatlit->green = ps.color.green;
    			quadflatlit->blue  = ps.color.blue;
				/* add normal */
				normal = triCalcNormal( pts );
   				psxAddNormal( object, &normal, 1, nindex );
				quadflatlit->n0 = nindex[0];
				/* add vertex points */ 
    			psxAddVertex( object, pts, 4, vindex );
    			quadflatlit->v0 = vindex[0];
    			quadflatlit->v1 = vindex[1];
    			quadflatlit->v2 = vindex[2];
    			quadflatlit->v3 = vindex[3];
#if GFX_DEBUG
    			l = (u_long *)quadflatlit;
    			printf("quad (flat lit) 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3) );
#endif
				/* add double sided quad if necessary */
				if ( ps.doublesided ) {
	    			/* flat shaded */
    				quadflatlit = (PsxQuadFlatLit *)psxAddPrimitive( object, PSX_QUAD_FLAT_LIT );
    				psxSetPrimitive( &quadflatlit->prim, PSX_QUAD_FLAT_LIT );
    				quadflatlit->mode  = quadflatlit->prim.mode;
					/* add color of quad */
    				quadflatlit->red   = ps.color.red;
    				quadflatlit->green = ps.color.green;
    				quadflatlit->blue  = ps.color.blue;
					/* add normal */
					quadflatlit->n0 = nindex[0];
					/* add vertex points */
    				quadflatlit->v0 = vindex[0];
    				quadflatlit->v1 = vindex[2];
    				quadflatlit->v2 = vindex[1];
    				quadflatlit->v3 = vindex[3];
				}
			}
		}
	}
}


/**********************************************************
 * Function:	gfxDrawObject
 **********************************************************
 * Description: Draw an object
 * Inputs:		object - object to draw
 * Notes:
 * Returns:		void
 **********************************************************/

public void gfxDrawObject (
    GfxObject	*object
    )
{
    PsxObject		*psxobj;
    PsxMatrix		 lmat, smat;
	PsxObjHandler	*handler;

#if GFX_VALIDATE
	if ( object == NULL ) {
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}	
#endif

	/* init */
    psxobj  = (PsxObject *)object->data;
	handler = &psxobj->current->handler;

#if GFX_VALIDATE
	if ( psxobj == NULL ) {
		printf( "psxobj is NULL\n" );
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}	
#endif

    /* update state flag */
    psxobj->state = PSX_OBJ_DRAWN;
	//printf( "draw: obj %X drawing, %d ", psxobj, psxobj->state );

    /* link if first time drawing this object */
    if ( ! psxobj->current->linked ) {
    	GsLinkObject4( (u_long)&(psxobj->object), (GsDOBJ2 *)handler, 0 );
        psxobj->current->linked = TRUE;
    }

    /* get the local screen matrix for the object */
    GsGetLws( (GsCOORDINATE2 *)handler->position, (MATRIX *)&lmat, (MATRIX *)&smat );

    /* set the matrix */
    GsSetLightMatrix( (MATRIX *)&lmat );
    GsSetLsMatrix( (MATRIX *)&smat );

	/* add to order table */
	if ( ps.split_screen.on ) {
		GsSortObject4( ((GsDOBJ2 *)handler), ((GsOT *)&ps.screen.split.worldorder[(ps.drawpage + ps.split_screen.active_buffer)]),
					(14 - ((PSX_OT_LENGTH - 1) - ps.depth_step)),
					(u_long *)getScratchAddr(0) );
	} else {
		GsSortObject4( (GsDOBJ2 *)handler, (GsOT *)&ps.screen.full.worldorder[ps.drawpage],
					(14 - (PSX_OT_LENGTH - ps.depth_step)),
					(u_long *)getScratchAddr(0) );
	}
}


/**********************************************************
 * Function:	gfxRotateObject
 **********************************************************
 * Description: Rotate an object
 * Inputs:		object - object to rotate
 *				axis - axis to rotate on
 *				angle - angle in degrees to rotate by
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxRotateObject (
    GfxObject	*object,
    char 		 axis,
    int 		 angle
    )
{ 
    PsxObject	    *psxobj;
    PsxObjPosition  *position;

    /* validate */
    if ( angle == 0 ) return;

#if GFX_VALIDATE
	/* allow up to rotation of 2000 either way as a limit */
	if ( gfxAbs(angle) > 2000 ) { 
		gfxError( GFX_ERROR_BAD_ROTATION_ANGLE );
		gfxAbort(); 
	}
#endif

	/* force into range -179..180 */
	while( angle > 180 )
		angle -= 360;
	while( angle < -179 )
		angle += 360;

    /* init */
    psxobj = (PsxObject *)object->data;
    position = psxGetObjectPosition( psxobj );

    psxRotate( position, axis, angle );
}


/**********************************************************
 * Function:	gfxTranslateObject
 **********************************************************
 * Description: Translate (move) an object 
 * Inputs:		object - object to move
 *				x - distance to move in x axis   
 *				y - distance to move in y axis
 *				z - distance to move in z axis
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxTranslateObject(
    GfxObject 	*object,
    int 		 x,
    int 		 y,
    int 		 z
    )
{
    PsxObject	    *psxobj;
    PsxObjPosition  *position;

    /* init */
    psxobj = (PsxObject *)object->data;
    position = psxGetObjectPosition( psxobj );

    psxTranslate( position, x, y, z );
}                  


/**********************************************************
 * Function:	gfxScaleObject
 **********************************************************
 * Description: Scale (enlarge/shrink) an object 
 * Inputs:		object - object to scale
 *				x - scale factor for x axis   
 *				y - scale factor for y axis
 *				z - scale factor for z axis
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxScaleObject (
    GfxObject 	*object,
    fixed 	 	 x,
    fixed 	 	 y,
    fixed 	 	 z
    )
{
    PsxObject	    *psxobj;
    PsxObjPosition  *position;

    /* init */
    psxobj = (PsxObject *)object->data;
    position = psxGetObjectPosition( psxobj );

#if GFX_VALIDATE
	/* matrix will crap out at scale > 8 */
	if (( x == 0 || gfxAbs(x) > 32768 ) ||
		( y == 0 || gfxAbs(y) > 32768 ) ||
		( z == 0 || gfxAbs(z) > 32768 )) {
		gfxError( GFX_ERROR_BAD_SCALE );
		gfxAbort();
	}
#endif

    psxScale( position, x, y, z );
}


/**********************************************************
 * Function:	gfxGetObjectMatrix
 **********************************************************
 * Description: Get the matrix from an object
 * Inputs:	void
 * Notes:	
 * Returns:	object matrix
 **********************************************************/

public GfxMatrix gfxGetObjectMatrix (
    GfxObject 	*object
    )
{
    GfxMatrix	 gfxmat;
    PsxMatrix	 mat;
    PsxObject	*psxobj;

#if GFX_VALIDATE
	if ( object == NULL ) {
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}
#endif

    /* init */
    psxobj = (PsxObject *)object->data;
    //mat = psxobj->position.coord;

	memCopy( &psxobj->current->position.coord, &gfxmat, sizeof(PsxMatrix) );

    return( gfxmat );
}     

    
/**********************************************************
 * Function:	gfxSetPivot
 **********************************************************
 * Description: Select a pivot point in a skinned object
 * Inputs:		object - object
 *				pivot  - pivot index
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxSetPivot ( 
	GfxObject	*object,
	int			 pivot
	)
{
	/* not supported */
	//gfxError( GFX_ERROR_SKINNED_TMD_NOT_SUPPORTED );
	//gfxAbort();
}


/**********************************************************
 * Function:	gfxSetObjectMatrix
 **********************************************************
 * Description: Set the matrix for an object
 * Inputs:	void
 * Notes:	
 * Returns:	object matrix
 **********************************************************/

public void gfxSetObjectMatrix (
    GfxObject 	*object,
    GfxMatrix   *gfxmat
    )
{
    PsxMatrix	     mat;
    PsxObject	    *psxobj;
    PsxObjPosition  *position;

    /* init */
    psxobj = (PsxObject *)object->data;
    position = psxGetObjectPosition( psxobj );

    /* store matrix to object */
	memCopy( gfxmat, &position->coord, sizeof(PsxMatrix) );
	position->flag  = 0;
}


/**********************************************************
 * Function:	gfxGetObjectPoint
 **********************************************************
 * Description: Get a vertex point from an object
 * Inputs:		void
 * Notes:	
 * Returns:		object vertex
 **********************************************************/

public Point_i3d gfxGetObjectPoint (
    GfxObject 	*object,
	int			 v
    )
{
    PsxObject	 *psxobj;
	PsxTmdObject *tmd;
	Point_i3d	  pt;
	PsxVertex	 *vert;
	int			  i;

	/* init (point 1 = offset 0) */
	v--;

#if GFX_VALIDATE
	if ( object == NULL ) {
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}
#endif

    /* init */
    psxobj = (PsxObject *)object->data;
	tmd = &psxobj->object;

#if GFX_VALIDATE
	if ( v >= tmd->n_vert ) {
		gfxError( GFX_ERROR_POINT_OUT_OF_RANGE );
		gfxAbort();
	}
#endif
   
	/* get vertex */
	vert = (PsxVertex *)tmd->vert_top;
	pt.x = vert[v].x;
	pt.y = vert[v].z;
	pt.z = -(vert[v].y);

	return( pt );
}


/**********************************************************
 * Function:	gfxGetObjectNumPoints
 **********************************************************
 * Description: Get a vertex point from an object
 * Inputs:		void
 * Notes:	
 * Returns:		object vertex
 **********************************************************/

public int gfxGetObjectNumPoints (
    GfxObject 	*object
    )
{
    PsxObject	 *psxobj;
	PsxTmdObject *tmd;

#if GFX_VALIDATE
	if ( object == NULL ) {
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}
#endif

    /* init */
    psxobj = (PsxObject *)object->data;
	tmd = &psxobj->object;

	return( (int)tmd->n_vert );
}


/**********************************************************
 * Function:	gfxSetObjectPoint
 **********************************************************
 * Description: Set/update a vertex point in an object
 * Inputs:		void
 * Notes:		point must already exist
 * Returns:		object vertex
 **********************************************************/

public void gfxSetObjectPoint (
    GfxObject 	*object,
	int			 v,
	Point_i3d	*pt
    )
{
    PsxObject	 *psxobj;
	PsxTmdObject *tmd;
	PsxVertex	 *vert;

	/* init (point 1 = offset 0) */
	v--;

#if GFX_VALIDATE
	if ( object == NULL || pt == NULL ) {
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}
#endif

    /* init */
    psxobj = (PsxObject *)object->data;
	tmd = &psxobj->object;

#if GFX_VALIDATE
	if ( v >= tmd->n_vert ) {
		gfxError( GFX_ERROR_POINT_OUT_OF_RANGE );
		gfxAbort();
	}
#endif
   
	/* set vertex */
	vert = (PsxVertex *)tmd->vert_top;
	vert[v].x = (short)pt->x;
	vert[v].y = (short) -(pt->z);
	vert[v].z = (short)pt->y;
}


/**********************************************************
 * Function:	gfxSetObjectNormal
 **********************************************************
 * Description: Set/update a point normal in an object
 * Inputs:		void
 * Notes:		point normal must already exist
 * Returns:		nil
 **********************************************************/

public void gfxSetObjectNormal (
    GfxObject 	*object,
	int			 n,
	Point_fx3d	*normal
    )
{
    PsxObject	 *psxobj;
	PsxTmdObject *tmd;
	PsxVertex	 *norm;

	/* init (normal 1 = offset 0) */
	n--;

#if GFX_VALIDATE
	if ( object == NULL || normal == NULL ) {
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}
#endif

    /* init */
    psxobj = (PsxObject *)object->data;
	tmd = &psxobj->object;

#if GFX_VALIDATE
	if ( n >= tmd->n_normal ) {
		gfxError( GFX_ERROR_NORMAL_OUT_OF_RANGE );
		gfxAbort();
	}
#endif
   
	/* set vertex */
	norm = (PsxVertex *)tmd->normal_top;
	norm[n].x = (short)normal->x;
	norm[n].y = (short) -(normal->z);
	norm[n].z = (short)normal->y;
}


/****************************
 *
 * Private PSX functions... 
 *
 ****************************/


/**********************************************************
 * Function:	psxClearModels
 **********************************************************
 * Description: Clear (or init) models currently loaded
 * Inputs:		void
 * Notes:	
 * Returns:		void
 **********************************************************/

void psxClearModels (	
	void
	)
{
	int		i;

    /* initialise static graphics data storage */
    for ( i=0; i<PSX_MAX_OBJECT; i++ ) {
    	//ps.tmd.gfxobj[i].dynamic = FALSE;
		ps.tmd.gfxobj[i].id      = i;
        ps.tmd.gfxobj[i].data    = &ps.tmd.psxobj[i];
    }
    ps.tmd.primitive_size = 0;
    ps.tmd.nobjects       = 0;
    ps.tmd.n_vert	     = 0;
    ps.tmd.n_normal	     = 0;

	/* reset extra handlers */
	ps.nextrahandlers = 0;
}


/**********************************************************
 * Function:	psxClear
 **********************************************************
 * Description: Clear the PSX ordering table
 * Inputs:	void
 * Notes:	Must be called each frame buffer swap
 * Returns:	void
 **********************************************************/

private void psxClear (
    void
    )
{
    register unsigned int	i;

    /* init */
    i = ps.drawpage;

    /* set address for GPU scratchpad area */
    GsSetWorkBase( (PACKET*)ps.gpupacket[i] );
    /* clear the ordering table */
	if ( ps.split_screen.on ) {
		GsClearOt( 0, 0, (GsOT *)&ps.screen.split.worldorder[i] );
		GsClearOt( 0, 0, (GsOT *)&ps.screen.split.worldorder[(i + 2)] );
		GsClearOt( 0, 0, (GsOT *)&ps.screen.split.worldorder[(i + 4)] );
	} else {
		GsClearOt( 0, 0, (GsOT *)&ps.screen.full.worldorder[i] );
	}

    /* update object state */
    for ( i=0; i<ps.tmd.nobjects; i++ ) {
    	ps.tmd.psxobj[i].state = PSX_OBJ_NOTDRAWN;
        ps.tmd.psxobj[i].current = &ps.tmd.psxobj[i].handle_list;
    }
}


/**********************************************************
 * Function:	psxSetPrimitive
 **********************************************************
 * Description: Set the mode info for a PSX primitive
 * Inputs:	prim - pointer to primitive
 *		type - primitive type
 * Notes:	
 * Returns:	void
 **********************************************************/

private void psxSetPrimitive (
    PsxPrimitive *prim,
    PsxObjType	  type
    )
{
    switch( type ) {
    case PSX_TRI_FLAT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = FALSE;
    	prim->olen 	 			 = 4;
    	prim->ilen 				 = 3;
    	prim->mode.has_texture   = FALSE;
    	prim->mode.texture_as_is = TRUE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = TRUE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_TRI_FLAT_LIT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = FALSE;
    	prim->olen 	 			 = 4;
    	prim->ilen 				 = 3;
    	prim->mode.has_texture   = FALSE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break; 
    case PSX_TRI_FLAT_TEX:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = FALSE;
    	prim->olen 		 		 = 7;
    	prim->ilen 		 		 = 6;
    	prim->mode.has_texture   = TRUE; 
    	prim->mode.texture_as_is = TRUE; 
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = TRUE; 
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_TRI_FLAT_TEX_LIT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = FALSE;
    	prim->olen 		 		 = 7;
    	prim->ilen 		 		 = 5;
    	prim->mode.has_texture   = TRUE; 
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_TRI_GOURAUD_LIT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = FALSE;
    	prim->olen 		 		 = 6;
    	prim->ilen 		 		 = 4;
    	prim->mode.has_texture   = FALSE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = TRUE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_TRI_GOURAUD_TEX_LIT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = FALSE;
    	prim->olen 		 		 = 9;
    	prim->ilen 		 		 = 6;
    	prim->mode.has_texture   = TRUE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = TRUE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_QUAD_FLAT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = TRUE;
    	prim->olen 	 			 = 4;
    	prim->ilen 				 = 3;
    	prim->mode.has_texture   = FALSE;
    	prim->mode.texture_as_is = TRUE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = TRUE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_QUAD_FLAT_TEX:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = TRUE;
    	prim->olen 	 			 = 4;
    	prim->ilen 				 = 3;
    	prim->mode.has_texture   = FALSE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break; 
    case PSX_QUAD_FLAT_LIT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = TRUE;
    	prim->olen 		 		 = 5;
    	prim->ilen 		 		 = 3;
    	prim->mode.has_texture   = FALSE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;      
    case PSX_QUAD_FLAT_TEX_LIT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad			 = TRUE;
    	prim->olen 				 = 9;
    	prim->ilen 				 = 7;
    	prim->mode.has_texture   = TRUE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_QUAD_GOURAUD_LIT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = TRUE;
    	prim->olen 		 		 = 8;
    	prim->ilen 		 		 = 5;
    	prim->mode.has_texture   = FALSE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = TRUE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_QUAD_GOURAUD_TEX_LIT:
    	prim->mode.type 		 = PSX_POLYGON;
    	prim->mode.quad 		 = TRUE;
    	prim->olen 		 		 = 12;
    	prim->ilen 		 		 = 8;
    	prim->mode.has_texture   = TRUE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = TRUE;
    	prim->lightingoff 		 = FALSE;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
        break;
    case PSX_LINE_FLAT: 
    	prim->mode.type 		 = PSX_LINE;
    	prim->mode.quad 		 = FALSE;
    	prim->olen 				 = 3;
    	prim->ilen 				 = 2;
    	prim->mode.has_texture   = FALSE;
    	prim->mode.texture_as_is = FALSE;
    	prim->mode.transparency  = ps.transparency;
    	prim->mode.gouraud		 = FALSE;
    	prim->lightingoff 		 = ps.lightingoff;
    	prim->doublesided 		 = FALSE;
    	prim->gradation   		 = FALSE;
    	prim->unused      		 = 0x0;
		break;
    }
}


/**********************************************************
 * Function:	psxAddPrimitive
 **********************************************************
 * Description: Add a PSX primitive to a PSX object
 * Inputs:	psxobj - object to add to
 *		type - primitive type to add
 * Notes:	Allocates/reallocs memory for the primitive
 *		and increments the primitive count	
 * Returns:	pointer to new primitive memory
 **********************************************************/

private void *psxAddPrimitive (
    GfxObject 	*gfxobj,
    PsxObjType 	 type
    )
{
    int 	  	  size;
    int		  	  newsize;
    void	 	 *prim;
    PsxTmdObject *obj;
    PsxObject 	 *psxobj;
    void		 *data;

    /* init */
    psxobj = (PsxObject *)gfxobj->data;
    obj = &(psxobj->object);

    /* work out the size of the primitive we are adding */
    switch( type ) {
    case PSX_TRI_FLAT:
    	size = sizeof(PsxTriFlat);
        break;       
    case PSX_TRI_FLAT_LIT:
    	size = sizeof(PsxTriFlatLit);
        break;         
    case PSX_TRI_FLAT_TEX:
    	size = sizeof(PsxTriFlatTex);
        break;        
    case PSX_TRI_FLAT_TEX_LIT:
    	size = sizeof(PsxTriFlatTexLit);
        break;        
    case PSX_TRI_GOURAUD_LIT:
    	size = sizeof(PsxTriGourLit);
        break;
    case PSX_TRI_GOURAUD_TEX_LIT:
    	size = sizeof(PsxTriGourTexLit);
        break;
    case PSX_QUAD_FLAT:
    	size = sizeof(PsxQuadFlat);
        break;          
    case PSX_QUAD_FLAT_TEX:
    	size = sizeof(PsxQuadFlatTex);
        break;
    case PSX_QUAD_FLAT_LIT:
    	size = sizeof(PsxQuadFlatLit);
        break;          
    case PSX_QUAD_FLAT_TEX_LIT:
    	size = sizeof(PsxQuadFlatTexLit);
        break;
    case PSX_QUAD_GOURAUD_LIT:
    	size = sizeof(PsxQuadGourLit);
        break;          
    case PSX_QUAD_GOURAUD_TEX_LIT:
    	size = sizeof(PsxQuadGourTexLit);
        break;
    case PSX_LINE_FLAT:
    	size = sizeof(PsxLineFlat);
        break;
    default:
    	return(NULL);
    }

    /* size is in longs */   
    size /= 4;

    /* update static/dynamic primitive buffer size */
    ps.tmd.primitive_size += size;
	//printf( "object=%s, prim=%d, %d\n", ((gfxobj->name==NULL)?("NULL"):gfxobj->name), size, ps.tmd.primitive_size );
    if ( (ps.tmd.primitive_size<<2) > PSX_PRIMITIVE_BUFFER_SIZE ) {
		gfxError( GFX_ERROR_PRIMITIVE_OVERRUN );
		gfxAbort();
	}

    /* update object info */
    prim = (void *)(obj->primitive_top + psxobj->primitive_size);
#if GFX_DEBUG
    printf("primitive top = 0x%X\n", (u_long *)prim );
#endif
    psxobj->primitive_size += size;    
    obj->n_primitive++;

    /* return pointer to new primitive memory */
    return( prim );
}

 
/**********************************************************
 * Function:	psxAddVertex
 **********************************************************
 * Description: Add a PSX vertex to a PSX object
 * Inputs:	object - PSX object to add vertex to
 *			pts - vertex points to add
 *			vcnt - number of points
 * Notes:	
 * Returns:	vindex
 **********************************************************/

internal void psxAddVertex (
    GfxObject    *gfxobj,
    Point_i3d	  pts[],
    int		  	  vcnt,
	u_short		 *vindex
    )
{
    PsxVertex 	 *vertex;
    size_t	  	  newsize;
    PsxTmdObject *object;
    PsxObject 	 *psxobj;
    void		 *data;    
    int			  i, v;
	Bool		  found;

    /* init */
    psxobj = (PsxObject *)gfxobj->data;
    object = &(psxobj->object);

#if GFX_DEBUG
	printf( "psxAddVertex\n" );
#endif
	for ( v=0; v<vcnt; v++ ) {
		/* find if we have already set this vertex */
		found = FALSE;

#if GFX_USE_EXISTING_POINTS
		i = 0;
		while( i < object->n_vert ) {
			vertex = (PsxVertex *)object->vert_top;
			if ( ( (short)pts[v].x     == vertex[i].x ) &&
				 ( (short) -(pts[v].z) == vertex[i].y ) &&
				 ( (short)pts[v].y     == vertex[i].z ) ) {
				if ( vindex )
					vindex[v] = i;
				found = TRUE;
				break;
			}
			i++;
		}
#endif
		/* add vertex to list */
		if ( ! found ) {
		    /* update static/dynamic vertex counter */
			vertex = &ps.tmd.vertex[ps.tmd.n_vert];
			//if ( vindex )
			//	vindex[v] = ps.tmd.n_vert;
			ps.tmd.n_vert++;
			if ( ps.tmd.n_vert >= PSX_MAX_VERTEX ) {
				gfxError( GFX_ERROR_VERTEX_OVERRUN );
				gfxAbort();
			}
			/* add vertex */
		   	vertex->x = (short)pts[v].x;
			vertex->y = (short)-(pts[v].z);
			vertex->z = (short)pts[v].y;
			vertex->unused = 0;	
#if GFX_DEBUG
			printf("add point %d = %d, %d, %d, %X\n", ps.tmd.n_vert, vertex->x, vertex->y, vertex->z, vertex );
#endif
			if ( vindex )
				vindex[v] = object->n_vert;
    		object->n_vert++;
		}
    }
}


/**********************************************************
 * Function:	psxAddNormal
 **********************************************************
 * Description: Add a PSX normal to a PSX object
 * Inputs:	object - PSX object to add vertex to
 *		pts - normal points to add
 *		vcnt - number of points
 * Notes:	
 * Returns:	void
 **********************************************************/

internal void psxAddNormal (
    GfxObject 	 *gfxobj,
    Point_fx3d	  pts[],
    int		 	  ncnt,
	u_short		 *nindex
    )
{
    PsxNormal 	 *normal;
    size_t	 	  newsize;
    PsxTmdObject *object;
    PsxObject 	 *psxobj;
    int			  i, n;
	Bool		  found;

    /* validate */
#if GFX_VALIDATE
    if ( pts == NULL ) {
		gfxError( GFX_ERROR_NULL_NORMAL_PASSED );
		gfxAbort();
    }
#endif

    /* init */
    psxobj = (PsxObject *)gfxobj->data;
    object = &(psxobj->object);

	for ( n=0; n<ncnt; n++ ) {
		/* update static/dynamic normal counter */
		normal = &ps.tmd.normal[ps.tmd.n_normal];
		nindex[n] = ps.tmd.n_normal;
		ps.tmd.n_normal++;
#if GFX_VALIDATE
		if ( ps.tmd.n_normal >= PSX_MAX_NORMAL ) {
			gfxError( GFX_ERROR_NORMAL_OVERRUN );
			gfxAbort();
		}
#endif
		/* add normal */
       	normal->x = (short)pts[n].x;
    	normal->y = (short) -(pts[n].z);
    	normal->z = (short)pts[n].y;
#if GFX_DEBUG
		//printf2( "normal (passed) = %f, %f, %f\n", pts[i].x, pts[i].y, pts[i].z );
    	printf( "add normal %s %d = %d, %d, %d, %X\n", gfxobj->name, ps.tmd.n_normal, normal->x, normal->y, normal->z, normal );
#endif
		normal->unused = 0;

		if ( nindex )
			nindex[n] = object->n_normal;

		object->n_normal++;
    }
}


/**********************************************************
 * Function:	psxNewObject
 **********************************************************
 * Description: Allocate storage for a new GFX object
 * Inputs:	dynamic - dynamic or static object
 * Notes:	new object pointer
 * Returns:	void
 **********************************************************/

private GfxObject * psxNewObject (
	Text		 name,
    Bool    	 unused
    )
{
    int			 i;
    u_long		*nobjects;
    GfxObject	*newobj;

    i = ps.drawpage;
   	nobjects = &ps.tmd.nobjects;
   	if ( *nobjects == (PSX_MAX_OBJECT - 1) ) {
		gfxError( GFX_ERROR_OBJECT_OVERRUN );
		gfxAbort();
	}
    newobj = &ps.tmd.gfxobj[*nobjects];
    (*nobjects)++;
	//printf( "obj=%d\n", *nobjects );

	/* set name */
	newobj->name = name;

    return( newobj );
}


/**********************************************************
 * Function:	psxScale
 **********************************************************
 * Description: Scale a PSX Object
 * Inputs:	position - positional info to update
 *              x - x scale factor     
 *              y - y scale factor
 *              z - z scale factor
 * Notes:	
 * Returns:	updated position
 **********************************************************/

private void psxScale (
    PsxObjPosition	*position,
    fixed		 	 x,
    fixed		 	 y,
    fixed		 	 z
    )
{ 
    VECTOR		 vector;
    PsxMatrix	*mat;
   
    /* init */ 
    mat = &position->coord;
    vector.vx = (long)x;
    vector.vy = (long)z;
    vector.vz = (long)y;

    /* add translation */
    ScaleMatrix( (MATRIX *)mat, &vector );

    /* update flag */
    position->flag = 0;
}


/**********************************************************
 * Function:	psxTranslate
 **********************************************************
 * Description: Translate a PSX Object
 * Inputs:	position - positional info to update
 *              x - x translation     
 *              y - y translation
 *              z - z translation
 * Notes:	
 * Returns:	updated position
 **********************************************************/

private void psxTranslate (
    PsxObjPosition 	*position,
    int		 	 	 x,
    int		 	 	 y,
    int		 	 	 z
    )
{
    VECTOR		 vector;
    PsxMatrix	*mat;

    /* init */ 
    mat = &position->coord;

#if 1
    /* update matrix */
    mat->t[0] += (long)x; 
    mat->t[1] += (long)-(z);
    mat->t[2] += (long)y;
	//printf2("%f, %f, %f psx trans = %d,%d,%d\n", x, y, z, mat->t[0], mat->t[1], mat->t[2] );
#else
	vector.vx = (long)x;
	vector.vy = (long)-(z);
	vector.vz = (long)y;
	TransMatrix( mat, &vector );
#endif

    /* update flag */
    position->flag = 0;
}


/**********************************************************
 * Function:	psxRotate
 **********************************************************
 * Description: Rotate a PSX Object
 * Inputs:	position - positional info to update
 *		axis  - axis to rotate on
 *		angle - angle in degrees to rotate by
 * Notes:	
 * Returns:	updated position
 **********************************************************/

private void psxRotate (
    PsxObjPosition	*position,
    char 	 		 axis,
    int 			 angle
    )
{
    PsxMatrix	*mat;
    long	 	 rot;

    /* init */ 
    mat = &position->coord;

    /* perform rotation */
    switch( axis ) {
    case 'x':
    case 'X':
		rot = DEGREES_TO_PSXANGLE(angle);
    	RotMatrixX( rot, (MATRIX *)mat );
        break;
    case 'y':
    case 'Y': 
		rot = DEGREES_TO_PSXANGLE(angle);
    	RotMatrixZ( rot, (MATRIX *)mat );
        break;
    case 'z':
    case 'Z':
		rot = DEGREES_TO_PSXANGLE(angle);
    	RotMatrixY( rot, (MATRIX *)mat );
        break;
    }

    /* update flag */
    position->flag = 0;
}
        

/**********************************************************
 * Function:	psxGetObjectPosition
 **********************************************************
 * Description: Get position data for a PSX Object
 * Inputs:	psxobj - PSX Object 
 * Notes:	Used ONLY by scale/translate/rotate functions	
 * Returns:	position pointer
 **********************************************************/

private  PsxObjPosition * psxGetObjectPosition (
    PsxObject	*psxobj
    )
{
	PsxObjHandleList *list;

#if GFX_VALIDATE
#endif

    /* if 'drawn' object we need to add a new position handler */
    if ( psxobj->state == PSX_OBJ_DRAWN ) {
		//printf( "getpos: obj %X is drawn, %d ", psxobj, psxobj->state );
		list = psxobj->current->next;
		if ( ! list )
			list = psxAllocateExtraHandler( psxobj->current );
		/* copy current position to new handler */
		list->position = psxobj->current->position;
		list->position.flag = 0;
		psxobj->current = list;
        /* update state of object */
        psxobj->state = PSX_OBJ_NOTDRAWN;
		//printf( "not drawn = %d\n", psxobj, psxobj->state );
    } 

    return( &psxobj->current->position );    
}


/**********************************************************
 * Function:	psxAllocateExtraHandler
 **********************************************************
 * Description: Get position data for a PSX Object
 * Inputs:		psxobj - PSX Object
 * Notes:		Used ONLY by scale/translate/rotate functions	
 * Returns:		position pointer
 **********************************************************/

private PsxObjHandleList * psxAllocateExtraHandler (
	PsxObjHandleList *current 
	) 
{
	PsxObjHandleList	*newhandler;
	PsxObjPosition		*newposition;

    /* check for buffer overrun */
    if ( ps.nextrahandlers >= PSX_MAX_EXTRAHANDLERS ) {
		gfxError( GFX_ERROR_EXTRAHANDLERS_OVERRUN );
		gfxAbort();
    }

    /* create and link new handler */
	newhandler  = &ps.tmd.extrahandlers[ps.nextrahandlers];
    newposition = &newhandler->position;
    ps.nextrahandlers++;

	//printf( "new handler %d, current=%X,new=%X\n", ps.nextrahandlers, current, newhandler );

#if 0
	GsInitCoordinate2( WORLD, (GsCOORDINATE2 *)newposition );
    newposition->coord = current->position.coord;
    newposition->flag = 0;
#else
	*newposition = current->position;
    newposition->flag = 0;
#endif

	newhandler->handler.position = newposition;
#if 0
	/* init object handler */
	newhandler->attribute.unused       = 0;
	newhandler->attribute.lightingoff  = ps.lightingoff;
	newhandler->attribute.unused       = 0;
	//newhandler->attribute.subdivision = PSX_SUBDIV_NONE;
	newhandler->attribute.unused2      = 0;
	newhandler->attribute.transparency = PSX_TRANSPARENCY_25;
	newhandler->attribute.displayoff   = 0;
	newhandler->attribute.subdivision  = psxobj->current->attribute.subdivision;
#else
	newhandler->handler.attribute = current->handler.attribute;
#endif
    /* set link flag */
    newhandler->linked = FALSE;
	newhandler->next   = NULL;

	/* update current handler to point to this one as next handler in list */
	current->next = newhandler;

	return( newhandler );
}


/**********************************************************
 * Function:	psxApplyMatrix
 **********************************************************
 * Description: Apply a matrix to a PSX Object
 * Inputs:		gfxobj - Object
 * Notes:		
 * Returns:		
 **********************************************************/

void psxApplyMatrix ( 
	GfxObject	*object, 
	PsxMatrix	*m
	)
{
	PsxMatrix	     mat;
    PsxObject	    *psxobj;
    PsxObjPosition  *position;

    /* init */
    psxobj = (PsxObject *)object->data;
    position = psxGetObjectPosition( psxobj );

    /* multiply matrix */
	MulMatrix( (MATRIX *)&position->coord, (MATRIX *)m );
	position->flag = 0;
}

#endif



