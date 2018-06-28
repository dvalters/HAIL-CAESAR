#include <cmath>
#include "libgeodecomp.h"
#include "catchmentmodel/LSDCatchmentModel.hpp"

//
// Using convention that the origin is at NW corner and South is positive in y direction
//



using namespace LibGeoDecomp;


#define STEPS 300
#define XDIM 31
#define YDIM 31
#define outputFrequency 1
#define edgeslope 0.005

// Define possible cell types to implement boundary conditions
// Note: edges include neither corner cells nor corner+1 cells (where the latter are declared)
enum CellType {CORNER_NW, CORNER_NW_XPLUS1, CORNER_NE, EDGE_NORTH,	\
	       CORNER_NW_YPLUS1, CORNER_NW_INNER, CORNER_NE_YPLUS1, EDGE_NORTH_INNER, \
	       CORNER_SW, CORNER_SW_XPLUS1, CORNER_SE, EDGE_SOUTH,	\
	       EDGE_WEST, EDGE_WEST_INNER, EDGE_EAST,			\
	       INTERNAL,						\
	       NODATA							\
};
// should declare CellType enum  elsewhere (header file?). Used in both both Cell and CellInitializer classes. 




class Cell
{
public:
  class API :
    public APITraits::HasStencil<Stencils::VonNeumann<2,1> >
  {};

  Cell(CellType celltype_in = INTERNAL,		\
       double elev_in = 0.0,			\
       double water_depth_in = 0.0,		\
       double qx_in = 0.0,			\
       double qy_in = 0.0,			\
       bool persistent_rain_flag = false)					\
    : celltype(celltype_in), elev(elev_in), water_depth(water_depth_in), qx(qx_in), qy(qy_in), persistent_rain(persistent_rain_flag)
  {}
  
  CellType celltype;
  double elev, water_depth;
  double qx, qy;
  TNT::Array1D<double> vel_dir = TNT::Array1D<double> (9, 0.0); // refactor (redefinition of declaration in include/catchmentmodel/LSDCatchmentModel.hpp
  double waterinput = 0; // refactor (already declared in include/catchmentmodel/LSDCatchmentModel.hpp)
  unsigned rfnum = 1; // refactor (already declared in include/catchmentmodel/LSDCatchmentModel.hpp)
  
  
  // refactor - Should replace these defines with type alias declarations (= C++11 template typedef)
  // refactor - check that grid orientation makes sense (write test)
#define WEST neighborhood[Coord<2>(-1, 0)]   
#define EAST neighborhood[Coord<2>( 1, 0)]  
#define NORTH neighborhood[Coord<2>( 0, -1)] 
#define SOUTH neighborhood[Coord<2>( 0,  1)] 


