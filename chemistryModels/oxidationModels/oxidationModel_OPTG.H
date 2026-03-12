#pragma once

#include "oxidationModel.H"
#include "state.H"

namespace soot {

//////////////////////////////////////////////////////////////////////////////////
///
/// Guo et al. optimized O2 & OH oxidation model
///
//////////////////////////////////////////////////////////////////////////////////

class oxidationModel_OPTG : public oxidationModel {

//////////////// DATA MEMBERS /////////////////////

public:

//////////////// MEMBER FUNCTIONS /////////////////

public:

     double getOxidationSootRate(const state &state) const override;

//////////////// CONSTRUCTOR FUNCTIONS ////////////

public:

     oxidationModel_OPTG();
    ~oxidationModel_OPTG() override = default;
};
} // namespace soot
