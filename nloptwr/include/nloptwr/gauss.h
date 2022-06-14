

#ifndef GAUSS_H
#define GAUSS_H

#include <vector>

namespace utilx {
    
/**
 * solution of equation A*x=b
 * by using the Gauss algorithm
 * 
 * @param a a quadratic matrix (dim n*n)
 * @param b right hand term (dim n)
 * @param x solution (dim n, return vector)
 * 
 */
void gauss(std::vector<std::vector<double> > &a, std::vector<double> &b, std::vector<double> &x);

/**
 * Dot product b=A*x
 * 
 * @param a a quadratic matrix (dim n*n)
 * @param x 1-dim vector (dim n)
 * @param b result of product A*x (dim n, , return value)
 * 
 */
void dotProd(const std::vector<std::vector<double> > &a, const std::vector<double> &x, std::vector<double> &b);

} // namespace utilx 

#endif // GAUSS_H
