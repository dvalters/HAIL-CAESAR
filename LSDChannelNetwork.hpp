/** @file LSDChannelNetwork.hpp
@author Simon M. Mudd, University of Edinburgh
@author David Milodowski, University of Edinburgh
@author Martin D. Hurst, British Geological Survey
@author Stuart W. D. Grieve, University of Edinburgh
@author Fiona Clubb, University of Edinburgh

@version Version 0.0.1
@brief Object to create a channel network from an LSDFlowInfo object.
@details This object is built around Braun and Willett's fastscape algorithm and
contains a number of analysis tools built around drainage networks.

@date 30/08/2012
*/

//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------

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

///@brief Object to create a channel network from an LSDFlowInfo object.
class LSDChannelNetwork
{
	public:
	/// @brief This defines a channel network based on a FlowInfo object and a list of source nodes.
  /// @param FlowInfo LSDFlowInfo object.
  /// @param Sources vector of source nodes.
	LSDChannelNetwork(vector<int> Sources, LSDFlowInfo& FlowInfo)
											{ create(Sources, FlowInfo); }

  ///@brief Recursive add_to_stack routine to build the junction tree, from Braun and Willett (2012)
  ///equations 12 and 13.
  ///@param lm_index Integer
  ///@param j_index Integer
  ///@param bl_node Integer
  void add_to_stack(int lm_index, int& j_index, int bl_node);

	// this returns all the upstream junction of a junction_number_outlet
	/// @brief This returns all the upstream junction of a junction_number_outlet.
  /// @param junction_number_outlet Integer of junction of interest. 
  /// @return integer vector containing all the junction numbers upslope
  /// of the chosen junction.
	vector<int> get_upslope_junctions(int junction_number_outlet);
	/// @brief This function maps a junction onto the indexing of the upslope junction list.
  ///
  /// @details If you get an upslope junction list the indexing starts at the furthest downslope
  /// junction. All of the junction pointing refers to the master junction list however.
  /// @param upslope_junctions Vector of upslope junctions of interest.
  /// @param junction Integer of junction of interest.
  /// @return Vector of mapped junctions.
	int map_junction_to_upslope_junction_list(vector<int> upslope_junctions, int junction);

	// functions for finding specific basins
	/// @brief This function returns the maximum stream order in the DEM.
  /// @return Maximum stream order as an integer. 
  int get_maximum_stream_order();

	// this prints the link array to raster
	/// @brief This sends the StreamOrderArray to a LSDIndexRaster. 
  /// @return LSDIndexRaster of StreamOrderArray.
	LSDIndexRaster StreamOrderArray_to_LSDIndexRaster();
	/// @brief This sends the JunctionArray to a LSDIndexRaster. 
  /// @return LSDIndexRaster of JunctionArray.
	LSDIndexRaster JunctionArray_to_LSDIndexRaster();
	/// @brief This sends the JunctionIndexArray to a LSDIndexRaster. 
  /// @return LSDIndexRaster of JunctionIndexArray.
	LSDIndexRaster JunctionIndexArray_to_LSDIndexRaster();
	/// @brief Turns the StreamOrderArray into a binary rastser where 1 is channel and 0 is hillslope. 
  /// @return Binary LSDIndexRaster of the channel network.
	LSDIndexRaster StreamOrderArray_to_BinaryNetwork_LSDIndexRaster();

	// this gets the largest donor junction to the baselevel nodes so that you can
	// automate basin selection (e.g. for use with chi analysis)
	/// @brief This gets the largest donor junction to the baselevel nodes so that you can
	/// automate basin selection.
  ///
  /// @details This function returns a integer vector containing the junction number of the largest
  /// donor catchment (i.e. donor junction with greatest drainage area) upslope of each
  /// baselevel node. These can then be used as the starting locations for performing chi
  /// analysis.
  ///
  /// IMPORTANT: the junctions always point downstream since they can have one and only
  /// one receiver. However, for a basin of given order, this starts just upstream of the
  /// confluence to the next basin order. So the basin <b>INCLUDES</b> the channel flowing
  /// downstream to the penultamite node.
  /// @return Integer vector containing the junction number of the largest donor catchment.
  /// @author MDH 
  /// @date 19/6/13
  vector<int> get_BaseLevel_DonorJunctions();

