/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	gfxhmd.c
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
 *	psxGetObjectCoord()
 *				Get the coordinate pointer for an object.
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
 *
 **********************************************************
 * Revision History:
 * 07-Dec-99	Theyer	Initial creation from gfxpsx.c
 **********************************************************/

#ifdef PSX_USE_HMD_FORMAT

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
private void        *psxAddPrimitive( GfxObject *gfxobj, PsxObjType type, int num );
private GfxObject   *psxNewObject( Text name, Bool dynamic );
private GsCOORDUNIT *psxGetObjectCoord( PsxHmdObject *psxobj );
private PsxHmdHandleList * psxAllocateExtraHandler( PsxHmdHandleList *current );
private void         psxScale( GsCOORDUNIT *coord, fixed x, fixed y, fixed z );
private void         psxTranslate( GsCOORDUNIT *coord, int x, int y, int z );
private void  	     psxRotate( GsCOORDUNIT *coord, char axis, int angle );
private int			 psxGetHmdDriver( PsxHmdObject *psxobj, PsxObjType type );
private void		 psxPrintMatrix( MATRIX	*mat );

/*
 * variables
 */

extern PsxData ps;

static PsxHmdMeshGourTexLit	*last_mesh = NULL;


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
    ps.hmd.view.vpx = (long)eye->x;
    ps.hmd.view.vpy = (long)-(eye->z);
    ps.hmd.view.vpz = (long)eye->y;

    /* this stupid system needs to look past the view point */
    x = ((int)ref->x - (int)eye->x);
    y = ((int)ref->y - (int)eye->y);
    z = ((int)ref->z - (int)eye->z);
    ps.hmd.view.vrx = (long)(int)ref->x;
    ps.hmd.view.vry = (long)-((int)ref->z);
    ps.hmd.view.vrz = (long)(int)ref->y;

	/* set projection distance */
	x /= 100;
	y /= 100;
	z /= 100;
    dist = mthFastSqrt( gfxAbs(((x * x) + (y * y) + (z * z))) ) * 100;
    GsSetProjection( FIXED_TO_INT((dist * proj))  );

	/* other stuff */
    ps.hmd.view.rz    = 0;
    ps.hmd.view.super = WORLD;
    GsSetRefViewUnit( (GsRVIEWUNIT *)&ps.hmd.view );
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
    PsxHmdObject	*parentobj;

    /* link to parent object or divorce */
    if ( parent != NULL ) {
		parentobj = (PsxHmdObject*)parent->data;
	    ps.hmd.view.super = &parentobj->handle_list.coord;
    } else {
	    ps.hmd.view.super = WORLD;
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
	/* locked screen? */
	if ( ps.screen_lock ) {
		gfxError( GFX_ERROR_SCREEN_LOCKED );
		gfxAbort();
	}

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

		/* set clip region */
		GsSetClip( &ps.split_screen.clip[1] );
		GsSetOffset( ps.split_screen.offsetx[1], ps.split_screen.offsety[1] );

		/* register request to draw ordering table */
		GsDrawOt( &ps.screen.split.worldorder[(ps.drawpage+2)] );
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
    PsxHmdObject	*psxobj;
    PsxHmdObject	*parentobj;
    u_long          *l;
    int				 i;
    int				 n_vert;
    int				 n_norm;
    int				 p_size;

#if GFX_DEBUG
    printf("gfxBeginObject...");
#endif

    /* allocate memory and init */
    gfxobj  = psxNewObject( name, 0 );
    psxobj  = (PsxHmdObject *)gfxobj->data;

	/* init temporary buffer space (uses order table memory) */
	for ( i=0; i<PSX_NUM_TEMP_BUFFERS; i++ )
		ps.screen.temp.size[i] = 0;

	/* lock screen as we are using the temp buffer */
	ps.screen_lock = TRUE;

	/* re-init transparency */
	ps.transparency = FALSE;

    /* init object */
    psxobj->n_vert         = 0;
    psxobj->n_normal       = 0;
    psxobj->n_primitive    = 0;

	n_vert = ps.hmd.n_vert;
    n_norm = ps.hmd.n_normal;
    p_size = ps.hmd.primitive_size;
#if GFX_VERTEX_LOCAL
    psxobj->vert_top       = (u_long *)&(ps.hmd.vertex[n_vert]);
    psxobj->normal_top     = (u_long *)&(ps.hmd.normal[n_norm]);
#else
    psxobj->vert_top       = (u_long *)&(ps.hmd.vertex);
    psxobj->normal_top     = (u_long *)&(ps.hmd.normal);
#endif
    psxobj->primitive_top  = &(ps.hmd.primitive[p_size]);
    psxobj->primitive_size = 0;

	/* init non shared primitive header */
	psxobj->prim_hdr.size            = 3;
	psxobj->prim_hdr.poly_data_ptr   = psxobj->primitive_top;
	psxobj->prim_hdr.vertex_data_ptr = psxobj->vert_top;
	psxobj->prim_hdr.normal_data_ptr = psxobj->normal_top;

	/* init primitive block */
	psxobj->block.next         = (PsxHmdPrimBlock *)0xFFFFFFFF;
	psxobj->block.prim_hdr_ptr = (PsxHmdPrimHdr *)&psxobj->prim_hdr;
	psxobj->block.type_count   = 0;

    /* init coord handle */
    memClear( &psxobj->handle_list.coord, sizeof(GsCOORDUNIT) );
	psxobj->handle_list.coord.matrix.m[0][0] = FIXED_ONE;
	psxobj->handle_list.coord.matrix.m[1][1] = FIXED_ONE;
	psxobj->handle_list.coord.matrix.m[2][2] = FIXED_ONE;
    psxobj->current = &psxobj->handle_list;
	psxobj->handle_list.unit.coord   = &psxobj->handle_list.coord;
	psxobj->handle_list.unit.primtop = (u_long *)&psxobj->block;
	psxobj->handle_list.next = NULL;

    /* link to parent object if necessary */
    if ( parent != NULL ) {
		parentobj = (PsxHmdObject*)parent->data;
        psxobj->handle_list.coord.super = &parentobj->handle_list.coord;
    }
 
	/* skinned model support */
	if ( skinned ) {
		psxobj->shared = (PsxHmdSharedData *)psxAddPrimitive( gfxobj, PSX_HMD_SHARED_DATA, 0 );
		memClear( psxobj->shared, sizeof(PsxHmdSharedData) );
		psxobj->shared->current = GFX_PIVOT_WORLD;
	} else {
		psxobj->shared = NULL;
	}

    /* save the gfx object */
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
	PsxHmdObject			*psxobj;
	int						 i, v;
	int						 size;
	int						 offset;
	int						 npts;
	Point_i3d				 pt;
	Point_fx3d				 fpt;
	PsxHmdSharedPrimHdr		*hdr;
	PsxHmdSharedPrimBlock	*block;
	PsxHmdSharedBlock		*pblock;
	PsxHmdSharedBlock		*sblock;
	PsxVertex				*vert;
	int						 tx, ty, tz;
	GsCOORDUNIT				*coord;

    psxobj  = (PsxHmdObject *)object->data;

	/* move primitives from temporary to prim' buffer */
	offset = 0;
	for ( i=0; i<psxobj->block.type_count; i++ ) {
		size = ps.screen.temp.size[i];
		if ( size > 0 ) {
			/* update primitive buffer size (long units) */
			ps.hmd.primitive_size += (size>>2);
			if ( (ps.hmd.primitive_size<<2) > PSX_PRIMITIVE_BUFFER_SIZE ) {
				gfxError( GFX_ERROR_PRIMITIVE_OVERRUN );
				gfxAbort();
			}
			/* copy memory into buffer */
			memCopy( &ps.screen.temp.data[i], (((Byte *)psxobj->primitive_top) + psxobj->primitive_size), size );
			/* update object info */
			psxobj->primitive_size += size;
			psxobj->n_primitive += psxobj->block.type[i].poly_count;
			psxobj->block.type[i].poly_offset = offset;
			offset += (size>>2);
		}
		//printf( "temp buffer = %d, size = %d, poly count = %d, psize = %d\n", i, size, psxobj->block.type[i].poly_count, (ps.hmd.primitive_size<<2) );
	}

#if 1
	if ( psxobj->shared != NULL ) {
		/* validate points == normals for skinned characters */
		if ( psxobj->n_normal != psxobj->n_vert ) {
			gfxError( GFX_ERROR_HMD_POINTS_NORMALS_NOT_SAME );
			gfxAbort();
		}
		/* init */
		pt.x = pt.y = pt.z = 0;
		fpt.x = fpt.y = fpt.z = 0;
		/* calculate number of shared points used */
		npts = 0;
		for ( i=0; i<psxobj->shared->nblocks; i++ ) {
			npts += psxobj->shared->blocks[i].block.vertex_count;
			//printf( "block=%d, pts=%d, total=%d\n",
			//	(i+1), psxobj->shared->blocks[i].block.vertex_count, npts );
		}
#if GFX_VALIDATE
		/* validate */
		if ( npts > psxobj->n_vert ) {
			printf( "total=%d, n vertex=%d\n", npts, psxobj->n_vert );
			gfxError( GFX_ERROR_HMD_POINTS_MISSING );
			gfxAbort();
		}
#endif
		/* more points? */
		if ( npts < psxobj->n_vert ) {
			/* need another block to pre-calculate rest of points in the model */
			gfxAddPivot( object, pt, GFX_PIVOT_WORLD, (psxobj->n_vert - npts) );
			/* set this block to point to the object matrix */
			psxobj->shared->blocks[(psxobj->shared->nblocks - 1)].handle_list.unit.coord = &psxobj->handle_list.coord;
		}
		/* double points to create calculation buffer */
		npts = psxobj->n_vert;
		/* got enough room? */
		if ( (ps.hmd.n_vert + npts) >= PSX_MAX_VERTEX ) {
			gfxError( GFX_ERROR_VERTEX_OVERRUN );
			gfxAbort();
		}
		/* init vertex memory */
		memClear( &ps.hmd.vertex[ps.hmd.n_vert], sizeof(PsxVertex)*npts );
		ps.hmd.n_vert += npts;		
		/* double normals to create calculation buffer */
		npts = psxobj->n_normal;
		/* got enough room? */
		if ( (ps.hmd.n_normal + npts) >= PSX_MAX_NORMAL ) {
			gfxError( GFX_ERROR_NORMAL_OVERRUN );
			gfxAbort();
		}
		/* init normal memory */
		memClear( &ps.hmd.normal[ps.hmd.n_normal], sizeof(PsxVertex)*npts );
		ps.hmd.n_normal += npts;
		/* get size of shared data and blocks */
		size = ps.screen.temp.size[PSX_TEMP_SHARED_BUFFER];
		/* update primitive buffer size (long units) */
		ps.hmd.primitive_size += (size>>2);
	    if ( (ps.hmd.primitive_size<<2) > PSX_PRIMITIVE_BUFFER_SIZE ) {
			gfxError( GFX_ERROR_PRIMITIVE_OVERRUN );
			gfxAbort();
		}
		/* copy memory into buffer */
		memCopy( &ps.screen.temp.data[PSX_TEMP_SHARED_BUFFER], (((Byte *)psxobj->primitive_top) + psxobj->primitive_size), size );
		/* update */
		psxobj->shared = (PsxHmdSharedData *)(((Byte *)psxobj->primitive_top) + psxobj->primitive_size);
		psxobj->primitive_size += size;
		/* update pointers in shared primitive blocks */
		for ( i=0; i<psxobj->shared->nblocks; i++ ) {
			/* update shared primitive block */
			block  = &psxobj->shared->blocks[i].block;
			//block->ptr_to_next			 = (u_long *)&block->next;
			block->shared_prim_hdr_ptr		 = &psxobj->shared->prim_hdr;
			/* update shared data block */
			sblock = &psxobj->shared->blocks[i];
			if ( sblock->handle_list.unit.coord != &psxobj->handle_list.coord ) {
				sblock->handle_list.unit.coord = &sblock->handle_list.coord;
			}
			sblock->handle_list.unit.primtop = (u_long *)block;	
			sblock->current = &sblock->handle_list;
		    /* link to parent object as necessary */
		    if ( ((int)sblock->parent) == GFX_PIVOT_WORLD ) {
				sblock->handle_list.coord.super = &psxobj->handle_list.coord;
			} else {
#if GFX_VALIDATE
				if ( sblock->parent >= psxobj->shared->nblocks ) {
					gfxError( GFX_ERROR_INVALID_PIVOT_PARENT );
					gfxAbort();
				}
#endif
				pblock = &psxobj->shared->blocks[sblock->parent];
				sblock->handle_list.coord.super = &pblock->handle_list.coord;
			}
	    }
		/* orientate points to joint origin */
		for ( i=0; i<psxobj->shared->nblocks; i++ ) {
			/* init */
			sblock = &psxobj->shared->blocks[i];
			block  = &psxobj->shared->blocks[i].block;
			vert   = (PsxVertex *)psxobj->vert_top;
			/* update points */
			for ( v=block->src_vertex_offset; v<(block->src_vertex_offset + block->vertex_count); v++ ) {
				vert[v].x -= sblock->cx;
				vert[v].y -= sblock->cy;
				vert[v].z -= sblock->cz;
			}
		}
#if 1
		/* init translation */
		for ( i=0; i<psxobj->shared->nblocks; i++ ) {
			/* init */
			sblock = &psxobj->shared->blocks[i];
			coord  = &sblock->current->coord;
			if ( sblock->parent == GFX_PIVOT_WORLD ) {
				coord->matrix.t[0] = sblock->cx;
				coord->matrix.t[1] = sblock->cy;
				coord->matrix.t[2] = sblock->cz;
			} else {
				coord->matrix.t[0] = (sblock->cx - psxobj->shared->blocks[sblock->parent].cx);
				coord->matrix.t[1] = (sblock->cy - psxobj->shared->blocks[sblock->parent].cy);
				coord->matrix.t[2] = (sblock->cz - psxobj->shared->blocks[sblock->parent].cz);
			}
		}
#endif
		/* initialise header */
		hdr = &psxobj->shared->prim_hdr;
		hdr->size					= 5;
		hdr->poly_data_ptr			= psxobj->primitive_top;
		hdr->vertex_data_ptr		= psxobj->vert_top;
		hdr->calc_vertex_data_ptr	= psxobj->vert_top + (psxobj->n_vert<<1);
#if 0
		while(1) {
			for ( i=0; i<psxobj->n_vert; i++ )
				printf( "v  %d=0x%X\n", i, *(hdr->vertex_data_ptr + (i<<1)) );
			for ( i=0; i<psxobj->n_vert; i++ )
				printf( "vc %d=0x%X\n", i, *(hdr->calc_vertex_data_ptr + (i<<1)) );
		}
#endif
		hdr->normal_data_ptr		= psxobj->normal_top;
		hdr->calc_normal_data_ptr	= psxobj->normal_top + (psxobj->n_normal<<1);
		/* update primitive block */
		psxobj->block.prim_hdr_ptr  = (PsxHmdPrimHdr *)&psxobj->shared->prim_hdr;
	}
#endif

	/* unlock screen */
	ps.screen_lock = FALSE;
	/* do a clear do clear temp buffer data */
	psxClear();
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
    PsxHmdObject	*psxobj;
	int				 i;
  
	return;

    psxobj  = (PsxHmdObject *)object->data;
	for ( i=0; i<psxobj->block.type_count; i++ ) {
		psxobj->block.type[i].poly_subdiv = subdiv;
	}
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
    PsxHmdObject		*psxobj;
    PsxHmdObject		*parentobj;

    /* init */ 
    psxobj = (PsxHmdObject *)object->data;

    /* link to parent object or divorce */
    if ( parent != NULL ) {
		parentobj = (PsxHmdObject*)parent->data;
        psxobj->handle_list.coord.super = &parentobj->handle_list.coord;
    } else {
		psxobj->handle_list.coord.super = NULL;
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
 * Notes:	
 * Returns:		void
 **********************************************************/

public int gfxAddPivot ( 
	GfxObject	*object, 
	Point_i3d	 pt, 
	int			 parent, 
	int			 npts 
	)
{
	PsxHmdObject			*psxobj;
    PsxHmdSharedBlock		*block;
    PsxHmdSharedBlock		*pblock;
    PsxHmdSharedPrimBlock	*prim;
	int						 i, cnt;

    /* init */ 
    psxobj = (PsxHmdObject *)object->data;

#if GFX_VALIDATE
	/* validate */
	if ( psxobj->shared == NULL ) {
		gfxError( GFX_ERROR_OBJECT_NOT_SKINNED );
		gfxAbort();
	}
#endif

	if ( psxobj->shared->nblocks > 0 ) {
		/* allocate temporary memory */
		block = (PsxHmdSharedBlock *)psxAddPrimitive( object, PSX_HMD_SHARED_BLOCK, 0 );
		memClear( block, sizeof(PsxHmdSharedBlock) );
	} else {
		/* first block is already part of shared data structure */
		block = &psxobj->shared->blocks[0];
	}

	/* update number of blocks in this skinned object */
	psxobj->shared->nblocks++;

    /* init coord handle */
    memClear( &block->handle_list.coord, sizeof(GsCOORDUNIT) );
	block->handle_list.coord.matrix.m[0][0] = FIXED_ONE;
	block->handle_list.coord.matrix.m[1][1] = FIXED_ONE;
	block->handle_list.coord.matrix.m[2][2] = FIXED_ONE;
	//block->handle_list.coord.matrix.t[0] = pt.x;
	//block->handle_list.coord.matrix.t[1] = -(pt.z);
	//block->handle_list.coord.matrix.t[2] = pt.y;
    block->current = &block->handle_list;							// update in gfxEndObject()
	block->handle_list.unit.coord   = &block->handle_list.coord;	// update in gfxEndObject()
	block->handle_list.unit.primtop = (u_long *)&block->block;		// update in gfxEndObject()
	block->handle_list.next = NULL;
    /* link parent reference later (in gfxEndObject()) */
    block->handle_list.coord.super = (GsCOORDUNIT *)parent;

	/* save joint centre */
	block->cx = pt.x;
	block->cy = -(pt.z);
	block->cz = pt.y;

	/* save parent */
	block->parent = parent;
	//printf( "pivot block %d parent = %d\n", (psxobj->shared->nblocks - 1), parent );

	/* init these primitive block pointers in gfxEndObject() */
	prim = &block->block;
	//prim->ptr_to_next			= (u_long *)0xFFFFFFFF;				// update in gfxEndObject()
	prim->shared_prim_hdr_ptr	= NULL;								// update in gfxEndObject()

	/* init */
	//prim->unused_prim_hdr_ptr	= (PsxHmdPrimHdr *)&psxobj->prim_hdr;;
	//prim->unused_type_count	= 0;
	prim->next					= (u_long *)0xFFFFFFFF;
	prim->type_count			= 1;
	prim->driver				= (u_long *)GsU_01000000;	// vertex and normal calculation HMD driver
	prim->size					= 7;
	prim->poly_count			= 0;
	/* get vertex and normal offset */
	for ( cnt=0, i=0; i<(psxobj->shared->nblocks - 1); i++ )
		cnt += psxobj->shared->blocks[i].block.vertex_count;
	/* init */
	prim->vertex_count			= npts;
	prim->src_vertex_offset		= cnt;
	prim->dst_vertex_offset		= cnt;
	prim->normal_count			= npts;
	prim->src_normal_offset		= cnt;
	prim->dst_normal_offset		= cnt;

	/* return array offset */
	return( (psxobj->shared->nblocks - 1) );
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
    PsxHmdObject	 	*psxobj;
    size_t	   	   	 	 newsize;
    PsxHmdTriFlat  	 	*triflat;
    PsxHmdTriFlatTex 	*triflattex;
	PsxHmdTriGourLit	*trigourlit;
	PsxHmdTriGourTexLit	*trigourtexlit;
    PsxHmdTriFlatLit  	*triflatlit;
    PsxHmdTriFlatTexLit *triflattexlit;
    PsxTexPoint  		*atxy;
    u_char	  			*c;
    short	  			*s;
    int		   			 i;
    u_long	  			*l;
    Point_fx3d	   		 normal;
	u_short				 nindex[2];
	u_short				 vindex[4];

    /* init */
    psxobj = (PsxHmdObject *)object->data;

	last_mesh = NULL;

	/* create primitive */
#if 1	
    if ( ps.textureon ) {
		/* gouraud shaded, textured with lighting */
/*
		Gouraud Texture Triangle
		0x0000000d; DRV(0)|PRIM_TYPE(TRI|IIP|TME); GsUGT3
		B(u0); B(v0); H(cba);        
		B(u1); B(v1); H(tsb);
		B(u2); B(v2); H(0);        
		H(norm0); H(vert0);
		H(norm1); H(vert1);        
		H(norm2); H(vert2);
*/
		atxy = psxAdjustTexturePoints( txy, 3 );
    	trigourtexlit = (PsxHmdTriGourTexLit *)psxAddPrimitive( object, PSX_HMD_TRI_GOURAUD_TEX_LIT, 0 );
		/* add triangle data */
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
		if ( normals != NULL ) {
    		trigourtexlit->n0 = normals[0];
    		trigourtexlit->n1 = normals[1];
    		trigourtexlit->n2 = normals[2];
		} else {
			/* add normal */
			normal = triCalcNormal( pts );
   			psxAddNormal( object, &normal, 1, nindex );
			trigourtexlit->n0 = nindex[0];
			trigourtexlit->n1 = nindex[0];
			trigourtexlit->n2 = nindex[0];
		}
#if GFX_DEBUG
    	l = (u_long *)trigourtexlit;
    	printf("tri (goraud tex lit) 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3), *(l+4), *(l+5) );
#endif
		/* add double sided triangle if necessary */
        if ( ps.doublesided ) {    	
    		trigourtexlit = (PsxHmdTriGourTexLit *)psxAddPrimitive( object, PSX_HMD_TRI_GOURAUD_TEX_LIT, 0 );
        	trigourtexlit->tx0    = atxy[0].x;
        	trigourtexlit->ty0    = atxy[0].y;
        	trigourtexlit->tx1    = atxy[2].x;
        	trigourtexlit->ty1    = atxy[2].y;
	    	trigourtexlit->tx2    = atxy[1].x;
	    	trigourtexlit->ty2    = atxy[1].y;
	    	trigourtexlit->unused = 0;
           	/* add texture info */
  	    	psxSetTextureInfo( &trigourtexlit->texture, &trigourtexlit->clut );
            /* add reversed points */
			trigourtexlit->v0 = vindex[0];
    		trigourtexlit->v1 = vindex[2];
    		trigourtexlit->v2 = vindex[1];
			if ( normals != NULL ) {
    			trigourtexlit->n0 = normals[0];
    			trigourtexlit->n1 = normals[2];
    			trigourtexlit->n2 = normals[1];
			} else {
				trigourtexlit->n0 = nindex[0];
				trigourtexlit->n1 = nindex[0];
				trigourtexlit->n2 = nindex[0];
			}
        }
	} else {
		/* goraud shaded, no texture with lighting */
/*
		Gouraud No-Texture Triangle
		0x0000000c; DRV(0)|PRIM_TYPE(TRI|IIP); GsUG3
		B(r); B(g); B(b); B(0x30);        
		H(norm0); H(vert0);
		H(norm1); H(vert1);        
		H(norm2); H(vert2);
*/
		trigourlit = (PsxHmdTriGourLit *)psxAddPrimitive( object, PSX_HMD_TRI_GOURAUD_LIT, 0 );
		/* add triangle data */
		trigourlit->red   = ps.color.red;
		trigourlit->green = ps.color.green;
		trigourlit->blue  = ps.color.blue;
		trigourlit->mode  = 0x30;
		/* add vertex and normal data */
		psxAddVertex( object, pts, 3, vindex );
		trigourlit->v0 = vindex[0];
		trigourlit->v1 = vindex[1];
		trigourlit->v2 = vindex[2];
		if ( normals != NULL ) {
			trigourlit->n0 = normals[0];
			trigourlit->n1 = normals[1];
			trigourlit->n2 = normals[2];
		} else {
			/* add normal */
			normal = triCalcNormal( pts );
   			psxAddNormal( object, &normal, 1, nindex );
			trigourlit->n0 = nindex[0];
			trigourlit->n1 = nindex[0];
			trigourlit->n2 = nindex[0];
		}
#if GFX_DEBUG
		l = (u_long *)trigourlit;
		printf("tri (gouraud lit) 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3) );
#endif
		/* add double sided triangle if necessary */
		if ( ps.doublesided ) {
 			trigourlit = (PsxHmdTriGourLit *)psxAddPrimitive( object, PSX_HMD_TRI_GOURAUD_LIT, 0 );
			/* add triangle data */
			trigourlit->red   = ps.color.red;
    		trigourlit->green = ps.color.green;
    		trigourlit->blue  = ps.color.blue;
			trigourlit->mode  = 0x30;
    		/* add reversed vertex data */
			if ( normals != NULL ) {
				trigourlit->n0 = normals[0];
				trigourlit->n1 = normals[2];
				trigourlit->n2 = normals[1];
			} else {
				trigourlit->n0 = nindex[0];
				trigourlit->n1 = nindex[0];
				trigourlit->n2 = nindex[0];
			}
			trigourlit->v0 = vindex[0];
    		trigourlit->v1 = vindex[2];
    		trigourlit->v2 = vindex[1];
		}
	}
#else
	/* create primitive */
	if ( ps.lightingoff ) {
		/* no lighting */
    	if ( ps.textureon ) {
    		/* flat shaded, textured, no lighting */
/*
			Flat Texture Triangle
		    0x00040049; DRV(LGT)|PRIM_TYPE(LMD|TRI|TME); GsUNFT3
			B(r); B(g); B(b); B(0x24);        
			B(u0); B(v0); H(cba);
			B(u1); B(v1); H(tsb);        
			B(u2); B(v2); H(vert0);
			H(vert1); H(vert2);
*/
			atxy = psxAdjustTexturePoints( txy, 3 );
    		triflattex = (PsxHmdTriFlatTex *)psxAddPrimitive( object, PSX_HMD_TRI_FLAT_TEX, 0 );
			/* add triangle data */			
			triflattex->red   = ps.color.red;
    		triflattex->green = ps.color.green;
    		triflattex->blue  = ps.color.blue;
			triflattex->mode  = 0x24;
        	triflattex->tx0   = atxy[0].x;
        	triflattex->ty0   = atxy[0].y;
        	triflattex->tx1   = atxy[1].x;
        	triflattex->ty1   = atxy[1].y;
			triflattex->tx2   = atxy[2].x;
			triflattex->ty2   = atxy[2].y;
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
    		    triflattex = (PsxHmdTriFlatTex *)psxAddPrimitive( object, PSX_HMD_TRI_FLAT_TEX, 0 );
				triflattex->red   = ps.color.red;
				triflattex->green = ps.color.green;
    			triflattex->blue  = ps.color.blue;
				triflattex->mode  = 0x24;
        	    triflattex->tx0   = atxy[0].x;
        	    triflattex->ty0   = atxy[0].y;
        	    triflattex->tx1   = atxy[2].x;
        	    triflattex->ty1   = atxy[2].y;
	    		triflattex->tx2   = atxy[1].x;
	    		triflattex->ty2   = atxy[1].y;
           		/* add texture info */
  	    		psxSetTextureInfo( &triflattex->texture, &triflattex->clut );
            	/* add reversed points */
    			triflattex->v0 = vindex[0];
    			triflattex->v1 = vindex[2];
    			triflattex->v2 = vindex[1];
        	}
    	} else {
    		/* flat, no texture, no lighting */
/*
	        Flat No-Texture Triangle
		    0x00040048; DRV(LGT)|PRIM_TYPE(LMD|TRI); GsUNF3
			B(r); B(g); B(b); B(0x20);        
			H(vert0); H(vert1);
			H(vert2); H(0);
*/
    		triflat = (PsxHmdTriFlat *)psxAddPrimitive( object, PSX_HMD_TRI_FLAT, 0 );
			/* add triangle data */
			triflat->red   = ps.color.red;
    		triflat->green = ps.color.green;
    		triflat->blue  = ps.color.blue;
    		triflat->mode  = 0x20;
    		/* add vertex data */
    		psxAddVertex( object, pts, 3, vindex );
    		triflat->v0 = vindex[0];
    		triflat->v1 = vindex[1];
    		triflat->v2 = vindex[2];
			triflat->unused = 0;
#if GFX_DEBUG
    		l = (u_long *)triflat;
    		printf("tri (flat not lit) 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3) );
#endif
    		/* add double sided triangle if necessary */
        	if ( ps.doublesided ) {
 	    		triflat = (PsxHmdTriFlat *)psxAddPrimitive( object, PSX_HMD_TRI_FLAT, 0 );
	    		/* add triangle data */
				triflat->red   = ps.color.red;
				triflat->green = ps.color.green;
    			triflat->blue  = ps.color.blue;
    			triflat->mode  = 0x20;
    		    /* add reversed vertex data */ 
    			triflat->v0 = vindex[0];
    			triflat->v1 = vindex[2];
    			triflat->v2 = vindex[1];
				triflat->unused = 0;
        	}
		}
	} else {
		/* lighting supported */
    	if ( ps.textureon ) {
			if ( normals != NULL ) {
				/* gouraud shaded, textured with lighting */
/*
				Gouraud Texture Triangle
				0x0000000d; DRV(0)|PRIM_TYPE(TRI|IIP|TME); GsUGT3
				B(u0); B(v0); H(cba);        
				B(u1); B(v1); H(tsb);
				B(u2); B(v2); H(0);        
				H(norm0); H(vert0);
				H(norm1); H(vert1);        
				H(norm2); H(vert2);
*/
				atxy = psxAdjustTexturePoints( txy, 3 );
    			trigourtexlit = (PsxHmdTriGourTexLit *)psxAddPrimitive( object, PSX_HMD_TRI_GOURAUD_TEX_LIT, 0 );
				/* add triangle data */
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
    				trigourtexlit = (PsxHmdTriGourTexLit *)psxAddPrimitive( object, PSX_HMD_TRI_GOURAUD_TEX_LIT, 0 );
        			trigourtexlit->tx0    = atxy[0].x;
        			trigourtexlit->ty0    = atxy[0].y;
        			trigourtexlit->tx1    = atxy[2].x;
        			trigourtexlit->ty1    = atxy[2].y;
	    			trigourtexlit->tx2    = atxy[1].x;
	    			trigourtexlit->ty2    = atxy[1].y;
	    			trigourtexlit->unused = 0;
           			/* add texture info */
  	    			psxSetTextureInfo( &trigourtexlit->texture, &trigourtexlit->clut );
            		/* add reversed points */
	    			trigourtexlit->n0 = normals[0];
					trigourtexlit->n1 = normals[2];
    				trigourtexlit->n2 = normals[1];
					trigourtexlit->v0 = vindex[0];
    				trigourtexlit->v1 = vindex[2];
    				trigourtexlit->v2 = vindex[1];
        		}		
			} else {
				/* flat shaded, textured with lighting */
/*
				Flat Texture Triangle
				0x00000009; DRV(0)|PRIM_TYPE(TRI|TME); GsUFT3
				B(u0); B(v0); H(cba);        
				B(u1); B(v1); H(tsb);
				B(u2); B(v2); H(0);        
				H(norm0); H(vert0);
				H(vert1); H(vert2);
*/
				atxy = psxAdjustTexturePoints( txy, 3 );
    			triflattexlit = (PsxHmdTriFlatTexLit *)psxAddPrimitive( object, PSX_HMD_TRI_FLAT_TEX_LIT, 0 );
				/* add triangle data */
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
    				triflattexlit = (PsxHmdTriFlatTexLit *)psxAddPrimitive( object, PSX_HMD_TRI_FLAT_TEX_LIT, 0 );
        			triflattexlit->tx0    = atxy[0].x;
        			triflattexlit->ty0    = atxy[0].y;
        			triflattexlit->tx1    = atxy[2].x;
        			triflattexlit->ty1    = atxy[2].y;
	    			triflattexlit->tx2    = atxy[1].x;
	    			triflattexlit->ty2    = atxy[1].y;
	    			triflattexlit->unused = 0;
           			/* add texture info */
  	    			psxSetTextureInfo( &triflattexlit->texture, &triflattexlit->clut );
            		/* add reversed points */
            		triflattexlit->n0 = nindex[0];
					triflattexlit->v0 = vindex[0];
    				triflattexlit->v1 = vindex[2];
    				triflattexlit->v2 = vindex[1];
        		}		
			}
    	} else {
			if ( normals != NULL ) {
    			/* goraud shaded, no texture with lighting */
/*
				Gouraud No-Texture Triangle
				0x0000000c; DRV(0)|PRIM_TYPE(TRI|IIP); GsUG3
				B(r); B(g); B(b); B(0x30);        
				H(norm0); H(vert0);
				H(norm1); H(vert1);        
				H(norm2); H(vert2);
*/
    			trigourlit = (PsxHmdTriGourLit *)psxAddPrimitive( object, PSX_HMD_TRI_GOURAUD_LIT, 0 );
				/* add triangle data */
				trigourlit->red   = ps.color.red;
    			trigourlit->green = ps.color.green;
    			trigourlit->blue  = ps.color.blue;
    			trigourlit->mode  = 0x30;
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
 	    			trigourlit = (PsxHmdTriGourLit *)psxAddPrimitive( object, PSX_HMD_TRI_GOURAUD_LIT, 0 );
	    			/* add triangle data */
	    			trigourlit->red   = ps.color.red;
    				trigourlit->green = ps.color.green;
    				trigourlit->blue  = ps.color.blue;
	    			trigourlit->mode  = 0x30;
    				/* add reversed vertex data */ 
					trigourlit->n0 = normals[0];
					trigourlit->n1 = normals[2];
					trigourlit->n2 = normals[1];
					trigourlit->v0 = vindex[0];
    				trigourlit->v1 = vindex[2];
    				trigourlit->v2 = vindex[1];
				}
			} else {
    			/* flat, no texture with lighting */
/*
		        Flat No-Texture Triangle
			    0x00000008; DRV(0)|PRIM_TYPE(TRI); GsUF3
				B(r); B(g); B(b); B(0x20);	
				H(norm0); H(vert0);
			    H(vert1); H(vert2);
*/
				triflatlit = (PsxHmdTriFlatLit *)psxAddPrimitive( object, PSX_HMD_TRI_FLAT_LIT, 0 );
				/* add triangle data */
				triflatlit->red   = ps.color.red;
    			triflatlit->green = ps.color.green;
    			triflatlit->blue  = ps.color.blue;
    			triflatlit->mode  = 0x20;
				/* add normal */
				normal = triCalcNormal( pts );
    			psxAddNormal( object, &normal, 1, nindex );
				triflatlit->n0 = nindex[0];
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
					triflatlit = (PsxHmdTriFlatLit *)psxAddPrimitive( object, PSX_HMD_TRI_FLAT_LIT, 0 );
					/* add triangle data */
					triflatlit->red   = ps.color.red;
    				triflatlit->green = ps.color.green;
    				triflatlit->blue  = ps.color.blue;
    				triflatlit->mode  = 0x20;
    				/* add reversed vertex data */ 
        			triflatlit->n0 = nindex[0];
					triflatlit->v0 = vindex[0];
    				triflatlit->v1 = vindex[2];
    				triflatlit->v2 = vindex[1];
				}
			}
		}
    }
#endif
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
#if 1
	Point_i3d	 s_pts[3];
	Point_fx2d	 s_txy[3];
	int			 s_normals[3];

	/* support triangles only at this stage */
	gfxAddTri( object, pts, txy, normals );
	s_pts[0] = pts[2];
	s_pts[1] = pts[1];
	s_pts[2] = pts[3];
	s_txy[0] = txy[2];
	s_txy[1] = txy[1];
	s_txy[2] = txy[3];
	if ( normals != NULL ) {
		s_normals[0] = normals[2];
		s_normals[1] = normals[1];
		s_normals[2] = normals[3];
		gfxAddTri( object, s_pts, s_txy, s_normals );
	} else {
		gfxAddTri( object, s_pts, s_txy, NULL );
	}
#else
    PsxHmdObject	 			*psxobj;
    size_t	   	   	 			 newsize;
	PsxHmdMeshGourTexLit		*meshgourtexlit, *saved;
	PsxHmdMeshGourTexLitEntry	*meshgtlentry;
    PsxTexPoint  				*atxy;
    u_char	  					*c;
    short	  					*s;
    int		   					 i;
    u_long	  					*l;
    Point_fx3d	   				 normal;
	u_short						 nindex[2];
	u_short						 vindex[4];

	/* mesh optimisation */
	static Point_i3d			 last_pts[2] = { { 0,0,0 }, { 0,0,0 } };
    static PsxHmdObject	 		*last_psxobj = NULL;

    /* init */
    psxobj = (PsxHmdObject *)object->data;

#if 0
	/* add to mesh? */
	if ( (psxobj == last_psxobj) && (last_mesh != NULL) ) {
		/* last quad and this quad points match up? */
		if (((last_pts[0].x == pts[0].x) && (last_pts[0].y == pts[0].y) && (last_pts[0].z == pts[0].z)) &&
			((last_pts[1].x == pts[1].x) && (last_pts[1].y == pts[1].y) && (last_pts[1].z == pts[1].z))) {
			/* init */
			atxy = psxAdjustTexturePoints( txy, 4 );
			/* add first quad triangle to mesh */
		    meshgtlentry = (PsxHmdMeshGourTexLitEntry *)psxAddPrimitive( object, PSX_HMD_MESH_GOURAUD_TEX_LIT_ENTRY, 1 );
			/* add next vertex mesh part */
			meshgtlentry->tx0 = atxy[0].x;
			meshgtlentry->ty0 = atxy[0].y;
			meshgtlentry->tx1 = atxy[1].x;
			meshgtlentry->ty1 = atxy[1].y;
			meshgtlentry->tx2 = atxy[2].x;
			meshgtlentry->ty2 = atxy[2].y;
			/* add texture info */
			psxSetTextureInfo( &meshgtlentry->texture, &meshgtlentry->clut );
			meshgtlentry->unused0 = 0;
			/* add vertex */
		    psxAddVertex( object, pts, 4, vindex );
			meshgtlentry->v0 = vindex[2];
			/* add normals */
			if ( normals != NULL ) {
    			meshgtlentry->n0 = normals[0];
    			meshgtlentry->n1 = normals[1];
    			meshgtlentry->n2 = normals[2];
			} else {
				/* add normal */
				normal = triCalcNormal( pts );
   				psxAddNormal( object, &normal, 1, nindex );
    			meshgtlentry->n0 = nindex[0];
    			meshgtlentry->n1 = nindex[0];
    			meshgtlentry->n2 = nindex[0];
			}
			/* add second quad triangle to mesh */
		    meshgtlentry = (PsxHmdMeshGourTexLitEntry *)psxAddPrimitive( object, PSX_HMD_MESH_GOURAUD_TEX_LIT_ENTRY, 1 );
			/* add next vertex mesh part */
			meshgtlentry->tx0 = atxy[1].x;
			meshgtlentry->ty0 = atxy[1].y;
			meshgtlentry->tx1 = atxy[2].x;
			meshgtlentry->ty1 = atxy[2].y;
			meshgtlentry->tx2 = atxy[3].x;
			meshgtlentry->ty2 = atxy[3].y;
			/* add texture info */
			psxSetTextureInfo( &meshgtlentry->texture, &meshgtlentry->clut );
			meshgtlentry->unused0 = 0;
			/* add vertex */
			meshgtlentry->v0 = vindex[3];
			/* add normals */
			if ( normals != NULL ) {
    			meshgtlentry->n0 = normals[1];
    			meshgtlentry->n1 = normals[2];
    			meshgtlentry->n2 = normals[3];
			} else {
				/* add normal */
    			meshgtlentry->n0 = nindex[0];
    			meshgtlentry->n1 = nindex[0];
    			meshgtlentry->n2 = nindex[0];
			}
			/* update triangle count */
			last_mesh->ntri += 2;
			/* save points */
			last_pts[0] = pts[2];
			last_pts[1] = pts[3];
			//printf( "add to mesh\n" );
			return;
		}
	}
#endif

	/* gouraud shaded, textured with lighting */
/*		
	Gouraud Texture Mesh
	0x0000001d; DRV(0)|PRIM_TYPE(MESH|IIP|TME); GsUMGT3
	H(num); H(0);
	B(u0); B(v0); H(cba);
	B(u1); B(v1); H(tsb);
	B(u2); B(v2); H(0);
	H(norm0); H(vert0);
	H(norm1); H(vert1);
	H(norm2); H(vert2);
	* * *
	B(u1a); B(v1a); H(cba);
	B(u2a); B(v2a); H(tsb);
	B(u3); B(v3); H(0);
	H(norm1); H(norm2);
	H(norm3); H(vert3);
*/
	//printf( "add mesh\n" );
	atxy = psxAdjustTexturePoints( txy, 4 );
    meshgourtexlit = (PsxHmdMeshGourTexLit *)psxAddPrimitive( object, PSX_HMD_MESH_GOURAUD_TEX_LIT, 1 );
	/* add mesh data */
    meshgourtexlit->ntri	= 2;
    meshgourtexlit->unused0	= 0;
    meshgourtexlit->tx0     = atxy[0].x;
    meshgourtexlit->ty0     = atxy[0].y;
    meshgourtexlit->tx1     = atxy[1].x;
    meshgourtexlit->ty1     = atxy[1].y;
	meshgourtexlit->tx2     = atxy[2].x;
	meshgourtexlit->ty2     = atxy[2].y;
	meshgourtexlit->unused1 = 0;
    /* add texture info */
    psxSetTextureInfo( &meshgourtexlit->texture, &meshgourtexlit->clut );
    /* add vertex and normal data */
    psxAddVertex( object, pts, 4, vindex );
    meshgourtexlit->v0 = vindex[0];
    meshgourtexlit->v1 = vindex[1];
    meshgourtexlit->v2 = vindex[2];
	if ( normals != NULL ) {
    	meshgourtexlit->n0 = normals[0];
    	meshgourtexlit->n1 = normals[1];
    	meshgourtexlit->n2 = normals[2];
	} else {
		/* add normal */
		normal = triCalcNormal( pts );
   		psxAddNormal( object, &normal, 1, nindex );
    	meshgourtexlit->n0 = nindex[0];
    	meshgourtexlit->n1 = nindex[0];
    	meshgourtexlit->n2 = nindex[0];
	}
	/* add next vertex mesh part */
    meshgtlentry = meshgourtexlit->next;
    meshgtlentry->tx0 = atxy[1].x;
    meshgtlentry->ty0 = atxy[1].y;
    meshgtlentry->tx1 = atxy[2].x;
    meshgtlentry->ty1 = atxy[2].y;
	meshgtlentry->tx2 = atxy[3].x;
	meshgtlentry->ty2 = atxy[3].y;
    /* add texture info */
    psxSetTextureInfo( &meshgtlentry->texture, &meshgtlentry->clut );
	meshgtlentry->unused0 = 0;
	/* add vertex */
    meshgtlentry->v0 = vindex[3];
	/* add normals */
	if ( normals != NULL ) {
    	meshgtlentry->n0 = normals[1];
    	meshgtlentry->n1 = normals[2];
    	meshgtlentry->n2 = normals[3];
	} else {
		/* add normal */
    	meshgtlentry->n0 = nindex[0];
    	meshgtlentry->n1 = nindex[0];
    	meshgtlentry->n2 = nindex[0];
	}
#if GFX_DEBUG
    l = (u_long *)meshgourtexlit;
    printf("mesh (goraud tex lit) 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n", *l, *(l+1), *(l+2), *(l+3), *(l+4), *(l+5) );
#endif
	/* add double sided mesh if necessary */
    if ( ps.doublesided ) { 
		saved = meshgourtexlit; 
	    meshgourtexlit = (PsxHmdMeshGourTexLit *)psxAddPrimitive( object, PSX_HMD_MESH_GOURAUD_TEX_LIT, 1 );
		memCopy( saved, meshgourtexlit, sizeof(meshgourtexlit) );
	    meshgourtexlit->tx1 = atxy[2].x;
	    meshgourtexlit->ty1 = atxy[2].y;
		meshgourtexlit->tx2 = atxy[1].x;
		meshgourtexlit->ty2 = atxy[1].y;
		meshgourtexlit->v1  = vindex[2];
		meshgourtexlit->v2  = vindex[1];
		if ( normals != NULL ) {
    		meshgourtexlit->n1 = normals[2];
    		meshgourtexlit->n2 = normals[1];
		}
		meshgtlentry = meshgourtexlit->next;
		meshgtlentry->tx0 = atxy[1].x;
		meshgtlentry->ty0 = atxy[1].y;
		meshgtlentry->tx1 = atxy[2].x;
		meshgtlentry->ty1 = atxy[2].y;
		if ( normals != NULL ) {
			meshgtlentry->n0 = normals[1];
			meshgtlentry->n1 = normals[2];
		}
    }		

	/* save */
	if ( ps.doublesided ) {
		last_psxobj = NULL;
		last_mesh   = NULL;
	} else {
		last_psxobj = psxobj;
		last_mesh   = meshgourtexlit;
		last_pts[0] = pts[2];
		last_pts[1] = pts[3];
	}
#endif
}


