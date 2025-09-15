/* Copyright (c) 1998 Slingshot Game Technology

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
// DEM.cpp	jbl 09/07/98	Copyright Slingshot

// Code for the DEM class, which represents data read from a USGS
// Digital Elevation Model.  See documentation in ShredDEM.cpp.
// Also the DEMBag class, which holds a collection of DEMs.

#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <math.h>	// for fmod()
#include "dem.hpp"


using namespace std;


UTMVector	RegionSWCorner, RegionNECorner;


DEM::DEM()
// Constructor.
{
	ZSize = 0;
	XSize = 0;

//	ZOriginDistance = 0;
//	XOriginDistance = 0;

	Data = NULL;
}


void	DEM::SetSize(int xsize, int zsize)
// Sets the size of the DEM array, and allocates & initializes the array.
{
	if (Data) {
		if (xsize == XSize && zsize == ZSize) {
			// No change, no problem.
			return;
		}
		
		cout << "DEM::SetSize(" << xsize << ", " << zsize << ") Data already allocated as (" << XSize << ", " << ZSize << ")." << endl;
		exit(-1);
	}
	
	XSize = xsize;
	ZSize = zsize;
	
	Data = new int[ZSize * XSize];
	if (!Data) {
		cout << "Unable to allocate DEM Data[" << ZSize << "][" << XSize << "]" << endl;
		exit(-1);
	}
	
	// Initialize the Data to UNSET.
	for (int i=0; i < ZSize*XSize; i++) {
		Data[i] = UNSET;
	}
}


void	DEM::SetDomain(const UTMVector& LowerLeft, double XResolution, double YResolution)
// Sets the domain of the DEM array.
// GetValue() queries are linearly warped into the array space using these domain
// parameters, in order to accurately represent the data.
{
	Origin = LowerLeft;
	XRes = XResolution;
	YRes = YResolution;
	
//	//xxxxxxx
//	XOriginDistance = LowerLeft.X();
//	ZOriginDistance = LowerLeft.Y() + YRes * (ZSize - 1);
//	//xxxxxxx
}

	
void	DEM::SetValue(int iz, int ix, int Val)
// Set the elevation value of an individual point in a DEM, at the specified
// indices.
{
	if (Data == NULL) {
		cout << "DEM::SetValue(): Data not allocated." << endl;
		exit(-1);
	}

	//xxxxx
	if (Val == 0) {
		cout << "DEM::SetValue to 0, ix = " << ix << ", iz = " << iz;
	}
	
	Data[iz * XSize + ix] = Val;
}


// In GetValue() it can happen that the requested point is in the seam between
// two DEMs.  We can't do the interpolation within one DEM because some of the
// corner values (UpperLeft, ..., LowerRight) will be missing.  So we save the
// corner values that we have and trust that another DEM will fill in the
// missing pieces.  After all the DEMs have been queried, the DEMBag can
// complete the interpolation.
// 
// Create one Pieces structure for communication 'tween da dembag 'n da dems.

#define UL	0	// indices for Corners[]; UpperLeft...
#define UR	1
#define LL	2
#define LR	3

static struct {
	bool	Missing;	// true if the DEM found some values missing

	float	Corners[4];	// the pieces
	float	Weight[4];
	
	int	nPieces;	// number of values in the average
} Pieces;


float	DEM::GetValue(double XDistance, double ZDistance)
// Return the elevation value at the distance coordinates (UTM, meters).
// If it's outside our set, return UNSET.
{
	if (Data == NULL) {
		cout << "DEM::SetValue(): Data not allocated." << endl;
		exit(-1);
	}
	
//	int	Pitch = DEM_PITCH;
	float	Val = UNSET;
	int	i;

	// Convert query to DEM index coordinates.
	double	cx, cy;
	cx = (XDistance - Origin.X()) / XRes;
	cy = (ZDistance - Origin.Y()) / YRes;
	cy = (ZSize - 1) - cy;
	
	// Get our closest point indices, rounded down.
	
	int ix = (int) floor(cx);
	int iz = (int) floor(cy);
	if (ix < -1 || iz < -1 || ix >= XSize || iz >= ZSize) {
		return(UNSET);
	}

	// Fraction of distance to the next point.
	float dx = cx - ix;
	float dz = cy - iz;
	if (dx < 0 || dz < 0 || dx > 1 || dz > 1) {	// an assertion
		cout << "Bad dx or dz in DEM::GetValue(). dx = " << dx << ", dz = " << dz << endl;
		exit(-1);
	}

	float Corners[4];
	for (i=0; i < 4; i++) {
		Corners[i] = UNSET;
	}

	// Fill the Corners[] array with data.
	if (ix >= 0 && ix < XSize) {
		if (iz >= 0 && iz < ZSize) {
			Corners[UL] = Data[iz * XSize + ix];
		}
		if (iz + 1 >= 0 && iz + 1 < ZSize) {
			Corners[LL] = Data[(iz+1) * XSize + ix];
		}
	}
	if (ix + 1 >= 0 && ix + 1 < XSize) {
		if (iz >= 0 && iz < ZSize) {
			Corners[UR] = Data[iz * XSize + (ix+1)];
		}
		if (iz + 1 >= 0 && iz + 1 < ZSize) {
			Corners[LR] = Data[(iz+1) * XSize + (ix+1)];
		}
	}

	// Check for missing pieces.  Note that zero is a valid value.
	// If ALL the corner pieces are missing, that's not a seam
	// problem, it's a point entirely outside this DEM, probably near
	// a jaggy edge.  A seam is when 1 to 3 of the pieces are missing.
	int nCornersMissing = 0;
	for (i=0; i < 4; i++) {
		if (Corners[i] == UNSET) nCornersMissing++;
	}
	if (nCornersMissing == 0) {
		// A normal good point.  Complete the interpolation.
		float Left = Corners[UL] + dz * (Corners[LL] - Corners[UL]);
		float Right = Corners[UR] + dz * (Corners[LR] - Corners[UR]);
		Val = Left + dx * (Right - Left);
		
	} else if (nCornersMissing == 4) {
		// A point entirely outside this DEM.
		Val = UNSET;

		//cout << "DEM::GetValue returning UNSET, point entirely outside us" << endl;
	} else {
		// A seam point, partially inside this DEM.
		Pieces.Missing = true;
		for (i=0; i < 4; i++) {
			if (Corners[i] != UNSET) {
				// I wanna see whether there's overlap at
				// the seams, a corner owned by two DEMs.
				if (Pieces.Corners[i] != UNSET) {
					cout << ">>>Overlap at corner " << i << ". Z,XDistance= " << ZDistance << ", " << XDistance << ". This DEM's Origin = " << Origin.X() << ", " << Origin.Y() << endl;
					cout << ">> Previous value = " << Pieces.Corners[i] << ", this value = " << Corners[i] << endl;
				}
				Pieces.Corners[i] = Corners[i];
				Pieces.Weight[i] = ((i == UL || i == LL) ? (1 - dx) : dx) * ((i == UL || i == UR) ? (1 - dz) : dz);
			}
			if (Pieces.Corners[i] != UNSET) Pieces.nPieces++;
		}

		// Return a nongood indicator.
		Val = UNSET;
	}

	return (Val);
}


#ifdef NOT
void	DEM::SetRegion(double z0, double x0, double z1, double x1)
// Remember the corners of the desired region in this DEM's coordinates.
{
	// Find our nearest corner points that enclose the region.
	// It's okay to compute the MinValue with a region a bit big.
	// E.g., for x0, find the array index such that
	// DEM's origin + pitch * index <= x0.
	iX0Region = ((x0 - Origin.X())) / DEM_PITCH;
	iZ0Region = (-1 * (z0 - (Origin.Y() + YRes * (ZSize-1)))) / DEM_PITCH;
	// For the high ends, round up.
	iX1Region = ((x1 - Origin.X()) + DEM_PITCH) / DEM_PITCH;
	iZ1Region = (-1 * (z1 - (Origin.Y() + YRes * (ZSize-1))) + DEM_PITCH) / DEM_PITCH;

	// Note:  We don't clip the region to our DEM edges here.  We don't
	// want Preprocess() to fade at DEM edges, only at the real
	// region edges.  GetMinValue() does care about clipping, but it can
	// do its own.
}
#endif // NOT


int	DEM::GetMinValue()
// Return the minvalue for the specified region.
{
	if (Data == NULL) {
		cout << "DEM::SetValue(): Data not allocated." << endl;
		exit(-1);
	}
	
	int MinValue = INT_MAX;

	// Only consider points within the output region.
	int	iz0 = (int) ((Origin.Y() - RegionNECorner.Y()) / YRes + (ZSize-1));
	int	iz1 = (int) ((Origin.Y() - RegionSWCorner.Y()) / YRes + (ZSize-1));
	int	ix0 = (int) ((RegionSWCorner.X() - Origin.X()) / XRes);
	int	ix1 = (int) ((RegionNECorner.X() - Origin.X()) / XRes);
	if (iz0 < 0) iz0 = 0;
	if (iz1 > ZSize) iz1 = ZSize;
	if (ix0 < 0) ix0 = 0;
	if (ix1 > XSize) ix1 = XSize;

	for (int z = iz0; z < iz1; z++) {
		for (int x = ix0; x < ix1; x++) {
			int Value = Data[z * XSize + x];
			if (Value > 0 && Value < MinValue) {
				MinValue = Value;
			}
		}
	}

//	if (MinValue == INT_MAX) {
//		cout << "Internal error in DEM::GetMinValue()!" << endl;
//		exit(-1);
//	}

	return(MinValue);
}


void	DEM::ApplyCurveOfTheEarth(const UTMVector& center, float EarthRadius)
// Applies a correction for the curve of the earth.
{
	if (Data == NULL) {
		cout << "DEM::ApplyCurveOfTheEarth(): Data not allocated." << endl;
		exit(-1);
	}

	float	EarthRadius2 = EarthRadius * EarthRadius;
	
	float	X, Z;
	
	Z = Origin.Y() + (ZSize - 1) * YRes - center.Y();
	for (int iz = 0; iz < ZSize; iz++, Z -= YRes) {
		X = Origin.X() - center.X();
		for (int ix = 0; ix < XSize; ix++, X += XRes) {
			int Val = Data[iz * XSize + ix];

			// Ignore missing points.
			if (Val == UNSET) {
//				Val = BaseElevation;
				continue;
			}
			
			// Compute correction.
			float	d2 = X * X + Z * Z;
			float	delta = EarthRadius - sqrt(EarthRadius2 - d2);

			// Apply it.
			Val -= (int) delta;
			Data[iz * XSize + ix] = Val;
		}
	}
}


void	DEM::Preprocess(int BaseElevation, int ZeroBand, int FadeBand)
// Subtract the base elevation and fades the edges of the region to zero.
// This is called after all the DEMs are read but before they're written to
// grids.
{
	if (Data == NULL) {
		cout << "DEM::SetValue(): Data not allocated." << endl;
		exit(-1);
	}
	
	int d, dmin;

	// Clip the region to our DEM size for the loop limits.
	// But keep the region limits, where fading is done.

	int	iZ0Region = (int) ((Origin.Y() - RegionNECorner.Y()) / YRes + (ZSize-1));
	int	iZ1Region = (int) ((Origin.Y() - RegionSWCorner.Y()) / YRes + (ZSize-1));
	int	iX0Region = (int) ((RegionSWCorner.X() - Origin.X()) / XRes);
	int	iX1Region = (int) ((RegionNECorner.X() - Origin.X()) / XRes);
	
	int	iz0 = iZ0Region;
	int	iz1 = iZ1Region;
	int	ix0 = iX0Region;
	int	ix1 = iX1Region;
	if (iz0 < 0) iz0 = 0;
	if (iz1 > ZSize) iz1 = ZSize;
	if (ix0 < 0) ix0 = 0;
	if (ix1 > XSize) ix1 = XSize;

	for (int iz = 0; iz < ZSize; iz++) {
		for (int ix = 0; ix < XSize; ix++) {
			int Val = Data[iz * XSize + ix];

			// Ignore missing points.
			if (Val == UNSET) {
//				Val = BaseElevation;
				continue;
			}
			
			if (Val == 0) {
				// Shouldn't happen, missing data should be UNSET.
				// I wanna see it.
				cout << "Zero value in Preprocess!" << endl;
				Val = BaseElevation;	//xxxxxxxxx
			}

			// Subtract out base elevation.
			Val -= BaseElevation;
			if (Val < 0) {
				Val = 0;
				Data[iz * XSize + ix] = Val;
//				// I wanna see it.
//				cout << "Warning:  value < base!" << endl;
				continue;
			}

			// Then the zero band.
			if ((ix - iX0Region) < ZeroBand ||
			    (iX1Region - ix - 1) < ZeroBand ||
			    (iz - iZ0Region) < ZeroBand ||
			    (iZ1Region - iz - 1) < ZeroBand)
			{
				Val = 0;
				Data[iz * XSize + ix] = Val;
				continue;
			}

			d = dmin = INT_MAX;

			if ((ix - iX0Region) < FadeBand + ZeroBand) {
				d = ix - iX0Region - ZeroBand; // left edge
			} else if ((iX1Region - ix - 1) < FadeBand + ZeroBand) {
				d = iX1Region - ix - 1 - ZeroBand; // right edge
			}
			if (d < dmin) dmin = d;	// distance from closest edge

			if ((iz - iZ0Region) < FadeBand + ZeroBand) {
				d = iz - iZ0Region - ZeroBand; // left edge
			} else if ((iZ1Region - iz - 1) < FadeBand + ZeroBand) {
				d = iZ1Region - iz - 1 - ZeroBand; // right edge
			}
			if (d < dmin) dmin = d;	// distance from closest edge

			if (dmin < FadeBand) {
				Val = (int) (Val * (float(dmin) / FadeBand));
			}

			Data[iz * XSize + ix] = Val;
		}
	}
}


DEMBag::DEMBag()
{
	nDEMS = 0;
}


void	DEMBag::Add(DEM* pDEM)
// Add a DEM to the bag.
{
	if (nDEMS >= MAX_DEMS) {
		cout << "Max number of DEMS is " << MAX_DEMS << endl;
		exit(-1);
	}

	DEMs[nDEMS] = pDEM;
	nDEMS++;
}
 

float	DEMBag::GetValue(double XDistance, double ZDistance)
// Get the elevation at a point in UTM space.
// Ask each DEM whether it has a nonzero value at the point.
{
	// Check to see if the request is within the region-of-interest.  If not, just return 0.
	if (XDistance < RegionSWCorner.X() || XDistance > RegionNECorner.X() ||
	    ZDistance < RegionSWCorner.Y() || ZDistance > RegionNECorner.Y())
	{
		// Query outside region.
		return 0;
	}
	
	int	i;
	float	Val;

	// Reset the missing-pieces structure.
	Pieces.Missing = false;
	for (i=0; i < 4; i++) {
		Pieces.Corners[i] = UNSET;
		Pieces.Weight[i] = 0;
	}
	Pieces.nPieces = 0;

	// Ask the DEMs.
	for (i=0; i < nDEMS; i++) {
		Val = DEMs[i]->GetValue(XDistance, ZDistance);
		if (Val != UNSET) break;	// got a good value, can quit asking
	}

	if (Val == UNSET && !Pieces.Missing) {	// assertion
		// Was the point in the seam between DEMs?  The DEMs would have
		// stored the corner values in the Pieces structure and set
		// the Missing flag.
		
		// Wanna see it.  This is abnormal.
		cout << "Val == UNSET but not Pieces.Missing!  Z,XDistance=" << ZDistance << ", " << XDistance << endl;
//		exit(-4);
		Val = 0;
		
	} else if (Val != UNSET && Pieces.Missing) {
		// This case is not abnormal.  A prior DEM could've caught a corner,
		// and a later DEM got a full point.
		
//		// wanna see it
//		cout << "Val not UNSET and Pieces.Missing, ok.  Val = " << Val << " Z,XDistance = " << ZDistance << ", " << XDistance << endl;
//		cout << "Corners = " << Pieces.Corners[0] << ", " << Pieces.Corners[1] << ", " << Pieces.Corners[2] << ", " << Pieces.Corners[3] << endl;

	} else if (Val == UNSET && Pieces.Missing) {	// the normal seam case
		// Yes.  Complete the interpolation now.
		// Unless some piece is still missing.
		bool PieceStillMissing = false;
		for (i=0; i < 4; i++) {
			if (Pieces.Corners[i] == UNSET) {
				PieceStillMissing = true;
				break;
			}
		}
		if (PieceStillMissing) {
			// I wanna see it.
			// Darn.  It happened in the 4-way seam, the
			// central intersection of all the DEMs.  Was
			// missing the UpperLeft value.  Means the upper-left
			// DEM was missing an edge point? or the alignment
			// is simply a little off?
			// Quick-patch it by filling in the average of the
			// other corner points, and emit a warning.
			// I'll look at the point manually later.
			Val = 0;
			int nPieces = 0;
			float	TotalWeight = 0;
			for (i=0; i < 4; i++) {
				if (Pieces.Corners[i] != UNSET) {
					Val += Pieces.Corners[i] /* * Pieces.Weight[i] */;
					TotalWeight += Pieces.Weight[i];
					nPieces++;
				}
			}
