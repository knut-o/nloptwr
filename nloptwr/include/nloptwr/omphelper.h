
#ifndef OMP_HELPER_H
#define OMP_HELPER_H

namespace utl {

/**
 * This class is an OpenMP helper for convenience
 */
class OmpHelper {

public:
  /**
   * constructor
   */
  OmpHelper();

  /**
   * destructor
   */
  ~OmpHelper();

  /**
   * set start for mesurmen of time
   */
  void setStart();

  /**
   * set end for measurement of time
   */
  void setEnd();

  /**
   * get duration (timeEnd-timeBegin)
   * @return duration
   */
  double getDuration() const;

  /**
   * get the number of processors
   * @return number of processors
   */
  static int getNumProcs();

  /**
   * get the number of working threads
   * @return number of working threads
   */
  static int getNumThreads();

  /**
   * get the current number of thread
   * @return current number of thread
   */
  static int getThreadNum();

private:
#ifdef _OPENMP
  double
#else
  clock_t
#endif
      /// start time
      tStart,

      /// end time
      tEnd;
};

} // namespace utl

#endif // OMP_HELPER_H
