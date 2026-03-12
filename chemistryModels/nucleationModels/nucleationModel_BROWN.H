#pragma once

#include "nucleationModel.H"
#include "state.H"

namespace soot {

/////////////////////////////////////////////////////////////////////
///
/// Brown soot formation (nucleation) model 
///
/////////////////////////////////////////////////////////////////////

class nucleationModel_BROWN : public nucleationModel {

//////////////// DATA MEMBERS //////////////////////// 

public:

//////////////// MEMBER FUNCTIONS //////////////////// 

    double getNucleationSootRate(state &state) override;

/////////////// CONSTRUCTOR FUNCTIONS //////////////// 

    nucleationModel_BROWN();
    ~nucleationModel_BROWN() override = default;
};    
}
