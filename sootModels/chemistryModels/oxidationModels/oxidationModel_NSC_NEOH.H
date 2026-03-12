#pragma once

#include "oxidationModel.H"
#include "state.H"

namespace soot {

//////////////////////////////////////////////////////////////////////////////////
///
/// NSC (O2) + Neoh (OH) Oxidation model
///
//////////////////////////////////////////////////////////////////////////////////

class oxidationModel_NSC_NEOH : public oxidationModel {

//////////////// DATA MEMBERS /////////////////////

public:

//////////////// MEMBER FUNCTIONS /////////////////

public:

     double getOxidationSootRate(const state &state) const override;

//////////////// CONSTRUCTOR FUNCTIONS ////////////

public:

     oxidationModel_NSC_NEOH();
    ~oxidationModel_NSC_NEOH() override = default;
};
} // namespace soot
