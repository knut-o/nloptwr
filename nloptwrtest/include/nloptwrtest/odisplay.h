
#ifndef O_DISPLAY_H
#define O_DISPLAY_H

#include "nloptwrtest/oproblemtest.h"
#include "nloptwrtest/griewank.h"
#include "nloptwrtest/oproblem.h"

#include "nloptwr/optfktnbase.h"
#include "nloptwr/nloptwrsstrat.h"
#include "nloptwr/nloptwralgorithm.h"

#include <cmath>
#include <vector>

#include <iostream>
#include <iomanip>
#include <cmath>

namespace opttest
{

    /**
     * diplays the search algoritm and the result of the optimization
     * @param optWr opt wrapper object
     * @param sStrat search strategy
     * @param opt_stat status of result of optimization
     * @param minf optimized value
     */
int display(const nloptwr::NLOptWrapper& optWr, const nloptwr::NLOptWrSStrat& sStrat, nlopt::result opt_stat, double minf);

    /**
     * diplays the search algoritm of the optimization
     * @param optWr opt wrapper object
     * @param sStrat search strategy
     */
int displaySearchAlgorithms(const nloptwr::NLOptWrapper& optWr, const nloptwr::NLOptWrSStrat& sStrat );

    /**
     * diplays the result of the optimization
     * @param optWr opt wrapper object
     * @param opt_stat status of result of optimization
     * @param minf optimized value
     */
int displayResult(const nloptwr::NLOptWrapper& optWr, nlopt::result opt_stat, double minf);

}

#endif // O_DISPLAY_H