  // DEFINING TEMPORARILY TO BE ABLE TO COMPILE - refactor to read from file
  // ***********************************************************************
  // default values taken from Boscastle_test_paramfile_March2018.params and LSDCatchmentModel.cpp in master branch
  double hflow_threshold = 0.00001;
  double DX = 1.0; 
  double DY = 1.0;
  double courant_number = 0.5;
  double maxdepth = 0.01;
  double time_factor = 1.0;
  double local_time_factor;
  double gravity = 9.8;
  double mannings = 0.04;
  double froude_limit = 0.8;
  bool persistent_rain;
  // ***********************************************************************
  

  
  
  
  
  
  template<typename COORD_MAP>
  void update(const COORD_MAP& neighborhood, unsigned nanoStep)
  {
    set_global_timefactor();
    set_local_timefactor();
    flow_route_x(neighborhood);
    flow_route_y(neighborhood);
    depth_update(neighborhood);
  } 






  
  
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // THE WATER ROUTING ALGORITHM: LISFLOOD-FP
  //
  //           X direction
  //
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  template<typename COORD_MAP>
  void flow_route_x(const COORD_MAP& neighborhood)
  {
    double hflow;
    double tempslope;
    double west_elev;
    double west_water_depth;
    
    switch (celltype){
    case INTERNAL:
    case EDGE_NORTH:
    case EDGE_NORTH_INNER:
    case EDGE_SOUTH:
      west_elev = WEST.elev;
      west_water_depth = WEST.water_depth;
      tempslope = ((west_elev + west_water_depth) - (elev + water_depth)) / DX;
      break;
    case EDGE_WEST:
    case CORNER_NW:
    case CORNER_NW_YPLUS1:
    case CORNER_SW:
      west_elev = -9999;
      west_water_depth = 0.0;
      tempslope = -edgeslope;
      break;
    case EDGE_WEST_INNER:
    case CORNER_NW_INNER:
    case CORNER_NW_XPLUS1:
    case CORNER_SW_XPLUS1:
      west_elev = WEST.elev;
      west_water_depth = WEST.water_depth;
      //tempslope = -edgeslope; // make second layer of cells away from western boundary still feel edgeslope
      tempslope = ((west_elev + west_water_depth) - (elev + water_depth)) / DX; // treat second layer of cells away from western boundary just like bulk
      break;      
    case EDGE_EAST:
    case CORNER_NE:
    case CORNER_NE_YPLUS1:
    case CORNER_SE:
      west_elev = WEST.elev;
      west_water_depth = WEST.water_depth;
      tempslope = edgeslope;
      break;
    default:
      std::cout << "\n\n WARNING: no x-direction flow route rule specified for cell type " << celltype << "\n\n";
      break;
    }

    
    if (water_depth > 0 || west_water_depth > 0)
      {
	hflow = std::max(elev + water_depth, west_elev + west_water_depth) - std::max(elev, west_elev);
	if (hflow > hflow_threshold)
	  {
	    update_qx(hflow, tempslope);
	    froude_check_x(hflow);
	    discharge_check_x();
	    discharge_check_x_negative(west_water_depth);
	  }
	else
	  {
	    qx = 0.0;
	    // qxs = 0.0;
	  }
      }
    

    
    
    // calc velocity now
    //if (qx > 0)
    //  {
    //	vel_dir[7] = qx / hflow;
    //  }
    // refactor: old code tries to update vel_dir belonging to neighbour sites - can't do this because neighborhood is passed as a CONST. Should make flow_route only modify its own vel_dir components, based on neighbouring qx qy (i.e. flip around the current logic of modifying neigbouring vel_dir based on local qx qy)
    // But need all cells to record their hflow (or precompute and store -qx/hflow)
    /*	if (qx < 0)
	{
	WEST.vel_dir[3] = (0 - qx) / hflow;
	}
	}*/
  }



  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // THE WATER ROUTING ALGORITHM: LISFLOOD-FP
  //
  //           Y direction
  //
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  template<typename COORD_MAP>
  void flow_route_y(const COORD_MAP& neighborhood)
  {
    double hflow;
    double tempslope;
    double north_elev;
    double north_water_depth;
    
    switch (celltype){
    case INTERNAL:
    case EDGE_WEST:
    case EDGE_WEST_INNER:
    case EDGE_EAST:
      north_elev = NORTH.elev;
      north_water_depth = NORTH.water_depth;
      tempslope = ((north_elev + north_water_depth) - (elev + water_depth)) / DY;
    break;
    case EDGE_NORTH:
    case CORNER_NW:
    case CORNER_NW_XPLUS1:
    case CORNER_NE:
      north_elev = -9999;
      north_water_depth = 0.0;
      tempslope = -edgeslope;
      break;
    case EDGE_NORTH_INNER:
    case CORNER_NW_YPLUS1:
    case CORNER_NW_INNER:
    case CORNER_NE_YPLUS1:
      north_elev = NORTH.elev;
      north_water_depth = NORTH.water_depth;
      //tempslope = -edgeslope; // make second layer of cells away from northern boundary still feel edgeslope
      tempslope = ((north_elev + north_water_depth) - (elev + water_depth)) / DY; // treat second layer of cells away from northern boundary just like bulk
      break;
    case EDGE_SOUTH:
    case CORNER_SW:
    case CORNER_SW_XPLUS1:
    case CORNER_SE:
      north_elev = NORTH.elev;
      north_water_depth = NORTH.water_depth;
      tempslope = edgeslope;
      break;
    default:
      std::cout << "\n\n WARNING: no y-direction flow route rule specified for cell type " << static_cast<int>(celltype) << "\n\n";
      break;
    }

  
    if (water_depth > 0 || north_water_depth > 0)
      {
	hflow = std::max(elev + water_depth, north_elev + north_water_depth) - std::max(elev, north_elev);
	if (hflow > hflow_threshold)
	  {
	    update_qy(hflow, tempslope);
	    froude_check_y(hflow);
	    discharge_check_y();
	    discharge_check_y_negative(north_water_depth);
	  }
	else
	  {
	    qy = 0.0;
	    // qys = 0.0;
	  }
      }

    

    
    // calc velocity now
    //if (qy > 0)
    //  {
    //vel_dir[1] = qy / hflow;
    // }
    //refactor: old code tries to update vel_dir belonging to neighbour sites - can't do this because neighborhood is passed as a CONST. Should make flow_route only modify its own vel_dir components, based on neighbouring qx qy (i.e. flip around the current logic of modifying neigbouring vel_dir based on local qx qy)
    // But need all cells to record their hflow (or precompute and store -qx/hflow)

    /*		if (qx < 0)
		{
		NORTH.vel_dir[5] = (0 - qy) / hflow;
		}
		
		}*/
  }
  



  

  
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // DEPTH UPDATE
  //
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  template<typename COORD_MAP>
  void depth_update(const COORD_MAP& neighborhood)
  {
    double east_qx;
    double south_qy;
    
    switch (celltype){
    case INTERNAL:
    case EDGE_NORTH:
    case EDGE_NORTH_INNER:
    case EDGE_WEST:
    case EDGE_WEST_INNER:
    case CORNER_NW:
    case CORNER_NW_INNER:
    case CORNER_NW_YPLUS1:
    case CORNER_NW_XPLUS1:
      east_qx = EAST.qx;
      south_qy = SOUTH.qy;
      update_water_depth(east_qx, south_qy);
      break;
    case EDGE_EAST:
    case CORNER_NE:
    case CORNER_NE_YPLUS1:
      east_qx = 0.0;
      south_qy = SOUTH.qy;
      update_water_depth(east_qx, south_qy);
      break;
    case EDGE_SOUTH:
    case CORNER_SW:
    case CORNER_SW_XPLUS1:
      east_qx = EAST.qx;
      south_qy = 0.0;
      update_water_depth(east_qx, south_qy);
      break;
    case CORNER_SE:
      east_qx = 0.0;
      south_qy = 0.0;
      update_water_depth(east_qx, south_qy);
      break;
    case NODATA:
      water_depth = 0.0;
      break;
    default:
      std::cout << "\n\n WARNING: no depth update rule specified for cell type " << CellType(int(celltype))<< "\n\n";
      break;
    }


	if(persistent_rain) water_depth = 1.0;
      
  }
  
  
  