	/// @brief Get Junction number at a location.
  /// @param row Integer row index.
  /// @param col Integer column index.
  /// @return Junction number at location row,col.
  int retrieve_junction_number_at_row_and_column(int row,int col)
										{ return JunctionIndexArray[ row ][ col ]; }

	/// @brief Function for printing out the longest channel upstream of a point.
  /// @param outlet_junction
  /// @param FInfo LSDFlowInfo object.
  /// @param dist_code 
  /// @param dist_from_outlet
  void print_longest_channel(int outlet_junction, LSDFlowInfo& FInfo, LSDIndexRaster& dist_code,
									LSDRaster& dist_from_outlet);

	/// @brief Prints the information about the junctions to file.
  /// @param filename Output filename to be appended with '.txt'.
	void print_junction_info_vectors(string filename);

	/// @brief This generates an LSDChannelIndex object given a junction.
  ///
  /// @details NOTE: junctions start at the upstream end of the channel section.
  /// @param start_junction Junction to extract the channel from.
  /// @param FlowInfo LSDFlowInfo object. 
  /// @return The channel for the given junction.
	LSDIndexChannel generate_link_index_channel_from_junction(int start_junction,
													LSDFlowInfo& FlowInfo);

	/// @brief This function extracts the longest channel originating from a junction number
  /// outlet_junction. 
  /// @param outlet_junction Outlet of junction.
  /// @param FInfo LSDFlowInfo object.
  /// @param dist_from_outlet Distance from outlet junction.
  /// @return LSDIndexRaster of the longest channel.
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
	/// @brief This generates the longest channel in a basin. 
  ///
  /// @details The basin starts where a channel of some order intersects with a 
  /// channel of higher order. So the bain includes the basin junction, but also 
  /// the channel flowing downstream from this basin junction. It starts from the
  /// node of the reciever junction, so if one were to extract the basin from
  /// this node one would get a basin that starts one node upstream from the lowest node in this.
  /// @param basin_junction
  /// @param FInfo LSDFlowInfo object.
  /// @param dist_from_outlet
  /// @return LSDIndexRaster of the longest channel.
	LSDIndexChannel generate_longest_index_channel_in_basin(int basin_junction, LSDFlowInfo& FInfo,
																LSDRaster& dist_from_outlet);

	/// @brief This extracts the junction numbers, in a vector of integers, of all basins of a
	/// given order.
  ///
  /// @details For basins, the basin includes nodes downstream of the basinJunction,
	/// until the penulatmite node in this downstream channel.
  ///
  /// IMPORTANT: the junctions always point downstream since they can have one and only
  /// one receiver. However, for a basin of given order, this starts just upstream of the
  /// confluence to the next basin order. So the basin <b>INCLUDES</b> the channel flowing
  /// downstream to the penultamite node. 
  /// @param BasinOrder Integer of the basin order.
  /// @param FlowInfo LSDFlowInfo object.
  /// @return Vector of junctions of basins of given order.
	vector<int> extract_basins_order_outlet_junctions(int BasinOrder, LSDFlowInfo& FlowInfo);

	// this function gets the outlet node of a list of basins, contined within the
	// BasinOutletJunctions parameter. The basin outlet node is _DOWNSTREAM_ from
	// the outlet junction, it is the penultamite node of the channel index.

	/// @brief this function gets the outlet node of a list of basins.
  ///
  /// @details The basin outlet node is _DOWNSTREAM_ from the outlet junction, 
  /// it is the penultamite node of the channel index.
  /// @param BasinOutletJunctions 
  /// @param FlowInfo LSDFlowInfo object.
  /// @return Vector of outlet nodes of basins.
	vector<int> extract_basins_order_outlet_nodes(vector<int>& BasinOutletJunctions, LSDFlowInfo& FlowInfo);
 
