#include "libgeodecomp.h"
#include "catchmentmodel/LSDCatchmentModel_geodecomp.hpp"

using namespace LibGeoDecomp;


class Cell
{
public:
  class API :
    public APITraits::HasStencil<Stencils::VonNeumann<2,1> >
  {};


  Cell()
  {}

  double elev, water_depth;
  double qx, qy, qxs, qys;
  TNT::Array1D<double> vel_dir = TNT::Array1D<double> (9, 0.0); // refactor (redefinition of declaration in include/catchmentmodel/LSDCatchmentModel.hpp
  double waterinput = 0; // refactor (already declared in include/catchmentmodel/LSDCatchmentModel.hpp)
  unsigned rfnum = 1; // refactor (already declared in include/catchmentmodel/LSDCatchmentModel.hpp)


  // refactor - Should replace these defines with type alias declarations (= C++11 template typedef)
  // refactor - check that grid orientation makes sense (write test)
#define WEST neighborhood[Coord<2>(-1, 0)]   // refactor: have used this for array[x-1][y]
#define EAST neighborhood[Coord<2>( 1, 0)]  // refactor: have used this for array[x+1][y]
#define NORTH neighborhood[Coord<2>( 0, 1)] // refactor: have used this for array[x][y-1]
#define SOUTH neighborhood[Coord<2>( 0, -1)] // refactor: have used this for array[x][y+1]


  // DEFINING TEMPORARILY TO BE ABLE TO COMPILE - refactor
  // *****************************************************
  double hflow_threshold;
  double DX;
  double time_factor;
  double local_time_factor = time_factor; // refactor?
  double gravity; // should come from topotools/LSDRaster?
  double mannings;
  double froude_limit;
  // *****************************************************




