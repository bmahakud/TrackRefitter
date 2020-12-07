import sys

num=sys.argv[2]

print ("num: ",num)


import FWCore.ParameterSet.Config as cms

process = cms.Process("Refitting")

### Standard Configurations
#process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
#process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load('Configuration.Geometry.GeometryRecoDB_cff')
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

  


process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
# choose!
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data_GRun', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_GRun', '')
#auto:run2_mc_FULL



### Track refitter specific stuff
import RecoTracker.TrackProducer.TrackRefitter_cfi
#import CommonTools.RecoAlgos.recoTrackRefSelector_cfi
#process.mytkselector = CommonTools.RecoAlgos.recoTrackRefSelector_cfi.recoTrackRefSelector.clone()
#process.mytkselector.quality = ['highPurity']
#process.mytkselector.min3DLayer = 2
#process.mytkselector.ptMin = 0.5
#process.mytkselector.tip = 1.0

#import RecoTracker.TrackProducer.VertexConstraintProducer_cfi
#process.vertexConstraint = RecoTracker.TrackProducer.VertexConstraintProducer_cfi.vertexConstraint.clone()

process.myRefittedTracks = RecoTracker.TrackProducer.TrackRefitter_cfi.TrackRefitter.clone()
process.myRefittedTracks.src= 'innerTrkProd:innerTrackerMuons'#mytkselector'
process.myRefittedTracks.NavigationSchool = ''
process.myRefittedTracks.Fitter = 'FlexibleKFFittingSmoother'
process.myRefittedTracks.constraint = 'vertex'
process.myRefittedTracks.srcConstr = 'vertexConstraint'


### and an analyzer
process.trajCout = cms.EDAnalyzer('TrajectoryAnalyzer',
   trajectoryInput=cms.InputTag('myRefittedTracks')
)


process.innerTrkProd= cms.EDProducer("InnerMuonTrackProducer",
 #srcMuon =cms.InputTag("muons"),
 srcpatMuon = cms.InputTag("slimmedMuons")
)


process.vertexConstraint = cms.EDProducer(
    "VertexConstraintProducer",
    srcTrk = cms.InputTag("innerTrkProd:innerTrackerMuons"),
    secondaryVtxInfo = cms.string("SecVtxFile_2.dat"),
    srcVtx = cms.InputTag("offlinePrimaryVertices")
)




#fileNameIn = 'file:/eos/user/b/bimahaku/BstoMuMu_RECOFiles/step3_RAW2DIGI_L1Reco_RECO_%s.root'%(str(num))


fileNameIn_miniAOD ="file:/afs/cern.ch/user/b/bimahaku/public/BeamSpotRefit/Test2/New2/CMSSW_10_2_18/src/PrimarySecondary_RecoAndMiniAOD/BPH-RunIIAutumn18DRPremix-00048_MiniAODSIM.root"
fileNameIn_RECO = 'file:/afs/cern.ch/user/b/bimahaku/public/BeamSpotRefit/Test2/New2/CMSSW_10_2_18/src/PrimarySecondary_RecoAndMiniAOD/BPH-RunIIAutumn18DRPremix-00048_RECO.root'



#fileNameIn_miniAOD = 'file:/eos/user/b/bimahaku/BstoMuMu_RECO_miniAOD_Files/BstoMuMuFile-RunIIAutumn18MiniAOD_1.root'

#fileNameIn_RECO = 'file:/eos/user/b/bimahaku/BstoMuMu_RECOFiles/step3_RAW2DIGI_L1Reco_RECO_1.root'


fileNameout = 'file:/eos/user/b/bimahaku/BsToMuMuPrimaryVertexRefitTest_%s_fromaod.root'%(str(num))

process.source = cms.Source ("PoolSource",
                             fileNames=cms.untracked.vstring(fileNameIn_miniAOD),#'file:/eos/user/b/bimahaku/BstoMuMu_RECOFiles/step3_RAW2DIGI_L1Reco_RECO_1.root',
                             secondaryFileNames = cms.untracked.vstring(fileNameIn_RECO), 
                             skipEvents=cms.untracked.uint32(0)
                             )


process.TRACKS = cms.OutputModule("PoolOutputModule",
                                 outputCommands = cms.untracked.vstring('drop *_*_*_*',
                                                                        'keep recoTracks_*_*_*',
                                                                        'keep recoTrackExtras_*_*_*'),

                                fileName = cms.untracked.string(fileNameout)#'file:/eos/user/b/bimahaku/BsToMuMuPrimaryVertexRefitTest_1_fromaod.root')
                               )







process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

#process.Path = cms.Path(process.mytkselector+process.myRefittedTracks+process.trajCout)

#process.Path = cms.Path(process.mytkselector+process.vertexConstraint*process.myRefittedTracks+process.trajCout)

process.Path = cms.Path(process.innerTrkProd*process.vertexConstraint*process.myRefittedTracks+process.trajCout)


process.outpath = cms.EndPath(process.TRACKS)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)
