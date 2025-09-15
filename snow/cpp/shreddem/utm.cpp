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
// utm.cpp	Code for converting from lat/long to UTM, and vice versa

// Slightly modified from code posted to xxxxxxxxxx by Chuck Gantz- chuck.gantz@globalstar.com



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utm.hpp"


/*Reference ellipsoids derived from Peter H. Dana's website- 
http://www.utexas.edu/depts/grg/gcraft/notes/datum/elist.html
Department of Geography, University of Texas at Austin
Internet: pdana@mail.utexas.edu
3/22/95

Source
Defense Mapping Agency. 1987b. DMA Technical Report: Supplement to Department of Defense World Geodetic System
1984 Technical Report. Part I and II. Washington, DC: Defense Mapping Agency
*/


const double PI = 3.14159265;
const double FOURTHPI = PI / 4;
const double deg2rad = PI / 180;
const double rad2deg = 180.0 / PI;


class Ellipsoid
{
public:
	Ellipsoid(){};
	Ellipsoid(int Id, char* name, double radius, double ecc)
	{
		id = Id; ellipsoidName = name; 
		EquatorialRadius = radius; eccentricitySquared = ecc;
	}

	int id;
	char* ellipsoidName;
	double EquatorialRadius; 
	double eccentricitySquared;  

};


static Ellipsoid ellipsoid[] = 
{//  id, Ellipsoid name, Equatorial Radius, square of eccentricity	
	Ellipsoid( -1, "Placeholder", 0, 0),//placeholder only, To allow array indices to match id numbers
	Ellipsoid( 1, "Airy", 6377563, 0.00667054),
	Ellipsoid( 2, "Australian National", 6378160, 0.006694542),
	Ellipsoid( 3, "Bessel 1841", 6377397, 0.006674372),
	Ellipsoid( 4, "Bessel 1841 (Nambia) ", 6377484, 0.006674372),
	Ellipsoid( 5, "Clarke 1866", 6378206, 0.006768658),
	Ellipsoid( 6, "Clarke 1880", 6378249, 0.006803511),
	Ellipsoid( 7, "Everest", 6377276, 0.006637847),
	Ellipsoid( 8, "Fischer 1960 (Mercury) ", 6378166, 0.006693422),
	Ellipsoid( 9, "Fischer 1968", 6378150, 0.006693422),
	Ellipsoid( 10, "GRS 1967", 6378160, 0.006694605),
	Ellipsoid( 11, "GRS 1980", 6378137, 0.00669438),
	Ellipsoid( 12, "Helmert 1906", 6378200, 0.006693422),
	Ellipsoid( 13, "Hough", 6378270, 0.00672267),
	Ellipsoid( 14, "International", 6378388, 0.00672267),
	Ellipsoid( 15, "Krassovsky", 6378245, 0.006693422),
	Ellipsoid( 16, "Modified Airy", 6377340, 0.00667054),
	Ellipsoid( 17, "Modified Everest", 6377304, 0.006637847),
	Ellipsoid( 18, "Modified Fischer 1960", 6378155, 0.006693422),
	Ellipsoid( 19, "South American 1969", 6378160, 0.006694542),
	Ellipsoid( 20, "WGS 60", 6378165, 0.006693422),
	Ellipsoid( 21, "WGS 66", 6378145, 0.006694542),
	Ellipsoid( 22, "WGS-72", 6378135, 0.006694318),
	Ellipsoid( 23, "WGS-84", 6378137, 0.00669438)
};


void LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long, 
			 double* UTMNorthing, double* UTMEasting, char* UTMZone)
