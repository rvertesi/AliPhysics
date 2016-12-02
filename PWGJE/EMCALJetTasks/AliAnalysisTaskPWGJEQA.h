/**
 * \file AliAnalysisTaskPWGJEQA.h
 * \brief Declaration of class AliAnalysisTaskPWGJEQA
 *
 * In this header file the class AliAnalysisTaskPWGJEQA is declared.
 *
 * \author James Mulligan <james.mulligan@yale.edu>, Yale University
 * \date Dec 1, 2016
 */

#ifndef ALIANALYSISTASKPWGJEQA_H
#define ALIANALYSISTASKPWGJEQA_H

/* Copyright(c) 1998-2016, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

class TH1;
class TH2;
class TH3;
class THnSparse;

#include "THistManager.h"
#include "AliTLorentzVector.h"
//#include "AliAnalysisTaskEmcalJetLight.h"
#include "AliAnalysisTaskEmcalJet.h"

/**
 * \class AliAnalysisTaskPWGJEQA
 * \brief This is a task used to do basic PWGJE QA on tracks, clusters, and jets.
 * Based on code from Salvatore Aiola: See the tasks AliAnalysisTaskEmcalJetQA (clusters),
 * AliAnalysisTaskEmcalJetSpectraQA (jets), and AliEmcalTrackingQATask (tracks) for more detailed histograms.
 */
class AliAnalysisTaskPWGJEQA : public AliAnalysisTaskEmcalJet {
  
public:
  
  struct EventQA_t {
    EventQA_t() : fCent(0), fNTracks(0), fMaxTrack() { fNClusters[0] = 0; fNClusters[1] = 0; fNClusters[2] = 0;}
    Float_t fCent;
    Int_t fNTracks;
    Int_t fNClusters[3];
    AliTLorentzVector fMaxTrack;
    AliTLorentzVector fMaxCluster[3];
  };
  
  enum ClusterType {
    kNA       = -1,//!< Undefined
    kEMCal    = 0, //!< EMCal
    kDCal     = 1, //!< DCal
    kPHOS     = 2  //!< PHOS
  };

  AliAnalysisTaskPWGJEQA();
  AliAnalysisTaskPWGJEQA(const char *name);
  virtual ~AliAnalysisTaskPWGJEQA();
  
  void                        UserCreateOutputObjects();

  void                        SetCellEnergyCut(Float_t cut)                        { fCellEnergyCut      = cut        ; }
  void                        SetSeparateEMCalDCal(Bool_t b)                       { fSeparateEMCalDCal = b           ; }
  void                        SetPtBin(Float_t w, Float_t max)                     { fPtBinWidth        = w; fMaxPt = max ; }
  void                        SetGeneratorLevelName(const char* name)              { fGeneratorLevelName = name       ; }
  void                        SetDetectorLevelName(const char* name)               { fDetectorLevelName = name        ; }
  
  void                        SetDoTrackQA(Bool_t b) { fDoTrackQA = b; }
  void                        SetDoEmcalQA(Bool_t b) { fDoEmcalQA = b; }
  void                        SetDoJetQA(Bool_t b)   { fDoJetQA   = b; }
  void                        SetDoEventQA(Bool_t b) { fDoEventQA = b; }
  
protected:
  
  void                        ExecOnce()                                                    ;
  Bool_t                      FillHistograms()                                              ;
  Bool_t                      RetrieveEventObjects()                                        ;
  
  void                        AllocateTrackHistograms()                                     ;
  void                        AllocateCellHistograms()                                      ;
  void                        AllocateClusterHistograms()                                   ;
  void                        AllocateJetHistograms()                                       ;
  void                        AllocateEventQAHistograms()                                   ;
  
  void                        FillTrackHistograms()                                         ;
  void                        FillCellHistograms()                                          ;
  void                        FillClusterHistograms()                                       ;
  void                        FillJetHistograms()                                           ;
  void                        FillEventQAHistograms()                                       ;
  
