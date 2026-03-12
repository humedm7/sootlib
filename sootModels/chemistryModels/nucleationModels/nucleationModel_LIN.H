#pragma once

#include "nucleationModel.H"
#include "state.H"

namespace soot {

//////////////////////////////////////////////////////////////////////////////////
///
/// Lindstedt nucleation model
///
//////////////////////////////////////////////////////////////////////////////////

class nucleationModel_LIN : public nucleationModel {

//////////////// DATA MEMBERS /////////////////////

private:

//////////////// MEMBER FUNCTIONS /////////////////

public:

    double getNucleationSootRate(state &state) override;

    //////////////// CONSTRUCTOR FUNCTIONS ////////////

    nucleationModel_LIN();
    ~nucleationModel_LIN() override = default;
};
} // namespace soot
