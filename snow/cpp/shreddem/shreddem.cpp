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
// ShredDEM.cpp		-jbl 08/12/98  Copyright Slingshot

/*
Converts USGS DEM datasets from their SDTS format into our .srt (Soul Ride
terrain) format.  It uses the USGS SDTS++ package to read the SDTS-formatted
DEM files.  (See the SDTS++ file sio_Reader_t.cpp for an example of the basic
reading code; I started with that.  Any name here that starts with sio_ or sc_
talks to that library.  All the SDTS-related code is in this file.)

Typically we generate a terrain of 8192 m on a side.  Our terrain 
format contains one or more regular grids of float elevation values,
which can be overlapped and summed to represent the terrain height.
Each grid can have up to 64 x 64 points, at a pitch of 1 to 256 m.
The best resolution (finest pitch) we try to generate is 32 m, since the
DEM files have a pitch of 30 m in the horizontal plane (our x, z).

The tricky parts are:

 * It seldom happens that the interesting part of the terrain (the ski area)
   is in the middle sweet spot of a single DEM dataset.  We have to 
   stitch together multiple neighboring DEMs, and then cut out a section
   that places the ski area in the middle.

 * Each DEM has a different origin that is not aligned with the
   grids of its neighbors.  Each DEM's corners are on latitude/longitude
   points that are multiples of 7.5 arc-minutes, which is seldom an
   even multiple of 30 m.  We can convert lat/long to flat-projected UTM
   coordinates in meters -- see the tool ConvertXY for example -- in order
   to calculate the linear distance between DEM origins.

 * A DEM is not a perfect rectangle.  It's a quadrilateral and the edges
   have jaggies.  Actually this shouldn't be tricky, if the aformentioned
   origin alignment works as it should.  The jaggies should line up with
   each other like a jigsaw puzzle, leaving only one data point at each
   position.  Or at least one or more similar data points.  I hope.

 * A DEM has a 30m internal data spacing, but we want 32 m spacing.  Plenty of
   bilinear interpolation needed.

Drawing an example based on the VailWest and Minturn DEMs:

         UTM origin
            0,0  (N 39 deg 45', W 106 30')
             +-----------------------------------+   --> our x = east
             | ..                   VailWest     |
             | .internal                         |
             | .30m spacing                      |
             | .                                 |
             | .                                 |
             |                                   |
 UTM origin  |                                   |
 -232.0,   +-----------------------------------+ |
 -13873.6  | +---------------------------------|-+
(39 37'30" |                                   |
106 30')   |                         Minturn   |
           |                                   |
           |                                   |
           |                                   |
           |                                   |
           |                                   |
           +-----------------------------------+

	   |
	   |
	   v
     our z = south


That's a picture of the incoming data.  Now for the outgoing.

(Updated 4/19/1999 by thatcher)

The goal is to represent a 64km x 64km region.  However, we need to
use a varying level of detail in order to cover that region and still
have a reasonable amount of detail in the areas near the user.  So
what we do is create a large, coarse 256x256 grid with 256m pitch, to
cover the entire terrain.  Then, in the middle of the region, which
we assume will be the region of most interest, we place a smaller
256x256 grid, but with 32m pitch, to provide more detail.


#ifdef OLD_WAY

The terrain is too large for a single grid with 32-m pitch.
We first generate a coarse grid with 32 x 32 points at 256 m pitch.
Then we generate sixteen (4 x 4) fine grids, each of 64 x 64 points at 32 m.
Each fine grid is the difference between the coarse grid's interpolated height
and the DEM data.  It overlays a section of the coarse grid, and the sum
of the two equals the correct height.

That tiled overlay scheme works but it gives slow performance because
of "cliffs" -- rapid variations in height (from something to zero) at an edge.
Cliffs slow down the terrain engine's automatic smoothing and caching
algorithms.

Hence we force the edges of the fine grids to zero.  We overlap them
slightly (one row or column of overlap) so that the next overlapping grid
can pick up the difference value where the first one is forced to 0.

	+=========================== coarse grid (256 m pitch)
	|0----------10----------1
	|.          ..          . edges
	|. fine grid..          . all 0
	|. (30 m    ..          .
	|.  pitch)  ..          .
	|.          ..          .
	|0----------10----------1
	|<-FineGrid0->
	|           <-FineGrid1->

The fine grids contain only 62 x 62 points of real (non-forced-zero) data.

For the same reason we force the edges of the coarse grid to zero and fade
the height to zero in the border near the edges.  In this case there's no
overlapping grid to make up the values we took away... the borders of the
terrain remain artificially sloped, and the user isn't expected to go there.

#endif // OLD_WAY


Objects:

A DEM is an object that keeps its own data array and its own origin relative
to some arbitrary origin in flat UTM space.  It can be asked for an elevation
in UTM space and will do the origin subtraction and interpolation internally.

A DEMBag is a collection of DEMs (usually 4).  It knows how to preprocess
the collection to fade the borders to zero.  It can be asked for an elevation
value at a UTM point; it in turn asks the DEMs to find out which one covers
that point.

A Grid holds the data converted to our format.  It can be asked to write
itself to a file.

A GridBag is a collection of Grids (usually 17).  Mainly it knows how to write
its grids to a file. 
*/


