/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     control.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/control/src/control.c $
    
    7     8/18/00 2:42p Dante
    Added win32.h
    
    6     8/04/00 5:06p John
    Added CTRLBuildInverseMatrix, CTRLGet* functions.
    
    5     7/19/00 12:09p John
    Changed sin and cos variable names to nSin and nCos since these are
    #define'd in MAC build.
    
    4     7/18/00 7:30p John
    Optimized CTRL library to arithmetically build matrices.
    Changed CTRLSetType to CTRLInit.
    
    3     3/30/00 2:39p Ryan
    Update to change order of concatenation to X, Y, Z for Euler angles
    
    2     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <math.h>
#include <CharPipe/control.h>

#ifdef flagWIN32
#include <win32/win32.h>
#endif

/*---------------------------------------------------------------------*

Name:           CTRLSetScale

Description:    Adds scale to the given control.  The control will not
                be controlled by a matrix (if it used to be).

Arguments:      control - pointer to the control
                x, y, z - scale parameters

Return:         none

*---------------------------------------------------------------------*/
void CTRLSetScale ( CTRLControlPtr control, float x, float y, float z )
{
    // Set the scale bit
    CTRLEnable( control, CTRL_SCALE );

    // Negate the mtx bit, since final matrix will be yielded by
    // scale, translation, and/or rotation
    CTRLDisable( control, CTRL_MTX );

    // Set the scale parameters
    control->controlParams.srt.s.x = x;
    control->controlParams.srt.s.y = y;
    control->controlParams.srt.s.z = z;
}

/*---------------------------------------------------------------------*

Name:           CTRLSetRotation

Description:    Adds euler XYZ rotation to the given control.  The control
                will not be controlled by a matrix (if it used to be).

Arguments:      control - pointer to the control
                x, y, z - euler XYZ rotations in degrees

Return:         none

*---------------------------------------------------------------------*/
void CTRLSetRotation ( CTRLControlPtr control, float x, float y, float z )
{
    // Set the euler rotation bit
    CTRLEnable( control, CTRL_ROT_EULER );
    CTRLDisable( control, CTRL_ROT_QUAT );

    // Negate the mtx bit, since final matrix will be yielded by
    // scale, translation, and/or rotation
    CTRLDisable( control, CTRL_MTX );

    // Set the euler rotation parameters
    control->controlParams.srt.r.x = x;
    control->controlParams.srt.r.y = y;
    control->controlParams.srt.r.z = z;
}

/*---------------------------------------------------------------------*

Name:           CTRLSetQuat

Description:    Adds quaternion rotation to the given control.  The control
                will not be controlled by a matrix (if it used to be).

Arguments:      control    - pointer to the control
                x, y, z, w - quaternion parameters

Return:         none

*---------------------------------------------------------------------*/
void CTRLSetQuat ( CTRLControlPtr control, float x, float y, float z, float w )
{
    // Set the quaternion rotation bit
    CTRLEnable( control, CTRL_ROT_QUAT );
    CTRLDisable( control, CTRL_ROT_EULER );

    // Negate the mtx bit, since final matrix will be yielded by
    // scale, translation, and/or rotation
    CTRLDisable( control, CTRL_MTX );

    // Set the euler rotation parameters
    control->controlParams.srt.r.x = x;
    control->controlParams.srt.r.y = y;
    control->controlParams.srt.r.z = z;
    control->controlParams.srt.r.w = w;
}

/*---------------------------------------------------------------------*

Name:           CTRLSetTranslation

Description:    Adds translation to the given control.  The control will
                not be controlled by a matrix (if it used to be).

Arguments:      control - pointer to the control
                x, y, z - translation parameters

Return:         none

*---------------------------------------------------------------------*/
void CTRLSetTranslation ( CTRLControlPtr control, float x, float y, float z )
{
    // Set the translation bit
    CTRLEnable( control, CTRL_TRANS );

    // Negate the mtx bit, since final matrix will be yielded by
    // individual scale, translation, and rotation
    CTRLDisable( control, CTRL_MTX );

    // Set the translation parameters
    control->controlParams.srt.t.x = x;
    control->controlParams.srt.t.y = y;
    control->controlParams.srt.t.z = z;
}

/*---------------------------------------------------------------------*

Name:           CTRLSetMatrix

Description:    The given control will be controlled by the given matrix.

Arguments:      control - pointer to the control
                m       - matrix to use in this control

Return:         none

*---------------------------------------------------------------------*/
void CTRLSetMatrix ( CTRLControlPtr control, Mtx m )
{
    // Set the matrix bit (clears scale, rotation, and translation bits)
    control->type = CTRL_MTX;

    // Copy the matrix parameters
    MTXCopy( m, control->controlParams.mtx.m );
}

