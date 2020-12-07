// -*- C++ -*-
//
// Package:    VertexConstraintProducer
// Class:      VertexConstraintProducer
// 
/**\class VertexConstraintProducer VertexConstraintProducer.cc RecoTracker/ConstraintProducerTest/src/VertexConstraintProducer.cc

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Giuseppe Cerati
//         Created:  Tue Jul 10 15:05:02 CEST 2007
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

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
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
//
//
// class decleration
//

class VertexConstraintProducer: public edm::EDProducer {
public:
  explicit VertexConstraintProducer(const edm::ParameterSet&);
  ~VertexConstraintProducer();

private:
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override ;
      
  // ----------member data ---------------------------
  const edm::InputTag srcTrkTag_;
  edm::EDGetTokenT<reco::TrackCollection> trkToken_;

  //const edm::InputTag srcMuonTag_;
  //edm::EDGetTokenT<std::vector<reco::Muon>> muonToken_;
  //
  std::string secVtx_;
  

  const edm::InputTag srcVtxTag_;
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;

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
VertexConstraintProducer::VertexConstraintProducer(const edm::ParameterSet& iConfig) :
srcTrkTag_(iConfig.getParameter<edm::InputTag>("srcTrk")),
//srcMuonTag_(iConfig.getParameter<edm::InputTag>("srcMuon")),
secVtx_(iConfig.getParameter<std::string>("secondaryVtxInfo")),
srcVtxTag_(iConfig.getParameter<edm::InputTag>("srcVtx"))
{
  //declare the consumes
  trkToken_ = consumes<reco::TrackCollection>(edm::InputTag(srcTrkTag_));
  //muonToken_ = consumes<std::vector<reco::Muon>>(edm::InputTag(srcMuonTag_));
  vtxToken_ = consumes<reco::VertexCollection>(edm::InputTag(srcVtxTag_));

  //register your products
  produces<std::vector<VertexConstraint> >();
  produces<TrackVtxConstraintAssociationCollection>();
  //produces<reco::TrackCollection>();


  //now do what ever other initialization is needed
}


VertexConstraintProducer::~VertexConstraintProducer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void VertexConstraintProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;

  std::cout<<"fileName: "<<secVtx_<<std::endl;

   const char *FileName;
   FileName = secVtx_.c_str();

  char SVInfofile[100];

  sprintf(SVInfofile,"%s",FileName);









   std::ifstream read(SVInfofile); 
   int eventNum;
   double xpos,ypos,zpos,xerr,yerr,zerr,xyerr,yzerr,zxerr;

   int realEventNum = iEvent.id().event();

   while(!read.eof()){

   read>>eventNum>>xpos>>ypos>>zpos>>xerr>>yerr>>zerr>>xyerr>>yzerr>>zxerr;
 
   if(realEventNum == eventNum)break;

  }
   



  Handle<reco::TrackCollection> theTCollection;
  iEvent.getByToken(trkToken_, theTCollection);


  //Handle<std::vector<reco::Muon>> theMuons;
  //iEvent.getByToken(muonToken_, theMuons);
  //
  //if(theMuons.isValid())reco::TrackRef trackRef=theMuons.innerTrack();

  //std::unique_ptr<reco::TrackCollection> myTracks(new reco::TrackCollection());

  

  Handle<reco::VertexCollection> theVertexHandle;
  iEvent.getByToken(vtxToken_, theVertexHandle);

  edm::RefProd<std::vector<VertexConstraint> > rPairs = iEvent.getRefBeforePut<std::vector<VertexConstraint> >();
  std::unique_ptr<std::vector<VertexConstraint> > pairs(new std::vector<VertexConstraint>);
  std::unique_ptr<TrackVtxConstraintAssociationCollection> output(new TrackVtxConstraintAssociationCollection(theTCollection, rPairs));


  //for(std::vector<reco::Muon>::const_iterator mu=theMuons->begin(); mu!=theMuons->end(); ++mu) 
  //{ 
  // reco::TrackRef trackRef=mu->innerTrack();
  // myTracks->push_back(*trackRef);


   //}


  int index = 0;
  
  //primary vertex extraction

  if (theVertexHandle->size()>0){
    const reco::Vertex& pv = theVertexHandle->front();

    for (reco::TrackCollection::const_iterator i=theTCollection->begin(); i!=theTCollection->end();i++) {

        //std::cout<<"muon inner tracks"<<std::endl;    

       // VertexConstraint tmp(
       //  GlobalPoint(pv.x(), pv.y(), pv.z()),
       //  GlobalError(
       // pv.covariance(0, 0),
       // pv.covariance(1, 0), pv.covariance(1, 1),
       //  pv.covariance(2, 0), pv.covariance(2, 1), pv.covariance(2, 2)
      // )
      // );



       //secondary vertex constraint
       VertexConstraint tmp(
        GlobalPoint(xpos, ypos, zpos),
        GlobalError(
        xerr,
        xyerr, yerr,
        zxerr, yzerr, zerr
        )
        );





      pairs->push_back(tmp);
      output->insert(reco::TrackRef(theTCollection,index), edm::Ref<std::vector<VertexConstraint> >(rPairs,index) );
      index++;
    }
  }

  iEvent.put(std::move(pairs));
  iEvent.put(std::move(output));
}

// ------------ method called once each job just after ending the event loop  ------------
void VertexConstraintProducer::endJob() {}

//define this as a plug-in
DEFINE_FWK_MODULE(VertexConstraintProducer);
