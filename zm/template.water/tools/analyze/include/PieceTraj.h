#ifndef __PieceTraj_h_wanghan__
#define __PieceTraj_h_wanghan__

#include <vector>

using namespace std;

template <class VALUETYPE >
class PieceTraj
{
  vector<VALUETYPE > data;
  int posi;
  int nValid;
public :
  PieceTraj ();
  PieceTraj (const int & ndata);
public:
  void push_back (const VALUETYPE & value);
  void reinit (const int & ndata);
  void clear ();
  bool full () const {return nValid == int(data.size());}
  int normalizedIdx (const int & idx) const;
  const int & getNValid () const {return nValid;}
  const int tailPosi () const {return normalizedIdx(posi-1);}
  const VALUETYPE & getValue (const int & idx) const {return data[normalizedIdx(idx)];}
  const VALUETYPE & front () const {return getValue(posi);}
  const VALUETYPE & back  () const {return getValue(posi-1);}
}
    ;

template <class VALUETYPE >
inline int PieceTraj<VALUETYPE>::
normalizedIdx (const int & idx) const
{
  int newidx (idx);
  if (idx >= int(data.size())) newidx -= data.size();
  else if (idx < 0) newidx += data.size();
  return newidx;
}

template <class VALUETYPE >
inline void PieceTraj<VALUETYPE>::
push_back (const VALUETYPE & value)
{
  data[posi] = value;
  posi++;
  posi = normalizedIdx (posi);
  if (nValid < int(data.size())) ++nValid;
}

template <class VALUETYPE >
inline void PieceTraj<VALUETYPE>::
clear ()
{
  posi = 0;
  nValid = 0;
}

template <class VALUETYPE >
PieceTraj<VALUETYPE>::
PieceTraj (const int & ndata)
    : data (ndata), posi(0), nValid(0)
{
}

template <class VALUETYPE >
PieceTraj<VALUETYPE>::
PieceTraj () 
    : posi(0), nValid(0)
{
}

template <class VALUETYPE >
void PieceTraj<VALUETYPE>::
reinit (const int & ndata) 
{
  data.clear ();
  data.resize (ndata);
  posi = 0;
  nValid = 0;
}


#endif
