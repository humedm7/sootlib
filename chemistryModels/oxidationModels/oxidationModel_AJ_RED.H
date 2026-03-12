#pragma once

#include "oxidationModel.H"
#include "state.H"

namespace soot {

//////////////////////////////////////////////////////////////////////////////////
///
/// Oxidation by Alex Josephson (2020)
///
//////////////////////////////////////////////////////////////////////////////////

class oxidationModel_AJ_RED : public oxidationModel {

//////////////// DATA MEMBERS /////////////////////

public:

//////////////// MEMBER FUNCTIONS /////////////////

public:

     double getOxidationSootRate(const state &state) const override;

//////////////// CONSTRUCTOR FUNCTIONS ////////////

public:

     oxidationModel_AJ_RED();
    ~oxidationModel_AJ_RED() override = default;

};
} // namespace soot