/*******************************************************/
#if 0
#if 1
	Point_i3d	 s_pts[3];
	Point_fx2d	 s_txy[3];
	int			 s_normals[3];

	/* support triangles only at this stage */
	gfxAddTri( object, pts, txy, normals );
	s_pts[0] = pts[2];
	s_pts[1] = pts[1];
	s_pts[2] = pts[3];
	s_txy[0] = txy[2];
	s_txy[1] = txy[1];
	s_txy[2] = txy[3];
	if ( normals != NULL ) {
		s_normals[0] = normals[2];
		s_normals[1] = normals[1];
		s_normals[2] = normals[3];
		gfxAddTri( object, s_pts, s_txy, s_normals );
	} else {
		gfxAddTri( object, s_pts, s_txy, NULL );
	}
#else
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
#endif
#endif
/*******************************************************/
//}


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
    PsxHmdObject	*psxobj;
    PsxMatrix		 lmat, smat;
	int				 i;
	u_long			*block;

	/* validate lock screen */
	if ( ps.screen_lock ) return;

#if GFX_DEBUG
	printf( "gfxDrawObject..." );
#endif

	/* init */
    psxobj  = (PsxHmdObject *)object->data;

    /* update state flag */
    psxobj->state = PSX_OBJ_DRAWN;

	if ( psxobj->shared ) {
		for ( i=0; i<psxobj->shared->nblocks; i++ ) {
		    /* get the local screen matrix for the object */
			GsGetLwsUnit( (GsCOORDUNIT *)&psxobj->shared->blocks[i].current->coord, (MATRIX *)&lmat, (MATRIX *)&smat );
			/* set the matrix */
			GsSetLightMatrix( (MATRIX *)&lmat );
			GsSetLsMatrix( (MATRIX *)&smat );
			/* add to order table */
			if ( ps.split_screen.on ) {
				GsSortUnit( (GsUNIT *)&psxobj->shared->blocks[i].current->unit, ((GsOT *)&ps.screen.split.worldorder[(ps.drawpage + ps.split_screen.active_buffer)]), getScratchAddr(0) );
			} else {
				GsSortUnit( (GsUNIT *)&psxobj->shared->blocks[i].current->unit, (GsOT *)&ps.screen.full.worldorder[ps.drawpage], getScratchAddr(0) );
			}
		}
	}

    /* get the local screen matrix for the object */
	GsGetLwsUnit( (GsCOORDUNIT *)&psxobj->current->coord, (MATRIX *)&lmat, (MATRIX *)&smat );

    /* set the matrix */
    GsSetLightMatrix( (MATRIX *)&lmat );
    GsSetLsMatrix( (MATRIX *)&smat );

	/* add to order table */
	if ( ps.split_screen.on ) {
		GsSortUnit( (GsUNIT *)&psxobj->current->unit, ((GsOT *)&ps.screen.split.worldorder[(ps.drawpage + ps.split_screen.active_buffer)]), getScratchAddr(0) );
		//GsSortObject4( ((GsDOBJ2 *)handler), ((GsOT *)&ps.screen.split.worldorder[(ps.drawpage + ps.split_screen.active_buffer)]),
		//			(14 - ((PSX_OT_LENGTH - 1) - ps.depth_step)),
		//			(u_long *)getScratchAddr(0) );
	} else {
		//printf( "sort unit %X %X type count=%d, drv=%X", (u_long *)psxobj->current, (u_long *)&psxobj->handle_list, psxobj->block.type_count, psxobj->block.type[0].driver );
		//printf( "poly count=%d\n", psxobj->block.type[0].poly_count );
		//psxPrintMatrix( &psxobj->current->unit.coord->matrix );
		GsSortUnit( (GsUNIT *)&psxobj->current->unit, (GsOT *)&ps.screen.full.worldorder[ps.drawpage], getScratchAddr(0) );
		//GsSortObject4( (GsDOBJ2 *)handler, (GsOT *)&ps.screen.full.worldorder[ps.drawpage],
		//			(14 - (PSX_OT_LENGTH - ps.depth_step)),
		//			(u_long *)getScratchAddr(0) );
		//printf( "\n" );
	}

