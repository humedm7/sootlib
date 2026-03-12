#pragma once

#include "nucleationModel.H"
#include "state.H"
#include "coagulationModel_HM.H"

namespace soot {

//////////////////////////////////////////////////////////////////////////////////
///
/// PAH nucleation model (Blanquart)
///
//////////////////////////////////////////////////////////////////////////////////

class nucleationModel_PAH : public nucleationModel {

//////////////// DATA MEMBERS /////////////////////

public:

//////////////// MEMBER FUNCTIONS /////////////////

    double getNucleationSootRate(state &state) override;

//////////////// CONSTRUCTOR FUNCTIONS ////////////

     nucleationModel_PAH();
    ~nucleationModel_PAH() override = default;
};
} // namespace soot
