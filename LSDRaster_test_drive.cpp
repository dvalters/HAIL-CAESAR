//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDRaster_test_drive.cpp
//
// This program is used for testing the LSDRaster object
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Simon M. Mudd
// University of Edinburgh
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <string>
#include <vector>
#include "LSDRaster.hpp"
#include "LSDIndexRaster.hpp"
#include "LSDFlowInfo.hpp"
#include "LSDChannelNetwork.hpp"

int main ()
{


	//string DEM_name = "teeny_tiny_test";
	string DEM_name = "tiny_test";
	string DEM_extension = "asc";

	LSDRaster topo_test(DEM_name, DEM_extension);
	cout << "LINE 41" << endl;

	LSDRaster filled_topo_test = topo_test.fill();

	//string DEM_outname = "teeny_filltest";
	string DEM_outname = "tiny_filltest";
	string DEM_asc_extension = "asc";
	filled_topo_test.write_raster(DEM_outname,DEM_asc_extension);

	vector<string> boundary_conditions(4);
	boundary_conditions[0] = "No";
	boundary_conditions[1] = "no flux";
	boundary_conditions[2] = "no flux";
	boundary_conditions[3] = "No flux";


	//string FD_name = "teeny_tiny_test_FD2";
	//string NI_name = "teeny_tiny_test_NI2";
	//string FLC_name = "teeny_tiny_test_FLC2";
	//string CP_name = "teeny_tiny_test_CP";
	//string CP_name2 = "teeny_tiny_test_CPI";
	//string FDist_name = "teeny_tiny_test_FDist";
	//string SO_name = "teeny_tiny_test_SO";
	//string J_name = "teeny_tiny_test_J";
	string FD_name = "tiny_test_FD2";
	string NI_name = "tiny_test_NI2";
	string CP_name = "tiny_test_CP";
	string CP_name2 = "tiny_test_CPI";
	string FDist_name = "tiny_test_FDist";
	string SO_name = "tiny_test_SO";
	string J_name = "tiny_test_J";
	string JI_name = "tiny_test_JI";


	//NodeIndex.write_raster(NI_name,DEM_asc_extension);
	//FlowLengthCode.write_raster(FLC_name,DEM_asc_extension);

	// get a flow info object
	LSDFlowInfo FlowInfo(boundary_conditions,filled_topo_test);

	// print off some information
	LSDIndexRaster FlowDirection = FlowInfo.write_FlowDirection_to_LSDIndexRaster();
	FlowDirection.write_raster(FD_name,DEM_asc_extension);

	//LSDIndexRaster ContributingPixels = FlowInfo.calculate_n_pixels_contributing_from_upslope();
	LSDIndexRaster ContributingPixelsI = FlowInfo.calculate_n_pixels_contributing_from_upslope_include_self();
	//ContributingPixels.write_raster(CP_name,DEM_asc_extension);
	ContributingPixelsI.write_raster(CP_name2,DEM_asc_extension);

	FlowInfo.calculate_upslope_reference_indices();
	LSDIndexRaster ContributingPixelsI2 = FlowInfo.calculate_n_pixels_contributing_from_upslope_include_self_from_indices();
	ContributingPixelsI2.write_raster(CP_name,DEM_asc_extension);

	// test the get_upslope_nodes algorithm
	int node_number_outlet = 12;
	vector<int> upslope_nodes;

	for (int j = 10; j<14; j++)
	{
		node_number_outlet = j;
		upslope_nodes = FlowInfo.get_upslope_nodes(node_number_outlet);

		cout << "LINE 89 main, upslope_nodes from node " << node_number_outlet << endl;
		for (int i = 0; i< int(upslope_nodes.size()); i++)
		{
			cout << upslope_nodes[i] << endl;
		}
		cout << endl << endl;
	}

	// calcualte the distance from outlet
	LSDRaster DistanceFromOutlet = FlowInfo.distance_from_outlet();
	DistanceFromOutlet.write_raster(FDist_name,DEM_asc_extension);

	string FI_fname = "flow_information";
	FlowInfo.print_flow_info_vectors(FI_fname);

	// get sources where the contributing pixels (from the algorithm that counts hilltops
	// as having 1 pixel) as greater than or equal to a pixel number of 2.
	//
	int threshold = 1;
	vector<int> sources;
	sources = FlowInfo.get_sources_index_threshold(ContributingPixelsI, threshold);


	cout << "LINE 93 main initializing Channel network" << endl;
	LSDChannelNetwork ChanNetwork(sources, FlowInfo);
	LSDIndexRaster SOArray = ChanNetwork.StreamOrderArray_to_LSDIndexRaster();
	LSDIndexRaster JArray = ChanNetwork.JunctionArray_to_LSDIndexRaster();
	LSDIndexRaster JIArray = ChanNetwork.JunctionIndexArray_to_LSDIndexRaster();

	SOArray.write_raster(SO_name,DEM_asc_extension);
	JArray.write_raster(J_name,DEM_asc_extension);
	JIArray.write_raster(JI_name,DEM_asc_extension);


}