	/// @brief This function gets tributaries along a continous channel.
  ///
  /// @details What it does is goes down the index channel looking at the JunctionIndexArray
	/// to see if there is a junction. If it hits a junction then all the contributing junction
	/// it overwrites two vectors: \n
	/// tributary_junctions, which lists all junctions whose reciever is the main
	/// stem and nodes_on_main_stem_of_tributaries, which are the njodes on the 
  /// main_stem LSDIndexChannel where the tributaries intersect the main stem
	/// this second vector is used to calcualte the chi values of the downstream node
	/// of the tributaries.
  /// @param MainStem LSDIndexChannel of the main stem.
  /// @param FlowInfo LSDFlowInfo object. 
  /// @param tributary_junctions
  /// @param nodes_on_main_stem_of_tributaries
	void extract_tributary_junctions_to_main_stem(LSDIndexChannel& MainStem, LSDFlowInfo& FlowInfo,
																		vector<int>& tributary_junctions,
																		vector<int>& nodes_on_main_stem_of_tributaries);
 
	/// @brief this function gets the tributary junctions upstream of the starting_junction based on
	/// pruning criteria. 
  ///
  /// @details This function extracts tributaries juncions to the main stem of the 
  /// channel, then selects a sample based on various criteria set by an integer 
  /// called pruning switch \n\n
  /// pruning_switch == 0  channels are only added if they exceed a threshold drainage area \n
  /// pruning_switch == 1  channels are only added if the ratio between them and the mainstem exceeds a certain value (pruning_threshold)\n
  /// pruning_switch == 2	channels are only added if the ratio between them and the area of the	mainstem _at the junction_ exceeds a certain value\n
///// pruning_switch == 3 channels are only added if the channel order is >= threshold.
  /// @param FlowInfo LSDFlowInfo object.
  /// @param ChannelNetwork LSDChannelNetwork object.
  /// @param starting_junction
  /// @param DistanceFromOutlet LSDIndexRaster of outlet distances.
  /// @param pruning_switch
  /// @param pruning_threshold 
  /// @return Pruned tributary junctions.
  /// @author DTM 
  /// @date 30/04/2013
	vector<int> get_pruned_tributaries_from_main_stem(LSDFlowInfo& FlowInfo, LSDChannelNetwork& ChannelNetwork,
			int starting_junction, LSDRaster& DistanceFromOutlet, int pruning_switch, double pruning_threshold);

	/// @brief This function extracts basin nodes according to their accumulated drainage area.
  /// @param DrainageAreaThreshold Threshold drainage area.
  /// @param FlowInfo LSDFlowInfo object. 
  /// @return Vector of basin nodes.
  /// @author DTM 
  /// @date 07/05/2013
	vector<int> extract_basin_nodes_by_drainage_area(double DrainageAreaThreshold, LSDFlowInfo& FlowInfo);

	/// @brief This function gets the node indices of outlets of basins of a certain order
  ///
  /// @details IMPORTANT: The junctions always point downstream since they can have one and only
	/// one receiver. However, for a basin of given order, this starts just upstream of the
	/// confluence to the next basin order. So the basin <b>INCLUDES</b> the channel flowing
	/// downstream to the penultamite node.
	///
  /// @param basin_junction Reference number for printing to the IndexRaster.
  /// @param basin_reference_number
  /// @param FlowInfo LSDFlowInfo object.
  /// @return LSDIndexRaster of extracted basin. 
	LSDIndexRaster extract_basin_from_junction(int basin_junction, int basin_reference_number, LSDFlowInfo& FlowInfo);


  LSDIndexRaster extract_basins_from_junction_vector(vector<int> basin_junctions, LSDFlowInfo& FlowInfo);

  /// @brief Basin extraction - extracts all drainage basins of specified stream order.
  /// @param BasinOrder Integer basin order to extract.
  /// @param FlowInfo LSDFlowInfo object.
  /// @return LSDIndexRaster of extracted basins.
  /// @author DTM 
  /// @date 17/10/2012
  LSDIndexRaster ExtractBasinsOrder(int BasinOrder, LSDFlowInfo& FlowInfo);

