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


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDChannelNetwork.cpp
// LSDChannelNetwork object
// LSD stands for Land Surface Dynamics
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This object is written by
// Simon M. Mudd, University of Edinburgh
// David Milodowski, University of Edinburgh
// Martin D. Hurst, British Geological Survey
// Stuart Grieve, University of Edinburgh
// <your name here>
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Version 1.0.0		15/07/2013
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "TNT/tnt.h"
#include "LSDFlowInfo.hpp"
#include "LSDRaster.hpp"
#include "LSDChannel.hpp"
#include "LSDChannelNetwork.hpp"
#include "LSDIndexChannel.hpp"
#include "LSDStatsTools.hpp"
using namespace std;
using namespace TNT;

#ifndef LSDChannelNetwork_CPP
#define LSDChannelNetwork_CPP

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// create
// this defines a channel network based on a FlowInfo object
// and a list of source nodes
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDChannelNetwork::create(vector<int> Sources, LSDFlowInfo& FlowInfo)
{
	NRows = FlowInfo.NRows;
	NCols = FlowInfo.NCols;
	XMinimum = FlowInfo.XMinimum;
	YMinimum = FlowInfo.YMinimum;
	DataResolution = FlowInfo.DataResolution;
	NoDataValue = FlowInfo.NoDataValue;

	SourcesVector = Sources;

	// start arrays where the data all begins as nodata
	Array2D<int> TempLinkArray(NRows,NCols,NoDataValue);

	JunctionArray = TempLinkArray.copy();
	StreamOrderArray = TempLinkArray.copy();
	JunctionIndexArray = TempLinkArray.copy();

	vector<int> TempVector;
	JunctionVector = TempVector;
	BaseLevelJunctions = TempVector;

	// we loop through the sources file
	// for each source we burn down to either base level
	// or the next channel
	//
	// with link numbers by adding integers
	//
	int n_sources = SourcesVector.size();
	int current_node;
	int current_row,current_col;
	int receiver_node;
	int baselevel_switch;		// 0 if not a base level node, 1 if so
	int current_stream_order;
	int junction_switch;
	int donor_node, donor_row,donor_col;
	int n_current_stream_order_donors;

	// loop through sources.
	// this loop sets the stream orders and identifies the
	// junctions
	// it is the first of two loops through the stream network
	// the second will generate the stream link information
	for(int src = 0; src<n_sources; src++)
	{
		baselevel_switch =0;			// 0 == not base level
		junction_switch = 0;			// 0 == no junction so far
		current_node = SourcesVector[src];
		current_row = FlowInfo.RowIndex[current_node];
		current_col = FlowInfo.ColIndex[current_node];
		receiver_node = FlowInfo.ReceiverVector[current_node];

		current_stream_order = 1;

		if (current_node == receiver_node)
		{
			baselevel_switch = 1;
		}


		// follow node down through receivers until it hits a base level node
		// the switch is set to 2 because if there is a baselevel node
		// the algorithm has to go through the loop once to register the
		// 'downslope node' as the baselevel
		while ( baselevel_switch <2 )
		{

			// check if this node has already been added to the channel network
			// if it hasn't, then this becomes a channel of order of the current order
			if(StreamOrderArray[current_row][current_col] == NoDataValue)
			{
				StreamOrderArray[current_row][current_col] = current_stream_order;
			}
			// if it isn't a nodata node:
			// note that the junction switch starts out as a zero.
			// the channel is followed looking at nodata values in the stream order array
			// once it hits the first junction, the StreamOrderArray has finite values,
			// so this logic is triggered.
			else if(StreamOrderArray[current_row][current_col] != NoDataValue)
			{

				// each source contributes a junction unless it is the
				// only source to go to a given baselevel node
				// check to see if this is the first time the channel
				// has hit another channel. If so, add the junction
				// and set the junction switch to 1 so that no further
				// junctions are added
				// also if it is a junction check to see if the stream order has been incremented
				// if it has not, the base level switch is turn to 2 and the
				// algorithm exits this loop and goes to the next source
				// if it has, this and all downstream nodes take on the new stream order
				// if junction switch is zero it means this is the first visit of a previously visited channel
				if (junction_switch == 0)
				{
					junction_switch = 1;
					JunctionArray[current_row][current_col]  = 1;

					// if it is the the first junction for this source, the current_stream_order
					// is one. Therefore any junction will result in a stream order
					// of at least 2.
					// If the junction is currently at a stream order of 1, then it
					// gets incremented
					if (StreamOrderArray[current_row][current_col] == current_stream_order)
					{
						current_stream_order ++;
						StreamOrderArray[current_row][current_col]= current_stream_order;
					}
					// if the junction is two or greater, the loop exits since there can
					// be no more incrementing of the stream order
					else
					{
						baselevel_switch = 2;
					}
				}
				else
				{
					// first, we check to see if it is not a junction. if not, we update the
					// stream order. If the stream order hasn't changed, then something
					// is amiss since there is no point moving downstream
					// when the current stream order is the same as the previous stream order,
					// since it can't increase stream order downstream
					// nodes following downstream will be at the current stream order
					if (JunctionArray[current_row][current_col]  != 1)
					{
						// THIS IS NOT A JUNCTION
						// if the current stream order is bigger than the existing stream order
						// at this point, then increase the existing stream order
						if ( current_stream_order > StreamOrderArray[current_row][current_col])
						{
							StreamOrderArray[current_row][current_col] = current_stream_order;
						}
						else
						{
							baselevel_switch = 2;
						}

					}
					// if it is a junction, see if there is an increment
					// in the current stream order
					// the node it has come from has an updated current stream order
					// so just look at the donor nodes to
					// see if there are two or more
					// donors at a higher stream order
					else if (JunctionArray[current_row][current_col]  == 1)
					{
						// THIS IS A JUNCTION
						// first, check to see if this has a higher stream order than the current stream
						// order
						if (StreamOrderArray[current_row][current_col] > current_stream_order)
						{
							// yes, the stream order at this junction is higher than the current stream order
							// even if the junction is incremented the downstream search would
							// stop here, so get out with the baselevel switch
							baselevel_switch = 2;
						}
						else if ( StreamOrderArray[current_row][current_col] == current_stream_order)
						{


							// this means that the current stream order is equal to or less than the streamorder
							// currently at this node this means you need to check and see if the thing is incremented
							n_current_stream_order_donors = 0;
							for(int dnode = 0; dnode<FlowInfo.NDonorsVector[current_node]; dnode++)
							{
								donor_node = FlowInfo.DonorStackVector[ FlowInfo.DeltaVector[current_node]+dnode];

								// ignore the base level donor
								if (donor_node != current_node)
								{
									donor_row = FlowInfo.RowIndex[ donor_node ];
									donor_col = FlowInfo.ColIndex[ donor_node ];

									if (StreamOrderArray[donor_row][donor_col] == current_stream_order)
									{
										n_current_stream_order_donors++;
									}
								}
							}

							// now check to see if the stream order has increased
							if (n_current_stream_order_donors >= 2)
							{
								current_stream_order++;
								StreamOrderArray[current_row][current_col] = current_stream_order;
							}
							else			// if it hasn't, the loop ends here.
							{
								baselevel_switch = 2;
							}

						}
						else if (StreamOrderArray[current_row][current_col] < current_stream_order)
						{

							// the current stream order is higher than the stream order at this point.
							// the node needs to update its stream order and keep going
							StreamOrderArray[current_row][current_col] = current_stream_order;
						}
						else
						{
							cout << "something about the logic has gone wrong. " <<endl;
							cout << "hhh node: " << current_node << ", current_stream_order " << current_stream_order
								 << " array: " << StreamOrderArray[current_row][current_col]<<" and src: " << SourcesVector[src] << endl;
						}

					}	// end logic for if this is a downstream junction
				}		// end logic for if this is not the first junction
			}			// end logic for if this is not a NoData node


			// get the next current node, which is this nodes receiver
			current_node = FlowInfo.ReceiverVector[current_node];

			// get the next receiver node, which is the next node
			receiver_node = FlowInfo.ReceiverVector[current_node];
			current_row = FlowInfo.RowIndex[current_node];
			current_col = FlowInfo.ColIndex[current_node];

			// if this is a baselevel node
			if (current_node == receiver_node)
			{
				baselevel_switch ++;
			}


		}						// end flow to baselevel loop

	}							// end sources loop


	// now you need to loop through the sources once more, creating links
	// each link has a starting node, and ending node
	// a stream order
	// a starting link and an ending link
	//
	// this should be arranged in an analagous way to the fastscape algorithm
	// all sources are on 1st order links
	int this_junction = -1;
	for(int src = 0; src<n_sources; src++)
	{
		this_junction++;				// increment the last junction
		baselevel_switch =0;			// 0 == not base level
		junction_switch = 0;			// 0 == no junction so far
		current_node = SourcesVector[src];
		current_row = FlowInfo.RowIndex[current_node];
		current_col = FlowInfo.ColIndex[current_node];
		receiver_node = FlowInfo.ReceiverVector[current_node];

		//cout << "LINE 257 ChNet, SOURCE: " << src <<  " n_src: " << n_sources << " current_node: " << current_node
		//     << " and rnode: " << receiver_node << endl;

		//each source is a junction node. Push back the junction vector
		JunctionVector.push_back(current_node);

		// set the junction Index Array
		JunctionIndexArray[current_row][current_col] = this_junction;

		// stream order only increases at junctions. So the junction node has a stream
		// order that remains the same until it gets to the next junction, where it possibly
		// could change
		StreamOrderVector.push_back( StreamOrderArray[current_row][current_col] );

		// check if this is a baselevel node
		if(receiver_node == current_node)
		{
			baselevel_switch = 1;			// turn the baselevel switch on
			ReceiverVector.push_back(this_junction);		// the Receiver node is iteself

			// this logic only applies to sources, which cannot lie downstream of another source
			// ***THIS MUST BE ENFORCED BY THE GET_SOURCES ALGORITHM***
			// this means that if a source is also a baselevel, then this is the one and only time
			// this baselevel junction is added, so add it to the baselevel vector
			BaseLevelJunctions.push_back(this_junction);
		}

		// the next element is the receiver junction, we need to follow the path to this receiver
		// the routine continues until the junction has been visited or until it hits
		// a baselevel node
		//cout << "LINE 280" << endl;
		while (baselevel_switch == 0 && junction_switch <2)
		{
			//cout << "Line 283" << endl;

			//cout << "Line 286, current node = " << current_node << " and rode: " << receiver_node << endl;
			current_node = receiver_node;
			//cout << "Line 288, current node = " << current_node << " and rode: " << receiver_node << endl;
			current_row = FlowInfo.RowIndex[current_node];
			current_col = FlowInfo.ColIndex[current_node];
			receiver_node = FlowInfo.ReceiverVector[current_node];

			// first we need logic for if this is a baselevel node
			if (current_node == receiver_node)
			{
				//cout << "source: " << src << " and BASELEVEL, node: " << current_node << " rnode: " << receiver_node << endl;
				// check to see if it has a junction index number.
				if(JunctionIndexArray[current_row][current_col] == NoDataValue)
				{
					// it doens't have a JunctionIndexNumber. This is a new
					// junction
					this_junction++;

					// this junction has the this_junction index. Set the JunctionIndexArray
					JunctionIndexArray[current_row][current_col] = this_junction;

					// the receiver node of the previous junction is the new junction
					ReceiverVector.push_back( JunctionIndexArray[current_row][current_col] );

					//push back the junction vector
					JunctionVector.push_back(current_node);

					// because this is a baselevel node, the Receiver of this junction
					// is iteself
					ReceiverVector.push_back( JunctionIndexArray[current_row][current_col] );

					// the stream order of this node is also determined by the node
					StreamOrderVector.push_back( StreamOrderArray[current_row][current_col] );

					// finally, this is the first time we have vistid this baselevel node.
					// So it gets added to the baselevel vector
					BaseLevelJunctions.push_back(this_junction);

				}
				else				// this junction does have an index number, no new junction is created
				{
					// the receiver node of the previous junction is the new junction
					ReceiverVector.push_back( JunctionIndexArray[current_row][current_col] );
				}
				junction_switch = 2;
				baselevel_switch = 1;			// this is a baselevel. It will exit the
												// loop and move to the next source
			}
			else						// this is not a baselevel node
			{
				//cout << "LINE 330, not baselevel; src: " << src << " and node: " << current_node << " rnde: " << receiver_node << endl;
				// the node in the junction array is zero if it is not a
				// junction, 1 if it is an unvisited junction, and 2 or more if it
				// is a visited junction
				if(JunctionArray[current_row][current_col] != NoDataValue)
				{
					//cout << "LINE 338, found a junction at node: " << current_node
					//	 << " JArray: " << JunctionArray[current_row][current_col]  << endl;
					junction_switch = JunctionArray[current_row][current_col];
					JunctionArray[current_row][current_col] ++;		// increment the junction array
																	// it will be greater than 1 if
																	// the junction has been visited

					// if this junction has been visited, it will have a junction number
					// include the receiver vector
					if (JunctionIndexArray[current_row][current_col] != NoDataValue )
					{
						ReceiverVector.push_back( JunctionIndexArray[current_row][current_col] );

						// the loop will not continue; it will move onto the next
						// source since it has visited an already visited junction
					}
					else			// the junction has not been visited
					{
						// this is a new junction. Increment the 'last junction' int
						this_junction++;

						// this junction has the this_junction index. Set the JunctionIndexArray
						JunctionIndexArray[current_row][current_col] = this_junction;

						// the receiver node of the previous junction is the new junction
						ReceiverVector.push_back( JunctionIndexArray[current_row][current_col] );

						//push back the junction vector; this is a new junction
						JunctionVector.push_back(current_node);

						// get the stream order of this new junction
						StreamOrderVector.push_back( StreamOrderArray[current_row][current_col] );
					}
				}	// end logic for is this a junction
			}		// end logic for not a baselevel node
		}				// end baselevel logic
	}					// end sources loop

	//cout << "ChanNet; LINE 368; sz ReceiverVec: " << ReceiverVector.size() << " sz JuncVec: " << JunctionVector.size()
	//   << " sz SOVec: " << StreamOrderVector.size() << endl;

	//for(int i = 0; i< int(BaseLevelJunctions.size()); i++)
	//{
	//	cout << "LINE 382 bl node["<<i<<"]: " << BaseLevelJunctions[i] << endl;
	//}

	//cout << "LINE 385: links data " << endl;
	//for (int i = 0; i< int(StreamOrderVector.size()); i++)
	//{
	//	cout << "Junc: " << i << " node: " << JunctionVector[i]
	//	     << " receiv: " << ReceiverVector[i] << " Order: " << StreamOrderVector[i] << endl;
	//}


	// get the number of junctions
	NJunctions = int(JunctionVector.size());

	// now we implement the fastscape algorithm


	// set the sizes of the member vectors
	vector<int> ndn_vec(NJunctions,0);
	vector<int> ndn_nodata_vec(NJunctions,NoDataValue);
	vector<int> ndn_plusone_vec(NJunctions+1,0);
	vector<int> w_vector(NJunctions,0);

	NDonorsVector = ndn_vec;
	DonorStackVector = ndn_vec;
	DeltaVector = ndn_plusone_vec;

	SVector = ndn_nodata_vec;
	BLBasinVector = ndn_nodata_vec;

	// first create the number of donors vector
	// from braun and willett eq. 5
	for(int i = 0; i<NJunctions; i++)
	{
		NDonorsVector[ ReceiverVector[i] ]++;
	}

	// now create the delta vector
	// this starts on the last element and works its way backwards
	// from Braun and Willett eq 7 and 8
	DeltaVector[NJunctions] = NJunctions;
	for(int i = NJunctions; i>0; i--)
	{
		DeltaVector[i-1] = DeltaVector[i] -  NDonorsVector[i-1];
	}

	// now the DonorStack and the r vectors. These come from Braun and Willett
	// equation 9.
	// Note that in the manscript I have there is a typo in eqaution 9
	// (Jean Braun's code is correct)
	// it should be w_{r_i} = w_{r_i}+1
	int r_index;
	int w_index;
	int delta_index;
	for (int i = 0; i<NJunctions; i++)
	{
		r_index = ReceiverVector[i];
		delta_index = DeltaVector[ r_index ];
		w_index = w_vector[ r_index ];
		DonorStackVector[  delta_index+w_index ] = i;
		w_vector[r_index] += 1;
	}

	// now go through the base level node list, building the drainage tree for each of these nodes as one goes along
	int n_base_level_nodes;
	n_base_level_nodes = BaseLevelJunctions.size();

	int k;
	int j_index;
	int begin_delta_index, end_delta_index;
	int l_index;

	j_index = 0;
	for (int i = 0; i<n_base_level_nodes; i++)
	{
		k = BaseLevelJunctions[i];			// set k to the base level node

		// This doesn't seem to be in Braun and Willet but to get the ordering correct you
		// need to make sure that the base level node appears first in the donorstack
		// of nodes contributing to the baselevel node.
		// For example, if base level node is 4, with 4 donors
		// and the donor stack has 3 4 8 9
		// the code has to put the 4 first.
		if (DonorStackVector[ DeltaVector[k] ] != k)
		{
			int this_index = DonorStackVector[ DeltaVector[k] ];
			int bs_node = k;

			for(int ds_node = 1; ds_node < NDonorsVector[k]; ds_node++)
			{
				if( DonorStackVector[ DeltaVector[k] + ds_node ] == bs_node )
				{
					DonorStackVector[ DeltaVector[k] ] = k;
					DonorStackVector[ DeltaVector[k] + ds_node ] = this_index;
				}
			}
		}

		// now run recursive algorithm
		begin_delta_index = DeltaVector[k];
		end_delta_index = DeltaVector[k+1];

		for (int delta_index = begin_delta_index; delta_index<end_delta_index; delta_index++)
		{
			l_index = DonorStackVector[delta_index];
			add_to_stack(l_index, j_index, k);
		}
	}

	// now run the indexing and accumulation routine
	vector<int> vectorized_contributing_pixels(NJunctions,1);
	SVectorIndex = vectorized_contributing_pixels;

	int receiver_junction;
	int donor_junction;

	// loop through the s vector, adding pixels to receiver nodes
	for(int junc = NJunctions-1; junc>=0; junc--)
	{
		donor_junction = SVector[junc];
		receiver_junction = ReceiverVector[ donor_junction ];

		// every node is visited once and only once so we can map the
		// unique positions of the nodes to the SVector
		SVectorIndex[donor_junction] = junc;

		// add the upslope area (note no action is taken
		// for base level nodes since they donate to themselves and
		// we must avoid double counting
		if (donor_junction != receiver_junction)
		{
			vectorized_contributing_pixels[ receiver_junction ] +=  vectorized_contributing_pixels[ donor_junction ];
		}
	}
	//cout << "LINE 525 did area calcs " << endl;

	NContributingJunctions = vectorized_contributing_pixels;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// recursive add_to_stack routine, from Braun and Willett eq. 12 and 13
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDChannelNetwork::add_to_stack(int lm_index, int& j_index, int bl_node)
{
	//cout << "j_index: " << j_index << " and s_vec: " << lm_index << endl;

	SVector[j_index] = lm_index;
	BLBasinVector[j_index] = bl_node;
	j_index++;


	int begin_m,end_m;
	int l_index;
	// if donating to itself, need escape hatch
	if ( lm_index == bl_node)
	{
		begin_m = 0;
		end_m = 0;
	}
	else
	{
		begin_m = DeltaVector[lm_index];
		end_m =  DeltaVector[ lm_index+1];
	}
	//cout << "lm_index: " << lm_index << " begin_m: " << begin_m << " end m: " << end_m << endl;
	for( int m_index = begin_m; m_index<end_m; m_index++)
	{
		//cout << "recursion, begin_m: " << begin_m << " and end_m: " << end_m << endl;
		l_index = DonorStackVector[m_index];
		add_to_stack(l_index, j_index, bl_node);
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function returns a integer vector containing all the junction numbers upslope
// of of the junction with number junction_number_outlet
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
vector<int> LSDChannelNetwork::get_upslope_junctions(int junction_number_outlet)
{
	vector<int> us_junctions;

	if(junction_number_outlet < 0 || junction_number_outlet > NJunctions-1)
	{
		cout << "Tried LSDChannelNetwork::get_upslope_junctions but the"
		     << "  junction number does not exist" << endl;
		exit(0);
	}

	int start_SVector_junction = SVectorIndex[junction_number_outlet];
	int end_SVector_junction = start_SVector_junction+NContributingJunctions[junction_number_outlet];

	for(int junction = start_SVector_junction; junction < end_SVector_junction; junction++)
	{
		us_junctions.push_back(SVector[junction]);
	}

	return us_junctions;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// If you get an upslope junction list the indexing starts at the furthest downslope
// junction. All of the junction pointing refers to the master junction list however.
//
// This function maps a junction onto the indexing of the upslope junction list
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int LSDChannelNetwork::map_junction_to_upslope_junction_list(vector<int> upslope_junctions, int junction)
{
	// get the SVector location of the first junction
	int start_SVector_junction = SVectorIndex[ upslope_junctions[0] ];

	// now find how many elements away in the upslope_junction list
	// the current junction is from the start junction
	int mapped_us_junction_index = SVectorIndex[ junction ] - start_SVector_junction;

	if(mapped_us_junction_index < 0 || mapped_us_junction_index > int(upslope_junctions.size())-1)
	{
		cout << "Tried LSDChannelNetwork::map_junction_to_upslope_junction_list"
		     << "  junction number is not within the list of upslope junctions" << endl;
		exit(EXIT_FAILURE);
	}


	return mapped_us_junction_index;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function returns the maximum stream order
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int LSDChannelNetwork::get_maximum_stream_order()
{
	int max_stream_order = 0;
	for (int i = 0; i<NJunctions; i++)
	{
		if ( max_stream_order < StreamOrderVector[i])
		{
			max_stream_order = StreamOrderVector[i];
		}
	}
	return max_stream_order;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function gets the junction index of a node.  If there is not a junction at this node
// then it will give NoDataValue
//
// FC 31/10/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int LSDChannelNetwork::get_Junction_of_Node(int Node, LSDFlowInfo& FlowInfo)
{
	int JunctionNumber, Row, Col;
	
  FlowInfo.retrieve_current_row_and_col(Node, Row, Col);
  JunctionNumber = JunctionIndexArray[Row][Col];
  
  return JunctionNumber;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function generates and LSDIndexChannel given a starting junction
// NOTE: Each junction is the UPSTREAM end of a channel
// this is because junctions can have one and only once receiver
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexChannel LSDChannelNetwork::generate_link_index_channel_from_junction(int start_junction,
													LSDFlowInfo& FlowInfo)
{
	// check bounds on junction
	if(start_junction < 0 || start_junction > NJunctions-1)
	{
		cout << "Tried LSDChannelNetwork::generate_link_index_channel_from_junction"
		     << "  but the junction number does not exist" << endl;
		exit(0);
	}

	// get the starting and ending junctions and nodes
	int end_junction = ReceiverVector[start_junction];
	int start_node = JunctionVector[start_junction];
	int end_node = JunctionVector[end_junction];;

	// extract the channel segment
	LSDIndexChannel this_channel(start_junction, start_node,
	                			 end_junction, end_node, FlowInfo);

	return this_channel;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function extracts the longest channel originating from a junction number
// outlet_junction
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexChannel LSDChannelNetwork::generate_longest_index_channel_from_junction(int outlet_junction, LSDFlowInfo& FInfo,
									LSDRaster& dist_from_outlet)
{

	if (outlet_junction >= int(JunctionVector.size()))
	{
		cout << "LSDChannelNetwork::generate_longest_index_channel_from_junction junction not in list" << endl;
		exit(EXIT_FAILURE);
	}

	// first get the number of nodes within the junction
	vector<int> us_junctions = get_upslope_junctions(outlet_junction);

	int n_us_junctions = int(us_junctions.size());
	int farthest_junc = outlet_junction;
	double farthest_dist = 0;
	double current_dist;
	int current_junc, current_row, current_col, current_node;

	// loop through these junctions, looking for the one that is farthest from the outlet
	for (int j = 0; j<n_us_junctions; j++)
	{
		current_junc = us_junctions[j];
		current_node = JunctionVector[current_junc];
		current_row = FInfo.RowIndex[current_node];
		current_col = FInfo.ColIndex[current_node];
		current_dist = dist_from_outlet.get_data_element(current_row,current_col);
		if(current_dist > farthest_dist)
		{
			farthest_dist = current_dist;
			farthest_junc = current_junc;
		}
	}

	int start_junction_node = JunctionVector[farthest_junc];
	int end_junction_node = JunctionVector[outlet_junction];
	LSDIndexChannel Longest_channel(farthest_junc, start_junction_node,
	                outlet_junction, end_junction_node, FInfo);

	return Longest_channel;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function extracts the longest channel originating from a basin
// this differs from the extract_longest_channel_from_junction in that
// basins continue down to a reciever junction
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexChannel LSDChannelNetwork::generate_longest_index_channel_in_basin(int basin_junction, LSDFlowInfo& FInfo,
									LSDRaster& dist_from_outlet)
{
	if (basin_junction >= int(JunctionVector.size()))
	{
		cout << "LSDChannelNetwork::generate_longest_index_channel_in_basin junction not in list" << endl;
		exit(EXIT_FAILURE);
	}

	// first get the number of junctions upslope of the junction
	vector<int> us_junctions = get_upslope_junctions(basin_junction);

	int n_us_junctions = int(us_junctions.size());
	int farthest_junc = basin_junction;
	double farthest_dist = 0;
	double current_dist;
	int current_junc, current_row, current_col, current_node;

	// loop through these junctions, looking for the one that is farthest from the outlet
	for (int j = 0; j<n_us_junctions; j++)
	{
		current_junc = us_junctions[j];
		current_node = JunctionVector[current_junc];
		current_row = FInfo.RowIndex[current_node];
		current_col = FInfo.ColIndex[current_node];
		current_dist = dist_from_outlet.get_data_element(current_row,current_col);
		if(current_dist > farthest_dist)
		{
			farthest_dist = current_dist;
			farthest_junc = current_junc;
		}
	}

	int start_junction_node = JunctionVector[farthest_junc];

	int outlet_junction = ReceiverVector[basin_junction];
	int end_junction_node = JunctionVector[outlet_junction];
	LSDIndexChannel Longest_channel(farthest_junc, start_junction_node,
	                outlet_junction, end_junction_node, FInfo);

	return Longest_channel;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDChannelNetwork::extract_tributary_junctions_to_main_stem(LSDIndexChannel& MainStem, LSDFlowInfo& FlowInfo,
																		vector<int>& tributary_junctions,
																		vector<int>& nodes_on_main_stem_of_tributaries)
{
	int n_channel_nodes = MainStem.get_n_nodes_in_channel();
	int node, row, col;
	int curr_junc;
	int previous_junc;
	int n_donors;
	int donor_junc;

	vector<int> temp_tributary_junctions;
	vector<int> temp_nodes_on_main_stem_of_tributaries;

	// the channel starts at a junction, get this junction and set it to
	// be the previous_junction
	MainStem.get_node_row_col_in_channel(0, node, row, col);
	previous_junc = JunctionIndexArray[row][col];

	// now loop through channel, starting at the top
	for(int ch_node = 1; ch_node<n_channel_nodes; ch_node++)
	{
		// get the node index as well as the row and column of the current node in the channel
		MainStem.get_node_row_col_in_channel(ch_node, node, row, col);

		curr_junc = JunctionIndexArray[row][col];
		// if the current junction does not equal the no data value, look for
		// donor nodes
		if(curr_junc != NoDataValue)
		{
			// use the delta vector to get the donor nodes
			n_donors = NDonorsVector[curr_junc];
			for(int donor = 0; donor<n_donors; donor++)
			{
				donor_junc = DonorStackVector[ DeltaVector[curr_junc]+donor ];
				if (donor_junc!=previous_junc)
				{
					temp_tributary_junctions.push_back( donor_junc );
					temp_nodes_on_main_stem_of_tributaries.push_back( ch_node );
				}
			}

			// now set the current junction to the previous junction in order to adnvance further down
			// the channel
			previous_junc = curr_junc;
		}
	}

	tributary_junctions = temp_tributary_junctions;
	nodes_on_main_stem_of_tributaries = temp_nodes_on_main_stem_of_tributaries;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// vector<int> LSDChannelNetwork::get_pruned_tributaries_from_main_stem
// this function extracts tributaries juncions to the main stem of the channel, then selects
// a sample based on various criteria set by an integer called pruning switch
// pruning_switch == 0  channels are only added if they exceed a threshold drainage area
// pruning_switch == 1  channels are only added if the ratio between them and the mainstem
//						exceeds a certain value (pruning_threshold)
// pruning_switch == 2	channels are only added if the ratio between them and the area of the
//						mainstem _at the junction_ exceeds a certain value
// pruning_switch == 3 channels are only added if the channel order is >= threshold
// DTM 30/04/2013
//--------------------------------------------------------------------------------------------
vector<int> LSDChannelNetwork::get_pruned_tributaries_from_main_stem(LSDFlowInfo& FlowInfo, LSDChannelNetwork& ChannelNetwork,
int starting_junction, LSDRaster& DistanceFromOutlet, int pruning_switch, double pruning_threshold)
{
  NRows = FlowInfo.get_NRows();
  NCols = FlowInfo.get_NCols();
  DataResolution = FlowInfo.get_DataResolution();
  XMinimum = FlowInfo.get_XMinimum();
  YMinimum = FlowInfo.get_YMinimum();
  NoDataValue = ChannelNetwork.get_NoDataValue();
  double pixel_area = DataResolution*DataResolution;

  int nodes_in_channel;

  // initiate the main stem
  LSDIndexChannel main_stem = ChannelNetwork.generate_longest_index_channel_in_basin(starting_junction,
  FlowInfo, DistanceFromOutlet);
  nodes_in_channel = main_stem.get_n_nodes_in_channel();

  // Get drainage area of main stem - for pruning tributaries later on
  double main_stem_drainage_area = double(main_stem.get_contributing_pixels_at_outlet(FlowInfo))*pixel_area;
  double contributing_pixel_area;
  double main_stem_junc_area;
  int tributary_order;
  // now get the tributaries for the main stem
  vector<int> tributary_junctions;
  vector<int> node_of_tributaries;
  // Get the tributary junctions
  ChannelNetwork.extract_tributary_junctions_to_main_stem(main_stem, FlowInfo, tributary_junctions, node_of_tributaries);

  // get the number of main stem tributaries
  int n_main_stem_tributaries = tributary_junctions.size();

  // now loop through the tributaries, pruning them so that only those large enough
  // or of the correct order are selected
  vector<int> target_tributary_junctions;
  for (int trib = 0; trib<n_main_stem_tributaries; trib++)
  {
    // Create LSDIndexChannel object for this tributary
    LSDIndexChannel main_stem_tributary = ChannelNetwork.generate_longest_index_channel_in_basin(tributary_junctions[trib],
    FlowInfo, DistanceFromOutlet);

    // get contributing area for tributary to junction with main stem
    contributing_pixel_area = pixel_area*double(main_stem_tributary.get_contributing_pixels_at_penultimate_node(FlowInfo));
    // get contributing area for main stem at the junction with the tributary
    main_stem_junc_area = double(main_stem.get_contributing_pixels_at_node(node_of_tributaries[trib], FlowInfo))*pixel_area;
    // get order of tributary channel
    tributary_order = StreamOrderVector[ tributary_junctions[trib] ];

    // PRUNE TRIBUTARY NETWORK TO ONLY CONSIDER TARGET CHANNELS - USING SPECIFIED PRUNING THRESHOLDS
    if (pruning_switch == 0)        // simple contributing area threshold
    {
      if (contributing_pixel_area > pruning_threshold) target_tributary_junctions.push_back(tributary_junctions[trib]);
    }
    else if (pruning_switch == 1)   // ratio of contributing area of tributary to that of main stem
    {
      if (contributing_pixel_area/main_stem_drainage_area >pruning_threshold) target_tributary_junctions.push_back(tributary_junctions[trib]);
    }
    else if (pruning_switch == 2)   // ratio of contributing area of tributary to that of main stem at junction
    {
      if ( contributing_pixel_area/main_stem_junc_area > pruning_threshold) target_tributary_junctions.push_back(tributary_junctions[trib]);
    }
    else if (pruning_switch == 3)   // channel order
    {
      if (tributary_order > pruning_threshold) target_tributary_junctions.push_back(tributary_junctions[trib]);
    }
    else                            // No pruning
    {
      target_tributary_junctions.push_back(tributary_junctions[trib]);
    }
  }
  return target_tributary_junctions;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// This function extracts basins according to their accumulated drainage area.
// Added by DTM 07/05/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
vector<int> LSDChannelNetwork::extract_basin_nodes_by_drainage_area(double DrainageAreaThreshold, LSDFlowInfo& FlowInfo)
{
  // declare the vector containeing the ##node## numbers of the basin outlets
  vector<int> outlet_nodes_of_basins;
  double PixelArea = DataResolution*DataResolution;
  // Loop through junction network until you reach channel junction with required drainage area.
  //int BasinID = 0;
  for (int CurrentJunc=0; CurrentJunc<NJunctions; ++CurrentJunc)
  {
    // Current Junction (should be <= threshold area)
    int CurrentNode = get_Node_of_Junction(CurrentJunc);
    double CurrentJuncDrainageArea = FlowInfo.NContributingNodes[CurrentNode] * PixelArea;
    // ReceiverJunc (should be > threshold area)
    int ReceiverJunc = ReceiverVector[CurrentJunc];
    int ReceiverJunc_Node = get_Node_of_Junction(ReceiverJunc);
    double ReceiverJuncDrainageArea = FlowInfo.NContributingNodes[ReceiverJunc_Node] * PixelArea;

    // Loop through all stream junctions of the required drainage area.  Need to find the
    // stream link in which the drainage area threshold is crossed, before searching through
    // the nodes in this stream link to find the exact node for which to extract the basin
    if ((CurrentJuncDrainageArea <= DrainageAreaThreshold) && (ReceiverJuncDrainageArea > DrainageAreaThreshold))
    {
		  cout << "JUNCTION " << CurrentJunc << "/" << NJunctions << " Drainage area = " << CurrentJuncDrainageArea << endl;
      cout << "\t -> found channel segment... upstream drainage area = " << CurrentJuncDrainageArea << " downstream = "
           << ReceiverJuncDrainageArea << "starting search to find the threshold" << endl;

      // First, get a vector containing channel link nodes
		  LSDIndexChannel StreamLinkVector = LSDIndexChannel(CurrentJunc, JunctionVector[CurrentJunc],
                                                           ReceiverJunc, JunctionVector[ReceiverJunc], FlowInfo);
      // Find number of nodes in channel
      //int n_nodes_in_channel = StreamLinkVector.get_n_nodes_in_channel();

      // Loop through nodes in stream link vector until we reach required drainage area threshold
		  int flag = 0;
		  //int ChannelSizeTest;
      int NodeCount = 0;
		  while ((flag == 0))
		  {
        int TargetNode = StreamLinkVector.get_node_in_channel(NodeCount);
        double TargetNodeDrainageArea = FlowInfo.NContributingNodes[TargetNode] * PixelArea;

        // Test for a minimum acceptable drainage area e.g. 80% of desired drainage area before
        // channel is accepted.
        if (TargetNodeDrainageArea >= DrainageAreaThreshold)
        {
          int OutletNode = StreamLinkVector.get_node_in_channel(NodeCount-1);
          double OutletNodeDrainageArea = FlowInfo.NContributingNodes[OutletNode] * PixelArea;

          if (OutletNodeDrainageArea >= (0.8*DrainageAreaThreshold))
          {
            cout << "\t\t\t got it - drainage area = " << FlowInfo.NContributingNodes[OutletNode] * PixelArea << endl;
            outlet_nodes_of_basins.push_back(OutletNode);
            flag = 1;
          }
          else
          {
            cout << "\t\t\t nope - drainage area = " << FlowInfo.NContributingNodes[OutletNode] * PixelArea
                 << " -> too small to be considered" << endl;
            flag = 2;
          }
        }
        ++ NodeCount;
      }
    }
  }
  // Return vector with the relevant junction numbers
  return outlet_nodes_of_basins;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// This function extracts the junctions of a given basin order that are the lowermost
// junction in the basin
//
// IMPORTANT: the junctions always point downstream since they can have one and only
// one receiver. However, for a basin of given order, this starts just upstream of the
// confluence to the next basin order. So the basin ##INCLUDES## the channel flowing
// downstream to the penultimite node
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
vector<int> LSDChannelNetwork::extract_basins_order_outlet_junctions(int BasinOrder, LSDFlowInfo& FlowInfo)
{
  // declare the vector containeing the ##Junction## numbers of the basin outlets
  vector<int> outlet_junctions_of_basins_of_order_x;

  // Loop through junction network until you reach nth order channel junction.
  int BasinID = 0;
  int current_junc,receiver_junc,receiver_junc_SO;
  for (int junctionID=0; junctionID<NJunctions; ++junctionID)
  {
    // Loop through all stream junctions of the required basin order.
    // Note that the nth order basins are defined as capturing the full
    // drainage area for the nth order stream.  An nth order stream terminates
    // at a junction an order greater than order n.
    if (StreamOrderVector[junctionID] == BasinOrder)
    {
      // Get info from ChanelNetwork object regarding position of junction
      current_junc = junctionID;//JunctionVector[junctionID];
      receiver_junc = ReceiverVector[current_junc];
      receiver_junc_SO = StreamOrderVector[receiver_junc];
      // Identify outlet of nth order basin using the condition that the
      // receiver junction should be of higher order.
      if (receiver_junc_SO > BasinOrder)
      {
		outlet_junctions_of_basins_of_order_x.push_back(current_junc);
        // Increment BasinID to ensure that each basin is distinct.
        ++BasinID;
      } // end of while logic - have searched through junction catchment
    }
  }
  // Return vector with the relevant junction numbers
  return outlet_junctions_of_basins_of_order_x;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// This function gets the node indices of outlets of basins of a certain order
//
// IMPORTANT: the junctions always point downstream since they can have one and only
// one receiver. However, for a basin of given order, this starts just upstream of the
// confluence to the next basin order. So the basin ##INCLUDES## the channel flowing
// downstream to the penultimite node
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
vector<int> LSDChannelNetwork::extract_basins_order_outlet_nodes(vector<int>& BasinOutletJunctions, LSDFlowInfo& FlowInfo)
{
	// declare the vector containeing the ##Node## numbers of the basin outlets
  	vector<int> outlet_nodes_of_basins_of_order_x;

  	// find how many basins there are
  	int n_basins = BasinOutletJunctions.size();

  	int current_junc, receiver_junc, n_nodes_in_channel,basin_outlet;

  	for (int basinID=0; basinID<n_basins; ++basinID)
  	{
		current_junc = BasinOutletJunctions[basinID];
		receiver_junc = ReceiverVector[current_junc];

		// First, get channel pixels draining from the current junction.
		LSDIndexChannel StreamLinkVector = LSDIndexChannel(current_junc, JunctionVector[current_junc],
                                                           receiver_junc, JunctionVector[receiver_junc], FlowInfo);

        // Find final nth order channel pixel, which is the penultimate pixel
        // in channel.
        n_nodes_in_channel = StreamLinkVector.get_n_nodes_in_channel();
        // ======================
        // gets crazy again here.
        basin_outlet = StreamLinkVector.get_node_in_channel(n_nodes_in_channel-2);
        outlet_nodes_of_basins_of_order_x.push_back(basin_outlet);
  	}
  	// Return vector with the relevant junction numbers
  	return outlet_nodes_of_basins_of_order_x;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// This function extracts the drainage basins of a given order, n
// DTM 17/10/2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::ExtractBasinsOrder(int BasinOrder, LSDFlowInfo& FlowInfo)
{
  // Declare drainage basins array
  Array2D<int> basins(NRows,NCols,NoDataValue);
  // Loop through junction network until you reach nth order channel junction.
  int BasinID = 0;
  int row,col,node,current_junc,receiver_junc,receiver_junc_SO,n_nodes_in_channel,basin_outlet;
  for (int junctionID=0; junctionID<NJunctions; ++junctionID)
  {
    // Loop through all stream junctions of the required basin order.
    // Note that the nth order basins are defined as capturing the full
    // drainage area for the nth order stream.  An nth order stream terminates
    // at a junction an order greater than order n.
    if (StreamOrderVector[junctionID] == BasinOrder)
    {
      // Get info from ChanelNetwork object regarding position of junction
      current_junc = junctionID;//JunctionVector[junctionID];
      receiver_junc = ReceiverVector[current_junc];
      receiver_junc_SO = StreamOrderVector[receiver_junc];
      // Identify outlet of nth order basin using the condition that the
      // receiver junction should be of higher order.
      if (receiver_junc_SO > BasinOrder)
      {
        // First, get channel pixels draining from the current junction.
        LSDIndexChannel StreamLinkVector = LSDIndexChannel(current_junc, JunctionVector[current_junc],
                                                           receiver_junc, JunctionVector[receiver_junc], FlowInfo);
        // Find final nth order channel pixel, which is the penultimate pixel
        // in channel.
        n_nodes_in_channel = StreamLinkVector.get_n_nodes_in_channel();
        // ======================
        // gets crazy again here.
        basin_outlet = StreamLinkVector.get_node_in_channel(n_nodes_in_channel-2);
        // Get all contributing pixels and label with BasinID
        vector<int> BasinNodeVector = FlowInfo.get_upslope_nodes(basin_outlet);
        // Loop through basin to label basin pixels with basin ID
        for (int BasinIndex = 0; BasinIndex < int(BasinNodeVector.size()); ++BasinIndex)
        {
          node = BasinNodeVector[BasinIndex];
          FlowInfo.retrieve_current_row_and_col(node,row,col);
          basins[row][col] = BasinID;
        }
        // Increment BasinID to ensure that each basin is distinct.
        ++BasinID;
      } // end of while logic - have searched through junction catchment
    }
  }
  // Return raster with all nth order drainage basins.
  LSDIndexRaster basin_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,basins);
	return basin_raster;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// This function extracts the juctions of all non-beheaded drainage basins of a given order, n
// SWDG 23/10/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
vector<int> LSDChannelNetwork::ExtractBasinJunctionOrder(int BasinOrder, LSDFlowInfo& FlowInfo)
{
  vector<int> Junctions;
  // Loop through junction network until you reach nth order channel junction.
  
  int current_junc,receiver_junc,receiver_junc_SO;
  for (int junctionID=0; junctionID<NJunctions; ++junctionID)
  {
    // Loop through all stream junctions of the required basin order.
    // Note that the nth order basins are defined as capturing the full
    // drainage area for the nth order stream.  An nth order stream terminates
    // at a junction an order greater than order n.
    if (StreamOrderVector[junctionID] == BasinOrder)
    {
      // Get info from ChanelNetwork object regarding position of junction
      current_junc = junctionID;//JunctionVector[junctionID];
      receiver_junc = ReceiverVector[current_junc];
      receiver_junc_SO = StreamOrderVector[receiver_junc];
      // Identify outlet of nth order basin using the condition that the
      // receiver junction should be of higher order.
      if (receiver_junc_SO > BasinOrder)
      {
        //use the node tester to get rid of any basins that are beheaded
        if (node_tester(FlowInfo, current_junc) == false){ 
          Junctions.push_back(current_junc);
        } 
      } 
    }
  }
 
	return Junctions;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function takes a junction number and then cycles through the upslope
// junctions, looking for sources. Once it finds a source it then traces
// the channel to the divide (finding the longest segment) and returns the
// node indices of these hilltop nodes.
//
// SMM 25/9/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
vector<int> LSDChannelNetwork::FindFarthestUpslopeHilltopsFromSources(int JunctionNumber,
                                          LSDFlowInfo& FlowInfo, LSDRaster& FlowDistance)
{
	// get the list of upslope junctions
	vector<int> upslope_juncs = get_upslope_junctions(JunctionNumber);


	vector<int> hilltops_from_sources;
	int upslope_node_index;
	int this_source_node;

	// loop through these junctions, looking for sources
	int n_upslope_juncs = upslope_juncs.size();
	for(int i = 0; i<n_upslope_juncs; i++)
	{
		// test to see if the junction has donor, if not it is a source
		if (NDonorsVector[ upslope_juncs[i] ] == 0)
		{
			//cout << "source found, junction: " << upslope_juncs[i] << endl;

			// get the node
			this_source_node =  JunctionVector[ upslope_juncs[i] ];

			// now find the upslope node of this souce
			upslope_node_index =  FlowInfo.find_farthest_upslope_node(this_source_node, FlowDistance);

			// add it to the vector
			hilltops_from_sources.push_back(upslope_node_index);
		}
	}

	return hilltops_from_sources;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
// This function returns a vector of nodeindex values of potential channel heads
// above a given junction
//
// SMM 26/09/2013
//
// Modified by FC 26/09/2013 to extract the node of the predicted channel heads for each
// hilltop node; now returns a vector of integers.

//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
vector<int> LSDChannelNetwork::GetChannelHeadsChiMethodFromJunction(int JunctionNumber,
                                      int MinSegLength, double A_0, double m_over_n,
											                LSDFlowInfo& FlowInfo, LSDRaster& FlowDistance, LSDRaster& ElevationRaster)
{
	vector<int> ChannelHeadNodes;
	double downslope_chi = 0;

	// get the node index of this junction
	int downstream_node_index = JunctionVector[JunctionNumber];

	// get the nodes of the hilltops.
	vector<int> hilltop_nodes = FindFarthestUpslopeHilltopsFromSources(JunctionNumber,
                                          FlowInfo, FlowDistance);

    // now loop through the hilltops, getting a channel for each one
    int n_hilltops = hilltop_nodes.size();
    ChannelHeadNodes.resize(n_hilltops);

    for(int i = 0; i<n_hilltops; i++)
    {
      //cout << "hilltop " << i << " of " << n_hilltops << endl;
      LSDChannel new_channel(hilltop_nodes[i], downstream_node_index, downslope_chi, m_over_n, A_0, FlowInfo,  ElevationRaster);
		  int channel_head_node = new_channel.calculate_channel_heads(MinSegLength, A_0, m_over_n, FlowInfo);
		  ChannelHeadNodes[i] = channel_head_node;
	  }

  return ChannelHeadNodes;
}      
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
// This function returns all potential channel heads in a DEM. It looks for
// channel heads organized by a basin order which is fed to the code
// The basin order just determines how far downstream the algorithm looks for the 'fluvial'
// section.
// It returns a vector<int> of nodeindices where the channel heads are
//
// SMM 26/09/2013
//
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
vector<int> LSDChannelNetwork::GetChannelHeadsChiMethodBasinOrder(int BasinOrder,
                                      int MinSegLength, double A_0, double m_over_n,
									  LSDFlowInfo& FlowInfo, LSDRaster& FlowDistance,
									  LSDRaster& ElevationRaster)
{
	vector<int> ChannelHeadNodes;
	vector<int> ChannelHeadNodes_temp;

  	vector<int> junction_list = extract_basins_order_outlet_junctions(BasinOrder, FlowInfo);
  	int max_junctions = junction_list.size();
  	cout << "No of junctions: " << max_junctions << endl;
  	int junction_number = 0;

	 //loop through junctions collecting channel heads
    for (int i = 0; i < max_junctions; i++)
  	{
		  cout << "Junction " << i << " of " << max_junctions << endl;

		  // get the junction number
		  junction_number = junction_list[i];

		  // get a local list of channel heads
		  vector<int> these_channel_heads = GetChannelHeadsChiMethodFromJunction(junction_number,
                                      			MinSegLength, A_0, m_over_n, FlowInfo,
                                       			FlowDistance, ElevationRaster);

      // now append these channel heads to the master list
		  ChannelHeadNodes_temp.insert(ChannelHeadNodes_temp.end(), these_channel_heads.begin(), these_channel_heads.end());
	  }
	  
	  // Removing any nodes that are not the furthest upstream
    int upstream_test = 0;
    vector<int>::iterator find_it;

    for (unsigned int node =0; node < ChannelHeadNodes_temp.size(); node++)
    {
      vector<int> tests;
      int current_node = ChannelHeadNodes_temp[node];
      for (unsigned int i = 0; i < ChannelHeadNodes_temp.size(); i++)
      {
        if (ChannelHeadNodes_temp[i] != current_node)
        {
          int test_node = ChannelHeadNodes_temp[i];
          upstream_test = FlowInfo.is_node_upstream(current_node, test_node);
          tests.push_back(upstream_test);
        }
      }
      find_it = find(tests.begin(), tests.end(), 1);
      if (find_it == tests.end())
      {
        ChannelHeadNodes.push_back(current_node);
      }
    }
    
    return ChannelHeadNodes;
}                              
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
// This function returns all potential channel heads in a DEM. It looks for
// channel heads based on the outlet junctions of the valleys (which are identified by looking 
// for portions of the landscape with 10 or more nodes with a high curvature that are linked)
// It returns a vector<int> of nodeindices where the channel heads are
//
// FC 31/10/13
//
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
vector<int> LSDChannelNetwork::GetChannelHeadsChiMethodFromValleys(Array2D<int>& ValleyJunctions,
                                      int MinSegLength, double A_0, double m_over_n,
									                    LSDFlowInfo& FlowInfo, LSDRaster& FlowDistance,
									                    LSDRaster& ElevationRaster)
{
	vector<int> ChannelHeadNodes;
	vector<int> ChannelHeadNodes_temp;
	vector<int> junction_list;
	
	for (int row = 0; row < NRows; row++)
	{
    for (int col =0; col < NCols; col++)
    {
        if (ValleyJunctions[row][col] != NoDataValue)
        {
          junction_list.push_back(ValleyJunctions[row][col]);
        }
    }
  }
  
  int max_junctions = junction_list.size();
  cout << "No of junctions: " << max_junctions << endl;
  int junction_number = 0;
  
  //loop through junctions collecting channel heads
  for (int i = 0; i < max_junctions; i++)
  {
	  cout << "Junction " << i+1 << " of " << max_junctions << endl;

    // get the junction number
	  junction_number = junction_list[i];
  
    // get a local list of channel heads
	  vector<int> these_channel_heads = GetChannelHeadsChiMethodFromJunction(junction_number,
                                      			MinSegLength, A_0, m_over_n, FlowInfo,
                                       			FlowDistance, ElevationRaster);

     // now append these channel heads to the master list
	  ChannelHeadNodes_temp.insert(ChannelHeadNodes_temp.end(), these_channel_heads.begin(), these_channel_heads.end());
	}
	  
	// Removing any nodes that are not the furthest upstream
  int upstream_test = 0;
  vector<int>::iterator find_it;

  for (unsigned int node =0; node < ChannelHeadNodes_temp.size(); node++)
  {
    vector<int> tests;
    int current_node = ChannelHeadNodes_temp[node];
    for (unsigned int i = 0; i < ChannelHeadNodes_temp.size(); i++)
    {
      if (ChannelHeadNodes_temp[i] != current_node)
      {
        int test_node = ChannelHeadNodes_temp[i];
        upstream_test = FlowInfo.is_node_upstream(current_node, test_node);
        tests.push_back(upstream_test);
      }
    }
    find_it = find(tests.begin(), tests.end(), 1);
    if (find_it == tests.end())
    {
      ChannelHeadNodes.push_back(current_node);
    }
  }  
    
  return ChannelHeadNodes;
}                              
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
// This function returns a 2D integer array containing the locations of all pixels identified
// as being part of the channel using chi profiles.  It calculates the chi and elevation value
// of every pixel upstream of the given junction, then bins this data and calculates the pixels
// in the 95th percentile of each bin.  Any pixels above the 95th percentile are considered part
// of the channel, and any below are considered to be hillslopes.  This is the first part of the
// channel head prediction using chi profiles.
//
// Parameters: Junction number, A_0, m over n, bin width (suggested value of 10), FlowInfo object,
// Elevation raster
// Returns: Array2D<int> with the channel pixel locations
//
// FC 01/10/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
Array2D<int> LSDChannelNetwork::GetChannelHeadsChiMethodAllPixels(int JunctionNumber,
                                      double A_0, double m_over_n, double bin_width, LSDFlowInfo& FlowInfo,
                                      LSDRaster& ElevationRaster)
{
  Array2D<int> channel_pixels(NRows,NCols,NoDataValue);
  // get the node index of this junction
	int starting_node = JunctionVector[JunctionNumber];
	string jn_name = itoa(JunctionNumber);
	string uscore = "_";
	jn_name = uscore+jn_name;

  //get the chi and elevation values of each upslope node
  vector<int> upslope_nodes = FlowInfo.get_upslope_nodes(starting_node);
  vector<double> upslope_chi = FlowInfo.get_upslope_chi(starting_node, m_over_n, A_0);
  vector<double> elevation;
  int row,col;

  string string_filename_all;
	string filename_all = "chi_profile_all";
	string dot = ".";
	string extension = "txt";
	string_filename_all = filename_all+jn_name+dot+extension;
	ofstream chi_profile_all;
	chi_profile_all.open(string_filename_all.c_str());
	//cout << "The filename is " << string_filename_all << endl;

  for (unsigned int node=0; node < upslope_nodes.size(); node++)
  {
    FlowInfo.retrieve_current_row_and_col(upslope_nodes[node], row, col);
    double elev = ElevationRaster.get_data_element(row,col);
    elevation.push_back(elev);
    chi_profile_all << upslope_chi[node] << " " << elev << endl;
  }

	string string_filename;
	string filename = "chi_profile";
	string_filename = filename+jn_name+dot+extension;
	//cout << "The filename is " << string_filename << endl;

	ofstream chi_profile;
	chi_profile.open(string_filename.c_str());

  double lower_limit = 0;
  vector<double> mean_chi;
  vector<double> mean_elev;
  vector<double> midpoints;
  vector<double> st_dev_chi;
  vector<double> st_dev_elev;
  vector<double> range_min;
  vector<double> range_max;

  int NoDataValue = FlowInfo.get_NoDataValue();

  //bin the data to find the range of the 95th percentile
  bin_data(upslope_chi, elevation, bin_width, mean_chi, mean_elev, midpoints,
           st_dev_chi, st_dev_elev, range_min, range_max, lower_limit, NoDataValue);

  //find the most linear channel segment (highest r2 value)
  int n_bins = mean_chi.size();
  int min_seg_length = n_bins/10;
  vector<double> channel_chi;
  vector<double> channel_elev;
  vector<double>::iterator vec_iter_start;
	vector<double>::iterator vec_iter_end;
	double max_r2 = 0;
	double elev_limit = 0;

  for (int channel_segment = min_seg_length; channel_segment <= n_bins-min_seg_length; channel_segment++)
  {
    //assigning the chi values of the channel segment
    channel_chi.resize(channel_segment);
    vec_iter_start = mean_chi.begin();
	  vec_iter_end = vec_iter_start+channel_segment;
	  channel_chi.assign(vec_iter_start,vec_iter_end);

	  // assigning the elevation values of the channel segment
    channel_elev.resize(channel_segment);
	  vec_iter_start = range_min.begin();
	  vec_iter_end = vec_iter_start+channel_segment;
	  channel_elev.assign(vec_iter_start,vec_iter_end);

	  //performing linear regression on channel segment: getting highest r2
	  vector<double> residuals_chan;
    vector<double> results_chan = simple_linear_regression(channel_chi,channel_elev, residuals_chan);
    double r2 = results_chan[2];
    if (r2 > max_r2)
    {
      max_r2 = r2;
      elev_limit = channel_elev.back();
    }
  }

  //extend the linear channel segment up through the plot
  vector<double> mean_chi_regression;
  vector<double> range_min_regression;
  vector<double> elev_regression;
  mean_chi_regression.resize(range_min.size());
  range_min_regression.resize(range_min.size());
  elev_regression.resize(range_min.size());
  double regression_pointer = 0;

  for (unsigned int i=0; i<range_min.size(); i++)
  {
    if (range_min[i] <= elev_limit)
    {
      if (range_min[i] !=0)
      {
        mean_chi_regression[i] = mean_chi[i];
        range_min_regression[i] = range_min[i];
        regression_pointer = i;
      }
    }
  }

  double x1 = mean_chi_regression.front();
  double y1 = range_min_regression.front();
  double x2 = mean_chi_regression[regression_pointer];
  double y2 = range_min_regression[regression_pointer];
  double gradient = (y2 - y1)/(x2 - x1);
  double intercept = y2 - (gradient * x2);

  for (int i = 0; i < n_bins; i++)
  {
    if (range_min[i] <=elev_limit)
    {
      elev_regression[i] = range_min[i];
    }
    else
    {
      elev_regression[i] = mean_chi[i]*gradient + intercept;
    }
  }

  for(int i = 0 ; i< n_bins; i++)
  {
    if (mean_chi[i] != 0)
    {
      chi_profile << mean_chi[i] << " " << mean_elev[i] << " " << range_min[i] << " " << range_max[i] << " " << elev_regression[i] << endl;
    }
  }
  chi_profile.close();

  //classify any nodes to the left of the channel segment as the channel; any nodes to the
  // right are classified as hillslopes
  vector<int> channel_nodes;
  vector<int> upslope_nodes_temp;
  vector<int> source_nodes;
  vector<int>::iterator iterator_find;

  for (unsigned int i=0; i < upslope_nodes.size(); i++)
  {
    int bin_id = int((upslope_chi[i]-lower_limit)/bin_width);
    FlowInfo.retrieve_current_row_and_col(upslope_nodes[i], row, col);
    if (upslope_chi[i] <= mean_chi[bin_id] && elevation[i] >= elev_regression[bin_id])
    {
      channel_pixels[row][col] = 1;
      channel_nodes.push_back(upslope_nodes[i]);
    }
    else
    {
      channel_pixels[row][col] = 0;
    }
  }

  return channel_pixels;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
// This function returns a 2D integer array containing the locations of all pixels identified
// as being part of the channel using chi profiles.  It calculates the chi and elevation value
// of every pixel upstream of the given junction, then bins this data and calculates the pixels
// in the 95th percentile of each bin.  Any pixels above the 95th percentile are considered part
// of the channel, and any below are considered to be hillslopes.  This is the first part of the
// channel head prediction using chi profiles.
//
// Parameters: Junction number, A_0, m over n, bin width (suggested value of 10), FlowInfo object,
// Elevation raster
// Returns: Array2D<int> with the channel pixel locations
//
// FC 01/10/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-
vector<int> LSDChannelNetwork::GetSourceNodesChiMethodAllPixels(int JunctionNumber,
                                      double A_0, double m_over_n, double bin_width, LSDFlowInfo& FlowInfo,
                                      LSDRaster& ElevationRaster)
{
  // get the node index of this junction
	int starting_node = JunctionVector[JunctionNumber];
	string jn_name = itoa(JunctionNumber);
	string uscore = "_";
	jn_name = uscore+jn_name;

  //get the chi and elevation values of each upslope node
  vector<int> upslope_nodes = FlowInfo.get_upslope_nodes(starting_node);
  vector<double> upslope_chi = FlowInfo.get_upslope_chi(starting_node, m_over_n, A_0);
  vector<double> elevation;
  int row,col;

  for (unsigned int node=0; node < upslope_nodes.size(); node++)
  {
    FlowInfo.retrieve_current_row_and_col(upslope_nodes[node], row, col);
    double elev = ElevationRaster.get_data_element(row,col);
    elevation.push_back(elev);
  }

  double lower_limit = 0;
  vector<double> mean_chi;
  vector<double> mean_elev;
  vector<double> midpoints;
  vector<double> st_dev_chi;
  vector<double> st_dev_elev;
  vector<double> range_min;
  vector<double> range_max;

  int NoDataValue = FlowInfo.get_NoDataValue();

  //bin the data to find the range of the 95th percentile
  bin_data(upslope_chi, elevation, bin_width, mean_chi, mean_elev, midpoints,
           st_dev_chi, st_dev_elev, range_min, range_max, lower_limit, NoDataValue);

  //find the most linear channel segment (highest r2 value)
  int n_bins = mean_chi.size();
  int min_seg_length = n_bins/10;
  vector<double> channel_chi;
  vector<double> channel_elev;
  vector<double>::iterator vec_iter_start;
	vector<double>::iterator vec_iter_end;
	double max_r2 = 0;
	double elev_limit = 0;

  for (int channel_segment = min_seg_length; channel_segment <= n_bins-min_seg_length; channel_segment++)
  {
    //assigning the chi values of the channel segment
    channel_chi.resize(channel_segment);
    vec_iter_start = mean_chi.begin();
	  vec_iter_end = vec_iter_start+channel_segment;
	  channel_chi.assign(vec_iter_start,vec_iter_end);

	  // assigning the elevation values of the channel segment
    channel_elev.resize(channel_segment);
	  vec_iter_start = range_min.begin();
	  vec_iter_end = vec_iter_start+channel_segment;
	  channel_elev.assign(vec_iter_start,vec_iter_end);

	  //performing linear regression on channel segment: getting highest r2
	  vector<double> residuals_chan;
    vector<double> results_chan = simple_linear_regression(channel_chi,channel_elev, residuals_chan);
    double r2 = results_chan[2];
    if (r2 > max_r2)
    {
      max_r2 = r2;
      elev_limit = channel_elev.back();
    }
  }

  //extend the linear channel segment up through the plot
  vector<double> mean_chi_regression;
  vector<double> range_min_regression;
  vector<double> elev_regression;
  mean_chi_regression.resize(range_min.size());
  range_min_regression.resize(range_min.size());
  elev_regression.resize(range_min.size());
  double regression_pointer = 0;

  for (unsigned int i=0; i<range_min.size(); i++)
  {
    if (range_min[i] <= elev_limit)
    {
      if (range_min[i] !=0)
      {
        mean_chi_regression[i] = mean_chi[i];
        range_min_regression[i] = range_min[i];
        regression_pointer = i;
      }
    }
  }

  double x1 = mean_chi_regression.front();
  double y1 = range_min_regression.front();
  double x2 = mean_chi_regression[regression_pointer];
  double y2 = range_min_regression[regression_pointer];
  double gradient = (y2 - y1)/(x2 - x1);
  double intercept = y2 - (gradient * x2);

  for (int i = 0; i < n_bins; i++)
  {
    if (range_min[i] <=elev_limit)
    {
      elev_regression[i] = range_min[i];
    }
    else
    {
      elev_regression[i] = mean_chi[i]*gradient + intercept;
    }
  }

  //classify any nodes to the left of the channel segment as the channel; any nodes to the
  // right are classified as hillslopes
  vector<int> channel_nodes;
  vector<int> source_nodes;

  for (unsigned int i=0; i < upslope_nodes.size(); i++)
  {
    int bin_id = int((upslope_chi[i]-lower_limit)/bin_width);
    FlowInfo.retrieve_current_row_and_col(upslope_nodes[i], row, col);
    if (upslope_chi[i] <= mean_chi[bin_id] && elevation[i] >= elev_regression[bin_id])
    {
      channel_nodes.push_back(upslope_nodes[i]);
    }
  }

  // find the furthest upslope nodes classified as being part of the channel network (use as sources for next
  // step of chi method)

  int upstream_test = 0;
  vector<int>::iterator find_it;

  for (unsigned int node =0; node < channel_nodes.size(); node++)
  {
     vector<int> tests;
     int current_node = channel_nodes[node];
     for (unsigned int i = 0; i < channel_nodes.size(); i++)
     {
      if (channel_nodes[i] != current_node)
      {
        int test_node = channel_nodes[i];
        upstream_test = FlowInfo.is_node_upstream(current_node, test_node);
        tests.push_back(upstream_test);
      }
     }
     find_it = find(tests.begin(), tests.end(), 1);
     if (find_it == tests.end())
     {
      source_nodes.push_back(current_node);
     }
  }
  cout << "No of channel nodes: " << channel_nodes.size() << endl;
  cout << "No of source nodes: " << source_nodes.size() << endl;
  return source_nodes;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// PREDICTING CHANNEL HEADS USING TANGENTIAL CURVATURE
//
// This function is used to predict channel head locations based on the method proposed by
// Pelletier (2013).  It creates a contour curvature map and identifies channel heads as pixels greater
// than a user defined contour curvature threshold value, set by default at 0.1.  The threshold curvature
// can also be defined as a multiple of the standard deviation of the curvature.  Before this function is called
// the DEM must be filtered using the wiener filter in the LSDRasterSpectral object in order to remove high frequency
// noise.
//
// Reference: Pelletier (2013) A robust, two-parameter method for the extraction of drainage
// networks from high-resolution digital elevation models (DEMs): Evaluation using synthetic and real-world
// DEMs, Water Resources Research 49: 1-15
//
// added by FC 16/07/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

vector<int> LSDChannelNetwork::calculate_pelletier_channel_heads(double tan_curv_threshold, LSDFlowInfo& FlowInfo, Array2D<double>& tan_curv_array)
{
	cout << "Getting Pelletier channel heads" << endl;
  Array2D<double> chan_head_locations(NRows,NCols,NoDataValue);
	double total_curv = 0;
	int n_observations = 0;

  //get the mean of the tangential curvature
  for (int row = 0; row < NRows; row++)
	{
    for(int col = 0; col < NCols; col++)
    {
      if (tan_curv_array[row][col] != NoDataValue)
      {
        total_curv = total_curv + tan_curv_array[row][col];
        ++n_observations;
      }
    }
  }

  double mean_curv = total_curv/n_observations;
  double total_st_dev = 0;

  // get the standard deviation of the curvature and use 3*st dev as the threshold value
  for (int row = 0; row < NRows; row++)
	{
    for(int col = 0; col < NCols; col++)
    {
      if (tan_curv_array[row][col] != NoDataValue)
      {
        total_st_dev = ((tan_curv_array[row][col] - mean_curv)*(tan_curv_array[row][col] - mean_curv)) + total_st_dev;
      }
    }
  }

  double st_dev = sqrt(total_st_dev/n_observations);
  tan_curv_threshold = 3*st_dev;
  cout << "Got standard deviation" << endl;

  // Get all the locations where the tan curvature is greater than the user defined threshold
  vector<int> channel_nodes;
  int CurrentNodeIndex = 0;
  
  for (int row = 0; row < NRows; row++)
	{
    for(int col = 0; col < NCols; col++)
    {
      if (tan_curv_array[row][col] > tan_curv_threshold)
      {
        chan_head_locations[row][col] = tan_curv_array[row][col];
        CurrentNodeIndex = FlowInfo.NodeIndex[row][col];
        channel_nodes.push_back(CurrentNodeIndex);
      }
      else
      {
        chan_head_locations[row][col] = 0;
      }
    }
  }
  cout << "Got channel nodes" << endl;  
  // STEP 3: Finding the furthest upstream channel node
  
  int upstream_test = 0;
  vector<int>::iterator find_it;
  vector<int> source_nodes;

  // identify whether there are any further upstream channel nodes, if not then
  // the node is a channel head
  for (unsigned int node =0; node < channel_nodes.size(); node++)
  {
     vector<int> tests;
     int current_node = channel_nodes[node];
     for (unsigned int i = 0; i < channel_nodes.size(); i++)
     {
      if (channel_nodes[i] != current_node)
      {
        int test_node = channel_nodes[i];
        upstream_test = FlowInfo.is_node_upstream(current_node, test_node);
        tests.push_back(upstream_test);
      }
     }
     find_it = find(tests.begin(), tests.end(), 1);
     if (find_it == tests.end())
     {
      source_nodes.push_back(current_node);
     }
  }
  cout << "Got source nodes" << endl;
  cout << "No of channel nodes: " << channel_nodes.size() << " No of source nodes: " << source_nodes.size() << endl;
  return source_nodes;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// IDENTIFYING VALLEYS USING TANGENTIAL CURVATURE
//
// This function is used to identify concave portions of the landscape using a tangential
// curvature threshold. It defines the threshold based on a multiple of the standard deviation
// of the curvature.  It then identifies valleys in which there are a linked series of pixels
// which have a curvature value greater than the threshold, and finds the outlet junction number
// of this valley.  This can be passed to the channel head prediction algorithm using the chi
// method.
// Reference: Peuker, T. K. and D. H. Douglas, (1975), "Detection of surface-specific points by local
// parallel processing of discrete terrain elevation data," Comput. Graphics Image Process., 4: 375-387
//
// added by FC 28/10/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<int> LSDChannelNetwork::find_valleys(LSDFlowInfo& FlowInfo, Array2D<double>& tan_curv_array, vector<int> sources)
{
  Array2D<int> NodesVisitedBefore(NRows,NCols,0);
  Array2D<int> NodesVisitedBeforeTemp(NRows,NCols,0);
  Array2D<int> valley_junctions(NRows,NCols,NoDataValue);   
  vector<int> valley_nodes;
  double tan_curv_threshold = 0.1;
    
  //Find valleys with linked pixels greater than the threshold
  int n_sources = sources.size();
  cout << "No of sources: " << n_sources << endl;
  int min_segment_length = 10;
  
  // Loop through all the sources, moving downstream - keep a count of the number of connected
  // nodes that are above the threshold curvature.  If there are more than 5 nodes that are 
  // connected then it is a valley - get the outlet junction of the valley and store in a vector
  
  for (int source = 0; source < n_sources; source++)
  {
    bool EndofReach = false;
    int max_no_connected_nodes =0;
    int CurrentNode = sources[source];
    int CurrentRow,CurrentCol,ReceiverNode,ReceiverRow,ReceiverCol; 
    
    while (EndofReach == false)
    {
      FlowInfo.retrieve_current_row_and_col(CurrentNode,CurrentRow,CurrentCol);
      FlowInfo.retrieve_receiver_information(CurrentNode, ReceiverNode, ReceiverRow, ReceiverCol);
      if (tan_curv_array[CurrentRow][CurrentCol] != NoDataValue)
      {
        double node_curvature = tan_curv_array[CurrentRow][CurrentCol];
        //cout << node_curvature << endl;
        NodesVisitedBefore[CurrentRow][CurrentCol] = 1;
    
        if (node_curvature > tan_curv_threshold)
        {
          ++max_no_connected_nodes;
          //cout << "Max no of connecting nodes: " << max_no_connected_nodes << endl;
        }
        else
        {
          max_no_connected_nodes = 0;
        }
        
        //check whether the no of connected nodes has been reached; if it has then identify the 
        // outlet junction of the valley
        if (max_no_connected_nodes > min_segment_length)
        {
          EndofReach = true;
          int this_node = CurrentNode;
          int current_row,current_col,downslope_node,downslope_row,downslope_col,current_SO,downslope_SO;
          bool reached_outlet = false;
          while (reached_outlet == false)
          {
            FlowInfo.retrieve_current_row_and_col(this_node, current_row, current_col);
            FlowInfo.retrieve_receiver_information(this_node, downslope_node, downslope_row, downslope_col);
            current_SO = StreamOrderArray[current_row][current_col];
            downslope_SO = StreamOrderArray[downslope_row][downslope_col];
            NodesVisitedBeforeTemp[current_row][current_col] = 1;
            bool BeentoReceiver = false;
            if (downslope_SO > current_SO)
            {
              valley_nodes.push_back(this_node);
              int valley_junction = find_upstream_junction_from_channel_nodeindex(this_node, FlowInfo);
              valley_junctions[current_row][current_col] = valley_junction;
              reached_outlet = true;
             // cout << "valley junction: " << valley_junction << endl;
            }
            if (NodesVisitedBeforeTemp[downslope_row][downslope_col] ==1) BeentoReceiver = true;
            if(BeentoReceiver == false) 
            {
              //Move downstream
              this_node = downslope_node;        
            }
            else
            {
              //Push back the valley node
              reached_outlet = true;
            }
          }   
        }  
        
        bool ReceiverVisitedBefore = false;
        // test to see whether we have visited this node before
        if(NodesVisitedBefore[ReceiverRow][ReceiverCol]==1) ReceiverVisitedBefore = true;
        if(ReceiverVisitedBefore == false) 
        {
          //Move downstream
          CurrentNode = ReceiverNode;        
        }
        else
        {
          //Move to next source
          EndofReach = true;
        }
      }
      else
      {
        EndofReach = true;
      }
    }      
  }

  return valley_junctions;  
}  
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
                   
=======
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
                 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// IDENTIFYING VALLEYS USING TANGENTIAL CURVATURE
//
// This function is used to identify concave portions of the landscape using a tangential
// curvature threshold. It defines the threshold based on a multiple of the standard deviation
// of the curvature.  It then identifies valleys in which there are a linked series of pixels
// which have a curvature value greater than the threshold, and finds the outlet junction number
// of this valley.  This can be passed to the channel head prediction algorithm using the chi
// method.
// Reference: Peuker, T. K. and D. H. Douglas, (1975), "Detection of surface-specific points by local
// parallel processing of discrete terrain elevation data," Comput. Graphics Image Process., 4: 375-387
//
// added by FC 28/10/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<int> LSDChannelNetwork::find_valleys(LSDFlowInfo& FlowInfo, Array2D<double>& tan_curv_array, vector<int> sources)
{
  Array2D<int> NodesVisitedBefore(NRows,NCols,0);
  Array2D<int> NodesVisitedBeforeTemp(NRows,NCols,0);
  Array2D<int> valley_junctions(NRows,NCols,NoDataValue);   
  vector<int> valley_nodes;
  double tan_curv_threshold = 0.1;
    
  //Find valleys with linked pixels greater than the threshold
  int n_sources = sources.size();
  cout << "No of sources: " << n_sources << endl;
  int min_segment_length = 10;
  
  // Loop through all the sources, moving downstream - keep a count of the number of connected
  // nodes that are above the threshold curvature.  If there are more than 5 nodes that are 
  // connected then it is a valley - get the outlet junction of the valley and store in a vector
  
  for (int source = 0; source < n_sources; source++)
  {
    bool EndofReach = false;
    int max_no_connected_nodes =0;
    int CurrentNode = sources[source];
    int CurrentRow,CurrentCol,ReceiverNode,ReceiverRow,ReceiverCol; 
    
    while (EndofReach == false)
    {
      FlowInfo.retrieve_current_row_and_col(CurrentNode,CurrentRow,CurrentCol);
      FlowInfo.retrieve_receiver_information(CurrentNode, ReceiverNode, ReceiverRow, ReceiverCol);
      if (tan_curv_array[CurrentRow][CurrentCol] != NoDataValue)
      {
        double node_curvature = tan_curv_array[CurrentRow][CurrentCol];
        //cout << node_curvature << endl;
        NodesVisitedBefore[CurrentRow][CurrentCol] = 1;
    
        if (node_curvature > tan_curv_threshold)
        {
          ++max_no_connected_nodes;
          //cout << "Max no of connecting nodes: " << max_no_connected_nodes << endl;
        }
        else
        {
          max_no_connected_nodes = 0;
        }
        
        //check whether the no of connected nodes has been reached; if it has then identify the 
        // outlet junction of the valley
        if (max_no_connected_nodes > min_segment_length)
        {
          EndofReach = true;
          int this_node = CurrentNode;
          int current_row,current_col,downslope_node,downslope_row,downslope_col,current_SO,downslope_SO;
          bool reached_outlet = false;
          while (reached_outlet == false)
          {
            FlowInfo.retrieve_current_row_and_col(this_node, current_row, current_col);
            FlowInfo.retrieve_receiver_information(this_node, downslope_node, downslope_row, downslope_col);
            current_SO = StreamOrderArray[current_row][current_col];
            downslope_SO = StreamOrderArray[downslope_row][downslope_col];
            NodesVisitedBeforeTemp[current_row][current_col] = 1;
            bool BeentoReceiver = false;
            if (downslope_SO > current_SO)
            {
              valley_nodes.push_back(this_node);
              int valley_junction = find_upstream_junction_from_channel_nodeindex(this_node, FlowInfo);
              valley_junctions[current_row][current_col] = valley_junction;
              reached_outlet = true;
             // cout << "valley junction: " << valley_junction << endl;
            }
            if (NodesVisitedBeforeTemp[downslope_row][downslope_col] ==1) BeentoReceiver = true;
            if(BeentoReceiver == false) 
            {
              //Move downstream
              this_node = downslope_node;        
            }
            else
            {
              //Push back the valley node
              reached_outlet = true;
            }
          }   
        }  
        
        bool ReceiverVisitedBefore = false;
        // test to see whether we have visited this node before
        if(NodesVisitedBefore[ReceiverRow][ReceiverCol]==1) ReceiverVisitedBefore = true;
        if(ReceiverVisitedBefore == false) 
        {
          //Move downstream
          CurrentNode = ReceiverNode;        
        }
        else
        {
          //Move to next source
          EndofReach = true;
        }
      }
      else
      {
        EndofReach = true;
      }
    }      
  }

  return valley_junctions;  
}  

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function extracts the ridge network by defining it as the co-boundaries
// of basins of equivalent order, for all basin orders within the landscape.
// This is relatively trivial since in each array containing the basins of the
// same order, each basin is labelled with a unique identifier, thus co-
// boundaries are found by locating pixels that neighbour pixels from another
// basin of the same order.
//
// Updated to return an LSDRaster object as ridges can now be assigned CHT values,
// using LSDRaster::RidgeSample, which are not integers. - SWDG
//
// DTM 18/10/2012
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDChannelNetwork::ExtractRidges(LSDFlowInfo& FlowInfo)
{
  //LSDChannelNetwork ChanNetwork(sources, FlowInfo);
  Array2D<double> RidgeNetwork(NRows,NCols,NoDataValue);
  // Find maximum stream order
  int MaxStreamOrder = 0;
  for (int i = 0; i < int(StreamOrderVector.size()); ++i)
  {
    if (StreamOrderVector[i] != NoDataValue && StreamOrderVector[i] > MaxStreamOrder)
    {
      MaxStreamOrder = StreamOrderVector[i];
    } // end logic for updating maximum stream order
  }   // end loop through StreamOrderVector

  // Loop through basin orders getting basin and then finding adjacent basin
  // margins
  for (int order = 1; order < MaxStreamOrder + 1; ++order)
  {
    LSDIndexRaster basins = ExtractBasinsOrder(order, FlowInfo);
    for (int row = 0; row < NRows; ++row)
    {
      for (int col = 0; col < NCols; ++col)
      {
        if (basins.get_data_element(row, col) != NoDataValue)
        {
          // Deal with corners
          if (row == 0 && col == 0)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
            		&& basins.get_data_element(row+1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col+1)
              		&& basins.get_data_element(row+1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue))
            {
              RidgeNetwork[row][col] = 1;
            }
          }
          else if (row == 0 && col == NCols - 1)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
            		&& basins.get_data_element(row+1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col-1)
              		&& basins.get_data_element(row+1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
              		&& basins.get_data_element(row, col-1) != NoDataValue))
            {
              RidgeNetwork[row][col] = 1;
            }
          }
          else if (row == NRows - 1  && col == 0)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
            		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col+1)
              		&& basins.get_data_element(row-1, col+1) != NoDataValue))
            {
              RidgeNetwork[row][col] = 1;
            }
          }
          else if (row == NRows - 1  && col == NCols - 1)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
            		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              (basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
             		&& basins.get_data_element(row, col-1) != NoDataValue) ||
              (basins.get_data_element(row, col) != basins.get_data_element(row-1, col-1)
              		&& basins.get_data_element(row-1, col-1) != NoDataValue))
            {
              RidgeNetwork[row][col] = 1;
            }
          }
          // Edge pixels
         else if (row == 0)
         {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
            		&& basins.get_data_element(row+1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col+1)
              		&& basins.get_data_element(row+1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col-1)
              		&& basins.get_data_element(row+1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
              		&& basins.get_data_element(row, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue))
            {
              RidgeNetwork[row][col] = 1;
            }
          }
          else if (row == NRows -1)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
            		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col+1)
              		&& basins.get_data_element(row-1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col-1)
              		&& basins.get_data_element(row-1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
              		&& basins.get_data_element(row, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue))
            {
              RidgeNetwork[row][col] = 1;
            }
          }
          else if (col == 0)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col+1)
            		&& basins.get_data_element(row+1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col+1)
              		&& basins.get_data_element(row-1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
              		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
              		&& basins.get_data_element(row+1, col) != NoDataValue))
            {
              RidgeNetwork[row][col] = 1;
            }
          }
          else if (col == NCols - 1)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col-1)
            		&& basins.get_data_element(row+1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col-1)
              		&& basins.get_data_element(row-1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
              		&& basins.get_data_element(row, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
              		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
              		&& basins.get_data_element(row+1, col) != NoDataValue))
            {
              RidgeNetwork[row][col] = 1;
            }
          }

          // Non edge pixels
          else if ((basins.get_data_element(row, col) != basins.get_data_element(row-1, col-1)
          				&& basins.get_data_element(row-1, col-1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
            			&& basins.get_data_element(row-1, col) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row-1, col+1)
            			&& basins.get_data_element(row-1, col+1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
           			 && basins.get_data_element(row, col-1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
            			&& basins.get_data_element(row, col+1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row+1, col-1)
            			&& basins.get_data_element(row+1, col-1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
            			&& basins.get_data_element(row+1, col) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row+1, col+1)
            			&& basins.get_data_element(row+1, col+1) != NoDataValue))
          {
            RidgeNetwork[row][col] = 1;
          }
        } // end logic for locating basin margins
      }   // end loop through col
    }     // end loop through row
  }       // end loop through different basin orders.
  // Return raster with all nth order drainage basins.
  	LSDRaster ridge_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,RidgeNetwork);
	return ridge_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This overloaded function extracts the ridge network for a defined stream
// order, passed in by the user.
//
// Updated to return an LSDRaster object as ridges can now be assigned CHT values,
// using LSDRaster::RidgeSample, which are not integers. - SWDG
//
// DTM 18/10/2012
// SWDG 28/03/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDRaster LSDChannelNetwork::ExtractRidges(LSDFlowInfo& FlowInfo, int& min_order, int& max_order)
{
  //LSDChannelNetwork ChanNetwork(sources, FlowInfo);
  Array2D<double> RidgeNetwork(NRows,NCols,NoDataValue);
  // Find maximum stream order
  int MaxStreamOrder = 0;
  for (int i = 0; i < int(StreamOrderVector.size()); ++i)
  {
    if (StreamOrderVector[i] != NoDataValue && StreamOrderVector[i] > MaxStreamOrder)
    {
      MaxStreamOrder = StreamOrderVector[i];
    } // end logic for updating maximum stream order
  }   // end loop through StreamOrderVector


  // Check min_order is lower than or equal to max_order, if test fails,
  // set min_order to 1
  if (min_order > max_order){
      min_order = 1;
      cout << "min_order larger than max_order, min_order set to 1" << endl;
  }

  // test the upper limit against the maximum possible value and set
  // MaxStreamOrder to the user defined value. If user supplied value
  // is too high, use original MaxStreamOrder value.
  if (max_order < MaxStreamOrder){
      MaxStreamOrder = max_order;
  }
  else if(max_order > MaxStreamOrder){
      cout << "max_order exceeds stream orders found in dem, max_order set to "
      << MaxStreamOrder << endl;
  }

  // Loop through basin orders getting basin and then finding adjacent basin
  // margins
  for (int order = min_order; order < MaxStreamOrder + 1; ++order)
  {
    LSDIndexRaster basins = ExtractBasinsOrder(order, FlowInfo);
    for (int row = 0; row < NRows; ++row)
    {
      for (int col = 0; col < NCols; ++col)
      {
        if (basins.get_data_element(row, col) != NoDataValue)
        {
          // Deal with corners
          if (row == 0 && col == 0)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
            		&& basins.get_data_element(row+1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col+1)
              		&& basins.get_data_element(row+1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue))
            {
              RidgeNetwork[row][col] = order;
            }
          }
          else if (row == 0 && col == NCols - 1)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
            		&& basins.get_data_element(row+1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col-1)
              		&& basins.get_data_element(row+1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
              		&& basins.get_data_element(row, col-1) != NoDataValue))
            {
              RidgeNetwork[row][col] = order;
            }
          }
          else if (row == NRows - 1  && col == 0)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
            		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col+1)
              		&& basins.get_data_element(row-1, col+1) != NoDataValue))
            {
              RidgeNetwork[row][col] = order;
            }
          }
          else if (row == NRows - 1  && col == NCols - 1)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
            		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              (basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
             		&& basins.get_data_element(row, col-1) != NoDataValue) ||
              (basins.get_data_element(row, col) != basins.get_data_element(row-1, col-1)
              		&& basins.get_data_element(row-1, col-1) != NoDataValue))
            {
              RidgeNetwork[row][col] = order;
            }
          }
          // Edge pixels
         else if (row == 0)
         {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
            		&& basins.get_data_element(row+1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col+1)
              		&& basins.get_data_element(row+1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col-1)
              		&& basins.get_data_element(row+1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
              		&& basins.get_data_element(row, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue))
            {
              RidgeNetwork[row][col] = order;
            }
          }
          else if (row == NRows -1)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
            		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col+1)
              		&& basins.get_data_element(row-1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col-1)
              		&& basins.get_data_element(row-1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
              		&& basins.get_data_element(row, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue))
            {
              RidgeNetwork[row][col] = order;
            }
          }
          else if (col == 0)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col+1)
            		&& basins.get_data_element(row+1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col+1)
              		&& basins.get_data_element(row-1, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
              		&& basins.get_data_element(row, col+1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
              		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
              		&& basins.get_data_element(row+1, col) != NoDataValue))
            {
              RidgeNetwork[row][col] = order;
            }
          }
          else if (col == NCols - 1)
          {
            if ((basins.get_data_element(row, col) != basins.get_data_element(row+1, col-1)
            		&& basins.get_data_element(row+1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col-1)
              		&& basins.get_data_element(row-1, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
              		&& basins.get_data_element(row, col-1) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
              		&& basins.get_data_element(row-1, col) != NoDataValue) ||
              	(basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
              		&& basins.get_data_element(row+1, col) != NoDataValue))
            {
              RidgeNetwork[row][col] = order;
            }
          }

          // Non edge pixels
          else if ((basins.get_data_element(row, col) != basins.get_data_element(row-1, col-1)
          				&& basins.get_data_element(row-1, col-1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row-1, col)
            			&& basins.get_data_element(row-1, col) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row-1, col+1)
            			&& basins.get_data_element(row-1, col+1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row, col-1)
           			 && basins.get_data_element(row, col-1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row, col+1)
            			&& basins.get_data_element(row, col+1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row+1, col-1)
            			&& basins.get_data_element(row+1, col-1) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row+1, col)
            			&& basins.get_data_element(row+1, col) != NoDataValue) ||
            		(basins.get_data_element(row, col) != basins.get_data_element(row+1, col+1)
            			&& basins.get_data_element(row+1, col+1) != NoDataValue))
          {
            RidgeNetwork[row][col] = order;
          }
        } // end logic for locating basin margins
      }   // end loop through col
    }     // end loop through row
  }       // end loop through different basin orders.
  // Return raster with all nth order drainage basins.
  LSDRaster ridge_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,RidgeNetwork);
	return ridge_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// ExtractHilltops
