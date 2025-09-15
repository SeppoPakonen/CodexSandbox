#include "DolphinRE.h"

GXVtxDescList		vcd[VCD_SIZE + 1];
GXVtxAttrFmtList	vat[VCD_SIZE + 1];
Vec					verts[14];
int					nrm_cnt;
Vec*				nrm_tab;
PolygonId			polygons[12];
Vec					odata[6];
byte				ondex[24];
Vec					idata[12];
byte				index_[60];


void GXDrawCylinder(u8 edge_count) {
	float edge_angle_x[CIRCLE_STEPS];
	float edge_angle_y[CIRCLE_STEPS];
	float top = 1.0;
	float bottom = -1.0;
	ASSERTMSG(edge_count < CIRCLE_STEPS, "GXDrawCylinder: too many edges");
	
	GetVertState();
	
	for(int edge = 0; edge <= edge_count; edge++) {
		double edge_angle_rad = (double)edge * M_PI_MUL2 / (double)edge_count;
		double x = cos(edge_angle_rad);
		double y = sin(edge_angle_rad);
		edge_angle_x[edge] = x;
		edge_angle_y[edge] = y;
	}
	
	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (int)edge_count * 2 + 2);
	for(int edge = 0; edge <= edge_count; edge++) {
		EmGXPosition3f32	(edge_angle_x[edge], edge_angle_y[edge], bottom);
		EmGXNormal3f32		(edge_angle_x[edge], edge_angle_y[edge], 0.0);
		EmGXPosition3f32	(edge_angle_x[edge], edge_angle_y[edge], top);
		EmGXNormal3f32		(edge_angle_x[edge], edge_angle_y[edge], 0.0);
	}
	GXEnd();
	
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT3, (int)edge_count + 2);
	EmGXPosition3f32	(0.0, 0.0, top);
	EmGXNormal3f32		(0.0, 0.0, 1.0);
	for(int edge = 0; edge <= edge_count; edge++) {
		EmGXPosition3f32	(edge_angle_x[edge], -edge_angle_y[edge], top);
		EmGXNormal3f32		(0.0, 0.0, 1.0);
	}
	GXEnd();
	
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT3, (int)edge_count + 2);
	EmGXPosition3f32	(0.0, 0.0, bottom);
	EmGXNormal3f32		(0.0, 0.0, -1.0);
	for(int edge = 0; edge <= edge_count; edge++) {
		EmGXPosition3f32	(edge_angle_x[edge], edge_angle_y[edge], bottom);
		EmGXNormal3f32		(0.0, 0.0, -1.0);
	}
	GXEnd();
	
	RestoreVertState();
}


void GetVertState() {
	GXGetVtxDescv(vcd);
	GXGetVtxAttrFmtv(GX_VTXFMT3, vat);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
	GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_NRM, GX_TEX_S, GX_RGBA6, 0);
}


void RestoreVertState() {
	GXSetVtxDescv(vcd);
	GXSetVtxAttrFmtv(GX_VTXFMT3, vat);
}


void GXDrawTorus(f32 rc, u8 numc, u8 numt) {
	ASSERTMSG(rc < 1.0, "GXDrawTorus: doughnut too fat");
	
	float rc_diff = 1.0 - (float)rc;
	GXAttrType attr_type;
	GXGetVtxDesc(GX_VA_TEX0, &attr_type);
	GetVertState();
	if (attr_type != GX_NONE) {
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
	}
	for(int c = 0; c < numc; c++) {
		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (ushort)numt * 2 + 2);
		for(int t = 0; t <= numt; t++) {
			for(int c_off = 1; c_off >= 0; c_off--) {
				double c_mod = (c + c_off) % numc;
				double t_mod = t % numt;
				double c_x = cos(c_mod * M_PI_MUL2 / numc);
				double c_y = sin(c_mod * M_PI_MUL2 / numc);
				double t_x = cos(t_mod * M_PI_MUL2 / numt);
				double t_y = sin(t_mod * M_PI_MUL2 / numt);
				double c_rc_diff = rc_diff - c_x * rc;
				double t_rc = t_y * c_rc_diff;
				double tc_mul = t_x * (rc_diff - c_x * rc);
				
				EmGXPosition3f32(tc_mul, t_rc, c_y * rc);
				EmGXNormal3f32(-t_x * c_x, -t_y * c_x, c_y);
				
				if (attr_type != GX_NONE) {
					double c_frac = (double)(c + c_off) / (double)numc;
					double t_frac = (double)t / (double)numt;
					EmGXTexCoord2f32(c_frac, t_frac);
				}
			}
		}
		GXEnd();
	}
	RestoreVertState();
}


