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
// Grid.hpp	jbl 09/07/98	Copyright Slingshot

// Declarations for the Grid class, which represents a height-field model
// that's been converted from a DEM, ready for writing to a SoulRide
// terrain file.  See the documentation in ShredDEM.cpp.
// Also for the GridBag class, which is a collection of Grids, usually 17.


#ifndef GRID_HPP
#define GRID_HPP


#include "dem.hpp"


// Use -1 to indicate missing data.
#define UNSET	-1

#define MAX_GRIDS	100


class Grid {
public:
	Grid(int scale, int xsize, int zsize, const UTMVector& NWCorner);

	void	GetCoords(UTMVector* result, int xindex, int zindex);
	
	void	SetValue(int x, int z, float val);
	void	Preprocess();
	float	GetValue(const UTMVector& v);
	
	void	Write(FILE* fp);

	static void	SetGridIndexOrigin(const UTMVector& NWCorner);
	
private:
	int	Scale;
	int	Pitch;	// Actually Pitch is redundant; it's 2 ^ Scale.
	int	ZSize, XSize;

	int	XIndexOrigin, ZIndexOrigin;
//	float	ZOriginDistance, XOriginDistance;
//	float	ZRegionOriginDistance, XRegionOriginDistance;

	float*	Data;
};


class GridBag {
public:
	GridBag::GridBag();
	void	Add(Grid* pGrid);
	void	Preprocess();
	void	Write(char *filename);
	float	GetValue(const UTMVector& v);

private:
	int	nGrids;
	// The zeroth Grid is the BigGrid.
	Grid* 	Grids[MAX_GRIDS];
};


#endif // GRID_HPP
