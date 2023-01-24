# Project NLOptWr

The name of program ***NLOptWr*** is an **easy to use** C++ wrapper of ***NLOpt*** with a simple parallelization.

The program (library) is still under development.

## Description

The program ***NLOptWr*** is an extension to the Program [NLOpt](https://nlopt.readthedocs.io/en/latest/#nlopt_1).

Initially it was started a an experiment to use the Program [NLOpt](https://nlopt.readthedocs.io/en/latest/#nlopt_1) more easily.

The program has the following functionalities:

1. ***C++ API***
2. ***rule based selection of algorithms***
3. ***numerical calculation of gradients*** in parallel
with [OpenMP](https://de.wikipedia.org/wiki/OpenMP) if needed

Because of numerical calculation of gradients a lot of algorithms is available.

In case of many parameters the calculation
the calculation of gradients can be done in parallel.

The program does not require a implementation of object serialization with the  [boost](https://www.boost.org/doc/libs/1_80_0/libs/serialization/doc/index.html) library.

### C++ API

The class ***oif::OptFknBase*** is an interface class can be derived.

For less implementation the derived class ***oif::OptFknClass*** can be derived.

The remaining following functions have to be implemented:

<ol start="1">
  <li>virtual <b>void optFktn ( const std::vector<double>& x, std::vector<double>& fc )</b>
     override;
    // target function
</li>
  <li>virtual <b>void initialize ( double lb, double ub, double xInit )</b>;
    // initialize function
</li>
<li>virtual <b>oif::OptFknBase* clone() const</b>
    override;
    // clone method
</li>
</ol>

**The API has changed**. The function is "void" now.

* "x" contains the function arguments.
* "fc" are the result with function values; fc[0] it the funktion value, followed by the (equality constraints (fc[i>0] == 0) und the no-equality constraints (fc[i>0] <= 0).
* "lb" is the lower bound
* "ub" is the upper bound
* "xInit" is the initial value

The class ***PDemo01*** in the file "NLOptWr_test1.cpp" in a example.

### Rule based selection of algorittms

An instance of parameter class ***NLOptWr::NLOptWrSStrat*** can be used to select the search methods.
The constructor of this class has three arguments:

1. ***NLOptWr::SSTRAT***
        // enum of search strategies
2. ***useGrad***
        // usage of gradient methods (default=true)
3. ***useAugLagBeforeMlsl***
        // prefer Augmented Lagrangian before MLSL (default=true)

The emun class <b>NLOptWr::SSTRAT</b> can have following valid values:

 * ***L*** = Local search strategy is preferred
 * ***G*** = Global search strategy is chosen
 * ***R*** = Random search strategy is chosen (NLOPT has no acceleration with gradient at all)
 * ***GM*** = Global meta search strategy is chosen
 * ***LM*** = Local meta search strategy is chosen (sometimes a global search strategy is selected)

The selected methods also depends on the number of parameters (see ***NLOptWr::NLOptWrSearchAlgoParam3*** and ***NLOptWr::NLOptWrParamFactory***).

### Calculation of gradients

The numerical calculation can be activated with the flag ***useGrad*** of ***NLOptWr::NLOptWrSStrat***.
The numerical calculation is done automatically in parallel with ***OpenMP***.
Necessary is a correct working clone method.

The method of numerical derivation is the 2-point formula.


# Getting Started

## Dependencies

The package needs the following packages:

* C-compiler (gcc and g++) und libc
* OpenMP (libomp-dev, libgomp1)
* NLOPT installation with C++ extensions (libnlopt-dev,  libnlopt0)

The the Debian ".deb" 64-bit binary package (*Bullseye*) has the following dependencies:
libc6, libgcc-s1, libgomp1, libnlopt-cxx0, libstdc++6

Other OS can try to build it from source.

The directory "cmake" contains some files (FindNLOPT.cmake, FindNLOPTCXX.cmake  FindNLOptWr.cmake, FindOpenMP.cmake).

## Installation from source

To build the library and the demo programs a C++ compiler ([gcc](https://gcc.gnu.org/) or clang) and [CMake](https://cmake.org/) are needed.
On LINUX thise packages can be install with the package managers of the system.

It is necessary to install the **NLOpt** library.
This library can be installed with [vcpkg](https://github.com/microsoft/vcpkg). In this case the environment variable **CMAKE_TOOLCHAIN_FILE** (or **VCPKG_ROOT**) should be set properly.

On LINUX the **NLOpt** package can be install with the package managers of the system as well.

The file [.github/workflows/build.yml](https://github.com/knut-o/nloptwr/blob/main/.github/workflows/build.yml) shows how NLoptWr can be build and tested. 
 
### LINUX

The sources can be downloaded
and build with the following commands:

```bash
git clone https://github.com/knut-o/NLOptWr.git
cd NLOptWr
mkdir build
cd    build

rm -rf ../build/* && cmake -Wno-dev -DCMAKE_BUILD_TYPE=Release  .. &&  cmake --build . --config Release && ctest -C Release
```

After a successful compilation the tests can be launched:

```bash
ctest
../lib/test/NLOptWrtest/NLOptWr_test6_grafik.sh
```
This script generates a file
[nloptwr_test6.pdf](docu/nloptwr_test6.pdf)
and shows it in a viewer.

Packages can be build as well:
```bash
cpack
```

### Windows with Cygwin

The installation is similar to the LINUX installation (see above).

### Windows VisualC

The [CMake](https://cmake.org/) program has to be installed as well.
It is necessary to install the NLOpt library.
This library can be installed with [vcpkg](https://github.com/microsoft/vcpkg).

```bash
vcpkg.exe install nlopt
```


```bash
rm -r ..\build\* ; cmake -DCMAKE_BUILD_TYPE=Debug .. ; cmake --build . -j 4 ; ctest -C Debug
```

### Executing program

#### Execution the precompiled demo programs

The precompiled demo programs can be launched manually in the test (NLOptWrtest) directory:

```bash
./NLOptWr_test01
./NLOptWr_test02
./NLOptWr_test03
./NLOptWr_test04
./NLOptWr_test05
./NLOptWr_test06
./selalg_test
```

The test can be started with "**ctest**" after the build with "**cmake**".

### Create your own optimization program with NLOptWr

#### Create your own class with your tatget function

The class ***oif::OptFknClass*** can be derived.

The class "opttest::PDemo01" in the file "NLOptWr_test1.cpp" is an expample of such implementation.

In the same file the usage of the is explained:

1. deturmine some parameters (number of threads, time limit, limit of calculations, initial result)

2. create instance of search strategy (NLOptWr::NLOptWrSStrat)

3. create an instance of class with target function (opttest::PDemo01)

4. create an instance of NLOptWr::NLOptWrapper

5. start optimization

6. get the solution

7. other tasks (display the result, check the result, print the selected algorithms)

The progams can be linked with **-lnloptwr -lnlopt -lm**.

## Alternaltives

There are many alternatives.
Two of them are the following:

* [PaGMO](https://github.com/esa/pagmo2)
  is a C++ scientific library for massively parallel optimization.
 [NLOpt](https://nlopt.readthedocs.io/en/latest/#nlopt_1) can be used with [PaGMO](https://github.com/esa/pagmo2).

* For large scale problems [GENEVA](https://github.com/gemfony/geneva) seems to be a good choice. The availability of recent versions is shown at the [dashboard](https://my.cdash.org/index.php?project=Geneva).


## Help

Any advise for common problems or issues.

## Authors

Contributors and contact info:
[Knut Opherden](knut.opherden@hotmail.com)

## Links

[NLOpt](https://nlopt.readthedocs.io/en/latest/#nlopt_1)

[NLOpt Algorithms](https://nlopt.readthedocs.io/en/latest/NLopt_Algorithms/)

[NLOpt C++ Reference](https://nlopt.readthedocs.io/en/latest/NLopt_C-plus-plus_Reference/)


## Version History

* 0.1
    * Initial Release

## License

See the
[LICENSE](https://github.com/knut-o/NLOptWr/blob/main/LICENSE)
file for details.