void GXDrawSphere(u8 major, u8 minor) {
	/*
	float major_angle_;
	float major_y__;
	float major_y_next__;
	int iVar1;
	undefined4 *puVar2;
	double major_y;
	double major_y_next;
	double major_x;
	double major_x_next;
	double minor_x;
	double minor_y;
	undefined4 uStack0000001c;
	float major_y_;
	uint minor_;
	uint major_;
	float minor_y_;
	float minor_x_;
	float major_x_next_;
	float major_x_;
	float major_y_next_;
	int minor_i;
	int major_i;
	float major_step_;
	float minor_step_;
	float minor_x__;
	*/
	
	float major_step = M_PI			/ (float)major;
	float minor_step = M_PI_MUL2	/ (float)minor;
	GXAttrType attr_type;
	
	GXGetVtxDesc(GX_VA_TEX0, &attr_type);
	GetVertState();
	if (attr_type != GX_NONE) {
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
	}
	
	for(int major_i = 0; major_i < major; major_i++) {
		float major_angle	= major_i * major_step;
		float major_x		= cos(major_angle);
		float major_y		= sin(major_angle);
		float major_x_next	= cos(major_angle + major_step);
		float major_y_next	= sin(major_angle + major_step);
		
		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (ushort)minor * 2 + 2);
		for(int minor_i = 0; minor_i <= minor; minor_i++) {
			float minor_x = cos(minor_i * minor_step);
			float minor_y = sin(minor_i * minor_step);
			
			EmGXPosition3f32(
				minor_x * major_y_next,
				minor_y * major_y_next,
				major_x_next);
			
			EmGXNormal3f32(
				minor_x * major_y_next / 1.0,
				minor_y * major_y_next / 1.0,
				major_x_next);
			
			if (attr_type != GX_NONE) {
				EmGXTexCoord2f32(
					(float)minor_i			/ (float)minor,
					(float)(major_i + 1)	/ (float)major);
			}
			EmGXPosition3f32(
				minor_x * major_y,
				minor_y * major_y,
				major_x);
			
			EmGXNormal3f32(
				minor_x * major_y,
				minor_y * major_y,
				major_x);
			
			if (attr_type != GX_NONE) {
				EmGXTexCoord2f32(
					(float)minor_i / (float)minor,
					(float)major_i / (float)major);
			}
		}
		GXEnd();
	}
	RestoreVertState();
}


void GXDrawCube() {
	
	GXAttrType tex_type;
	GXAttrType nbt_type;
	
	GXGetVtxDesc(GX_VA_NBT, &nbt_type);
	GXGetVtxDesc(GX_VA_TEX0, &tex_type);
	GetVertState();
	
	if (nbt_type != GX_NONE) {
		GXSetVtxDesc(GX_VA_NBT, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_NBT, GX_TEX_ST, GX_RGBA6, 0);
	}
	
	if (tex_type != GX_NONE) {
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);
	}
	
	GXBegin(GX_QUADS, GX_VTXFMT3, 0x18);
	
	GXDrawCubeFace(-1.0,  0.0,  0.0,  0.0,  0.0, -1.0,  0.0,  1.0,  0.0, nbt_type, tex_type);
	GXDrawCubeFace( 1.0,  0.0,  0.0,  0.0,  1.0,  0.0,  0.0,  0.0, -1.0, nbt_type, tex_type);
	GXDrawCubeFace( 0.0, -1.0,  0.0, -1.0,  0.0,  0.0,  0.0,  0.0,  1.0, nbt_type, tex_type);
	GXDrawCubeFace( 0.0,  1.0,  0.0,  0.0,  0.0,  1.0, -1.0,  0.0,  0.0, nbt_type, tex_type);
	GXDrawCubeFace( 0.0,  0.0, -1.0,  0.0, -1.0,  0.0,  1.0,  0.0,  0.0, nbt_type, tex_type);
	GXDrawCubeFace( 0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0, -1.0,  0.0, nbt_type, tex_type);
	
	GXEnd();
	RestoreVertState();
}