// Disable the warning "identifier truncated to 255 characters in debug info".
#pragma warning( disable : 4786 )


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
//#include <conio.h>
//#include <io.h>
#include <string.h>

using namespace std;

//#include "io/sio_Reader.h"
//#include "io/sio_8211Converter.h"
//#include "container/sc_Record.h"

extern "C" {
#include "stc123.h"
};

#include "dem.hpp"
#include "grid.hpp"


//// These are the parameters that the user will specify for a terrain.
// The source data sets and the desired sub-region.  
// They should be read from an external parameter file -- see vail.sdm
// for example -- if anyone but myself is going to be doing this conversion.
// But for now, embed them here in the code.

// The first set must be the upper-left one.

struct dataset {
	char	filename[80];
	dataset*	Next;
};
			    

dataset*	sets = NULL;
int	InputDataSetCount = 0;


// Array of structs for holding specs of grids to be generated.
struct GridCommandInfo {
	UTMVector	NWCorner;
	int	XSize, ZSize, Scale;
};
const int	MAX_GRID_COMMANDS = 20;
int	GridCommandCount = 0;
GridCommandInfo	GridCommand[MAX_GRID_COMMANDS];


#define BIG_GRID_SCALE	8
#define BIG_GRID_SIZE	256

#define SMALL_GRID_SCALE 5
#define SMALL_GRID_SIZE	256

//#define SMALL_GRIDS_PER_SIDE	4	// 4x4 small grids



// Perhaps REGION_SIZE ZERO_BAND and FADE_BAND should also be specifiable,
// but for now they're hard-wired.


// Number of samples to force to 0 at edges of internal array.  1 => edges only.
#define ZERO_BAND	2
// Number of samples over which we fade to the zero band.
#define FADE_BAND	50



//void	ExtractRecord(sc_Record const & record, DEM *pDEM);
int	ExtractData(FILE* in, DEM *pDEM);


// Data shared between main() and ExtractData().
int	CornerCount;
UTMVector	LowerLeft;
double	XResolution, YResolution;
float	VerticalScale = 1.0;
bool	set_size = false;
bool	set_domain = false;


char*	DatasetBasepath = "";
char*	OutputFilename = "shreddem.srt";
float	OutputDatasetSize = 8192;
// Center of the output data, in UTM coords.
UTMVector	OutputCenter;


void	ProcessCommandLine(int argc, char* argv[]);


void	ProcessCommandsFromFile(char* filename)
// Read the file and process the contents as if they appeared on the command line.
{
	const int	MAX_CHARS = 4096;
	const int	MAX_TOKENS = 400;
	char	buf[MAX_CHARS];
	char*	argv[MAX_TOKENS];

	int	CharCount = 0;
	int	TokenCount = 0;

	// Read the contents of the file into a buffer.
	FILE*	fp = fopen(filename, "r");
	if (fp == NULL) {
		cout << "Can't open file '" << filename << "' for input." << endl;
		exit(1);
	}

	for (;;) {
		if (CharCount >= MAX_CHARS) {
			cout << "Input file '" << filename << "' exceeded temporary buffer." << endl;
			exit(1);
		}
		
		int	c = fgetc(fp);
		if (c == EOF) {
			buf[CharCount++] = 0;
			break;
		}
		buf[CharCount++] = c;
	}

	fclose(fp);

	// Tokenize the buffer.
	argv[0] = "shreddem";	// stand-in for arg 0, the program name.
	TokenCount = 1;
	bool	QuotesOn = false;
	bool	GotChars = false;
	argv[TokenCount] = &buf[0];
	
	for (int i = 0; i < CharCount; i++) {
		char	c = buf[i];
		if ((QuotesOn && c == '\"') ||
		    (QuotesOn == false && GotChars == true && (isspace(c) || c == 0)))
		{
			// End of a token.
			QuotesOn = false;
			GotChars = false;
			buf[i] = 0;
			TokenCount++;
		} else if (GotChars == false && isspace(c) == 0) {
			// Start of a token.
			if (TokenCount >= MAX_TOKENS) {
				cout << "Input file '" << filename << "' exceeded the max # of tokens." << endl;
				exit(1);
			}
			
			GotChars = true;
			if (c == '\"') {
				QuotesOn = true;
				i++;
			}
			argv[TokenCount] = &buf[i];
		}
	}

	// Process the tokens.
	ProcessCommandLine(TokenCount, argv);
}


