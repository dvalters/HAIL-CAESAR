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
#include <fstream>
#include <algorithm>
#include <math.h>
#include <time.h>
#include "TNT/tnt.h"
#include "TNT/jama_lu.h"
#include "LSDStatsTools.hpp"
using namespace std;
using namespace TNT;
using namespace JAMA;

#ifndef StatsTools_CPP
#define StatsTools_CPP

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*********************************************************\
**  ran3
**
**  Random number generator from Numerical Recipes.
**  Returns a uniform random number between 0.0 and 1.0.
**  Set idum to any negative value to initialize or
**  reinitialize the sequence.
**
**  Parameters: idum - random seed
**
\*********************************************************/
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define MBIG  990000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)

float ran3(long *idum)
{
   //cout << &idum << endl;
   static int inext,inextp;
   static long ma[56];
   static int iff=0;
   long mj,mk;
   int i,ii,k;

   if (*idum < 0 || iff == 0) {
      iff=1;
      if(*idum>0)
       *idum = - *idum;
      mj=MSEED+ *idum;
      if (mj<0)
       mj = -mj;
      mj %= MBIG;
      ma[55]=mj;
      mk=1;
      for (i=1;i<=54;i++) {
         ii=(21*i) % 55;
         ma[ii]=mk;
         mk=mj-mk;
         if (mk < MZ) mk += MBIG;
         mj=ma[ii];
      }
      for (k=1;k<=4;k++)
          for (i=1;i<=55;i++) {
             ma[i] -= ma[1+(i+30) % 55];
             if (ma[i] < MZ) ma[i] += MBIG;
          }
      inext=0;
      inextp=31;
      *idum=1;
   }
   if (++inext == 56) inext=1;
   if (++inextp == 56) inextp=1;
   mj=ma[inext]-ma[inextp];
   if (mj < MZ) mj += MBIG;
   ma[inext]=mj;
   return fabs(mj*FAC);
}
#undef MBIG
#undef MSEED
#undef MZ
#undef FAC
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Randomly sample from a vector without replacement DTM 21/04/2014
//------------------------------------------------------------------------------
vector<float> sample_without_replacement(vector<float> population_vector, int N)
{
  vector<float> sample_vector;
  int Population = population_vector.size(); 
  if(N > Population)
  {
  	cout << "N>Population size, try again" << endl;
    exit(EXIT_FAILURE);
  }
  
  int sample_count = 0;
  float random_number,Population_remaining;
  long seed;
  int vector_ref;
  while (sample_count < N)
  {
    Population_remaining = population_vector.size();
    seed = time(NULL);
    random_number = ran3(&seed);
    vector_ref = floor(random_number*Population_remaining);
    
    if(vector_ref == Population_remaining)
    {
      vector_ref = Population_remaining - 1;
    }
    
    sample_vector.push_back(population_vector[vector_ref]);
    population_vector.erase(population_vector.begin()+vector_ref);
    ++sample_count;
  }                                                               
  return sample_vector;
}
vector<int> sample_without_replacement(vector<int> population_vector, int N)
{
  vector<int> sample_vector;
  int Population = population_vector.size(); 
  if(N > Population)
  {
  	cout << "N>Population size, try again" << endl;
    exit(EXIT_FAILURE);
  }
  
  int sample_count = 0;
  float random_number,Population_remaining;
  long seed;
  int vector_ref;
  while (sample_count < N)
  {
    Population_remaining = population_vector.size();
    seed = time(NULL);
    random_number = ran3(&seed);
    vector_ref = floor(random_number*Population_remaining);
    
    if(vector_ref == Population_remaining)
    {
      vector_ref = Population_remaining - 1;
    }
    
    sample_vector.push_back(population_vector[vector_ref]);
    population_vector.erase(population_vector.begin()+vector_ref);
    ++sample_count;
  }                                                               
  return sample_vector;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gets the mean from a population of y_data
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float get_mean(vector<float>& y_data)
{
	int n_data_points = y_data.size();

	float total = 0;
	float mean;
	for (int i = 0; i< n_data_points; i++)
	{
		total+=y_data[i];
	}
	mean = total/float(n_data_points);
	return mean;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gets the mean from a population of y_data
// This takes and array and ignores no data values
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float get_mean_ignore_ndv(Array2D<float>& data, float ndv)
{
  int NRows = data.dim1();
  int NCols = data.dim2();

  int count = 0;
  float total = 0;
  float mean;

  for (int row = 0; row<NRows; row++)
  {
    for (int col = 0; col<NCols; col++)
    {
      if (data[row][col] != ndv)
      {
        total+= data[row][col];
        count++;
      }
    }
  }

  mean = total/float(count);
  return mean;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gets the total sum of squares from a population of data
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float get_SST(vector<float>& y_data, float mean)
{
	int n_data_points = y_data.size();

	float total = 0;
	for (int i = 0; i< n_data_points; i++)
	{
		total+=(y_data[i]-mean)*(y_data[i]-mean);
	}
	return total;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gets the total sum of squares from a population of data
// this uses and array and ignores no data values
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float get_variance_ignore_ndv(Array2D<float>& data, float ndv, float mean)
{
  int NRows = data.dim1();
  int NCols = data.dim2();

  int count = 0;
  float total = 0;
  float variance;

  for (int row = 0; row<NRows; row++)
  {
    for (int col = 0; col<NCols; col++)
    {
      if (data[row][col] != ndv)
      {
        total+=(data[row][col]-mean)*(data[row][col]-mean);
        count++;
      }
    }
  }

  variance = total/float(count);
  return variance;
}  


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gets the standard deviation from a population of data
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float get_standard_deviation(vector<float>& y_data, float mean)
{
	int n_data_points = y_data.size();

	float total = 0;
	for (int i = 0; i< n_data_points; i++)
	{
		total+=(y_data[i]-mean)*(y_data[i]-mean);
	}
	return sqrt(total/float(n_data_points));
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gets the standard error from a population of data
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float get_standard_error(vector<float>& y_data, float standard_deviation)
{
	int n_data_points = y_data.size();
	return standard_deviation/(sqrt(float(n_data_points)));
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function returns a vector with several common statistics
// the vector has the following elements
// 0 mean
// 1 sum of squares
// 2 standard deviation
// 3 standard error
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
vector<float> get_common_statistics(vector<float>& y_data)
{
	int n_data_points = y_data.size();
	float mean = get_mean(y_data);
	float SST = get_SST(y_data, mean);
	float standard_deviation = sqrt(SST/float(n_data_points));
	float standard_error = standard_deviation/(sqrt(float(n_data_points)));

	vector<float> common_statistics(4);
	common_statistics[0] = mean;
	common_statistics[1] = SST;
	common_statistics[2] = standard_deviation;
	common_statistics[3] = standard_error;

	return common_statistics;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// gets specified percentile, from pre-sorted vector, following same method as MS Excel.  Note
// that the percentile should be expressed as a percentage i.e. for median percentile = 50, NOT
// 0.5!
// DTM 14/04/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float get_percentile(vector<float>& data, float percentile)
{
	int N = data.size();                                               
	float n = percentile*(float(N)-1)/100;                                
	int k = int(floor(n));                                             
	float d = n - floor(n);                                            
	float percentile_value;                                            
  if(k>=N-1) percentile_value = data[k-1];                             
  else if (k < 0) percentile_value = data[0];                       
  else percentile_value = data[k] + d*(data[k+1]-data[k]);         
  return percentile_value;   
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function tests for autocorrelation between residuals
// if the number is less than 2 the residuals show autocorrelation
// if the number is less than 1 there is clear evidence for autocorrelation
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float get_durbin_watson_statistic(vector<float> residuals)
{
	int n_observations = residuals.size();

	float top_term = 0;
	float bottom_term = 0;
	for (int i = 0; i<n_observations; i++)
	{
		if (i!=0)
		{
			top_term+=(residuals[i]-residuals[i-1])*(residuals[i]-residuals[i-1]);
		}
		bottom_term+=residuals[i]*residuals[i];
	}

	if(bottom_term == 0)
	{
		bottom_term = 1e-10;
	}

	return(top_term/bottom_term);
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// calculate the imaginary error function using trapezoid rule integration
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
double erfi(double tau)
{
	double sum = 0;
	double N_nodes = 100000;
	double dtau = tau/N_nodes;
	//double mini_dtau = dtau/4.0;
	//double term1,term2, term3, term4, term5;
	//double a;
	//double front_term = dtau/90.0;
	int ii;

	// trapezoidal rule
	double xloc_old;
	double xloc_new = 0.0;
	for (ii= 0; ii<N_nodes; ii++)
	{
		xloc_old = xloc_new;
		xloc_new = double(ii)*dtau;
		sum+= 0.5*dtau*(exp(xloc_old*xloc_old) + exp(xloc_new*xloc_new) );
	}

	// simpson's
	//	double xloc_old;
	//	double xloc_new = 0.0;
	//	double xloc_mid;
	//	front_term = dtau/6;
	//	for (ii= 0; ii<N_nodes; ii++)
	//	{
	//		xloc_old = xloc_new;
	//		xloc_new = double(ii)*dtau;
	//		xloc_mid = 0.5*(xloc_old+xloc_new);
	//		sum+= front_term*(exp(xloc_old*xloc_old) + 4*exp(xloc_mid*xloc_mid) + exp(xloc_new*xloc_new) );
	//	}


	// boole's
	//	for (ii = 0; ii<N_nodes; ii++);
	//	{
	//		a = dtau*double(ii);
	//		term1 =  7.0*exp(a*a);
	//		term2 = 32.0*exp((a+mini_dtau)*(a+mini_dtau));
	//		term3 = 12.0*exp((a+2.0*mini_dtau)*(a+2.0*mini_dtau));
	//		term4 = 32.0*exp((a+3.0*mini_dtau)*(a+3.0*mini_dtau));
	//		term5 =  7.0*exp((a+4.0*mini_dtau)*(a+4.0*mini_dtau));
	//		sum += front_term*(term1+term2+term3+term4+term5);
	//	}

	return 2*sum/sqrt(M_PI);
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function returning a Gaussian random number
// DAV 16/10/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
double Gauss_rand(int Nrand, double GaussAdd, double GaussFac)
{
	double sum = 0;
	
	for (int i=1; i <= Nrand; i++)
	{
		sum = sum + rand();
	}
	return (GaussFac * sum - GaussAdd);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this gets a simple linear regression where the regression model is y = mx+b
// it returns a vector with the best fit values for m, b, r^2 and the durban_watson
// statistic (which is used to test if the residuals are autocorrelated
// it also replaces the residuals vector with the actual residuals from the
// best fit
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
vector<float> simple_linear_regression(vector<float>& x_data, vector<float>& y_data, vector<float>& residuals)
{
	float rounding_cutoff = 1e-12;

	int n_rows = x_data.size();
	int n_cols = 2;
	Array2D<float> A(n_rows,n_cols);
	Array2D<float> b(n_rows,1);
	Array2D<float> A_transpose(n_cols,n_rows);

	// construct solution matrices
	for(int i = 0; i<n_rows; i++)
	{
		A[i][0] = x_data[i];
		A[i][1] = 1.0;
		A_transpose[0][i] = x_data[i];
		A_transpose[1][i] = 1.0;
		b[i][0] = y_data[i];
	}


	// solve the system
	Array2D<float> LHS = matmult(A_transpose,A);
	Array2D<float> RHS = matmult(A_transpose,b);
	LU<float> LU_mat(LHS);
	Array2D<float> solution= LU_mat.solve(RHS);


	vector<float> soln;
	for(int i = 0; i<2; i++)
	{
		soln.push_back(solution[i][0]);
	}

	// get some statistics
	float mean = get_mean(y_data);
	float SST = get_SST(y_data, mean);
	// now get the predictions
	vector<float> predicted;
	vector<float> temp_residuals;

	// get predicted, residuals, etc
	float SS_reg = 0;
	float SS_err = 0;
	//cout << endl;
	for(int i = 0; i<n_rows; i++)
	{
		predicted.push_back(soln[0]*x_data[i] + soln[1]);
		temp_residuals.push_back(predicted[i]-y_data[i]);
		if (fabs(temp_residuals[i]) < rounding_cutoff)
		{
			temp_residuals[i] = 0;
		}
		SS_reg+=(predicted[i]-mean)*(predicted[i]-mean);


		SS_err+=temp_residuals[i]*temp_residuals[i];

		//cout << "RESIDUAL, i: " << i << " pred: " << predicted[i] << " data: " << y_data[i] << " resid: " << temp_residuals[i] << endl;
	}

	//cout << "SST: " << SST << " SS_reg: " << SS_reg << " SS_err " << SS_err << endl;

	// now get R^2
	soln.push_back(1 - SS_err/SST);

	// now get the durbin_watson statistic
	soln.push_back( get_durbin_watson_statistic(temp_residuals) );

	residuals = temp_residuals;
	return soln;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// least_squares_linear_regression
// DTM 07/10/2014
void least_squares_linear_regression(vector<float> x_data,vector<float> y_data, float& intercept, float& gradient, float& R_squared)
{
  float SS_xx=0;
  float SS_yy=0;
  float SS_xy=0;
  float x_mean = get_mean(x_data);
  float y_mean = get_mean(y_data);
  for(int i = 0; i<x_data.size(); ++i)
  {
    SS_xx += (x_data[i]-x_mean)*(x_data[i]-x_mean);
    SS_yy += (y_data[i]-y_mean)*(y_data[i]-y_mean);
    SS_xy += (x_data[i]-x_mean)*(y_data[i]-y_mean);
  }
  R_squared = SS_xy*SS_xy/(SS_xx*SS_yy);
  gradient = SS_xy/SS_xx;
  intercept = y_mean - gradient*x_mean; 
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Slice_vector
// This function accesses a slice of a vector
// Usage vector<float> new_vec = slice_vector(original_vector.begin() + 1, original_vector.begin() + 5);
// DTM 30/10/2014
vector<float> slice_vector(vector<float>::iterator first,vector<float>::iterator last)
{
  vector<float> slice;
  for (vector<float>::iterator cur = first; cur != last; ++cur)
  {
    slice.push_back(*cur);
  }
  return slice;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function drives the partitioning algorithms
// k is the number of elements in the partition
// minimum lenght is the minimum length of the segment
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void partition_driver_to_screen(int k, int minimum_length)		// this just prints partitions to screen
{
	int n = 2*k;
	int t = 0;
	vector<int> p(k,0);

	partitions_with_minimum_length( n, k, t, minimum_length, p);

}

// this version returns the partition vecvecvec
vector< vector < vector<int> > > partition_driver_to_vecvecvec(int k, int minimum_length)
{
	int n = 2*k;
	int t = 0;
	vector<int> p(k,0);

	int max_segments = k/minimum_length;
	vector< vector < vector<int> > > partitions(max_segments);

	// run the partitioning code
	cout << "partition_driver_to_vecvecvec, doing partitions" << endl;
	partitions_with_minimum_length( n, k, t, minimum_length, p, partitions);
	cout << "partition_driver_to_vecvecvec, finished partitions" << endl;

	return partitions;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// an integer partition algorithm
// Algorithm and original Pascal implementation: Frank Ruskey, 1995.
// Translation to C: Joe Sawada, 1997
// grabbed from http://theory.cs.uvic.ca/inf/nump/NumPartition.html
// adapted smm 21/12/2012
// algorith described in
// http://mathworld.wolfram.com/PartitionFunctionP.html
// and
// Skiena, S. Implementing Discrete Mathematics: Combinatorics and Graph Theory with Mathematica. Reading, MA: Addison-Wesley, 1990.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void integer_partition(int n, int k, int t, vector<int>& p)
{

	int j;

	p[t] = k;
	if (n==k)
	{
		partition_print(t,p);
	}
	for (j=partitions_min(k,n-k); j>=1; j--)
	{
		integer_partition (n-k,j,t+1, p);
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// an integer partition algorithm
// Algorithm and original Pascal implementation: Frank Ruskey, 1995.
// Translation to C: Joe Sawada, 1997
// grabbed from http://theory.cs.uvic.ca/inf/nump/NumPartition.html
// adapted smm 21/12/2012
// algorith described in
// http://mathworld.wolfram.com/PartitionFunctionP.html
// and
// Skiena, S. Implementing Discrete Mathematics: Combinatorics and Graph Theory with Mathematica. Reading, MA: Addison-Wesley, 1990.
// this is a further adaptation that only presents solution to the partition
// with segments of a minimum length
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void partitions_with_minimum_length(int n, int k, int t, int min_length, vector<int>& p)
{

	int j;

	p[t] = k;
	if (n==k)
	{
		partition_print(t,p);
	}
	for (j=partitions_min(k,n-k); j>=min_length; j--)
	{
		partitions_with_minimum_length(n-k,j,t+1,min_length,p);
	}
}

// overloaded function that stores all the partitions
// http://www.codeguru.com/cpp/cpp/algorithms/article.php/c5123/Permutations-in-C.htm
// http://www.cplusplus.com/reference/algorithm/next_permutation/
// http://mdm4u1.wetpaint.com/page/4.3+Permutations+with+Some+Identical+Elements
void partitions_with_minimum_length(int n, int k, int t, int min_length, vector<int>& p,
								vector< vector < vector<int> > >& partitions)
{

	int j;

	p[t] = k;
	if (n==k)
	{
		partition_assign(t, p, partitions);
	}
	for (j=partitions_min(k,n-k); j>=min_length; j--)
	{
		partitions_with_minimum_length(n-k,j,t+1,min_length,p, partitions);
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// a function for use with the permutations
// gets the mininum of two values
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int partitions_min( int x, int y)
{
	if (x<y)
	{
		return x;
	}
	else
	{
		return y;
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// a function for use with the permutations
// this assigns values into the vecvecvec that contains all the partitioning information
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void partition_assign(int t, vector<int>& p, vector< vector < vector<int> > >& partitions)
{

	vector< vector<int> > this_nsegments_vecvec = partitions[t-1];
	vector<int> this_partitions_partitions;

	for(int i=1; i<=t; i++)
	{
		this_partitions_partitions.push_back( p[i] );
	}
	this_nsegments_vecvec.push_back(this_partitions_partitions);
	partitions[t-1] = this_nsegments_vecvec;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// a function for use with the permutations
// gets the mininum of two values
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void partition_print(int t, vector<int>& p)
{

	for(int i=1; i<=t; i++)
	{
		cout << p[i] << " ";
	}
	cout << endl;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function prints the elements in the vecvecvec of possible partitions
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void partition_vecvecvec_print(vector< vector < vector<int> > >& partitions)
{
	cout << "n_possible_segments: " << partitions.size() << endl;
	vector< vector <int> > partition_vecvec;
	for (int i = 0; i< int(partitions.size()); i++)
	{
		partition_vecvec = partitions[i];
		int n_partitions_this_nsegments = partition_vecvec.size();
		//cout << "n_segments: " << i+1 << " and number of partitions of this n segments: " << n_partitions_this_nsegments << endl;
		for (int j = 0; j< n_partitions_this_nsegments; j++)
		{
			vector<int> individual_partition = partition_vecvec[j];
			int sz_ind_partition = individual_partition.size();
			for(int k = 0; k<sz_ind_partition; k++)
			{
				cout << individual_partition[k] << " ";
			}
			cout << endl;
		}
	}
}


void partition_vecvecvec_print_with_permutation(vector< vector < vector<int> > >& partitions)
{
	cout << "n_possible_segments: " << partitions.size() << endl;
	vector< vector <int> > partition_vecvec;
	for (int i = 0; i< int(partitions.size()); i++)
	{
		partition_vecvec = partitions[i];
		int n_partitions_this_nsegments = partition_vecvec.size();
		cout << "n_segments: " << i+1 << " and number of partitions of this n segments: " << n_partitions_this_nsegments << endl;
		for (int j = 0; j< n_partitions_this_nsegments; j++)
		{
			vector<int> individual_partition = partition_vecvec[j];
			permute_partitioned_integer_vector(individual_partition);
			cout << endl;
		}
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function permutes a partitioned integer vector
// the partitioning supplies vectors sorted in reverse order
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void permute_partitioned_integer_vector(vector<int> permute_vector)
{
	int n_elements = permute_vector.size();
	//for (int i = 0; i<n_elements; i++)
	//{
	//	cout << permute_vector[i] << " ";
	//}
	//cout << endl;

	do
	{
		for (int i = 0; i<n_elements; i++)
		{
			cout << permute_vector[i] << " ";
		}
		cout << endl;
	} while ( prev_permutation (permute_vector.begin(),permute_vector.end()) );
}




//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function is used to calcualte the slope, intercept, and likelihood of
// all possible linear segments along a series of data points.
// the function requires the data in x and y vectors, the maximum segment length
// and sigma, the standard deviation of the measured data. This will be approxamately
// the error in the surface elevation, although it might have to be increased simply because
// likelihood will tend to zero if this is too small. sigma should also be considered to
// contain the 'noise' inherent in channel incision so perhaps 1-5 metres is appropriate
// the maximum segment length is an integer: it is the number of data points used.
// these data points from raw chi data are irregularly spaced so two segments of the same
// 'length' can have different lengths in chi space. One remedey for this is a preprocessor that
// places the zeta vs chi data along evenly spaced points.
//
// The routine generates three matrices. The row of the matrix is the starting node of the segment.
// The column of the matrix is the ending node of the segment. Thus the routine will generate a
// matrix that is dimension n x n where n is the number of data points.
//
// One potential future development is to implement this using a sparse matrix from the boost mtl
// library to reduce the memory usage.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void calculate_segment_matrices(vector<float>& all_x_data, vector<float>& all_y_data, int minimum_segment_length,
								float sigma, Array2D<float>& like_array, Array2D<float>& m_array,
								Array2D<float>& b_array, Array2D<float>& rsquared_array,
								Array2D<float>& DW_array)
{
	int n_data_points = all_x_data.size();
	if (minimum_segment_length>n_data_points)
	{
		cout << "LSDStatsTools find_linear_segments: your segment length is greater than half the number of data points" << endl;
		cout << "This means that there can only be overlapping segments. Changing segment length to maximum segment length "<< endl;
		minimum_segment_length = n_data_points;
	}

	// set up the arrays
	// in the future I might consider using sparse arrays but for now we'll just populate
	// the empty spots with placeholders
	float no_data_value = -9999;
	Array2D<float> temp_array(n_data_points,n_data_points,no_data_value);
	like_array = temp_array.copy();
	m_array = temp_array.copy();
	b_array = temp_array.copy();
	rsquared_array = temp_array.copy();
	DW_array = temp_array.copy();

	int start_node = 0;
	int end_node = n_data_points-1;

	// populate the matrix.
	// the get segment row function is recursive so it moves down through all the possible
	// starting nodes
	//cout << "LINE 518, sigma is: " << sigma << endl;
	populate_segment_matrix(start_node, end_node, no_data_value, all_x_data, all_y_data, minimum_segment_length,
							sigma, like_array, m_array,b_array, rsquared_array, DW_array);

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function popultes the matrices of liklihood, m and b values
// it is a recursive algorithm so in fact it deosn't just get one row
// but drills down through all the possible starting nodes to coplete the
// matrix
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void populate_segment_matrix(int start_node, int end_node, float no_data_value,
								vector<float>& all_x_data, vector<float>& all_y_data, int minimum_segment_length,
								float sigma, Array2D<float>& like_array, Array2D<float>& m_array,
								Array2D<float>& b_array, Array2D<float>& rsquared_array, Array2D<float>& DW_array)
{

	if (like_array[start_node][end_node] == no_data_value)
	{
		// create the two segments
		vector<float> segment_x;
		vector<float> segment_y;
		vector<float> residuals;
		vector<float> regression_results;
		float this_MLE;

		// now create iterators to deal with these segments
		vector<float>::iterator vec_iter_start;
		vector<float>::iterator vec_iter_end;

		// the first step is to get the segment starting on the
		// first node and ending on the last node
		// find out how many nodes are in the segment
		int n_nodes_in_segment = end_node - start_node+1;

		// resize the vectors accordingly
		segment_x.resize(n_nodes_in_segment);
		segment_y.resize(n_nodes_in_segment);

		// get the iterators for the start and end of the vectors
		vec_iter_start = all_x_data.begin()+start_node;
		vec_iter_end = vec_iter_start+n_nodes_in_segment;
		segment_x.assign(vec_iter_start,vec_iter_end);

		vec_iter_start = all_y_data.begin()+start_node;
		vec_iter_end = vec_iter_start+n_nodes_in_segment;
		segment_y.assign(vec_iter_start,vec_iter_end);

		// do the least squares regression on this segment
		// cout << "LINE 568, sigma is: " << sigma << endl;
		regression_results = simple_linear_regression(segment_x, segment_y, residuals);
		this_MLE = calculate_MLE_from_residuals( residuals, sigma);

		//cout << "LINE 584 doing start: " << start_node << " end: " << end_node << endl;

		like_array[start_node][end_node] = this_MLE;
		m_array[start_node][end_node] = regression_results[0];
		b_array[start_node][end_node] = regression_results[1];
		rsquared_array[start_node][end_node] = regression_results[2];
		DW_array[start_node][end_node] = regression_results[3];



		// now loop through all the end nodes that are allowed that are not the final node.
		// that is the first end node is first plus the maximum length -1 , and then
		// the final end node before the end of the data is the last node minus the
		// maximum length of the segment
		for (int loop_end = start_node+minimum_segment_length-1; loop_end< end_node-minimum_segment_length+1; loop_end++)
		{
			if (like_array[start_node][loop_end] == no_data_value)
			{
				// get this segment and resize the vectors
				n_nodes_in_segment = loop_end - start_node+1;
				segment_x.resize(n_nodes_in_segment);
				segment_y.resize(n_nodes_in_segment);

				// get the iterators for the start and end of the vectors
				vec_iter_start = all_x_data.begin()+start_node;
				vec_iter_end = vec_iter_start+n_nodes_in_segment;
				segment_x.assign(vec_iter_start,vec_iter_end);

				vec_iter_start = all_y_data.begin()+start_node;
				vec_iter_end = vec_iter_start+n_nodes_in_segment;
				segment_y.assign(vec_iter_start,vec_iter_end);

				// do the least squares regression on this segment
				regression_results = simple_linear_regression(segment_x, segment_y, residuals);
				this_MLE = calculate_MLE_from_residuals( residuals, sigma);

				// fill in the matrices
				like_array[start_node][loop_end] = this_MLE;
				m_array[start_node][loop_end] = regression_results[0];
				b_array[start_node][loop_end] = regression_results[1];
				rsquared_array[start_node][loop_end] = regression_results[2];
				DW_array[start_node][loop_end] = regression_results[3];
				//cout << "LINE 612 doing start: " << start_node << " end: " << loop_end << endl;

				// now get the row from the next segment
				populate_segment_matrix(loop_end+1, end_node, no_data_value,
										all_x_data, all_y_data, minimum_segment_length,
										sigma, like_array, m_array,b_array, rsquared_array, DW_array);
			}
		}

	}

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function drives the entire AIC engine
// it takes the minimum segment length
// and the x and y data
// and an estimate of the variability of the data
// It then overwrites a raft of data elements
// First, it returns
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void best_fit_driver_AIC_for_linear_segments(int minimum_segment_length, float sigma,
											vector<float> all_x_data, vector<float> all_y_data,
											vector<float>& max_MLE)
{

	float norm_sigma = 1.0;
	Array2D<float> like_array;			// array holding the liklihood values
	Array2D<float> m_array;			// array holding the m values
	Array2D<float> b_array;			// array holding the b values
	Array2D<float> rsquared_array;		// array holding R2 of individual segments
	Array2D<float> DW_array;			// array holding the durbin-watson statistic of indiviudal segments

	cout << "best_fit_driver_AIC_for_linear_segments, getting like data" <<endl;
	calculate_segment_matrices(all_x_data, all_y_data, minimum_segment_length,
								norm_sigma, like_array, m_array, b_array, rsquared_array,DW_array);
	cout << "best_fit_driver_AIC_for_linear_segments, got like data" <<endl;

	vector<float> one_sig_max_MLE;
	vector<float> AIC_of_segments;
	vector<float> AICc_of_segments;
	vector< vector<int> > segments_for_each_n_segments;
	find_max_like_of_segments(minimum_segment_length, like_array,
								one_sig_max_MLE, segments_for_each_n_segments);

	max_MLE = change_normalized_like_vector_to_new_sigma(sigma, one_sig_max_MLE);

	//print_to_screen_most_likeley_segment_lengths( segments_for_each_n_segments,
	//									one_sig_max_MLE);

	// now loop through a variety of sigma values to see what the minimum sigma is
	float d_sigma = 0.01;
	vector<float> sigma_values;
	for(int i = 1; i<=10; i++)
	{
		sigma_values.push_back(d_sigma*float(i));
	}

	vector<int> best_fit_AIC;
	vector<int> best_fit_AICc;
	vector< vector<float> > AIC_for_each_n_segments;
	vector< vector<float> > AICc_for_each_n_segments;
	get_n_segments_for_various_sigma(sigma_values, one_sig_max_MLE, all_x_data,
								      best_fit_AIC, best_fit_AICc, AIC_for_each_n_segments,
								      AICc_for_each_n_segments);

	print_AIC_and_AICc_to_screen(sigma_values, segments_for_each_n_segments,
								  best_fit_AIC, best_fit_AICc,
								  AIC_for_each_n_segments, AICc_for_each_n_segments);



	vector<float> b_values;
	vector<float> m_values;
	vector<float> r2_values;
	vector<float> DW_values;

	// get the m, b, etc from the data
	int n_sigma_for_printing = 2;
	int bestfit_segments_node = best_fit_AICc[n_sigma_for_printing];
	get_properties_of_best_fit_segments(bestfit_segments_node, segments_for_each_n_segments,
										 m_values, m_array, b_values, b_array,
										 r2_values, rsquared_array, DW_values, DW_array);

	// now print this data
	cout << "sigma is: " << sigma_values[n_sigma_for_printing]
		 << " one_sig MLE: " << one_sig_max_MLE[ best_fit_AICc[n_sigma_for_printing] ]
		 << " and the number of segments is: " << best_fit_AICc[n_sigma_for_printing]+1 << endl;
	for (int i = 0; i< best_fit_AICc[n_sigma_for_printing]+1; i++)
	{
		cout << m_values[i] << " " << b_values[i] << " " << r2_values[i] << " " << DW_values[i] << endl;
	}

}


// this function takes the normalized MLE values (normalized with sigma = 1) and returns the
// best fit number of segments from both the AIC and the AICc measures. It also returns
// two vector of vectors which are the AIC values for the varius values of sigma
// passed to the function in the sigma values vector
void get_n_segments_for_various_sigma(vector<float> sigma_values, vector<float> one_sig_max_MLE,
									  vector<float>& all_x_data,
								      vector<int>& best_fit_AIC, vector<int>& best_fit_AICc,
								      vector< vector<float> >& AIC_for_each_n_segments,
								      vector< vector<float> >& AICc_for_each_n_segments)
{
	int n_sigma = sigma_values.size();
	vector<float> empty_vec;
	vector<float> AIC_of_segments;
	vector<float> AICc_of_segments;
	vector< vector<float> > AIC_for_each(n_sigma);
	vector< vector<float> > AICc_for_each(n_sigma);
	vector<int> bf_AIC(n_sigma);
	vector<int> bf_AICc(n_sigma);

	// loop through the sigma values collecting the AIC and AICc values
	for (int i = 0; i< n_sigma; i++)
	{
		// calcualte the AIC values for this value of sigma
		calculate_AIC_of_segments_with_normalized_sigma(sigma_values[i], one_sig_max_MLE, all_x_data,
								AIC_of_segments,AICc_of_segments);
		AIC_for_each[i] = AIC_of_segments;
		AICc_for_each[i] = AICc_of_segments;

		// now find the minimum AIC and AICc
		float minimum_AIC = 10000;
		int min_AIC_segments = 0;
		float minimum_AICc = 10000;
		int min_AICc_segments = 0;

		int n_AIC = AIC_of_segments.size();
		for (int n_seg = 0; n_seg<n_AIC; n_seg++)
		{
			if(AIC_of_segments[n_seg] < minimum_AIC)
			{
				minimum_AIC = AIC_of_segments[n_seg];
				min_AIC_segments = n_seg;
			}

			if(AICc_of_segments[n_seg] < minimum_AICc)
			{
				minimum_AICc = AICc_of_segments[n_seg];
				min_AICc_segments = n_seg;
			}
		}

		bf_AIC[i] = min_AIC_segments;
		bf_AICc[i] = min_AICc_segments;
	}

	// replace vectors
	AIC_for_each_n_segments = AIC_for_each;
	AICc_for_each_n_segments = AICc_for_each;
	best_fit_AIC = bf_AIC;
	best_fit_AICc = bf_AICc;

}

// this function prints out the segment data to screen
void print_AIC_and_AICc_to_screen(vector<float> sigma_values, vector< vector<int> > segments_for_each_n_segments,
								      vector<int> best_fit_AIC, vector<int> best_fit_AICc,
								      vector< vector<float> > AIC_for_each_n_segments,
								      vector< vector<float> > AICc_for_each_n_segments)
{
	// get the number of sigma values
	int n_sigs = sigma_values.size();
	vector<float> AI_values;
	vector<int> this_minimum;
	int AI_sz;
	int n_mins;


	cout << endl << "LSDStatsTools print_AIC_and_AICc_to_screen" << endl;

	// loop through sigma values printing the best fits
	for (int i = 0; i<n_sigs; i++)
	{
		cout << endl << endl << "sigma is: " << sigma_values[i] << endl;
		AI_values = AIC_for_each_n_segments[i];
		AI_sz = int(AI_values.size());
		cout << "Min AIC node is: " << best_fit_AIC[i] << " and AIC values: ";
		for (int j = 0; j<AI_sz; j++)
		{
			cout << AI_values[j] << " ";
		}
		cout << endl;

		cout << "the segments lengths are: ";
		this_minimum = segments_for_each_n_segments[ best_fit_AIC[i] ];
		n_mins = best_fit_AIC[i]+1;
		for (int seg = 0; seg<n_mins; seg++)
		{
			cout << this_minimum[seg] << " ";
		}
		cout << endl;

		cout << "Min AICc node is: " << best_fit_AICc[i] << " and AICc values: ";
		AI_values = AICc_for_each_n_segments[i];
		for (int j = 0; j<AI_sz; j++)
		{
			cout << AI_values[j] << " ";
		}
		cout << endl;

		cout << "the segments lengths are: ";
		this_minimum = segments_for_each_n_segments[ best_fit_AICc[i] ];
		n_mins = best_fit_AICc[i]+1;
		for (int seg = 0; seg<n_mins; seg++)
		{
			cout << this_minimum[seg] << " ";
		}
		cout << endl;
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this takes a likelihood array that has been calcualted with a given sigma value and
// normalizes the sigma values as though sigma was equal to 1.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<float> normalize_like_matrix_to_sigma_one(float sigma, Array2D<float>& like_array)
{
	// get dimensions of the like array
	int nRows = like_array.dim1();
	int nCols = like_array.dim2();
	float no_data_value = -9999;
	float sigsquared = sigma*sigma;
	Array2D<float> sig1_like_array = like_array.copy();

	for (int row = 0; row<nRows; row++)
	{
		for(int col = 0; col<nCols; col++)
		{
			if(sig1_like_array[row][col] != no_data_value)
			{
				sig1_like_array[row][col] = pow(sig1_like_array[row][col],sigsquared);
			}
		}
	}

	return sig1_like_array;
}

// this normalizes but with vector data, for use with MLE vector for segments
vector<float> normalize_like_vector_to_sigma_one(float sigma, vector<float> like_vector)
{
	// get dimensions of the like vector
	int ndata = like_vector.size();
	float sigsquared = sigma*sigma;
	vector<float> sig1_like_vector(ndata);

	for (int i = 0; i<ndata; i++)
	{

		sig1_like_vector[i] = pow(like_vector[i],sigsquared);

	}

	return sig1_like_vector;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this takes a normalize likelihood array and updates the values to a new sigma value
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Array2D<float> change_normalized_like_matrix_to_new_sigma(float sigma, Array2D<float>& sig1_like_array)
{
	// get dimensions of the like array
	int nRows = sig1_like_array.dim1();
	int nCols = sig1_like_array.dim2();
	float no_data_value = -9999;
	float one_over_sigsquared = 1/(sigma*sigma);
	Array2D<float> like_array = sig1_like_array.copy();

	for (int row = 0; row<nRows; row++)
	{
		for(int col = 0; col<nCols; col++)
		{
			if(like_array[row][col] != no_data_value)
			{
				like_array[row][col] = pow(like_array[row][col],one_over_sigsquared);
			}
		}
	}

	return like_array;
}

// this does the same as above except for vector data
vector<float> change_normalized_like_vector_to_new_sigma(float sigma, vector<float> sig1_like_vector)
{
	// get dimensions of the like vector
	int ndata = sig1_like_vector.size();
	float one_over_sigsquared = 1/(sigma*sigma);
	vector<float> like_vector(ndata);

	for (int i = 0; i<ndata; i++)
	{
		like_vector[i] = pow(sig1_like_vector[i],one_over_sigsquared);
	}

	return like_vector;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function calcualtes the most likeley combination of segments given the liklihood
// of individual segments calcualted by the calculate_segment_matrices function
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void find_max_like_of_segments(int minimum_segment_length, Array2D<float>& like_array,
								vector<float>& max_MLE, vector< vector<int> >& segments_for_each_n_segments)
{
	// first get the number of nodes
	int n_data_points = like_array.dim1();
	if (minimum_segment_length>n_data_points)
	{
		cout << "LSDStatsTools find_max_AIC_of_segments: your segment length is greater than the number of data points" << endl;
		cout << "This means that there can only be overlapping segments. Changing segment length to minimum segment length "<< endl;
		minimum_segment_length = n_data_points;
	}

	// get the maximum number of segments
	int max_n_segments = n_data_points/minimum_segment_length;

	// initialize a vector for holding the MLE of each n_segments
	vector<float> MLE_for_segments(max_n_segments,0.0);

	// initialize a vecvec for holding the MLE segment partition
	vector< vector <int> > most_likely_segments(max_n_segments);

	// create the partition data element
	//cout << "find_max_like_of_segments, getting partitions" << endl;
	vector< vector < vector<int> > > partitions = partition_driver_to_vecvecvec(n_data_points, minimum_segment_length);
	//cout << "find_max_like_of_segments, got partitions" << endl;

	// now loop through the number of segments, calucalting the maximum likelihood each time
	vector< vector <int> > partition_vecvec;
	float this_MLE;
	int start_node,end_node;
	for (int n_elem = 0; n_elem< int(partitions.size()); n_elem++)
	{

		partition_vecvec = partitions[n_elem];
		int n_partitions_this_nsegments = partition_vecvec.size();
		//cout << "n_segments: " << n_elem+1 << " and number of partitions of this n segments: " << n_partitions_this_nsegments << endl;
		for (int n_partition = 0; n_partition< n_partitions_this_nsegments; n_partition++)
		{
			//cout << "number of partitions: " << n_partition+1 << " of " << n_partitions_this_nsegments << endl;
			vector<int> individual_partition = partition_vecvec[n_partition];
			int n_elements = individual_partition.size();

			do
			{
				// calcualte the MLE for this particular permutation
				this_MLE = 1;
				start_node = 0;
				for (int i = 0; i<n_elements; i++)
				{
					end_node = start_node+individual_partition[i]-1;
					//cout << "start node: " << start_node << " " << " end node: " << end_node
					//     << " and like: " << like_array[start_node][end_node] << endl;
					this_MLE = this_MLE*like_array[start_node][end_node];
					start_node = end_node+1;
				}
				//cout << "This MLE: " << this_MLE << " seg MLE: " << MLE_for_segments[n_elem] << endl;

				// now test if this MLE is better than the the best MLE so far for this number of
				// partitions
				if( this_MLE > MLE_for_segments[n_elem] )
				{
					//cout << "element is: " << n_elem << " new MLE" << endl;
					MLE_for_segments[n_elem] = this_MLE;
					most_likely_segments[n_elem] = individual_partition;
				}
			} while ( prev_permutation (individual_partition.begin(),individual_partition.end()) );

		}
	}
	segments_for_each_n_segments = most_likely_segments;
	max_MLE = MLE_for_segments;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function returns the m, b, r^2 and D-W values for the best fit segments
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void get_properties_of_best_fit_segments(int bestfit_segments_node, vector< vector<int> >& segments_for_each_n_segments,
										 vector<float>& m_values, Array2D<float>& m_array,
										 vector<float>& b_values, Array2D<float>& b_array,
										 vector<float>& r2_values, Array2D<float>& rsquared_array,
										 vector<float>& DW_values, Array2D<float>& DW_array)
{


	int n_segments = bestfit_segments_node+1; 		// this converts from 0-based indexing to
													// the actual number of segments

	// initialize temp_vectors
	vector<float> m(n_segments);
	vector<float> b(n_segments);
	vector<float> r2(n_segments);
	vector<float> DW(n_segments);

	// the start node and end node, used to index into the arrays
	int start_node,end_node;

	// get the segment lengths
	vector<int> individual_partition = segments_for_each_n_segments[bestfit_segments_node];

	// now loop through the segments
	start_node = 0;
	for (int i = 0; i<n_segments; i++)
	{
		end_node = start_node+individual_partition[i]-1;
		cout << "start node: " << start_node << " " << " end node: " << end_node
		     << " m: " << m_array[start_node][end_node] << " b: " << b_array[start_node][end_node]
		     << " r^2: " << rsquared_array[start_node][end_node]
		     << " DW: " << DW_array[start_node][end_node] << endl;
		m[i] = m_array[start_node][end_node];
		b[i] = b_array[start_node][end_node];
		r2[i] = rsquared_array[start_node][end_node];
		DW[i] = DW_array[start_node][end_node];
		start_node = end_node+1;
	}

	m_values = m;
	b_values = b;
	r2_values = r2;
	DW_values = DW;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function calculates AIC and AICc of segments taking the maximum_MLE based on a sigma of one
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void calculate_AIC_of_segments_with_normalized_sigma(float sigma,
								vector<float>& one_sigma_max_MLE, vector<float>& all_x_data,
								vector<float>& AIC_of_segments,vector<float>& AICc_of_segments)
{

	// recast the MLE_vector
	vector<float> new_sig_MLE = change_normalized_like_vector_to_new_sigma(sigma, one_sigma_max_MLE);

	// initialize the vector holding the Aikake Information Criterion
	// and then calcualte the AIC and the AICc
	vector<float> AIC(one_sigma_max_MLE.size(),0.0);
	vector<float> AICc(one_sigma_max_MLE.size(),0.0);
	for (int n_elem = 0; n_elem< int(one_sigma_max_MLE.size()); n_elem++)
	{
		float AICk = (float(n_elem)+1);
		float AICn = float(all_x_data.size());
		// if the MLE is 0 or less, this will throw an error. This only happens if the fit
		// is terrible so in this case set AIC and AICc to a large positive number
		if(new_sig_MLE[n_elem]<= 0)
		{
			AIC[n_elem] = 9999;
			AICc[n_elem] = 9999;
		}
		else
		{
			//cout << "n_segs: " << n_elem+1
			//	 << " MLE: " <<  new_sig_MLE[n_elem] << " log: "
			//	 << log( new_sig_MLE[n_elem]) << " 2nd term: "
			//	 << -2*log( new_sig_MLE[n_elem]) << endl;
			AIC[n_elem] = 2*AICk-2*log( new_sig_MLE[n_elem]);
			AICc[n_elem] = AIC[n_elem] + 2*AICk*(AICk+1)/(AICn-AICk-1);
		}
		//cout << "AIC: " << AIC[n_elem] << " and AICc: " << AICc[n_elem] << endl << endl;

	}
	AIC_of_segments = AIC;
	AICc_of_segments = AICc;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function prints the most likeley segment lengths to screen
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void print_to_screen_most_likeley_segment_lengths( vector< vector<int> > segments_for_each_n_segments,
										vector<float> MLE_for_segments)
{

	// loop through the number of segments, printing the
	// most likeley segment lengths to screen
	cout << endl << "printing most likeley segment lenghts: " << endl;
	for (int n_elem = 0; n_elem< int(segments_for_each_n_segments.size()); n_elem++)
	{
		cout << "n elements: " << n_elem << " and MLE: " << MLE_for_segments[n_elem] << endl;
		vector<int> individual_partition = segments_for_each_n_segments[n_elem];
		cout << "segment lengths: " << individual_partition.size() << endl;
		if ( int(individual_partition.size()) != n_elem+1)
		{
			cout << "LINE 707 statstools something is wrong n partitions is incorrect" << endl;
		}

		for (int i = 0; i<=n_elem; i++)
		{
			cout << individual_partition[i] << " ";
		}
	}
	cout << endl << "finished printing most likeley segment lengths" << endl;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// this function calcualtes the most likeley combination of segments given the liklihood
// of individual segfments calcualted by the calculate_segment_matrices function
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void find_max_AIC_of_segments(int minimum_segment_length, vector<float>& all_x_data, vector<float>& all_y_data,
								Array2D<float>& like_array,
								vector<float>& max_MLE, vector<float>& AIC_of_segments,
								vector<float>& AICc_of_segments, vector< vector<int> >& segments_for_each_n_segments)
{
	// first get the number of nodes
	int n_data_points = all_x_data.size();
	if (minimum_segment_length>n_data_points)
	{
		cout << "LSDStatsTools find_max_AIC_of_segments: your segment length is greater than the number of data points" << endl;
		cout << "This means that there can only be overlapping segments. Changing segment length to minimum segment length "<< endl;
		minimum_segment_length = n_data_points;
	}

	// get the maximum number of segments
	int max_n_segments = n_data_points/minimum_segment_length;

	// initialize a vector for holding the MLE of each n_segments
	vector<float> MLE_for_segments(max_n_segments,0.0);

	// initialize a vecvec for holding the MLE segment partition
	vector< vector <int> > most_likely_segments(max_n_segments);

	// create the partition data element
	vector< vector < vector<int> > > partitions = partition_driver_to_vecvecvec(n_data_points, minimum_segment_length);

	// now loop through the number of segments, calucalting the maximum likelihood each time
	vector< vector <int> > partition_vecvec;
	float this_MLE;
	int start_node,end_node;
	for (int n_elem = 0; n_elem< int(partitions.size()); n_elem++)
	{

		partition_vecvec = partitions[n_elem];
		int n_partitions_this_nsegments = partition_vecvec.size();
		//cout << "n_segments: " << n_elem+1 << " and number of partitions of this n segments: " << n_partitions_this_nsegments << endl;
		for (int n_partition = 0; n_partition< n_partitions_this_nsegments; n_partition++)
		{
			vector<int> individual_partition = partition_vecvec[n_partition];
			int n_elements = individual_partition.size();

			do
			{
				// calcualte the MLE for this particular permutation
				this_MLE = 1;
				start_node = 0;
				for (int i = 0; i<n_elements; i++)
				{
					end_node = start_node+individual_partition[i]-1;
					//cout << "start node: " << start_node << " " << " end node: " << end_node
					//     << " and like: " << like_array[start_node][end_node] << endl;
					this_MLE = this_MLE*like_array[start_node][end_node];
					start_node = end_node+1;
				}
				//cout << "This MLE: " << this_MLE << " seg MLE: " << MLE_for_segments[n_elem] << endl;

				// now test if this MLE is better than the the best MLE so far for this number of
				// partitions
				if( this_MLE > MLE_for_segments[n_elem] )
				{
					//cout << "element is: " << n_elem << " new MLE" << endl;
					MLE_for_segments[n_elem] = this_MLE;
					most_likely_segments[n_elem] = individual_partition;
				}
			} while ( prev_permutation (individual_partition.begin(),individual_partition.end()) );

		}
	}

	// initialize the vector holding the Aikake Information Criterion
	// and then calcualte the AIC and the AICc
	vector<float> AIC(partitions.size(),0.0);
	vector<float> AICc(partitions.size(),0.0);
	for (int n_elem = 0; n_elem< int(partitions.size()); n_elem++)
	{
		cout << "n elements: " << n_elem << " and MLE: " << MLE_for_segments[n_elem] << endl;
		vector<int> individual_partition = most_likely_segments[n_elem];
		cout << "segment lengths: " << individual_partition.size() << endl;
		if ( int(individual_partition.size()) != n_elem+1)
		{
			cout << "LINE 707 statstools something is wrong n partitions is incorrect" << endl;
		}

		for (int i = 0; i<=n_elem; i++)
		{
			cout << individual_partition[i] << " ";
		}
		cout << endl;

		float AICk = (float(n_elem)+1);
		float AICn = float(all_x_data.size());
		// if the MLE is 0 or less, this will throw an error. This only happens if the fit
		// is terrible so in this case set AIC and AICc to a large positive number
		if(MLE_for_segments[n_elem]<= 0)
		{
			AIC[n_elem] = 9999;
			AICc[n_elem] = 9999;
		}
		else
		{
			cout << "MLE: " << MLE_for_segments[n_elem] << " log: "
				 << log(MLE_for_segments[n_elem]) << " 2nd term: "
				 << -2*log(MLE_for_segments[n_elem]) << endl;
			AIC[n_elem] = 2*AICk-2*log(MLE_for_segments[n_elem]);
			AICc[n_elem] = AIC[n_elem] + 2*AICk*(AICk+1)/(AICn-AICk-1);
		}
		cout << "AIC: " << AIC[n_elem] << " and AICc: " << AICc[n_elem] << endl << endl;

	}

	segments_for_each_n_segments = most_likely_segments;
	max_MLE = MLE_for_segments;
	AIC_of_segments = AIC;
	AICc_of_segments = AICc;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


// this is a function that generates linear segments randomly. Each segment has a
// slope and intercept.
void generate_random_segments(float sigma, int minimum_n_nodes, int mean_segment_length, int segment_range,
							  float dx, float offset_range, float m_range,
							 vector<float>& x_data, vector<float>& y_data,
							 vector<int>& segment_lengths, vector<float>& slope, vector<float>& intercept)
{

	if (segment_range > 2*mean_segment_length)
	{
		cout << "generate_random_segments LSDStatstools.cpp, segment range will result in negative segment lengths" << endl;
		cout << " changing to maximum segment length allowed" << endl;
		segment_range = 2*mean_segment_length -2;
	}
	long seed = time(NULL);

	// set up data vectors
	vector<float> empty_vec;
	vector<float> segment_x_data;
	vector<float> segment_y_data;
	vector<float> all_x_data;
	vector<float> all_y_data;
	vector<int>    nodes_in_segments;
	vector<float> slope_of_segments;
	vector<float> intercept_of_segments;

	// first get the segment lengths
	int total_nodes = 0;
	int this_segment_length;
	while (total_nodes < minimum_n_nodes)
	{
		this_segment_length = int (float(segment_range)*ran3(&seed))
		                      + mean_segment_length-int(0.5*float(segment_range));

		nodes_in_segments.push_back(this_segment_length);
		total_nodes+=this_segment_length;
		cout << "this segment length is: " << this_segment_length << " and total nodes: " << total_nodes << endl;
	}

	// now loop through each segment adding x and y data
	int n_segments = nodes_in_segments.size();
	float old_x = 0;
	float old_y = 0;
	float this_m, this_offset;
	for (int i = 0; i<n_segments; i++)
	{

		segment_x_data = empty_vec;
		segment_y_data = empty_vec;

		if (i == 0)
		{
			segment_x_data.push_back(0.0);
			segment_y_data.push_back(0.0);
		}
		else
		{
			this_offset = ran3(&seed)*offset_range;
			segment_x_data.push_back(old_x+dx);
			segment_y_data.push_back(old_y+this_offset);
			old_x += dx;
			old_y += this_offset;
		}

		// get the slope of this segment
		this_m = ran3(&seed)*m_range;
		slope_of_segments.push_back(this_m);

		// loop through the segment nodes, adding segments as one goes.
		int nodes_this_seg = nodes_in_segments[i];
		for(int node = 1; node<nodes_this_seg; node++)
		{
			segment_x_data.push_back(old_x+dx);
			old_x = old_x+dx;

			segment_y_data.push_back(this_m*dx+old_y);
			old_y = this_m*dx+old_y;
		}

		vector<float> residuals;
		vector<float> lr = simple_linear_regression(segment_x_data, segment_y_data, residuals);
		cout << "Imposed m: " << this_m << " and regressed m: " << lr[0] << " and b: " << lr[1] << endl;
		intercept_of_segments.push_back(lr[1]);

		// add the segment to all_x_data
		for(int node = 0; node<nodes_this_seg; node++)
		{
			all_x_data.push_back(segment_x_data[node]);
			all_y_data.push_back(segment_y_data[node]);
		}


	}

	// now superimpose noise
	cout << endl << endl;
	cout << "n_nodes: " << total_nodes << " and in all_x_data: " << all_x_data.size() << endl;
	for(int node = 0; node<total_nodes; node++)
	{
		all_y_data[node] += sigma*(ran3(&seed)-0.5);
		cout << all_x_data[node] << " " << all_y_data[node] << endl;
	}
	cout << endl << endl;

	cout << endl << endl << "n_segments: " << n_segments << endl;
	for (int i = 0; i< n_segments; i++)
	{
		cout << "segment: " << i << " has " << nodes_in_segments[i]
		     << "  with slope: " << slope_of_segments[i]
		     << " and intercept: " << intercept_of_segments[i] << endl;
	}


	// replace data in vectors passed to function
	x_data = all_x_data;
	y_data = all_y_data;
	slope = slope_of_segments;
	intercept = intercept_of_segments;
	segment_lengths = nodes_in_segments;


}


// this function attepts to find linear segments in a series of x,y data using a brute force method
// it loop through segments of a given length. Right now the only option is to use a fixed number of nodes
// rather than a fixed distance in x space but this can be altered later.
// it then calculates the regression coefficients and the durban-watson statistic on the segments.
// the function creates a vector of the slope, intercept, and durbin watson statistic of the best fit
// these are then scanned once more
// adjacent segments that lie on the same 'true' linear segment will have m and b vlaues that are approximately equal.
// however between segments there will be a transition where the m or b values move gradually to the
// new value. These gradual movments should be the length of a segment. So to see if there are two segments you compare the
// m and b values for segments that are seperated by the number of nodes in the segment to
// see if they are different. Eventally this will involve a statistical test but for now you can just let it
// pass a threshold.
// the code then identifies breakpoints and redoes the calucaltion using the number of identified segments
// one strategy for finding the 'most linear' fit is to loop through decreasing thresholds. The greater the
// threshold, the more segments there should be. So one can preform an Aikake test to see where the improment in
// fit no longer outweighs the increased number of segments.
void find_linear_segments(vector<float>& all_x_data, vector<float>& all_y_data, int segment_length)
{
	int n_data_points = all_x_data.size();				// get the number of data points
	if (segment_length>n_data_points)
	{
		cout << "LSDStatsTools find_linear_segments: your segment length is greater than half the number of data points" << endl;
		cout << "This means that there can only be overlapping segments. Changing segment length to maximum segment length "<< endl;
		segment_length = int(float(n_data_points)/2);
	}

	vector<float> segment_slopes;
	vector<float> segment_intercepts;
	vector<float> segment_r2;
	vector<float> segment_DW_stat;
	vector<float> regression_data;
	vector<float> residuals;

	vector<int> calibrated_segment_start;
	vector<int> calibrated_segment_end;

	float slope_threshold = 0.1;			// the threshold difference in slopes, beyond which
										// a breakpoint is identified
	float intercept_threshold = 0.1;		// the threshold difference in intercept, beyond which
										// a breakpoint is identified
	float slope_difference;			// difference in slope across a span that is the length of the segment
	float intercept_difference;		// difference in intercept across a span that is the length of the segment

	// set up the scanning. The number of regressions will be n_data_points-segment_length+1
	int n_segments = n_data_points - segment_length +1;
	for (int segment = 0; segment<n_segments; segment++)
	{
		// first get segment
		// NOTE there must be a better way to do this using the STL
		// THIS BIT NEEDS TO BE LOOKED AT LATER
		vector<float> segment_x;
		vector<float> segment_y;
		for(int i = 0; i<segment_length; i++)
		{
			segment_x.push_back(all_x_data[segment+i]);
			segment_y.push_back(all_y_data[segment+i]);
		}

		// now do regression on the segments
		regression_data = simple_linear_regression(segment_x, segment_y, residuals);

		// add the regrssion data to the vectors
		segment_slopes.push_back(regression_data[0]);
		segment_intercepts.push_back(regression_data[1]);
		segment_r2.push_back(regression_data[2]);
		segment_DW_stat.push_back(regression_data[3]);

		// print to screen
		cout << segment << " " << regression_data[0] << " " << regression_data[1] << " ";
		cout << regression_data[2] << " " << regression_data[3] << endl;
	}

	// now loop through segments seeing if there is a transition.
	// we look for non overlapping data.
	// the first break point has to be beyond the end of the first segment, so start looking there
	int start = 0;
	for (int segment = segment_length; segment < n_segments; segment++)
	{
		slope_difference = fabs(segment_slopes[segment] - segment_slopes[segment-segment_length]);
		intercept_difference = fabs(segment_intercepts[segment] - segment_intercepts[segment-segment_length]);

		if (slope_difference > slope_threshold || intercept_difference > intercept_threshold)
		{
			calibrated_segment_start.push_back(start);
			calibrated_segment_end.push_back(segment-1);
			start = segment;
		}
	}
	calibrated_segment_start.push_back(start);
	calibrated_segment_end.push_back(n_segments-1);

	vector<float> calibrated_segment_slopes;
	vector<float> calibrated_segment_intercepts;
	vector<float> calibrated_segment_r2;
	vector<float> calibrated_segment_DW_stat;

	// get the number of calibrated segments
	int n_calibrated_segments = calibrated_segment_start.size();

	// now get these segements and do regression on each one
	for (int calib_segment = 0; calib_segment<n_calibrated_segments; calib_segment++)
	{
		vector<float> calib_x;
		vector<float> calib_y;

		int calib_start = calibrated_segment_start[calib_segment];
		int calib_end =  calibrated_segment_end[calib_segment];

		for (int i = calib_start; i<=calib_end; i++)
		{
			calib_x.push_back(all_x_data[i]);
			calib_y.push_back(all_y_data[i]);
		}

		regression_data = simple_linear_regression(calib_x, calib_y, residuals);
		calibrated_segment_slopes.push_back(regression_data[0]);
		calibrated_segment_intercepts.push_back(regression_data[1]);
		calibrated_segment_r2.push_back(regression_data[2]);
		calibrated_segment_DW_stat.push_back(regression_data[3]);

		// print to screen
		cout << endl << "and now for the claibrated segments" << endl;
		cout << calib_segment << " " << regression_data[0] << " " << regression_data[1] << " ";
		cout << regression_data[2] << " " << regression_data[3] << endl;
	}


}


// get the least squared maximum likelihood estimator
float calculate_MLE(vector<float>& measured, vector<float>& modelled, vector<float>& sigma)
{
	// get the number of samples
	int n_samples = measured.size();
	float MLE_tot = 1;
	for (int i = 0; i<n_samples; i++)
	{
		//cout << "exp term: " << -0.5*(measured[i]-modelled[i])*(measured[i]-modelled[i])/
		//							 sigma[i]*sigma[i] << endl;
		MLE_tot = MLE_tot*exp(-0.5*(measured[i]-modelled[i])*(measured[i]-modelled[i])/
									 (sigma[i]*sigma[i]));
	}
	return MLE_tot;
}

// get the least squared maximum likelihood estimator
float calculate_MLE(vector<float>& measured, vector<float>& modelled, float sigma)
{
	// get the number of samples
	int n_samples = measured.size();
	float MLE_tot = 1;
	for (int i = 0; i<n_samples; i++)
	{
		MLE_tot = MLE_tot*exp(-0.5* (measured[i]-modelled[i])*(measured[i]-modelled[i])/
									 (sigma*sigma));
	}
	return MLE_tot;
}

// get the least squared maximum likelihood estimator based on residuals
float calculate_MLE_from_residuals(vector<float>& residuals, float sigma)
{
	//cout << "sigma is: " << sigma << endl;

	// get the number of samples
	int n_samples = residuals.size();
	float MLE_tot = 1;
	for (int i = 0; i<n_samples; i++)
	{
		MLE_tot = MLE_tot*exp(-0.5* (residuals[i]*residuals[i])/
									 (sigma*sigma));
	}
	return MLE_tot;
}

string itoa(int num)
{
    stringstream converter;
    converter << num;
    return converter.str();
}

string dtoa(float num)
{
    stringstream converter;
    converter << num;
    return converter.str();
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// LOG BINNING OF 2D ARRAY
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Takes 2D arrays for two corresponding variables (e.g. drainage area and slope)
// and sorts into logarithmically spaced bins of a specified bin width.
// The inputs are:
//    - InputArrayX = the independent variable (usually plotted on the x axis)
//    - InputArrayY = the dependent variable (usually plotted on the y axis)
//    - log_bin_width = the width (in log space) of the bins, with respect to
//          InputArrayX
//  The outputs are:
//  FC 13/11/12; corrected and modified by DM 04/12/12. Now generalised to be
//  appropriate for any data, nor just slope-area
//
// Original version of this module produced a bad_alloc memory error. Now it has been
// re-ported from the working LSDRaster original version and has now been tested and
// produces the same results as the original with no memory errors. - SWDG 30/8/13
//
// Updated to return the number of values per bin in a vector by reference - SWDG 11/11/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void log_bin_data(Array2D<float>& InputArrayX, Array2D<float>& InputArrayY, float log_bin_width, vector<float>&  MeanX_output, vector<float>& MeanY_output,
                      vector<float>& midpoints_output, vector<float>& StandardDeviationX_output, vector<float>& StandardDeviationY_output,
                      vector<float>& StandardErrorX_output, vector<float>& StandardErrorY_output, vector<int>& num_observations, float NoDataValue)
{

  int NRows = InputArrayX.dim1();
  int NCols = InputArrayX.dim2();

  // Finding max contributing area to use as upper limit for the bins
  float max_X = 0;
	float min_X = 0;
	for (int row = 0; row < NRows; row++)
	{
    for(int col = 0; col < NCols; col++)
    {
      if (InputArrayX[row][col] > max_X)
      {
        max_X = InputArrayX[row][col];
      }
      if (InputArrayX[row][col] < min_X || min_X ==0)    // Cannot have take a logarithm of zero.
      {
        min_X = InputArrayX[row][col];
      }
    }
  }
  
  // Defining the upper limit, lower limit and width of the bins
  float upper_limit = ceil((log10(max_X)/log_bin_width))*log_bin_width;
  float lower_limit;
  if (min_X >= 1)
  {
    lower_limit = floor((log10(min_X)/log_bin_width))*log_bin_width;
  }
  else
  {
    lower_limit = 0;
  }
  int NBins = int(((upper_limit - lower_limit)/log_bin_width) + 1); 
  
  // Looping through all the rows and columns and calculating which bin the
  // contributing area is in, and putting the slope in this bin

  vector<int> number_observations(NBins,0);
  vector<float> Y_data(NBins,0.0);
  vector<float> X_data(NBins,0.0);

  // These will be copied into their respective function output vectors
  vector<float> MeanX(NBins,0.0);
	vector<float> MeanY(NBins,0.0);
  vector<float> mid_points(NBins,NoDataValue);

  // vector<vector> objects house data in each bin.
  vector< vector<float> > binned_data_X;
  vector< vector<float> > binned_data_Y;

  	// create the vector of vectors.  Nested vectors will store data within that
  // bin.
  vector<float> empty_vector;
  for(int i = 0; i<NBins; i++)
  {
	  binned_data_X.push_back(empty_vector);
	  binned_data_Y.push_back(empty_vector);
  }

  // Bin Data into logarithmically spaced bins
  for (int row = 0; row < NRows; row++)
  {
    for (int col = 0; col < NCols; col++)
    {
      float Y = InputArrayY[row][col];

      if (Y != NoDataValue)
      {
        float X = InputArrayX[row][col];
        // Get bin_id for this particular value of X
        int bin_id = int(((log10(X))-lower_limit)/log_bin_width);
        if (bin_id < 0)
        {
          bin_id = 0;
        }  
        // Store X and corresponding Y into this bin, for their respective
        // vector<vector> object
        binned_data_X[bin_id].push_back(X); 
        binned_data_Y[bin_id].push_back(Y);
        Y_data[bin_id] += Y;
        X_data[bin_id] += X;
        ++number_observations[bin_id]; 
      }
    }
  }
   
  // Calculating the midpoint in x direction of each bin and the mean of x and y
  // in each bin.  Probably want to plot MeanX vs MeanY, rather than midpoint of
  // x vs Mean Y to be most robust.  At the moment the program returns both.
  float midpoint_value = lower_limit + log_bin_width/2;
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
    mid_points[bin_id] = midpoint_value;
    midpoint_value = midpoint_value + log_bin_width;
    if (number_observations[bin_id] != 0)
    {
      MeanY[bin_id] = Y_data[bin_id]/number_observations[bin_id];
      MeanX[bin_id] = X_data[bin_id]/number_observations[bin_id];
    }
  }
  
  // These will be copied into their respective function output vectors
  vector<float> StandardDeviationX(NBins,0.0);
  vector<float> StandardDeviationY(NBins,0.0);
  vector<float> StandardErrorX(NBins,0.0);
  vector<float> StandardErrorY(NBins,0.0);

  // iterators to move through vec<vec>
	vector<float>::iterator vec_iterator_X;
  vector<float>::iterator vec_iterator_Y;

  // Getting the standard deviation of each bin.  First get sum of the squared
  // deviations from the mean
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
	  if (number_observations[bin_id] != 0)
    {
		  // for the independent variable X...
      vec_iterator_X = binned_data_X[bin_id].begin();
		  while (vec_iterator_X != binned_data_X[bin_id].end())
		  {
			  float Xi = (*vec_iterator_X);
        StandardDeviationX[bin_id] += (Xi - MeanX[bin_id]) * (Xi - MeanX[bin_id]);
        vec_iterator_X++;
      }

      // ...and for the dependent variable Y
      vec_iterator_Y = binned_data_Y[bin_id].begin();
		  while (vec_iterator_Y != binned_data_Y[bin_id].end())
		  {
			  float Yi = (*vec_iterator_Y);
        StandardDeviationY[bin_id] += (Yi - MeanY[bin_id]) * (Yi - MeanY[bin_id]);
        vec_iterator_Y++;
      }
    }
  }
  // Finally, divide by number of observations in each bin then square root
  // to give standard deviation within the bin.
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
    if (number_observations[bin_id] != 0)
    {
      StandardDeviationX[bin_id] = sqrt(StandardDeviationX[bin_id]/number_observations[bin_id]);
      StandardDeviationY[bin_id] = sqrt(StandardDeviationY[bin_id]/number_observations[bin_id]);
      StandardErrorX[bin_id] = StandardDeviationX[bin_id]/sqrt(number_observations[bin_id]);
      StandardErrorY[bin_id] = StandardDeviationY[bin_id]/sqrt(number_observations[bin_id]);
    }
  } 
  // Copy output into output vectors
  MeanX_output = MeanX;
  MeanY_output = MeanY;
  midpoints_output = mid_points;
  StandardDeviationX_output = StandardDeviationX;
  StandardDeviationY_output = StandardDeviationY;
  StandardErrorX_output = StandardErrorX;
  StandardErrorY_output = StandardErrorY;
  num_observations = number_observations;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// LOG BINNING OF 1D VECTOR
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Takes vectors for two corresponding variables (e.g. drainage area and slope)
// and sorts into logarithmically spaced bins of a specified bin width.
// The inputs are:
//    - InputVectorX = the independent variable (usually plotted on the x axis)
//    - InputVectorY = the dependent variable (usually plotted on the y axis)
//    - log_bin_width = the width (in log space) of the bins, with respect to
//          InputArrayX
//  The outputs are:
// FC 13/11/12; modified by DM 04/12/12
//
//
// THIS HAS NOT BEEN TESTED AND MAY NEED RE-PORTED FROM THE LSDRASTER ORIGINAL.
// SEE THE OTHER LOG BINNING COMMENTS FOR DETAILS -- SWDG 30/8/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void log_bin_data(vector<float>& InputVectorX, vector<float>& InputVectorY, float log_bin_width,
                  vector<float>&  MeanX_output, vector<float>& MeanY_output,
                      vector<float>& midpoints_output, vector<float>&  StandardDeviationX_output,
                      vector<float>&  StandardDeviationY_output,int NoDataValue)
{
  
	int n_data = InputVectorY.size();
  float max_X = InputVectorX[n_data-1];
	float min_X = InputVectorX[1];

	//cout << "LSDStatsTools line 1757, n_data_X: " << n_data << " and Y: " << InputVectorX.size() << endl;

  for (int i = 0; i < n_data; ++i)
	{
    if (InputVectorX[i] > max_X)
    {
      max_X = InputVectorX[i];
    }
    if (InputVectorX[i] < min_X || min_X == 0)    // Cannot have take a logarithm of zero.
    {
      min_X = InputVectorX[i];
    }
  }

  // Defining the upper limit, lower limit and width of the bins
  float upper_limit = ceil((log10(max_X)/log_bin_width))*log_bin_width;
  float lower_limit = floor((log10(min_X)/log_bin_width))*log_bin_width;
  int NBins = int( (upper_limit - lower_limit)/log_bin_width )+1;

  // Looping through all the rows and columns and calculating which bin the
  // contributing area is in, and putting the slope in this bin
  vector<int> number_observations(NBins,0);
  vector<float> Y_data(NBins,0.0);
  vector<float> X_data(NBins,0.0);

  // These will be copied into their respective function output vectors
  vector<float> MeanX(NBins,0.0);
	vector<float> MeanY(NBins,0.0);
  vector<float> mid_points(NBins,NoDataValue);

  // vector<vector> objects house data in each bin.
  vector< vector<float> > binned_data_X;
  vector< vector<float> > binned_data_Y;

	// create the vector of vectors.  Nested vectors will store data within that
  // bin.
  vector<float> empty_vector;
  for(int i = 0; i<NBins; i++)
  {
	  binned_data_X.push_back(empty_vector);
	  binned_data_Y.push_back(empty_vector);
  }

  // Bin Data into logarithmically spaced bins
  for (int i = 0; i < n_data; ++i)
  {
    float Y = InputVectorY[i];
    if (Y != NoDataValue)
    {
      float X = InputVectorX[i];
      if (X > 0)
      {
        // Get bin_id for this particular value of X
        int bin_id = int(((log10(X))-lower_limit)/log_bin_width);

		//cout << "LINE 1818, bin id: " << bin_id << " i: " << i << " XDsz: " << X_data.size() << " YDsz: " << Y_data.size() << endl;
		//cout << "LINE 1819, bdxsz: " << binned_data_X.size() << " bdysz: " << binned_data_Y.size() << endl << endl;
        // Store X and corresponding Y into this bin, for their respective
        // vector<vector> object
        binned_data_X[bin_id].push_back(X);
        binned_data_Y[bin_id].push_back(Y);
        Y_data[bin_id] += Y;
        X_data[bin_id] += X;
        ++number_observations[bin_id];
      }
    }
  }


  // Calculating the midpoint in x direction of each bin and the mean of x and y
  // in each bin.  Probably want to plot MeanX vs MeanY, rather than midpoint of
  // x vs Mean Y to be most robust.  At the moment the program returns both.
  float midpoint_value = lower_limit + log_bin_width/2;
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
    mid_points[bin_id] = midpoint_value;
    midpoint_value = midpoint_value + log_bin_width;
    if (number_observations[bin_id] != 0)
    {
      MeanY[bin_id] = Y_data[bin_id]/number_observations[bin_id];
      MeanX[bin_id] = X_data[bin_id]/number_observations[bin_id];
    }
  }


  // These will be copied into their respective function output vectors
  vector<float> StandardDeviationX(NBins,0.0);
  vector<float> StandardDeviationY(NBins,0.0);
  
  // iterators to move through vec<vec>
	vector<float>::iterator vec_iterator_X;
  vector<float>::iterator vec_iterator_Y;

  // Getting the standard deviation of each bin.  First get sum of the squared
  // deviations from the mean
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
	  if (number_observations[bin_id] != 0)
    {
		  // for the independent variable X...
      vec_iterator_X = binned_data_X[bin_id].begin();

      while (vec_iterator_X != binned_data_X[bin_id].end())
		  {
        float Xi = (*vec_iterator_X);
        StandardDeviationX[bin_id] += (Xi - MeanX[bin_id]) * (Xi - MeanX[bin_id]);
        vec_iterator_X++;
      }

      // ...and for the dependent variable Y
      vec_iterator_Y = binned_data_Y[bin_id].begin();
      
		  while (vec_iterator_Y != binned_data_Y[bin_id].end())
		  {
			  float Yi = (*vec_iterator_Y);
        StandardDeviationY[bin_id] += (Yi - MeanY[bin_id]) * (Yi - MeanY[bin_id]);
        vec_iterator_Y++;
      }
    }
  }

  // Finally, divide by number of observations in each bin then square root
  // to give standard deviation within the bin.
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
    if (number_observations[bin_id] != 0)
    {
      StandardDeviationX[bin_id] = sqrt(StandardDeviationX[bin_id]/number_observations[bin_id]);
      StandardDeviationY[bin_id] = sqrt(StandardDeviationY[bin_id]/number_observations[bin_id]);
    }
  }

  // Copy output into output vectors
  MeanX_output = MeanX;
  MeanY_output = MeanY;
  midpoints_output = mid_points;
  StandardDeviationX_output = StandardDeviationX;
  StandardDeviationY_output = StandardDeviationY;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// BINNING OF 1D VECTOR 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Takes vectors for two corresponding variables (e.g. drainage area and slope)
// and sorts into bins of a specified bin width.
// The inputs are:
//    - InputVectorX = the independent variable (usually plotted on the x axis)
//    - InputVectorY = the dependent variable (usually plotted on the y axis)
//    - bin_width = the width of the bins, with respect to
//          InputArrayX
//  The outputs are:
//FC 13/11/12; modified by DM 04/12/12
// WARNING - will not work on vectors with negative values (e.g. hilltop curvature). If using
// vector of negative values take the absolute values before passing to function.
//
// Modified by FC 30/09/13 to calculate the range of the 95th percentile for each bin - 
// used for channel head prediction through chi segment fitting.  Changed from log binning to
// regular binning.
// Modified by FC 13/01/13 to calculate the median of each bin and return the standard error.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void bin_data(vector<float>& InputVectorX, vector<float>& InputVectorY, float bin_width,
                  vector<float>&  MeanX_output, vector<float>& MeanY_output,
                  vector<float>& midpoints_output, vector<float>& MedianY_output, 
                  vector<float>&  StandardDeviationX_output,vector<float>&  StandardDeviationY_output,
                  vector<float>& StandardErrorX_output, vector<float>& StandardErrorY_output, vector<int>& number_observations_output, 
                  float& bin_lower_limit, float NoDataValue)
{

	// Finding max contributing area to use as upper limit for the bins
	int n_data = InputVectorY.size();
  float max_X = InputVectorX[n_data-1];
	float min_X = InputVectorX[1];

	cout << "LSDStatsTools line 1757, n_data_X: " << InputVectorX.size() << " and Y: " << InputVectorY.size() << endl;

  for (int i = 0; i < n_data; ++i)
	{
    if (InputVectorX[i] > max_X)
    {
      max_X = InputVectorX[i];
    }
    if (InputVectorX[i] < min_X || min_X == 0)    // Cannot have take a logarithm of zero.
    {
      min_X = InputVectorX[i];
    }
  }

  // Defining the upper limit, lower limit and width of the bins
  float upper_limit = ceil(max_X/bin_width)*bin_width;
  float lower_limit = floor(min_X/bin_width)*bin_width;
  if (lower_limit < 0 || isnan(lower_limit) == 1)
  { 
    lower_limit = 0;
  }
  int NBins = int( (upper_limit - lower_limit)/bin_width )+1;
  cout << "Upper limit: " << upper_limit << " Lower limit: " << lower_limit << " NBins: " << NBins << endl;

  // Looping through all the rows and columns and calculating which bin the
  // contributing area is in, and putting the slope in this bin
  vector<int> number_observations(NBins,0);
  vector<float> Y_data(NBins,0.0);
  vector<float> X_data(NBins,0.0);

  // These will be copied into their respective function output vectors
  vector<float> MeanX(NBins,0.0);
	vector<float> MeanY(NBins,0.0);
  vector<float> mid_points(NBins,NoDataValue);

  // vector<vector> objects house data in each bin.
  vector< vector<float> > binned_data_X;
  vector< vector<float> > binned_data_Y;

	// create the vector of vectors.  Nested vectors will store data within that
  // bin.
  vector<float> empty_vector;
  for(int i = 0; i<NBins; i++)
  {
	  binned_data_X.push_back(empty_vector);
	  binned_data_Y.push_back(empty_vector);
  }

  // Bin Data into logarithmically spaced bins
  for (int i = 0; i < n_data; ++i)
  {
    float Y = InputVectorY[i];
    if (Y != NoDataValue)
    {
      float X = InputVectorX[i];
      if (X != 0)
      {
        // Get bin_id for this particular value of X
        int bin_id = int((X-lower_limit)/bin_width);
        //cout << "X: " << X << " Y: " << Y << " bin_id: " << bin_id << endl;
        if (bin_id >= 0)
        {
		      //cout << "LINE 1818, bin id: " << bin_id << " i: " << i << " XDsz: " << X_data.size() << " YDsz: " << Y_data.size() << endl;
		      //cout << "LINE 1819, bdxsz: " << binned_data_X.size() << " bdysz: " << binned_data_Y.size() << endl << endl;
          // Store X and corresponding Y into this bin, for their respective
          // vector<vector> object
          binned_data_X[bin_id].push_back(X);
          binned_data_Y[bin_id].push_back(Y);
          Y_data[bin_id] += Y;
          X_data[bin_id] += X;
          ++number_observations[bin_id];
        }
      }
    }
  }


  // Calculating the midpoint in x direction of each bin and the mean of x and y
  // in each bin.  Probably want to plot MeanX vs MeanY, rather than midpoint of
  // x vs Mean Y to be most robust.  At the moment the program returns both.
  float midpoint_value = lower_limit + bin_width/2;
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
    mid_points[bin_id] = midpoint_value;
    midpoint_value = midpoint_value + bin_width;
    if (number_observations[bin_id] != 0)
    {
      MeanY[bin_id] = Y_data[bin_id]/number_observations[bin_id];
      MeanX[bin_id] = X_data[bin_id]/number_observations[bin_id];
      //cout << "No observations in bin: " << number_observations[bin_id] << endl;
    }
  }


  // These will be copied into their respective function output vectors
  vector<float> StandardDeviationX(NBins,0.0);
  vector<float> StandardDeviationY(NBins,0.0);
  vector<float> YDataVector;
  vector<float> StandardErrorX(NBins,0.0);
  vector<float> StandardErrorY(NBins,0.0);
  vector<float> MedianY(NBins,0.0);
  // iterators to move through vec<vec>
	vector<float>::iterator vec_iterator_X;
  vector<float>::iterator vec_iterator_Y;

  // Getting the standard deviation of each bin.  First get sum of the squared
  // deviations from the mean
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
	  if (number_observations[bin_id] != 0)
    {
		  // for the independent variable X...
      vec_iterator_X = binned_data_X[bin_id].begin();

      while (vec_iterator_X != binned_data_X[bin_id].end())
		  {
        float Xi = (*vec_iterator_X);
        StandardDeviationX[bin_id] += (Xi - MeanX[bin_id]) * (Xi - MeanX[bin_id]);
        vec_iterator_X++;
      }

      // ...and for the dependent variable Y
      vec_iterator_Y = binned_data_Y[bin_id].begin();
      
		  while (vec_iterator_Y != binned_data_Y[bin_id].end())
		  {
			  float Yi = (*vec_iterator_Y);
        StandardDeviationY[bin_id] += (Yi - MeanY[bin_id]) * (Yi - MeanY[bin_id]);
        vec_iterator_Y++;
        YDataVector.push_back(Yi);
      }
      
      //find the median of the dependent variable Y
      sort(YDataVector.begin(), YDataVector.end());
      int YDataSize = YDataVector.size();
      MedianY.push_back(YDataVector[floor(YDataSize/2)]);     
    }
  }

  // Finally, divide by number of observations in each bin then square root
  // to give standard deviation within the bin.
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
    if (number_observations[bin_id] != 0)
    {
      StandardDeviationX[bin_id] = sqrt(StandardDeviationX[bin_id]/number_observations[bin_id]);
      StandardDeviationY[bin_id] = sqrt(StandardDeviationY[bin_id]/number_observations[bin_id]);
      StandardErrorX[bin_id] = StandardDeviationX[bin_id]/sqrt(number_observations[bin_id]);
      StandardErrorY[bin_id] = StandardDeviationY[bin_id]/sqrt(number_observations[bin_id]);
    }
  }

  // Copy output into output vectors
  MeanX_output = MeanX;
  MeanY_output = MeanY;
  midpoints_output = mid_points;
  MedianY_output = MedianY;
  StandardDeviationX_output = StandardDeviationX;
  StandardDeviationY_output = StandardDeviationY;
  StandardErrorX_output = StandardErrorX;
  StandardErrorY_output = StandardErrorY;
  number_observations_output = number_observations;
}


//look for bins with very few (or no) data points output from the log binning function and removes them to avoid 
//plotting several empty bins at 0,0 in some cases. 
//pass in a threshold fraction *above* which all bins will be kept. Pass in 0 to remove only empty bins. 
//SWDG 6/11/13
void RemoveSmallBins(vector<float>& MeanX_output, vector<float>& MeanY_output, vector<float>& midpoints_output,
                     vector<float>& StandardDeviationX_output, vector<float>& StandardDeviationY_output,
                     vector<float>& StandardErrorX_output, vector<float>& StandardErrorY_output, vector<int>& number_observations, float bin_threshold){
                                                                                      
  // temp vectors to store all the kept data
  vector<float> MeanX_output_temp;
  vector<float> MeanY_output_temp;
  vector<float> midpoints_output_temp;
  vector<float> StandardDeviationX_output_temp;
  vector<float> StandardDeviationY_output_temp;
  vector<float> StandardErrorX_output_temp;
  vector<float> StandardErrorY_output_temp;
  vector<int> number_observations_temp;

  //Get total number of measurements across all bins
  float TotalNoOfMesurements = 0;
  for (int k = 0; k < int(number_observations.size()); ++k){
    TotalNoOfMesurements += number_observations[k];
  }

  // loop over all the elements and make a copy of all kept values
  for(int q = 0; q < int(MeanX_output.size()); ++q){
    if (number_observations[q]/TotalNoOfMesurements > bin_threshold){
      MeanX_output_temp.push_back(MeanX_output[q]);
      MeanY_output_temp.push_back(MeanY_output[q]);
      midpoints_output_temp.push_back(midpoints_output[q]);
      StandardDeviationX_output_temp.push_back(StandardDeviationX_output[q]);
      StandardDeviationY_output_temp.push_back(StandardDeviationY_output[q]);
      StandardErrorX_output_temp.push_back(StandardErrorX_output[q]);
      StandardErrorY_output_temp.push_back(StandardErrorY_output[q]);
      number_observations_temp.push_back(number_observations[q]);     
    }
  }

  //delete all the elements from the original vectors
  MeanX_output.clear();
  MeanY_output.clear();
  midpoints_output.clear();
  StandardDeviationX_output.clear();
  StandardDeviationY_output.clear();
  StandardErrorX_output.clear();
  StandardErrorY_output.clear();
  number_observations.clear();

  // swap temp data which contains no zeros back into the original emptied 
  // vectors, so they can be returned by reference.
  MeanX_output.swap(MeanX_output_temp);
  MeanY_output.swap(MeanY_output_temp);
  midpoints_output.swap(midpoints_output_temp);
  StandardDeviationX_output.swap(StandardDeviationX_output_temp);
  StandardDeviationY_output.swap(StandardDeviationY_output_temp);
  StandardErrorX_output.swap(StandardErrorX_output_temp);
  StandardErrorY_output.swap(StandardErrorY_output_temp);
  number_observations.swap(number_observations_temp);

}

// 
// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// // calculate_histogram
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Takes a raster and condenses it into a histogram
// DTM 20/11/2013
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void print_histogram(vector<float> input_values, float bin_width, string filename)
{
	// Finding max contributing area to use as upper limit for the bins
  int n_data = input_values.size();
  float max_X = input_values[0];
	float min_X = input_values[0];

  // Loop through to find the min and max of the dataset
  for (int i = 0; i < n_data; ++i)
	{
    if (input_values[i] > max_X)
    {
      max_X = input_values[i];
    }
    if (input_values[i] < min_X)    
    {
      min_X = input_values[i];
    }
  }
//  cout << min_X << " " << max_X << endl;
  // Defining the upper limit, lower limit and the bin width.
  // Extend range by one bin at each end so that the histogram is bounded by 
  // zeros for plotting
  float upper_limit = (ceil(max_X/bin_width)+1)*bin_width;
  float lower_limit = (floor(min_X/bin_width)-1)*bin_width;
  int NBins = int( (upper_limit - lower_limit)/bin_width )+1;

  // Looping through all the rows and columns and calculating which bin the
  // contributing area is in, and putting the slope in this bin
  vector<int> number_observations(NBins,0);
  vector<float> bin_midpoints(NBins,0.0);
  vector<float> bin_lower_lim(NBins,0.0);
  vector<float> bin_upper_lim(NBins,0.0);
  vector<float> probability_density(NBins,0);

	// create the vector of vectors.  Nested vectors will store data within that
  // bin.
  vector<float> empty_vector;
  float midpoint_value, lower_lim_value, upper_lim_value;

  // Bin Data
  int n_nan=0;
  for (int i = 0; i < n_data; ++i)
  {
    float X = input_values[i];
    // Get bin_id for this particular value of X
    int bin_id = int((X-lower_limit)/bin_width);
    if(input_values[i]!=input_values[i])
    {
      cout << "FOUND NAN - skipping" << endl;
      ++n_nan;
    }
    else ++number_observations[bin_id];
  }
  for(int i = 0; i<NBins; i++)
  {
    midpoint_value = lower_limit + (float(i)+0.5)*bin_width;
    lower_lim_value = lower_limit + float(i)*bin_width;
    upper_lim_value = float(i+1)*bin_width;  
    
    bin_midpoints[i]= midpoint_value;
    bin_lower_lim[i]= lower_lim_value;
    bin_upper_lim[i]= upper_lim_value;
    
    probability_density[i] = number_observations[i]/float(n_data-n_nan);    
  }

  // Print histogram to file
  cout << "\t printing histogram to " << filename << endl;
  ofstream ofs;
  ofs.open(filename.c_str());
  
  if(ofs.fail())
  {
    cout << "\nFATAL ERROR: unable to write output_file" << endl;
		exit(EXIT_FAILURE);
  }
  
  ofs << "Midpoint LowerLim UpperLim Count ProbabilityDensity\n";
  for(int i = 0; i < NBins; ++i)
  {
    ofs << bin_midpoints[i] << " " << bin_lower_lim[i] << " " << bin_upper_lim[i] << " " << number_observations[i] << " " << probability_density[i] << "\n";
  }
  
  ofs.close();

}
 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// bin_data
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This is a much simpler version of the binning software.  It takes two vectors, and
// sorts the values held within the first vector into bins according to their respective
// values in the second vector.  The output is a vector<vector> with the binned dataset.
// and a vector of bin midpoints.  These can then be analysed ahd plotted as desired.
// DTM 14/04/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void bin_data(vector<float>& vector1, vector<float>& vector2, float min, float max, float bin_width, vector<float>& mid_points, vector< vector<float> >& binned_data)
{
	
  // Defining the upper limit, lower limit and width of the bins
  float upper_limit = ceil(max/bin_width)*bin_width;
  float lower_limit = floor(min/bin_width)*bin_width;
  int NBins = int( (upper_limit - lower_limit)/bin_width );//+1;
  vector<float> empty_vector;
  vector<float> mid_points_temp;
  vector< vector<float> > binned_data_temp(NBins,empty_vector);

  // Bin Data from vector1 according to values in vector2
  for (int i = 0; i < int(vector1.size()); ++i)
  {
    if((vector2[i] >= min) && (vector2[i] <= max))
    {
      // Get bin_id from vector 2
      int bin_id = int((vector2[i]-lower_limit)/bin_width);
      if (bin_id >= 0)
      {
        // Store value from vector1 into this bin
        binned_data_temp[bin_id].push_back(vector1[i]);
      }
    }
  }

  // Calculating the midpoint in x direction of each bin and the mean of x and y
  // in each bin.  Probably want to plot MeanX vs MeanY, rather than midpoint of
  // x vs Mean Y to be most robust.  At the moment the program returns both.
  float midpoint_value = lower_limit + bin_width/2;
  for (int i = 0; i < NBins; i++)
  {
    midpoint_value = lower_limit + (float(i)+0.5)*bin_width;
    mid_points_temp.push_back(midpoint_value);
  }
  mid_points = mid_points_temp;
  binned_data = binned_data_temp;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// log_bin_data
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This is a much simpler version of the log-binning software.  It takes two vectors, and
// sorts the values held within the first vector into bins according to their respective
// values in the second vector.  The output is a vector<vector> with the binned dataset.
// and a vector of bin midpoints.  These can then be analysed ahd plotted as desired.
// DTM 30/10/2014
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void log_bin_data(vector<float>& vector1, vector<float>& vector2, float log_bin_width, vector<float>& bin_mid_points, vector<float>& bin_vector1_mean, vector<float>& bin_vector2_mean, vector< vector<float> >& binned_data, const float NoDataValue)
{
	int n_data = vector1.size();
  float max_X = vector2[n_data-1];
	float min_X = vector2[1];
  for (int i = 0; i < n_data; ++i)
	{
    if (vector2[i] > max_X)
    {
      max_X = vector2[i];
    }
    if (vector2[i] < min_X || min_X == 0)    // Cannot have take a logarithm of zero.
    {
      min_X = vector2[i];
    }
  }
  // Defining the upper limit, lower limit and width of the bins
  float upper_limit = ceil((log10(max_X)/log_bin_width))*log_bin_width;
  float lower_limit = floor((log10(min_X)/log_bin_width))*log_bin_width;
  int NBins = int( (upper_limit - lower_limit)/log_bin_width )+1;
  // These will be copied into their respective function output vectors
	vector<float> mid_points(NBins,NoDataValue);
  vector<float> binned_vector1_mean(NBins,NoDataValue);
  vector<float> binned_vector2_mean(NBins,NoDataValue);
  vector< vector<float> > binned_vector1;
  vector< vector<float> > binned_vector2;
	// create the vector of vectors.  Nested vectors will store data within that
  // bin.
  vector<float> empty_vector;
  for(int i = 0; i<NBins; i++)
  {
	  binned_vector1.push_back(empty_vector);
	  binned_vector2.push_back(empty_vector);
  }
  // Bin Data into logarithmically spaced bins
  for (int i = 0; i < n_data; ++i)
  {
    if (vector1[i] != NoDataValue)
    {
      if (vector2[i] > 0 && vector1[i] > 0)
      {
        // Get bin_id for this particular value of X
        int bin_id = int(((log10(vector2[i]))-lower_limit)/log_bin_width);

        // Store X and corresponding Y into this bin, for their respective
        // vector<vector> object
        binned_vector1[bin_id].push_back(vector1[i]);
        binned_vector2[bin_id].push_back(vector2[i]);
      }
    }
  }
  // Calculating the midpoint in x direction of each bin and the mean of x and y
  // in each bin.  Probably want to plot MeanX vs MeanY, rather than midpoint of
  // x vs Mean Y to be most robust.  At the moment the program returns both.
  float midpoint_value = lower_limit + log_bin_width/2;
  for (int bin_id = 0; bin_id < NBins; bin_id++)
  {
    mid_points[bin_id] = midpoint_value;
    midpoint_value = midpoint_value + log_bin_width;
    if (binned_vector1[bin_id].size() != 0)
    {
      binned_vector1_mean[bin_id] = get_mean(binned_vector1[bin_id]);
      binned_vector2_mean[bin_id] = get_mean(binned_vector2[bin_id]);
    }
    else 
    {
      binned_vector1_mean[bin_id] = NoDataValue;
      binned_vector2_mean[bin_id] = NoDataValue;
    }
  }
  // Copy output into output vectors
  bin_mid_points = mid_points;
	binned_data = binned_vector1;
	bin_vector1_mean = binned_vector1_mean;
	bin_vector2_mean = binned_vector2_mean;
	cout << "\t\t\t\t\t\t log binning complete!" << endl;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// SORT MODULE (required for constructing radial frequency)  S.M.Mudd
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
///@brief Comparison struct used by sort.
///@details http://bytes.com/topic/c/answers/132045-sort-get-index
template<class T> struct index_cmp
{
  /// Comparison index.
  index_cmp(const T arr) : arr(arr) {}
  /// Comparison operator.
  bool operator()(const size_t a, const size_t b) const
  {
    return arr[a] < arr[b];
  }
  /// Array.
  const T arr;
};

// This implementation is O(n), but also uses O(n) extra memory
void matlab_float_reorder(std::vector<float> & unordered, std::vector<size_t> const & index_map, std::vector<float> & ordered)
{
  // copy for the reorder according to index_map, because unsorted may also be
  // sorted
  vector<float> copy = unordered;
  ordered.resize(index_map.size());
  for(int i = 0; i< int(index_map.size());i++)
  {
    ordered[i] = copy[index_map[i]];
  }
}

void matlab_float_sort(vector<float>& unsorted, vector<float>& sorted, vector<size_t>& index_map)
{
  // Original unsorted index map
  index_map.resize(unsorted.size());
  for(size_t i=0;i<unsorted.size();i++)
  {
    index_map[i] = i;
  }
  // Sort the index map, using unsorted for comparison
  sort(index_map.begin(), index_map.end(), index_cmp<std::vector<float>& >(unsorted));
  sorted.resize(unsorted.size());
  matlab_float_reorder(unsorted,index_map,sorted);
}

void matlab_int_sort(vector<int>& unsorted, vector<int>& sorted, vector<size_t>& index_map)
{
  // Original unsorted index map
  index_map.resize(unsorted.size());
  for(size_t i=0;i<unsorted.size();i++)
  {
    index_map[i] = i;
  }
  // Sort the index map, using unsorted for comparison
  sort(index_map.begin(), index_map.end(), index_cmp<std::vector<int>& >(unsorted));
  sorted.resize(unsorted.size());
  matlab_int_reorder(unsorted,index_map,sorted);
}

void matlab_float_sort_descending(vector<float>& unsorted, vector<float>& sorted, vector<size_t>& index_map)
{
  // Original unsorted index map
  index_map.resize(unsorted.size());
  for(size_t i=0;i<unsorted.size();i++)
  {
    index_map[i] = i;
  }
  // Sort the index map, using unsorted for comparison
  // uses reverse iterators to sort descending - SWDG 16/4/13
  sort(index_map.rbegin(), index_map.rend(), index_cmp<std::vector<float>& >(unsorted));
  sorted.resize(unsorted.size());
  matlab_float_reorder(unsorted,index_map,sorted);
}

// This implementation is O(n), but also uses O(n) extra memory
void matlab_int_reorder(std::vector<int> & unordered, std::vector<size_t> const & index_map, std::vector<int> & ordered)
{
  // copy for the reorder according to index_map, because unsorted may also be
  // sorted
  vector<int> copy = unordered;
  ordered.resize(index_map.size());
  for(int i = 0; i< int(index_map.size());i++)
  {
    ordered[i] = copy[index_map[i]];
  }
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Spline fitting function called by PlotCubicSplines() to generate an array of
// parameters used to fit splines to vectors of data. Should not be called directly.
//
// Ported from a python implementation from http://jayemmcee.wordpress.com/cubic-splines/
//
// Not at all optimised for c++ architecture.
//
// SWDG - 31/10/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Array2D<float> CalculateCubicSplines(vector<float> X, vector<float> Y){

  int np = X.size(); //number of points in the dataset
  
  //declare vectors used in calculations
  vector<float> a(Y); //copy vector Y into a -> WHY? 
  vector<float> b((np-1),0.0);
  vector<float> d((np-1),0.0);
  vector<float> alpha((np-1),0.0);
  vector<float> c(np,0.0);
  vector<float> u(np,0.0);
  vector<float> z(np,0.0);
  vector<float> L(np,0.0);
  L[0] = 1.0;
  
  //Output array
  Array2D<float> Splines((np-1),5);

  vector<float> h;
  for (int i = 0; i < np-1; ++i){
    h.push_back(X[i+1] - X[i]);
  }

  for (int i = 1; i < (np-1); ++i){           
    alpha[i] = 3/h[i]*(a[i+1]-a[i]) - 3/h[i-1]*(a[i]-a[i-1]);      
  }

  for (int i = 1; i < (np-1); ++i){   
    L[i] = 2*(X[i+1]-X[i-1]) - h[i-1]*u[i-1];
    u[i] = h[i]/L[i];
    z[i] = (alpha[i]-h[i-1]*z[i-1])/L[i];    
  }
  
  L[np-1] = 1.0;
  z[np-1] = 0.0;
  c[np-1] = 0.0;

  for (int j = np-2; j > -1; --j){
    c[j] = z[j] - u[j]*c[j+1];
    b[j] = (a[j+1]-a[j])/h[j] - (h[j]*(c[j+1]+2*c[j]))/3;
    d[j] = (c[j+1]-c[j])/(3*h[j]);
  }
  
  for (int i = 0; i < (np-1); ++i){  
    Splines[i][0] = a[i];
    Splines[i][1] = b[i];
    Splines[i][2] = c[i];
    Splines[i][3] = d[i];
    Splines[i][4] = X[i]; 
  }
  
  return Splines;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Spline plotting function which calls CalculateCubicSplines() to generate a  pair of
// X and Y vectors containing coordinates to plot a spline curve from a given set of data.
//
// Call this with vectors of X and Y data, a resolution (used to get the number of points 
// between each data point for the output curve) and Spline_X and Spline_Y, which are passed in
// by reference and are overwritten with the resulting sets of coordinates.

// Ported from a python implementation from http://jayemmcee.wordpress.com/cubic-splines/
//
// Not at all optimised for c++ architecture.
//
// SWDG - 31/10/13
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void PlotCubicSplines(vector<float> X, vector<float> Y, int SplineResolution, vector<float>& Spline_X, vector<float>& Spline_Y){

  //declare temporary variables used in the calculations
  float x0;
  float x1;
  float h;
  
  //Call the cubic splines function to get the splines data as an array
  Array2D<float> Splines = CalculateCubicSplines(X, Y);

  int n = Splines.dim1(); // This is one fewer than the number of points in the dataset
  
  //check there will be enough partitions between each pair of points for the plotting
  int perSpline = SplineResolution/n; // integer division
  if (perSpline < 3){perSpline = 3;} 
  
  for (int i = 0; i < n-1; ++i){
    x0 = Splines[i][4];
    x1 = Splines[i+1][4];
    vector<float> x = linspace(x0, x1, perSpline); //vector of evenly spaced values between the max and min given    
    
    //calculate x and y values for points along the spline curve between each set of points
    for (int q = 0; q != int(x.size()); ++q){ 
      Spline_X.push_back(x[q]);
      h = (x[q] - x0);
      Spline_Y.push_back(Splines[i][0] + Splines[i][1] * h + Splines[i][2]*(h*h) + (Splines[i][3]*(h*h*h)));    
    }
  }
    
  //calculate x and y values for the curve between the final pair of points
  x0 = Splines[n-1][4]; 
  x1 = X[n];
  vector<float> x2 = linspace(x0, x1, perSpline); //vector of evenly spaced values between the max and min given
    
  for (int q = 0; q != int(x2.size()); ++q){
    Spline_X.push_back(x2[q]);
    h = (x2[q] - x0);
    Spline_Y.push_back(Splines[n-1][0] + Splines[n-1][1] * h + Splines[n-1][2]*(h*h) + (Splines[n-1][3]*(h*h*h)));
  }    
  
}

//Overloaded function to take in an array of integers and return a vector of the
//unique values found in the array, excluding the passed in NoDataValue.
//SWDG 12/11/13 
vector<int> Unique(Array2D<int> InputArray, int NoDataValue){

  // set up output vector                                   
  vector<int> UniqueValues;
  
  //get array dimensions for looping
  int Rows = InputArray.dim1();
	int Cols = InputArray.dim2();
  
  //make list of unique values in each array
  for (int i = 0; i < Rows; ++i){
    for (int j = 0; j < Cols; ++j){
      int Value = InputArray[i][j];
      if (Value != NoDataValue){
        //check if next value is unique
        if(find(UniqueValues.begin(), UniqueValues.end(), Value) == UniqueValues.end()){
          UniqueValues.push_back(Value);
        }
      }
    }
  }

  return UniqueValues;
}

//Overloaded function to take in an array of floats and return a vector of the
//unique values found in the array, excluding the passed in NoDataValue.
//SWDG 12/11/13
vector<float> Unique(Array2D<float> InputArray, int NoDataValue){

  // set up output vector                                   
  vector<float> UniqueValues;
  
  //get array dimensions for looping
  int Rows = InputArray.dim1();
	int Cols = InputArray.dim2();
  
  //make list of unique values in each array
  for (int i = 0; i < Rows; ++i){
    for (int j = 0; j < Cols; ++j){
      int Value = InputArray[i][j];
      if (Value != NoDataValue){
        //check if next value is unique
        if(find(UniqueValues.begin(), UniqueValues.end(), Value) == UniqueValues.end()){
          UniqueValues.push_back(Value);
        }
      }
    }
  }

  return UniqueValues;
}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Simple linear spacing algorithm to return a vector of evenly spaced floats
// between a min and max range (inclusive). Equivalent to np.linspace() in python 
// and linspace in Matlab.
//
// Found at: http://dsj23.wordpress.com/2013/02/13/matlab-linspace-function-written-in-c/
//
// SWDG - 31/10/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
vector<float> linspace(float min, float max, int n){
  vector<float> result;
  int iterator = 0;
  for (int i = 0; i <= n-2; i++){
    float temp = min + i*(max-min)/(floor((float)n) - 1);
    result.insert(result.begin() + iterator, temp);
    iterator += 1;
  }
  result.insert(result.begin() + iterator, max);
  
  return result;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Convert a bearing with 0/360 degrees at north to radians with 0/2pi radians at east - 
// used in Hilltop flow routing
// SWDG 6/9/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float BearingToRad(float Bearing)
{
  return rad((-1 * Bearing) + 90);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Convert degrees to radians - used in TopoShield - SWDG 11/4/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float rad(float degree)
{
	float pi = 3.14159265;
	float deg = 180.0;
	return degree*(pi/deg);
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// conversion from radians to degrees - SWDG 12/12/13
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float deg(float radians){
  float pi = 3.14159265;
  float deg = 180.0;
  return (radians/pi)*deg;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to generate Statistical distribution. - DTM
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void get_distribution_stats(vector<float>& y_data, float& mean, float& median, float& UpperQuartile, float& LowerQuartile, float& MaxValue) 
{
    // Mean
  int n_data_points = y_data.size();
  MaxValue = 0;
    float total = 0;
    
  for (int i = 0; i< n_data_points; i++)
    {
        total+=y_data[i];
     
    }
    
    mean = total/float(n_data_points);
    
    // Get other statistics
    // Sort vector
  vector<long unsigned int> index_map;
  matlab_float_sort(y_data,y_data,index_map);
  // Median
  int point50 = n_data_points/2;
  if (n_data_points % 2 == 0)
  {
    median = (y_data[point50]+y_data[point50+1])/2;
  }
  else
  {
    median = y_data[point50];
  }
  // Quartiles
  int point75 = n_data_points*0.75;
  int point25 = n_data_points*0.25;
  if (n_data_points % 4 == 0)
  {
    UpperQuartile = y_data[point75];
    LowerQuartile = y_data[point25];
  }
  if (n_data_points % 4 == 1);
  {
    UpperQuartile = 0.25*y_data[point75]+0.75*y_data[point75+1];
    LowerQuartile = 0.75*y_data[point25]+0.25*y_data[point25+1];
  }
  if (n_data_points % 4 == 2);
  {
    UpperQuartile = (y_data[point75]+y_data[point75+1])/2;
    LowerQuartile = (y_data[point25]+y_data[point25+1])/2;
  }
  if (n_data_points % 4 == 3);
  {
    UpperQuartile = 0.75*y_data[point75]+0.25*y_data[point75+1];
    LowerQuartile = 0.25*y_data[point25]+0.75*y_data[point25+1];
  }
  // Max
  MaxValue = y_data[n_data_points-1];
} 

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Method to calculate the quadratic mean. - DTM
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
double get_QuadraticMean(vector<double> input_values, double bin_width)
{
	// Finding max contributing area to use as upper limit for the bins
  int n_data = input_values.size();
  double max_X = input_values[0];
	double min_X = input_values[0];

  // Loop through to find the min and max of the dataset
  for (int i = 0; i < n_data; ++i)
	{
    if (input_values[i] > max_X)
    {
      max_X = input_values[i];
    }
    if (input_values[i] < min_X)    
    {
      min_X = input_values[i];
    }
  }
  // Defining the upper limit, lower limit and the bin width.
  // Extend range by one bin at each end so that the histogram is bounded by 
  // zeros for plotting
  double upper_limit = (ceil(max_X/bin_width)+1)*bin_width;
  double lower_limit = (floor(min_X/bin_width)-1)*bin_width;
  int NBins = int( (upper_limit - lower_limit)/bin_width )+1;

  // Looping through all the rows and columns and calculating which bin the
  // contributing area is in, and putting the slope in this bin
  vector<int> number_observations(NBins,0);
  vector<double> bin_midpoints(NBins,0.0);
  vector<double> bin_lower_lim(NBins,0.0);
  vector<double> bin_upper_lim(NBins,0.0);
  vector<double> probability_density(NBins,0);

	// create the vector of vectors.  Nested vectors will store data within that
  // bin.
  vector<double> empty_vector;
  double midpoint_value, lower_lim_value, upper_lim_value;

  // Bin Data
  for (int i = 0; i < n_data; ++i)
  {
    double X = input_values[i];
    // Get bin_id for this particular value of X
    int bin_id = int((X-lower_limit)/bin_width);
    ++number_observations[bin_id];
  }
  
  for(int i = 0; i<NBins; i++)
  {
    midpoint_value = lower_limit + (double(i)+0.5)*bin_width;
    lower_lim_value = lower_limit + double(i)*bin_width;
    upper_lim_value = double(i+1)*bin_width;  
    
    bin_midpoints[i]= midpoint_value;
    bin_lower_lim[i]= lower_lim_value;
    bin_upper_lim[i]= upper_lim_value;
    
    probability_density[i] = number_observations[i]/double(n_data);    
  }
  
  double QuadraticMean = 0;
  for(int i = 0; i<NBins; i++)
  {
    QuadraticMean += probability_density[i]*bin_midpoints[i]*bin_midpoints[i];
  }
  QuadraticMean = sqrt(QuadraticMean);
  return QuadraticMean;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Line parser for parameter files - JAJ 08/01/2014
// This might be better off somewhere else
//
// To be used on a parameter file of the format:
// 	Name: 100		comments etc.
// Which sets parameter as "Name" and value as "100"
//
// This just does one line at a time; you need a wrapper function to get all
// the information out of the file
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void parse_line(ifstream &infile, string &parameter, string &value)
{
	char c;
	char buff[128];
	int pos = 0;
	int word = 0;

	while ( infile.get(c) )	
	{
		if (pos >= 128)
		{
			cout << "Buffer overrun, word too long in parameter line: " << endl;
			string line;
			getline(infile, line);
			cout << "\t" << buff << " ! \n" << line << endl;
			exit(1);
		}
		// preceeding whitespace
		if (c == '#')
		{
			if (word == 0)
			{
				parameter = "NULL";
				value = "NULL";
			}
			if (word == 1)
				value = "NULL";
			word = 2;
		}

		if ((c == ' ' || c == '\t') && pos == 0)
			continue;
		else if ( (c == ':' && word == 0) || ( (c == ' ' || c == '\n' || c == '\t') && word == 1))
		{
			while (buff[pos-1] == ' ' || buff[pos-1] == '\t')
				--pos;		// Trailing whitespace
			buff[pos] = '\0';	// Append Null char
			if (word == 0)
				parameter = buff;	// Assign buffer contents
			else if (word == 1)
				value = buff;
			++word;
			pos = 0;		// Rewind buffer
		}
		else if ( c == '\n' && word == 0 )
		{
			parameter = "NULL";
			buff[pos] = '\0';
			value = buff;
			++word;
		}
		else if (word < 2)
		{
			buff[pos] = c;
			++pos;
		}

		if (c == '\n')
			break;
	}
	if (word == 0)
	{
		parameter = "NULL";
		value = "NULL";
	}
}

// Method to get the maximum value in a 2D array of floats - SWDG 12/6/14
float Get_Maximum(Array2D<float> Input, float NDV){

  float max = 0;
	for (int i = 0; i < Input.dim1(); ++i){
    for(int j = 0; j < Input.dim2(); ++j){
      if (Input[i][j] > max && Input[i][j] != NDV){
        max = Input[i][j];
      }
    }
  }
  return max;
}

// Method to get the maximum value in a 2D array of integers - SWDG 12/6/14
float Get_Maximum(Array2D<int> Input, float NDV){

  int max = 0;
	for (int i = 0; i < Input.dim1(); ++i){
    for(int j = 0; j < Input.dim2(); ++j){
      if (Input[i][j] > max && Input[i][j] != NDV){
        max = Input[i][j];
      }
    }
  }
  return max;
}

//Method to flatten a 2D array into a 1D vector
//generates a vector in row major order
//SWDG 12/6/14
vector<int> Flatten(Array2D<int> Input){

  vector<int> flat;     
  for (int i = 0; i < Input.dim1(); ++i){
    for (int j = 0; j < Input.dim2(); ++j){
      flat.push_back(Input[i][j]);
    }
  }
  return flat;

}

//Method to flatten a 2D array into a 1D vector
//generates a vector in row major order
//SWDG 12/6/14
vector<float> Flatten(Array2D<float> Input){

  vector<float> flat;     
  for (int i = 0; i < Input.dim1(); ++i){
    for (int j = 0; j < Input.dim2(); ++j){
      flat.push_back(Input[i][j]);
    }
  }
  return flat;

}

//Method to flatten a 2D array into a 1D vector
//generates a vector in row major order, excluding nodata values
//SWDG 12/6/14
vector<int> Flatten_Without_Nodata(Array2D<int> Input, float NDV){

  vector<int> flat;     
  for (int i = 0; i < Input.dim1(); ++i){
    for (int j = 0; j < Input.dim2(); ++j){
      if (Input[i][j] != NDV){
        flat.push_back(Input[i][j]);
      }
    }
  }
  return flat;

}

//Method to flatten a 2D array into a 1D vector
//generates a vector in row major order, excluding nodata values
//SWDG 12/6/14
vector<float> Flatten_Without_Nodata(Array2D<float> Input, float NDV){

  vector<float> flat;     
  for (int i = 0; i < Input.dim1(); ++i){
    for (int j = 0; j < Input.dim2(); ++j){
      if (Input[i][j] != NDV){
        flat.push_back(Input[i][j]);
      }
    }
  }
  return flat;

}


//Method to count the number of instances of a given value in an array
//SWDG 17/6/14
int CountValue(Array2D<int> Input, int Value){

  int count = 0;

  for (int i = 0; i < Input.dim1(); ++i){
    for (int j = 0; j < Input.dim2(); ++j){
      if (Input[i][j] == Value){
        ++count;
      }
    }
  }

  return count;
}

//Method to count the number of instances of a given value in an array
//SWDG 17/6/14
int CountValue(Array2D<float> Input, float Value){

  int count = 0;

  for (int i = 0; i < Input.dim1(); ++i){
    for (int j = 0; j < Input.dim2(); ++j){
      if (Input[i][j] == Value){
        ++count;
      }
    }
  }

  return count;
}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Method used to generate a Kolmogorov-Smirnov statistic and p value
//from numerical recipes
//SWDG 26/6/14
float PKS(float z){

  if (z < 0.0){
    cout << "Bad z value." << endl;
    exit(EXIT_FAILURE);
  }

  if (z == 0.0) { return 0.0;}
  
  if (z < 1.18){
  
    float y = exp(-1.23370055013616983/pow(z,2)); 
    return 2.25675833419102515 * sqrt(-log(y)) * (y + pow(y,9) + pow(y,25) + pow(y,49));
  }
  else{
   
   double x = exp(-2.0 * pow(z,2));
   double u = ((x - pow(x,4) + pow(x,9)));
   return 2.0 * (x - pow(x,4) + pow(x,9)); 
  
  }

}

//Method used to generate a Kolmogorov-Smirnov statistic and p value
//from numerical recipes
//Data1 and Data2 must be sorted.
//d is the KS statistic value
//p is the p-value. In numerical recipes it is provided as a value subtracted from
//1, this code has been modified to present value the without this subtraction
//so that it matches the result from the scipy implementation. 
//SWDG 26/6/14
void KStwo(vector<float> Data1, vector<float> Data2, float& d, double& p){

  int j1 = 0;
  int j2 = 0;
  int n1 = Data1.size();
  int n2  = Data2.size();
  float d1;
  float d2;
  float dt;
  float en1 = n1;
  float en2 = n2;
  float en;
  float fn1 = 0.0;
  float fn2 = 0.0;
  
  d = 0.0;

  while (j1 < n1 && j2 < n2){
    
    if ((d1 = Data1[j1]) <= (d2 = Data2[j2])) {
      
      do{
        fn1 = ++j1/en1;
      } while (j1 < n1 && d1 == Data1[j1]);
        
    }
  
    if (d2 <= d1){
      
      do{
        fn2 = ++j2/en2;
      } while (j2 < n2 && d2 == Data2[j2]);
      
    }
    if ((dt = abs(fn2-fn1)) > d) { d = dt; }
  
  } //close while

  en = sqrt(en1 * en2 / (en1 + en2));    
  p = PKS((en + 0.12 + 0.11 / en) * d );

}


// Given a filestream object, read the file into memory and return
// it as a string. From: http://www.cplusplus.com/forum/general/58945/
// No error handling.
// SWDG 16/07/14
string ReadTextFile(ifstream& File){

  string Lines = "";        //All lines
    
  if (File){                      //Check if everything is good  
	  while (File.good ()){
	    string TempLine;                  //Temp line
	    getline(File, TempLine);        //Get temp line
	    TempLine += "\n";                      //Add newline character
	    Lines += TempLine;                     //Add newline
	  }
	  return Lines;
    }
  else{
    return "";
  }  
} 

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==
// This function removes control characters from the end of a string
// These get introduced if you use the DOS format in your parameter file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--==
string RemoveControlCharactersFromEndOfString(string toRemove)
{
  int len =  toRemove.length();  
  if(len != 0)  
  {  
    if (iscntrl(toRemove[len-1]))  
    { 
      //cout << "Bloody hell, here is another control character! Why Microsoft? Why?" << endl; 
      toRemove.erase(len-1);  
    }  
  }  
  return toRemove;
}

#endif


