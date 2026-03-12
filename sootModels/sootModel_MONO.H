#pragma once

#include "sootModel.H"
#include "sootDefs.H"
#include "state.H"
#include "tarModel_NONE.H"

namespace soot {

///////////////////////////////////////////////////////////////////////////////
///
/// Monodispersed model (MONO)
///
///////////////////////////////////////////////////////////////////////////////

class sootModel_MONO : public sootModel {

    //////////////// DATA MEMBERS /////////////////////

    //////////////// MEMBER FUNCTIONS /////////////////

public:

    virtual void setSourceTerms(state &state);
    
    virtual double pahSootCollisionRatePerDimer(const state &state, const double mDimer) {return 0;} 

    //////////////// CONSTRUCTOR FUNCTIONS ////////////

    sootModel_MONO(size_t            nsoot_,
                   nucleationModel  *nucl_,
                   growthModel      *grow_,
                   oxidationModel   *oxid_,
                   coagulationModel *coag_,
                   size_t            Ntar_ = 0,
                   tarModel         *tar_ = new tarModel_NONE );

    sootModel_MONO(size_t           nsoot_,
                   nucleationMech   Nmech,
                   growthMech       Gmech,
                   oxidationMech    Omech,
                   coagulationMech  Cmech,
                   size_t            Ntar_ = 0,
                   tarMech          Tmech = tarMech::NONE );

    virtual ~sootModel_MONO() {};

};
} // namespace soot