  void update_water_depth(double east_qx, double south_qy)
  {
    water_depth += local_time_factor * ( (east_qx - qx)/DX + (south_qy - qy)/DY );
  }
  
  
  
  void catchment_waterinputs() // refactor - incomplete (include runoffGrid for complex rainfall)
  {
    // refactor - incomplete
    waterinput = 0;
    catchment_water_input_and_hydrology();
  }
  
  
  void catchment_water_input_and_hydrology()
  {
    // topmodel_runoff(); (call some number of times depending on timescales)
    // calchydrograph();
    // zero_and_calc_drainage_area();
    // get_catchment_input_points();
  }
  
  
  void update_qx(double hflow, double tempslope)
  {
    qx = ((qx - (gravity * hflow * local_time_factor * tempslope))	\
	  / (1 + gravity * hflow * local_time_factor * (mannings * mannings) \
	     * std::abs(qx) / std::pow(hflow, (10.0 / 3.0))));
  }
  
  
  void update_qy(double hflow, double tempslope)
  {
    qy = ((qy - (gravity * hflow * local_time_factor * tempslope))	\
	  / (1 + gravity * hflow * local_time_factor * (mannings * mannings) \
	     * std::abs(qy) / std::pow(hflow, (10.0 / 3.0))));
  }
  
  
  void froude_check_x(double hflow)
  {
    if ((std::abs(qx / hflow) / std::sqrt(gravity * hflow)) > froude_limit)
      {
	qx = std::copysign(hflow * (std::sqrt(gravity*hflow) * froude_limit), qx);
      }
  }
  
  void froude_check_y(double hflow)
  {
    // need to have these lines to stop too much water moving from
    // one cell to another - resulting in negative discharges
    // which causes a large instability to develop
    // - only in steep catchments really
    
    if ((std::abs(qy / hflow) / std::sqrt(gravity * hflow)) > froude_limit)
      {
	qy = std::copysign(hflow * (std::sqrt(gravity*hflow) * froude_limit), qy);
      }
  }
  
  
  // DISCHARGE MAGNITUDE/TIMESTEP CHECKS
  // If the discharge is too high for this timestep, scale back...
  void discharge_check_x()
  {
    if (qx > 0 && (qx * local_time_factor / DX) > (water_depth / 4))
      {
	qx = ((water_depth * DX) / 5) / local_time_factor;
      }
    }

  
  // DISCHARGE MAGNITUDE/TIMESTEP CHECKS
  // If the discharge is too high for this timestep, scale back...
  void discharge_check_y()
  {
    if (qy > 0 && (qy * local_time_factor / DX) > (water_depth / 4)) 
      {
	qy = ((water_depth * DX) / 5) / local_time_factor;
      }
  }

  
  void discharge_check_x_negative(double west_water_depth)
  {
    // If the discharge is negative and too large, scale back...
    if (qx < 0 && std::abs(qx * local_time_factor / DX) > (west_water_depth / 4))
      {
	qx = 0 - ((west_water_depth * DX) / 5) / local_time_factor;
      }
  }
  