/*---------------------------------------------------------------------*

Name:           CTRLGetScale

Description:    Returns the current scale in the given control.

Arguments:      control - pointer to the control
                x, y, z - scale parameter return values

Return:         none

*---------------------------------------------------------------------*/
void CTRLGetScale( CTRLControlPtr control, float *x, float *y, float *z )
{
    ASSERT( control && x && y && z );
    ASSERT( control->type & CTRL_SCALE );

    *x = control->controlParams.srt.s.x;
    *y = control->controlParams.srt.s.y;
    *z = control->controlParams.srt.s.z;
}

/*---------------------------------------------------------------------*

Name:           CTRLGetRotation

Description:    Returns the current euler XYZ rotation in the given control.

Arguments:      control - pointer to the control
                x, y, z - euler XYZ rotation parameter return values

Return:         none

*---------------------------------------------------------------------*/
void CTRLGetRotation( CTRLControlPtr control, float *x, float *y, float *z )
{
    ASSERT( control && x && y && z );
    ASSERT( control->type & CTRL_ROT_EULER );

    *x = control->controlParams.srt.r.x;
    *y = control->controlParams.srt.r.y;
    *z = control->controlParams.srt.r.z;
}

/*---------------------------------------------------------------------*

Name:           CTRLGetQuat

Description:    Returns the current quaternion rotation in the given control.

Arguments:      control - pointer to the control
                x, y, z - quaternion rotation parameter return values

Return:         none

*---------------------------------------------------------------------*/
void CTRLGetQuat( CTRLControlPtr control, float *x, float *y, float *z, float *w )
{
    ASSERT( control && x && y && z && w );
    ASSERT( control->type & CTRL_ROT_QUAT );

    *x = control->controlParams.srt.r.x;
    *y = control->controlParams.srt.r.y;
    *z = control->controlParams.srt.r.z;
    *w = control->controlParams.srt.r.w;
}

/*---------------------------------------------------------------------*

Name:           CTRLGetTranslation

Description:    Returns the current translation in the given control.

Arguments:      control - pointer to the control
                x, y, z - translation parameter return values

Return:         none

*---------------------------------------------------------------------*/
void CTRLGetTranslation( CTRLControlPtr control, float *x, float *y, float *z )
{
    ASSERT( control && x && y && z );
    ASSERT( control->type & CTRL_TRANS );

    *x = control->controlParams.srt.t.x;
    *y = control->controlParams.srt.t.y;
    *z = control->controlParams.srt.t.z;
}