void GXDrawCubeFace(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, GXAttrType nbt_type, GXAttrType tex_type) {
	Vec arr;
	float zzz = f8;
	float mul = SQRT_3_DIV_3;
	
	EmGXPosition3f32(
		(f0 + f3 + f6) * SQRT_3_DIV_3,
		(f1 + f4 + f7) * SQRT_3_DIV_3,
		(f2 + f5 + f8) * SQRT_3_DIV_3);
	EmGXNormal3f32(f0, f1, f2);
	
	if (nbt_type != GX_NONE) {
		EmGXNormal3f32(f3, f4, f5);
		EmGXNormal3f32(f6, f7, f8);
	}
	if (tex_type != GX_NONE) {
		EmGXTexCoord2s8(1, 1);
	}
	
	EmGXPosition3f32(
		(f0 - f3 + f6) * mul,
		(f1 - f4 + f7) * mul,
		(f2 - f5 + f8) * mul);
	EmGXNormal3f32(f0, f1, f2);
	
	if (nbt_type != GX_NONE) {
		EmGXNormal3f32(f3, f4, f5);
		EmGXNormal3f32(f6, f7, f8);
	}
	if (tex_type != GX_NONE) {
		EmGXTexCoord2s8(0, 1);
	}
	
	EmGXPosition3f32(
		(f0 - f3 - f6) * mul,
		(f1 - f4 - f7) * mul,
		(f2 - f5 - f8) * mul);
	EmGXNormal3f32(f0, f1, f2);
	
	if (nbt_type != GX_NONE) {
		EmGXNormal3f32(f3, f4, f5);
		EmGXNormal3f32(f6, f7, f8);
	}
	if (tex_type != GX_NONE) {
		EmGXTexCoord2s8(0, 0);
	}
	
	EmGXPosition3f32(
		(f0 + f3 - f6) * mul,
		(f1 + f4 - f7) * mul,
		(f2 + f5 - f8) * mul);
	EmGXNormal3f32(f0, f1, f2);
	
	if (nbt_type != GX_NONE) {
		EmGXNormal3f32(f3, f4, f5);
		EmGXNormal3f32(f6, f7, f8);
	}
	if (tex_type != GX_NONE) {
		EmGXTexCoord2s8(1, 0);
	}
	
}



void GXDrawDodeca() {
	Vec cross_10_12;
	Vec sub12, sub10;
	Vec* vtx[3];
	uint i;
	
	GetVertState();
	for(int i = 0; i < 0xC; i++) {
		vtx[2] = &verts[polygons[i].vtx[0]];
		vtx[1] = &verts[polygons[i].vtx[1]];
		vtx[0] = &verts[polygons[i].vtx[2]];
		
		VECSubtract(vtx[1], vtx[0], &sub10);
		VECSubtract(vtx[1], vtx[2], &sub12);
		VECCrossProduct(&sub10, &sub12, &cross_10_12);
		VECNormalize1(&cross_10_12);
		
		GXBegin(GX_TRIANGLEFAN, GX_VTXFMT3, 5);
		VecPosNorm(&verts[polygons[i].norm[1]], &cross_10_12);
		VecPosNorm(&verts[polygons[i].norm[0]], &cross_10_12);
		VecPosNorm(vtx[0], &cross_10_12);
		VecPosNorm(vtx[1], &cross_10_12);
		VecPosNorm(vtx[2], &cross_10_12);
		GXEnd();
	}
	
	RestoreVertState();
}

/*
void VECSubtract(Vec *a, Vec *b, Vec *a_b) {
	a_b->x = b->x - a->x;
	a_b->y = b->y - a->y;
	a_b->z = b->z - a->z;
}


void VECCrossProduct(Vec *a, Vec *b, Vec *axb) {
	float az = a->z;
	float bx = b->x;
	float ax = a->x;
	float bz = b->z;
	float by = b->y;
	float ay = a->y;
	
	axb->x = ay * bz - az * by;
	axb->y = az * bx - ax * bz;
	axb->z = ax * by - ay * bx;
}
*/

void VECNormalize1(Vec *src) {
	double len = sqrt(
		src->z * src->z +
		src->y * src->y +
		src->x * src->x);
	
	ASSERTMSG(len != 0.0, "normalize: zero length vector");
	
	src->x = src->x / len;
	src->y = src->y / len;
	src->z = src->z / len;
}


void VecPosNorm(Vec *a, Vec *b) {
	EmGXPosition3f32(a->x, a->y, a->z);
	EmGXNormal3f32(b->x, b->y, b->z);
}


void GXDrawOctahedron() {
	GetVertState();
	
	for(int i = 7; i >= 0; i--)
		SubDivTriangle(0, i, odata, ondex);
	
	RestoreVertState();
}


