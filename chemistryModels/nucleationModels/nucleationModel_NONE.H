#pragma once

#include "nucleationModel.H"
#include "state.H"

namespace soot {

////////////////////////////////////////////////////////////////////////////////
///
/// Nucleation: no model
///
////////////////////////////////////////////////////////////////////////////////

class nucleationModel_NONE : public nucleationModel {

    //////////////// DATA MEMBERS /////////////////////

private:

    //////////////// MEMBER FUNCTIONS /////////////////

public:

    double getNucleationSootRate(state &state) override { return 0; };

    //////////////// CONSTRUCTOR FUNCTIONS ////////////

    nucleationModel_NONE() : nucleationModel() {
        nucleationRxnRatios[(int)gasSp::C]    =  1;  // to avoid NAN in case of nucleationMech::NONE
        mechType = nucleationMech::NONE;
    };
    ~nucleationModel_NONE() override = default;

};
} // namespace soot
