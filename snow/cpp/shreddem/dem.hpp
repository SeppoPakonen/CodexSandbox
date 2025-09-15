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
// DEM.hpp	jbl 09/07/98	Copyright Slingshot

// Declaration for the DEM class, which represents data read from a USGS
// Digital Elevation Model.  See documentation in ShredDEM.cpp.
// Also the DEMBag class, which holds a collection of DEMs.


#ifndef DEM_HPP
#define DEM_HPP


// Zero is a valid value for us, like at the edges of the region.
// Use -1 to indicate missing data.
#define UNSET	-1


#define MAX_DEMS	100


#define DEM_PITCH	30.0	// 30 m between points in the incoming data


// Utility class for doing math on UTM coords.
struct UTMVector {
	UTMVector(double x = 0, double y = 0) { v[0] = x; v[1] = y; }

	UTMVector	operator+(const UTMVector& u) const { return UTMVector(v[0] + u.v[0], v[1] + u.v[1]); }
	UTMVector	operator-() const { return UTMVector(-v[0], -v[1]); }
	UTMVector	operator-(const UTMVector& u) const { return UTMVector(v[0] - u.v[0], v[1] - u.v[1]); }
	UTMVector	operator*(double d) const { return UTMVector(v[0] * d, v[1] * d); }
	UTMVector&	operator*=(double d) { v[0] *= d; v[1] *= d; return *this; }
	double	operator*(const UTMVector& u) const { return v[0] * u.v[0] + v[1] * u.v[1]; }

	double	X() const { return v[0]; }
	double	Y() const { return v[1]; }
	void	SetX(double d) { v[0] = d; }
	void	SetY(double d) { v[1] = d; }

// data:
	double	v[2];
};



class DEM {
public:
	// Constructor:  allocates the data array.
	// The origin is in meters, relative to the origin of the whole bag
	// of DEMs (the upper left corner of the upper left DEM).
	DEM(/* 	int ZSize, int XSize, float ZOriginDistance, float XOriginDistance,
		int signzsouth, int signxeast */);

	// Set a point, indexed relative to the DEM's own origin.
	// The first point is 0,0, the next one 30 m to the right is 0,1.
	void	SetValue(int iz, int ix, int Val);

	// Get the elevation at a point in UTM space, z,x in meters.
	// Might return zero if that point isn't in this DEM's set.
	float	GetValue(double XDistance, double ZDistance);

	// Remember the corners of our desired terrain region in
	// this DEM's coordinates.
	void	SetRegion(double z0, double x0, double z1, double x1);

	// Return the minimum elevation value for the specified region.
	int	GetMinValue();

	void	ApplyCurveOfTheEarth(const UTMVector& center, float EarthRadius);
	
	void	Preprocess(int BaseElevation, int ZeroBand, int FadeBand);


	// thatcher:
	void	SetSize(int xsize, int ysize);
	void	SetDomain(const UTMVector& LowerLeft, double XResolution, double YResolution);
	
private:
//	double	XOriginDistance, ZOriginDistance;
	int	ZSize, XSize;

//	// Indices of our data array enclosing the desired region.
//	int	iZ0Region, iX0Region, iZ1Region, iX1Region;

	int*	Data;	// The incoming DEM values are integers.

	// Parameters of mapping from UTM to grid & vice-versa.
	// xxx UTM zone;
	UTMVector	Origin;
	double	XRes, YRes;
};


class DEMBag {
public:
	DEMBag::DEMBag();

	void	Add(DEM* pDEM);

	// Get the elevation at a point in UTM space, z,x in meters.
	float	GetValue(double XDistance, double ZDistance);

	// Returns the minimum value for the region.
	int	GetMinValue();

	void	ApplyCurveOfTheEarth(const UTMVector& center, float EarthRadius);
	
	void	Preprocess(int BaseElevation, int ZeroBand, int FadeBand);

	void	SetRegion(double z0, double x0, double z1, double x1);

private:
	int	nDEMS;
	DEM *	DEMs[MAX_DEMS];
};


#endif // DEM_HPP
