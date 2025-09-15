#ifndef _DolphinRE_Mtx_h_
#define _DolphinRE_Mtx_h_








//---------------------------------------------------------------------------------




//     GENERAL   SECTION

#define	MTX_IDENTITY_1			"MtxIdentity():  NULL Mtx 'm' "

#define	MTX_COPY_1				"MTXCopy():  NULL MtxPtr 'src' "
#define MTX_COPY_2				"MTXCopy():  NULL MtxPtr 'dst' "
			
#define	MTX_CONCAT_1			"MTXConcat():  NULL MtxPtr 'a'  "
#define	MTX_CONCAT_2			"MTXConcat():  NULL MtxPtr 'b'  "
#define	MTX_CONCAT_3			"MTXConcat():  NULL MtxPtr 'ab' "	
		
#define	MTX_MULTVEC_1			"MTXMultVec():  NULL MtxPtr 'm' "
#define	MTX_MULTVEC_2			"MTXMultVec():  NULL MtxPtr 'src' "
#define	MTX_MULTVEC_3			"MTXMultVec():  NULL MtxPtr 'dst' "
		
#define	MTX_MULTVECARRAY_1		"MTXMultVecArray():  NULL MtxPtr 'm' "
#define	MTX_MULTVECARRAY_2		"MTXMultVecArray():  NULL MtxPtr 'srcBase' "
#define	MTX_MULTVECARRAY_3		"MTXMultVecArray():  NULL MtxPtr 'dstBase' "
	   
#define	MTX_MULTVECSR_1		"MTXMultVecSR():  NULL MtxPtr 'm' "
#define	MTX_MULTVECSR_2		"MTXMultVecSR():  NULL MtxPtr 'src' "
#define	MTX_MULTVECSR_3		"MTXMultVecSR():  NULL MtxPtr 'dst' "
		
#define	MTX_MULTVECARRAYSR_1	"MTXMultVecArraySR():  NULL MtxPtr 'm' "
#define	MTX_MULTVECARRAYSR_2	"MTXMultVecArraySR():  NULL MtxPtr 'srcBase' "
#define	MTX_MULTVECARRAYSR_3	"MTXMultVecArraySR():  NULL MtxPtr 'dstBase' "
	   
#define	MTX_TRANSPOSE_1			"MTXTranspose():  NULL MtxPtr 'src' "
#define	MTX_TRANSPOSE_2			"MTXTranspose():  NULL MtxPtr 'xPose' "
	
#define	MTX_INVERSE_1			"MTXInverse():  NULL MtxPtr 'src' "		
#define	MTX_INVERSE_2			"MTXInverse():  NULL MtxPtr 'inv' "


//  MODEL  SECTION

#define	MTX_ROTDEG_1			"MTXRotDeg():  NULL MtxPtr 'm' "
#define	MTX_ROTDEG_2			"MTXRotDeg():  invalid 'axis' value "
	
#define	MTX_ROTTRIG_1			"MTXRotTrig():  NULL MtxPtr 'm' "
#define	MTX_ROTTRIG_2			"MTXRotTrig():  invalid 'axis' value "
	
#define	MTX_ROTAXIS_1			"MTXRotAxis():  NULL MtxPtr 'm' "
#define	MTX_ROTAXIS_2			"MTXRotAxis():  NULL VecPtr 'axis' "  

#define	MTX_TRANS_1				"MTXTrans():  NULL MtxPtr 'm' "
	
#define	MTX_SCALE_1				"MTXScale():  NULL MtxPtr 'm' "

#define	MTX_QUAT_1				"MTXQuat():  NULL MtxPtr 'm' "
#define	MTX_QUAT_2				"MTXQuat():  NULL QuaternionPtr 'q' "		
#define	MTX_QUAT_3				"MTXQuat():  zero-value quaternion "



//  VIEW   SECTION

#define	MTX_LOOKAT_1			"MTXLookAt():  NULL MtxPtr 'm' " 
#define	MTX_LOOKAT_2			"MTXLookAt():  NULL VecPtr 'camPos' "
#define	MTX_LOOKAT_3			"MTXLookAt():  NULL VecPtr 'camUp' "	
#define	MTX_LOOKAT_4			"MTXLookAt():  NULL Point3dPtr 'target' "