#if GFX_DEBUG
	printf( "done\n" );
#endif

}

#if 0
private void psxPrintMatrix (
	MATRIX	*mat
	)
{
	printf( "x = %d, %d, %d\n", mat->m[0][0], mat->m[0][1], mat->m[0][2] );
	printf( "y = %d, %d, %d\n", mat->m[1][0], mat->m[1][1], mat->m[1][2] );
	printf( "z = %d, %d, %d\n", mat->m[2][0], mat->m[2][1], mat->m[2][2] );
	printf( "t = %d, %d, %d\n", mat->t[0], mat->t[1], mat->t[2] );
}
#endif

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
    PsxHmdObject	*psxobj;
	GsCOORDUNIT		*coord;

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
    psxobj = (PsxHmdObject *)object->data;
    coord  = psxGetObjectCoord( psxobj );
	
    psxRotate( coord, axis, angle );
};    


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
    PsxHmdObject    *psxobj;
	GsCOORDUNIT		*coord;

    /* init */
    psxobj = (PsxHmdObject *)object->data;
    coord  = psxGetObjectCoord( psxobj );

    psxTranslate( coord, x, y, z );
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
    PsxHmdObject	*psxobj;
	GsCOORDUNIT		*coord;

#if GFX_VALIDATE
	/* matrix will crap out at scale > 8 */
	if (( x == 0 || gfxAbs(x) > 32768 ) ||
		( y == 0 || gfxAbs(y) > 32768 ) ||
		( z == 0 || gfxAbs(z) > 32768 )) {
		gfxError( GFX_ERROR_BAD_SCALE );
		gfxAbort();
	}
