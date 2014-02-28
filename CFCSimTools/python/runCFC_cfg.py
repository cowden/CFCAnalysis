import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("IOMC.EventVertexGenerators.VtxSmearedGauss_cfi")
process.load("Geometry.HGCalCommonData.testCFCXML_cfi")
#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("Geometry.HcalCommonData.hcalNumberingInitialization_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load("SimG4Core.Application.g4SimHits_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout','cerr'),
    categories = cms.untracked.vstring('CaloSim', 
        'CFCSim', 'G4cerr', 'G4cout'),
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG'),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(10)
        ),
        DEBUG = cms.untracked.PSet(
            limit = cms.untracked.int32(10)
        ),
        CaloSim = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        CFCSim = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        G4cerr = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        G4cout = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
    ),
    cerr = cms.untracked.PSet(
        optionalPSet = cms.untracked.bool(True),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        noTimeStamps = cms.untracked.bool(False),
        FwkReport = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(1),
            limit = cms.untracked.int32(10000000)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(0)
        ),
        FwkJob = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(0)
        ),
        FwkSummary = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(1),
            limit = cms.untracked.int32(10000000)
        ),
        threshold = cms.untracked.string('INFO')
    ),
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.load("IOMC.RandomEngine.IOMC_cff")
process.RandomNumberGeneratorService.generator.initialSeed = 456789
process.RandomNumberGeneratorService.g4SimHits.initialSeed = 9876
process.RandomNumberGeneratorService.VtxSmeared.initialSeed = 123456789

#process.Timing = cms.Service("Timing")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)

process.source = cms.Source("EmptySource",
    firstRun        = cms.untracked.uint32(1),
    firstEvent      = cms.untracked.uint32(1)
)

process.generator = cms.EDProducer("FlatRandomEGunProducer",
    PGunParameters = cms.PSet(
#        PartID = cms.vint32(-211), # pi-
#        PartID = cms.vint32(13), # mu-
        PartID = cms.vint32(11), # e-
#        MinEta = cms.double(1.75),
        MinEta = cms.double(2.0),
#        MaxEta = cms.double(2.90),
        MaxEta = cms.double(2.0),
        MinPhi = cms.double(0.436),
        MaxPhi = cms.double(0.436),
        MinE   = cms.double(80.00),
        MaxE   = cms.double(80.00)
    ),
    Verbosity       = cms.untracked.int32(0),
    AddAntiParticle = cms.bool(False)
)

process.o1 = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    fileName = cms.untracked.string('simevent.root')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('CFCLibrary.root')
)

process.p1 = cms.Path(process.generator*process.VtxSmeared*process.g4SimHits)
process.outpath = cms.EndPath(process.o1)
process.g4SimHits.Physics.type = 'SimG4Core/Physics/QGSP_FTFP_BERT_EML'
process.g4SimHits.Physics.DefaultCutValue   = 0.1
process.g4SimHits.CFCSD.FileName = cms.FileInPath('SimG4CMS/Calo/data/combined.root')


