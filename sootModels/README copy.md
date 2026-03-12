# SootLib overview

[SootLib](https://github.com/BYUignite/sootlib.git) is an open-source C++ library that computes soot source terms using moment-based particle size distribution models for combustion CFD simulations. Detailed code documentation for the original library is available [here](https://ignite.byu.edu/sootlib_documentation/). Click [here](https://www.sciencedirect.com/science/article/pii/S2352711023000717) for the paper published in SoftwareX.

**This version of SootLib is designed specifically for use in OpenFOAM.**

# Dependencies and installation

The code is intended to be built and used on within an existing, compiled OpenFOAM environment. It was developed using OpenFOAM-v2412. 

Required software:
* OpenFOAM

## Build instructions

1. Clone repo to `$WM_PROJECT_USER_DIR/src`, or a user-specific location for custom OpenFOAM libraries
1. Navigate to the top-level directory
3. Build SootLib: `./Allwmake`
4. Access SootLib from the `$FOAM_USER_LIBBIN/libsootlib.dylib`


# Using SootLib

The SootLib library consists of two main object classes that users can interact with: `sootModel` and `state`, both of which are contained within the `soot` namespace. The `state` object holds user-specified details about the current thermodynamic state in which the soot chemistry occurs, including variables such as temperature, pressure, and gas species mass fractions. The `sootModel` object contains information about the selected models and mechanisms and performs the calculations that generate moment source terms. In the context of a traditional CFD simulation, the `state` object would be updated via the `setState` function at each individual time step and/or grid point, while the `sootModel` parameters only need to be specified once when the object is created, and then its `calcSourceTerms` function invoked at each step following the `setState` update. The resulting moment source terms and gas species source terms can be accessed via the `sootModel` object. Refer to `examples/simple_example.cc` for a basic example of setting up the objects, calculating source terms, and retrieving values.

## Example workflow
1. Create `sootModel` object, specifying the desired soot chemistry and PSD mechanism.
2. Create an empty `state` object.
3. Populate the `state` object with the thermodynamic conditions using the `setState` function.
4. Calculate the soot source terms using the `calcSourceTerms` function, which takes a reference to a `state` object as its input. 
5. Retrieve the desired source terms from the `sootModel` object. 

In the case of a temporally or spatially evolving simulation, only steps 3–5 need be performed at each individual step. SootLib does not store previously calculated values, so source terms must be retrieved at each step or otherwise lost.  

