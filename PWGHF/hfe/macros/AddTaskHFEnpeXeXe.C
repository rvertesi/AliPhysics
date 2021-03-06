AliAnalysisTask *AddTaskHFEnpeXeXe(
                                   Double_t centrMin = 0,
                                   Double_t centrMax = 100,
                                   Bool_t MCthere = kFALSE,       // DATA false
                                   Bool_t kNPERef = kTRUE,
                                   Bool_t kNPETOFITS = kTRUE,
                                   Bool_t newCentralitySelection = kTRUE, // kTRUE: new framework used; kFALSE: old framework used
                                   Bool_t kNPERefTPConly = kFALSE,
                                   Bool_t isAOD = kTRUE,
                                   Bool_t kNPETOFlast = kFALSE
                                   )
{
    
    const Bool_t isBeauty = kFALSE; // should be false to prevent inclusive analysis
    
    // Default settings (TOF-TPC PbPb)
    const int    kDefTPCcl    = 100;  // 100 (Andrea)
    const int    kDefTPCclPID    =  90;  // 90 (Andrea)
    const int kDefTPCclshared = 1.1;
    const int    kDefITScl    =   4;  // 5 (Andrea)
    const int kDefITSchi2percluster = -1; // cleanup removes badly matching tracks - effects high pt  (cut value = 36) ---> 36 default value for AOD
    const double    kDefDCAr    =   1.; // 2.4 (Andrea)
    const double    kDefDCAz    =   2.; // 3.2 (Andrea)
    const double    kDefTOFs    =   3.;
    const double    kDefITSsmin    =   -2.; // -1,1 up to 1.5, -2,2 up to 3 (Andrea)
    const double    kDefITSsmax    =   2.; // -1,1 up to 1.5, -2,2 up to 3 (Andrea)
    const double  kDefEtaIncMin = -0.8;
    const double  kDefEtaIncMax = 0.8;
    const Bool_t   etacorrection   = kFALSE;
    const Bool_t   multicorrection = kFALSE;
    
    // --- TPC nsigma max and min cut ---
    Double_t dEdxhm[12] = {3.0,3.0,3.0,3.0,3.0,3.0,3.0,3.0,3.0,3.0,3.0,3.0};
    Double_t tpcl1[12]  = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};                              //0,3
    Double_t tpcl15[12]  = {-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};                 // -1,3
    
    // Default setting for the associated electron for the NonPhotonic Analysis
    const double    kassETAm = -0.9;                // -0.9 (Andrea)
    const double    kassETAp = 0.9;                 // 0.9 (Andrea)
    const int    kassITS        =   2;          // # cluster
    const int    kassTPCcl    = 60;           // 80 (Andrea)
    const int    kassTPCPIDcl    =  60;          // not used (Andrea) ---> directly in the filterbit
    const double    kassDCAr    =   1.0;        // not used (Andrea) ---> directly in the filterbit 2.4
    const double    kassDCAz    =   2.0;        // not used (Andrea) ---> directly in the filterbit 3.2
    const double    kassTPCSminus    =  -3.0;
    const double    kassTPCSplus    =   3.0;
    const double kassMinpT = 0.0;
    
    
    Int_t kWei = -1;
    // if we want weights to be applied kWei MUST be 0!!!
    if (MCthere) kWei = 0; // default Pb-Pb
    enum {
        k16g1 = 60,         // 60: PbPb LHC16g1 minimum bias MC (mfaggin, 29/06/2017)
        k16g1_2 = 61         // 61: PbPb LHC16g1 minimum bias MC using charged pion data spectra (mfaggin, 26/07/2017)
    };
    Int_t kWeightMC = 52;
    
    
    //get the current analysis manager
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr) {
        Error("AddTask_hfe_HFE", "No analysis manager found.");
        return 0;
    }
    
    //mgr->AddClassDebug("AliAnalysisTaskHFE",12);
    AliAnalysisDataContainer *cinput  = mgr->GetCommonInputContainer();
    
    Double_t dEdxaclm[12], dEdxachm[12];
    for(int icent = 0; icent < 12; icent++){
        dEdxaclm[icent] = kassTPCSminus;
        dEdxachm[icent] = kassTPCSplus;
    }
    
    
    if(kNPERef)
    {
        // **************************************************************
        // Reference task TPC+TOF
        // **************************************************************
        if(MCthere)
        {
            // TPC low cut = -0.1 (tpcl2) NO WEIGHTS
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm, kDefTOFs,0.,0., AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax,
                                kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,-1);
            // TPC low cut = -0.1 (tpcl2) WITH WEIGHTS
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm, kDefTOFs,0.,0., AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax,
                                kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,kWei,kWeightMC);
        }
        else
        {
            // TPC low cut = -0.1 (tpcl2) NO WEIGHTS for data
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm, kDefTOFs,0.,0., AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax,
                                kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,-1);
        }
    }
    if(kNPERef && kNPETOFITS)
    {
        // **************************************************************
        // Reference task ITS+TPC+TOF
        // **************************************************************
        if(MCthere)
        {
            // WITH WEIGHTS
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm,  kDefTOFs,  kDefITSsmin,kDefITSsmax, AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax, kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,kWei,kWeightMC);
            // NO WEIGHTS
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm,  kDefTOFs,  kDefITSsmin,kDefITSsmax, AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax, kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,-1);
            
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm,  kDefTOFs,  -4,2, AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax, kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,kWei,kWeightMC);
            // NO WEIGHTS
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm,  kDefTOFs,  -4,2, AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax, kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,-1);
        }
        else
        {
            // NO WEIGHTS FOR DATA
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm,  kDefTOFs,  kDefITSsmin,kDefITSsmax, AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax,
                                kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,-1);
            
            RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl15, dEdxhm,  kDefTOFs,  -4,2, AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kDefEtaIncMin, kDefEtaIncMax, kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,-1);
        }
        
    }
    
    
    if(kNPERefTPConly){
        // **************************************************************
        // Reference task
        // **************************************************************
        RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl1, dEdxhm, 0.,0.,0., AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kFALSE, kDefEtaIncMin, kDefEtaIncMax,
                            kassETAm, kassETAp,kassMinpT kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,-1);
    }
    if(kNPETOFlast){
        // **************************************************************
        //
        // Apply TOF after TPC for mismatch background studies
        //
        // **************************************************************
        RegisterTaskNPEXeXe( centrMin,centrMax,newCentralitySelection,MCthere, isAOD, isBeauty, kDefTPCcl, kDefTPCclPID, kDefITScl, kDefDCAr, kDefDCAz, tpcl1, dEdxhm, kDefTOFs,0.,0., AliHFEextraCuts::kBoth, kDefITSchi2percluster, kDefTPCclshared, etacorrection, multicorrection, kTRUE, kDefEtaIncMin, kDefEtaIncMax,
                            kassETAm, kassETAp,kassMinpT, kassITS, kassTPCcl, kassTPCPIDcl, kassDCAr, kassDCAz, dEdxaclm, dEdxachm, kTRUE, kFALSE,-1);
    }
    

    
    return NULL;
    
}