void	ProcessCommandLine(int argc, char* argv[])
// Processes the given list of tokens as command-line arguments.
{
	int	i;
	for (i = 1; i < argc; i++) {
		char*	opt = argv[i];

		if (opt[0] == '-' || opt[0] == '/') {
			switch (opt[1]) {
			default:
			case 'h':
				// Print usage info.
				printf("ShredDEM elevation model conversion utility.\n");
				printf("Copyright 1998-1999 Slingshot Game Technology, Inc.\n");
				printf("\n");
				printf("Collects and converts data from SDTS format files to an .SRT file suitable\n");
				printf("for use by Slingshot's Soul Ride terrain engine.\n");
				printf("\n");
				printf("-h              Prints this message.\n");
				printf("-p <path>       Specifies the path under which the input data files are stored.\n");
				printf("-f <filename>   Specifies a file containing a list of command-line options.\n");
				printf("-o <filename>   Specifies the output file name.\n");
				printf("<filename> ...  Specifies input data subdirs and basenames.\n");
				printf("-s <meters>     Specifies the size of the output dataset.\n");
				printf("\n");
				printf("For defining output:\n");
				printf("-g <zone> <easting> <northing> <scale> <xsize> <zsize>	Specifies an output grid to be generated.\n");
				printf("	<zone> <easting> <northing> are the UTM coordinates of the center of the grid.\n");
				printf("	<scale> specifies the spacing of grid samples.  The spacing is 2^<scale> meters.\n");
				printf("	<xsize> <zsize> gives the sample count in the east-west and north-south directions.\n");
//				printf("-u <zone> <easting> <northing>    UTM coordinates.\n");
////				printf("-l <latitude> <longitude>         Lat/lon, Use decimal degrees.  Neg long. is W.\n");
				exit(1);
				break;

			case 'p':
				// Set the data file path.
				i++;
				DatasetBasepath = new char[strlen(argv[i]) + 1];
				strcpy(DatasetBasepath, argv[i]);
				break;

			case 'f':
				// Include a file containing command-line options.
				i++;
				ProcessCommandsFromFile(argv[i]);
				break;

			case 'o':
				// Set the output filename.
				i++;
				OutputFilename = new char[strlen(argv[i]) + 1];
				strcpy(OutputFilename, argv[i]);
				break;

			case 's':
				// Read the size in meters of the output dataset.
				i++;
				OutputDatasetSize = atof(argv[i]);
				break;

			case 'u':
				// Read the center of the desired output dataset, in UTM coordinates.
				i++;
				// xxxx ignore the zone code for now.
				i++;
				OutputCenter.SetX(atof(argv[i]));
				i++;
				OutputCenter.SetY(atof(argv[i]));
				break;

			case 'g':
			{
				// Generate a grid-command based on the given args, and add it to the list.
				if (GridCommandCount >= MAX_GRID_COMMANDS) {
					printf("MAX_GRID_COMMANDS exceeded\n");
					exit(1);
				}
				GridCommandInfo&	gc = GridCommand[GridCommandCount++];
				
				i++;
				// xxxx ignore UTM zone for now.
				i++;
				float	cx = atof(argv[i]);
				i++;
				float	cy = atof(argv[i]);

				i++;
				int	scale = atoi(argv[i]);
				i++;
				int	xsize = atoi(argv[i]);
				i++;
				int	zsize = atoi(argv[i]);

				gc.NWCorner.SetX(cx - (xsize << scale) * 0.5);
				gc.NWCorner.SetY(cy + (zsize << scale) * 0.5);
				gc.Scale = scale;
				gc.XSize = xsize;
				gc.ZSize = zsize;

				break;
			}
			
			}
		} else {
			// Interpret the token as an input dataset filename.
			dataset*	d = new dataset;
			strcpy(d->filename, argv[i]);
			d->Next = sets;
			sets = d;
			InputDataSetCount++;
		}
	}
}	
	

void	NewGrid(GridBag* outbag, DEMBag* inbag, int Scale, int XSize, int ZSize, const UTMVector& NWCorner);


