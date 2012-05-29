#include "BlockAverage.h"
#include <cmath>

void BlockAverage::
calAvg (const std::vector<double > & vec,
	double & my_avg,
	double & my_error_avg)
{
  my_avg = 0;
  for (unsigned i = 0; i < nDataUsed; ++i){
    my_avg += vec[i];
  }
  my_avg /= double(nDataUsed);

  std::vector<double > blockAverage;  
  for (unsigned i = 0; i < nBlock; ++i){
    double tmp = 0;
    for (unsigned j = 0; j < nDataInBlock; ++j){
      tmp += vec[i * nDataInBlock + j];
    }
    tmp /= double (nDataInBlock);
    blockAverage.push_back (tmp);
  }
  
  double sum2 = 0;
  for (unsigned i = 0; i < nBlock; ++i){
    sum2 += (blockAverage[i] - my_avg) * (blockAverage[i] - my_avg);
  }

  my_error_avg = sqrt (sum2 / (nBlock * (nBlock-1)));
}

void BlockAverage::
processData (const std::vector<double > & vec,
	     const unsigned & nBlock_)
{
  nBlock = nBlock_;
  unsigned nData = vec.size();
  nDataInBlock = nData / nBlock;
  nDataUsed = nDataInBlock * nBlock;

  calAvg (vec, avg, error_avg);
  
  std::vector<double > var_vec;
  std::vector<double > var3_vec;
  for (unsigned i = 0; i < nDataUsed; ++i){
    double tmp = vec[i] - avg;
    var_vec.push_back (tmp * tmp);
    var3_vec.push_back (tmp * tmp * tmp);
  }
  calAvg (var_vec, var, error_var);
  calAvg (var3_vec, var3, error_var3);

  var *= nDataUsed / (nDataUsed - 1.);
  error_var *= nDataUsed / (nDataUsed - 1.);

  var3 *= double(nDataUsed) * double(nDataUsed)
      / ((nDataUsed - 1.) * (nDataUsed - 2.));
  error_var3 *= double(nDataUsed) * double(nDataUsed)
      / ((nDataUsed - 1.) * (nDataUsed - 2.));
}


void JackknifeAverage::
calAvg (const std::vector<double > & vec,
	double & my_avg,
	double & my_error_avg)
{
  double my_sum;
  my_avg = 0;
  for (unsigned i = 0; i < nDataUsed; ++i){
    my_avg += vec[i];
  }
  my_sum = my_avg;
  my_avg /= double(nDataUsed);

  std::vector<double > jackkniftAverage;  
  for (unsigned i = 0; i < nBlock; ++i){
    double tmp = 0;
    for (unsigned j = 0; j < nDataInBlock; ++j){
      tmp += vec[i * nDataInBlock + j];
    }
//    tmp /= double (nDataInBlock);
    jackkniftAverage.push_back (tmp);
  }
  double my_avg2 = 0.;
  for (unsigned i = 0; i < jackkniftAverage.size(); ++i){
    jackkniftAverage[i] = (my_sum - jackkniftAverage[i]) / (double (nDataUsed - nDataInBlock));
    my_avg2 += jackkniftAverage[i];
  }
  my_avg2 /= double (nBlock);
  
  double sum2 = 0;
  for (unsigned i = 0; i < nBlock; ++i){
    sum2 += (jackkniftAverage[i] - my_avg2) * (jackkniftAverage[i] - my_avg2);
  }

  my_error_avg = sqrt (sum2 * (nBlock-1) / (nBlock));
}

void JackknifeAverage::
processData (const std::vector<double > & vec,
	     const unsigned & nBlock_)
{
  nBlock = nBlock_;
  unsigned nData = vec.size();
  nDataInBlock = nData / nBlock;
  nDataUsed = nDataInBlock * nBlock;

  calAvg (vec, avg, error_avg);
  
  std::vector<double > var_vec;
  std::vector<double > var3_vec;
  for (unsigned i = 0; i < nDataUsed; ++i){
    double tmp = vec[i] - avg;
    var_vec.push_back (tmp * tmp);
    var3_vec.push_back (tmp * tmp * tmp);
  }
  calAvg (var_vec, var, error_var);
  calAvg (var3_vec, var3, error_var3);

  var *= nDataUsed / (nDataUsed - 1.);
  error_var *= nDataUsed / (nDataUsed - 1.);

  var3 *= double(nDataUsed) * double(nDataUsed)
      / ((nDataUsed - 1.) * (nDataUsed - 2.));
  error_var3 *= double(nDataUsed) * double(nDataUsed)
      / ((nDataUsed - 1.) * (nDataUsed - 2.));
}





void FourthDerivative::
calAvg (const std::vector<double > & vec,
	double & my_avg,
	double & my_error_avg)
{
  my_avg = 0;
  for (unsigned i = 0; i < nDataUsed; ++i){
    my_avg += vec[i];
  }
  my_avg /= double(nDataUsed);

  std::vector<double > blockAverage;  
  for (unsigned i = 0; i < nBlock; ++i){
    double tmp = 0;
    for (unsigned j = 0; j < nDataInBlock; ++j){
      tmp += vec[i * nDataInBlock + j];
    }
    tmp /= double (nDataInBlock);
    blockAverage.push_back (tmp);
  }
  
  double sum2 = 0;
  for (unsigned i = 0; i < nBlock; ++i){
    sum2 += (blockAverage[i] - my_avg) * (blockAverage[i] - my_avg);
  }

  my_error_avg = sqrt (sum2 / (nBlock * (nBlock-1)));
}

void FourthDerivative::
processData (const std::vector<double > & vec,
	     const unsigned & nBlock_)
{
  nBlock = nBlock_;
  unsigned nData = vec.size();
  nDataInBlock = nData / nBlock;
  nDataUsed = nDataInBlock * nBlock;

  calAvg (vec, avg, error_avg);
  
  std::vector<double > avg2_vec;
  std::vector<double > var4_vec;
  for (unsigned i = 0; i < nDataUsed; ++i){
    avg2_vec.push_back (vec[i] * vec[i]);
    double tmp = vec[i] - avg;
    tmp = tmp * tmp;
    var4_vec.push_back (tmp * tmp);
  }
  calAvg (avg2_vec, avg2, error_avg2);
  calAvg (var4_vec, var4, error_var4);

  final_avg = 3 * (avg2 - avg*avg) * (avg2 - avg*avg) - var4;
}

