
#ifndef LREGX_H
#define LREGX_H

#include <vector>

namespace utilx {

/// function arguments object    
struct FArg {
public:
    /// function value
    double f;
    
    /// arguments of function
    std::vector<double> x;

    /// values of non equal constraints
    std::vector<double> c;
};

/**
 * regression of dimension dim
 * @class LRegX
 */
class LRegX {

public:
    
    /**
     * constructor 
     * @param xDim dimension of aproach
     */
    LRegX(std::size_t xDim=3);
    
    /// destructor
    ~LRegX();
    
    /**
     * resize dimension
     * and initializes (resets) all values
     */
    void resize(std::size_t xDim=3);
    
    /**
     * add a data point
     * @param x experimental value of x 
     * @param y experimental value of y
     */
    void add(double x, double y);
    
    /**
     * reset data points
     */
    void reset();
    
    /**
     * addition operator
     * all counter are added
     * @param src instance
     */
    void operator += (const LRegX& src);
    
    
    /**
     * calculate the regression parameters result of dim
     * y = result[0]*x^0 (+ result[1]*x^1 + result[2]*x^2 + ...)
     * @return result parameter of aproach
     */
    void calc(std::vector<double>& result);
    
private:
    
    /// dimension of aproach
    std::size_t dim;
    
    /// internal used variable
    std::size_t dim_x;
    
    /// sum of x^i
    std::vector<double> x_i;
    
    /// sum of y*x^i
    std::vector<double> yx_i;
    
    /// internal used matrix (built from x_i)
    std::vector<std::vector<double> > a;
    
    /// internal used vector (=x_i)
    std::vector<double> b;
    
};

} // namespace utilx

#endif // LREGX_H

