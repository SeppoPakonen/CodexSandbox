#include "DolphinRE.h"



/*---------------------------------------------------------------------*
   Paired-single optimized code

   All paired-single code assumes GQR0 = 0.
 *---------------------------------------------------------------------*/
#ifdef GEKKO

/*---------------------------------------------------------------------*

Name:           PSMTXReorder

Description:    Creates a reordered (column-major) matrix from a
                row-major matrix, using paired single operations.
                Reordered matrices are required for the PSMTXRO* 
                functions, which operate faster than their non-reordered
                counterparts.

                Performance:  ~15 cycles.

Arguments:      src      source matrix.
                dest     destination matrix, note type is ROMtx.

Return:         none

*---------------------------------------------------------------------*/
asm void 
PSMTXReorder(register Mtx src, register ROMtx dest)
{
    nofralloc
#define S00_S01 fp0
#define S02_S03 fp1
#define S10_S11 fp2
#define S12_S13 fp3
#define S20_S21 fp4
#define S22_S23 fp5
#define D00_D10 fp6
#define D11_D21 fp7
#define D02_D12 fp8
#define D22_D03 fp9
#define D13_D23 fp10
#define D20_D01 fp12

    
    psq_l       S00_S01, 0(src),  0, 0 
    psq_l       S10_S11, 16(src), 0, 0 
    psq_l       S20_S21, 32(src), 0, 0 
    psq_l       S02_S03, 8(src),  0, 0 
    ps_merge00  D00_D10, S00_S01, S10_S11 
    psq_l       S12_S13, 24(src), 0, 0 
    ps_merge01  D20_D01, S20_S21, S00_S01
    psq_l       S22_S23, 40(src), 0, 0 
    ps_merge11  D11_D21, S10_S11, S20_S21
    psq_st      D00_D10, 0(dest), 0, 0
    ps_merge00  D02_D12, S02_S03, S12_S13 
    psq_st      D20_D01, 8(dest), 0, 0
    ps_merge01  D22_D03, S22_S23, S02_S03 
    psq_st      D11_D21, 16(dest),0, 0
    ps_merge11  D13_D23, S12_S13, S22_S23
    psq_st      D02_D12, 24(dest),0, 0
    psq_st      D22_D03, 32(dest),0,0
    psq_st      D13_D23, 40(dest),0,0

    blr
#undef S00_S01
#undef S02_S03
#undef S10_S11
#undef S12_S13
#undef S20_S21
#undef S22_S23
#undef D00_D10
#undef D11_D21
#undef D02_D12
#undef D22_D03
#undef D13_D23
#undef D20_D01

}

/*---------------------------------------------------------------------*

Name:           PSMTXConcat

Description:    Fast matrix concatenation using paired-single 
                operations.
                Order of operation is A x B = AB.
                OK for any of ab == a == b, no additional cost if
                destination = any source matrix.
                
                Note that this performs NO error checking.

                Performance : ~36-39 cycles.

Arguments:      src      source matrix.
                dest     destination matrix, note type is ROMtx.

Return:         none

*---------------------------------------------------------------------*/
f32 Unit01[] = {0.0f, 1.0f};

