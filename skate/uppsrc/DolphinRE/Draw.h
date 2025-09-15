#pragma once


#define CIRCLE_STEPS	100
#define SQRT_3_DIV_3	0.57735026


struct PolygonId {
	uint vtx[3];
	uint norm[2];
};


extern GXVtxDescList		vcd[VCD_SIZE + 1];
extern GXVtxAttrFmtList		vat[VCD_SIZE + 1];
extern Vec					verts[14];
extern int					nrm_cnt;
extern Vec*					nrm_tab;
extern PolygonId			polygons[12];
extern Vec					odata[6];
extern byte					ondex[24];
extern Vec					idata[12];
extern byte					index_[60];



void GetVertState();
void RestoreVertState();
void GXGetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList* vat);
void GXSetVtxDescv(GXVtxDescList* attrPtr);
void GXDrawCubeFace(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, GXAttrType nbt_type, GXAttrType tex_type);
void VECNormalize1(Vec *src);
void VecPosNorm(Vec *a, Vec *b);
void SubDivTriangle(uint depth, uint i, Vec *v, byte *b);
void Subdivide(uint depth, Vec *v0, Vec *v1, Vec *v2);
void DumpTriangle(Vec *v0, Vec *v1, Vec *v2);
void SubDivNrm(uint depth, uint i, Vec *v, byte *b);
void SubdivideNrm(u8 depth, Vec *v0, Vec *v1, Vec *v2);
void AddNormal(Vec *v);
bool CmpNormal32(Vec *a, Vec *b);

/*
void GXDrawCylinder(u8 edge_count);
void GXDrawTorus(f32 rc,u8 numc,u8 numt);
void GXDrawSphere(u8 major,u8 minor);
void GXDrawCube(void);
void GXDrawDodeca(void);
void VECSubtract(Vec *a,Vec *b,Vec *a_b);
void VECCrossProduct(Vec *a,Vec *b,Vec *axb);
void GXDrawOctahedron(void);
void GXDrawIcosahedron(void);
void GXDrawSphere1(u8 depth);
u32 GXGenNormalTable(u8 depth, Vec* table);
void GXEnd(void)
void EmGXNormal3f32(float x, float y, float z)
void EmGXPosition3f32(float x, float y, float z)
void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts)
double sin(double x)
double cos(double x)
void OSPanic(char * file, int line, char * msg)
undefined __chkesp()
void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac)
void GXSetVtxDesc(GXAttr attr, GXAttrType type)
undefined GXClearVtxDesc()
void GXGetVtxDescv(GXVtxDescList * vcd)
void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, GXVtxAttrFmtList * list)
void EmGXTexCoord2f32(float s, float t)
void GXGetVtxDesc(GXAttr attr, GXAttrType * type)
void EmGXTexCoord2s8(s8 s, s8 t)
double sqrt(double __x)

*/