//  PROJECTION   SECTION

#define	MTX_FRUSTUM_1			"MTXFrustum():  NULL Mtx44Ptr 'm' "
#define	MTX_FRUSTUM_2			"MTXFrustum():  't' and 'b' clipping planes are equal "
#define	MTX_FRUSTUM_3			"MTXFrustum():  'l' and 'r' clipping planes are equal "
#define	MTX_FRUSTUM_4			"MTXFrustum():  'n' and 'f' clipping planes are equal "
		
#define	MTX_PERSPECTIVE_1		"MTXPerspective():  NULL Mtx44Ptr 'm' "
#define	MTX_PERSPECTIVE_2		"MTXPerspective():  'fovY' out of range "
#define	MTX_PERSPECTIVE_3		"MTXPerspective():  'aspect' is 0 "
	
#define	MTX_ORTHO_1				"MTXOrtho():  NULL Mtx44Ptr 'm' "
#define	MTX_ORTHO_2				"MTXOrtho():  't' and 'b' clipping planes are equal "
#define	MTX_ORTHO_3				"MTXOrtho():  'l' and 'r' clipping planes are equal "	
#define	MTX_ORTHO_4				"MTXOrtho():  'n' and 'f' clipping planes are equal "



//  STACK   SECTION

#define	MTX_INITSTACK_1			"MTXInitStack():  NULL MtxStackPtr 'sPtr' "
#define	MTX_INITSTACK_2			"MTXInitStack():  'sPtr' contains a NULL ptr to stack memory "
#define	MTX_INITSTACK_3			"MTXInitStack():  'numMtx' is 0 " 
	
#define	MTX_PUSH_1				"MTXPush():  NULL MtxStackPtr 'sPtr' " 
#define	MTX_PUSH_2				"MTXPush():  'sPtr' contains a NULL ptr to stack memory "
#define	MTX_PUSH_3				"MTXPush():  NULL MtxPtr 'm' "
#define	MTX_PUSH_4				"MTXPush():  stack overflow "
			
#define	MTX_PUSHFWD_1			"MTXPushFwd():  NULL MtxStackPtr 'sPtr' " 
#define	MTX_PUSHFWD_2			"MTXPushFwd():  'sPtr' contains a NULL ptr to stack memory "
#define	MTX_PUSHFWD_3			"MTXPushFwd():  NULL MtxPtr 'm' "
#define	MTX_PUSHFWD_4			"MTXPushFwd():  stack overflow"

#define	MTX_PUSHINV_1			"MTXPushInv():  NULL MtxStackPtr 'sPtr' "
#define	MTX_PUSHINV_2			"MTXPushInv():  'sPtr' contains a NULL ptr to stack memory "
#define	MTX_PUSHINV_3			"MTXPushInv():  NULL MtxPtr 'm' "
#define	MTX_PUSHINV_4			"MTXPushInv():  stack overflow"
      
#define	MTX_PUSHINVXPOSE_1		"MTXPushInvXpose():  NULL MtxStackPtr 'sPtr' "
#define	MTX_PUSHINVXPOSE_2		"MTXPushInvXpose():  'sPtr' contains a NULL ptr to stack memory "
#define	MTX_PUSHINVXPOSE_3		"MTXPushInvXpose():  NULL MtxPtr 'm' "
#define	MTX_PUSHINVXPOSE_4		"MTXPushInvXpose():  stack overflow "

#define	MTX_INVXPOSE_1		"MTXInvXpose():  NULL MtxStackPtr 'ptr' "
#define	MTX_INVXPOSE_3		"MTXInvXpose():  NULL MtxPtr 'm' "
#define	MTX_INVXPOSE_4		"MTXInvXpose():  stack overflow "

#define	MTX_POP_1				"MTXPop():  NULL MtxStackPtr 'sPtr' "
#define MTX_POP_2				"MTXPop():  'sPtr' contains a NULL ptr to stack memory "
		    