//===============================================================================

//===============================================================================
AliAnalysisTask *RegisterTaskNPEXeXe(
                                     Int_t centrMin = 0, Int_t centrMax = 100,
                                     Bool_t newCentralitySelection = kTRUE, // kTRUE: new framework used; kFALSE: old framework used
                                     Bool_t useMC, Bool_t isAOD, Bool_t beauty,
                                     Int_t tpcCls=120, Int_t tpcClsPID=80,
                                     Int_t itsCls=4, Double_t dcaxy=1.0, Double_t dcaz=2.0,
                                     Double_t *tpcdEdxcutlow=NULL, Double_t *tpcdEdxcuthigh=NULL,
                                     Double_t tofs=3., Double_t itssmin=0., Double_t itssmax=0.,
                                     Int_t itshitpixel =AliHFEextraCuts::kBoth,
                                     Double_t itschi2percluster = -1, Double_t tpcsharedcluster = 1.1,
                                     Bool_t etacorr=kFALSE, Bool_t multicorr = kFALSE,
                                     Double_t etaIncMin = -0.8, Double_t etaIncMax = 0.8,
                                     Double_t assETAm=-0.8, Double_t assETAp=0.8, Double_t assMinpTvalue = 0.0,
                                     Int_t assITS=2, Int_t assTPCcl=100,
                                     Int_t assTPCPIDcl=80, Double_t assDCAr=1.0, Double_t assDCAz=2.0,
                                     Double_t *assTPCSminus = NULL, Double_t *assTPCSplus=NULL,
                                     Bool_t useCat1Tracks = kTRUE, Bool_t useCat2Tracks = kTRUE,
                                     Int_t weightlevelback = -1,Int_t wei = 2,Bool_t toflast = kFALSE
                                     )
{
    Bool_t usekfparticle = kFALSE;
    Bool_t ipCharge = kFALSE;
    Bool_t ipOpp = kFALSE;
    Bool_t releasemcvx = kFALSE;
    Bool_t nondefaultcentr = kFALSE;
    
    //
    // Cuts on the inclusive leg
    //
    Int_t idcaxy = (Int_t)(dcaxy*10.);
    Int_t idcaz = (Int_t)(dcaz*10.);
    Int_t tpclow = 0;
    // ------- to manage containers name with negative TPC low cut --------
    bool IsTPClowcutNegative = kFALSE;
    if(tpcdEdxcutlow)
    {
        tpclow = (Int_t) (tpcdEdxcutlow[0]*1000.);
        if(tpclow<0)
        {
            IsTPClowcutNegative = kTRUE;
            tpclow = 0 - tpclow;            // switched signed (ready to be used in the container name)
        }
    }
    // --------------------------------------------------------------------
    Int_t itofs = (Int_t)(tofs*10.);
    Int_t iitssmin = (Int_t)(TMath::Abs(itssmin)*10.);
    Int_t iitssmax = (Int_t)(itssmax*10.);
    Int_t ipixelany = itshitpixel;
    Int_t imult = multicorr ? 1 : 0;
    Int_t itofpos = toflast ? 1 : 0;
    
    //
    // Cuts on the associated leg
    Int_t iassDCAr = (Int_t)(assDCAr*10);
    Int_t iassDCAz = (Int_t)(assDCAz*10);
    Int_t iassMinpT = (Int_t) (assMinpTvalue*100);
    Int_t iassTPCSplus  = assTPCSplus ? (Int_t)(assTPCSplus[0]*1000) : 0;
    Int_t icat1 = useCat1Tracks ? 1 : 0;
    Int_t icat2 = useCat2Tracks ? 1 : 0;
    Int_t etaInclusiveMax = (Int_t)(etaIncMax*100);
    
    
    TString cweightsback("");
    if(weightlevelback>=0) {
        cweightsback += "Wa";
        cweightsback += weightlevelback;
        cweightsback += "_";
        cweightsback += wei;
    }

    
    TString cmvx("");
    if(releasemcvx) {
        cmvx += "MCVR";
    }
    
    TString kfp("");
    if(usekfparticle) {
        kfp += "kf";
    }
    
    if(beauty) {
        if(ipCharge && ipOpp) TString cbeauty("BeautyIPopp");
        else if(ipCharge) TString cbeauty("BeautyIPcrg");
        else if(!ipCharge) TString cbeauty("Beauty");
        else TString cbeauty("BeautyWrong");
    }
    else TString cbeauty("");
    
    // ------- to manage containers name with negative TPC low cut --------
    TString appendix = "";                                                                   // letter 'm' added in this point (after TPCs)
    if(IsTPClowcutNegative)      appendix+=TString::Format("SPD%d_incEta%dTPCc%dTPCp%dITS%dDCAr%dz%dTPCsm%dTOFs%dITSmins%dITSmaxs%dm%dt%d_photMinpT%dTPCc%dTPCp%dITS%dDCAr%dDCAz%dTPCs%d%s%s%s%s",ipixelany,etaInclusiveMax,tpcCls,tpcClsPID,itsCls,idcaxy,idcaz,tpclow,itofs,iitssmin,iitssmax,imult,itofpos,iassMinpT,assTPCcl,assTPCPIDcl,assITS,iassDCAr,iassDCAz,iassTPCSplus,cweightsback.Data(),cmvx.Data(),cbeauty.Data(),kfp.Data());
    else                         appendix+=TString::Format("SPD%d_incEta%dTPCc%dTPCp%dITS%dDCAr%dz%dTPCs%dTOFs%dITSmins%dITSmaxs%dm%dt%d_photMinpT%dTPCc%dTPCp%dITS%dDCAr%dDCAz%dTPCs%d%s%s%s%s",ipixelany,etaInclusiveMax,tpcCls,tpcClsPID,itsCls,idcaxy,idcaz,tpclow,itofs,iitssmin,iitssmax,imult,itofpos,iassMinpT,assTPCcl,assTPCPIDcl,assITS,iassDCAr,iassDCAz,iassTPCSplus,cweightsback.Data(),cmvx.Data(),cbeauty.Data(),kfp.Data());
    
    printf("Add macro appendix %s\n", appendix.Data());
    
    // GRID version
    if(useMC&&!gROOT->GetListOfGlobalFunctions()->FindObject("ConfigWeightFactors")) gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/hfe/macros/configs/ConfigWeightFactors.C");
    if(!gROOT->GetListOfGlobalFunctions()->FindObject("ConfigHFEnpeXeXe"))gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/hfe/macros/configs/PbPb/ConfigHFEnpeXeXe.C");
    /*
     // GSI version
     // ----- my weights (mfaggin, 29/06/2017) -----
     //if(useMC&&!gROOT->GetListOfGlobalFunctions()->FindObject("ConfigWeightFactors")) gROOT->LoadMacro("$TRAIN_ROOT/util/hfe/configs/ConfigWeightFactors.C");
     if(useMC&&!gROOT->GetListOfGlobalFunctions()->FindObject("ConfigWeightFactors_PbPb5TeV")) gROOT->LoadMacro("$TRAIN_ROOT/util/hfe/configs/ConfigWeightFactors_PbPb5TeV.C");
     // --------------------------------------------
     if(!gROOT->GetListOfGlobalFunctions()->FindObject("ConfigHFEnpePbPb5TeV")) gROOT->LoadMacro("$TRAIN_ROOT/util/hfe/configs/ConfigHFEnpePbPb5TeV.C");
     // --------------------------------------------------------------------
     */
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    AliAnalysisDataContainer *cinput  = mgr->GetCommonInputContainer();
    
    AliAnalysisTaskHFE *task = ConfigHFEnpeXeXe(useMC, isAOD, appendix, tpcCls, tpcClsPID, itsCls, dcaxy, dcaz, tpcdEdxcutlow, tpcdEdxcuthigh, tofs, 0, itssmin,itssmax,
                                                itshitpixel, itschi2percluster, tpcsharedcluster, etacorr, multicorr, toflast, etaIncMin, etaIncMax,
                                                assETAm, assETAp,assMinpTvalue, assITS, assTPCcl, assTPCPIDcl,
                                                assDCAr, assDCAz, assTPCSminus, assTPCSplus,
                                                useCat1Tracks, useCat2Tracks, weightlevelback);
    
    if(isAOD)
        task->SetAODAnalysis();
    else
        task->SetESDAnalysis();
    
    if (useMC)    task->SetHasMCData(kTRUE);
    else       task->SetHasMCData(kFALSE);
    
    if(useMC && weightlevelback>=0) {
        ConfigWeightFactors(task,kFALSE,wei,"nonHFEcorrect_XeXe.root");
    }
    
    // ----- trigger selecton ---------
    if(!newCentralitySelection)   task->SelectCollisionCandidates(AliVEvent::kMB | AliVEvent::kCentral | AliVEvent::kSemiCentral); // old framework
    if(newCentralitySelection)    task->SelectCollisionCandidates(AliVEvent::kINT7);                                               // new framework
    // ----- centrality selection -----
    task->SetCentralityCheck(newCentralitySelection,"V0M");
    task->SetCentralityInterval(centrMin,centrMax);               // all events outside the desired centrality interval are rejected
    // --------------------------------
    TString containerName = mgr->GetCommonFileName();
    containerName += ":HFEtask";
    containerName += appendix.Data();
    printf("container name: %s\n", containerName.Data());
    
    //create data containers
    task->ConnectOutput(1, mgr->CreateContainer(Form("HFE_Results_%s", appendix.Data()), TList::Class(), AliAnalysisManager::kOutputContainer, containerName.Data()));
    task->ConnectOutput(2, mgr->CreateContainer(Form("HFE_QA_%s", appendix.Data()), TList::Class(), AliAnalysisManager::kOutputContainer, containerName.Data()));
    mgr->ConnectInput(task,  0, cinput );
    
    mgr->AddTask(task);
    
    return NULL;
}