  /// @brief Ridge network extraction - extracts ridge network, defined as boundaries
  /// between two basins of the same stream order.
  ///
  /// @details This function extracts the ridge network by defining it as the co-boundaries
  /// of basins of equivalent order, for all basin orders within the landscape.
  /// This is relatively trivial since in each array containing the basins of the
  /// same order, each basin is labelled with a unique identifier, thus co-
  /// boundaries are found by locating pixels that neighbour pixels from another
  /// basin of the same order.
  ///
  /// Updated to return an LSDRaster object as ridges can now be assigned CHT values,
  /// using LSDRaster::RidgeSample, which are not integers. - SWDG
  /// @param FlowInfo LSDFlowInfo object.
  /// @return LSDRaster of ridges.
  /// @author DTM 
  /// @date 18/10/2012
  LSDRaster ExtractRidges(LSDFlowInfo& FlowInfo);
  /// @brief 
  ///
  /// @details This overloaded function extracts the ridge network for a defined stream
  /// order, passed in by the user.
  ///
  /// Updated to return an LSDRaster object as ridges can now be assigned CHT values,
  /// using LSDRaster::RidgeSample, which are not integers. - SWDG
  /// @param FlowInfo LSDFlowInfo object.
  /// @param min_order Lowest order of ridges to extract. 
  /// @param max_order Highest order of ridges to extract. 
  /// @return LSDRaster of ridges.
  /// @author DTM, SWDG 
  /// @date 18/10/2012, 28/03/2013
  LSDRaster ExtractRidges(LSDFlowInfo& FlowInfo, int& min_order, int& max_order);
  /// @brief this last function gets the hilltops: ridges limited by a maximum slope.
  ///
  /// @details Resticts ridgeline to part of ridge network where the slope is less than a
  /// threshold value.
  /// @param RidgeRaster LSDIndexRaster of extracted ridges.
  /// @param SlopeRaster LSDRaster of slope.
  /// @param MaxSlope Maximum threshold slope value.
  /// @return LSDIndexRaster of hilltops. 
  /// @author DTM 
  /// @date 01/04/2013
  LSDIndexRaster ExtractHilltops(LSDIndexRaster& RidgeRaster, LSDRaster& SlopeRaster, double MaxSlope);

	/// @brief This function iterates through the junction nodes and assigns the unique
	/// junction ID to every stream pixel.
  ///
  /// @details This can be used with the LSDRaster
  /// hilltop_flow_routing function to assign a unique ID to each hilltop
  /// section tying it to a specific section of the channel network. 
  /// @param flowinfo LSDFlowInfo object.
  /// @return LSDIndexRaster of the indexed channel newtork.
  /// @author SWDG
  /// @date 04/04/13
  LSDIndexRaster ChannelIndexer(LSDFlowInfo& flowinfo);

	// simple functions for getting streams. These do not return channel data elements but
	// instead return an LSDIndexRaster with the streams of a given order retained
	
  /// @brief Quick and dirty way to get channels of a defined stream order.
  ///
  /// @details No input error handling, will return an LSDIndexRaster of NoDataValues if an erroneous order is passed in.
  /// @param order Integer of the required stream order.
  /// @return LSDIndexRaster of the desired channels.
  /// @author SWDG 
  /// @date 04/13
  LSDIndexRaster GetStreams(int order);
  /// @brief Quick and dirty way to get channels of a defined range of stream orders.
  ///
  /// @details No input error handling, will return an LSDIndexRaster of NoDataValues if an erroneous order is passed in.
  /// @param min_order Integer of the miniumum required stream order.
  /// @param max_order Integer of the max required stream order.
  /// @return LSDIndexRaster of the desired channels.
  /// @author SWDG 
  /// @date 04/13
	LSDIndexRaster GetStreams(int min_order, int max_order);

	/// @brief Function to test whether a junction's upstream nodes border areas of No Data
  /// important to ensure basins are not being artificially truncated.
  /// @param FlowInfo LSDFlowInfo object.
  /// @param input_junction Junction to be tested.
  /// @return Boolean indicating if no data values are present or not: \n
  /// false (0) = only good data values \n
  /// true (1) = no data values present \n
  /// @author SWDG 
  /// @date 27/06/2013
	bool node_tester(LSDFlowInfo& FlowInfo, int input_junction);

	// Get functions

