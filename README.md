# Project NLOptWR

The name of program ***NLOptWR*** is an abbreviation of an wrapper of ***NLOPT***.

## Description

The program ***NLOptWR*** is an extension to the Program **NLOPT**.
Initially it was started a an experiment to use the Program **NLOPT** more easyly.

The program has the following functionalities:

1. ***C++ API***
2. ***rule based selection of algorithms***
3. ***numerical calculation of gradients*** in parallel with **OpenMP** if needed


Because of numerical calculation of gradients a lot of algorithms is available.
In case of many parameters the calculation can be done in parallel efficient.

The program does not require a implementation of object serialization with ***boost***.

### C++ API

The class ***oif::OptFknBase*** is an interface class can be derived.

For less implementation the derived class ***oif::OptFknClass*** can be derived.

The remaining following functions have to be implemented:

<ol start="1">
  <li>virtual <b>double optFktn ( const std::vector<double>& x, std::vector<double>& c )</b>
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

The function return the function value.

* "x" contains the function arguments.
* "c" are the constraints (c[i] <= 0).
* "lb" is the lower bound
* "ub" is the upper bound
* "xInit" is the initial value

The class ***PDemo01*** in the file "nloptwr_test1.cpp" in a example.

### Rule based selection of algorittms

An instance of parameter class ***nloptwr::NLOptWrSStrat*** can be used to select the search methods.
The constructor of this class has three arguments:

1. ***nloptwr::SSTRAT***
        // enum of search strategies
2. ***useGrad***
        // usage of gradient methods (default=true)
3. ***useAugLagBeforeMlsl***
        // prefer Augmented Lagrangian before MLSL (default=true)

The emun class <b>nloptwr::SSTRAT</b> can have following valid values:

 * ***L*** = Local search strategy is preferred
 * ***G*** = Global search strategy is chosen
 * ***R*** = Random search strategy is chosen (NLOPT has no acceleration with gradient at all)
 * ***GM*** = Global meta search strategy is chosen
 * ***LM*** = Local meta search strategy is chosen (sometimes a global search strategy is selected)

The selected methods also depends on the number of parameters (see ***nloptwr::NLOptWrSearchAlgoParam3*** and ***nloptwr::NLOptWrParamFactory***).

### Calculation of gradients

The numerical calculation can be activated with the flag ***useGrad*** of ***nloptwr::NLOptWrSStrat***.
The numerical calculation is done automatically in parallel with ***OpenMP***.
Necessary is a correct working clone method.

The method of numerical derivation is the 2-point formula.


## Getting Started

### Dependencies

The package needs the following packages:

* C-compiler (gcc and g++) und libc
* OpenMP (libomp-dev, libgomp1)
* NLOPT installation with C++ extensions (libnlopt-dev,  libnlopt0)

The the Debian ".deb" 64-bit binary package (*Bullseye*) has the following dependencies:
libc6, libgcc-s1, libgomp1, libnlopt-cxx0, libstdc++6

Other OS can try to build it from source.

The directory "cmake" contains some files (FindNLOPT.cmake, FindNLOPTCXX.cmake  FindNLOPTWR.cmake, FindOpenMP.cmake).

### Installing

You can get this program from git repository.
Under LINUX (Debian or RPM based systems, X86, 64-bit)
you can install the library and the demo-programs from package.

Other system can install the package from source.

### Executing program

#### Execution the prcompiled demo programs

The precompiled demo programs can be launched manually:

* nloptwr_test01
* nloptwr_test02
* nloptwr_test03
* nloptwr_test04
* nloptwr_test05
* nloptwr_test06
* selalg_test

The test can be started with "ctest" after "cmake" and "build".

#### Create your own optimization program with NLOptWR

##### Create your own class with your tatget function

The class ***oif::OptFknClass*** can be derived.

The class "opttest::PDemo01" in the file "nloptwr_test1.cpp" is an expample of such implementation.

In the same file the usage of the is explained:

1. deturmine some parameters (number of threads, time limit, limit of calculations, initial result)

2. create instance of search strategy (nloptwr::NLOptWrSStrat)

3. create an instance of class with target function (opttest::PDemo01)

4. create an instance of nloptwr::NLOptWrapper

5. start optimization

6. get the solution

7. other tasks (display the result, check the result, print the selected algorithms)

## Help

Any advise for common problems or issues.

## Authors

Contributors and contact info:
[Knut Opherden](knut.opherden@hotmail.com)
[knut.opherden@hotmail.com](knut.opherden@hotmail.com)

## Links

[NLOpt](https://nlopt.readthedocs.io/en/latest/#nlopt_1)

[NLOpt Algorithms](https://nlopt.readthedocs.io/en/latest/NLopt_Algorithms/)

[NLOpt C++ Reference](https://nlopt.readthedocs.io/en/latest/NLopt_C-plus-plus_Reference/)

[OpenMP](https://de.wikipedia.org/wiki/OpenMP)

[PaGMO](https://esa.github.io/pagmo2/)

[GENEVA on Hithub](https://github.com/gemfony/geneva)

[GENEVA](https://www.gemfony.eu/)


## Version History

* 0.1
    * Initial Release

## License

This project is licensed under the [GNU AFFERO GENERAL PUBLIC LICENSE, Version 3](https://www.gnu.org/licenses/agpl-3.0.txtl) from 19 November 2007 - see the [LICENSE](https://github.com/knut-o/nloptwr/blob/main/LICENSE) file for details.


## Acknowledgments