//------------------------------------------------------------------------------
// Resticts ridgeline to part of ridge network where the slope is less than a
// threshold value
//
// Now outputs an LSDRaster to fall in line with other hilltop tools - SWDG 29/8/13
//
// DTM 01/04/2013
//------------------------------------------------------------------------------
LSDRaster LSDChannelNetwork::ExtractHilltops(LSDRaster& RidgeRaster, LSDRaster& SlopeRaster, double MaxSlope)
{
  Array2D<double> Hilltops(NRows,NCols,NoDataValue) ;
  for (int row = 0; row < NRows; ++row)
  {
    for (int col = 0; col < NCols; ++col)
    {
      if (SlopeRaster.get_data_element(row,col) < MaxSlope)
      {
        Hilltops[row][col] = RidgeRaster.get_data_element(row,col);
      }
    }
  }
  LSDRaster hilltop_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,Hilltops);
	return hilltop_raster;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// This function iterates through the junction nodes and assigns the unique
// junction ID (q) to every stream pixel. This can be used with the LSDRaster
// hilltop_flow_routing function to assign a unique ID to each hilltop
// section tying it to a specific section of the channel network.
//
// Takes a flowinfo object and returns an LSDIndexRaster of the indexed channel
// newtork.
//
// SWDG - 04/04/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::ChannelIndexer(LSDFlowInfo& flowinfo)
{
  Array2D<int> StreamOutput(NRows,NCols,NoDataValue);

  int g = 0;  //ints to store the row and col of the current px
  int h = 0;

  for (int q = 1; q < NJunctions; ++q){

    if (q % 100 == 0){
      cout << "\tJunction = " << q << " / " << NJunctions << "    \r";
    }

    int sourcenodeindex = JunctionVector[q]; //first cell of segment
    int recieverjunction = ReceiverVector[q];
    int recievernodeindex = JunctionVector[recieverjunction]; //last cell of segment

    //get row and col of last px in junction. This location should not be written,
    //as it is the start of a new junction.
    int a = 0;
    int b = 0;
    flowinfo.retrieve_current_row_and_col(recievernodeindex,a,b);

    //write first pixel
    flowinfo.retrieve_current_row_and_col(sourcenodeindex,g,h);
    StreamOutput[g][h] = q;

    bool Flag = false; //Flag used to indicate if end of stream segemnt has been reached
    int CurrentNodeIndex = 0;

    while(Flag == false){

      CurrentNodeIndex = flowinfo.NodeIndex[g][h]; //update node index to move 1 px downstream
      flowinfo.retrieve_receiver_information(CurrentNodeIndex, recievernodeindex, g, h);

      if (CurrentNodeIndex == recievernodeindex){  //need to stop 1 px before node
          Flag = true;
      }
      else{
          StreamOutput[g][h] = q;
      }
    }
  }
  cout << endl;
  LSDIndexRaster IndexedChannels(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, StreamOutput);
  return IndexedChannels;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// SplitChannel
// This function splits the channel into a series of segments, providing a
// convenient unit with which to analyse landscapes.  The user provides the
// TargetSegmentLength, which specifies how many nodes should be in each 
// segment, and a MinimumSegmentLength, which specifies the fewest permissable
// number of nodes.  Segments smaller than this are amalgamated into the 
// upstream segment.
// The algorithm loops through the sources and traces downstream, stopping a 
// segment after the target segment length, when the stream order increases (to
// preserve structure of drainage network), or when a channel pixel has already
// been visited.
//
// DTM 23/10/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::SplitChannel(LSDFlowInfo& FlowInfo, vector<int> Sources, int TargetSegmentLength)//, int MinimumSegmentLength)
{
  //LSDChannelNetwork ChanNetwork(sources, FlowInfo);
  Array2D<int> ChannelSegments(NRows,NCols,int(NoDataValue));
  Array2D<int> NodesVisitedBefore(NRows,NCols,0);
  //----------------------------------------------------------------------------
  // 
  int SegmentID = 0;
  int N_Sources = Sources.size();
  // Loop through sources
  for (int i_source = 0; i_source < N_Sources; ++i_source)
  {
    bool EndOfReach = false;
    int NodeCount = 0;
    vector<int> ChannelNodesInSegment;
    int CurrentNode = Sources[i_source];
    int CurrentRow,CurrentCol,ReceiverNode,ReceiverRow,ReceiverCol,CurrentStreamOrder,ReceiverStreamOrder; 
    // Trace downstream until you rach the end of this channel reach
    while(EndOfReach == false)
    {
      FlowInfo.retrieve_current_row_and_col(CurrentNode,CurrentRow,CurrentCol);
      FlowInfo.retrieve_receiver_information(CurrentNode, ReceiverNode, ReceiverRow, ReceiverCol);

      ChannelSegments[CurrentRow][CurrentCol] = SegmentID;
      NodesVisitedBefore[CurrentRow][CurrentCol] = 1;
      ++NodeCount;
      
      // Now check whether we have enough channel nodes
      if(NodeCount >= TargetSegmentLength)
      {
        ++SegmentID;
        NodeCount = 0;
      }
      // Now check to see whether stream order increases (want to start a new
      // segment if this is the case)
      ReceiverStreamOrder = StreamOrderArray[ReceiverRow][ReceiverCol];
      CurrentStreamOrder = StreamOrderArray[CurrentRow][CurrentCol];
      if (ReceiverStreamOrder > CurrentStreamOrder)
      {
        NodeCount = 0;
        ++SegmentID;
      }
          
      bool ReceiverVisitedBefore = false;
      // test to see whether we have visited this node before
      if(NodesVisitedBefore[ReceiverRow][ReceiverCol]==1) ReceiverVisitedBefore = true;
      
      if(ReceiverVisitedBefore == true)
      {
        EndOfReach = true;
        ++SegmentID;
        ++i_source;
      }
      else
      {
        // Move downstream
        CurrentNode = ReceiverNode;
      }
    }  
  }
  LSDIndexRaster ChannelSegmentsRaster(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, ChannelSegments);
  return ChannelSegmentsRaster;    
}      

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// SplitHillslopes
// This function is intended to follow the SplitChannel function.  It traces
// through the receiver nodes from every hillslope pixel and then assigns them 
// an integer value that matches the index of the section of channel that is
// setting the base level of that hillslope.
//
// DTM 29/10/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::SplitHillslopes(LSDFlowInfo& FlowInfo, LSDIndexRaster& ChannelSegmentsRaster)
{
  Array2D<int> HillslopeSegmentArray(NRows,NCols,NoDataValue);
  Array2D<int> ChannelSegmentArray = ChannelSegmentsRaster.get_RasterData();
  vector<int> rows_visited,cols_visited;
  Array2D<int> VisitedBefore(NRows,NCols,0);
  int CurrentNode,ReceiverNode,ReceiverRow,ReceiverCol;
  // loop through the raster finding hillslope pixels
  for(int i = 0; i < NRows; ++i)
  {
    for(int j = 0; j < NCols; ++j)
    {
      // Has node been visited before?
      bool VisitedBeforeTest;
      if(VisitedBefore[i][j]==1)VisitedBeforeTest = true;  
      // If not visted before, then we can carry on, but mark as now visited
      else
      {
        VisitedBeforeTest = false;
        VisitedBefore[i][j]=1;
      }
      // Test that the node is a data node but not a channel node, and that it 
      // hasn't been visited yet!
      if((FlowInfo.NodeIndex[i][j]!=NoDataValue) && (ChannelSegmentArray[i][j]==NoDataValue) && (VisitedBeforeTest == false))
      {
        bool finish_trace = false; 
        CurrentNode = FlowInfo.NodeIndex[i][j];
        rows_visited.push_back(i);
        cols_visited.push_back(j);
        while(finish_trace == false)
        {
          FlowInfo.retrieve_receiver_information(CurrentNode, ReceiverNode, ReceiverRow, ReceiverCol);
          // if the receiver is a stream pixel then read through the vector of 
          // visited rows/columns and update hillslope segment array for each 
          // using the index of the channel segment.
          if(ChannelSegmentArray[ReceiverRow][ReceiverCol] != NoDataValue)
          {
            finish_trace = true;
            int N_nodes = rows_visited.size();
            for (int i_vec = 0; i_vec < N_nodes; ++i_vec)
            {
              HillslopeSegmentArray[rows_visited[i_vec]][cols_visited[i_vec]] = ChannelSegmentArray[ReceiverRow][ReceiverCol];
            }
            rows_visited.clear();
            cols_visited.clear();
          }
          // else if the receiver is a base level node, in which case it will 
          // never reach a channel -> set hillslope segment array for vector of 
          // visited rows and columns as nodata.
          else if (ReceiverNode == CurrentNode)
          {
            finish_trace = true;
            int N_nodes = rows_visited.size();
            for (int i_vec = 0; i_vec < N_nodes; ++i_vec)
            {
              HillslopeSegmentArray[rows_visited[i_vec]][cols_visited[i_vec]] = NoDataValue;
            }
            rows_visited.clear();
            cols_visited.clear();
          }
          // else if the receiver has been visited before, then read through the
          // vector of visited rows/columns and update hillslope segment array 
          // for each using the index of the receiver.
          else if(VisitedBeforeTest==true)
          {
            finish_trace = true;
            int N_nodes = rows_visited.size();
            for (int i_vec = 0; i_vec < N_nodes; ++i_vec)
            {
              HillslopeSegmentArray[rows_visited[i_vec]][cols_visited[i_vec]] = HillslopeSegmentArray[ReceiverRow][ReceiverCol];
            }
            rows_visited.clear();
            cols_visited.clear();
          }
          // otherwise the next pixel must be a hillslope pixel downslope that
          // has not yet been visited.  Add it to the vectors of visited points
          // and move downstream.
          else
          {
            rows_visited.push_back(ReceiverRow);
            cols_visited.push_back(ReceiverCol);
            VisitedBefore[ReceiverRow][ReceiverCol]=1;
            CurrentNode = ReceiverNode;
          }
        }
      } 
    }
  }
  LSDIndexRaster HillslopeSegmentsRaster(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, HillslopeSegmentArray);
  return HillslopeSegmentsRaster;          
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// SplitHillslopes
// This is an overloaded function doing the same as the previous version to
// segment hillslopes according to the channel index of the channel setting its
// base level.  However, this has been adapted to include an additional input
// raster - MultiThreadChannelRaster - which recognises that real channels may
// be multithreaded and/or have widths greater than or equal to one pixel.
// To be rigourous, these should be removed from analyses of hillslope
// properties.
//
// DTM 29/10/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::SplitHillslopes(LSDFlowInfo& FlowInfo, LSDIndexRaster& ChannelSegmentsRaster, LSDIndexRaster& MultiThreadChannelRaster)
{
  Array2D<int> HillslopeSegmentArray(NRows,NCols,NoDataValue);
  Array2D<int> ChannelSegmentArray = ChannelSegmentsRaster.get_RasterData();
  vector<int> rows_visited,cols_visited;
  Array2D<int> VisitedBefore(NRows,NCols,0);
  Array2D<int> MultiThreadChannelArray = MultiThreadChannelRaster.get_RasterData();
  int CurrentNode,ReceiverNode,ReceiverRow,ReceiverCol;
  
  // loop through the raster finding hillslope pixels
  for(int i = 0; i < NRows; ++i)
  {
    for(int j = 0; j < NCols; ++j)
    {
      // Has node been visited before?
      bool VisitedBeforeTest;
      if(VisitedBefore[i][j]==1)VisitedBeforeTest = true;  
      // If not visted before, then we can carry on, but mark as now visited
      else
      {
        VisitedBeforeTest = false;
        VisitedBefore[i][j]=1;
      }
      // Test that the node is a data node but not a channel node, and that it 
      // hasn't been visited yet!
      if((FlowInfo.NodeIndex[i][j]!=NoDataValue) && (ChannelSegmentArray[i][j] == NoDataValue)
          && (MultiThreadChannelArray[i][j] == 0) && (VisitedBeforeTest == false))
      {
        bool finish_trace = false; 
        bool reached_channel_but_trace_to_single_thread_channel = false;
        CurrentNode = FlowInfo.NodeIndex[i][j];
        rows_visited.push_back(i);
        cols_visited.push_back(j);
        while(finish_trace == false)
        {
          FlowInfo.retrieve_receiver_information(CurrentNode, ReceiverNode, ReceiverRow, ReceiverCol);
          if(VisitedBefore[ReceiverRow][ReceiverCol]==1)VisitedBeforeTest = true;  
          // if the receiver is a stream pixel then read through the vector of 
          // visited rows/columns and update hillslope segment array for each 
          // using the index of the channel segment.
          if(ChannelSegmentArray[ReceiverRow][ReceiverCol] != NoDataValue)
          {
            finish_trace = true;
            int N_nodes = rows_visited.size();
            for (int i_vec = 0; i_vec < N_nodes; ++i_vec)
            {
              HillslopeSegmentArray[rows_visited[i_vec]][cols_visited[i_vec]] = ChannelSegmentArray[ReceiverRow][ReceiverCol];
            }
            rows_visited.clear();
            cols_visited.clear();
          }
          // else if the receiver is a base level node, in which case it will 
          // never reach a channel -> set hillslope segment array for vector of 
          // visited rows and columns as nodata.
          else if (ReceiverNode == CurrentNode)
          {
            finish_trace = true;
            int N_nodes = rows_visited.size();
            for (int i_vec = 0; i_vec < N_nodes; ++i_vec)
            {
              HillslopeSegmentArray[rows_visited[i_vec]][cols_visited[i_vec]] = NoDataValue;
            }
            rows_visited.clear();
            cols_visited.clear();
          }
          // else if the receiver has been visited before, then read through the
          // vector of visted rows/columns and update hillslope segment array 
          // for each using the index of the receiver.
          else if(VisitedBeforeTest==true)
          {
            finish_trace = true;
            int N_nodes = rows_visited.size();
            for (int i_vec = 0; i_vec < N_nodes; ++i_vec)
            {
              HillslopeSegmentArray[rows_visited[i_vec]][cols_visited[i_vec]] = HillslopeSegmentArray[ReceiverRow][ReceiverCol];
            }
            rows_visited.clear();
            cols_visited.clear();
          }
          
          // otherwise the next pixel must be a hillslope pixel downslope that
          // has not yet been visited.  Add it to the vectors of visited points
          // and move downstream.
          else
          {
            if(MultiThreadChannelArray[ReceiverRow][ReceiverCol] == 1) reached_channel_but_trace_to_single_thread_channel = true;
            else
            {
              rows_visited.push_back(ReceiverRow);
              cols_visited.push_back(ReceiverCol);
              VisitedBefore[ReceiverRow][ReceiverCol]=1;
            }
            // Update CurrentNode to trace downstream
            CurrentNode = ReceiverNode;
          }
        }
      } 
    }
  }
  LSDIndexRaster HillslopeSegmentsRaster(NRows, NCols, XMinimum, YMinimum, DataResolution, NoDataValue, HillslopeSegmentArray);
  return HillslopeSegmentsRaster;          
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
// This function gets the node indices of outlets of basins of a certain order
//
// IMPORTANT: the junctions always point downstream since they can have one and only
// one receiver. However, for a basin of given order, this starts just upstream of the
// confluence to the next basin order. So the basin ##INCLUDES## the channel flowing
// downstream to the penultamite node
//
// the basin_reference_number is just a reference number for printing to
// the IndexRaster
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::extract_basin_from_junction(int basin_junction, int basin_reference_number, LSDFlowInfo& FlowInfo)
{


	if (basin_junction >= int(JunctionVector.size()))
	{
		cout << "LSDChannelNetwork::extract_basin_from_junction junction not in list" << endl;
		exit(EXIT_FAILURE);
	}

  	int receiver_junc, n_nodes_in_channel,basin_outlet;
	Array2D<int> Basin(NRows,NCols,NoDataValue);
	// get the reciever junction
  	receiver_junc = ReceiverVector[basin_junction];

  	LSDIndexChannel StreamLinkVector = LSDIndexChannel(basin_junction, JunctionVector[basin_junction],
                                                           receiver_junc, JunctionVector[receiver_junc], FlowInfo);

    // Find final nth order channel pixel, which is the penultimate pixel
    // in channel.
     n_nodes_in_channel = StreamLinkVector.get_n_nodes_in_channel();
     int node,row,col;

    basin_outlet = StreamLinkVector.get_node_in_channel(n_nodes_in_channel-2);
    vector<int> BasinNodeVector = FlowInfo.get_upslope_nodes(basin_outlet);
    // Loop through basin to label basin pixels with basin ID
    for (int BasinIndex = 0; BasinIndex < int(BasinNodeVector.size()); ++BasinIndex)
    {
		node = BasinNodeVector[BasinIndex];
        FlowInfo.retrieve_current_row_and_col(node,row,col);
        Basin[row][col] = basin_reference_number;
    }
	LSDIndexRaster IR(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, Basin);
	return IR;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
//This function gets the an LSDIndexRaster of basins draining from a vector of junctions.
//
// IMPORTANT: the junctions always point downstream since they can have one and only
// one receiver. However, for a basin of given order, this starts just upstream of the
// confluence to the next basin order. So the basin ##INCLUDES## the channel flowing
// downstream to the penultamite node
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::extract_basins_from_junction_vector(vector<int> basin_junctions, LSDFlowInfo& FlowInfo)
{

  Array2D<int> Basin(NRows,NCols,NoDataValue);

  for (vector<int>::iterator it = basin_junctions.begin(); it !=  basin_junctions.end(); ++it){

    int basin_junction = *it;

		if (basin_junction >= int(JunctionVector.size()))
		{
			cout << "LSDChannelNetwork::extract_basin_from_junction junction not in list" << endl;
			exit(EXIT_FAILURE);
		}

    int receiver_junc, n_nodes_in_channel, basin_outlet;

		// get the reciever junction
    	receiver_junc = ReceiverVector[basin_junction];

    	LSDIndexChannel StreamLinkVector = LSDIndexChannel(basin_junction, JunctionVector[basin_junction],
                                                             receiver_junc, JunctionVector[receiver_junc], FlowInfo);

      // Find final nth order channel pixel, which is the penultimate pixel
      // in channel.
      n_nodes_in_channel = StreamLinkVector.get_n_nodes_in_channel();
      int node,row,col;

      basin_outlet = StreamLinkVector.get_node_in_channel(n_nodes_in_channel-2);
      vector<int> BasinNodeVector = FlowInfo.get_upslope_nodes(basin_outlet);
      // Loop through basin to label basin pixels with basin ID
      for (int BasinIndex = 0; BasinIndex < int(BasinNodeVector.size()); ++BasinIndex)
      {
			node = BasinNodeVector[BasinIndex];
          FlowInfo.retrieve_current_row_and_col(node,row,col);
          Basin[row][col] = basin_junction;
      }

  }

	LSDIndexRaster IR(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, Basin);
	return IR;
}




//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this sends the StreamOrderArray to a LSDIndexRaster
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::StreamOrderArray_to_LSDIndexRaster()
{
	LSDIndexRaster IR(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, StreamOrderArray);
	return IR;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this sends the JunctionArray to a LSDIndexRaster
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::JunctionArray_to_LSDIndexRaster()
{
	LSDIndexRaster IR(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, JunctionArray);
	return IR;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this sends the JunctionIndexArray to a LSDIndexRaster
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::JunctionIndexArray_to_LSDIndexRaster()
{
	LSDIndexRaster IR(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, JunctionIndexArray);
	return IR;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this turns the StreamOrderArray into a binary rastser
// where
// 1 == channel
// 0 == hillslope
//
// SMM 01/09/2012
// fixed bug where output was georef to Xmin,Xmin instead of Xmin,Ymin --SWDG 2/7/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::StreamOrderArray_to_BinaryNetwork_LSDIndexRaster()
{
	Array2D<int> BinaryNetwork(NRows,NCols,0);
	for(int row = 0; row<NRows; row++)
	{
		for (int col = 0; col<NCols; col++)
		{
			if(StreamOrderArray[row][col] == NoDataValue)
			{
				BinaryNetwork[row][col] = NoDataValue;
			}
			else if (StreamOrderArray[row][col] >= 1)
			{
				BinaryNetwork[row][col] = 1;
			}
		}
	}

	LSDIndexRaster IR(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, BinaryNetwork);
	return IR;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this prints the junction information
// it does it in binary information
// there is a seperate 'pickle' function that puts everyting into binary format
//
// SMM 01/09/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDChannelNetwork::print_junction_info_vectors(string filename)
{
	string string_filename;
	string dot = ".";
	string extension = "txt";
	string_filename = filename+dot+extension;
	cout << "The filename is " << string_filename << endl;

	// print out all the donor, reciever and stack info
	ofstream donor_info_out;
	donor_info_out.open(string_filename.c_str());
	for(int i = 0; i<NJunctions; i++)
	{
		donor_info_out << i << " ";
	}
	donor_info_out << endl;
	for(int i = 0; i<NJunctions; i++)
	{
		donor_info_out << JunctionVector[i] << " ";
	}
	donor_info_out << endl;
	for(int i = 0; i<NJunctions; i++)
	{
		donor_info_out << StreamOrderVector[i] << " ";
	}
	donor_info_out << endl;
	for(int i = 0; i<NJunctions; i++)
	{
		donor_info_out << ReceiverVector[i] << " ";
	}
	donor_info_out << endl;
	for(int i = 0; i<NJunctions; i++)
	{
		donor_info_out << NDonorsVector[i] << " ";
	}
	donor_info_out << endl;
	for(int i = 0; i<NJunctions+1; i++)
	{
		donor_info_out << DeltaVector[i] << " ";
	}
	donor_info_out << endl;
	for(int i = 0; i<NJunctions; i++)
	{
		donor_info_out << DonorStackVector[i] << " ";
	}
	donor_info_out << endl;
	for(int i = 0; i<NJunctions; i++)
	{
		donor_info_out << SVector[i] << " ";
	}
	donor_info_out << endl;

	if( int(SVectorIndex.size()) == NJunctions)
	{
		for(int i = 0; i<NJunctions; i++)
		{
			donor_info_out << SVectorIndex[i] << " ";
		}
		donor_info_out << endl;
		for(int i = 0; i<NJunctions; i++)
		{
			donor_info_out << NContributingJunctions[i] << " ";
		}
		donor_info_out << endl;
	}

	cout << "LINE 746 " << endl;
	donor_info_out.close();
	cout << "LINE 749" << endl;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Quick and dirty way to get channels of a defined stream order, no input error handling,
// will return an LSDIndexRaster of NoDataValues if an erroneous order is passed in.
//
// Input an integer of the required stream order, returns an LSDIndexRaster of the desired
// channels. - SWDG 04/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::GetStreams(int order)
{
  Array2D<int> SingleStream(NRows,NCols,NoDataValue);

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (StreamOrderArray[i][j] == order){
        SingleStream[i][j] = StreamOrderArray[i][j];
      }
    }
  }

  LSDIndexRaster Stream(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, SingleStream);
	return Stream;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Overloaded version of the GetStreams function wich operates in the same way but takes a
// minimum and a maximum order integer and will return an LSDIndexRaster of the relevant
// channels. No error handling - could give odd results if strange values are passed in.
// SWDG 04/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::GetStreams(int min_order, int max_order)
{
  Array2D<int> SelectedStreams(NRows,NCols,NoDataValue);

  for (int i = 0; i < NRows; ++i){
    for (int j = 0; j < NCols; ++j){
      if (StreamOrderArray[i][j] >= min_order && StreamOrderArray[i][j] <= max_order){
        SelectedStreams[i][j] = StreamOrderArray[i][j];
      }
    }
  }

  LSDIndexRaster Stream(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, SelectedStreams);
	return Stream;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Function to test whether a junction's upstream nodes border areas of No Data
// important to ensure basins are not being artificially truncated
//
// Pass in a flowinfo object and the junction index you want to test
//
// returns a boolean indicating if no data values are present or not:
// false (0) = only good data values
// true (1) = no data values present
//
// Updated 24/10/13 to handle junction numbers in the same way that the basin extraction code does,
// by searching one junction downstream of the given junction and then going back up by one node. SWDG
//
// SWDG 27/06/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool LSDChannelNetwork::node_tester(LSDFlowInfo& FlowInfo, int input_junction)
{

  Array2D<int> FlowDirection = FlowInfo.get_FlowDirection();  //used as a proxy of the elevation data
  bool flag = false;
      
  //get reciever junction of the input junction
  int receiver_junc = ReceiverVector[input_junction];
      
  // Create channel segement from input junction down to receiver junction 
  LSDIndexChannel StreamLinkVector = LSDIndexChannel(input_junction, JunctionVector[input_junction],
                                                     receiver_junc, JunctionVector[receiver_junc], FlowInfo);
                
  // Get the number of nodes (DEM Cells) that make up the channel segment
  int n_nodes_in_channel = StreamLinkVector.get_n_nodes_in_channel();
       
  // get the penultimate node in the channel. Eg one pixel upstream from the outlet node of a basin.
  // -2 is used due to zero indexing.
  int basin_outlet = StreamLinkVector.get_node_in_channel(n_nodes_in_channel-2);      
  
  //Get all cells upslope of a junction - eg every cell of the drainage basin of interest
  vector<int> upslope_nodes = FlowInfo.get_upslope_nodes(basin_outlet);

  //loop over each cell in the basin and test for No Data values
  for(vector<int>::iterator it = upslope_nodes.begin(); it != upslope_nodes.end(); ++it){

    int i;
    int j;
    FlowInfo.retrieve_current_row_and_col(*it,i,j);
    
    //check for edges of the file
    if (i == 0 || i == (NRows - 1) || j == 0 || j == (NCols - 1)){
    flag = true;   
    return flag;}

    // check surrounding cells for NoDataValue
    else if (FlowDirection[i+1][j+1] == NoDataValue){flag = true;
    return flag;}
    else if (FlowDirection[i+1][j] == NoDataValue){flag = true;
    return flag;}
    else if (FlowDirection[i+1][j-1] == NoDataValue){flag = true;
    return flag;}
    else if (FlowDirection[i][j+1] == NoDataValue){flag = true;
    return flag;}
    else if (FlowDirection[i][j-1] == NoDataValue){flag = true;
    return flag;}
    else if (FlowDirection[i-1][j+1] == NoDataValue){flag = true;
    return flag;}
    else if (FlowDirection[i-1][j] == NoDataValue){flag = true;
    return flag;}
    else if (FlowDirection[i-1][j-1] == NoDataValue){flag = true;
    return flag;}
  }
  return flag;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This function returns a integer vector containing the junction number of the largest
// donor catchment (i.e. donor junction with greatest drainage area) upslope of each
// baselevel node. These can then be used as the starting locations for performing chi
// analysis.
//
// // IMPORTANT: the junctions always point downstream since they can have one and only
// one receiver. However, for a basin of given order, this starts just upstream of the
// confluence to the next basin order. So the basin ##INCLUDES## the channel flowing
// downstream to the penultamite node
//
// MDH 19/6/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
vector<int> LSDChannelNetwork::get_BaseLevel_DonorJunctions()
{
	vector<int> BL_Donor_junctions;

	//loope through baselevel junctions and get junction immediately upstrea,
	for (int junc = 0; junc< int(BaseLevelJunctions.size()); ++junc)
	{
		if(BaseLevelJunctions[junc] < 0 || BaseLevelJunctions[junc] > NJunctions-1)
		{
			cout << " Tried LSDChannelNetwork::get_BaseLevel_DonorJunctions but the"
			     << " junction number does not exist" << endl;
			exit(0);
		}

		int SVector_junction = SVectorIndex[BaseLevelJunctions[junc]];
		BL_Donor_junctions.push_back(SVector[SVector_junction+1]);
	}
	return BL_Donor_junctions;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// GET RECEIVER JUNCTION FOR SPECIFIED COORDINATES
// For input coordinates (e.g. the location of a cosmogenic radionuclide sample), get the
// closest downslope node of the catchment.  This enables easy extraction of catchment
// for analysis.
// DTM 17/10/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int LSDChannelNetwork::get_receiver_junction_for_specified_coordinates(double X_coordinate, double Y_coordinate, LSDFlowInfo& FlowInfo)
{
  // Shift origin to that of dataset
  double X_coordinate_shifted_origin = X_coordinate - XMinimum;
  double Y_coordinate_shifted_origin = Y_coordinate - YMinimum;

  // Get row and column of point
  int col_point = int(X_coordinate_shifted_origin/DataResolution);
  int row_point = (NRows - 1) - int(round(Y_coordinate_shifted_origin/DataResolution));

  // Find first downstream junction by running through receiver nodes until you
  // find a junction.
  int at_junction = 0;
  int CurrentNode = FlowInfo.NodeIndex[row_point][col_point];
  int ReceiverRow, ReceiverCol, ReceiverNode, junction;
  while(at_junction<1)
  {
    FlowInfo.retrieve_receiver_information(CurrentNode, ReceiverNode, ReceiverRow, ReceiverCol);
    CurrentNode = ReceiverNode;
    junction = retrieve_junction_number_at_row_and_column(ReceiverRow,ReceiverCol);
    //test to see if receiver node is in channel
    if(junction != NoDataValue) ++at_junction;
  }
  return junction;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// GET node index of nearest channel for specificed coordinates
// For input coordinates (e.g. the location of a cosmogenic radionuclide sample), get the
// node index of the nearest channel.  This enables easy extraction of catchment
// for analysis.
//
// The X_coordinate and Y_coordinate should be in the
// same spatial reference as the DEM, typically in UTM
//
// The threshold stream order is the stream order that qualifies as a 'channel'
//
// SMM 21/10/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int LSDChannelNetwork::get_nodeindex_of_nearest_channel_for_specified_coordinates(double X_coordinate,
                            double Y_coordinate, int search_radius_nodes, int threshold_stream_order, LSDFlowInfo& FlowInfo)
{

	// variables neighbor search
	int kernal_size = search_radius_nodes*2+1;
	int this_krow, this_kcol;
	int largest_SO_in_kernal;
	int this_SO;
	int largest_SO_row, largest_SO_col;

	// Shift origin to that of dataset
  	double X_coordinate_shifted_origin = X_coordinate - XMinimum;
  	double Y_coordinate_shifted_origin = Y_coordinate - YMinimum;

  	// Get row and column of point
  	int col_point = int(X_coordinate_shifted_origin/DataResolution);
  	int row_point = (NRows - 1) - int(round(Y_coordinate_shifted_origin/DataResolution));

  	// Find first downstream junction by running through receiver nodes until you
  	// find a junction.
  	int CurrentNode = FlowInfo.NodeIndex[row_point][col_point];
  	int ReceiverRow, ReceiverCol, ReceiverNode, CurrentCol, CurrentRow;

  	// get the current row and column
  	FlowInfo.retrieve_current_row_and_col(CurrentNode,CurrentRow,CurrentCol);

	// get the first receiver
	FlowInfo.retrieve_receiver_information(CurrentNode, ReceiverNode, ReceiverRow, ReceiverCol);


  	// make sure you are not at base level
  	int NearestChannel = NoDataValue;

	// check to see if this node has a stream order >= 1
	if(StreamOrderArray[CurrentRow][CurrentCol] >= threshold_stream_order)
	{
		NearestChannel = CurrentNode;
	}

	//cout << "Node: " << CurrentNode << " Row: " << CurrentRow << " Col: "
	//	 << CurrentCol << " SO: " << StreamOrderArray[CurrentRow][CurrentCol] << endl;

	// loop until you find a channel
	while(NearestChannel == NoDataValue && CurrentNode != ReceiverNode)
	{
		// now move down one node
		CurrentNode = ReceiverNode;

		// get the current row and column
		FlowInfo.retrieve_current_row_and_col(CurrentNode,CurrentRow,CurrentCol);

		//cout << "Node: " << CurrentNode << " Row: " << CurrentRow << " Col: "
		//     << CurrentCol << " SO: " << StreamOrderArray[CurrentRow][CurrentCol] << endl;

		// now search the kernal
		largest_SO_in_kernal = NoDataValue;
		largest_SO_row = NoDataValue;
		largest_SO_col = NoDataValue;
		for (int krow = 0; krow<kernal_size; krow++)
		{
			for (int kcol = 0; kcol<kernal_size; kcol++)
			{
				// get the row and column
				this_krow = CurrentRow-search_radius_nodes+krow;
				this_kcol = CurrentCol-search_radius_nodes+kcol;

				// only test if it within size of the Stream Order array
				if(this_krow >= 0 && this_krow < NRows-1 && this_kcol >= 0 && this_kcol < NCols-1)
				{
					this_SO = StreamOrderArray[this_krow][this_kcol];
					if (this_SO >= threshold_stream_order && this_SO > largest_SO_in_kernal)
					{
						largest_SO_in_kernal = this_SO;
						largest_SO_row = this_krow;
						largest_SO_col = this_kcol;
					}
				}
			}
		}

		// check to if the kernal returned a channel node
		if(largest_SO_in_kernal != NoDataValue)
		{
			NearestChannel = FlowInfo.NodeIndex[largest_SO_row][largest_SO_col];
		}
		else		// get the next node
		{
			FlowInfo.retrieve_receiver_information(CurrentNode, ReceiverNode, ReceiverRow, ReceiverCol);
			//cout << "CurrentNode: " << CurrentNode << " RN: " << ReceiverNode << endl;
		}

	}

	//cout << "Nearest_channel is: " << NearestChannel << endl;
	return NearestChannel;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Find upstream junction from channel nodeindex
//
// This function takes a nodeindex, checks to see if it is a channel, and if so
// it marches upstream until it finds a junction
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int LSDChannelNetwork::find_upstream_junction_from_channel_nodeindex(int ChannelNodeIndex, LSDFlowInfo& FlowInfo)
{
	int UpstreamJunction = NoDataValue;
	int CurrentNode = ChannelNodeIndex;
	int CurrentRow, CurrentCol;
	//int DonorRow, DonorCol;
	int junction;
	int donor_channel_order;

	// get the current row and column
	FlowInfo.retrieve_current_row_and_col(CurrentNode,CurrentRow,CurrentCol);

	// get the stream order
	int this_channel_order = StreamOrderArray[CurrentRow][CurrentCol];

	if (this_channel_order != NoDataValue)
	{
		// first test if this is a junction
		junction = retrieve_junction_number_at_row_and_column(CurrentRow,CurrentCol);
		if(junction != NoDataValue)
		{
			UpstreamJunction = junction;
		}



		while (UpstreamJunction == NoDataValue)
		{
			// get the donors
			vector<int> donors = FlowInfo.get_donor_nodes(CurrentNode);

			// now loop through the donors looking for a channel of the same order
			int n_donors = donors.size();
			int this_donor = 0;
			do
			{
				//cout << "this donor: " << this_donor << " and the donor NI: " << donors[this_donor] << endl;

				FlowInfo.retrieve_current_row_and_col(donors[this_donor],CurrentRow,CurrentCol);

				donor_channel_order = StreamOrderArray[CurrentRow][CurrentCol];

				//cout << "donor_channel_order: " << donor_channel_order << " and tcho: " << this_channel_order << endl;

				this_donor++;

			} while( donor_channel_order != this_channel_order && this_donor<n_donors);

			// now check if the donor is a junction
			junction = retrieve_junction_number_at_row_and_column(CurrentRow,CurrentCol);
			//cout << "Junction is: " << junction << endl;

			if(junction != NoDataValue)		// if it is, set the junction
			{
				UpstreamJunction = junction;
			}
			else			// if it isn't, go upslope
			{
				CurrentNode = donors[this_donor-1];
			}

			//cout << "Current node yo1: " << CurrentNode << endl;
		}


	}


	return UpstreamJunction;
}



#endif