/*---------------------------------------------------------------------*

Name:           CTRLBuildMatrix

Description:    Yields a matrix for the given control.  Arithmetically
                and selectively builds for control types other than
                CTRL_MTX.

Arguments:      control - pointer to the control
                m       - return value matrix

Return:         none, but matrix is set through the argument m.

*---------------------------------------------------------------------*/
void CTRLBuildMatrix ( CTRLControlPtr control, Mtx m )
{
    f32 nRad, nSin, nCos, temp;

    // Return the matrix
    if( control->type & CTRL_MTX )
    {
        MTXCopy(control->controlParams.mtx.m, m);

        return;
    }

    // Include the rotation component
    if( control->type & CTRL_ROT_QUAT )
    {
        MTXQuat( m, &(control->controlParams.srt.r) );
    }
    else if( control->type & CTRL_ROT_EULER )
    {
        // Rotate by x axis
        if( control->controlParams.srt.r.x != 0.0f )
        {
            nRad = MTXDegToRad( control->controlParams.srt.r.x );
            nSin = sinf( nRad );
            nCos = cosf( nRad );

            MTXRowCol(m, 0, 0) = 1.0f;
            MTXRowCol(m, 0, 1) = 0.0f;
            MTXRowCol(m, 0, 2) = 0.0f;
            MTXRowCol(m, 0, 3) = 0.0f;

            MTXRowCol(m, 1, 0) = 0.0f;
            MTXRowCol(m, 1, 1) = nCos;
            MTXRowCol(m, 1, 2) = -nSin;
            MTXRowCol(m, 1, 3) = 0.0f;

            MTXRowCol(m, 2, 0) = 0.0f;
            MTXRowCol(m, 2, 1) = nSin;
            MTXRowCol(m, 2, 2) = nCos;
            MTXRowCol(m, 2, 3) = 0.0f;
        }
        else
        {
            MTXIdentity( m );
        }

        // Rotate by y axis
        if( control->controlParams.srt.r.y != 0.0f )
        {
            nRad = MTXDegToRad( control->controlParams.srt.r.y );
            nSin = sinf( nRad );
            nCos = cosf( nRad );

            MTXRowCol(m, 0, 0) = nCos;
            MTXRowCol(m, 0, 1) = MTXRowCol(m, 2, 1) * nSin;
            MTXRowCol(m, 0, 2) = MTXRowCol(m, 2, 2) * nSin;

            MTXRowCol(m, 2, 0) = -nSin;
            MTXRowCol(m, 2, 1) *= nCos;
            MTXRowCol(m, 2, 2) *= nCos;
        }

        // Rotate by z axis
        if( control->controlParams.srt.r.z != 0.0f )
        {
            nRad = MTXDegToRad( control->controlParams.srt.r.z );
            nSin = sinf( nRad );
            nCos = cosf( nRad );

            MTXRowCol(m, 1, 0) = MTXRowCol(m, 0, 0) * nSin;
            MTXRowCol(m, 0, 0) *= nCos;

            temp = MTXRowCol(m, 1, 1);
            MTXRowCol(m, 1, 1) = (MTXRowCol(m, 0, 1) * nSin) + (MTXRowCol(m, 1, 1) * nCos);
            MTXRowCol(m, 0, 1) = (MTXRowCol(m, 0, 1) * nCos) - (temp * nSin);

            temp = MTXRowCol(m, 1, 2);
            MTXRowCol(m, 1, 2) = (MTXRowCol(m, 0, 2) * nSin) + (MTXRowCol(m, 1, 2) * nCos);
            MTXRowCol(m, 0, 2) = (MTXRowCol(m, 0, 2) * nCos) - (temp * nSin);
        }
    }
    else
        MTXIdentity( m );

    // Include the scale (pre-rotation)
    if( control->type & CTRL_SCALE )
    {
        MTXRowCol(m, 0, 0) *= control->controlParams.srt.s.x;
        MTXRowCol(m, 1, 0) *= control->controlParams.srt.s.x;
        MTXRowCol(m, 2, 0) *= control->controlParams.srt.s.x;

        MTXRowCol(m, 0, 1) *= control->controlParams.srt.s.y;
        MTXRowCol(m, 1, 1) *= control->controlParams.srt.s.y;
        MTXRowCol(m, 2, 1) *= control->controlParams.srt.s.y;

        MTXRowCol(m, 0, 2) *= control->controlParams.srt.s.z;
        MTXRowCol(m, 1, 2) *= control->controlParams.srt.s.z;
        MTXRowCol(m, 2, 2) *= control->controlParams.srt.s.z;
    }

    // Include the translation (4th column of matrix is zero-vector)
    if( control->type & CTRL_TRANS )
    {
        MTXRowCol(m, 0, 3) = control->controlParams.srt.t.x;
        MTXRowCol(m, 1, 3) = control->controlParams.srt.t.y;
        MTXRowCol(m, 2, 3) = control->controlParams.srt.t.z;
    }
}


