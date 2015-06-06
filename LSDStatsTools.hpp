//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDStatsTools
// Land Surface Dynamics StatsTools
//
// A collection of statistical routines for use with the University
//  of Edinburgh Land Surface Dynamics group topographic toolbox
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
//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------

#include <vector>
#include <map>
#include "TNT/tnt.h"
using namespace std;
using namespace TNT;


#ifndef StatsTools_H
#define StatsTools_H

// tools for reversing arrays
Array2D<double> reverse_array_rows(Array2D<double>& data);
Array2D<double> reverse_array_cols(Array2D<double>& data);
Array2D<float> reverse_array_rows(Array2D<float>& data);
Array2D<float> reverse_array_cols(Array2D<float>& data);
Array2D<int> reverse_array_rows(Array2D<int>& data);
Array2D<int> reverse_array_cols(Array2D<int>& data);

// computes linear regression
// replaces data in residuals with residuals and returns a 4 element vector, which has slope, intercept, r^2 and
// the Durbin-Watson test statistic which looks for autocorrelation of the residuals
vector<float> simple_linear_regression(vector<float>& x_data, vector<float>& y_data, vector<float>& residuals);
float get_mean(vector<float>& y_data);
float get_mean_ignore_ndv(Array2D<float>& data, float ndv);
float get_SST(vector<float>& y_data, float mean);
float get_variance_ignore_ndv(Array2D<float>& data, float ndv, float mean); 
float get_range_ignore_ndv(Array2D<float>& data, float ndv);
float get_durbin_watson_statistic(vector<float> residuals);
float get_standard_deviation(vector<float>& y_data, float mean);
float get_standard_error(vector<float>& y_data, float standard_deviation);
vector<float> get_common_statistics(vector<float>& y_data);
float get_percentile(vector<float>& data, float percentile);

// sorts data; produces quartile-quantile comparison against standard normal variate, returning
// an (evenly spaced) sorted subsample of N_points, their corresponding normal variate and the
// reference value  from the standard normal distribution.  Test for departures from normality
// within the given distribution.  
void generate_q_q_plot(vector<float>& data, vector<float>& values, vector<float>& standard_normal_variates, vector<float>& mn_values, int N_points);

// declaration of the quantile_quantile analysis
void quantile_quantile_analysis(vector<float>& data, vector<float>& values, vector<float>& standard_normal_variates, vector<float>& mn_values, int N_points);

// calculates least squares linear regression for two datasets, returning
// gradient and intercept of regression line, alongside the R-squared value. 
// DTM 07/10/2014
void least_squares_linear_regression(vector<float> x_data, vector<float> y_data, float& intercept, float& gradient, float& R_squared);
// take a slice of a vector
// DTM 30/10/2014
vector<float> slice_vector(vector<float>::iterator first,vector<float>::iterator last);


// interpolation
double interp1D_ordered(vector<double>& x, vector<double>& y, double x_interp_loc);
vector<double> interp1D_ordered(vector<double>& x, vector<double>& y, vector<double> x_interp_loc);
float interp1D_ordered(vector<float>& x, vector<float>& y, float x_interp_loc);
vector<float> interp1D_ordered(vector<float>& x, vector<float>& y, vector<float> x_interp_loc);
vector<double> interp1D_spline_ordered(vector<double>& x_data, vector<double>& y_data, 
                                       vector<double>& x_interp_locs);
float interp1D_unordered(vector<float> x, vector<float> y, float x_interp_loc);
vector<float> interp1D_unordered(vector<float> x, vector<float> y, vector<float>& x_interp_loc);
double interp1D_unordered(vector<double> x, vector<double> y, double x_interp_loc);
vector<double> interp1D_unordered(vector<double> x, vector<double> y, vector<double>& x_interp_loc);
vector<double> interp1D_spline_unordered(vector<double> x_data, vector<double> y_data, 
                                       vector<double>& x_interp_locs);
double interp2D_bilinear(vector<double>& x_locs, vector<double>& y_locs, Array2D<double> data, 
                        double x_interp, double y_interp);    
float interp2D_bilinear(vector<float>& x_locs, vector<float>& y_locs, Array2D<float> data, 
                        float x_interp, float y_interp);                                                           
                                       
