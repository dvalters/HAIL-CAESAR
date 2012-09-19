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
// <your name here>
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Version 0.0.1		30/08/2012
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <vector>
#include "TNT/tnt.h"
//#include "LSDFlowInfo.hpp"
//#include "LSDRaster.hpp"
#include "LSDChannelNetwork.hpp"
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
	SourcesBaseLevelNodes = TempVector;

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

	//for(int src = 0; src<n_sources; src++)
	//{
	//	cout << "Source " << src << " and node: " <<  SourcesVector[src] << endl;
	//}

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

		//cout << endl << endl <<"Source " << src << " row: " << current_row << " and col: " << current_col << endl;

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
				//cout << "node: " << current_node << " i used to be NoData " << endl;
				StreamOrderArray[current_row][current_col] = current_stream_order;
			}
			// if it isn't a nodata node:
			// note that the junction switch starts out as a zero.
			// the channel is followed looking at nodata values in the stream order array
			// once it hits the first junction, the StreamOrderArray has finite values,
			// so this logic is triggered.
			else
			{
				//cout << "node: " << current_node << " I've been visited before " << endl;
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
				//
				// if junction switch is zero it means this is the first visit of a previously visited channel
				if (junction_switch == 0)
				{
					//cout << "node: " << current_node << " this is my source's first junction" << endl;
					junction_switch = 1;
					JunctionArray[current_row][current_col]  = 1;

					// if it is the the first junction for this source, the current_stream_order
					// is one. Therefore any junction will result in a stream order
					// of at least 2.
					// If the junction is currently at a stream order of 1, then it
					// gets incremented
					if (StreamOrderArray[current_row][current_col] == current_stream_order)
					{
						//cout << "I've become a second order or more junction" << endl;
						current_stream_order ++;
						StreamOrderArray[current_row][current_col]= current_stream_order;

					}
					// if the junction is two or greater, the loop exits since there can
					// be no more incrementing of the stream order
					else
					{
						//cout << "I'm already a second order or more junction" << endl;
						baselevel_switch = 2;
					}
				}
				// logic below for if this is not the first junction
				else
				{
					//cout << "node: " << current_node << "I'm not the first junction "<< endl;
					// nodes following downstream will be at the current stream order
					StreamOrderArray[current_row][current_col] = current_stream_order;

					// if it is a junction, see if there is an increment
					// in the current stream order
					// the node it has come from has an updated current stream order
					// so just look at the donor nodes to
					// see if there are two or more
					// donors at a higher stream order
					if (JunctionArray[current_row][current_col]  == 1)
					{
						// this node is a junction.
						//cout << "node " << current_node << " 2ndary jxn, curr SO: " << current_stream_order << endl;
						// get the number of donors
						n_current_stream_order_donors = 0;
						for(int dnode = 0; dnode<FlowInfo.NDonorsVector[current_node]; dnode++)
						{
							donor_node = FlowInfo.DonorStackVector[ FlowInfo.DeltaVector[current_node]+dnode];

							// ignore the base level donor
							if (donor_node != current_node)
							{
								donor_row = FlowInfo.RowIndex[ donor_node ];
								donor_col = FlowInfo.ColIndex[ donor_node ];

								//cout << "donor node: " << donor_node << " donor SO: " << StreamOrderArray[donor_row][donor_col] << endl;

								if (StreamOrderArray[donor_row][donor_col] == current_stream_order)
								{
									n_current_stream_order_donors++;
								}
							}
						}
						if (n_current_stream_order_donors >= 2)
						{

							current_stream_order++;
							StreamOrderArray[current_row][current_col] = current_stream_order;
							//cout << "raising my stream order! node: " << current_node << " new SO: " << current_stream_order << endl;
						}
						else
						{
							baselevel_switch = 2;
						}
					}
				}		// end logic for if this is not the first junction
			}			// end logic for if this is not a NoData node


			//cout << endl << endl << JunctionArray << endl << endl << StreamOrderArray << endl;

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

		// this source should have got to baselevel, set the junction number to 1 here
		//cout << "ChanNet Line 226 source number is: " << src << endl;
		//if (current_node == receiver_node)
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

		//cout << "LINE 257 ChNet, SOURCE: " << src <<  " current_node: " << current_node << " and rode: " << receiver_node << endl;

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
			SourcesBaseLevelNodes.push_back(current_node);
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
					SourcesBaseLevelNodes.push_back(this_junction);

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

	cout << "ChanNet; LINE 368; sz ReceiverVec: " << ReceiverVector.size() << " sz JuncVec: " << JunctionVector.size()
	     << " sz SOVec: " << StreamOrderVector.size() << endl;

	for(int i = 0; i< int(SourcesBaseLevelNodes.size()); i++)
	{
		cout << "LINE 382 bl node["<<i<<"]: " << SourcesBaseLevelNodes[i] << endl;
	}

	cout << "LINE 385: links data " << endl;
	for (int i = 0; i< int(StreamOrderVector.size()); i++)
	{
		cout << "Junc: " << i << " node: " << JunctionVector[i]
		     << " receiv: " << ReceiverVector[i] << " Order: " << StreamOrderVector[i] << endl;
	}


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
		//cout << "i: " << i << " r_i: " << r_index << " delta_i: " << delta_index << " w_index: " << w_index << endl;
	}

	// now go through the base level node list, building the drainage tree for each of these nodes as one goes along
	int n_base_level_nodes;
	n_base_level_nodes = SourcesBaseLevelNodes.size();

	int k;
	int j_index;
	int begin_delta_index, end_delta_index;
	int l_index;

	j_index = 0;
	for (int i = 0; i<n_base_level_nodes; i++)
	{
		//cout << "Base level node: " <<  BaseLevelNodeList[i] << endl;
		k = SourcesBaseLevelNodes[i];			// set k to the base level node

		// now run recursive algorithm
		begin_delta_index = DeltaVector[k];
		end_delta_index = DeltaVector[k+1];

		for (int delta_index = begin_delta_index; delta_index<end_delta_index; delta_index++)
		{
			l_index = DonorStackVector[delta_index];
			//cout << "delta_index is: " << delta_index << " and l_index is: " << l_index << endl;
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
		cout << "the junction number does not exist" << endl;
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
// this sends the StreamOrderArray to a LSDIndexRaster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::StreamOrderArray_to_LSDIndexRaster()
{
	LSDIndexRaster IR(NRows,NCols, XMinimum, XMinimum, DataResolution, NoDataValue, StreamOrderArray);
	return IR;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this sends the JunctionArray to a LSDIndexRaster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::JunctionArray_to_LSDIndexRaster()
{
	LSDIndexRaster IR(NRows,NCols, XMinimum, XMinimum, DataResolution, NoDataValue, JunctionArray);
	return IR;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this sends the JunctionIndexArray to a LSDIndexRaster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::JunctionIndexArray_to_LSDIndexRaster()
{
	LSDIndexRaster IR(NRows,NCols, XMinimum, XMinimum, DataResolution, NoDataValue, JunctionIndexArray);
	return IR;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this turns the StreamOrderArray into a binary rastser
// where
// 1 == channel
// 0 == hillslope
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LSDIndexRaster LSDChannelNetwork::StreamOrderArray_to_BinaryNetwork_LSDIndexRaster()
{
	Array2D<int> BinaryNetwork(NRows,NCols,0);
	for(int row = 0; row<NRows; row++)
	{
		for (int col = 0; col<NCols; col++)
		{
			if(StreamOrderArray[row][col] = NoDataValue)
			{
				BinaryNetwork[row][col] = NoDataValue;
			}
			else if (StreamOrderArray[row][col] >= 1)
			{
				BinaryNetwork[row][col] = 1;
			}
		}
	}
	LSDIndexRaster IR(NRows,NCols, XMinimum, XMinimum, DataResolution, NoDataValue, BinaryNetwork);
	return IR;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function extracts the longest channel originating from a junction number
// outlet_junction
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDChannelNetwork::print_longest_channel(int outlet_junction, LSDFlowInfo& FInfo, LSDIndexRaster& dist_code,
									LSDRaster& dist_from_outlet)
{
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


	// now you need to start from the farthest node and collect data until the
	// outlet junction

	// !!!TO BE DONE!!!
}

#endif
