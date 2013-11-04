//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDChannelNetwork
// Land Surface Dynamics ChannelNetwork
//
// An object within the University
//  of Edinburgh Land Surface Dynamics group topographic toolbox
//  for organizing channel routing under the Fastscape algorithm
//  (see Braun and Willett, Geomorphology 2013, v180, p 170-179)
//  It uses the algorithm to create channel junction networks
//  that can be searched for network connectivity
//
//
// Developed by:
//  Simon M. Mudd
//  Martin D. Hurst
//  David T. Milodowski
//  Stuart W.D. Grieve
//  Declan A. Valters
//  Fiona Clubb
//
// Copyright (C) 2013 Simon M. Mudd 2013
//
// Developer can be contacted by simon.m.mudd _at_ ed.ac.uk
//
//    Simon Mudd
//    University of Edinburgh
//    School of GeoSciences
//    Drummond Street
//    Edinburgh, EH8 9XP
//    Scotland
//    United Kingdom
//
// This program is free software;
// you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY;
// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the
// GNU General Public License along with this program;
// if not, write to:
// Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301
// USA
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

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
#include "LSDChannel.hpp"
#include "LSDStatsTools.hpp"
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
  /// @author SMM
  /// @date 01/09/12
	LSDChannelNetwork(vector<int> Sources, LSDFlowInfo& FlowInfo)
											{ create(Sources, FlowInfo); }

  ///@brief Recursive add_to_stack routine to build the junction tree, from Braun and Willett (2012)
  ///equations 12 and 13.
  ///@param lm_index Integer
  ///@param j_index Integer
  ///@param bl_node Integer
  /// @author SMM
  /// @date 01/09/12
  void add_to_stack(int lm_index, int& j_index, int bl_node);

	// this returns all the upstream junction of a junction_number_outlet
	/// @brief This returns all the upstream junction of a junction_number_outlet.
  /// @param junction_number_outlet Integer of junction of interest.
  /// @return integer vector containing all the junction numbers upslope
  /// of the chosen junction.
  /// @author SMM
  /// @date 01/09/12
	vector<int> get_upslope_junctions(int junction_number_outlet);
	/// @brief This function maps a junction onto the indexing of the upslope junction list.
  ///
  /// @details If you get an upslope junction list the indexing starts at the furthest downslope
  /// junction. All of the junction pointing refers to the master junction list however.
  /// @param upslope_junctions Vector of upslope junctions of interest.
  /// @param junction Integer of junction of interest.
  /// @return Vector of mapped junctions.
  /// @author SMM
  /// @date 01/09/12
	int map_junction_to_upslope_junction_list(vector<int> upslope_junctions, int junction);

	// functions for finding specific basins
	/// @brief This function returns the maximum stream order in the DEM.
  /// @return Maximum stream order as an integer.
  /// @author SMM
  /// @date 01/09/12
  int get_maximum_stream_order();
  
  /// @brief This gets the junction number of a given node.
  /// @param Node
  /// @return JunctionNumber
  /// @author FC
  /// @date 31/10/13
  int get_Junction_of_Node(int Node, LSDFlowInfo& FlowInfo);

	// this prints the link array to raster
	/// @brief This sends the StreamOrderArray to a LSDIndexRaster.
  /// @return LSDIndexRaster of StreamOrderArray.
  /// @author SMM
  /// @date 01/09/12
	LSDIndexRaster StreamOrderArray_to_LSDIndexRaster();
	/// @brief This sends the JunctionArray to a LSDIndexRaster.
  /// @return LSDIndexRaster of JunctionArray.
  /// @author SMM
  /// @date 01/09/12
	LSDIndexRaster JunctionArray_to_LSDIndexRaster();
	/// @brief This sends the JunctionIndexArray to a LSDIndexRaster.
  /// @return LSDIndexRaster of JunctionIndexArray.
  /// @author SMM
  /// @date 01/09/12
	LSDIndexRaster JunctionIndexArray_to_LSDIndexRaster();
	/// @brief Turns the StreamOrderArray into a binary rastser where 1 is channel and 0 is hillslope.
  /// @return Binary LSDIndexRaster of the channel network.
  /// @author SMM
  /// @date 01/09/12
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
  /// @author SMM
  /// @date 01/09/12
  int retrieve_junction_number_at_row_and_column(int row,int col)
										{ return JunctionIndexArray[ row ][ col ]; }

	/// @brief Function for printing out the longest channel upstream of a point.
  /// @param outlet_junction
  /// @param FInfo LSDFlowInfo object.
  /// @param dist_code
  /// @param dist_from_outlet
  /// @author SMM
  /// @date 01/09/12
  void print_longest_channel(int outlet_junction, LSDFlowInfo& FInfo, LSDIndexRaster& dist_code,
									LSDRaster& dist_from_outlet);

	/// @brief Prints the information about the junctions to file.
  /// @param filename Output filename to be appended with '.txt'.
  /// @author SMM
  /// @date 01/09/12
	void print_junction_info_vectors(string filename);

	/// @brief This generates an LSDChannelIndex object given a junction.
  ///
  /// @details NOTE: junctions start at the upstream end of the channel section.
  /// @param start_junction Junction to extract the channel from.
  /// @param FlowInfo LSDFlowInfo object.
  /// @return The channel for the given junction.
  /// @author SMM
  /// @date 01/09/12
	LSDIndexChannel generate_link_index_channel_from_junction(int start_junction,
													LSDFlowInfo& FlowInfo);

	/// @brief This function extracts the longest channel originating from a junction number
  /// outlet_junction.
  /// @param outlet_junction Outlet of junction.
  /// @param FInfo LSDFlowInfo object.
  /// @param dist_from_outlet Distance from outlet junction.
  /// @return LSDIndexRaster of the longest channel.
  /// @author SMM
  /// @date 01/09/12
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
  /// @author SMM
  /// @date 01/09/12
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
  /// @author SMM
  /// @date 01/09/12
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
  /// @author SMM
  /// @date 01/09/12
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
  /// @author SMM
  /// @date 01/09/12
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
  /// @param basin_junction Junction of basin to be extracted.
  /// @param basin_reference_number Reference number for printing to the IndexRaster.
  /// @param FlowInfo LSDFlowInfo object.
  /// @return LSDIndexRaster of extracted basin.
  /// @author SMM
  /// @date 01/09/12
	LSDIndexRaster extract_basin_from_junction(int basin_junction, int basin_reference_number, LSDFlowInfo& FlowInfo);

  /// @brief This function gets the an LSDIndexRaster of basins draining from a vector of junctions.
  ///
  /// @details IMPORTANT: The junctions always point downstream since they can have one and only
	/// one receiver. However, for a basin of given order, this starts just upstream of the
	/// confluence to the next basin order. So the basin <b>INCLUDES</b> the channel flowing
	/// downstream to the penultamite node.
	///
  /// @param basin_junctions Vector of junction numbers of basins to be extracted.
  /// @param FlowInfo LSDFlowInfo object.
  /// @return LSDIndexRaster of extracted basin.
  /// @author SMM
  /// @date 01/09/12
  LSDIndexRaster extract_basins_from_junction_vector(vector<int> basin_junctions, LSDFlowInfo& FlowInfo);

  /// @brief Basin extraction - extracts all drainage basins of specified stream order.
  /// @param BasinOrder Integer basin order to extract.
  /// @param FlowInfo LSDFlowInfo object.
  /// @return LSDIndexRaster of extracted basins.
  /// @author DTM
  /// @date 17/10/2012
  LSDIndexRaster ExtractBasinsOrder(int BasinOrder, LSDFlowInfo& FlowInfo);

  /// @brief This function extracts the juctions of all non-beheaded drainage basins of a given order, n.
  /// @param BasinOrder Integer basin order to extract.
  /// @param FlowInfo LSDFlowInfo object.
  /// @return Vector of junction indexes.
  /// @author SWDG
  /// @date 24/10/2013  
  vector<int> ExtractBasinJunctionOrder(int BasinOrder, LSDFlowInfo& FlowInfo);

  /// @brief Get farthest upslope hilltops.
  ///
  /// @details This function looks at all the source junctions in a network
  ///  upstream of a given junction and returns the node index of the
  ///  hilltop node that is the farthest upstream from the source junction
  /// @param JunctionNumber the junction number upstream of which you want to search for sources
  /// @param FlowInfo the flow info object
  /// @param FlowDistance distance upslope
  /// @return vector<int> a vector of node indices to the ridge nodes that are farthest upslope
  /// of the sources
  /// @author SMM
  /// @date 26/09/2013
  vector<int> FindFarthestUpslopeHilltopsFromSources(int JunctionNumber, LSDFlowInfo& FlowInfo, LSDRaster& FlowDistance);

  /// @brief This function generates LSDChannels that run from the hilltops above
  /// all the sources of the junction JunctionNumber
  /// @author SMM
  /// @date 26/09/2013
  vector<int> GetChannelHeadsChiMethodFromJunction(int JunctionNumber,
                              int MinSegLength, double A_0, double m_over_n,
											        LSDFlowInfo& FlowInfo, LSDRaster& FlowDistance, LSDRaster& ElevationRaster);



	/// @brief This function returns all potential channel heads in a DEM. It looks for
	/// channel heads organized by a basin order which is fed to the code
	/// The basin order just determines how far downstream the algorithm looks for the 'fluvial'
	/// section.
	/// It returns a vector<int> of nodeindices where the channel heads are
	/// @return vector<int> a vector of node_indices of potential channel heads
  /// @author SMM
  /// @date 26/09/2013
	vector<int> GetChannelHeadsChiMethodBasinOrder(int BasinOrder,
                                      int MinSegLength, double A_0, double m_over_n,
									  LSDFlowInfo& FlowInfo, LSDRaster& FlowDistance,
									  LSDRaster& ElevationRaster);
									  
	/// @brief This function returns all potential channel heads in a DEM. It looks for
  /// channel heads based on the outlet junctions of the valleys (which are identified by looking 
  /// for portions of the landscape with 10 or more nodes with a high curvature that are linked)
	/// @param ValleyJunctions
	/// @param MinSegLength
	/// @param A_0
	/// @param m_over_n
	/// @param FlowInfo
	/// @param FlowDistance
	/// @param ElevationRaster
	/// @return vector<int> a vector of node_indices of potential channel heads
  /// @author FC
  /// @date 31/10/2013
  vector<int> GetChannelHeadsChiMethodFromValleys(Array2D<int>& ValleyJunctions,
                                      int MinSegLength, double A_0, double m_over_n,
									                    LSDFlowInfo& FlowInfo, LSDRaster& FlowDistance,
									                    LSDRaster& ElevationRaster);


  /// @brief This function returns a 2D array containing the locations of all pixels identified
  /// as being part of the channel using chi profiles.  It calculates the chi and elevation value
  /// of every pixel upstream of the given junction, then bins this data and calculates the pixels
  /// in the 95th percentile of each bin.  Any pixels above the 95th percentile are considered part
  /// of the channel, and any below are considered to be hillslopes.  This is the first part of the
  /// channel head prediction using chi profiles.
  /// @param JunctionNumber
  /// @param A_0
  /// @param m_over_n
  /// @param bin_width
  /// @param FlowInfo Flow Info object
  /// @param ElevationRaster
  /// @return Array2D<double> with channel pixels
  /// @author FC
  /// @date 01/10/2013
  Array2D<int> GetChannelHeadsChiMethodAllPixels(int JunctionNumber,
                                      double A_0, double m_over_n, double bin_width, LSDFlowInfo& FlowInfo,
                                      LSDRaster& ElevationRaster);


  /// @brief This function returns an integer vector with the node indexes of the furthest upstream
  /// pixels identified as being part of the channel using chi profiles.  It calculates the chi and
  /// elevation value of every pixel upstream of the given junction, then bins this data and calculates
  /// the pixels in the 95th percentile of each bin.  Any pixels above the 95th percentile are considered
  /// part of the channel, and any below are considered to be hillslopes.  This is the first part of the
  /// channel head prediction using chi profiles.
  /// @param JunctionNumber
  /// @param A_0
  /// @param m_over_n
  /// @param bin_width
  /// @param FlowInfo Flow Info object
  /// @param ElevationRaster
  /// @return vector<int> with source nodes
  /// @author FC
  /// @date 04/10/2013
  vector<int> GetSourceNodesChiMethodAllPixels(int JunctionNumber,
                                      double A_0, double m_over_n, double bin_width, LSDFlowInfo& FlowInfo,
                                      LSDRaster& ElevationRaster);
                                      
  // channel head identification
	/// @brief This function is used to predict channel head locations based on the method proposed by Pelletier (2013).
  ///
  /// @details It creates a contour curvature map and identifies channel heads as pixels greater
  /// than a user defined contour curvature threshold value, set by default at 0.1.  The threshold curvature
  /// can also be defined as a multiple of the standard deviation of the curvature.  Before this function is called
  /// the DEM must be filtered using the wiener filter in the LSDRasterSpectral object in order to remove high frequency
  /// noise.
  ///
  /// Reference: Pelletier (2013) A robust, two-parameter method for the extraction of drainage
  /// networks from high-resolution digital elevation models (DEMs): Evaluation using synthetic and real-world
  /// DEMs, Water Resources Research 49: 1-15
  ///
  /// @param tan_curv_threshold Double curvature threshold value.
  /// @param tan_curv_array 2D array of tangential curvature.
  /// @return 2D array of predicted channel head locations.
  /// @author FC
  /// @date 16/07/13
	vector<int> calculate_pelletier_channel_heads(double tan_curv_threshold, LSDFlowInfo& FlowInfo, Array2D<double>& tan_curv_array);
	
	/// @brief This function is used to identify concave portions of the landscape using a tangential curvature threshold.
  ///
  /// @details It defines the threshold based on a multiple of the standard deviation
  /// of the curvature.  It then identifies valleys in which there are a linked series of pixels
  /// which have a curvature value greater than the threshold, and finds the outlet junction number
  /// of this valley.  This can be passed to the channel head prediction algorithm using the chi
  /// method.
  ///
  /// @param FlowInfo LSDFlowInfo object
  /// @param tan_curv_array 2D array with curvature
  /// @param sources vector with sources of channel network
  /// @return Array2D<int> with nodes at the base of each of the valleys
  /// @author FC
  /// @date 29/10/2013
  Array2D<int> find_valleys(LSDFlowInfo& FlowInfo, Array2D<double>& tan_curv_array, vector<int> sources);
  
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



  /// @brief This last function gets the hilltops: ridges limited by a maximum slope.
  ///
  /// @details Resticts ridgeline to part of ridge network where the slope is less than a
  /// threshold value.
  ///
  /// Now outputs an LSDRaster to fall in line with other hilltop tools - SWDG 29/8/13
  /// @param RidgeRaster LSDIndexRaster of extracted ridges.
  /// @param SlopeRaster LSDRaster of slope.
  /// @param MaxSlope Maximum threshold slope value.
  /// @return LSDIndexRaster of hilltops.
  /// @author DTM
  /// @date 01/04/2013
  LSDRaster ExtractHilltops(LSDRaster& RidgeRaster, LSDRaster& SlopeRaster, double MaxSlope);

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

  /// SplitChannel
  /// @brief This function splits the channel into a series of segments, providing a
  /// convenient unit with which to analyse landscapes.  The user provides the
  /// TargetSegmentLength, which specifies how many nodes should be in each 
  /// segment, and a MinimumSegmentLength, which specifies the fewest permissable
  /// number of nodes.  Segments smaller than this are amalgamated into the 
  /// upstream segment.
  /// The algorithm loops through the sources and traces downstream, stopping a 
  /// segment after the target segment length, when the stream order increases (to
  /// preserve structure of drainage network), or when a channel pixel has already
  /// been visited.
  ///
  /// @param LSDFlowInfo object
  /// @param Sources: a vector of sources
  /// @param TargetSegmentLength (suggest 200 for 1m DEM)
  /// @param MinimumSegmentLength (suggest 100 for 1m DEM)
  /// @return LSDIndexRaster with channel segments labelled by unique ID
  /// @author DTM
  /// @date 23/10/2013
  LSDIndexRaster SplitChannel(LSDFlowInfo& FlowInfo, vector<int> Sources, int TargetSegmentLength);

  /// SplitHillslopes
  /// @brief This function is intended to follow the SplitChannel function.  It traces
  /// through the receiver nodes from every hillslope pixel and then assigns them 
  /// an integer value that matches the index of the section of channel that is
  /// setting the base level of that hillslope.
  ///
  /// @param LSDFlowInfo object
  /// @param LSDIndexRaster: a raster of channel segments, produced by the SplitChannel function
  /// @return LSDIndexRaster: hillslope segments labelled by ID of channel segments
  /// @author DTM 
  /// @date 29/10/2013
  LSDIndexRaster SplitHillslopes(LSDFlowInfo& FlowInfo, LSDIndexRaster& ChannelSegmentsRaster);

  /// SplitHillslopes
  /// @brief This is an overloaded function doing the same as the previous version to
  /// segment hillslopes according to the channel index of the channel setting its
  /// base level.  However, this has been adapted to include an additional input
  /// raster - MultiThreadChannelRaster - which recognises that real channels may
  /// be multithreaded and/or have widths greater than or equal to one pixel.
  /// To be rigourous, these should be removed from analyses of hillslope
  /// properties.
  /// 
  /// @param LSDFlowInfo object
  /// @param LSDIndexRaster: a raster of channel segments, produced by the SplitChannel function
  /// @param LSDIndexRaster: a binary raster with the full channel extent
  /// @return LSDIndexRaster: hillslope segments labelled by ID of channel segments
  /// @author DTM 
  /// @date 29/10/2013
  LSDIndexRaster SplitHillslopes(LSDFlowInfo& FlowInfo, LSDIndexRaster& ChannelSegmentsRaster, LSDIndexRaster& MultiThreadChannelRaster);


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
  /// Updated 24/10/13 to handle junction numbers in the same way that the basin extraction code does,
  /// by searching one junction downstream of the given junction and then going back up by one node - SWDG.
  /// @author SWDG
  /// @date 27/06/2013
	bool node_tester(LSDFlowInfo& FlowInfo, int input_junction);


  /// @brief Function to snap input coordinates to the nearest junction. This
  /// enables easy extraction of a particular catchment for analysis.
  /// @param X_coordinate of point. In coordiantes of DEM (usually UTM).
  /// @param Y_coordinate of point. In coordiantes of DEM (usually UTM).
  /// @param FlowInfo LSDFlowInfo object.
  /// @author DTM
  /// @date 17/10/2013
  int get_receiver_junction_for_specified_coordinates(double X_coordinate, double Y_coordinate, LSDFlowInfo& FlowInfo);


  /// @brief Function to snap input coordinates to the nearest channel node. This
  /// enables easy extraction of a particular catchment for analysis.
  /// @param X_coordinate of point. In coordiantes of DEM (usually UTM).
  /// @param Y_coordinate of point. In coordiantes of DEM (usually UTM).
  /// @param threshold_stream_order The minimum stream order that will be considers a 'channel' by the algorithm
  /// @param search_radius_nodes the radius of the kernal used to check if there is a nearby channel. A radius of 0 only
  /// includes the centre point, a radius of 1 has a kernal diameter of 3,  radius of 2 has a kernal diameter of 5
  /// and so on
  /// @param FlowInfo LSDFlowInfo object.
  /// @return Returns the NodeIndex of the nearest channel node.
  /// @author SMM
  /// @date 21/10/2013
  int get_nodeindex_of_nearest_channel_for_specified_coordinates(double X_coordinate,
                            double Y_coordinate, int threshold_stream_order, int search_radius_nodes, LSDFlowInfo& FlowInfo);



	/// @brief This function takes a node index, checks to see if it is on a channel,
	/// and then works its way up the channel to find the upstream junction
	/// @param ChannelNodeIndex is the node index of the channel node (if it isn't a channel
	/// the function returns NoDataValue
	/// @param FlowInfo LSDFlowInfo object.
    /// @return Returns the Junction Number of the nearest upslope junction
    /// @author SMM
    /// @date 21/10/2013
	int find_upstream_junction_from_channel_nodeindex(int ChannelNodeIndex, LSDFlowInfo& FlowInfo);

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
