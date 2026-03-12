#pragma once

#include "oxidationModel.H"
#include "state.H"

namespace soot {

//////////////////////////////////////////////////////////////////////
///
/// Oxidation by Moss & Brookes
///
//////////////////////////////////////////////////////////////////////

class oxidationModel_MB : public oxidationModel {

////////////////// DATA MEMBERS //////////////////// 

public:

////////////////// MEMBER FUNCTIONS //////////////// 

public:

    double getOxidationSootRate(const state &state) const override;

////////////////// CONSTRUCTOR FUNCTIONS /////////// 

public:

    oxidationModel_MB();
    ~oxidationModel_MB() override = default;

};
} // namespace soot