// Generate spline curves from X and Y vectors of floats
Array2D<float> CalculateCubicSplines(vector<float> X, vector<float> Y);
void PlotCubicSplines(vector<float> X, vector<float> Y, int SplineResolution, vector<float>& Spline_X, vector<float>& Spline_Y);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// simple cubic spline interpolation library without external
// dependencies
//
// ---------------------------------------------------------------------
// Copyright (C) 2011, 2014 Tino Kluge (ttk448 at gmail.com)
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
// ---------------------------------------------------------------------
//
//
// SEE: http://kluge.in-chemnitz.de/opensource/spline/
//
// band matrix solver. This is for the spline
class band_matrix
{
  private:
     std::vector< std::vector<double> > m_upper;  // upper band
     std::vector< std::vector<double> > m_lower;  // lower band
  public:
     band_matrix() {};                             // constructor
     band_matrix(int dim, int n_u, int n_l);       // constructor
     ~band_matrix() {};                            // destructor
     void resize(int dim, int n_u, int n_l);      // init with dim,n_u,n_l
     int dim() const;                             // matrix dimension
     int num_upper() const
     {
       return m_upper.size()-1;
     }
     int num_lower() const
     {
       return m_lower.size()-1;
     }
     // access operator
     double & operator () (int i, int j);            // write
     double   operator () (int i, int j) const;      // read
     // we can store an additional diogonal (in m_lower)
     double& saved_diag(int i);
     double  saved_diag(int i) const;
     void lu_decompose();
     std::vector<double> r_solve(const std::vector<double>& b) const;
     std::vector<double> l_solve(const std::vector<double>& b) const;
     std::vector<double> lu_solve(const std::vector<double>& b,
                              bool is_lu_decomposed=false);
};

// spline interpolation
// -----------------------
//
// USAGE:
//
// spline s;
// s.set_points(X,Y);    // currently it is required that X is already sorted
// double x=1.5;
// cout << "spline at " << x << " is: " << s(x) <<endl;
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class spline 
{
  private:
   std::vector<double> m_x,m_y;           // x,y coordinates of points
   // interpolation parameters
   // f(x) = a*(x-x_i)^3 + b*(x-x_i)^2 + c*(x-x_i) + y_i
   std::vector<double> m_a,m_b,m_c,m_d;
  public:
   void set_points(const std::vector<double>& x,
                   const std::vector<double>& y, bool cubic_spline=true);
   double operator() (double x) const;
};
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// calculate the imaginary error function using trapezoid rule integration
double erfi(double tau);

// these look for linear segments within a data series.
void populate_segment_matrix(int start_node, int end_node, float no_data_value,
								vector<float>& all_x_data, vector<float>& all_y_data, int maximum_segment_length,
								float sigma, Array2D<float>& like_array, Array2D<float>& m_array,
								Array2D<float>& b_array, Array2D<float>& rsquared_array,
								Array2D<float>& DW_array);
void calculate_segment_matrices(vector<float>& all_x_data, vector<float>& all_y_data, int maximum_segment_length,
								float sigma, Array2D<float>& like_array, Array2D<float>& m_array,
								Array2D<float>& b_array, Array2D<float>& rsquared_array,
								Array2D<float>& DW_array);
void find_max_like_of_segments(int minimum_segment_length, Array2D<float>& like_array,
								vector<float>& max_MLE, vector< vector<int> >& segments_for_each_n_segments);
void find_max_AIC_of_segments(int minimum_segment_length, vector<float>& all_x_data, vector<float>& all_y_data,
								Array2D<float>& like_array,
								vector<float>& max_MLE, vector<float>& AIC_of_segments,
								vector<float>& AICc_of_segments, vector< vector<int> >& segments_for_each_n_segments);
void calculate_AIC_of_segments_with_normalized_sigma(float sigma,
								vector<float>& one_sigma_max_MLE, vector<float>& all_x_data,
								vector<float>& AIC_of_segments,vector<float>& AICc_of_segments);

// the below function is the main driver for the segment fitting code.
void best_fit_driver_AIC_for_linear_segments(int minimum_segment_length, float sigma,
											vector<float> all_x_data, vector<float> all_y_data,
											vector<float>& max_MLE);

// this gets the number of segments for several different values of sigma bassed in the vector sigma_values
void get_n_segments_for_various_sigma(vector<float> sigma_values, vector<float> one_sig_max_MLE,
									  vector<float>& all_x_data,
								      vector<int>& best_fit_AIC, vector<int>& best_fit_AICc,
								      vector< vector<float> >& AIC_for_each_n_segments,
								      vector< vector<float> >& AICc_for_each_n_segments);

