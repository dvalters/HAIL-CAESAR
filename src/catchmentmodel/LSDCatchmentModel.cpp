#include "libgeodecomp.h"
#include "catchmentmodel/LSDCatchmentModel.hpp"

//
// Using convention that the origin is at SW corner and North is positive in y direction
//



using namespace LibGeoDecomp;


#define STEPS 100
#define XDIM 10
#define YDIM 10
#define outputFrequency 1
#define edgeslope 0.005


enum CellType {INTERNAL, EDGE_WEST, EDGE_EAST, EDGE_NORTH, EDGE_SOUTH, CORNER_NW, CORNER_NE, CORNER_SW, CORNER_SE}; // should declare this elsewhere (header file?). Used in both both Cell and CellInitializer classes. 




class Cell
{
public:
  class API :
    public APITraits::HasStencil<Stencils::VonNeumann<2,1> >
  {};

  Cell(CellType celltype_in = INTERNAL,	\
       double elev_in = 0.0, \
       double water_depth_in = 0.0, \
       double qx_in = 0.0, \
       double qy_in = 0.0) \
    : celltype(celltype_in), elev(elev_in), water_depth(water_depth_in), qx(qx_in), qy(qy_in)
  {}
  
  CellType celltype;
  double elev, water_depth;
  double qx, qy;
  TNT::Array1D<double> vel_dir = TNT::Array1D<double> (9, 0.0); // refactor (redefinition of declaration in include/catchmentmodel/LSDCatchmentModel.hpp
  double waterinput = 0; // refactor (already declared in include/catchmentmodel/LSDCatchmentModel.hpp)
  unsigned rfnum = 1; // refactor (already declared in include/catchmentmodel/LSDCatchmentModel.hpp)


  // refactor - Should replace these defines with type alias declarations (= C++11 template typedef)
  // refactor - check that grid orientation makes sense (write test)
#define WEST neighborhood[Coord<2>(-1, 0)]   // refactor: have used this for array[x-1][y]
#define EAST neighborhood[Coord<2>( 1, 0)]  // refactor: have used this for array[x+1][y]
#define NORTH neighborhood[Coord<2>( 0, 1)] // refactor: have used this for array[x][y-1]
#define SOUTH neighborhood[Coord<2>( 0, -1)] // refactor: have used this for array[x][y+1]


