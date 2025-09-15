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
// Grid.cpp	jbl 09/07/98	Copyright Slingshot

// Code for the Grid class, which represents a height-field model
// that's been converted from a DEM, ready for writing to a SoulRide
// terrain file.  See the documentation in ShredDEM.cpp.
// Also for the GridBag class, which is a collection of Grids, usually 17.
// Mainly it knows how to write its grids to a file. 


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>	// for fmod()
#include "grid.hpp"

using namespace std;


UTMVector	GridIndexOrigin;


void	Grid::SetGridIndexOrigin(const UTMVector& NWCorner)
// Specify the UTM location of the 0,0 integral grid-index origin.
{
	GridIndexOrigin = NWCorner;
}

   
Grid::Grid(int scale, int xsize, int zsize, const UTMVector& NWCorner)
// Constructor.
{
	Pitch = 1 << scale;
	Scale = scale;
	ZSize = zsize;
	XSize = xsize;

	XIndexOrigin = int(NWCorner.X() - GridIndexOrigin.X()) & ~(Pitch - 1);
	ZIndexOrigin = int(GridIndexOrigin.Y() - NWCorner.Y()) & ~(Pitch - 1);
	
//	ZOriginDistance = zorigindistance;
//	XOriginDistance = xorigindistance;

	Data = new float[ZSize * XSize];
	if (!Data) {
		cout << "Unable to allocate Grid Data[" << ZSize << "][" << XSize << "]" << endl;
		exit(-1);
	}
	// Init the Data.
	for (int i=0; i < ZSize*XSize; i++) {
		Data[i] = UNSET;
	}
}


void	Grid::GetCoords(UTMVector* result, int xindex, int zindex)
// Given x and z indices, computes the UTM coordinates of the corresponding
// point, and puts it in *result.
{
	result->SetX((xindex * Pitch + XIndexOrigin) + GridIndexOrigin.X());
	result->SetY((zindex * Pitch + ZIndexOrigin) * -1 + GridIndexOrigin.Y());
}


void	Grid::SetValue(int x, int z, float Val)
// Set an elevation value at the specified indices.
{
	if (Val < -100 || Val > 3000) {
		cout << "Setting insane value: " << Val << " at (" << x << ", " << z << ")." << endl;
	}
	
	Data[z * XSize + x] = Val;
}


void	Grid::Preprocess()
// Make sure all the grid's edge values are zero.
{
	int iz, ix;

	// Top edge.
	for (ix = 0; ix < XSize; ix++) {
		Data[ix] = 0;
	}
	// Bottom edge.
	for (ix = 0; ix < XSize; ix++) {
		Data[(ZSize-1) * XSize + ix] = 0;
	}
	// Left edge.
	for (iz = 0; iz < ZSize; iz++) {
		Data[iz * XSize] = 0;
	}
	// Right edge.
	for (iz = 0; iz < ZSize; iz++) {
		Data[iz * XSize + XSize-1] = 0;
	}
}


float	Grid::GetValue(const UTMVector& v)
// Get a value at the specified UTM position.
// Might require interpolation, since the distance isn't necessarily
// aligned with this grid's pitch.  E.g., the small grids need to get
// values between points of the big grid.
// If the position is outside our range, return UNSET.  This won't
// happen in the normal BigGrid case.
{
	float Val;	// interpolated elevation

	// Get our closest point indices, rounded down.

	// Convert to grid-index coordinates.
//	float XDistanceInOurSpace = (XDistance - XOriginDistance);
//	float ZDistanceInOurSpace = - (ZDistance - ZOriginDistance);
	float	XDistanceInOurSpace = v.X() - GridIndexOrigin.X() - XIndexOrigin;
	float	ZDistanceInOurSpace = GridIndexOrigin.Y() - v.Y() - ZIndexOrigin;
	if (XDistanceInOurSpace < 0 || ZDistanceInOurSpace < 0) {
		// I wanna see it.
		return(UNSET);	// Outside our range.
	}

	int ix = (int) (XDistanceInOurSpace / Pitch);
	int iz = (int) (ZDistanceInOurSpace / Pitch);
	if (ix >= XSize || iz >= ZSize) {
		// I wanna see it.
		return(UNSET);
	}
	// Notice that we tested XDistanceInOurSpace < 0, not ix < 0.
	// A slightly negative distance would truncate to a 0 index.

	// Fraction of distance to the next point.
	float dx = fmod(XDistanceInOurSpace, Pitch) / float(Pitch);
	float dz = fmod(ZDistanceInOurSpace, Pitch) / float(Pitch);
	if (dx < 0 || dz < 0) {	// an assertion
		cout << "Negative dx or dz in Grid::GetValue()" << endl;
		exit(-1);
	}

	// Interpolate the left side's value:  x,z -> x,z+1.
	float UpperLeft = Data[iz * XSize + ix];
	float LowerLeft = Data[(iz+1) * XSize + ix];
	// Same for right side, x+1,z -> x+1,z+1.
	float UpperRight = Data[iz * XSize + (ix+1)];
	float LowerRight = Data[(iz+1) * XSize + (ix+1)];

	// Zero is a valid value but not UNSET.
	if (UpperLeft == UNSET || LowerLeft == UNSET ||
	    UpperRight == UNSET || LowerRight == UNSET)
	{
		// I wanna see it.  Shouldn't happen in Grid.
		cout << "Missing value in Grid::GetValue!" << endl;
	}

	float Left = UpperLeft + dz * (LowerLeft - UpperLeft);
	float Right = UpperRight + dz * (LowerRight - UpperRight);
	Val = Left + dx * (Right - Left);

	return(Val);
}


