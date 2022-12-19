
#include "nloptwr/omphelper.h"

#ifdef _OPENMP
#include <omp.h>
#else

#include <cctime>
using namespace std;

#endif

namespace utl {

OmpHelper::OmpHelper() { setStart(); }

OmpHelper::~OmpHelper() {}

void OmpHelper::setStart() {
#ifdef _OPENMP
  tStart = omp_get_wtime();
#else
  tStart = clock();
#endif
}

void OmpHelper::setEnd() {
#ifdef _OPENMP
  tEnd = omp_get_wtime();
#else
  tEnd = clock();
#endif
}

double OmpHelper::getDuration() const {

  double result = (tEnd - tStart);

#ifndef _OPENMP
  result /= CLOCKS_PER_SEC;
#endif

  return result;
}

// static
// inline
int OmpHelper::getNumProcs() {

  return
#ifdef _OPENMP
      omp_get_num_procs()
#else
      1
#endif
          ;
}

// static
// inline
int OmpHelper::getNumThreads() {

  return
#ifdef _OPENMP
      omp_get_num_threads()
#else
      1
#endif
          ;
}

// static
// inline
int OmpHelper::getThreadNum() {

  return
#ifdef _OPENMP
      omp_get_thread_num()
#else
      0
#endif
          ;
}

} // namespace utl