int	main(int argc, char* argv[])
{
	//
	// Process command-line options.
	//
	ProcessCommandLine(argc, argv);

#if 0	
	//// Read the data with the SDTS++ library.
	// Set up default converter hints for the mnemonics that are found
	// in the datasets.
	converter_dictionary	converters;
	// We use only two of the possible converters.
	sio_8211Converter_BI16	converter_bi16;
	sio_8211Converter_BI32	converter_bi32;
	converters["X"] = &converter_bi32;
	converters["Y"] = &converter_bi32;
	converters["ELEVATION"] = &converter_bi16;

	ifstream *ifs;                 // module to be read from
	sio_8211Reader *reader;
#endif // 0

	// Create the DEMBag.
	DEMBag *InBag = new DEMBag();
	if (!InBag) {
		cout << "Unable to create DEMBag!\n";
		exit(-1);
	}

	for (; sets; sets = sets->Next) {
		// Create a DEM object to hold the data.
		DEM *pDEM = new DEM(/* sets[s].ZSize, sets[s].XSize,
				    float(sets[s].ZOriginDistance), float(sets[s].XOriginDistance),
				    SignZSouth, SignXEast */);
		if (!pDEM) {
			cout << "Unable to create DEM from '" << sets->filename << "' !" << endl;
			exit(-1);
		}

		// Read the interesting files from the dataset.
		CornerCount = 0;
		LowerLeft = UTMVector(-1, -1);
		XResolution = YResolution = -1;
		VerticalScale = 1.0;
		set_size = false;
		set_domain = false;
		int	i;
		for (i = 0; i < 5; i++) {
			// Start with the path & base name.
			char	in_filename[1000];
			strcpy(in_filename, DatasetBasepath);
			strcat(in_filename, sets->filename);
			
			// Tack on the appropriate ending for the file we want to process.
			const static char*	File[5] = { "RSDF.DDF", "SPDM.DDF", "IREF.DDF", "DDOM.DDF", "CEL0.DDF" };
			strcat(in_filename, File[i]);

			// Progress messages are nice during the slow reading phase.
			cout << "Reading " << in_filename << endl;

#if 0			
			ifs = new ifstream;
			if (!ifs) {
				cout << "Unable to create ifs!\n";
				exit(-1);
			}
			ifs->open(in_filename, ios::binary);
			if (ifs == NULL || *ifs == NULL) {
				cout << "Unable to open input file " << in_filename << "\n";
				cout << "Error: " << strerror(errno);
				exit(-1);
			}
			
			reader = new sio_8211Reader(*ifs, &converters);
			if (!reader) {
				cout << "Unable to create reader!\n";
				exit(-1);
			}
			
			sio_8211ForwardIterator i(*reader);
			sc_Record record;
			
			// Extract data to the DEM.
			while (i) {
				i.get(record);
				ExtractRecord(record, pDEM);
				++i;
			}

			delete reader;
			
			ifs->close();
			delete ifs;
#else // not 0
			FILE*	in;
			long int	int_level;	// "interchange level" -- ???
			char	ice;	// "inline code extension indicator" -- ???
			char	ccs[4];	// "code character set indicator"
			if (! beg123file(in_filename, 'r', &int_level, &ice, ccs, &in)) {
				cout << "Unable to open input file " << in_filename << "\n";
				cout << "Error: " << strerror(errno);
				exit(-1);
			}

			int	status;
			char	record[5000];
			long int	len;
			int	err = rd123ddrec(in, record, &status);
			if (err == 3 || err == 4) {
				cout << "error reading DDR\n";
				exit(-1);
			}
			printf("DDR = %s\n", record);
			// chk123ddrec(...);

//			while (ExtractRecord(in, pDEM) != EOF) {
//				printf("extracted...\n");//xxxxxxx
//			}
			ExtractData(in, pDEM);
			printf("about to end123file\n");//xxx
			end123file(&in);

//			fclose(in);
#endif // 0
		}

		InBag->Add(pDEM);
	}

	//// End of reading the data.


	// Set the region, apply the curve of the earth correction, and get the region's base elevation.
	double	HalfSize = OutputDatasetSize * 0.5;
	UTMVector	SWCorner = OutputCenter - UTMVector(HalfSize, HalfSize);
	InBag->SetRegion(SWCorner.X(), SWCorner.Y(),
			 OutputCenter.X() + HalfSize, OutputCenter.Y() + HalfSize);

	InBag->ApplyCurveOfTheEarth(OutputCenter, 6300000);
	
	int BaseElevation = InBag->GetMinValue();
	cout << "Base elevation = " << BaseElevation << endl;

	// Subtract out the base elevation and fade the edges to zero.
	InBag->Preprocess(BaseElevation, ZERO_BAND, FADE_BAND);

	
	//
	// Create the grid objects.
	//
	
	cout << "Creating grids " << endl;

	UTMVector	Origin(10000000, 0);
	
	// Find the northwest grid data origin which is the minimum x and maximum y of all grid extents.
	int	i;
	for (i = 0; i < GridCommandCount; i++) {
		GridCommandInfo&	gc = GridCommand[i];
		
		if (gc.NWCorner.X() < Origin.X()) Origin.SetX(gc.NWCorner.X());
		if (gc.NWCorner.Y() > Origin.Y()) Origin.SetY(gc.NWCorner.Y());
	}
	Grid::SetGridIndexOrigin(Origin);
//	UTMVector	NWCorner(OutputCenter.X() - (BIG_GRID_SIZE << BIG_GRID_SCALE) * 0.5,
//				 OutputCenter.Y() + (BIG_GRID_SIZE << BIG_GRID_SCALE) * 0.5);
//	Grid::SetGridIndexOrigin(NWCorner);

	// Create a GridBag to hold the generated grids.
	GridBag *OutBag = new GridBag();
	if (!OutBag) {
		cout << "Unable to create GridBag!\n";
		exit(-1);
	}

	// Create the actual grids and initialize their data from the input DEMs.
	for (i = 0; i < GridCommandCount; i++) {
		GridCommandInfo&	gc = GridCommand[i];

		NewGrid(OutBag, InBag, gc.Scale, gc.XSize, gc.ZSize, gc.NWCorner);
	}

/*	
	// Create BigGrid.
	int	XSize = BIG_GRID_SIZE;
	int	ZSize = BIG_GRID_SIZE;
	int	Scale = BIG_GRID_SCALE;
	int	Pitch = 1 << BIG_GRID_SCALE;

	NewGrid(OutBag, InBag, Scale, XSize, ZSize, NWCorner);

	// A somewhat smaller grid.
	NewGrid(OutBag, InBag, 6, 256, 256, OutputCenter + UTMVector(-128 << 6, 128 << 6));
	
	// Create a smaller grid, centered on the region of interest.
	XSize = SMALL_GRID_SIZE;
	ZSize = SMALL_GRID_SIZE;
	Scale = SMALL_GRID_SCALE;
	Pitch = 1 << SMALL_GRID_SCALE;
	double	HalfSmallGridSize = (SMALL_GRID_SIZE * Pitch) * 0.5;
	NewGrid(OutBag, InBag, Scale, XSize, ZSize, UTMVector(OutputCenter.X() - HalfSmallGridSize, OutputCenter.Y() + HalfSmallGridSize));
*/
	
	//// End of creating grids.
	
	//// Write 'em all out.
	cout << "Writing grids " << endl;
	OutBag->Write(OutputFilename);

	return 0;
}