  void discharge_check_y_negative(double north_water_depth)
  {
    // If the discharge is negative and too large, scale back...
    if (qy < 0 && std::abs(qy * local_time_factor / DX) > (north_water_depth / 4)) 
      {
	qy = 0 - ((north_water_depth * DX) / 5) / local_time_factor;
      }
  }
  
  
  void set_global_timefactor()
  {
    if (maxdepth <= 0.1)
      {
	maxdepth = 0.1;
      }
    if (time_factor < (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
      {
	time_factor = (courant_number * (DX / std::sqrt(gravity * (maxdepth))));
      }
    /*    if (input_output_difference > in_out_difference_allowed && time_factor > (courant_number * (DX / std::sqrt(gravity * (maxdepth)))))
      {
	time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
	}*/
  }
  
  
  void set_local_timefactor()
  {
    local_time_factor = time_factor;
    if (local_time_factor > (courant_number * (DX  / std::sqrt(gravity * (maxdepth)))))
      {
	local_time_factor = courant_number * (DX / std::sqrt(gravity * (maxdepth)));
      }
  }

  

  
  /*  void set_inputoutput_diff()
  {
    input_output_difference = std::abs(waterinput - waterOut);
    }*/


  

  
  
}; // end of Cell class








class CellInitializer : public SimpleInitializer<Cell>
{
public:
  using SimpleInitializer<Cell>::gridDimensions;  // gridDimensions = dimensions of subgrid (in parallel case), not of overall grid
  
  CellInitializer() : SimpleInitializer<Cell>(Coord<2>(XDIM,YDIM), STEPS) // refactor - make dimensions variable. Second argument is the number of steps to run for
  {}
  
  /*   From libgeodecomp:
       "The initializer sets up the initial state of the grid. For this a
       Simulator will invoke Initializer::grid(). Keep in mind that grid()
       might be called multiple times and that for parallel runs each
       Initializer will be responsible just for a sub-cuboid of the whole
       grid." */
  
  
  void grid(GridBase<Cell, 2> *subdomain)
  {
    CellType celltype;
    
    for (int x=0; x<=XDIM-1; x++)
      {      
	for (int y=0; y<=YDIM-1; y++)
	  {
	    if (y == 0)
	      {
		if (x == 0) celltype = CORNER_NW;
		else if (x == 1) celltype = CORNER_NW_XPLUS1;
		else if (x == XDIM-1) celltype = CORNER_NE;
		else celltype = EDGE_NORTH; 
	      }
	    else if (y == 1)
	      {
		if (x == 0) celltype = CORNER_NW_YPLUS1;
		else if (x == 1) celltype = CORNER_NW_INNER;
		else if (x == XDIM-1) celltype = CORNER_NE_YPLUS1;
		else celltype = EDGE_NORTH_INNER;
	      }
	    else if (y == YDIM-1)
	      {
		if (x == 0) celltype = CORNER_SW; 
		else if (x == 1) celltype = CORNER_SW_XPLUS1;
		else if (x == XDIM-1) celltype = CORNER_SE; 
		else celltype = EDGE_SOUTH; 
	      }	      
	    else  // i.e. for  2 <= y <= YDIM-2
	      {
		if (x == 0) celltype = EDGE_WEST; 
		else if (x == 1) celltype = EDGE_WEST_INNER; 
		else if (x == XDIM-1) celltype = EDGE_EAST; 
		else celltype = INTERNAL;
	      }
	    
	    Coord<2> coordinate(x, y);

	    // DEBUG TEST INITIALISATIONS

	    // Uniform zero elevation, uniform zero water depth
	    //subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0));
	    
	    // Uniform zero elevation, uniform small water depth
	    //subdomain->set(coordinate, Cell(celltype, 0.0, 0.1, 0.0, 0.0));

	    // Uniform zero elevation, initial water depth 1.0 in NW corner cell and zero elsewhere
	    //if (x==0 && y==0) subdomain->set(coordinate, Cell(celltype, 0.0, 1.0, 0.0, 0.0));
	    //else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0));
	    
	    // Uniform zero elevation, water depth always at 1.0 in NW corner cell and zero elsewhere
	    /*if (x==0 && y==0) subdomain->set(coordinate, Cell(persistent, 0.0, 1.0, 0.0, 0.0)); 
	    else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0));*/

	    // Uniform zero elevation, water depth always at 1.0 in NE corner cell and zero elsewhere
	    //if (x==XDIM-1 && y==0) subdomain->set(coordinate, Cell(persistent, 0.0, 1.0, 0.0, 0.0)); 
	    //else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0));
	    
	    // Uniform zero elevation, water depth always at 1.0 in SW corner cell and zero elsewhere
	    /*	      if (x==0 && y==YDIM-1) subdomain->set(coordinate, Cell(celltype, 0.0, 1.0, 0.0, 0.0, false)); 
	      else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0, false));*/
	    
	    // Uniform zero elevation, water depth always at 1.0 in SE corner cell and zero elsewhere
	    /*CellType persistent = DEBUG_PERSISTENT_RAIN;
	    if (x==XDIM-1 && y==YDIM-1)	subdomain->set(coordinate, Cell(persistent, 0.0, 1.0, 0.0, 0.0, false));
	    else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0, false));*/
	    
	    // Uniform zero elevation, initial water depth 1.0 in central cell and zero elsewhere
	    //if (x == (int)(XDIM/2) && y == (int)(YDIM/2)) subdomain->set(coordinate, Cell(celltype, 0.0, 1.0, 0.0, 0.0, false)); 
	    //else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0));
	    
	    // Uniform zero elevation, water depth always at 1.0 in central cell, initial water depth zero elsewhere
	    if (x == (int)(XDIM/2) && y == (int)(YDIM/2)) subdomain->set(coordinate, Cell(celltype, 0.0, 1.0, 0.0, 0.0, true)); 
	    else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0, false));
	    
	    // Uniform zero elevation, water depth always at 1.0 in central cell and cells near all four corners, initial water depth zero elsewhere
	    //if (x == (int)(XDIM/2) && y == (int)(YDIM/2)) subdomain->set(coordinate, Cell(celltype, 0.0, 1.0, 0.0, 0.0, true)); // Central cell
	    //else if ( (x==5 || x==XDIM-6) && (y==5 || y==YDIM-6)) subdomain->set(coordinate, Cell(celltype, 0.0, 1.0, 0.0, 0.0, true)); // cells near four corners }
	    //else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0, false));
	    
	    // Constant ascending elevation in x direction, water depth always 1.0 in central cell and initial water depth zero elsewhere
	    //if (x == (int)(XDIM/2) && y == (int)(YDIM/2)) subdomain->set(coordinate, Cell(celltype, (double)x/(double)XDIM, 1.0, 0.0, 0.0, true)); 
	    //else subdomain->set(coordinate, Cell(celltype, (double)x/(double)XDIM, 0.0, 0.0, 0.0, false));

	    // Constant descending elevation in x direction, water depth always 1.0 for band at x=0 and initial water depth zero elsewhere
	    //if (x == 0) subdomain->set(coordinate, Cell(celltype, 1-(double)x/(double)XDIM, 1.0, 0.0, 0.0, true)); 
	    //else subdomain->set(coordinate, Cell(celltype, 1-(double)x/(double)XDIM, 0.0, 0.0, 0.0, false));
	  }
      }
  }
};






void runSimulation()
{
  SerialSimulator<Cell> sim(new CellInitializer());
  
  sim.addWriter(new PPMWriter<Cell>(&Cell::elev, 0.0, 1.0, "elevation", STEPS, Coord<2>(20,20)));
  sim.addWriter(new PPMWriter<Cell>(&Cell::water_depth, 0.0, 1.0, "water_depth", outputFrequency, Coord<2>(20,20)));
  //  sim.addWriter(new PPMWriter<Cell>(&Cell::qx, 0.0, 1.0, "qx", outputFrequency, Coord<2>(100,100)));
  //  sim.addWriter(new PPMWriter<Cell>(&Cell::qy, 0.0, 1.0, "qy", outputFrequency, Coord<2>(100,100)));
  
  sim.addWriter(new TracingWriter<Cell>(outputFrequency, STEPS));
  
  sim.run();
}


