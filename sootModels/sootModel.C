#include "sootModel.H"
#include <iostream>

#include "nucleationModel_NONE.H"
#include "nucleationModel_LL.H"
#include "nucleationModel_LIN.H"
#include "nucleationModel_LINA1.H"
#include "nucleationModel_PAH.H"
#include "nucleationModel_MB.H"
#include "nucleationModel_FAIR.H"
#include "nucleationModel_AJ_RED.H"
#include "nucleationModel_BROWN.H"

#include "growthModel_NONE.H"
#include "growthModel_LL.H"
#include "growthModel_LIN.H"
#include "growthModel_HACA.H"
#include "growthModel_MB.H"
#include "growthModel_FAIR.H"

#include "oxidationModel_NONE.H"
#include "oxidationModel_LL.H"
#include "oxidationModel_HACA.H"
#include "oxidationModel_LEE_NEOH.H"
#include "oxidationModel_NSC_NEOH.H"
#include "oxidationModel_OPTJ.H"
#include "oxidationModel_OPTG.H"
#include "oxidationModel_MB.H"
#include "oxidationModel_FAIR.H"
#include "oxidationModel_AJ_RED.H"
#include "oxidationModel_BROWN.H"

#include "coagulationModel_NONE.H"
#include "coagulationModel_FM.H"
#include "coagulationModel_CONTINUUM.H"
#include "coagulationModel_HM.H"
#include "coagulationModel_FUCHS.H"

#include "tarModel_NONE.H"
#include "tarModel_AJ_RED.H"
#include "tarModel_BROWN.H"

using namespace std;
using namespace soot;

////////////////////////////////////////////////////////////////////////////////
///
/// Constructor taking pointers to chemistry models as input.
/// User creates these pointers nominally by "new-ing" them.
///
/// @param nsoot_ \input number of soot variables (moments or sections)
/// @param nucl_  \input pointer to nucleation model.
/// @param grow_  \input pointer to growth model.
/// @param oxid_  \input pointer to oxidation model.
/// @param coag_  \input pointer to coagulation model.
/// @param tar_   \input pointer to tar model.
///
////////////////////////////////////////////////////////////////////////////////

sootModel::sootModel(size_t            nsoot_,
                     nucleationModel  *nucl_,
                     growthModel      *grow_,
                     oxidationModel   *oxid_,
                     coagulationModel *coag_,
                     size_t            Ntar_,
                     tarModel         *tar_) :
        nsoot(nsoot_), Ntar(Ntar_), nucl(nucl_), grow(grow_), oxid(oxid_), coag(coag_), tar(tar_), 
        mechsNewedHere(false), sources(nsoot_, Ntar_) {
        checkSpec();
        nucl->SM = this;
        grow->SM = this;
        oxid->SM = this;
        coag->SM = this;
}


////////////////////////////////////////////////////////////////////////////////
///
/// Constructor taking enumerations names as input.
/// Chemistry pointers are created (new-ed) here based on those enumerations.
///
/// @param nsoot_ \input number of soot variables (moments or sections)
/// @param Nmech  \input one of enum class nucleationMech in sootDefs.h
/// @param Gmech  \input one of enum class growthMech in sootDefs.h
/// @param Omech  \input one of enum class oxidationMech in sootDefs.h
/// @param Cmech  \input one of enum class coagulationMech in sootDefs.h
///
////////////////////////////////////////////////////////////////////////////////

