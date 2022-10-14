
#ifndef O_DISPLAY_H
#define O_DISPLAY_H

#include "nloptwrtest/griewank.h"
#include "nloptwrtest/oproblem.h"
#include "nloptwrtest/oproblemtest.h"

#include "nloptwr/nloptwr.h"
#include "nloptwr/nloptwralgorithm.h"
#include "nloptwr/nloptwrsstrat.h"
#include "nloptwr/optfktnbase.h"

#include <cmath>
#include <vector>

#include <cmath>
#include <iomanip>
#include <iostream>

namespace opttest {

/**
 * displays the search algorithm and the result of the optimization
 * @param optWr opt wrapper object
 * @param sStrat search strategy
 * @param opt_stat status of result of optimization
 * @param minf optimized value
 */
int display(const nloptwr::NLOptWrapper &optWr,
            const nloptwr::NLOptWrSStrat &sStrat, nlopt::result opt_stat,
            double minf);

/**
 * displays the search algorithm of the optimization
 * @param optWr opt wrapper object
 * @param sStrat search strategy
 */
int displaySearchAlgorithms(const nloptwr::NLOptWrapper &optWr,
                            const nloptwr::NLOptWrSStrat &sStrat);

/**
 * displays the result of the optimization
 * @param optWr opt wrapper object
 * @param opt_stat status of result of optimization
 * @param minf optimized value
 */
int displayResult(const nloptwr::NLOptWrapper &optWr, nlopt::result opt_stat,
                  double minf);

} // namespace opttest

#endif // O_DISPLAY_H