//			if (TotalWeight > 0) {
//				Val /= (TotalWeight);
//			} else {
//				Val = 0;
//			}
			Val /= nPieces;

			cout << ">>>Missing values in Pieces.  Supplied avg value " << Val << endl;
			cout << ">> Z,XDistance = " << ZDistance << ", " << XDistance << endl;
			cout << ">> Corners[UL,UR,LL,LR] = ";
			for (i=0; i < 4; i++) {
				cout << Pieces.Corners[i] << " ";
			}
			cout << endl;
		} else {
			Val = 0;
			float	TotalWeight = 0;
			for (i = 0; i < 4; i++) {
				Val += Pieces.Corners[i] /* * Pieces.Weight[i] */;
				TotalWeight += Pieces.Weight[i];
			}
//			if (TotalWeight > 0) {
//				Val /= TotalWeight;
//			} else {
//				Val = 0;
//			}
			Val /= 4;

			cout << "Seam w/ 4 valid corners.  Val = " << Val << endl;//xxxxx
		}
	}

	if (Val == UNSET) {
		// I wanna see it.
		cout << "Internal error in DEMBag::GetValue, UNSET value" << endl;
		exit(-3);
	}
	return(Val);
}


int	DEMBag::GetMinValue()
// Return the minimum elevation for the region.
{
	int MinValue = INT_MAX;

	for (int i=0; i < nDEMS; i++) {
		int ThisMinValue = DEMs[i]->GetMinValue();
		if (ThisMinValue < MinValue) {
			MinValue = ThisMinValue;
		}
	}

	// This could happen if the region doesn't intersect any DEMs.
	if (MinValue == INT_MAX) {
		cout << "Internal error in DEMBag::GetMinValue()!" << endl;
		exit(-1);
	}

	return(MinValue);
}


void	DEMBag::ApplyCurveOfTheEarth(const UTMVector& center, float EarthRadius)
// Modifies all DEM points to correct for the curve of the earth.  The
// specified center point is defined to be the point where no correction
// is required.
{
	for (int i=0; i < nDEMS; i++) {
		DEMs[i]->ApplyCurveOfTheEarth(center, EarthRadius);
	}
}


void	DEMBag::Preprocess(int BaseElevation, int ZeroBand, int FadeBand)
// Tell all the DEMs to preprocess themselves.  Subtract out the base
// elevation and fade the edges of the region.
{
	for (int i=0; i < nDEMS; i++) {
		DEMs[i]->Preprocess(BaseElevation, ZeroBand, FadeBand);
	}
}


void	DEMBag::SetRegion(double x0, double z0, double x1, double z1)
// Set the corners of the output region.
{
//	for (int i=0; i < nDEMS; i++) {
//		DEMs[i]->SetRegion(z0, x0, z1, x1);
//	}

	RegionSWCorner = UTMVector(x0, z0);
	RegionNECorner = UTMVector(x1, z1);
}