void	NewGrid(GridBag* outbag, DEMBag* inbag, int Scale, int XSize, int ZSize, const UTMVector& NWCorner)
// Creates a grid with the given scale, dimensions and origin,
// initializes its data from the inbag, and adds it to the outbag.
{
	Grid*	g = new Grid(Scale, XSize, ZSize, NWCorner);
	
	// Set the grid heights.  Sample the inbag at each point in the grid.
	int iz, ix;
	for (iz = 0; iz < ZSize; iz++) {
		for (ix = 0; ix < XSize; ix++) {
			UTMVector	v;
			g->GetCoords(&v, ix, iz);
			g->SetValue(ix, iz, inbag->GetValue(v.X(), v.Y()) - outbag->GetValue(v));
		}
	}
	g->Preprocess();
	outbag->Add(g);
}



#if 0
void	PrintSubfieldValue(sc_SubfieldCntr::const_iterator s)
// Prints the value of the subfield.
{
	sc_Subfield::SubfieldType	t = s->getSubfieldType();

	switch (t) {
	default:
		break;

	case sc_Subfield::is_A: {
		string	str;
		if (s->getA(str)) {
			cout << str;
			return;
		}
		break;
	}

	case sc_Subfield::is_I: {
		long	l;
		if (s->getI(l)) {
			cout << l;
			return;
		}
		break;
	}

	case sc_Subfield::is_R: {
		double	d;
		if (s->getR(d)) {
			cout << d;
			return;
		}
		break;
	}

	case sc_Subfield::is_BI16: {
		long	l;
		if (s->getBI16(l)) {
			cout << l;
			return;
		}
		break;
	}

	case sc_Subfield::is_BI32: {
		long	l;
		if (s->getBI32(l)) {
			cout << l;
			return;
		}
		break;
	}
			
	}

	// Can't convert or unknown type.
	cout << "xxx";
}
#endif // 0


#if 0
   
