// -*- C++ -*-
//
// Package:    CFCSimAnalysis
// Class:      CFCSimAnalysis
// 
/**\class CFCSimAnalysis CFCSimAnalysis.cc tmp/CFCSimAnalysis/plugins/CFCSimAnalysis.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  cowden
//         Created:  Fri, 28 Feb 2014 19:59:59 GMT
// $Id$
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

// include ROOT stuff
#include "TTree.h"


//
// class declaration
//

class CFCSimAnalysis : public edm::EDAnalyzer {
   public:
      explicit CFCSimAnalysis(const edm::ParameterSet&);
      ~CFCSimAnalysis();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    void clear();

      // ----------member data ---------------------------

  // handle for the TFileService
  edm::Service<TFileService> m_fs;

  edm::InputTag m_CFChits; 

  // output ntuple via TFileService m_fs
  TTree *m_tree; 

  // output ntuple branches
  unsigned m_cfc_N;  // number of sim hits
  std::vector<double> m_cfc_e;
  std::vector<double> m_cfc_em;
  std::vector<double> m_cfc_had;
  std::vector<double> m_cfc_t;
  std::vector<double> m_cfc_depth;
  std::vector<double> m_cfc_id;
  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CFCSimAnalysis::CFCSimAnalysis(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


CFCSimAnalysis::~CFCSimAnalysis()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
CFCSimAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

  clear(); // clear branch vectors

  // get a handle on CFCHits
  edm::InputTag CFCHitTag("g4SimHits","CFCHits");
  edm::Handle<edm::PCaloHitContainer> cfcHits;
  iEvent.getByLabel(CFCHitTag,cfcHits);


  // fill in information from sim hits
  edm::PCaloHitContainer::const_iterator cit = cfcHits->begin();
  edm::PCaloHitContainer::const_iterator citEnd = cfcHits->end();
  for ( ; cit != citEnd; cit++ ) {

    m_cfc_e.push_back( (*cit).energy() );
    m_cfc_em.push_back( (*cit).energyEM() );
    m_cfc_had.push_back( (*cit).energyHad() );
    m_cfc_t.push_back( (*cit).time() );
    m_cfc_depth.push_back( (*cit).depth() );
    m_cfc_id.push_back( (*cit).id() );
    m_cfc_N++;
    
  }

  m_tree->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
CFCSimAnalysis::beginJob()
{

  m_tree = m_fs->make<TTree>("SimTree","SimTree");

  m_tree->Branch("cfc_N",&m_cfc_N,"cfc_N/i");
  m_tree->Branch("cfc_e",&m_cfc_e);
  m_tree->Branch("cfc_em",&m_cfc_em);
  m_tree->Branch("cfc_had",&m_cfc_had);
  m_tree->Branch("cfc_t",&m_cfc_t);
  m_tree->Branch("cfc_depth",&m_cfc_depth);
  m_tree->Branch("cfc_id",&m_cfc_id);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
CFCSimAnalysis::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
CFCSimAnalysis::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
CFCSimAnalysis::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
CFCSimAnalysis::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
CFCSimAnalysis::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

void CFCSimAnalysis::clear() {
  // clear all of the branch variables
  
  m_cfc_N = 0U;
  m_cfc_e.clear();
  m_cfc_em.clear();
  m_cfc_had.clear();
  m_cfc_t.clear();
  m_cfc_depth.clear();
  m_cfc_id.clear();

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CFCSimAnalysis::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CFCSimAnalysis);