// converts lat/long to UTM coords.  Equations from USGS Bulletin 1532 
// East Longitudes are positive, West longitudes are negative. 
// North latitudes are positive, South latitudes are negative
// Lat and Long are in decimal degrees
// Does not take into account thespecial UTM zones between 0 degrees and 
// 36 degrees longitude above 72 degrees latitude and a special zone 32 
// between 56 degrees and 64 degrees north latitude 
{
	//Written by Chuck Gantz- chuck.gantz@globalstar.com

	double a = ellipsoid[ReferenceEllipsoid].EquatorialRadius;
	double eccSquared = ellipsoid[ReferenceEllipsoid].eccentricitySquared;
	double k0 = 0.9996;

	double LongOrigin;
	double eccPrimeSquared;
	double N, T, C, A, M;

	double LatRad = Lat*deg2rad;
	double LongRad = Long*deg2rad;
	double LongOriginRad;

	if(Long > -6 && Long <= 0) LongOrigin = -3;//arbitrarily set origin at 0 longitude to 3W
	else if(Long < 6 && Long > 0) LongOrigin = 3;
	else LongOrigin = int(Long/6)*6+3*int(Long/6)/abs(int(Long/6));
	LongOriginRad = LongOrigin * deg2rad;

	//compute the UTM Zone from the latitude and longitude
	sprintf(UTMZone, "%d%c", int((Long + 180)/6) + 1, UTMLetterDesignator(Lat));
	
	eccPrimeSquared = (eccSquared)/(1-eccSquared);

	N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
	T = tan(LatRad)*tan(LatRad);
	C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
	A = cos(LatRad)*(LongRad-LongOriginRad);

	M = a*((1	- eccSquared/4		- 3*eccSquared*eccSquared/64	- 5*eccSquared*eccSquared*eccSquared/256)*LatRad 
				- (3*eccSquared/8	+ 3*eccSquared*eccSquared/32	+ 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
									+ (15*eccSquared*eccSquared/256 + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad) 
									- (35*eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));
	
	*UTMEasting = (double)(k0*N*(A+(1-T+C)*A*A*A/6
					+ (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
					+ 500000.0);

	*UTMNorthing = (double)(k0*(M+N*tan(LatRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
				 + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));
	if(Lat < 0)
		*UTMNorthing += 10000000.0; //10000000 meter offset for southern hemisphere
}

char UTMLetterDesignator(double Lat)
//This routine determines the correct UTM letter designator for the given latitude
//returns 'Z' if latitude is outside the UTM limits of 80N to 80S
{
	//Written by Chuck Gantz- chuck.gantz@globalstar.com
	char LetterDesignator;

	if((80 >= Lat) && (Lat > 72)) LetterDesignator = 'X';
	else if((72 >= Lat) && (Lat > 64)) LetterDesignator = 'W';
	else if((64 >= Lat) && (Lat > 56)) LetterDesignator = 'V';
	else if((56 >= Lat) && (Lat > 48)) LetterDesignator = 'U';
	else if((48 >= Lat) && (Lat > 40)) LetterDesignator = 'T';
	else if((40 >= Lat) && (Lat > 32)) LetterDesignator = 'S';
	else if((32 >= Lat) && (Lat > 24)) LetterDesignator = 'R';
	else if((24 >= Lat) && (Lat > 16)) LetterDesignator = 'Q';
	else if((16 >= Lat) && (Lat > 8)) LetterDesignator = 'P';
	else if(( 8 >= Lat) && (Lat > 0)) LetterDesignator = 'N';
	else if(( 0 >= Lat) && (Lat > -8)) LetterDesignator = 'M';
	else if((-8>= Lat) && (Lat > -16)) LetterDesignator = 'L';
	else if((-16 >= Lat) && (Lat > -24)) LetterDesignator = 'K';
	else if((-24 >= Lat) && (Lat > -32)) LetterDesignator = 'J';
	else if((-32 >= Lat) && (Lat > -40)) LetterDesignator = 'H';
	else if((-40 >= Lat) && (Lat > -48)) LetterDesignator = 'G';
	else if((-48 >= Lat) && (Lat > -56)) LetterDesignator = 'F';
	else if((-56 >= Lat) && (Lat > -64)) LetterDesignator = 'E';
	else if((-64 >= Lat) && (Lat > -72)) LetterDesignator = 'D';
	else if((-72 >= Lat) && (Lat > -80)) LetterDesignator = 'C';
	else LetterDesignator = 'Z'; //This is here as an error flag to show that the Latitude is outside the UTM limits

	return LetterDesignator;
}


void UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, const char* UTMZone,
			  double* Lat,  double* Long )
//converts UTM coords to lat/long.  Equations from USGS Bulletin 1532 
//East Longitudes are positive, West longitudes are negative. 
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees. 
//Does not take into account the special UTM zones between 0 degrees 
//and 36 degrees longitude above 72 degrees latitude and a special 
//zone 32 between 56 degrees and 64 degrees north latitude
{
	//Written by Chuck Gantz- chuck.gantz@globalstar.com

	double k0 = 0.9996;
	double a = ellipsoid[ReferenceEllipsoid].EquatorialRadius;
	double eccSquared = ellipsoid[ReferenceEllipsoid].eccentricitySquared;
	double eccPrimeSquared;
	double e1 = (1-sqrt(1-eccSquared))/(1+sqrt(1-eccSquared));
	double N1, T1, C1, R1, D, M;
	double LongOrigin;
	double mu, phi1, phi1Rad;
	double x, y;
	int ZoneNumber;
	char* ZoneLetter;
	int NorthernHemisphere; //1 for northern hemispher, 0 for southern

	x = UTMEasting - 500000.0; //remove 500,000 meter offset for longitude
	y = UTMNorthing;

	ZoneNumber = strtoul(UTMZone, &ZoneLetter, 10);
	if((*ZoneLetter - 'N') > 0)
		NorthernHemisphere = 1;//point is in northern hemisphere
	else
	{
		NorthernHemisphere = 0;//point is in southern hemisphere
		y -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere
	}

	LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone

	eccPrimeSquared = (eccSquared)/(1-eccSquared);

	M = y / k0;
	mu = M/(a*(1-eccSquared/4-3*eccSquared*eccSquared/64-5*eccSquared*eccSquared*eccSquared/256));

	phi1Rad = mu	+ (3*e1/2-27*e1*e1*e1/32)*sin(2*mu) 
				+ (21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(4*mu)
				+(151*e1*e1*e1/96)*sin(6*mu);
	phi1 = phi1Rad*rad2deg;

	N1 = a/sqrt(1-eccSquared*sin(phi1Rad)*sin(phi1Rad));
	T1 = tan(phi1Rad)*tan(phi1Rad);
	C1 = eccPrimeSquared*cos(phi1Rad)*cos(phi1Rad);
	R1 = a*(1-eccSquared)/pow(1-eccSquared*sin(phi1Rad)*sin(phi1Rad), 1.5);
	D = x/(N1*k0);

	*Lat = phi1Rad - (N1*tan(phi1Rad)/R1)*(D*D/2-(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
					+(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared-3*C1*C1)*D*D*D*D*D*D/720);
	*Lat = *Lat * rad2deg;

	*Long = (D-(1+2*T1+C1)*D*D*D/6+(5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)
					*D*D*D*D*D/120)/cos(phi1Rad);
	*Long = LongOrigin + *Long * rad2deg;
}



#ifdef NOT


//UTM Conversion.cpp- test program for lat/long to UTM and UTM to lat/long conversions
#include <iostream.h>
#include <iomanip.h>
#include "LatLong-UTMconversion.h"


void main()
{
	double Lat = 47.37816667;
	double Long = 8.23250000;
	double UTMNorthing;
	double UTMEasting;
	double SwissNorthing;
	double SwissEasting;
	char UTMZone[4];
	int RefEllipsoid = 23;//WGS-84. See list with file "LatLong- UTM conversion.cpp" for id numbers

	cout << "Starting position(Lat, Long):  " << Lat << "   " << Long <<endl;

	LLtoUTM(RefEllipsoid, Lat, Long, UTMNorthing, UTMEasting, UTMZone);
	cout << setiosflags(ios::showpoint | ios::fixed) << setprecision(5);
	cout << "Calculated UTM position(Northing, Easting, Zone):  ";
	cout << UTMNorthing << "   " << UTMEasting;
	cout << "   " << UTMZone <<endl;
	
	UTMtoLL(RefEllipsoid, UTMNorthing, UTMEasting, UTMZone, Lat, Long);
	cout << "Calculated Lat, Long position(Lat, Long):  " << Lat << "   " << Long << endl <<endl;

	LLtoSwissGrid(Lat, Long, SwissNorthing, SwissEasting);
	cout << setiosflags(ios::showpoint | ios::fixed) << setprecision(5);
	cout << "Calculated Swiss Grid position(Northing, Easting):  ";
	cout << SwissNorthing << "   " << SwissEasting << endl;
}


/* N 47.38195ƒ E 8.54879ƒ  (Swiss Grid: 683.748 248.342)
  N 47ƒ12.625' / E 7ƒ 27.103'= N 47.21041667 E 7.45171667(Swiss Grid = 600920/228685) 
  N 47ƒ22.690' / E 8ƒ 13.950'= N 47.37816667 E 8.23250000 (Swiss Grid = 659879/247637)
*/


#endif // NOT
