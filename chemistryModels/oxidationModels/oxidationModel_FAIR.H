#pragma once

#include "oxidationModel.H"
#include "state.H"

namespace soot {

//////////////////////////////////////////////////////////////////////////////////
///
/// Oxidation by Fairweather (1992)
///
//////////////////////////////////////////////////////////////////////////////////

class oxidationModel_FAIR : public oxidationModel {

//////////////// DATA MEMBERS /////////////////////

public:

//////////////// MEMBER FUNCTIONS /////////////////

public:

     double getOxidationSootRate(const state &state) const override;

//////////////// CONSTRUCTOR FUNCTIONS ////////////

public:

     oxidationModel_FAIR();
    ~oxidationModel_FAIR() override = default;

};
} // namespace soot
