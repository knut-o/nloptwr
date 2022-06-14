
#include "nloptwr/lregx.h"
#include "nloptwr/gauss.h"

#include <vector>
#include <iomanip>
#include <iostream>
#include <stdexcept>

using namespace std;

namespace utilx {
    
LRegX::LRegX(std::size_t xDim) : dim(xDim)  {
    dim_x=1+2*((dim>0)? dim-1 : 0);
    
    x_i.resize(dim_x, 0.0);
    yx_i.resize(dim, 0.0);

    a.resize(dim);
    for (size_t j=0; j<dim; j++) {
        a[j].resize(dim, 0.0);
    }
    
    b.resize(dim, 0.0);
}
    
LRegX::~LRegX() {
    
}

void LRegX::resize(std::size_t xDim) {
    
    // only resize vectors if needed
    if (dim!=xDim) {
        dim=xDim;
        
        dim_x=1+2*((dim>0)? dim-1 : 0);
        
        x_i.resize(dim_x, 0.0);
        yx_i.resize(dim, 0.0);

        a.resize(dim);
        for (size_t j=0; j<dim; j++) {
            a[j].resize(dim, 0.0);
        }
        
        b.resize(dim, 0.0);
    }
    
    reset();   
}
    
void LRegX::add(double x, double y) {
    double x_tmp=1.0;
    double y_tmp=y;
    for (size_t i=0; i<dim_x; i++) {
        x_i[i]+=x_tmp;
        if (i<dim) {
            yx_i[i]+=y_tmp;
            y_tmp*=x;
        }
        x_tmp*=x;
    }
}

void LRegX::operator += (const LRegX& src) {
    
    if (dim!=src.dim || dim_x!=src.dim_x) {
        string msg("LRegX: ERROR: operator += needs compatible formats: false==(dim!=src.dim || dim_x!=src.dim_x)");
        cerr << msg << endl;
        throw new runtime_error(msg);
    }
    
    if (x_i.size()!=src.x_i.size() || yx_i.size()!=src.yx_i.size()) {
        string msg("LRegX: ERROR: operator += needs compatible formats: false==(x_i.size()!=src.x_i.size() || yx_i.size()!=src.yx_i.size())");
        cerr << msg << endl;
        throw new runtime_error(msg);
    }
    
    for (size_t i=0; i<x_i.size(); i++) {
        x_i[i] += src.x_i[i];
    }
    
    for (size_t i=0; i<yx_i.size(); i++) {
        yx_i[i] += src.yx_i[i];
    }
}

    
void LRegX::reset() {
    for (size_t i=0; i<dim_x; i++) {
        x_i[i]=0.0;
    }
    for (size_t i=0; i<dim; i++) {
        yx_i[i]=0.0;

        // a.resize(dim);
        for (size_t j=0; j<dim; j++) {
            a[i][j]=0.0;
        }

        b[i]=0.0;
    }
}
    
void LRegX::calc(std::vector<double>& result) {
        if (result.size()!=dim) result.resize(dim);
        
        b=yx_i;
        for (size_t i=0; i<dim; i++) {
            result[0]=0.0;
            for (size_t j=0; j<dim; j++) {
                a[i][j]=x_i[i+j]; 
            }
        }
        
        gauss(a, b, result);
}

} // namespace utilx