#endif

    /* init */
    psxobj = (PsxHmdObject *)object->data;
    coord  = psxGetObjectCoord( psxobj );

    psxScale( coord, x, y, z );
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
    GfxMatrix		 gfxmat;
    PsxHmdObject	*psxobj;
	GsCOORDUNIT		*coord;

#if GFX_VALIDATE
	if ( object == NULL ) {
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}
#endif

    /* init */
    psxobj = (PsxHmdObject *)object->data;
    coord  = psxGetObjectCoord( psxobj );

	memCopy( &coord->matrix, &gfxmat, sizeof(PsxMatrix) );

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
    PsxHmdObject    *psxobj;

    /* init */
    psxobj = (PsxHmdObject *)object->data;

	/* set */
	if ( psxobj->shared ) {
		if ( pivot > GFX_PIVOT_WORLD && pivot < psxobj->shared->nblocks ) 
			psxobj->shared->current = pivot;
		else
			psxobj->shared->current = GFX_PIVOT_WORLD;
	}
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
    PsxHmdObject    *psxobj;
	GsCOORDUNIT		*coord;

    /* init */
    psxobj = (PsxHmdObject *)object->data;
    coord  = psxGetObjectCoord( psxobj );

    /* store matrix to object */
	memCopy( gfxmat, &coord->matrix, sizeof(PsxMatrix) );
	coord->flg = 0;
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
    PsxHmdObject *psxobj;
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
    psxobj = (PsxHmdObject *)object->data;

