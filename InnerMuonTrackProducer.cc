// -*- C++ -*-
//
// Package:    InnerMuonTrackProducer
// Class:      InnerMuonTrackProducer
// 
/**\class InnerMuonTrackProducer InnerMuonTrackProducer.cc RecoTracker/ConstraintProducerTest/src/InnerMuonTrackProducer.cc

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Bibhuprasad Mahakud
//         Created:  29-10-2020
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/PatternTools/interface/TrackConstraintAssociation.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"


#include<vector>
//
//
// class decleration
//

class InnerMuonTrackProducer: public edm::EDProducer {
public:
  explicit InnerMuonTrackProducer(const edm::ParameterSet&);
  ~InnerMuonTrackProducer();

private:
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override ;
      
  // ----------member data ---------------------------

 // const edm::InputTag srcMuonTag_;
 // edm::EDGetTokenT<std::vector<reco::Muon>> muonToken_;

  const edm::InputTag srcpatMuonTag_;
  edm::EDGetTokenT<std::vector<pat::Muon>> patmuonToken_;




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
InnerMuonTrackProducer::InnerMuonTrackProducer(const edm::ParameterSet& iConfig) :
//srcMuonTag_(iConfig.getParameter<edm::InputTag>("srcMuon")),
srcpatMuonTag_(iConfig.getParameter<edm::InputTag>("srcpatMuon"))
{
  //declare the consumes
 // muonToken_ = consumes<std::vector<reco::Muon>>(edm::InputTag(srcMuonTag_));

  patmuonToken_ = consumes<std::vector<pat::Muon>>(edm::InputTag(srcpatMuonTag_));
  

  //register your products
  produces<reco::TrackCollection>("innerTrackerMuons");

  //now do what ever other initialization is needed
}


InnerMuonTrackProducer::~InnerMuonTrackProducer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void InnerMuonTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;


  Handle<std::vector<pat::Muon>> theMuons;
  iEvent.getByToken(patmuonToken_, theMuons);


  //std::unique_ptr<reco::TrackCollection> myTracks(new reco::TrackCollection());
  auto InnerMuons  = std::make_unique<reco::TrackCollection>();
  
 if (theMuons.isValid()){
  for(std::vector<pat::Muon>::const_iterator mu=theMuons->begin(); mu!=theMuons->end(); ++mu) 
  { 

   if ( mu->isGlobalMuon() && mu->pt() > 2.0) {
   reco::TrackRef trackRef=mu->innerTrack();
   //std::cout<<"trackpt:============================================================================= "<<trackRef->pt()<<std::endl;
   try{
   InnerMuons->push_back(*trackRef);
    }
   catch(...){}

  }}}



  iEvent.put(std::move(InnerMuons),"innerTrackerMuons");

}

// ------------ method called once each job just after ending the event loop  ------------
void InnerMuonTrackProducer::endJob() {}

//define this as a plug-in
DEFINE_FWK_MODULE(InnerMuonTrackProducer);