  // DEFINING TEMPORARILY TO BE ABLE TO COMPILE - refactor to read from file
  // ***********************************************************************
  double hflow_threshold = 0.0;
  double DX = 1.0; 
  double DY = 1.0;
  double time_factor = 1.0;
  double local_time_factor = time_factor; // refactor?
  double gravity = 1.0; // should come from topotools/LSDRaster?
  double mannings = 1.0;
  double froude_limit = 1.0;
  // ***********************************************************************
  
  
  
  
  template<typename COORD_MAP>
  void update(const COORD_MAP& neighborhood, unsigned nanoStep)
  {
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // THE WATER ROUTING ALGORITHM: LISFLOOD-FP
    //
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    
    //---------------------------
    // routing in x direction
    //---------------------------

    double hflow;
    double tempslope;
    
    switch (celltype){
    case INTERNAL:
    case EDGE_NORTH:
    case EDGE_SOUTH:
      if( (water_depth > 0 || WEST.water_depth > 0) )
	{
	  hflow = std::max(elev + water_depth, WEST.elev + WEST.water_depth) - std::max(elev, WEST.elev);
	  if(hflow > hflow_threshold)
	    {
	      tempslope = ((WEST.elev + WEST.water_depth) - (elev + water_depth)) / DX;
	      update_qx(hflow, tempslope);
	      checks_qx(hflow);
	      check_negative_discharge_qx(neighborhood);
	    }
	  else
	    {
	      qx = 0.0;
	      //qxs = 0.0;
	    }
	}
      break;
    case EDGE_WEST:
    case CORNER_NW:
    case CORNER_SW:
      if (water_depth > 0) // Appropriate condition?
	{
	  hflow = water_depth;  // GUESSING: uses above equation but make assumption about WEST
	  if(hflow > hflow_threshold)
	    {
	      tempslope = -edgeslope;
	      update_qx(hflow, tempslope);
	      checks_qx(hflow);
	      // Need adapted negative discharge check here?
	    }
	  else
	    {
	      qx = 0.0;
	      //qxs = 0.0;
	    }
	}
      break;
    case EDGE_EAST:
    case CORNER_NE:
    case CORNER_SE:
      if (water_depth > 0 || WEST.water_depth > 0)
	{
	  hflow = std::max(elev + water_depth, WEST.elev + WEST.water_depth) - std::max(elev, WEST.elev);
	  if(hflow > hflow_threshold)
	    {
	      tempslope = edgeslope;
	      update_qx(hflow, tempslope);
	      checks_qx(hflow);
	      check_negative_discharge_qx(neighborhood);
	    }
	  else
	    {
	      qx = 0.0;
	      //qxs = 0.0;
	    }
	}
      break;
    default:
      break;
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
    
    

    //---------------------------
    //routing in the y direction
    //---------------------------

    switch (celltype){
    case INTERNAL:
    case EDGE_WEST:
    case EDGE_EAST:
      if((water_depth > 0 || NORTH.water_depth > 0))
	{
	  hflow = std::max(elev + water_depth, NORTH.elev + NORTH.water_depth) - std::max(elev, NORTH.elev);
	  if (hflow > hflow_threshold)
	    {
	      tempslope = ((NORTH.elev + NORTH.water_depth) - (elev + water_depth)) / DY;
	      update_qy(hflow, tempslope);
	      checks_qy(hflow);
	      check_negative_discharge_qy(neighborhood);
	    }
	  else
	    {
	      qy = 0.0;
	      //qys = 0.0;
	    }
	}
      break;
    case EDGE_NORTH:
    case CORNER_NW:
    case CORNER_NE:
      if(water_depth > 0) // Appropriate condition?
	{
	  hflow = water_depth; // GUESSING: uses above equation but makes assumption about NORTH
	  if (hflow > hflow_threshold)
	    {
	      tempslope = -edgeslope;
	      update_qy(hflow, tempslope);
	      checks_qy(hflow);
	      // Need adapted negative discharge check here?
	    }
	  else
	    {
	      qy = 0.0;
	      //qys = 0.0;
	    }
	}
      break;
    case EDGE_SOUTH:
    case CORNER_SW:
    case CORNER_SE:
      if((water_depth > 0 || NORTH.water_depth > 0))
	{
	  hflow = std::max(elev + water_depth, NORTH.elev + NORTH.water_depth) - std::max(elev, NORTH.elev);
	  if (hflow > hflow_threshold)
	    {
	      tempslope = edgeslope;
	      update_qy(hflow, tempslope);
	      checks_qy(hflow);
	      check_negative_discharge_qy(neighborhood);
	    }
	  else
	    {
	      qy = 0.0;
	      // qys = 0.0;
	    }
	}
      break;
    default:
      break;
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


    depth_update(neighborhood);
    
    
  }
  











  
  
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // DEPTH UPDATE
  //
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  template<typename COORD_MAP>
  void depth_update(const COORD_MAP& neighborhood)
  {
    
    double local_time_factor = time_factor; // refactor?
    
    switch (celltype){
    case INTERNAL:
    case EDGE_NORTH:
    case EDGE_WEST:
    case CORNER_NW:
      water_depth += local_time_factor * ( (EAST.qx - qx)/DX + (SOUTH.qy - qy)/DY ); // generalised to DX != DY
      break;
    case EDGE_EAST:
    case EDGE_SOUTH:
    case CORNER_SE:
    case CORNER_SW:
    case CORNER_NE:
      water_depth += 0.0; // Appropriate boundary condition?
    default:
      break;
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
  
  


  void update_qx(double hflow, double tempslope)
  {
    qx = ((qx - (gravity * hflow * local_time_factor * tempslope))	\
	  / (1 + gravity * hflow * local_time_factor * (mannings * mannings) \
	     * std::abs(qx) / std::pow(hflow, (10 / 3))));
  }



  void update_qy(double hflow, double tempslope)
  {
    qy = ((qy - (gravity * hflow * local_time_factor * tempslope))	\
	  / (1 + gravity * hflow * local_time_factor * (mannings * mannings) \
	     * std::abs(qy) / std::pow(hflow, (10 / 3))));
  }




  void checks_qx(double hflow)
  {
    // need to have these lines to stop too much water moving from
    // one cell to another - resulting in negative discharges
    // which causes a large instability to develop
    // - only in steep catchments really
  
    // FROUDE NUMBER CHECKS
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
  }



  void checks_qy(double hflow)
  {
    // need to have these lines to stop too much water moving from
    // one cell to another - resulting in negative discharges
    // which causes a large instability to develop
    // - only in steep catchments really

    // FROUDE NUMBER CHECKS
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
    if (qy > 0 && (qy * local_time_factor / DX) > (water_depth / 4)) 
      {
	qy = ((water_depth * DX) / 5) / local_time_factor;
      }
  }

  
  template<typename COORD_MAP>
    void check_negative_discharge_qx(const COORD_MAP& neighborhood)
  {
    // If the discharge is negative and too large, scale back...
    if (qx < 0 && std::abs(qx * local_time_factor / DX) > (WEST.water_depth / 4))
      {
	qx = 0 - ((WEST.water_depth * DX) / 5) / local_time_factor;
      }
  }
  
  template<typename COORD_MAP>
    void check_negative_discharge_qy(const COORD_MAP& neighborhood)
  {
    // If the discharge is negative and too large, scale back...
    if (qy < 0 && std::abs(qy * local_time_factor / DX) > (NORTH.water_depth / 4)) 
      {
	qy = 0 - ((NORTH.water_depth * DX) / 5) / local_time_factor;
      }
  }
  


  
};








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
      
    for (int x=0; x<XDIM; x++)
      {      
	for (int y=0; y<YDIM; y++)
	  {
	    if (y == 0)
	      {
		if (x == 0){ celltype = CORNER_SW; }
		else if (x == XDIM-1){ celltype = CORNER_SE; }
		else celltype = EDGE_SOUTH;
	      }
	    else if (y == YDIM-1)
	      {
		if (x == 0){ celltype = CORNER_NW; }
		else if (x == XDIM-1){ celltype = CORNER_NE; }
		else celltype = EDGE_NORTH;
	      }	      
	    else  // i.e. for  0 < y < YDIM
	      {
		if (x == 0){ celltype = EDGE_WEST; }
		else if (x == XDIM-1){ celltype = EDGE_EAST; }
		else celltype = INTERNAL;
	      }
	    
	    Coord<2> coordinate(x, y);

	    // Uniform zero elevation, initial water depth 1.0 in central cell
	    if (x == (int)(XDIM/2) && y == (int)(YDIM/2)) subdomain->set(coordinate, Cell(celltype, 0.0, 1.0, 0.0, 0.0)); // 
	    else subdomain->set(coordinate, Cell(celltype, 0.0, 0.0, 0.0, 0.0));
	    
	    // Constant ascending slope in x direction, initial water depth 1.0 in central cell
	    /*	    if (x == (int)(XDIM/2) && y == (int)(YDIM/2)) subdomain->set(coordinate, Cell(celltype, (double)x/(double)XDIM, 1.0, 0.0, 0.0)); // 
		    else subdomain->set(coordinate, Cell(celltype, (double)x/(double)XDIM, 0.0, 0.0, 0.0));*/
	  }
      }
  }
};






void runSimulation()
{
  SerialSimulator<Cell> sim(new CellInitializer());
  
  sim.addWriter(new PPMWriter<Cell>(&Cell::elev, 0.0, 1.0, "elevation", STEPS, Coord<2>(100,100)));
  sim.addWriter(new PPMWriter<Cell>(&Cell::water_depth, 0.0, 1.0, "water_depth", outputFrequency, Coord<2>(100,100)));
  /*sim.addWriter(new PPMWriter<Cell>(&Cell::qx, 0.0, 0.002, "qx", outputFrequency, Coord<2>(1,1)));
    sim.addWriter(new PPMWriter<Cell>(&Cell::qy, 0.0, 0.002, "qy", outputFrequency, Coord<2>(1,1)));
    sim.addWriter(new PPMWriter<Cell>(&Cell::x, 0, 9, "x", outputFrequency, Coord<2>(1,1)));
    sim.addWriter(new PPMWriter<Cell>(&Cell::y, 0, 9, "y", outputFrequency, Coord<2>(1,1)));*/

  sim.addWriter(new TracingWriter<Cell>(outputFrequency, STEPS));
  
  sim.run();
}