// this prints full AIC and AICc information to screen
void print_AIC_and_AICc_to_screen(vector<float> sigma_values, vector< vector<int> > segments_for_each_n_segments,
								      vector<int> best_fit_AIC, vector<int> best_fit_AICc,
								      vector< vector<float> > AIC_for_each_n_segments,
								      vector< vector<float> > AICc_for_each_n_segments);

// this prints information about the most likeley segments to screen
void print_to_screen_most_likeley_segment_lengths( vector< vector<int> > segments_for_each_n_segments,
										vector<float> MLE_for_segments);

// this returns the m, b, r2 and DW stats of each segment
void get_properties_of_best_fit_segments(int bestfit_segments_node, vector< vector<int> >& segments_for_each_n_segments,
										 vector<float>& m_values, Array2D<float>& m_array,
										 vector<float>& b_values, Array2D<float>& b_array,
										 vector<float>& r2_values, Array2D<float>& rsquared_array,
										 vector<float>& DW_values, Array2D<float>& DW_array);

// these functions manipulate likelihood matrices and vectors for use with the segment tool
Array2D<float> normalize_like_matrix_to_sigma_one(float sigma, Array2D<float>& like_array);
vector<float> normalize_like_vector_to_sigma_one(float sigma, vector<float> like_vector);
Array2D<float> change_normalized_like_matrix_to_new_sigma(float sigma, Array2D<float>& sig1_like_array);
vector<float> change_normalized_like_vector_to_new_sigma(float sigma, vector<float> sig1_like_vector);

// this uses a moving window to find segments and is incomplete
void find_linear_segments(vector<float>& all_x_data, vector<float>& all_y_data, int segment_length);


// functions for partitioning and permutation (to be used with linear segment finding
int partitions_min( int x, int y);
void partition_print(int t, vector<int>& p);
void partitions_with_minimum_length(int n, int k, int t, int min_length, vector<int>& p);
void partitions_with_minimum_length(int n, int k, int t, int min_length, vector<int>& p,
								vector< vector < vector<int> > >& partitions);
void integer_partition(int n, int k, int t, vector<int>& p);
void partition_driver_to_screen(int n, int minimum_length);
vector< vector < vector<int> > > partition_driver_to_vecvecvec(int k, int minimum_length);
void partition_assign(int t, vector<int>& p, vector< vector < vector<int> > >& partitions);
void partition_vecvecvec_print(vector< vector < vector<int> > >& partitions);
void partition_vecvecvec_print_with_permutation(vector< vector < vector<int> > >& partitions);
void permute_partitioned_integer_vector(vector<int> permute_vector);

// this generates random segments for use in testing the segment finding algorithm
void generate_random_segments(float sigma, int minimum_n_nodes, int mean_segment_length, int segment_range,
							  float dx, float offset_range, float m_range,
							 vector<float>& x_data, vector<float>& y_data,
							 vector<int>& segment_length, vector<float>& slope, vector<float>& intercept);


// maxiumum likihood estimators
float calculate_MLE(vector<float>& measured, vector<float>& modelled, vector<float>& sigma);
float calculate_MLE(vector<float>& measured, vector<float>& modelled, float sigma);
float calculate_MLE_from_residuals(vector<float>& residuals, float sigma);

// a random number generator
float ran3( long *idum );
// Randomly sample from a vector without replacement DTM 21/04/2014
vector<float> sample_without_replacement(vector<float> population_vector, int N);
vector<int> sample_without_replacement(vector<int> population_vector, int N);
// conversion from numbers to strings
string itoa(int num);
string dtoa(float num);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function returning a Gaussian random number
// DAV 16/10/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
double Gauss_rand(int Nrand, double GaussAdd, double GaussFac);


// Log binning module
// two overloaded functions:
//    -> for data stored in a 2D array (e.g. slope-area)
void log_bin_data(Array2D<float>& InputArrayX, Array2D<float>& InputArrayY, float log_bin_width, vector<float>&  MeanX_output, vector<float>& MeanY_output,
                      vector<float>& midpoints_output, vector<float>& StandardDeviationX_output, vector<float>& StandardDeviationY_output,
                      vector<float>& StandardErrorX_output, vector<float>& StandardErrorY_output, vector<int>& num_observations, float NoDataValue);

