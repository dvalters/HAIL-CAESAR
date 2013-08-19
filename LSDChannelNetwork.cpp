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
#include "TNT/tnt.h"
//#include "LSDFlowInfo.hpp"
//#include "LSDRaster.hpp"
#include "LSDChannelNetwork.hpp"
#include "LSDIndexChannel.hpp"
using namespace std;
using namespace TNT;

#ifndef LSDChannelNetwork_CPP
#define LSDChannelNetwork_CPP

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// create
// this defines a channel network based on a FlowInfo object
// and a list of source nodes
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
//
// This function generates and LSDIndexChannel given a starting junction
// NOTE: Each junction is the UPSTREAM end of a channel
// this is because junctions can have one and only once receiver
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
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
// downstream to the penultamite node
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
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
// downstream to the penultamite node
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
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
// DTM 01/04/2013
//------------------------------------------------------------------------------
LSDIndexRaster LSDChannelNetwork::ExtractHilltops(LSDIndexRaster& RidgeRaster, LSDRaster& SlopeRaster, double MaxSlope)
{
  Array2D<int> Hilltops(NRows,NCols,NoDataValue) ;
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
  LSDIndexRaster hilltop_raster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,Hilltops);
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
// This function gets the node indices of outlets of basins of a certain order
//
// IMPORTANT: the junctions always point downstream since they can have one and only
// one receiver. However, for a basin of given order, this starts just upstream of the
// confluence to the next basin order. So the basin ##INCLUDES## the channel flowing
// downstream to the penultamite node
//
// the basin_reference_number is just a reference number for printing to
// the IndexRaster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
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
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this sends the StreamOrderArray to a LSDIndexRaster
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
// SWDG 27/06/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool LSDChannelNetwork::node_tester(LSDFlowInfo& FlowInfo, int input_junction)
{

  Array2D<int> FlowDirection = FlowInfo.get_FlowDirection();  //used as a proxy of the elevation data

  bool flag = false;

  int node_index = get_Node_of_Junction(input_junction);
  vector<int> upslope_nodes = FlowInfo.get_upslope_nodes(node_index);

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

#endif