void SubDivTriangle(uint depth, uint i, Vec *v, byte *b) {
	Subdivide(		depth,
					&v[b[i * 3 + 0]],
					&v[b[i * 3 + 1]],
					&v[b[i * 3 + 2]]);
}


void Subdivide(uint depth, Vec *v0, Vec *v1, Vec *v2) {
	Vec poly [3];
	
	if (depth == 0)
		DumpTriangle(v0, v1, v2);
	
	else {
		poly[2].x = v0->x + v1->x;
		poly[1].x = v1->x + v2->x;
		poly[0].x = v2->x + v0->x;
		
		poly[2].y = v0->y + v1->y;
		poly[1].y = v1->y + v2->y;
		poly[0].y = v2->y + v0->y;
		
		poly[2].z = v0->z + v1->z;
		poly[1].z = v1->z + v2->z;
		poly[0].z = v2->z + v0->z;
		
		VECNormalize1(&poly[2]);
		VECNormalize1(&poly[1]);
		VECNormalize1(&poly[0]);
		
		Subdivide(depth - 1,	v0,			poly + 2,	poly		);
		Subdivide(depth - 1,	v1,			poly + 1,	poly + 2	);
		Subdivide(depth - 1,	v2,			poly,		poly + 1	);
		Subdivide(depth - 1,	poly + 2,	poly + 1,	poly		);
	}
}


void DumpTriangle(Vec *v0, Vec *v1, Vec *v2) {
	GXBegin(GX_TRIANGLES, GX_VTXFMT3, 3);
	VecPosNorm(v0, v0);
	VecPosNorm(v1, v1);
	VecPosNorm(v2, v2);
	GXEnd();
}


void GXDrawIcosahedron() {
	GetVertState();
	
	for(int i = 0x13; i >= 0; i--)
		SubDivTriangle(0, i, idata, index_);
	
	RestoreVertState();
}


void GXDrawSphere1(u8 depth) {
	GetVertState();
	
	for(int i = 0x13; i >= 0; i--)
		SubDivTriangle(depth, i, idata, index_);
	
	RestoreVertState();
}


u32 GXGenNormalTable(u8 depth, Vec* table) {
	nrm_cnt = 0;
	nrm_tab = table;
	
	for(int i = 7; i >= 0; i--)
		SubDivNrm(depth, i, odata, ondex);
	
	return 0;
}


void SubDivNrm(uint depth, uint i, Vec *v, byte *b) {
	Vec* nrm[3];
	nrm[2] = &v[b[i * 3    ]];
	nrm[1] = &v[b[i * 3 + 1]];
	nrm[0] = &v[b[i * 3 + 2]];
	
	SubdivideNrm(depth, nrm[2], nrm[1], nrm[0]);
}


void SubdivideNrm(u8 depth, Vec *v0, Vec *v1, Vec *v2) {
	if (depth == 0) {
		AddNormal(v0);
		AddNormal(v1);
		AddNormal(v2);
	}
	else {
		Vec poly[3];
		
		poly[2].x = v0->x + v1->x;
		poly[1].x = v1->x + v2->x;
		poly[0].x = v2->x + v0->x;
		
		poly[2].y = v0->y + v1->y;
		poly[1].y = v1->y + v2->y;
		poly[0].y = v2->y + v0->y;
		
		poly[2].z = v0->z + v1->z;
		poly[1].z = v1->z + v2->z;
		poly[0].z = v2->z + v0->z;
		
		VECNormalize1(&poly[2]);
		VECNormalize1(&poly[1]);
		VECNormalize1(&poly[0]);
		
		SubdivideNrm(depth - 1,	v0,			poly + 2,	poly);
		SubdivideNrm(depth - 1,	v1,			poly + 1,	poly + 2);
		SubdivideNrm(depth - 1,	v2,			poly,		poly + 1);
		SubdivideNrm(depth - 1,	poly + 2,	poly + 1,	poly);
	}
}


void AddNormal(Vec *v) {
	for(int cnt = 0; cnt < nrm_cnt; cnt++)
		if (CmpNormal32(v, &nrm_tab[cnt]))
			return;
	
	nrm_tab[nrm_cnt].x = v->x;
	nrm_tab[nrm_cnt].y = v->y;
	nrm_tab[nrm_cnt].z = v->z;
	nrm_cnt++;
}


bool CmpNormal32(Vec *a, Vec *b) {
	return	a->x == b->x &&
			a->y == b->y &&
			a->z == b->z;
}