//    -> for data stored in a 1D vector (e.g. for spectral analysis)
void log_bin_data(vector<float>& InputVectorX, vector<float>& InputVectorY, float log_bin_width,
                  vector<float>&  MeanX_output, vector<float>& MeanY_output,
                      vector<float>& midpoints_output, vector<float>&  StandardDeviationX_output,
                      vector<float>&  StandardDeviationY_output, int NoDataValue);

// Regular binning algoritm for data stored in a 1D vector
void bin_data(vector<float>& InputVectorX, vector<float>& InputVectorY, float bin_width,
                  vector<float>&  MeanX_output, vector<float>& MeanY_output,
                      vector<float>& midpoints_output, vector<float>& MedianY_output,
                      vector<float>&  StandardDeviationX_output, vector<float>&  StandardDeviationY_output,
                      vector<float>& StandardErrorX_output, vector<float>& StandardErrorY_output, 
                      vector<int>& number_observations_output, float& bin_lower_limit, float NoDataValue);

//look for empty bins output from the log binning function and removes them to avoid 
//plotting several empty bins at 0,0 in some cases. SWDG 6/11/13
void RemoveSmallBins(vector<float>&  MeanX_output, vector<float>& MeanY_output,
                      vector<float>& midpoints_output, vector<float>& StandardDeviationX_output, vector<float>& StandardDeviationY_output,
                      vector<float>& StandardErrorX_output, vector<float>& StandardErrorY_output, vector<int>& number_observations, float bin_threshold);

// Load in a vector of data and convert into a histogram with a specified bin width
// that is printed to file containing:
//    Midpoint LowerLim UpperLim Count ProbabilityDensity                      
void print_histogram(vector<float> input_values, float bin_width, string filename);
// improved histogram functions
void calculate_histogram(vector<float> input_values, float bin_width, vector<float>& Midpoints, vector<float>& LLims, vector<float>& ULims, vector<int>& Count, vector<float>& ProbabilityDensity);
void calculate_histogram_fixed_limits(vector<float> input_values, float bin_width, float lower_limit, float upper_limit, vector<float>& Midpoints, vector<float>& LLims, vector<float>& ULims, vector<int>& Count, vector<float>& ProbabilityDensity);


// This is a much simpler version of the binning software.  It takes two vectors, and
// sorts the values held within the first vector into bins according to their respective
// values in the second vector.  The output is a vector<vector> with the binned dataset.
// and a vector of bin midpoints.  These can then be analysed ahd plotted as desired.
// DTM 14/04/2014
void bin_data(vector<float>& vector1, vector<float>& vector2, float min, float max, float bin_width, vector<float>& mid_points, vector< vector<float> >& binned_data);
// log_bin_data
// This is a similar version for log-binning
// DTM 30/10/2014
void log_bin_data(vector<float>& vector1, vector<float>& vector2, float log_bin_width, vector<float>& bin_mid_points, vector<float>& bin_vector1_mean, vector<float>& bin_vector2_mean, vector< vector<float> >& binned_data, const float NoDataValue = -9999);

// tools for sorting
template<class T> struct index_cmp;
void matlab_double_sort(vector<double>& unsorted, vector<double>& sorted, vector<size_t>& index_map);
void matlab_double_reorder(std::vector<double> & unordered, std::vector<size_t> const & index_map, std::vector<double> & ordered);
void matlab_float_sort(vector<float>& unsorted, vector<float>& sorted, vector<size_t>& index_map);
void matlab_float_reorder(std::vector<float> & unordered, std::vector<size_t> const & index_map, std::vector<float> & ordered);
void matlab_float_sort_descending(vector<float>& unsorted, vector<float>& sorted, vector<size_t>& index_map);
void matlab_int_sort(vector<int>& unsorted, vector<int>& sorted, vector<size_t>& index_map); // added 27/11/13 SWDG
void matlab_int_reorder(std::vector<int> & unordered, std::vector<size_t> const & index_map, std::vector<int> & ordered);



//Get vector of unique values in an input array of ints
vector<int> Unique(Array2D<int> InputArray, int NoDataValue);

//Get vector of unique values in an input array of floats
vector<float> Unique(Array2D<float> InputArray, int NoDataValue);

// Generate vector of evenly spaced numbers between two points
vector<float> linspace(float min, float max, int n);

// convert degree bearing from north to radians from east
float BearingToRad(float Bearing);