#if GFX_VALIDATE
	if ( v >= psxobj->n_vert ) {
		gfxError( GFX_ERROR_POINT_OUT_OF_RANGE );
		gfxAbort();
	}
#endif
   
	/* get vertex */
	vert = (PsxVertex *)psxobj->vert_top;
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
    PsxHmdObject *psxobj;

#if GFX_VALIDATE
	if ( object == NULL ) {
		gfxError( GFX_ERROR_NULL_OBJECT );
		gfxAbort();
	}
#endif

    /* init */
    psxobj = (PsxHmdObject *)object->data;

	return( (int)psxobj->n_vert );
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
    PsxHmdObject *psxobj;
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
    psxobj = (PsxHmdObject *)object->data;

#if GFX_VALIDATE
	if ( v >= psxobj->n_vert ) {
		gfxError( GFX_ERROR_POINT_OUT_OF_RANGE );
		gfxAbort();
	}
#endif
   
	/* set vertex */
	vert = (PsxVertex *)psxobj->vert_top;
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
    PsxHmdObject *psxobj;
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
    psxobj = (PsxHmdObject *)object->data;

#if GFX_VALIDATE
	if ( n >= psxobj->n_normal ) {
		gfxError( GFX_ERROR_NORMAL_OUT_OF_RANGE );
		gfxAbort();
	}