void	ExtractRecord(sc_Record const & record, DEM *pDEM)
// Extract the values from the record and set them into the DEM.
// A record contains an entire row of data.
{
	long CurrentCol;
	long CurrentRow;

	for ( sc_FieldCntr::const_iterator field_itr = record.begin();
	      field_itr != record.end();
	      field_itr++ )
	{
		string field_mnemonic = field_itr->getMnemonic();
		string name = field_itr->getName();

		if (field_mnemonic == "RSDF") {
			cout << field_mnemonic << " :: " << name << endl;//xxxxxxx
			
			// Extract the info about the number of rows & cols in this dataset.
			long	rows = -1;
			long	cols = -1;
			
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				cout << "\t" << subfield_itr->getMnemonic() << " :: ";
				PrintSubfieldValue(subfield_itr);
				cout << endl;//xxxxxxx

				if (subfield_itr->getMnemonic() == "RWXT") {
					subfield_itr->getI(rows);
				} else if (subfield_itr->getMnemonic() == "CLXT") {
					subfield_itr->getI(cols);
				}
			}

			if (rows == -1 || cols == -1) {
				cout << "RSDF record missing RWXT or CLXT!" << endl;
				exit(-1);
			}

			// Set the array size of the DEM.
			pDEM->SetSize(cols, rows);
			
		} else if (field_mnemonic == "DMSA") {
			cout << field_mnemonic << " :: " << name << endl;//xxxxxxx

			if (CornerCount == 0) {
				// Extract info about the corners of the dataset.
				for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
				      subfield_itr != (*field_itr).end();
				      subfield_itr++ )
				{
					cout << "\t" << subfield_itr->getMnemonic() << " :: ";
					PrintSubfieldValue(subfield_itr);
					cout << endl;//xxxxxxx
					
					double d = 0;
					if (subfield_itr->getMnemonic() == "X") {
						subfield_itr->getR(d);
						LowerLeft.SetX(d);
					} else if (subfield_itr->getMnemonic() == "Y") {
						subfield_itr->getR(d);
						LowerLeft.SetY(d);
					}
				}
			}

			CornerCount++;

		} else if (field_mnemonic == "IREF") {
			// Extract resolution info.

			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				cout << "\t" << subfield_itr->getMnemonic() << " :: ";
				PrintSubfieldValue(subfield_itr);
				cout << endl;//xxxxxxx
				
				double d = 0;
				if (subfield_itr->getMnemonic() == "XHRS") {
					subfield_itr->getR(d);
					XResolution = d;
				} else if (subfield_itr->getMnemonic() == "YHRS") {
					subfield_itr->getR(d);
					YResolution = d;
				}
			}
			
			// Pass the data along to the DEM.
			if (XResolution == -1 || YResolution == -1 || LowerLeft.X() == -1 || LowerLeft.Y() == -1) {
				cout << "Error: couldn't collect corner or resolution data." << endl;
				exit(-1);
			} else {
				pDEM->SetDomain(LowerLeft, XResolution, YResolution);
			}
			
		} else if (field_mnemonic == "DDOM") {
			// Extract vertical units info.
			bool	Valid = false;
			float	scale = 1.0;
			string	val;
			
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				cout << "\t" << subfield_itr->getMnemonic() << " :: ";
				PrintSubfieldValue(subfield_itr);
				cout << endl;//xxxxxxx

				if (subfield_itr->getMnemonic() == "RAVA") {
					subfield_itr->getA(val);
					if (val == "MAX") {
						Valid = true;
					}
				} else if (subfield_itr->getMnemonic() == "ADMU") {
					subfield_itr->getA(val);
					if (val == "feet") {
						scale = 0.3048;
					}
				}
			}

			if (Valid) {
				VerticalScale = scale;
				cout << "\tsetting vertical scale to " << VerticalScale << endl;
			}
			
		} else if (field_mnemonic == "CELL") {
			// Set up for ensuing row data.
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				// What's the diff between RCID and ROWI?
				if (subfield_itr->getMnemonic() == "RCID") {
					subfield_itr->getI(CurrentRow);
					// The dataset starts with Row = 1.
					// Make it start with 0.
					CurrentRow--;
					CurrentCol = - 1;
				}
			}
			
		} else if (field_mnemonic == "CVLS") {
			// Read a row of data.
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				if (subfield_itr->getMnemonic() == "ELEVATION") {
					CurrentCol++;
					long RawElevation;
					subfield_itr->getBI16(RawElevation);
					int	Elevation;
					// The dataset supplies -32766 for
					// an unset point.
					// It supplies -32767 for a "void area".
//					if (RawElevation <= -32766) {
					if (RawElevation <= 0) {
						Elevation = UNSET;
					} else {
						Elevation = RawElevation * VerticalScale;
					}
					pDEM->SetValue(CurrentRow, CurrentCol, Elevation);
				}
			}
		}
#ifdef NOT
		else {
			// For debugging.
			cout << field_mnemonic << " :: " << name << endl;//xxxxxxx
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				cout << "\t" << subfield_itr->getMnemonic() << " :: ";
				PrintSubfieldValue(subfield_itr);
				cout << endl;//xxxxxxx
			}
		}
#endif // NOT
	}
} // end ExtractRecord


