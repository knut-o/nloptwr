
// Program to demonstrate gaussian on a set of linear simultaneous equations
// see http://stackoverflow.com/questions/5201696/gauss-elimination-for-nxm-matrix

#include <iostream>
#include <cmath>
#include <vector>
#include "nloptwr/lregx.h"

using namespace std;

namespace utilx {
    
// tolerance
const double eps = 1.e-15;

// Preliminary pivoting strategy, Pivoting function
double pivot(vector<vector<double> > &a, vector<double> &b, int i)
 {
     int n = a.size();
     int j=i;
     double t=0;

     for(int k=i; k<n; k+=1)
     {
         double aki = fabs(a[k][i]);
         if(aki>t)
         {
             t=aki;
             j=k;
         }
     }
     if(j>i)
     {
         double dummy;
         for(int L=0; L<n; L+=1)
         {
             dummy = a[i][L];
             a[i][L]= a[j][L];
             a[j][L]= dummy;
         }
         double temp = b[j];
         b[i]=b[j];
         b[j]=temp;
     }
     return a[i][i];
 }        



// Forward elimination
void triang(vector<vector<double> > &a, vector<double> &b) 
{
    int n = a.size();
    for(int i=0; i<n-1; i+=1)
    {
        double diag = pivot(a,b,i);
        if(fabs(diag)<eps)
        {
            cout<<"zero det"<<endl;
            return;
        }
        for(int j=i+1; j<n; j+=1)
        {
            double mult = a[j][i]/diag;
            for(int k = i+1; k<n; k+=1)
            {
                a[j][k]-=mult*a[i][k];
            }
            b[j]-=mult*b[i];
        }
    }
}

// dot product
double dotProd(vector<double> &u, vector<double> &v, int k1,int k2)
{
  double sum = 0;
  for(int i = k1; i <= k2; i += 1)
  {
     sum += u[i] * v[i];
  }
  return sum;
}

// back substitution step
void backSubst(vector<vector<double> > &a, vector<double> &b, vector<double> &x)
{
    int n = a.size();
  for(int i =  n-1; i >= 0; i -= 1)
  {
    x[i] = (b[i] - dotProd(a[i], x, i + 1,  n-1))/ a[i][i];
  }
}

// refined gaussian elimination procedure
void gauss(vector<vector<double> > &a, vector<double> &b, vector<double> &x)
{
   triang(a, b);
   backSubst(a, b, x);
}                               

// dot product
void dotProd(const std::vector<std::vector<double> > &a, const std::vector<double> &x, std::vector<double> &b) {
    double sum=0.0;
    for (size_t i=0; i<x.size(); i++) {
        sum=0.0;
        for (size_t j=0; j<x.size(); j++) sum +=a.at(i).at(j)*x.at(j);
        b[i]=sum;
    }
}

} // namespace utilx