#endif
   
	/* set vertex */
	norm = (PsxVertex *)psxobj->normal_top;
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
    	//ps.hmd.gfxobj[i].dynamic = FALSE;
		ps.hmd.gfxobj[i].id   = i;
        ps.hmd.gfxobj[i].data = &ps.hmd.psxobj[i];
    }

    ps.hmd.primitive_size = 0;
    ps.hmd.nobjects       = 0;
    ps.hmd.n_vert	      = 0;
    ps.hmd.n_normal	      = 0;
	
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
	} else {
		GsClearOt( 0, 0, (GsOT *)&ps.screen.full.worldorder[i] );
	}

    /* update object state */
    for ( i=0; i<ps.hmd.nobjects; i++ ) {
    	ps.hmd.psxobj[i].state = PSX_OBJ_NOTDRAWN;
        ps.hmd.psxobj[i].current = &ps.hmd.psxobj[i].handle_list;
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
    PsxObjType 	 type,
	int			 num
    )
{
    int 	  	  size;
    int		  	  newsize;
    void	 	 *prim;
    PsxHmdObject *psxobj;
    void		 *data;
	int			  i;

    /* init */
    psxobj = (PsxHmdObject *)gfxobj->data;

    /* work out the size of the primitive we are adding */
    switch( type ) {
    case PSX_HMD_TRI_FLAT:
    	size = sizeof(PsxHmdTriFlat);
        break;       
    case PSX_HMD_TRI_FLAT_LIT:
    	size = sizeof(PsxHmdTriFlatLit);
        break;         
    case PSX_HMD_TRI_FLAT_TEX:
    	size = sizeof(PsxHmdTriFlatTex);
        break;        
    case PSX_HMD_TRI_FLAT_TEX_LIT:
    	size = sizeof(PsxHmdTriFlatTexLit);
        break;        
    case PSX_HMD_TRI_GOURAUD_LIT:
    	size = sizeof(PsxHmdTriGourLit);
        break;
    case PSX_HMD_TRI_GOURAUD_TEX_LIT:
    	size = sizeof(PsxHmdTriGourTexLit);
        break;
    case PSX_HMD_QUAD_FLAT:
    	size = sizeof(PsxHmdQuadFlat);
        break;          
    case PSX_HMD_QUAD_FLAT_TEX:
    	size = sizeof(PsxHmdQuadFlatTex);
        break;
    case PSX_HMD_QUAD_FLAT_LIT:
    	size = sizeof(PsxHmdQuadFlatLit);
        break;          
    case PSX_HMD_QUAD_FLAT_TEX_LIT:
    	size = sizeof(PsxHmdQuadFlatTexLit);
        break;
    case PSX_HMD_QUAD_GOURAUD_LIT:
    	size = sizeof(PsxHmdQuadGourLit);
        break;          
    case PSX_HMD_QUAD_GOURAUD_TEX_LIT:
    	size = sizeof(PsxHmdQuadGourTexLit);
        break;          
    case PSX_HMD_MESH_GOURAUD_TEX_LIT:
    	size = sizeof(PsxHmdMeshGourTexLit);
		if ( num > 1 )
			size += (sizeof(PsxHmdMeshGourTexLitEntry) * (num - 1));
        break;
    case PSX_HMD_MESH_GOURAUD_TEX_LIT_ENTRY:
    	size = (sizeof(PsxHmdMeshGourTexLitEntry) * num);
        break;
    case PSX_LINE_FLAT:
    	size = sizeof(PsxLineFlat);
        break;
	case PSX_HMD_SHARED_DATA:
		size = sizeof(PsxHmdSharedData);
		break;		
	case PSX_HMD_SHARED_BLOCK:
		size = sizeof(PsxHmdSharedBlock);
		break;		
    default:
		gfxError( GFX_ERROR_HMD_PRIM_TYPE_UNSUPPORTED );
		gfxAbort();
    }

#if 1
	/* get driver index for this primitive type */
	i = psxGetHmdDriver( psxobj, type );
	prim = (void *)(((u_long *)(&ps.screen.temp.data[i])) + (ps.screen.temp.size[i]>>2));
	/* allocate temporary static memory */
	ps.screen.temp.size[i] += size;
	/* check for overflow */
	if ( ps.screen.temp.size[i] > PSX_TEMP_BUFFER_SIZE ) {
		gfxError( GFX_ERROR_TEMP_BUFFER_OVERRUN );
		gfxAbort();
	}
	/* add to poly count */
	if ( i < PSX_HMD_MAX_TYPES_PER_OBJECT )
		psxobj->block.type[i].poly_count++;
#else
    /* size is in longs */   
    size /= 4;

    /* update static/dynamic primitive buffer size */
    ps.hmd.primitive_size += size;
	//printf( "object=%s, prim=%d, %d\n", ((gfxobj->name==NULL)?("NULL"):gfxobj->name), size, ps.hmd.primitive_size );
    if ( (ps.hmd.primitive_size<<2) > PSX_PRIMITIVE_BUFFER_SIZE ) {
		gfxError( GFX_ERROR_PRIMITIVE_OVERRUN );
		gfxAbort();
	}

    /* update object info */
    prim = (void *)(psxobj->primitive_top + psxobj->primitive_size);
#if GFX_DEBUG
    printf("primitive top = 0x%X\n", (u_long *)prim );
#endif
    psxobj->primitive_size += size;    
    psxobj->n_primitive++;
#endif

    /* return pointer to new primitive memory */
    return( prim );
}

 
/**********************************************************
 * Function:	psxGetHmdDriver
 **********************************************************
 * Description: Get the type index referencing a HMD driver
 *				Create the driver type if necessary.
 * Inputs:	psxobj - PSX HMD object to get type index from
 *			type   - primitive type
 * Notes:	
 * Returns:	index
 **********************************************************/