asm void 
#ifdef _DEBUG
PSMTXConcat(
#else
MTXConcat(
#endif // _DEBUG
    register Mtx mA,    // r3
    register Mtx mB,    // r4
    register Mtx mAB)   // r5
{
    nofralloc

#define A00_A01 fp0
#define A02_A03 fp1
#define A10_A11 fp2
#define A12_A13 fp3
#define A20_A21 fp4
#define A22_A23 fp5

#define B00_B01 fp6
#define B02_B03 fp7
#define B10_B11 fp8
#define B12_B13 fp9
#define B20_B21 fp10
#define B22_B23 fp11

#define D00_D01 fp12
#define D02_D03 fp13
#define D10_D11 fp14
#define D12_D13 fp15
#define D20_D21 fp2
#define D22_D23 fp0

#define UNIT01  fp31


    // don't save LR since we don't make any function calls
    //    mflr    r0
    //    stw     r0, 4(r1)
    stwu    r1, -64(r1)
    psq_l   A00_A01, 0(mA), 0, 0
    stfd    fp14, 8(r1)
    psq_l   B00_B01, 0(mB), 0, 0
    addis   r6, 0, Unit01@ha
    psq_l   B02_B03, 8(mB), 0, 0
    stfd    fp15, 16(r1)
    addi    r6, r6, Unit01@l
    stfd    fp31, 40(r1)
    psq_l   B10_B11, 16(mB), 0, 0
    // D00_D01 = b00a00 , b01a00  
    ps_muls0 D00_D01, B00_B01, A00_A01
    psq_l   A10_A11, 16(mA), 0, 0
    // D02_D03 = b02a00 , b03a00
    ps_muls0 D02_D03, B02_B03, A00_A01
    psq_l   UNIT01, 0(r6), 0, 0  
    // D10_D11 = a10b00 , a10b01
    ps_muls0 D10_D11, B00_B01, A10_A11
    psq_l   B12_B13, 24(mB), 0, 0
    // D12_D13 = a10b02 , a10b03
    ps_muls0 D12_D13, B02_B03, A10_A11
    psq_l   A02_A03, 8(mA), 0, 0 
        // fp12 = b10a01 + b00a00 , b11a01 + b01a00
        ps_madds1 D00_D01, B10_B11, A00_A01, D00_D01      
    psq_l   A12_A13, 24(mA), 0, 0
        // D10_D11 = a10b00 + a11b10 , a10b01 + a11b11
        ps_madds1 D10_D11, B10_B11, A10_A11, D10_D11
    psq_l   B20_B21, 32(mB), 0, 0
        // D02_D03 = b12a01 + b02a00 , b13a01 + b03a00
        ps_madds1 D02_D03, B12_B13, A00_A01, D02_D03  // YYY LAST TIME FP0 IS USED
    psq_l   B22_B23, 40(mB), 0, 0
        // D12_D13 = a10b02 + a11b12, a10b03+a11b13
        ps_madds1 D12_D13, B12_B13, A10_A11, D12_D13 // YYY LAST TIME FP2 IS USED
    psq_l   A20_A21, 32(mA), 0, 0
    psq_l   A22_A23, 40(mA), 0, 0
            // D00_D01 = b20a02 + b10a01 + b00a00 , b21a02 + b11a01 + b01a00
            ps_madds0 D00_D01, B20_B21, A02_A03, D00_D01 // m00, m01 computed
            // D02_D03 = b12a01 + b02a00 + b22a02 , b13a01 + b03a00 + b23a02
            ps_madds0 D02_D03, B22_B23, A02_A03, D02_D03 
            // D10_D11 = a10b00 + a11b10 +a12b20, a10b01 + a11b11 + a12b21
            ps_madds0 D10_D11, B20_B21, A12_A13, D10_D11 // m10, m11 computed
            // D12_D13 = a10b02 + a11b12 + a12b22, a10b03+a11b13 + a12b23 + a13
            ps_madds0 D12_D13, B22_B23, A12_A13, D12_D13


    // store m00m01 
    psq_st  D00_D01, 0(mAB), 0, 0 // YYY LAST TIME FP12 IS USED

    // D20_D21 = a20b00, a20b01
    ps_muls0 D20_D21, B00_B01, A20_A21 // YYY LAST TIME FP6 IS USED
                // get a03 from fp1 and add to D02_D03
                ps_madds1 D02_D03, UNIT01, A02_A03, D02_D03 // m02, m03 computed
                // YYY LAST TIME FP1 IS USED
    // D22_D23 = a20b02, a20b03
    ps_muls0 D22_D23, B02_B03, A20_A21 // YYY LAST TIME FP7 IS USED
    // store m10m11
    psq_st  D10_D11, 16(mAB), 0, 0 
                // get a13 from fp3 and add to D12_D13
                ps_madds1 D12_D13, UNIT01, A12_A13, D12_D13 // m12, m13 computed
    // store m02m03
    psq_st  D02_D03, 8(mAB), 0, 0 // YYY LAST TIME D02_D03 IS USED

        // D20_D21 = a20b00 + a21b10, a20b01 + a21b11
        ps_madds1 D20_D21, B10_B11, A20_A21, D20_D21 // YYY LAST TIME FP8 IS USED
        // D22_D23 = a20b02 + a21b12, a20b03 + a21b13
        ps_madds1 D22_D23, B12_B13, A20_A21, D22_D23
            // D20_D21 = a20b00 + a21b10 + a22b20, a20b01 + a21b11 + a22b21
            ps_madds0 D20_D21, B20_B21, A22_A23, D20_D21
    // Restore fp14
    lfd    fp14, 8(r1)  // D10_D11 
    // store m12m13
    psq_st  D12_D13, 24(mAB), 0, 0
            // D22_D23 = a20b02 + a21b12 + a22b22, a20b03 + a21b13 + a22b23 + a23
            ps_madds0 D22_D23, B22_B23, A22_A23, D22_D23
    // store m20m21
    psq_st  D20_D21, 32(mAB), 0, 0
                // get a23 from fp5 and add to fp17
                ps_madds1 D22_D23, UNIT01, A22_A23, D22_D23
    // restore stack frame
    lfd    fp15, 16(r1) // D12_D13 
    // store m22m23
    psq_st  D22_D23, 40(mAB), 0, 0

    lfd    fp31, 40(r1)
    addi   r1, r1, 64


    blr 


#undef A00_A01
#undef A02_A03
#undef A10_A11
#undef A12_A13
#undef A20_A21
#undef A22_A23

#undef B00_B01
#undef B02_B03
#undef B10_B11
#undef B12_B13
#undef B20_B21
#undef B22_B23

#undef D00_D01
#undef D02_D03
#undef D10_D11
#undef D12_D13
#undef D20_D21
#undef D22_D23

#undef UNIT01 


/*  
  / m[0][0] = a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0];
fp12m[0][1] = a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1];
  / m[0][2] = a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2];
fp13m[0][3] = a[0][0]*b[0][3] + a[0][1]*b[1][3] + a[0][2]*b[2][3] + a[0][3];

  / m[1][0] = a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0];
fp14m[1][1] = a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1];
  / m[1][2] = a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2];
fp15m[1][3] = a[1][0]*b[0][3] + a[1][1]*b[1][3] + a[1][2]*b[2][3] + a[1][3];

  / m[2][0] = a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0];
fp16m[2][1] = a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1];
  / m[2][2] = a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2];
fp17m[2][3] = a[2][0]*b[0][3] + a[2][1]*b[1][3] + a[2][2]*b[2][3] + a[2][3];
OPTIMIZATIONS : 
  fp0 instead of fp17
  fp2 instead of fp16
    psq_l   fp0, 0(mA), 0, 0    // a[0][0], a[0][1]
    psq_l   fp1, 8(mA), 0, 0    // a[0][2], a[0][3]
    psq_l   fp2, 16(mA), 0, 0   // a[1][0], a[1][1]
    psq_l   fp3, 24(mA), 0, 0   // a[1][2], a[1][3]
    psq_l   fp4, 32(mA), 0, 0   // a[2][0], a[2][1]
    psq_l   fp5, 40(mA), 0, 0   // a[2][2], a[2][3]

    psq_l   fp6, 0(mB), 0, 0    // b[0][0], b[0][1]
    psq_l   fp7, 8(mB), 0, 0    // b[0][2], b[0][3]
    psq_l   fp8, 16(mB), 0, 0   // b[1][0], b[1][1]
    psq_l   fp9, 24(mB), 0, 0   // b[1][2], b[1][3]
    psq_l   fp10, 32(mB), 0, 0   // b[2][0], b[2][1]
    psq_l   fp11, 40(mB), 0, 0   // b[2][2], b[2][3]

*/
}

/*---------------------------------------------------------------------*

Name:            PSMTXROMultVecArray

Description:    Multiplies an array of vectors by a reordered matrix, 
                using paired single operations.
                This function is significantly faster than 
                PSMTXMultVecArray, but requires that you have reordered
                the matrix in advance with PSMTXReorder.
                OK if source = destination.
                NOTE: number of vertices transformed cannot be less than 
                2.
                
                Note that NO error checking is performed.

                Performance : 9.586 - 9.814 cycles per vertex, where
                              count = 70

Arguments:      m         reordered matrix.
                srcBase   start of source vector array. 
                dstBase   start of resultant vector array.
                count     number of vectors in srcBase, dstBase arrays
                          COUNT MUST BE GREATER THAN 2.


Return:         none

*---------------------------------------------------------------------*/
asm void 
PSMTXROMultVecArray
( 
    register ROMtx  m,      // r3
    register VecPtr srcBase,// r4
    register VecPtr dstBase,// r5
    register u32    count   // r6
)
{
    nofralloc
#define M00_M10 fp0
#define M20_nnn fp1
#define M01_M11 fp2
#define M21_nnn fp3
#define M02_M12 fp4
#define M22_nnn fp5
#define M03_M13 fp6
#define M23_nnn fp7

// source vectors - 2 3D vectors in 3 PS registers
#define SX0_SY0 fp8
#define SZ0_SX1 fp9
#define SY1_SZ1 fp10
// Destination registers - 2 3d vectors in 4 PS registers
#define DX0_DY0 fp11
#define DZ0_nnn fp12
#define DX1_DY1 fp13
#define DZ1_nnn fp14
// temp registers for writing back values.  These registers store the final
// results from the PREVIOUS loop
#define WX0_WY0 fp15
#define WZ0_nnn fp16
#define WX1_WY1 fp17
#define WZ1_nnn fp18

    stwu    r1, -64(r1)
    stfd    fp14, 8(r1)
    // unrolled once, but since we're dividing by 2, add 1 to ensure if
    // odd # of vertices, the last one gets x-formed.
    addi    r7, count, -1    
    stfd    fp15, 16(r1)
    srwi    r7, r7, 1 // 2 at a time
    stfd    fp16, 24(r1)
    stfd    fp17, 32(r1)
    stfd    fp18, 40(r1)
    mtctr   r7
    // load matrix
    psq_l   M00_M10, 0(m),0,0  
    addi    srcBase, srcBase, -8
    psq_l   M20_nnn, 8(m),1,0  
    addi    dstBase, dstBase, -4
    psq_l   M03_M13, 36(m),0,0 
    psq_lu  SX0_SY0, 8(srcBase), 0, 0
    psq_l   M23_nnn, 44(m),1,0 
    psq_lu  SZ0_SX1, 8(srcBase), 0, 0

    // ------------------------------UNROLLED

    //  DX0=M00*SX0+M03, DY0=M10*SX0+M13
    //  DZ0=M20*SX0+M23
    //  DX1=M00*SX1+M03, DY1=M10*SX1+M13
    //  DZ1=M20*SX1+M23

    ps_madds0    DX0_DY0, M00_M10, SX0_SY0, M03_M13
    psq_l   M01_M11, 12(m),0,0 
    ps_madds0    DZ0_nnn, M20_nnn, SX0_SY0, M23_nnn
    psq_l   M21_nnn, 20(m),1,0   
    ps_madds1    DX1_DY1, M00_M10, SZ0_SX1, M03_M13
    psq_lu SY1_SZ1,8(srcBase), 0, 0
    ps_madds1    DZ1_nnn, M20_nnn, SZ0_SX1, M23_nnn
    psq_l   M22_nnn, 32(m),1,0 

    //  DX0=M01*SY0+DX0, DY0=M11*SY0+DY0
    //  DZ0=M21*SY0+DZ0
    //  DX1=M01*SY1+DX1, DY1=M11*SY1+DY1
    //  DZ1=M21*SY1+DZ1

    ps_madds1    DX0_DY0, M01_M11, SX0_SY0, DX0_DY0
    ps_madds1    DZ0_nnn, M21_nnn, SX0_SY0, DZ0_nnn
    psq_l   M02_M12, 24(m),0,0 
    ps_madds0    DX1_DY1, M01_M11, SY1_SZ1, DX1_DY1
    psq_lu SX0_SY0, 8(srcBase), 0, 0
    ps_madds0    DZ1_nnn, M21_nnn, SY1_SZ1, DZ1_nnn

    //  DX0=M02*SZ0+DX0, DY0=M12*SZ0+DY0
    //  DZ0=M22*SZ0+DZ0
    //  DX1=M02*SZ1+DX1, DY1=M12*SZ1+DY1
    //  DZ1=M22*SZ1+DZ1

    // Write final values to temp registers
    ps_madds0    WX0_WY0, M02_M12, SZ0_SX1, DX0_DY0
    ps_madds0    WZ0_nnn, M22_nnn, SZ0_SX1, DZ0_nnn
    psq_lu SZ0_SX1, 8(srcBase), 0, 0
    ps_madds1    WX1_WY1, M02_M12, SY1_SZ1, DX1_DY1
    ps_madds1    WZ1_nnn, M22_nnn, SY1_SZ1, DZ1_nnn
    psq_lu SY1_SZ1,8(srcBase), 0, 0

    // -------------------------- LOOP START
_mloop:
    ps_madds0    DX0_DY0, M00_M10, SX0_SY0, M03_M13
      psq_stu     WX0_WY0, 4(dstBase), 0, 0
    ps_madds0    DZ0_nnn, M20_nnn, SX0_SY0, M23_nnn
      psq_stu     WZ0_nnn, 8(dstBase), 1, 0
    ps_madds1    DX1_DY1, M00_M10, SZ0_SX1, M03_M13
      psq_stu     WX1_WY1, 4(dstBase), 0, 0
    ps_madds1    DZ1_nnn, M20_nnn, SZ0_SX1, M23_nnn
      psq_stu     WZ1_nnn, 8(dstBase), 1, 0
    ps_madds1    DX0_DY0, M01_M11, SX0_SY0, DX0_DY0
    ps_madds1    DZ0_nnn, M21_nnn, SX0_SY0, DZ0_nnn
    // YYY LAST SX0_SY0 USAGE FOR THIS ITERATION
      psq_lu SX0_SY0, 8(srcBase), 0, 0 // NEXT SX0 SY0
    ps_madds0    DX1_DY1, M01_M11, SY1_SZ1, DX1_DY1
    ps_madds0    DZ1_nnn, M21_nnn, SY1_SZ1, DZ1_nnn

    // Write final values to temp registers
    ps_madds0    WX0_WY0, M02_M12, SZ0_SX1, DX0_DY0
    ps_madds0    WZ0_nnn, M22_nnn, SZ0_SX1, DZ0_nnn
    // YYY LAST SZ0_SX1 USAGE FOR THIS ITERATION
      psq_lu SZ0_SX1, 8(srcBase), 0, 0 // NEXT SZ0 SX1
    ps_madds1    WX1_WY1, M02_M12, SY1_SZ1, DX1_DY1
    ps_madds1    WZ1_nnn, M22_nnn, SY1_SZ1, DZ1_nnn
    // YYY LAST SY1_SZ1 USAGE FOR THIS ITERATION
      psq_lu SY1_SZ1,8(srcBase), 0, 0 // NEXT SY1 SZ1

    bdnz+ _mloop    // -------------------------- LOOP END

    psq_stu     WX0_WY0, 4(dstBase), 0, 0
    rlwinm.     r7, count, 0, 31, 31 // check odd
    psq_stu     WZ0_nnn, 8(dstBase), 1, 0
    bne     _return
    // Skipped if odd number of vectors
    psq_stu     WX1_WY1, 4(dstBase), 0, 0
    // can't put anything here
    psq_stu     WZ1_nnn, 8(dstBase), 1, 0


_return:    
    lfd     fp14, 8(r1)
    lfd     fp15, 16(r1)
    lfd     fp16, 24(r1)
    lfd     fp17, 32(r1)
    lfd     fp18, 40(r1)
    addi    r1, r1, 64
    blr

#undef M00_M10
#undef M20_nnn
#undef M01_M11
#undef M21_nnn
#undef M02_M12
#undef M22_nnn
#undef M03_M13
#undef M23_nnn
#undef SX0_SY0
#undef SZ0_SX1
#undef SY1_SZ1
#undef DX0_DY0
#undef DZ0_nnn
#undef DX1_DY1
#undef DZ1_nnn
#undef WX0_WY0
#undef WZ0_nnn
#undef WX1_WY1
#undef WZ1_nnn

}

/*---------------------------------------------------------------------*

Name:           PSMTXROSkin2VecArray

Description:    Multiplies an array of vectors by a reordered matrix, 
                using paired single operations.
                This function is significantly faster than 
                PSMTXMultVecArray, but requires that you have reordered
                the matrix in advance with PSMTXReorder.
                OK if source = destination.
                NOTE: number of vertices transformed cannot be less than 
                2.
                
                Note that NO error checking is performed.

                Performance : ~20.8 cycles per vertex, where
                              count = 70

Arguments:      m0        first reordered matrix
                m1        second reordered matrix
                wtBase    start of per vertex weight array
                srcBase   start of source vector array. 
                dstBase   start of resultant vector array.
                count     number of vectors in srcBase, dstBase arrays
                          COUNT MUST BE GREATER THAN 2.


Return:         none

*---------------------------------------------------------------------*/
asm void 
PSMTXROSkin2VecArray
( 
    register ROMtx  m0,     // r3
    register ROMtx  m1,     // r4
    register f32 *  wtBase,  // r5
    register VecPtr srcBase,// r6
    register VecPtr dstBase,// r7
    register u32    count   // r8
)
{
    nofralloc
// transposed matrix
#define M00_10  fp0
#define M20     fp1
#define M01_11  fp2
#define M21     fp3
#define M02_12  fp4
#define M22     fp5
#define M03_13  fp6
#define M23     fp7

// source vector - 1 3D vectors in 2 PS registers
#define Sx_y    fp8
#define Sz      fp9

// Destination vector - 1 3d vector in 2 PS registers
#define Dx_y    fp10
#define Dz      fp11

// intermediate vector 1 3D vector in 2 PS registers
#define Ix_y    fp12
#define Iz      fp13

#define M0_00_10    fp14
#define M0_20       fp15
#define M0_01_11    fp16
#define M0_21       fp17
#define M0_02_12    fp18
#define M0_22       fp19
#define M0_03_13    fp20
#define M0_23       fp21


#define M1_00_10    fp22
#define M1_20       fp23
#define M1_01_11    fp24
#define M1_21       fp25
#define M1_02_12    fp26
#define M1_22       fp27
#define M1_03_13    fp28
#define M1_23       fp29

#define Wt          fp30

    // save FP regs
    stwu        r1, -160(r1)
    stfd        fp14,  8(r1)
    stfd        fp15, 16(r1)
    stfd        fp16, 24(r1)
    stfd        fp17, 32(r1)
    stfd        fp18, 40(r1)
    stfd        fp19, 48(r1)
    stfd        fp20, 56(r1)
    stfd        fp21, 64(r1)
    stfd        fp22, 72(r1)
    stfd        fp23, 80(r1)
    stfd        fp24, 88(r1)
    stfd        fp25, 96(r1)
    stfd        fp26, 104(r1)
    stfd        fp27, 112(r1)
    stfd        fp28, 120(r1)
    stfd        fp29, 128(r1)
    stfd        fp30, 136(r1)

    // always perform at least one iteration of loop
    addi        r9, count, -1    
    mtctr       r9

    addi        srcBase, srcBase, -4
    addi        dstBase, dstBase, -4
    addi        wtBase,  wtBase,  -4

    // load matrices m0 and (m1-m0)
    psq_l       M0_00_10,0(m0),0,0  
    psq_l       M1_00_10,0(m1),0,0  

    psq_l       M0_20, 8(m0),1,0  
    psq_l       M1_20, 8(m1),1,0  
    
    psq_l       M0_01_11, 12(m0),0,0 
    psq_l       M1_01_11, 12(m1),0,0 
    
    ps_sub      M1_00_10,M1_00_10,M0_00_10

    psq_l       M0_21, 20(m0),1,0 
    psq_l       M1_21, 20(m1),1,0 
    
    ps_sub      M1_20,M1_20,M0_20

    psq_l       M0_02_12, 24(m0),0,0 
    psq_l       M1_02_12, 24(m1),0,0 
    
    ps_sub      M1_01_11,M1_01_11,M0_01_11
    
    psq_l       M0_22, 32(m0),1,0 
    psq_l       M1_22, 32(m1),1,0 
    
    ps_sub      M1_21,M1_21,M0_21
    
    psq_l       M0_03_13, 36(m0),0,0 
    psq_l       M1_03_13, 36(m1),0,0 
    
    ps_sub      M1_02_12,M1_02_12,M0_02_12
    
    psq_l       M0_23, 44(m0),1,0 
    psq_l       M1_23, 44(m1),1,0 
    
    ps_sub      M1_22,M1_22,M0_22
    ps_sub      M1_03_13,M1_03_13,M0_03_13
    ps_sub      M1_23,M1_23,M0_23

    //start of first iteration
    psq_lu      Wt,4(wtBase),1,0                // Wt = *wtBase++;
    psq_lu      Sx_y, 4(srcBase), 0, 0          // Sx_y = *srcBase++;
    psq_lu      Sz, 8(srcBase), 1, 0            // Sz = *srcBase++;

    ps_madds0   M00_10,M1_00_10,Wt,M0_00_10     // m = lerp(m0, m1, wt);     
    ps_madds0   M20,M1_20,Wt,M0_20              // m = lerp(m0, m1, wt);     
    ps_madds0   M01_11,M1_01_11,Wt,M0_01_11     // m = lerp(m0, m1, wt);     
    ps_madds0   M21,M1_21,Wt,M0_21              // m = lerp(m0, m1, wt);     
    ps_madds0   M02_12,M1_02_12,Wt,M0_02_12     // m = lerp(m0, m1, wt);     
    ps_madds0   M22,M1_22,Wt,M0_22              // m = lerp(m0, m1, wt);     
    ps_madds0   M03_13,M1_03_13,Wt,M0_03_13     // m = lerp(m0, m1, wt);     
    ps_madds0   M23,M1_23,Wt,M0_23              // m = lerp(m0, m1, wt);     

    ps_madds0   Ix_y, M00_10, Sx_y, M03_13      // Ix_y = M03_13 + M00_10 * Sx;
    ps_madds0   Iz, M20, Sx_y, M23              // Iz   = M23    + M20    * Sx;

    psq_lu      Wt,4(wtBase),1,0                // Wt = *wtBase++;

_mloop:
    ps_madds1   Ix_y,M01_11,Sx_y,Ix_y           // Ix_y += M01_11 * Sy;
    ps_madds1   Iz,M21,Sx_y,Iz                  // Iz   += M21    * Sy;
    
    psq_lu      Sx_y, 4(srcBase), 0, 0          // Sx_y = *srcBase++;
    
    ps_madds0   Dx_y, M02_12, Sz, Ix_y          // Dx_y = Ix_y + M01_12 * Sz;
    ps_madds0   Dz, M22, Sz, Iz                 // Dz   = Iz   + M22    * Sz;
    
    psq_lu      Sz, 8(srcBase), 1, 0            // Sz = *srcBase++;

    ps_madds0   M00_10,M1_00_10,Wt,M0_00_10     // m = lerp(m0, m1, wt);     
    ps_madds0   M20,M1_20,Wt,M0_20              // m = lerp(m0, m1, wt);     
    ps_madds0   M01_11,M1_01_11,Wt,M0_01_11     // m = lerp(m0, m1, wt);     
    ps_madds0   M21,M1_21,Wt,M0_21              // m = lerp(m0, m1, wt);     
    ps_madds0   M02_12,M1_02_12,Wt,M0_02_12     // m = lerp(m0, m1, wt);     
    ps_madds0   M22,M1_22,Wt,M0_22              // m = lerp(m0, m1, wt);     
    ps_madds0   M03_13,M1_03_13,Wt,M0_03_13     // m = lerp(m0, m1, wt);     
    ps_madds0   M23,M1_23,Wt,M0_23              // m = lerp(m0, m1, wt);     

    psq_stu     Dx_y, 4(dstBase), 0, 0          // *dstBase++ = Dx_y;

    ps_madds0   Ix_y, M00_10, Sx_y, M03_13      // Ix_y = M03_13 + M00_10 * Sx;
    ps_madds0   Iz, M20, Sx_y, M23              // Iz   = M23    + M20    * Sx;
    
    psq_stu     Dz, 8(dstBase), 1, 0            // *dstBase++ = Dz;

    psq_lu      Wt,4(wtBase),1,0                // Wt = *wtBase++;

    bdnz+       _mloop
_mlend:

    ps_madds1   Ix_y,M01_11,Sx_y,Ix_y           // Ix_y += M01_11 * Sy;
    ps_madds1   Iz,M21,Sx_y,Iz                  // Iz   += M21    * Sy;
    
    ps_madds0   Dx_y, M02_12, Sz, Ix_y          // Dx_y = Ix_y + M01_12 * Sz;
    
    psq_stu     Dx_y, 4(dstBase), 0, 0          // *dstBase++ = Dx_y;
    
    ps_madds0   Dz, M22, Sz, Iz                 // Dz   = Iz   + M22    * Sz;
    
    psq_stu     Dz, 8(dstBase), 1, 0            // *dstBase++ = Dz;

    lfd         fp14,  8(r1)
    lfd         fp15, 16(r1)
    lfd         fp16, 24(r1)
    lfd         fp17, 32(r1)
    lfd         fp18, 40(r1)
    lfd         fp19, 48(r1)
    lfd         fp20, 56(r1)
    lfd         fp21, 64(r1)
    lfd         fp22, 72(r1)
    lfd         fp23, 80(r1)
    lfd         fp24, 88(r1)
    lfd         fp25, 96(r1)
    lfd         fp26, 104(r1)
    lfd         fp27, 112(r1)
    lfd         fp28, 120(r1)
    lfd         fp29, 128(r1)
    lfd         fp30, 136(r1)
    addi        r1, r1, 160
    
    blr

#undef M00_10
#undef M20
#undef M01_11
#undef M21
#undef M02_12
#undef M22
#undef M03_13
#undef M23

#undef Sx_y
#undef Sz

#undef Dx_y
#undef Dz

#undef Ix_y
#undef Iz

#undef  M0_00_10
#undef  M0_20   
#undef  M0_01_11
#undef  M0_21   
#undef  M0_02_12
#undef  M0_22   
#undef  M0_03_13
#undef  M0_23   


#undef  M1_00_10
#undef  M1_20   
#undef  M1_01_11
#undef  M1_21   
#undef  M1_02_12
#undef  M1_22   
#undef  M1_03_13
#undef  M1_23   

#undef  Wt
}

/*---------------------------------------------------------------------*

Name:           PSMTXROMultS16VecArray

Description:    Multiplies an array of signed 16 bit vectors by a
                reordered matrix, generating a Vec array of floats.
                No cost in conversion.  However, this code does take a
                hit because it uses mtspr to set up a quantization
                register to convert S16 -> F32.  For production code,
                the GQR should be set up in advance.

                OK if source = destination.
                
                Note that NO error checking is performed.

                Performance : 9.671 - 9.900 cycles per vertex where
                              count = 70

Arguments:      m         matrix.
                srcBase   start of source s16 vector array. 
                dstBase   start of resultant vector array. Note that 
                          available room should be twice as large as
                          source data.
                count     number of vectors in srcBase, dstBase arrays
                          COUNT MUST BE GREATER THAN 1.


Return:         none

*---------------------------------------------------------------------*/
asm void 
PSMTXROMultS16VecArray
( 
    register ROMtx      m,      // r3
    register S16VecPtr  srcBase,// r4
    register VecPtr     dstBase,// r5
    register u32        count   // r6
)
{
    nofralloc
#define M00_M10 fp0
#define M20_nnn fp1
#define M01_M11 fp2
#define M21_nnn fp3
#define M02_M12 fp4
#define M22_nnn fp5
#define M03_M13 fp6
#define M23_nnn fp7

// source vectors - 2 3D vectors in 3 PS registers
#define SX0_SY0 fp8
#define SZ0_SX1 fp9
#define SY1_SZ1 fp10
// Destination registers - 2 3d vectors in 4 PS registers
#define DX0_DY0 fp11
#define DZ0_nnn fp12
#define DX1_DY1 fp13
#define DZ1_nnn fp14
// temp registers for writing back values.  These registers store the final
// results from the PREVIOUS loop
#define WX0_WY0 fp15
#define WZ0_nnn fp16
#define WX1_WY1 fp17
#define WZ1_nnn fp18

    stwu    r1, -64(r1)
    stfd    fp14, 8(r1)
    // unrolled once, but since we're dividing by 2, add 1 to ensure if
    // odd # of vertices, the last one gets x-formed.
    addi    r7, count, -1    
    stfd    fp15, 16(r1)
    srwi    r7, r7, 1 // 2 at a time
    stfd    fp16, 24(r1)
    lis     r8, 0x0007  // setup GQR1
    stfd    fp17, 32(r1)
    mtspr   GQR1, r8    // this will stall like a monkey's butt
    stfd    fp18, 40(r1)
    mtctr   r7
    // load matrix
    psq_l   M00_M10, 0(m),0,0  
    addi    srcBase, srcBase, -4
    psq_l   M20_nnn, 8(m),1,0  
    addi    dstBase, dstBase, -4
    psq_l   M03_M13, 36(m),0,0 
    psq_lu  SX0_SY0, 4(srcBase), 0, 1
    psq_l   M23_nnn, 44(m),1,0 
    psq_lu  SZ0_SX1, 4(srcBase), 0, 1


    // ------------------------------UNROLLED

    //  DX0=M00*SX0+M03, DY0=M10*SX0+M13
    //  DZ0=M20*SX0+M23
    //  DX1=M00*SX1+M03, DY1=M10*SX1+M13
    //  DZ1=M20*SX1+M23


    ps_madds0    DX0_DY0, M00_M10, SX0_SY0, M03_M13
    psq_l   M01_M11, 12(m),0,0 
    ps_madds0    DZ0_nnn, M20_nnn, SX0_SY0, M23_nnn
    psq_l   M21_nnn, 20(m),1,0   
    ps_madds1    DX1_DY1, M00_M10, SZ0_SX1, M03_M13
    psq_lu SY1_SZ1, 4(srcBase), 0, 1
    ps_madds1    DZ1_nnn, M20_nnn, SZ0_SX1, M23_nnn
    psq_l   M22_nnn, 32(m),1,0 

    //  DX0=M01*SY0+DX0, DY0=M11*SY0+DY0
    //  DZ0=M21*SY0+DZ0
    //  DX1=M01*SY1+DX1, DY1=M11*SY1+DY1
    //  DZ1=M21*SY1+DZ1

    ps_madds1    DX0_DY0, M01_M11, SX0_SY0, DX0_DY0
    ps_madds1    DZ0_nnn, M21_nnn, SX0_SY0, DZ0_nnn
    psq_l   M02_M12, 24(m),0,0 
    ps_madds0    DX1_DY1, M01_M11, SY1_SZ1, DX1_DY1
    psq_lu SX0_SY0, 4(srcBase), 0, 1
    ps_madds0    DZ1_nnn, M21_nnn, SY1_SZ1, DZ1_nnn

    //  DX0=M02*SZ0+DX0, DY0=M12*SZ0+DY0
    //  DZ0=M22*SZ0+DZ0
    //  DX1=M02*SZ1+DX1, DY1=M12*SZ1+DY1
    //  DZ1=M22*SZ1+DZ1

    // Write final values to temp registers
    ps_madds0    WX0_WY0, M02_M12, SZ0_SX1, DX0_DY0
    ps_madds0    WZ0_nnn, M22_nnn, SZ0_SX1, DZ0_nnn
    psq_lu SZ0_SX1, 4(srcBase), 0, 1
    ps_madds1    WX1_WY1, M02_M12, SY1_SZ1, DX1_DY1
    ps_madds1    WZ1_nnn, M22_nnn, SY1_SZ1, DZ1_nnn
    psq_lu SY1_SZ1, 4(srcBase), 0, 1

    // -------------------------- LOOP START
_mloop:
    ps_madds0    DX0_DY0, M00_M10, SX0_SY0, M03_M13
      psq_stu     WX0_WY0, 4(dstBase), 0, 0
    ps_madds0    DZ0_nnn, M20_nnn, SX0_SY0, M23_nnn
      psq_stu     WZ0_nnn, 8(dstBase), 1, 0
    ps_madds1    DX1_DY1, M00_M10, SZ0_SX1, M03_M13
      psq_stu     WX1_WY1, 4(dstBase), 0, 0
    ps_madds1    DZ1_nnn, M20_nnn, SZ0_SX1, M23_nnn
      psq_stu     WZ1_nnn, 8(dstBase), 1, 0
    ps_madds1    DX0_DY0, M01_M11, SX0_SY0, DX0_DY0
    ps_madds1    DZ0_nnn, M21_nnn, SX0_SY0, DZ0_nnn
    // YYY LAST SX0_SY0 USAGE FOR THIS ITERATION
      psq_lu SX0_SY0, 4(srcBase), 0, 1 // NEXT SX0 SY0
    ps_madds0    DX1_DY1, M01_M11, SY1_SZ1, DX1_DY1
    ps_madds0    DZ1_nnn, M21_nnn, SY1_SZ1, DZ1_nnn

    // Write final values to temp registers
    ps_madds0    WX0_WY0, M02_M12, SZ0_SX1, DX0_DY0
    ps_madds0    WZ0_nnn, M22_nnn, SZ0_SX1, DZ0_nnn
    // YYY LAST SZ0_SX1 USAGE FOR THIS ITERATION
      psq_lu SZ0_SX1, 4(srcBase), 0, 1 // NEXT SZ0 SX1
    ps_madds1    WX1_WY1, M02_M12, SY1_SZ1, DX1_DY1
    ps_madds1    WZ1_nnn, M22_nnn, SY1_SZ1, DZ1_nnn
    // YYY LAST SY1_SZ1 USAGE FOR THIS ITERATION
      psq_lu SY1_SZ1, 4(srcBase), 0, 1 // NEXT SY1 SZ1

    bdnz+ _mloop    // -------------------------- LOOP END

    psq_stu     WX0_WY0, 4(dstBase), 0, 0
    rlwinm.     r7, count, 0, 31, 31 // check odd
    psq_stu     WZ0_nnn, 8(dstBase), 1, 0
    bne     _return
    // Skipped if odd number of vectors
    psq_stu     WX1_WY1, 4(dstBase), 0, 0
    // can't put anything here
    psq_stu     WZ1_nnn, 8(dstBase), 1, 0


_return:    
    lfd     fp14, 8(r1)
    lfd     fp15, 16(r1)
    lfd     fp16, 24(r1)
    lfd     fp17, 32(r1)
    lfd     fp18, 40(r1)
    addi    r1, r1, 64
    blr


#undef M00_M10
#undef M20_nnn
#undef M01_M11
#undef M21_nnn
#undef M02_M12
#undef M22_nnn
#undef M03_M13
#undef M23_nnn
#undef SX0_SY0
#undef SZ0_SX1
#undef SY1_SZ1
#undef DX0_DY0
#undef DZ0_nnn
#undef DX1_DY1
#undef DZ1_nnn
#undef WX0_WY0
#undef WZ0_nnn
#undef WX1_WY1
#undef WZ1_nnn

}


/*---------------------------------------------------------------------*

Name:           PSMTXMultVecArray

Description:    Multiplies an array of vectors by a matrix, using 
                paired single operations.
                This function takes longer than PSMTXROMultVecArray
                which uses reordered matrices.  It's approx 40% slower.
                OK if source = destination.
                
                Note that NO error checking is performed.

                Performance : 13.371 - 13.457 cycles per vertex where
                              count = 70

Arguments:      m         matrix.
                srcBase   start of source vector array. 
                dstBase   start of resultant vector array.
                count     number of vectors in srcBase, dstBase arrays
                          COUNT MUST BE GREATER THAN 1.


Return:         none

*---------------------------------------------------------------------*/
asm void
#ifdef _DEBUG
PSMTXMultVecArray 
#else
MTXMultVecArray
#endif // _DEBUG
( 
    register Mtx    m,      // r3
    register VecPtr srcBase,// r4
    register VecPtr dstBase,// r5
    register u32    count   // r6
)
{
    nofralloc
    //      cmpwi   count, 0
    //      beq-    _return
    psq_l   fp0, 0(m), 0, 0    // [0][0], [0][1]
    subi    count, count, 1 // unrolling once
    // fp6 - x y 
    psq_l   fp6, 0(srcBase), 0, 0 
    mtctr   count
    // fp7 - z 1
    psq_l   fp7, 8(srcBase), 1, 0 
    // unused slot here
    psq_l   fp1, 8(m), 0, 0    // [0][2], [0][3]
    addi    srcBase, srcBase, 8 // load ops will add 4, 
                                // but we already got the first vertex
    psq_l   fp2, 16(m), 0, 0   // [1][0], [1][1]
    addi    dstBase, dstBase, -4 // store ops will add 4
    psq_l   fp3, 24(m), 0, 0   // [1][2], [1][3]




    // ------ first loop starts here
    // fp8 = m00x m01y // next X
    ps_mul  fp8, fp0, fp6
    psq_l   fp4, 32(m), 0, 0   // [2][0], [2][1]
    // fp10 = m10x m11y // next Y
    ps_mul  fp10, fp2, fp6
    psq_l   fp5, 40(m), 0, 0   // [2][2], [2][3]
    // fp12 = m20x m21y // next Z
    ps_mul  fp12, fp4, fp6  // YYY last FP6 usage

    // fp6 - x y 
    psq_lu  fp6, 4(srcBase), 0, 0 // advance to x
    // Potential FP stall here if psq_lu dispatches same
    // cycle as previous ps_mul

    // fp8 = m00x + m02z  | m01y + m03
    ps_madd fp8, fp1, fp7 ,fp8
    // fp10 = m10x + m12z  | m11y + m13
    ps_madd fp10, fp3, fp7 ,fp10
    // fp12 = m20x + m22z  | m21y + m23
    ps_madd fp12, fp5, fp7 ,fp12 // YYY last FP7 usage

    // fp7 - z 1
    psq_lu  fp7, 8(srcBase), 1, 0 // advance to z, will be skipped by next lu
    // Potential FP stall here if psq_lu dispatches same
    // cycle as previous ps_madd (fp8 dependency)
    ps_sum0 fp9, fp8, fp8, fp8 // X ready
    
    // ------------------- main loop
_mloop:
    ps_sum0 fp11, fp10, fp10, fp10 // Y ready    
    // fp8 = m00x m01y // next X
    ps_mul  fp8, fp0, fp6
    ps_sum0 fp13, fp12, fp12, fp12 // Z ready
    // fp10 = m10x m11y // next Y
    ps_mul  fp10, fp2, fp6
      psq_stu  fp9,  4(dstBase), 1, 0   // prev X
    // fp12 = m20x m21y // next Z
    ps_mul  fp12, fp4, fp6  // YYY last FP6 usage
      psq_stu  fp11, 4(dstBase), 1, 0   // prev Y
    // fp8 = m00x + m02z  | m01y + m03
    ps_madd fp8, fp1, fp7 ,fp8
      psq_stu  fp13, 4(dstBase), 1, 0   // prev Z
    // fp10 = m10x + m12z  | m11y + m13
    ps_madd fp10, fp3, fp7 ,fp10
      // fp6 - x y 
      psq_lu  fp6, 4(srcBase), 0, 0 // advance to x
    // fp12 = m20x + m22z  | m21y + m23
    ps_madd fp12, fp5, fp7 ,fp12 // YYY last FP7 usage

    // fp7 - z 1
    psq_lu  fp7, 8(srcBase), 1, 0 // advance to z, will be skipped by next lu
    // Potential FP stall here if psq_lu dispatches same
    // cycle as previous ps_madd
    
    ps_sum0 fp9, fp8, fp8, fp8 // X ready
    bdnz+   _mloop 
    // ------------------- end of loop
    ps_sum0 fp11, fp10, fp10, fp10 // Y ready    
    ps_sum0 fp13, fp12, fp12, fp12 // Z ready

    // commit last iteration
    psq_stu  fp9,  4(dstBase), 1, 0
    // INTERLOCK ON FP11
    psq_stu  fp11, 4(dstBase), 1, 0
    psq_stu  fp13, 4(dstBase), 1, 0

_return:
    blr
}



/*---------------------------------------------------------------------*

Name:           PSMTXMultS16VecArray

Description:    Multiplies an array of signed 16 bit vectors by a matrix, 
                generating a Vec array of floats.  No cost in conversion.
                However, this code does take a hit because it uses 
                mtspr to set up a quantization register to convert
                S16 -> F32.  For production code, the GQR should be set
                up in advance.

                This function takes longer than PSMTXROMultS16VecArray
                which uses reordered matrices.  
                OK if source = destination.
                
                Note that NO error checking is performed.

                Performance : 13.714 - 13.786 cycles per vertex where
                              count = 70

Arguments:      m         matrix.
                srcBase   start of source vector array. 
                dstBase   start of resultant vector array. Note that 
                          available room should be twice as large as
                          source data.
                count     number of vectors in srcBase, dstBase arrays
                          COUNT MUST BE GREATER THAN 1.


Return:         none

*---------------------------------------------------------------------*/
asm void 
PSMTXMultS16VecArray 
( 
    register Mtx    m,      // r3
    register S16Vec* srcBase,// r4
    register VecPtr dstBase,// r5
    register u32    count   // r6
)
{
    nofralloc
    //      cmpwi   count, 0
    //      beq-    _return
    psq_l   fp0, 0(m), 0, 0    // [0][0], [0][1]
    lis     r7, 0x0007  // setup GQR1
    mtspr   GQR1, r7    // this will stall like a monkey's butt
    // fp6 - x y 
    psq_l   fp6, 0(srcBase), 0, 1
    subi    count, count, 1 // unrolling once
    // fp7 - z 1
    psq_l   fp7, 4(srcBase), 1, 1 
    mtctr   count
    // unused slot here
    psq_l   fp1, 8(m), 0, 0    // [0][2], [0][3]
    addi    srcBase, srcBase, 4 // load ops will add 2, 
                                // but we already got the first vertex
    psq_l   fp2, 16(m), 0, 0   // [1][0], [1][1]
    addi    dstBase, dstBase, -4 // store ops will add 4
    psq_l   fp3, 24(m), 0, 0   // [1][2], [1][3]




    // ------ first loop starts here
    // fp8 = m00x m01y // next X
    ps_mul  fp8, fp0, fp6
    psq_l   fp4, 32(m), 0, 0   // [2][0], [2][1]
    // fp10 = m10x m11y // next Y
    ps_mul  fp10, fp2, fp6
    psq_l   fp5, 40(m), 0, 0   // [2][2], [2][3]
    // fp12 = m20x m21y // next Z
    ps_mul  fp12, fp4, fp6  // YYY last FP6 usage

    // fp6 - x y 
    psq_lu  fp6, 2(srcBase), 0, 1 // advance to x
    // Potential FP stall here if psq_lu dispatches same
    // cycle as previous ps_mul

    // fp8 = m00x + m02z  | m01y + m03
    ps_madd fp8, fp1, fp7 ,fp8
    // fp10 = m10x + m12z  | m11y + m13
    ps_madd fp10, fp3, fp7 ,fp10
    // fp12 = m20x + m22z  | m21y + m23
    ps_madd fp12, fp5, fp7 ,fp12 // YYY last FP7 usage

    // fp7 - z 1
    psq_lu  fp7, 4(srcBase), 1, 1 // advance to z, will be skipped by next lu
    // Potential FP stall here if psq_lu dispatches same
    // cycle as previous ps_madd (fp8 dependency)
    ps_sum0 fp9, fp8, fp8, fp8 // X ready
    
    // ------------------- main loop
_mloop:
    ps_sum0 fp11, fp10, fp10, fp10 // Y ready    
    // fp8 = m00x m01y // next X
    ps_mul  fp8, fp0, fp6
    ps_sum0 fp13, fp12, fp12, fp12 // Z ready
    // fp10 = m10x m11y // next Y
    ps_mul  fp10, fp2, fp6
      psq_stu  fp9,  4(dstBase), 1, 0   // prev X
    // fp12 = m20x m21y // next Z
    ps_mul  fp12, fp4, fp6  // YYY last FP6 usage
      psq_stu  fp11, 4(dstBase), 1, 0   // prev Y
    // fp8 = m00x + m02z  | m01y + m03
    ps_madd fp8, fp1, fp7 ,fp8
      psq_stu  fp13, 4(dstBase), 1, 0   // prev Z
    // fp10 = m10x + m12z  | m11y + m13
    ps_madd fp10, fp3, fp7 ,fp10
      // fp6 - x y 
      psq_lu  fp6, 2(srcBase), 0, 1 // advance to x
    // fp12 = m20x + m22z  | m21y + m23
    ps_madd fp12, fp5, fp7 ,fp12 // YYY last FP7 usage

    // fp7 - z 1
    psq_lu  fp7, 4(srcBase), 1, 1 // advance to z, will be skipped by next lu
    // Potential FP stall here if psq_lu dispatches same
    // cycle as previous ps_madd
    
    ps_sum0 fp9, fp8, fp8, fp8 // X ready
    bdnz+   _mloop 
    // ------------------- end of loop
    ps_sum0 fp11, fp10, fp10, fp10 // Y ready    
    ps_sum0 fp13, fp12, fp12, fp12 // Z ready
    // commit last iteration
    psq_stu  fp9,  4(dstBase), 1, 0
    psq_stu  fp11, 4(dstBase), 1, 0
    psq_stu  fp13, 4(dstBase), 1, 0

_return:
    blr
}


#endif // GEKKO


/*===========================================================================*/

