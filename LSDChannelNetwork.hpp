//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDChannelNetwork.hpp
// header file for the LSDChannelNetwork object
// LSD stands for Land Surface Dynamics
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This object is written by
// Simon M. Mudd, University of Edinburgh
// David Milodowski, University of Edinburgh
// Martin D. Hurst, British Geological Survey
// <your name here>
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Version 0.0.1		30/08/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <vector>
#include "TNT/tnt.h"
#include "LSDFlowInfo.hpp"
#include "LSDRaster.hpp"
using namespace std;
using namespace TNT;

#ifndef LSDChannelNetwork_H
#define LSDChannelNetwork_H


class LSDChannelNetwork
{
	public:
	LSDChannelNetwork(vector<int> Sources, LSDFlowInfo& FlowInfo)
											{ create(Sources, FlowInfo); }

	// recursive algorithm for building the fastscape junction tree
	void add_to_stack(int lm_index, int& j_index, int bl_node);

	// this returns all the upstream junction of a junction_number_outlet
	vector<int> get_upslope_junctions(int junction_number_outlet);

	// this prints the link array to raster
	LSDIndexRaster StreamOrderArray_to_LSDIndexRaster();
	LSDIndexRaster JunctionArray_to_LSDIndexRaster();
	LSDIndexRaster JunctionIndexArray_to_LSDIndexRaster();
	LSDIndexRaster StreamOrderArray_to_BinaryNetwork_LSDIndexRaster();

	// function for printing out the longest channel upstream of a point
	void print_longest_channel(int outlet_junction, LSDFlowInfo& FInfo, LSDIndexRaster& dist_code,
									LSDRaster& dist_from_outlet);

	// get functions
	// these get data elements
	int get_NRows() const				{ return NRows; }
	int get_NCols() const				{ return NCols; }
	double get_XMinimum() const			{ return XMinimum; }
	double get_YMinimum() const			{ return YMinimum; }
	double get_DataResolution() const	{ return DataResolution; }
	int get_NoDataValue() const			{ return NoDataValue; }

	protected:

	// data for georeferencing
	int NRows;			// number of rows
	int NCols;			// number of columns
	double XMinimum;
	double YMinimum;

	// metadata
	double DataResolution;
	int NoDataValue;



	// Junction information
	int NJunctions;						// the number of junctions
	vector<int> SourcesVector;			// a list of the baselevel junctions. Note: these are not the
										// node indices. To find the node indices one uses these
										// values as an index into the JunctionVector
	vector<int> SourcesBaseLevelNodes;	// this vector lists the node index of the base level nodes
										// that have a source within their catchements.
	vector<int> JunctionVector;			// a list of the junctions. It is an index into the
										// nodevector from the FlowInfo object.
	vector<int> StreamOrderVector;		// the stream order of the junction node/link
										// note that each junction has one and only one receiver
										// junction so the stream order of a junction node
										// will apply to all nodes along the path to the next junction
	vector<int> BaseLevelNodeList;		// a list of base level nodes
	vector<int> BLBasinVector;			// a vector that give the baselevel index of each junction
										// node
	vector<int> NDonorsVector;			// stores the number of donors to each node
	vector<int> ReceiverVector;			// stores the node index of the receiving node
	vector<int> DeltaVector;			// stores the delta vector which proivides is used
										// to index into the donor stack and order contributing nodes
										// see Braun and Willett
	vector<int> DonorStackVector;		// This is a vector that stores the donor nodes of of the nodes
										// and is indexed by the DeltaVector
	vector<int> SVector;				// this vector is used to caluculate flow accumulation; for each
										// base level node it progresses from a hilltop to a confluence and then
										// jumps to the next hilltop so that by cascading down through
										// the node indices in this list one can quickly calcualte
										// drainage area, discharge, sediment flux, etc.
	vector<int> SVectorIndex;			// this points to the starting point in the S vector of each node
	vector<int> NContributingJunctions;	// the number of contributing junctions !!INCULDING SELF!! to a current
										// pixel. It is used in conjunction with the SVectorIndex to build
										// basins upslope of any and all nodes in the junction list


	// the following arrays are for keeping track of the junctions. For large DEMs this will be quite memory intensive
	// it might be sensible to try to devise a less data intensive method in the future.
	// one could do it with much less memory but that would involve searching
	Array2D<int> StreamOrderArray;		// this array stores the strem indices of all the channels
	Array2D<int> JunctionArray;			// this array stores a counter; if zero there is no junction
										// if 1 it is a junction unvisted by the junction gathering algorithm
										// if 2 or more it is a previously visited junction
	Array2D<int> JunctionIndexArray;	// this is an array where the elements are nodata if there is no junction and
										// an integer

	private:
	void create(vector<int> Sources, LSDFlowInfo& FlowInfo);
};

#endif