/*---------------------------------------------------------------------*

Name:           CTRLBuildInverseMatrix

Description:    Yields an inverse matrix for the given control.  You will
                save a MTXInverse if the type of control is not CTRL_MTX
                since the inverse matrix will be built arithmetically.

Arguments:      control - pointer to the control
                m       - return value matrix

Return:         none, but matrix is set through the argument m.

*---------------------------------------------------------------------*/
void CTRLBuildInverseMatrix ( CTRLControlPtr control, Mtx m )
{
    f32 nRad, nSin, nCos, temp;
    Quaternion q;

    // Return the matrix
    if( control->type & CTRL_MTX )
    {
        MTXInverse(control->controlParams.mtx.m, m);

        return;
    }

    // Include the inverse rotation
    if( control->type & CTRL_ROT_QUAT )
    {
        // Compute the multiplicative inverse (must be unit quaternion)
        q.x = -control->controlParams.srt.r.x;
        q.y = -control->controlParams.srt.r.y;
        q.z = -control->controlParams.srt.r.z;
        q.w = -control->controlParams.srt.r.w;

        MTXQuat( m, &q );
    }
    else if( control->type & CTRL_ROT_EULER )
    {
        // Rotate by z axis in the opposite direction
        if( control->controlParams.srt.r.z != 0.0f )
        {
            nRad = MTXDegToRad( -control->controlParams.srt.r.x );
            nSin = sinf( nRad );
            nCos = cosf( nRad );

            MTXRowCol(m, 0, 0) = nCos;
            MTXRowCol(m, 0, 1) = -nSin;
            MTXRowCol(m, 0, 2) = 0.0f;
            MTXRowCol(m, 0, 3) = 0.0f;

            MTXRowCol(m, 1, 0) = nSin;
            MTXRowCol(m, 1, 1) = nCos;
            MTXRowCol(m, 1, 2) = 0.0f;
            MTXRowCol(m, 1, 3) = 0.0f;

            MTXRowCol(m, 2, 0) = 0.0f;
            MTXRowCol(m, 2, 1) = 0.0f;
            MTXRowCol(m, 2, 2) = 1.0f;
            MTXRowCol(m, 2, 3) = 0.0f;
        }
        else
        {
            MTXIdentity( m );
        }

        // Rotate by y axis in the opposite direction
        if( control->controlParams.srt.r.y != 0.0f )
        {
            nRad = MTXDegToRad( -control->controlParams.srt.r.y );
            nSin = sinf( nRad );
            nCos = cosf( nRad );

            MTXRowCol(m, 2, 0) = MTXRowCol(m, 0, 0) * -nSin;
            MTXRowCol(m, 2, 1) = MTXRowCol(m, 0, 1) * -nSin;
            MTXRowCol(m, 2, 2) = nCos;

            MTXRowCol(m, 0, 0) *= nCos;
            MTXRowCol(m, 0, 1) *= nCos;
            MTXRowCol(m, 0, 2) = nSin;
        }

        // Rotate by x axis in the opposite direction
        if( control->controlParams.srt.r.x != 0.0f )
        {
            nRad = MTXDegToRad( -control->controlParams.srt.r.x );
            nSin = sinf( nRad );
            nCos = cosf( nRad );

            MTXRowCol(m, 1, 2) = MTXRowCol(m, 2, 2) * -nSin;
            MTXRowCol(m, 2, 2) *= nCos;

            temp = MTXRowCol(m, 1, 0);
            MTXRowCol(m, 1, 0) = (MTXRowCol(m, 1, 0) * nCos) - (MTXRowCol(m, 2, 0) * nSin);
            MTXRowCol(m, 2, 0) = (MTXRowCol(m, 2, 0) * nCos) + (temp * nSin);

            temp = MTXRowCol(m, 1, 1);
            MTXRowCol(m, 1, 1) = (MTXRowCol(m, 1, 1) * nCos) - (MTXRowCol(m, 2, 1) * nSin);
            MTXRowCol(m, 2, 1) = (MTXRowCol(m, 2, 1) * nCos) + (temp * nSin);
        }
    }
    else
    {
        MTXIdentity( m );
    }

    // Include the inverse translation (pre-rotation)
    if( control->type & CTRL_TRANS )
    {
        if( control->type & CTRL_ROT_QUAT || control->type & CTRL_ROT_EULER )
        {
            // Since inverse translation is pre-rotation, we must do more work
            MTXRowCol(m, 0, 3) = -((MTXRowCol(m, 0, 0) * control->controlParams.srt.t.x) +
                                   (MTXRowCol(m, 0, 1) * control->controlParams.srt.t.y) +
                                   (MTXRowCol(m, 0, 2) * control->controlParams.srt.t.z));
            MTXRowCol(m, 1, 3) = -((MTXRowCol(m, 1, 0) * control->controlParams.srt.t.x) +
                                   (MTXRowCol(m, 1, 1) * control->controlParams.srt.t.y) +
                                   (MTXRowCol(m, 1, 2) * control->controlParams.srt.t.z));
            MTXRowCol(m, 2, 3) = -((MTXRowCol(m, 2, 0) * control->controlParams.srt.t.x) +
                                   (MTXRowCol(m, 2, 1) * control->controlParams.srt.t.y) +
                                   (MTXRowCol(m, 2, 2) * control->controlParams.srt.t.z));
        }
        else
        {
            // No rotation, so inverse translation is easier to set
            MTXRowCol(m, 0, 3) = -control->controlParams.srt.t.x;
            MTXRowCol(m, 1, 3) = -control->controlParams.srt.t.y;
            MTXRowCol(m, 2, 3) = -control->controlParams.srt.t.z;
        }
    }

    // Include the scale
    if( control->type & CTRL_SCALE )
    {
        // Compute the inverse scale
        q.x = 1.0f / control->controlParams.srt.s.x;
        q.y = 1.0f / control->controlParams.srt.s.y;
        q.z = 1.0f / control->controlParams.srt.s.z;

        MTXRowCol(m, 0, 0) *= q.x;
        MTXRowCol(m, 0, 1) *= q.x;
        MTXRowCol(m, 0, 2) *= q.x;
        MTXRowCol(m, 0, 3) *= q.x;

        MTXRowCol(m, 1, 0) *= q.y;
        MTXRowCol(m, 1, 1) *= q.y;
        MTXRowCol(m, 1, 2) *= q.y;
        MTXRowCol(m, 1, 3) *= q.y;

        MTXRowCol(m, 2, 0) *= q.z;
        MTXRowCol(m, 2, 1) *= q.z;
        MTXRowCol(m, 2, 2) *= q.z;
        MTXRowCol(m, 2, 3) *= q.z;
    }
}