internal int psxGetHmdDriver (
    PsxHmdObject	*psxobj,
    PsxObjType 		 type
    )
{
	int				 i;
	u_long			*driver;

	/* set driver */
	driver = NULL;
	switch( type ) {
	case PSX_HMD_TRI_GOURAUD_LIT:
		if ( psxobj->shared )
			driver = (u_long *)(GsU_0100000c);		/* shared gour triangle */
		else {
			//if ( ps.transparency )
			//	driver = (u_long *)(GsU_0020000c);	/* semi-trans gour triangle */
			//else
			driver = (u_long *)(GsU_0000000c);		/* gour triangle */
			//driver = (u_long *)(GsU_0008000c);	/* active-div gour triangle */
			//driver = (u_long *)(GsU_0001000c);	/* div texture gour triangle */
		}
		break;
	case PSX_HMD_TRI_GOURAUD_TEX_LIT:		
		if ( psxobj->shared )
			driver = (u_long *)(GsU_0100000d);		/* shared texture gour triangle */
		else {
			//if ( ps.transparency )
			//	driver = (u_long *)(GsU_0020000d);	/* semi-trans texture gour triangle */
			//else
			driver = (u_long *)(GsU_0000000d);		/* texture gour triangle */
			//driver = (u_long *)(GsU_0008000d);	/* active-div texture gour triangle */
			//driver = (u_long *)(GsU_0001000d);	/* div texture gour triangle */
		}
		break;
	case PSX_HMD_MESH_GOURAUD_TEX_LIT:
	case PSX_HMD_MESH_GOURAUD_TEX_LIT_ENTRY:
		driver = (u_long *)(GsU_0000001d);		/* texture gour mesh */
		break;
	case PSX_HMD_SHARED_DATA:
	case PSX_HMD_SHARED_BLOCK:
		return( PSX_TEMP_SHARED_BUFFER );
		break;
	case PSX_HMD_TRI_FLAT:
	case PSX_HMD_TRI_FLAT_LIT:
	case PSX_HMD_TRI_FLAT_TEX:
	case PSX_HMD_TRI_FLAT_TEX_LIT:
	case PSX_HMD_QUAD_FLAT:
	case PSX_HMD_QUAD_FLAT_TEX:
	case PSX_HMD_QUAD_FLAT_LIT:
	case PSX_HMD_QUAD_FLAT_TEX_LIT:
	case PSX_HMD_QUAD_GOURAUD_LIT:
	case PSX_HMD_QUAD_GOURAUD_TEX_LIT:
	case PSX_LINE_FLAT:
	default:
		gfxError( GFX_ERROR_HMD_PRIM_TYPE_UNSUPPORTED );
		gfxAbort();
	}

	/* find driver index */
	i = 0;
	while( i<psxobj->block.type_count ) {
		if ( psxobj->block.type[i].driver == driver )
			return(i);
		i++;
	}

	/* room for another driver? */
	if ( psxobj->block.type_count >= PSX_HMD_MAX_TYPES_PER_OBJECT ) {
		gfxError( GFX_ERROR_HMD_TYPES_OVERRUN );
		gfxAbort();
	}

	/* create driver */
	psxobj->block.type[i].driver       = driver;
	psxobj->block.type[i].size	       = 2;
	psxobj->block.type[i].poly_count   = 0;
	psxobj->block.type[i].poly_offset  = 0;
	psxobj->block.type[i].poly_subdiv  = 0;
	psxobj->block.type_count++;

	/* return index */
	return(i);
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
    PsxHmdObject *psxobj;
    void		 *data;    
    int			  i, v;
	Bool		  found;

    /* init */
    psxobj = (PsxHmdObject *)gfxobj->data;

#if GFX_DEBUG
	printf( "psxAddVertex\n" );
#endif
	for ( v=0; v<vcnt; v++ ) {
		/* find if we have already set this vertex */
		found = FALSE;

#if GFX_USE_EXISTING_POINTS
		/* only reuse points if model is non shared */
		i = 0;
		while( i < psxobj->n_vert ) {
			vertex = (PsxVertex *)psxobj->vert_top;
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

		/* shared/skinned models cannot have duplicate points! */
		if ( (psxobj->shared != NULL) && found && vindex == NULL ) {
			gfxError( GFX_ERROR_HMD_DUPLICATE_POINTS );
			gfxAbort();
		}
#endif
		/* add vertex to list */
		if ( ! found ) {
		    /* update static/dynamic vertex counter */
			vertex = &ps.hmd.vertex[ps.hmd.n_vert];
			//if ( vindex )
			//	vindex[v] = ps.hmd.n_vert;
			ps.hmd.n_vert++;
			if ( ps.hmd.n_vert >= PSX_MAX_VERTEX ) {
				gfxError( GFX_ERROR_VERTEX_OVERRUN );
				gfxAbort();
			}
			/* add vertex */
		   	vertex->x = (short)pts[v].x;
			vertex->y = (short)-(pts[v].z);
			vertex->z = (short)pts[v].y;
			vertex->unused = 0;	
#if GFX_DEBUG
			printf("add point %d = %d, %d, %d, %X\n", ps.hmd.n_vert, vertex->x, vertex->y, vertex->z, vertex );
#endif
			if ( vindex )
				vindex[v] = psxobj->n_vert;
    		psxobj->n_vert++;
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
    PsxHmdObject *psxobj;
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
    psxobj = (PsxHmdObject *)gfxobj->data;

	for ( n=0; n<ncnt; n++ ) {
		/* update static/dynamic normal counter */
		normal = &ps.hmd.normal[ps.hmd.n_normal];
		nindex[n] = ps.hmd.n_normal;
		ps.hmd.n_normal++;
#if GFX_VALIDATE
		if ( ps.hmd.n_normal >= PSX_MAX_NORMAL ) {
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
    	printf( "add normal %s %d = %d, %d, %d, %X\n", gfxobj->name, ps.hmd.n_normal, normal->x, normal->y, normal->z, normal );
#endif
		normal->unused = 0;

		if ( nindex )
			nindex[n] = psxobj->n_normal;

		psxobj->n_normal++;
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
   	nobjects = &ps.hmd.nobjects;
   	if ( *nobjects == (PSX_MAX_OBJECT - 1) ) {
		gfxError( GFX_ERROR_OBJECT_OVERRUN );
		gfxAbort();
	}
    newobj = &ps.hmd.gfxobj[*nobjects];
    (*nobjects)++;

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
    GsCOORDUNIT		*coord,
    fixed		 	 x,
    fixed		 	 y,
    fixed		 	 z
    )
{ 
    VECTOR		 vector;
    PsxMatrix	*mat;
   
    /* init */ 
    mat = &coord->matrix;
    vector.vx = (long)x;
    vector.vy = (long)z;
    vector.vz = (long)y;

    /* add translation */
    ScaleMatrix( (MATRIX *)mat, &vector );

    /* update flag */
    coord->flg = 0;
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
    GsCOORDUNIT		*coord,
    int		 	 	 x,
    int		 	 	 y,
    int		 	 	 z
    )
{
    VECTOR		 vector;
    PsxMatrix	*mat;

    /* init */ 
    mat = &coord->matrix;

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
    coord->flg = 0;
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
    GsCOORDUNIT		*coord,
    char 	 		 axis,
    int 			 angle
    )
{
    PsxMatrix	*mat;
    long	 	 rot;

    /* init */ 
    mat = &coord->matrix;

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
    coord->flg = 0;
}
        

/**********************************************************
 * Function:	psxGetObjectCoord
 **********************************************************
 * Description: Get position data for a PSX Object
 * Inputs:		psxobj - PSX Object 
 * Notes:		Used ONLY by scale/translate/rotate functions	
 * Returns:		position pointer
 **********************************************************/

private  GsCOORDUNIT * psxGetObjectCoord (
    PsxHmdObject	*psxobj
    )
{
	PsxHmdHandleList *list;

	if ( psxobj->shared ) {
	    if ( psxobj->state == PSX_OBJ_DRAWN ) {
			gfxError( GFX_ERROR_SHARED_REDRAW_NOT_SUPPORTED );
			gfxAbort();
		}
		if ( psxobj->shared->current == GFX_PIVOT_WORLD )
		    return( &psxobj->current->coord );
		else
			return( &psxobj->shared->blocks[psxobj->shared->current].current->coord );
	}

    /* if 'drawn' object we need to add a new position handler */
    if ( psxobj->state == PSX_OBJ_DRAWN ) {
		list = psxobj->current->next;
		if ( ! list )
			list = psxAllocateExtraHandler( psxobj->current );
		/* copy current position to new handler */
		list->coord = psxobj->current->coord;
		list->coord.flg = 0;
		psxobj->current = list;
        /* update state of object */
        psxobj->state = PSX_OBJ_NOTDRAWN;
    } 

    return( &psxobj->current->coord );    
}


/**********************************************************
 * Function:	psxAllocateExtraHandler
 **********************************************************
 * Description: Get position data for a PSX Object
 * Inputs:		psxobj - PSX Object
 * Notes:		Used ONLY by scale/translate/rotate functions	
 * Returns:		position pointer
 **********************************************************/

private PsxHmdHandleList * psxAllocateExtraHandler (
	PsxHmdHandleList	*current 
	) 
{
	PsxHmdHandleList	*newhandler;

    /* check for buffer overrun */
    if ( ps.nextrahandlers >= PSX_MAX_EXTRAHANDLERS ) {
		gfxError( GFX_ERROR_EXTRAHANDLERS_OVERRUN );
		gfxAbort();
    }

    /* create and link new handler */
	newhandler  = &ps.hmd.extrahandlers[ps.nextrahandlers];
    ps.nextrahandlers++;

	/* init coord handle */
    memClear( newhandler, sizeof(GsCOORDUNIT) );
	newhandler->coord.matrix.m[0][0] = FIXED_ONE;
	newhandler->coord.matrix.m[1][1] = FIXED_ONE;
	newhandler->coord.matrix.m[2][2] = FIXED_ONE;
	newhandler->unit.coord   = &newhandler->coord;
	newhandler->unit.primtop = current->unit.primtop;
	newhandler->next = NULL;

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
    PsxHmdObject    *psxobj;
	GsCOORDUNIT		*coord;
	int				 t[3], i;

    /* init */
    psxobj = (PsxHmdObject *)object->data;
    coord  = psxGetObjectCoord( psxobj );

    /* multiply matrix */
#if 1
	MulMatrix( (MATRIX *)&coord->matrix, (MATRIX *)m );
#else
	for (i=0;i<3;i++)
		t[i] = m->t[i];
	memCopy( m, &coord->matrix, sizeof(PsxMatrix) );
	for (i=0;i<3;i++)
		coord->matrix.t[i] = t[i];
#endif
	coord->flg = 0;
}

#endif



