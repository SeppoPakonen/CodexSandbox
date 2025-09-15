
#ifndef THEYER_PSX_IL_H
#define THEYER_PSX_IL_H

#include <type/datatype.h>
#include <compile.h>

#ifdef OsTypeIsPsx

#include <gfx/gfxpsx.h>

extern PsxData ps;

/* inline function replacement macros */
#define gfxSetObjectPointIL(gfxobj,vindex,ipt) { \
	ps.tmp.i = (vindex - 1); \
    ps.tmp.psx_obj     = (PsxObject *)(gfxobj->data); \
    ps.tmp.psx_tmd_obj = &(ps.tmp.psx_obj->object); \
	ps.tmp.psx_vertex  = (PsxVertex *)(ps.tmp.psx_tmd_obj->vert_top); \
	ps.tmp.psx_vertex[ps.tmp.i].x = (short)  (((Point_i3d *)(ipt))->x); \
	ps.tmp.psx_vertex[ps.tmp.i].y = (short) -(((Point_i3d *)(ipt))->z); \
	ps.tmp.psx_vertex[ps.tmp.i].z = (short)  (((Point_i3d *)(ipt))->y); \
}

#define gfxSetObjectNormalIL(gfxobj,vindex,ipt) { \
	ps.tmp.i = (vindex - 1); \
    ps.tmp.psx_obj     = (PsxObject *)(gfxobj->data); \
    ps.tmp.psx_tmd_obj = &(ps.tmp.psx_obj->object); \
	ps.tmp.psx_vertex  = (PsxVertex *)(ps.tmp.psx_tmd_obj->normal_top); \
	ps.tmp.psx_vertex[ps.tmp.i].x = (short)  (((Point_fx3d *)(ipt))->x); \
	ps.tmp.psx_vertex[ps.tmp.i].y = (short) -(((Point_fx3d *)(ipt))->z); \
	ps.tmp.psx_vertex[ps.tmp.i].z = (short)  (((Point_fx3d *)(ipt))->y); \
}

#else

#define gfxSetObjectNormalIL(gfxobj,vindex,ipt)		gfxSetObjectNormal(gfxobj,vindex,ipt)
#define gfxSetObjectPointIL(gfxobj,vindex,ipt)		gfxSetObjectPoint(gfxobj,vindex,ipt)

#endif

#endif	// THEYER_PSX_IL_H

