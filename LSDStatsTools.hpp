#include <vector>
#include "TNT/tnt.h"
using namespace std;
using namespace TNT;


#ifndef StatsTools_H
#define StatsTools_H

// computes linear regression
// replaces data in residuals with residuals and returns a 4 element vector, which has slope, intercept, r^2 and
// the Durbin-Watson test statistic which looks for autocorrelation of the residuals
vector<double> simple_linear_regression(vector<double>& x_data, vector<double>& y_data, vector<double>& residuals);
double get_mean(vector<double>& y_data);
double get_SST(vector<double>& y_data, double mean);
double get_durbin_watson_statistic(vector<double> residuals);
double get_standard_deviation(vector<double>& y_data, double mean);
double get_standard_error(vector<double>& y_data, double standard_deviation);
vector<double> get_common_statistics(vector<double>& y_data);

// these look for linear segments within a data series.
void populate_segment_matrix(int start_node, int end_node, double no_data_value,
								vector<double>& all_x_data, vector<double>& all_y_data, int maximum_segment_length,
								double sigma, Array2D<double>& like_array, Array2D<double>& m_array,
								Array2D<double>& b_array, Array2D<double>& rsquared_array,
								Array2D<double>& DW_array);
void calculate_segment_matrices(vector<double>& all_x_data, vector<double>& all_y_data, int maximum_segment_length,
								double sigma, Array2D<double>& like_array, Array2D<double>& m_array,
								Array2D<double>& b_array, Array2D<double>& rsquared_array,
								Array2D<double>& DW_array);
void find_max_like_of_segments(int minimum_segment_length, Array2D<double>& like_array,
								vector<double>& max_MLE, vector< vector<int> >& segments_for_each_n_segments);
void find_max_AIC_of_segments(int minimum_segment_length, vector<double>& all_x_data, vector<double>& all_y_data,
								Array2D<double>& like_array,
								vector<double>& max_MLE, vector<double>& AIC_of_segments,
								vector<double>& AICc_of_segments, vector< vector<int> >& segments_for_each_n_segments);
void calculate_AIC_of_segments_with_normalized_sigma(double sigma,
								vector<double>& one_sigma_max_MLE, vector<double>& all_x_data,
								vector<double>& AIC_of_segments,vector<double>& AICc_of_segments);

// the below function is the main driver for the segment fitting code.
void best_fit_driver_AIC_for_linear_segments(int minimum_segment_length, double sigma,
											vector<double> all_x_data, vector<double> all_y_data,
											vector<double>& max_MLE);

// this gets the number of segments for several different values of sigma bassed in the vector sigma_values
void get_n_segments_for_various_sigma(vector<double> sigma_values, vector<double> one_sig_max_MLE,
									  vector<double>& all_x_data,
								      vector<int>& best_fit_AIC, vector<int>& best_fit_AICc,
								      vector< vector<double> >& AIC_for_each_n_segments,
								      vector< vector<double> >& AICc_for_each_n_segments);

// this prints full AIC and AICc information to screen
void print_AIC_and_AICc_to_screen(vector<double> sigma_values, vector< vector<int> > segments_for_each_n_segments,
								      vector<int> best_fit_AIC, vector<int> best_fit_AICc,
								      vector< vector<double> > AIC_for_each_n_segments,
								      vector< vector<double> > AICc_for_each_n_segments);

// this prints information about the most likeley segments to screen
void print_to_screen_most_likeley_segment_lengths( vector< vector<int> > segments_for_each_n_segments,
										vector<double> MLE_for_segments);

// this returns the m, b, r2 and DW stats of each segment
void get_properties_of_best_fit_segments(int bestfit_segments_node, vector< vector<int> >& segments_for_each_n_segments,
										 vector<double>& m_values, Array2D<double>& m_array,
										 vector<double>& b_values, Array2D<double>& b_array,
										 vector<double>& r2_values, Array2D<double>& rsquared_array,
										 vector<double>& DW_values, Array2D<double>& DW_array);

// these functions manipulate likelihood matrices and vectors for use with the segment tool
Array2D<double> normalize_like_matrix_to_sigma_one(double sigma, Array2D<double>& like_array);
vector<double> normalize_like_vector_to_sigma_one(double sigma, vector<double> like_vector);
Array2D<double> change_normalized_like_matrix_to_new_sigma(double sigma, Array2D<double>& sig1_like_array);
vector<double> change_normalized_like_vector_to_new_sigma(double sigma, vector<double> sig1_like_vector);

// this uses a moving window to find segments and is incomplete
void find_linear_segments(vector<double>& all_x_data, vector<double>& all_y_data, int segment_length);


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
void generate_random_segments(double sigma, int minimum_n_nodes, int mean_segment_length, int segment_range,
							  double dx, double offset_range, double m_range,
							 vector<double>& x_data, vector<double>& y_data,
							 vector<int>& segment_length, vector<double>& slope, vector<double>& intercept);


// maxiumum likihood estimators
double calculate_MLE(vector<double>& measured, vector<double>& modelled, vector<double>& sigma);
double calculate_MLE(vector<double>& measured, vector<double>& modelled, double sigma);
double calculate_MLE_from_residuals(vector<double>& residuals, double sigma);

// a random number generator
double ran3( long *idum );
// conversion from numbers to strings
string itoa(int num);
string dtoa(double num);


// Log binning module
// two overloaded functions:
//    -> for data stored in a 2D array (e.g. slope-area)
void log_bin_data(Array2D<double>& InputArrayX, Array2D<double>& InputArrayY, double log_bin_width,
				  vector<double>&  MeanX_output, vector<double>& MeanY_output, vector<double>& midpoints_output,
				  vector<double>&  StandardDeviationX_output, vector<double>&  StandardDeviationY_output,
				  double NoDataValue = -9999);
//    -> for data stored in a 1D vector (e.g. for spectral analysis)
void log_bin_data(vector<double>& InputVectorX, vector<double>& InputVectorY, double log_bin_width,
				  vector<double>&  MeanX_output, vector<double>& MeanY_output, vector<double>& midpoints_output,
				  vector<double>&  StandardDeviationX_output, vector<double>&  StandardDeviationY_output, int NoDataValue);

// tools for sorting
template<class T> struct index_cmp;
void matlab_double_sort(vector<double>& unsorted, vector<double>& sorted, vector<size_t>& index_map);
void matlab_double_reorder(std::vector<double> & unordered, std::vector<size_t> const & index_map, std::vector<double> & ordered);
void matlab_double_sort_descending(vector<double>& unsorted, vector<double>& sorted, vector<size_t>& index_map);

// conversion from degrees to radians
double rad(double degree);

#endif