#define	MTX_GETSTACKPTR_1		"MTXGetStackPtr():  NULL MtxStackPtr 'sPtr' "
#define	MTX_GETSTACKPTR_2		"MTXGetStackPtr():  'sPtr' contains a NULL ptr to stack memory " 



//  VECTOR   SECTION

#define	VEC_ADD_1				"VECAdd():  NULL VecPtr 'a' "
#define	VEC_ADD_2				"VECAdd():  NULL VecPtr 'b' "
#define	VEC_ADD_3				"VECAdd():  NULL VecPtr 'ab' "
			
#define	VEC_SUBTRACT_1			"VECSubtract():  NULL VecPtr 'a' "
#define	VEC_SUBTRACT_2			"VECSubtract():  NULL VecPtr 'b' "
#define	VEC_SUBTRACT_3			"VECSubtract():  NULL VecPtr 'a_b' "
		
#define	VEC_SCALE_1				"VECScale():  NULL VecPtr 'src' "
#define	VEC_SCALE_2				"VECScale():  NULL VecPtr 'dst' "
		
#define	VEC_NORMALIZE_1			"VECNormalize():  NULL VecPtr 'src' "
#define	VEC_NORMALIZE_2			"VECNormalize():  NULL VecPtr 'unit' "
#define	VEC_NORMALIZE_3			"VECNormalize():  zero magnitude vector "
	
#define	VEC_MAG_1				"VECMag():  NULL VecPtr 'v' "

		
#define	VEC_REFLECT_1			"VECReflect():  NULL VecPtr 'src' "
#define	VEC_REFLECT_2			"VECReflect():  NULL VecPtr 'normal' "
#define	VEC_REFLECT_3			"VECReflect():  NULL VecPtr 'dst' "
		
#define	VEC_DOTPRODUCT_1		"VECDotProduct():  NULL VecPtr 'a' "
#define	VEC_DOTPRODUCT_2		"VECDotProduct():  NULL VecPtr 'b' "
	
#define	VEC_CROSSPRODUCT_1		"VECCrossProduct():  NULL VecPtr 'a' "
#define	VEC_CROSSPRODUCT_2		"VECCrossProduct():  NULL VecPtr 'b' "
#define	VEC_CROSSPRODUCT_3		"VECCrossProduct():  NULL VecPtr 'axb' "
	
#define	VEC_HALFANGLE_1			"VECHalfAngle():  NULL VecPtr 'a' "
#define	VEC_HALFANGLE_2			"VECHalfAngle():  NULL VecPtr 'b' "
#define	VEC_HALFANGLE_3			"VECHalfAngle():  NULL VecPtr 'half' "

//	Texture Projection Section

#define	MTX_LIGHT_FRUSTUM_1		"MTXLightFrustum():  NULL MtxPtr 'm' "
#define	MTX_LIGHT_FRUSTUM_2		"MTXLightFrustum():  't' and 'b' clipping planes are equal "
#define	MTX_LIGHT_FRUSTUM_3		"MTXLightFrustum():  'l' and 'r' clipping planes are equal "
#define	MTX_LIGHT_FRUSTUM_4		"MTXLightFrustum():  'n' and 'f' clipping planes are equal "
		
#define	MTX_LIGHT_PERSPECTIVE_1	"MTXLightPerspective():  NULL MtxPtr 'm' "
#define	MTX_LIGHT_PERSPECTIVE_2	"MTXLightPerspective():  'fovY' out of range "
#define	MTX_LIGHT_PERSPECTIVE_3	"MTXLightPerspective():  'aspect' is 0 "

#define	MTX_LIGHT_ORTHO_1		"MTXLightOrtho():  NULL MtxPtr 'm' "
#define	MTX_LIGHT_ORTHO_2		"MTXLightOrtho():  't' and 'b' clipping planes are equal "
#define	MTX_LIGHT_ORTHO_3		"MTXLightOrtho():  'l' and 'r' clipping planes are equal "	
   







#endif
