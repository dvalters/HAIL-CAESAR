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
#include <string>
#include "TNT/tnt.h"
#include "LSDFlowInfo.hpp"
#include "LSDRaster.hpp"
#include "LSDIndexChannel.hpp"
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
	int map_junction_to_upslope_junction_list(vector<int> upslope_junctions, int junction);

	// functions for finding specific basins
	int get_maximum_stream_order();

	// this prints the link array to raster
	LSDIndexRaster StreamOrderArray_to_LSDIndexRaster();
	LSDIndexRaster JunctionArray_to_LSDIndexRaster();
	LSDIndexRaster JunctionIndexArray_to_LSDIndexRaster();
	LSDIndexRaster StreamOrderArray_to_BinaryNetwork_LSDIndexRaster();

	// this gets the largest donor junction to the baselevel nodes so that you can
	// automate basin selection (e.g. for use with chi analysis)
	vector<int> get_BaseLevel_DonorJunctions();

	int retrieve_junction_number_at_row_and_column(int row,int col)
										{ return JunctionIndexArray[ row ][ col ]; }

	// function for printing out the longest channel upstream of a point
	void print_longest_channel(int outlet_junction, LSDFlowInfo& FInfo, LSDIndexRaster& dist_code,
									LSDRaster& dist_from_outlet);

	// this prints the information about the junctions to file
	void print_junction_info_vectors(string filename);

	// this generates an LSDChannelIndex object given a junction
	// NOTE: junctions start at the upstream end of the channel section.
	LSDIndexChannel generate_link_index_channel_from_junction(int start_junction,
													LSDFlowInfo& FlowInfo);

	// this generates the longest channel originating from junction 'outlet_junction'
	LSDIndexChannel generate_longest_index_channel_from_junction(int outlet_junction,
																LSDFlowInfo& FInfo,
																LSDRaster& dist_from_outlet);

	// this generates the longest channel in a basin. The basin starts where a channel of
	// some order intersects with a channel of higher order. So the bain includes the
	// basin junction, but also the channel flowing downstream from this basin
	// junction
	// It starts from the node of the reciever junction, so if one were to extract
	// the basin from this node one would get a basin that starts one node upstream from
	// the lowest node in this
	LSDIndexChannel generate_longest_index_channel_in_basin(int basin_junction, LSDFlowInfo& FInfo,
																LSDRaster& dist_from_outlet);

	// this extracts the junction numbers, in a vector of integers, of all basins of a
	// given order. For basins, the basin includes nodes downstream of the basinJunction,
	// until the penulatmite node in this downstream channel. This penultamite node
	// can be extracted with the
	vector<int> extract_basins_order_outlet_junctions(int BasinOrder, LSDFlowInfo& FlowInfo);

	// this function gets the outlet node of a list of basins, contined within the
	// BasinOutletJunctions parameter. The basin outlet node is _DOWNSTREAM_ from
	// the outlet junction, it is the penultamite node of the channel index.
	vector<int> extract_basins_order_outlet_nodes(vector<int>& BasinOutletJunctions, LSDFlowInfo& FlowInfo);

	// this function gets tributaries along a continous channel.
	// What it does is goes down the index channel looking at the JunctionIndexArray
	// to see if there is a junction. If it hits a junction then all the contributing junction
	//
	// it overwrites two vectors:
	// tributary_junctions, which lists all junctions whose reciever is the main
	// stem
	// and
	// nodes_on_main_stem_of_tributaries, which are the njodes on the main_stem LSDIndexChannel
	// where the tributaries intersect the main stem
	// this second vector is used to calcualte the chi values of the downstream node
	// of the tributaries
	void extract_tributary_junctions_to_main_stem(LSDIndexChannel& MainStem, LSDFlowInfo& FlowInfo,
																		vector<int>& tributary_junctions,
																		vector<int>& nodes_on_main_stem_of_tributaries);


	// this function gets the tributary junctions upstream of the starting_junction based on
	// pruning criteria
	vector<int> get_pruned_tributaries_from_main_stem(LSDFlowInfo& FlowInfo, LSDChannelNetwork& ChannelNetwork,
			int starting_junction, LSDRaster& DistanceFromOutlet, int pruning_switch, double pruning_threshold);


	// this extracts juntions based on a threshold acumulated drainage area
	vector<int> extract_basin_nodes_by_drainage_area(double DrainageAreaThreshold, LSDFlowInfo& FlowInfo);


	// This function gets the node indices of outlets of basins of a certain order
	//
	// IMPORTANT: the junctions always point downstream since they can have one and only
	// one receiver. However, for a basin of given order, this starts just upstream of the
	// confluence to the next basin order. So the basin ##INCLUDES## the channel flowing
	// downstream to the penultamite node
	//
	// the basin_reference_number is just a reference number for printing to
	// the IndexRaster
	LSDIndexRaster extract_basin_from_junction(int basin_junction, int basin_reference_number, LSDFlowInfo& FlowInfo);

	// Basin extraction - extracts all drainage basins of specified stream order
  	// DTM 17/10/2012
  	LSDIndexRaster ExtractBasinsOrder(int BasinOrder, LSDFlowInfo& FlowInfo);

  	// Ridge network extraction - extracts ridge network, defined as boundaries
  	// between two basins of the same stream order
  	// DTM 18/10/2012
  	LSDRaster ExtractRidges(LSDFlowInfo& FlowInfo);
  	LSDRaster ExtractRidges(LSDFlowInfo& FlowInfo, int& min_order, int& max_order);
  	// this last function gets the hilltops: ridges limited by a maximum slope
  	LSDIndexRaster ExtractHilltops(LSDIndexRaster& RidgeRaster, LSDRaster& SlopeRaster, double MaxSlope);

	// This function iterates through the junction nodes and assigns the unique
	// junction ID (q) to every stream pixel. This can be used with the LSDRaster
	// hilltop_flow_routing function to assign a unique ID to each hilltop
	// section tying it to a specific section of the channel network.
	LSDIndexRaster ChannelIndexer(LSDFlowInfo& flowinfo);


	// simple functions for getting streams. These do not return channel data elements but
	// instead return an LSDIndexRaster with the streams of a given order retained
	LSDIndexRaster GetStreams(int order);
	LSDIndexRaster GetStreams(int min_order, int max_order);

	// thins functions looks to see if channels upstream nodes are bordering nodata to make
	// sure channels are not truncated.
	bool node_tester(LSDFlowInfo& FlowInfo, int input_junction);

	// get functions
	// these get data elements
	int get_NRows() const				{ return NRows; }
	int get_NCols() const				{ return NCols; }
	double get_XMinimum() const			{ return XMinimum; }
	double get_YMinimum() const			{ return YMinimum; }
	double get_DataResolution() const	{ return DataResolution; }
	int get_NoDataValue() const			{ return NoDataValue; }

	// these two are not bounds checked!! Need to adjust!!
	int get_Node_of_Junction(int junction) const
										{ return JunctionVector[junction]; }

	int get_Receiver_of_Junction(int junction) const
										{ return ReceiverVector[junction]; }

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
	vector<int> BaseLevelJunctions;		// this vector lists the node index of the base level nodes
										// that have a source within their catchements.
	vector<int> JunctionVector;			// a list of the junctions. It is an index into the
										// nodevector from the FlowInfo object.
	vector<int> StreamOrderVector;		// the stream order of the junction node/link
										// note that each junction has one and only one receiver
										// junction so the stream order of a junction node
										// will apply to all nodes along the path to the next junction
	vector<int> BLBasinVector;			// a vector that give the baselevel index of each junction
										// node
	vector<int> NDonorsVector;			// stores the number of donors to each node
	vector<int> ReceiverVector;			// stores the node index of the receiving node
	vector<int> DeltaVector;			// stores the delta vector which is used
										// to index into the donor stack and order contributing nodes
										// see Braun and Willett
	vector<int> DonorStackVector;		// This is a vector that stores the donor nodes of of the nodes
										// and is indexed by the DeltaVector
	vector<int> SVector;				// this vector is used to calculate flow accumulation; for each
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
	Array2D<int> StreamOrderArray;		// this array stores the stream indices of all the channels
	Array2D<int> JunctionArray;			// this array stores a counter; if zero there is no junction
										// if 1 it is a junction unvisted by the junction gathering algorithm
										// if 2 or more it is a previously visited junction
	Array2D<int> JunctionIndexArray;	// this is an array where the elements are nodata if there is no junction and
										// an integer indicating the junction number

	private:
	void create(vector<int> Sources, LSDFlowInfo& FlowInfo);
};

#endif