void	Grid::Write(FILE* fp)
// Write a header and all the Data to the specified file.
{
	fwrite((char*) &XSize, sizeof(int), 1, fp);
	fwrite((char*) &ZSize, sizeof(int), 1, fp);

	int XOriginScaled = XIndexOrigin >> Scale;
	int ZOriginScaled = ZIndexOrigin >> Scale;
	
	fwrite((char*) &XOriginScaled, sizeof(int), 1, fp);
	fwrite((char*) &ZOriginScaled, sizeof(int), 1, fp);

	fwrite((char*) &Scale, sizeof(int), 1, fp);

	int iz, ix;	// grid indices
	for (iz = 0; iz < ZSize; iz++) {
		for (ix = 0; ix < XSize; ix++) {
			float Val = Data[iz * XSize + ix];
			if (Val == UNSET) {
				// I wanna see it.
				cout << "Unset value in Grid::Write!" << endl;
			}

			if (Val < -100 || Val > 4000) {
				cout << "Writing insane grid value: " << Val << " at (" << ix << ", " << iz << ")." << endl;
			}
			
			fwrite((void*) &Val, sizeof(float), 1, fp);
		}
	}
}


GridBag::GridBag()
// Constructor.
{
	nGrids = 0;
}


void	GridBag::Add(Grid* pGrid)
// Add a Grid to the bag.
{
	if (nGrids >= MAX_GRIDS) {
		cout << "Max number of grids is " << MAX_GRIDS << endl;
		exit(-1);
	}

	Grids[nGrids] = pGrid;
	nGrids++;
}


//void	GridBag::SetRegion(float z0, float x0, float z1, float x1)
//// Tell all the Grids the corners of the region of interest.
//{
//	for (int i=0; i < MAX_GRIDS; i++) {
//		Grids[i]->SetRegion(z0, x0, z1, x1);
//	}
//}


void	GridBag::Preprocess()
// Tell all the Grids to preprocess themselves.
{
	for (int i=0; i < nGrids; i++) {
		Grids[i]->Preprocess();
	}
}


static void	Write32(FILE* fp, int i)
// Writes a 32-bit int to the output file.
{
	fwrite(&i, 4, 1, fp);
}


static void	Write16(FILE* fp, int i)
// Writes a 16-bit int to the output file.
{
	fwrite(&i, 2, 1, fp);
}


void	GridBag::Write(char* filename)
// Write the whole GridBag to a file.
{
	FILE* fp = fopen(filename, "wb");
	if (!fp) {
		cout << "Unable to open output_file " << filename << "\n";
		exit(-1);
	}

	// Header marker.
	Write32(fp, -1);

	// File version.
	Write32(fp, 7);

	// Grids.
	Write32(fp, nGrids);
	
	for (int i=0; i < nGrids; i++) {
		Grids[i]->Write(fp);
	}

	// Geometry count: 0
	Write32(fp, 0);

	// Solid count: 0
	Write32(fp, 0);

	// Object count: 0
	Write32(fp, 0);

	// Surface polygons: 0
	Write32(fp, 0);
	
	fclose(fp);
}


float	GridBag::GetValue(const UTMVector& v)
// Computes the sum of all contained grids at the given point, and returns it.
{
	float	sum = 0;
	for (int i=0; i < nGrids; i++) {
		sum += Grids[i]->GetValue(v);
	}
	return sum;
}

