
#ifndef V_COMPARE_H
#define V_COMPARE_H

#include <vector>

namespace opttest
{
    /**
    * compare two vectors value by value with tolerance eps
    * @param f1   value
    * @param f2   value
    * @param src1 vector
    * @param src2 value
    * @param eps
    * @return true if all elements of src1 are equal src2 (with tolerance)
     */
    bool fvcompare(double f1, double f2, const std::vector<double>& src1, double src2, double eps = 0.0006);

    /**
     * compare two valued and two vectors value by value with tolerance eps
     * @param f1   value
     * @param f2   value
     * @param src1 vector
     * @param src2 vector
     * @param eps
     * @return true if all values of the two vectors are equal (with tolerance)
     */
    bool fvcompare(double f1, double f2, const std::vector<double>& src1, const std::vector<double>& src2, double eps = 0.0006);

    /**
     * compare two vectors value by value with tolerance eps
     * @param src1 vector
     * @param src2 vector
     * @param eps
     * @return true if all values of the two vectors are equal (with tolerance)
     */
    bool vcompare(const std::vector<double>& src1, const std::vector<double>& src2, double eps = 0.0006);

    /**
    * compare two vectors value by value with tolerance eps
    * @param src1 vector
    * @param src2 value
    * @param eps
    * @return true if all elements of src1 are equal src2 (with tolerance)
     */
    bool vcompare(const std::vector<double>& src1, double src2, double eps=0.0006);

} // namespace opttest

#endif // V_COMPARE_H