	/// @return Number of rows as an integer.
	int get_NRows() const				{ return NRows; }
	/// @return Number of columns as an integer.
  int get_NCols() const				{ return NCols; }
  /// @return Minimum X coordinate as an integer.
	double get_XMinimum() const			{ return XMinimum; }
	/// @return Minimum Y coordinate as an integer.
	double get_YMinimum() const			{ return YMinimum; }
	/// @return Data resolution as an integer.                            
	double get_DataResolution() const	{ return DataResolution; }
	/// @return No Data Value as an integer.
	int get_NoDataValue() const			{ return NoDataValue; }

  /// @details Not bounds checked!! Need to adjust!!
  /// @param junction integer node index. 
  /// @return Integer node of junction.  
	int get_Node_of_Junction(int junction) const
										{ return JunctionVector[junction]; }

  /// @details Not bounds checked!! Need to adjust!!
  /// @param junction integer reciever index. 
  /// @return Integer reciever of junction.
	int get_Receiver_of_Junction(int junction) const
										{ return ReceiverVector[junction]; }

	protected:

	///Number of rows.
  int NRows;
  ///Number of columns.
	int NCols;
	///Minimum X coordinate.
  double XMinimum;
	///Minimum Y coordinate.
	double YMinimum;

	///Data resolution.
	double DataResolution;
	///No data value.
	int NoDataValue;

	// Junction information
	/// The number of junctions
  int NJunctions;
  /// @brief A list of the baselevel junctions.
  /// 
  /// @details Note: these are not the node indices. To find the node indices one uses these values as an index into the JunctionVector.						
	vector<int> SourcesVector;
  /// This vector lists the node index of the base level nodes that have a source within their catchements.			
	vector<int> BaseLevelJunctions;		
	/// A list of the junctions. It is an index into the nodevector from the FlowInfo object.
  vector<int> JunctionVector;			
	/// @brief The stream order of the junction node/link.
  ///
  ///@details Note that each junction has one and only one receiver junction so 
  ///the stream order of a junction node will apply to all nodes along the path 
  ///to the next junction.
  vector<int> StreamOrderVector;
	/// A vector that give the baselevel index of each junction node.
  vector<int> BLBasinVector;			
	/// Stores the number of donors to each node.
  vector<int> NDonorsVector;
  /// Stores the node index of the receiving node.
	vector<int> ReceiverVector;
	/// Stores the delta vector which is used to index into the donor stack and order contributing nodes see Braun and Willett [2012].
  vector<int> DeltaVector;
	/// This is a vector that stores the donor nodes of of the nodes and is indexed by the DeltaVector.
  vector<int> DonorStackVector;		
  
  ///@brief This vector is used to calculate flow accumulation.
  ///
  ///@details For each base level node it progresses from a hilltop to a confluence
  ///and then	jumps to the next hilltop so that by cascading down through
	///the node indices in this list one can quickly calculate drainage area, 
  ///discharge, sediment flux, etc.
	vector<int> SVector;
	/// This points to the starting point in the S vector of each node.
  vector<int> SVectorIndex;			
	/// @brief The number of contributing junctions !!INCULDING SELF!! to a current pixel. 
  ///
  ///@details It is used in conjunction with the SVectorIndex to build basins upslope of any and all nodes in the junction list.
  vector<int> NContributingJunctions;	


	// the following arrays are for keeping track of the junctions. For large DEMs this will be quite memory intensive
	// it might be sensible to try to devise a less data intensive method in the future.
	// one could do it with much less memory but that would involve searching
	
	/// This array stores the stream indices of all the channels.
	Array2D<int> StreamOrderArray;		
	/// @brief This array stores a junction counter.
  ///
  /// @details If zero there is no junction \n
	/// if 1 it is a junction unvisted by the junction gathering algorithm \n
	/// if 2 or more it is a previously visited junction
  Array2D<int> JunctionArray;			
	/// This is an array where the elements are nodata if there is no junction and an integer indicating the junction number.
  Array2D<int> JunctionIndexArray;	

	private:
	void create(vector<int> Sources, LSDFlowInfo& FlowInfo);
};

#endif
