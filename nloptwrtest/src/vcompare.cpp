
#include "nloptwrtest/vcompare.h"

#include <cmath>
#include <vector>

using namespace std;

namespace opttest {

    bool fvcompare(double f1, double f2, const std::vector<double>& src1, double src2, double eps) {
        bool result = (fabs(f1 - f2) < fabs(eps));
        if (result) {
            result = vcompare(src1, src2, eps);
        }
        return result;
    }


    bool fvcompare(double f1, double f2, const std::vector<double>& src1, const std::vector<double>& src2, double eps) {
        bool result = (fabs(f1 - f2) < fabs(eps));
        if (result) {
            result = vcompare(src1, src2, eps);
        }
        return result;
    }



    bool vcompare(const vector<double>& src1, const vector<double>& src2, double eps) {
        bool result=(src1.size()==src2.size());
        double diff=0.0;
        eps=fabs(eps);
        if (result) {
            for (size_t i=0; (i<src1.size())&&result; i++) {
                diff=fabs(src1.at(i)-src2.at(i));
                if (result) result=(diff <= eps);
            }
        }
        return result;
    }
    
    bool vcompare(const vector<double>& src1, double src2, double eps) {
        vector<double> v2(src1.size(), src2);

        return vcompare(src1, v2, eps);
    }


} // namespace opttest

