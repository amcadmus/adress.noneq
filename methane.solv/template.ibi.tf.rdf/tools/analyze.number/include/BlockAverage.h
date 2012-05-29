#ifndef __BlockAverage_h_wanghan__
#define __BlockAverage_h_wanghan__

#include <vector>

class BlockAverage 
{
  double avg;
  double var;
  double var3;
  double error_avg;
  double error_var;
  double error_var3;
  unsigned nBlock;
  unsigned nDataInBlock;
  unsigned nDataUsed;
private:
  void calAvg (const std::vector<double > & vec,
	       double & my_avg,
	       double & my_error_avg);
public:
  void processData (const std::vector<double > & vec,
		    const unsigned & nBlock);
  double getAvg () const {return avg;}
  double getVar () const {return var;}
  double getVar3 () const {return var3;}
  double getAvgError () const {return error_avg;}
  double getVarError () const {return error_var;}
  double getVar3Error () const {return error_var3;}
  unsigned getNumDataUsed () const {return nDataUsed;}
};

class JackknifeAverage
{
  double avg;
  double var;
  double var3;
  double error_avg;
  double error_var;
  double error_var3;
  unsigned nBlock;
  unsigned nDataInBlock;
  unsigned nDataUsed;
private:
  void calAvg (const std::vector<double > & vec,
	       double & my_avg,
	       double & my_error_avg);
public:
  void processData (const std::vector<double > & vec,
		    const unsigned & nBlock);
  double getAvg () const {return avg;}
  double getVar () const {return var;}
  double getVar3 () const {return var3;}
  double getAvgError () const {return error_avg;}
  double getVarError () const {return error_var;}
  double getVar3Error () const {return error_var3;}
  unsigned getNumDataUsed () const {return nDataUsed;}
};


class FourthDerivative
{
  double avg;
  double avg2;
  double var4;
  double error_avg;
  double error_avg2;
  double error_var4;
  unsigned nBlock;
  unsigned nDataInBlock;
  unsigned nDataUsed;
  double final_avg;
private:
  void calAvg (const std::vector<double > & vec,
	       double & my_avg,
	       double & my_error_avg);
public:
  void processData (const std::vector<double > & vec,
		    const unsigned & nBlock);
  double getResult () {return final_avg;}
  unsigned getNumDataUsed () const {return nDataUsed;}
};
  


#endif