  void                        GenerateHistoBins()                                           ;
  void                        AllocateDetectorLevelTHnSparse()                              ;
  void                        AllocateGeneratorLevelTHnSparse()                             ;
  void                        AllocateMatchedParticlesTHnSparse()                           ;
  void                        FillDetectorLevelTHnSparse(Double_t cent, Double_t trackEta, Double_t trackPhi, Double_t trackPt,
                                                         Double_t sigma1OverPt, Byte_t trackType);
  void                        FillGeneratorLevelTHnSparse(Double_t cent, Double_t partEta, Double_t partPhi, Double_t partPt);
  void                        FillMatchedParticlesTHnSparse(Double_t cent, Double_t partEta, Double_t partPhi, Double_t partPt,
                                                            Double_t trackEta, Double_t trackPhi, Double_t trackPt, Byte_t trackType);
  
  Float_t                     fCellEnergyCut;            ///< Energy cell cut
  Float_t                     fPtBinWidth;               ///< Histogram pt bin width
  Float_t                     fMaxPt;                    ///< Histogram pt limit
  Bool_t                      fSeparateEMCalDCal;        ///< Separate EMCal from DCal in QA plots
  Int_t                       fNTotClusters[3];          //!<!Total number of accepted clusters in current event (DCal/EMCal)
  AliTLorentzVector           fLeadingCluster[3];        //!<!Leading cluster in current event (EMCal/DCal)
  Int_t                       fNTotTracks;               //!<!Total number of accepted tracks in current event
  AliTLorentzVector           fLeadingTrack;             //!<!Leading track in current event
  Bool_t                      fDoTrackQA;                ///< Set whether to enable track QA
  Bool_t                      fDoEmcalQA;                ///< Set whether to enable cell/cluster QA
  Bool_t                      fDoJetQA;                  ///< Set whether to enable jet QA
  Bool_t                      fDoEventQA;                ///< Set whether to enable event QA
  TString               fGeneratorLevelName;             ///< generator level container name
  TString               fDetectorLevelName;              ///< detector level container name
  
  // Service fields (non-streamed)
  AliMCParticleContainer* fGeneratorLevel      ; //! generator level container
  AliTrackContainer*    fDetectorLevel         ; //! detector level container
  Int_t                 fNPtHistBins           ; //! number of pt bins
  Double_t*             fPtHistBins            ; //! pt bins
  Int_t                 fNEtaHistBins          ; //! number of eta bins
  Double_t*             fEtaHistBins           ; //! eta bins
  Int_t                 fNPhiHistBins          ; //! number of phi bins
  Double_t*             fPhiHistBins           ; //! phi bins
  Int_t                 fNCentHistBins         ; //! number of cent bins
  Double_t*             fCentHistBins          ; //! cent bins
  Int_t                 fNPtRelDiffHistBins    ; //! number of pt relative difference bins
  Double_t*             fPtRelDiffHistBins     ; //! pt relative difference bins
  Int_t                 fNPtResHistBins        ; //! number of pt res bins
  Double_t*             fPtResHistBins         ; //! pt res bins
  Double_t*             f1OverPtResHistBins    ; //! 1/pt res bins
  Int_t                 fN1OverPtResHistBins   ; //! number of 1/pt res bins
  Int_t                 fNIntegerHistBins      ; //! number of integer bins
  Double_t*             fIntegerHistBins       ; //! integer bins
  
  // Histograms
  THnSparse*            fTracks                ; //! all tracks
  THnSparse*            fParticlesPhysPrim     ; //! all physical primary particles
  THnSparse*            fParticlesMatched      ; //! primary particles matched to detector level tracks
  
  THistManager                fHistManager;              //!< Histogram manager
  
private:
  AliAnalysisTaskPWGJEQA(const AliAnalysisTaskPWGJEQA&);            // not implemented
  AliAnalysisTaskPWGJEQA &operator=(const AliAnalysisTaskPWGJEQA&); // not implemented
  
  /// \cond CLASSIMP
  ClassDef(AliAnalysisTaskPWGJEQA, 1);
  /// \endcond
};
#endif