#else // not 0

   
int	ExtractData(FILE* in, DEM *pDEM)
// Extract the values from the next and set them into the DEM.
// A record contains an entire row of data.
// Returns EOF on end-of-file, or 0 otherwise.
{
	int	reverse = 1;	//xxxxxxx

	long CurrentCol;
	long CurrentRow;

	int	status = 0;
	char	data[8192];
	char	description[8192];
	char	formats[1024];
	char	tag[10];
	char	lead_id;
	long int	data_len;

	// Info about this dataset.
	long	rows = -1;
	long	cols = -1;

	// read and process each subfield.
	for (;;) {
		if (! rd123sfld(in, tag, &lead_id, data, &data_len, &status)) {
			cout << "Error reading data record subfield.\n";
			exit(-1);
		}

		if (! chk123sfld(in, tag, description, formats)) {
			cout << "Error reading data record subfield description.\n";
			exit(-1);
		}

		if ( !strcmp( tag, "RSDF" ) ) {
			// Extract the info about the number of rows & cols in this dataset.
			
			if ( !strcmp( description, "RWXT") ) {
				printf( "%s :: %s\n", tag, description );
				rows = atol( data );
			}
			if ( !strcmp( description, "CLXT") ) {
				printf( "%s :: %s\n", tag, description );
				cols = atol( data );
			}

			if ( set_size == false && rows != -1 && cols != -1 ) {
				printf( "set size: cols = %d, rows = %d\n", cols, rows );	//xxxx

				// Set the array size of the DEM.
				pDEM->SetSize(cols, rows);
				set_size = true;
			}

		} else if ( !strcmp( tag, "DMSA" ) ) {

			if (CornerCount == 0) {
				double d = 0;
				if ( !strcmp( description, "!X" ) ) {
					printf( "%s :: %s %s\n", tag, description, data );//xxx
					d = atof( data );
					LowerLeft.SetX(d);

				} else if ( !strcmp( description, "!Y" ) ) {
					printf( "%s :: %s %s\n", tag, description, data );//xxx
					d = atof( data );
					LowerLeft.SetY(d);

					CornerCount++;
				}
			}

		} else if ( ! strcmp( tag, "IREF" ) ) {
			// Extract resolution info.

			if ( ! strcmp( description, "XHRS" ) ) {
				XResolution = atof( data );
			} else if ( ! strcmp( description, "YHRS" ) ) {
				YResolution = atof( data );
			}
			
			// Pass the data along to the DEM.
			if ( set_domain == false && XResolution != -1 && YResolution != -1
			     && LowerLeft.X() != -1 && LowerLeft.Y() != -1 )
			{
				pDEM->SetDomain(LowerLeft, XResolution, YResolution);
				set_domain = true;

				printf( "setting domain: xres = %f yres = %f, LL.x = %f LL.y = %f\n",
					XResolution, YResolution, LowerLeft.X(), LowerLeft.Y());	//xxxx
			}
			
		} else if ( ! strcmp( tag, "DDOM" ) ) {
			// Extract vertical units info.
			bool	Valid = false;
			float	scale = 1.0;
			
			if ( ! strcmp( description, "RAVA" ) ) {
				if ( ! strcmp( data, "MAX" ) ) {
					Valid = true;
				}
			} else if ( ! strcmp( description, "ADMU" ) ) {
				printf( "DDOM :: ADMU : %s\n", data );	//xxx
				if ( ! strcmp( data, "feet" ) ) {
					scale = 0.3048;
				}
			}

			if (Valid) {
				VerticalScale = scale;
				cout << "\tsetting vertical scale to " << VerticalScale << endl;
			}
			
		} else if ( ! strcmp( tag, "CELL" ) ) {
			// What's the diff between RCID and ROWI?
			if ( ! strcmp( description, "RCID" ) ) {
				CurrentRow = atoi( data );
				// The dataset starts with Row = 1.
				// Make it start with 0.
				CurrentRow--;
				CurrentCol = -1;
			}
			
		} else if ( ! strcmp( tag, "CVLS" ) ) {
			// Read a row of data.
			if ( ! strcmp( description, "!ELEVATION" ) ) {
				CurrentCol++;
				short RawElevation;
				RawElevation = (data[0] << 8) | data[1];	// hm...
				
				int	Elevation;
				// The dataset supplies -32766 for
				// an unset point.
				// It supplies -32767 for a "void area".
//				if (RawElevation <= -32766) {
				if (RawElevation <= 0) {
					Elevation = UNSET;
				} else {
					Elevation = (int) (RawElevation * VerticalScale);
				}
				pDEM->SetValue(CurrentRow, CurrentCol, Elevation);
			}

		} else if ( strcmp( tag, "0001" ) != 0) {
			printf( "subfield: tag = %s desc = %s\n", tag, description );//xxx
		}

		if (status == 4) {
			return EOF;
		}
	}

#if 0
		string field_mnemonic = field_itr->getMnemonic(); // "tag"
		string name = field_itr->getName();

		if (field_mnemonic == "RSDF") {
			cout << field_mnemonic << " :: " << name << endl;//xxxxxxx
			
			// Extract the info about the number of rows & cols in this dataset.
			long	rows = -1;
			long	cols = -1;
			
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				cout << "\t" << subfield_itr->getMnemonic() << " :: ";
				PrintSubfieldValue(subfield_itr);
				cout << endl;//xxxxxxx

				if (subfield_itr->getMnemonic() == "RWXT") {
					subfield_itr->getI(rows);
				} else if (subfield_itr->getMnemonic() == "CLXT") {
					subfield_itr->getI(cols);
				}
			}

			if (rows == -1 || cols == -1) {
				cout << "RSDF record missing RWXT or CLXT!" << endl;
				exit(-1);
			}

			// Set the array size of the DEM.
			pDEM->SetSize(cols, rows);
			
		} else if (field_mnemonic == "DMSA") {
			cout << field_mnemonic << " :: " << name << endl;//xxxxxxx

			if (CornerCount == 0) {
				// Extract info about the corners of the dataset.
				for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
				      subfield_itr != (*field_itr).end();
				      subfield_itr++ )
				{
					cout << "\t" << subfield_itr->getMnemonic() << " :: ";
					PrintSubfieldValue(subfield_itr);
					cout << endl;//xxxxxxx
					
					double d = 0;
					if (subfield_itr->getMnemonic() == "X") {
						subfield_itr->getR(d);
						LowerLeft.SetX(d);
					} else if (subfield_itr->getMnemonic() == "Y") {
						subfield_itr->getR(d);
						LowerLeft.SetY(d);
					}
				}
			}

			CornerCount++;

		} else if (field_mnemonic == "IREF") {
			// Extract resolution info.

			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				cout << "\t" << subfield_itr->getMnemonic() << " :: ";
				PrintSubfieldValue(subfield_itr);
				cout << endl;//xxxxxxx
				
				double d = 0;
				if (subfield_itr->getMnemonic() == "XHRS") {
					subfield_itr->getR(d);
					XResolution = d;
				} else if (subfield_itr->getMnemonic() == "YHRS") {
					subfield_itr->getR(d);
					YResolution = d;
				}
			}
			
			// Pass the data along to the DEM.
			if (XResolution == -1 || YResolution == -1 || LowerLeft.X() == -1 || LowerLeft.Y() == -1) {
				cout << "Error: couldn't collect corner or resolution data." << endl;
				exit(-1);
			} else {
				pDEM->SetDomain(LowerLeft, XResolution, YResolution);
			}
			
		} else if (field_mnemonic == "DDOM") {
			// Extract vertical units info.
			bool	Valid = false;
			float	scale = 1.0;
			string	val;
			
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				cout << "\t" << subfield_itr->getMnemonic() << " :: ";
				PrintSubfieldValue(subfield_itr);
				cout << endl;//xxxxxxx

				if (subfield_itr->getMnemonic() == "RAVA") {
					subfield_itr->getA(val);
					if (val == "MAX") {
						Valid = true;
					}
				} else if (subfield_itr->getMnemonic() == "ADMU") {
					subfield_itr->getA(val);
					if (val == "feet") {
						scale = 0.3048;
					}
				}
			}

			if (Valid) {
				VerticalScale = scale;
				cout << "\tsetting vertical scale to " << VerticalScale << endl;
			}
			
		} else if (field_mnemonic == "CELL") {
			// Set up for ensuing row data.
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				// What's the diff between RCID and ROWI?
				if (subfield_itr->getMnemonic() == "RCID") {
					subfield_itr->getI(CurrentRow);
					// The dataset starts with Row = 1.
					// Make it start with 0.
					CurrentRow--;
					CurrentCol = - 1;
				}
			}
			
		} else if (field_mnemonic == "CVLS") {
			// Read a row of data.
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				if (subfield_itr->getMnemonic() == "ELEVATION") {
					CurrentCol++;
					long RawElevation;
					subfield_itr->getBI16(RawElevation);
					int	Elevation;
					// The dataset supplies -32766 for
					// an unset point.
					// It supplies -32767 for a "void area".
//					if (RawElevation <= -32766) {
					if (RawElevation <= 0) {
						Elevation = UNSET;
					} else {
						Elevation = RawElevation * VerticalScale;
					}
					pDEM->SetValue(CurrentRow, CurrentCol, Elevation);
				}
			}
		}
#ifdef NOT
		else {
			// For debugging.
			cout << field_mnemonic << " :: " << name << endl;//xxxxxxx
			for ( sc_SubfieldCntr::const_iterator subfield_itr = (*field_itr).begin();
			      subfield_itr != (*field_itr).end();
			      subfield_itr++ )
			{
				cout << "\t" << subfield_itr->getMnemonic() << " :: ";
				PrintSubfieldValue(subfield_itr);
				cout << endl;//xxxxxxx
			}
		}
#endif // NOT

	}
#endif // 0

} // end ExtractRecord


#endif // not 0