sootModel::sootModel(size_t          nsoot_,
                     nucleationMech  Nmech,
                     growthMech      Gmech,
                     oxidationMech   Omech,
                     coagulationMech Cmech,
                     size_t          Ntar_,
                     tarMech         Tmech) : nsoot(nsoot_), Ntar(Ntar_), 
                                              mechsNewedHere(true),
                                              sources(nsoot_, Ntar_) {

    //---------- set nucleation model

    switch (Nmech) {
        case nucleationMech::NONE   : nucl = new nucleationModel_NONE();   break;
        case nucleationMech::LL     : nucl = new nucleationModel_LL();     break;
        case nucleationMech::LIN    : nucl = new nucleationModel_LIN();    break;
        case nucleationMech::LINA1  : nucl = new nucleationModel_LINA1();  break;
        case nucleationMech::PAH    : nucl = new nucleationModel_PAH();    break;
        case nucleationMech::MB     : nucl = new nucleationModel_MB();     break;
        case nucleationMech::FAIR   : nucl = new nucleationModel_FAIR();   break;
        case nucleationMech::AJ_RED : nucl = new nucleationModel_AJ_RED(); break;
        case nucleationMech::BROWN  : nucl = new nucleationModel_BROWN();  break;
                                    
        default: throw domain_error("Invalid nucleation model requested");
    }

    //---------- set growth model

    switch (Gmech) {
        case growthMech::NONE : grow = new growthModel_NONE(); break;
        case growthMech::LL   : grow = new growthModel_LL();   break;
        case growthMech::LIN  : grow = new growthModel_LIN();  break;
        case growthMech::HACA : grow = new growthModel_HACA(); break;
        case growthMech::MB   : grow = new growthModel_MB();   break;
        case growthMech::FAIR : grow = new growthModel_FAIR(); break;
        
        default: throw domain_error("Invalid growth model requested");
    }

    //---------- set oxidation model

    switch (Omech) {
        case oxidationMech::NONE     : oxid = new oxidationModel_NONE();     break;
        case oxidationMech::LL       : oxid = new oxidationModel_LL();       break;
        case oxidationMech::HACA     : oxid = new oxidationModel_HACA();     break;
        case oxidationMech::LEE_NEOH : oxid = new oxidationModel_LEE_NEOH(); break;
        case oxidationMech::NSC_NEOH : oxid = new oxidationModel_NSC_NEOH(); break;
        case oxidationMech::OPTJ     : oxid = new oxidationModel_OPTJ();     break;
        case oxidationMech::OPTG     : oxid = new oxidationModel_OPTG();     break;
        case oxidationMech::MB       : oxid = new oxidationModel_MB();       break;
        case oxidationMech::FAIR     : oxid = new oxidationModel_FAIR();     break;
        case oxidationMech::AJ_RED   : oxid = new oxidationModel_AJ_RED();   break;
        case oxidationMech::BROWN    : oxid = new oxidationModel_BROWN();    break;
        
        default: throw domain_error("Invalid oxidation model requested");
    }

    //---------- set coagulation model

    switch (Cmech) {
        case coagulationMech::NONE      : coag = new coagulationModel_NONE();      break;
        case coagulationMech::FM        : coag = new coagulationModel_FM();        break;
        case coagulationMech::CONTINUUM : coag = new coagulationModel_CONTINUUM(); break;
        case coagulationMech::HM        : coag = new coagulationModel_HM();        break;
        case coagulationMech::FUCHS     : coag = new coagulationModel_FUCHS();     break;
        
        default: throw domain_error("Invalid coagulation model requested");
    }

    //---------- set tar model

    switch (Tmech) {
        case tarMech::NONE   : tar = new tarModel_NONE();    break;
        case tarMech::AJ_RED : tar = new tarModel_AJ_RED();  break;
        case tarMech::BROWN  : tar = new tarModel_BROWN();   break;

        default: throw domain_error("Invalid tar model requested");
    }

    //----------- 

    nucl->SM = this;
    grow->SM = this;
    oxid->SM = this;
    coag->SM = this;
    tar ->SM = this;

    //----------- 

    checkSpec();
}

////////////////////////////////////////////////////////////////////////////////
///
/// Verify the specification, HACA validity.
///
////////////////////////////////////////////////////////////////////////////////

void sootModel::checkSpec() {

    if((grow->mechType==growthMech::HACA    && oxid->mechType!=oxidationMech::HACA) ||
       (oxid->mechType==oxidationMech::HACA && grow->mechType!=growthMech::HACA))
        cerr << endl
             << "**********************************************************************" << endl
             << "Warning: HACA surface growth should pair with HACA oxid and vice-versa" << endl
             << "**********************************************************************" << endl << endl;
        //throw domain_error("Invalid model combination: HACA surface growth must pair with HACA oxid and vice-versa");
}