  template<typename COORD_MAP>
  void update(const COORD_MAP& neighborhood, unsigned nanoStep)
  {
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // THE WATER ROUTING ALGORITHM: LISFLOOD-FP
    //
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


    if (elev > -9999) // to stop moving water in to -9999's on elev
      {


	//---------------------------
	// routing in x direction
	//---------------------------
	if((water_depth > 0 || WEST.water_depth > 0) && WEST.elev > -9999)
	  // need to check water and not -9999 on elev?
	  {
	    double hflow = std::max(elev + water_depth, WEST.elev + WEST.water_depth) - std::max(elev, WEST.elev);

	    if (hflow > hflow_threshold)
	      {
		double tempslope = ((WEST.elev + WEST.water_depth) - (elev + water_depth)) / DX;

		// Refactor for geodecomp
		//if (x == imax) tempslope = edgeslope;
		//if (x <= 2) tempslope = 0 - edgeslope;

		qx = ((qx - (gravity * hflow * local_time_factor * tempslope)) \
		      / (1 + gravity * hflow * local_time_factor * (mannings * mannings) \
			 * std::abs(qx)	/ std::pow(hflow, (10 / 3))));



		// need to have these lines to stop too much water moving from
		// one cell to another - resulting in negative discharges
		// which causes a large instability to develop
		// - only in steep catchments really

		// FROUDE NUBER CHECKS
		if (qx > 0 && (qx / hflow) / std::sqrt(gravity * hflow) > froude_limit )
		  {
		    qx = hflow * (std::sqrt(gravity*hflow) * froude_limit );
		  }
		// If the discharge is now negative and above the froude_limit...
		if (qx < 0 && std::abs(qx / hflow) / std::sqrt(gravity * hflow) > froude_limit )
		  {
		    qx = 0 - (hflow * (std::sqrt(gravity * hflow) * froude_limit ));
		  }
		// DISCHARGE MAGNITUDE/TIMESTEP CHECKS
		// If the discharge is too high for this timestep, scale back...
		if (qx > 0 && (qx * local_time_factor / DX) > (water_depth / 4))
		  {
		    qx = ((water_depth * DX) / 5) / local_time_factor;
		  }
		// If the discharge is negative and too large, scale back...
		if (qx < 0 && std::abs(qx * local_time_factor / DX) > (WEST.water_depth / 4))
		  {
		    qx = 0 - ((WEST.water_depth * DX) / 5) / local_time_factor;
		  }





		

		// refactor: tries to update vel_dir belonging to neighbour sites - can't do this because neighborhood is passed as a CONST. If this is fundamental to libgeodecomp then make flow_route only modify its own vel_dir components, based on neighbouring qx qy (i.e. flip around the current logic of modifying neigbouring vel_dir based on local qx qy

		
		/*		// calc velocity now
				if (qx > 0)
				{
				vel_dir[7] = qx / hflow;
				}
				if (qx < 0)
				{
				WEST.vel_dir[3] = (0 - qx) / hflow;
				}

				}*/
		else // namely if hflow < hflow_threshold
		  {
		    qx = 0;
		    qxs = 0;
		  }




		//---------------------------
		//routing in the y direction
		//---------------------------
		if((water_depth > 0 || NORTH.water_depth > 0) && NORTH.elev > -9999)
		  // need to check water and not -9999 on elev?
		  {
		    double hflow = std::max(elev + water_depth, NORTH.elev + NORTH.water_depth) - std::max(elev, NORTH.elev);

		    if (hflow > hflow_threshold)
		      {
			double tempslope = ((NORTH.elev + NORTH.water_depth) - (elev + water_depth)) / DX; // Arno: original code says DX, not DY - correct?


			// Refactor for geodecomp
			//if (y == imax) tempslope = edgeslope;
			//if (x <= 2) tempslope = 0 - edgeslope;

			qy = ((qy - (gravity * hflow * local_time_factor * tempslope)) \
			      / (1 + gravity * hflow * local_time_factor * (mannings * mannings) \
				 * std::abs(qy) / std::pow(hflow, (10 / 3))));



			// need to have these lines to stop too much water moving from
			// one cell to another - resulting in negative discharges
			// which causes a large instability to develop
			// - only in steep catchments really

			// FROUDE NUBER CHECKS
			if (qy > 0 && (qy / hflow) / std::sqrt(gravity * hflow) > froude_limit )
			  {
			    qy = hflow * (std::sqrt(gravity*hflow) * froude_limit );
			  }
			// If the discharge is now negative and above the froude_limit...
			if (qy < 0 && std::abs(qy / hflow) / std::sqrt(gravity * hflow) > froude_limit )
			  {
			    qy = 0 - (hflow * (std::sqrt(gravity * hflow) * froude_limit ));
			  }
			// DISCHARGE MAGNITUDE/TIMESTEP CHECKS
			// If the discharge is too high for this timestep, scale back...
			if (qy > 0 && (qy * local_time_factor / DX) > (water_depth / 4)) // refactor: original code says DX, not DY - correct?
			  {
			    qy = ((water_depth * DX) / 5) / local_time_factor;
			  }
			// If the discharge is negative and too large, scale back...
			if (qy < 0 && std::abs(qy * local_time_factor / DX) > (NORTH.water_depth / 4)) // refactor: original code says DX, not DY, correct?
			  {
			    qy = 0 - ((NORTH.water_depth * DX) / 5) / local_time_factor;
			  }





			// refactor: tries to update vel_dir belonging to neighbour sites - can't do this because neighborhood is passed as a CONST. If this is fundamental to libgeodecomp then make flow_route only modify its own vel_dir components, based on neighbouring qx qy (i.e. flip around the current logic of modifying neigbouring vel_dir based on local qx qy

			/*		// calc velocity now
					if (qy > 0)
					{
					vel_dir[1] = qy / hflow;
					}
					if (qx < 0)
					{
					NORTH.vel_dir[5] = (0 - qy) / hflow;
					}

					}*/
			else // namely if hflow < hflow_threshold
			  {
			    qy = 0;
			    qys = 0;
			  }
		      }

		  }
	      }

    
	    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	    // DEPTH UPDATE
	    //
	    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	    double local_time_factor = time_factor; // refactor?

	    water_depth += local_time_factor * (EAST.qx - qx + SOUTH.qy - qy) / DX;
	    if (water_depth > 0)
	      {
		// line to remove any water depth on nodata cells (that shouldnt get there!)
		if (elev == -9999) water_depth = 0;
	      }

	  }
      }
  }
  
  
  

  
  void catchment_waterinputs() // refactor - incomplete (include runoffGrid for complex rainfall)
  {
    // refactor - incomplete
    waterinput = 0;
    catchment_water_input_and_hydrology();
  }
  
  
  
  
  
  void catchment_water_input_and_hydrology()
  {
    water_depth += 0.000001;  // refactor - dummy for now
    
    // topmodel_runoff(); (call some number of times depending on timescales)
    // calchydrograph();
    // zero_and_calc_drainage_area();
    // get_catchment_input_points();
  }
  
  
  
  
};











class CellInitializer : public SimpleInitializer<Cell>
{
public:
  CellInitializer() : SimpleInitializer<Cell>(Coord<2>(512,512), 100) // refactor - make dimensions variable
  {}
  
  /*   From libgeodecomp:
       "The initializer sets up the initial state of the grid. For this a
       Simulator will invoke Initializer::grid(). Keep in mind that grid()
       might be called multiple times and that for parallel runs each
       Initializer will be responsible just for a sub-cuboid of the whole
       grid." */
  

  void grid(GridBase<Cell, 2> *subdomain)
  {
    // Stuff in here should initialise all the individual cells within a subdomain
    for (int y=0; y<256; y++)
      {      
	for (int x=0; x<256; x++)
	  {
	    Coord<2> c(x, y);
	    subdomain->set(c, Cell());
	  }
      }
    
  }
};






void runSimulation()
{
  SerialSimulator<Cell> sim(new CellInitializer());
  sim.run();
}