// conversion from degrees to radians
float rad(float degree);
double rad(double degree);

// conversion from radians to degrees
float deg(float radians);
double deg(double radians);

//Method to generate Statistical distribution. - DTM
void get_distribution_stats(vector<float>& y_data, float& mean, float& median, float& UpperQuartile, float& LowerQuartile, float& MaxValue);

// Method to calculate the quadratic mean. - DTM
double get_QuadraticMean(vector<double> input_values, double bin_width);

// basic parser for parameter files   JAJ  08/01/2014
// There may be a better place to put this, but I can't think where 
void parse_line(ifstream &infile, string &parameter, string &value); 

// Method to get the maximum value in a 2D array - SWDG 12/6/14
float Get_Maximum(Array2D<float> Input, float NDV);
float Get_Maximum(Array2D<int> Input, float NDV);
int Get_Maximum_Index(Array2D<float> Input, int NDV);
int Get_Maximum_Index(Array2D<int> Input, int NDV);

// Method to get the maximum value in a 2D array - MDH 27/8/14
float Get_Minimum(Array2D<float> Input, int NDV);
float Get_Minimum(Array2D<int> Input, int NDV);
int Get_Minimum_Index(Array2D<float> Input, int NDV);
int Get_Minimum_Index(Array2D<int> Input, int NDV);

//Routine to count the number of values in an array - MDH 27/8/14
int Get_Value_Count(Array2D<float> Input, int NDV);
int Get_Value_Count(Array2D<int> Input, int NDV);

//Method to flatten a 2D array into a 1D vector
//generates a vector in row major order
//SWDG 12/6/14
vector<float> Flatten(Array2D<float> Input);
vector<float> Flatten_Without_Nodata(Array2D<float> Input, float NDV);
vector<int> Flatten(Array2D<int> Input);
vector<int> Flatten_Without_Nodata(Array2D<int> Input, float NDV);

//Method to count the number of instances of a given value in an array
//SWDG 17/6/14
int CountValue(Array2D<int> Input, int Value);
int CountValue(Array2D<float> Input, float Value); 


//Method used to generate a Kolmogorov-Smirnov statistic and p value
//from numerical recipes
//Data1 and Data2 must be sorted.
//d is the KS statistic value
//p is the p-value. In numerical recipes it is provided as a value subtracted from
//1, this code has been modified to present value the without this subtraction
//so that it matches the result from the scipy implementation. SWDG 26/6/14
void KStwo(vector<float> Data1, vector<float> Data2, float& d, double& p);
float PKS(float z);

// gets the value of a normal distribution at a point x
// mu is mean
// sigma is standard deviation
// x is the point you want the normal distribution evaluated
float NormalDistributionAtX(float mu, float sigma, float x);

// this gets the p value of a normal distribution for a given Z
float pValueNormalDistribution(float Z);

// This is a function to perform the Mann-Whitney U test, a nonparametric
// test that checks if two data sets have the same median
float MannWhitneyUTest(vector<float>& sampleA, vector<float>& sampleB);

// this takes a sorted vector and then finds the normalised ranks (that is
// if data elements are the same the ranks take an average rank)
// It replaces two vectors passed to it
void rank_vector_with_groups(vector<float> sorted_data, 
                             vector<float>& ranks, vector<int>& number_in_groups);

// Given a filestream object, read the file into memory and return
// it as a string. From: http://www.cplusplus.com/forum/general/58945/
// No error handling.
// SWDG 16/07/14
string ReadTextFile(ifstream& File);

void Count_Instances(vector<int> Data, vector<int> Key_Values, map<int,int>& DataMap);

// removes control characters from the end of strings. 
// This is necessary when people use a DOS file format, which 
// stupidly adds control characters to the end of lines. 
string RemoveControlCharactersFromEndOfString(string toRemove);

// removes all control characters
string RemoveControlCharacters(string toRemove);

// removes spaces
string RemoveControlCharacters(string toRemove);

// INVERSE ERROR FUNCTIONS AND INVERSE COMPLEMENTARY ERROR FUNCTIONS
// DTM, Following Press et al.,2007; Numerical Recipes, the Art of Scientific Computing, CUP
// Inverse Complementary error function.  Returns x such that erfc(x)=p within limits 0<p<2
float inverfc(float p);
// Inverse Complementary error function.  Returns x such that erf(x)=p within limits -1<p<1
float inverf(float p);

#endif




