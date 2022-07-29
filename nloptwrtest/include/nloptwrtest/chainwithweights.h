
#ifndef CHAIN_WITH_WEIGHTS_H
#define CHAIN_WITH_WEIGHTS_H

#include "nloptwr/nloptwr.h"
#include "nloptwr/optfktnbase.h"
#include "nloptwr/optfktnclass.h"

#include <vector>
#include <functional>
#include <memory>
#include <iostream>

namespace opttest
{
    
/*
 * 
 *     h ^                            + hn=hc[nDim+1]
 *       |                           /|
 *       |                          / |
 *       |                         /  |
 *       |                        +   |
 *       |                       /#   |
 *       |                      /     |
 *       |                     /      |
 *       |                    / ...   |
 *    h0 +                   +        |
 * =hc[0]| \                /#        |
 *       |  \              / w3       |
 *       |   \ hc[1]      /           |
 *       |    \    hc[2] /            |
 *       |     +-------+              |
 *       |     #       #              |
 *       |    w1       w2             |
 *       |                            |
 *       |                            |
 *       +-----+-------+-----+--------+
 *      x0    xc[1]   xc[2] xc[3] ... xn
 *     =xc[0]                         =xc[nDim+1]
 * 
 *  w1, w2, ... = additional weights
 *  h = height
 *  hc[i] = height at position
 *  xc[i] = coordinate x at position
 * 
 *  
 */

/**
 * @class ChainWithWeights
 * A simple optimization task 
 * (implementation of test function)
 */
class ChainWithWeights : public oif::OptFknClass
{

public:


/**
 * constructor
 * @param dim dimension
 * @param xn  x-value on the right corner
 * @param hn  height on the right corner
 * @param lm  mass per length of chain (without separate weights)
 * @param li  length of a chain link
 * @param weights separate weights at pos (dim=n-1, pos=0 => i=1, ...)
 */
    ChainWithWeights( int dim, double xN=1000, double hN=250, double lM=1, double lI=1, const std::vector<double>& myWeights=std::vector<double>());

    /// destructor
    virtual ~ChainWithWeights();

    /**
     * implementation of function definition
     * @param x (vector of) function arguments
     * @param c vector of nonequal constraints (c_i <= 0.0)
     * @return value of target function
     */
    virtual double optFktn ( const std::vector<double>& x, std::vector<double>& c ) override;

    /**
     * initialize method
     * @param lb lower bounds
     * @param ub upper bounds
     * @param xInit initial value for all arguments
     */
    virtual void initialize (); //  double lb, double ub, double xInit );

    /**
     * clone method
     * @return refernce of clone (of this object)
     */
    virtual oif::OptFknBase* clone() const override;

    /**
     * print result 
     * @param x arguments
     * @param os output stream
     */
    void printResult(const std::vector<double>& x, std::ostream& os);

private:

    /// dimension of x-vector
    int nDim;

    /// number of nonequal constraints
    const static int mDim=4;
    
    /// left corner
    double x0;
    
    /// left corner
    double h0;
    
    /// right corner
    double xn;

    /// right corner
    double hn;
    
    /// mass per length
    double lm;

    /// length of a chain link
    double li;

    // weights 
    std::vector<double> weights;
    
    // coordinates x
    std::vector<double> xc;
    
    // coordinate h
    std::vector<double> hc;
    
    // epsilon
    static const double epsilon;
    
    // PI
    static const double PI;
};

} // namespace opttest

#endif // CHAIN_WITH_WEIGHTS_H
