//
//  weight.cpp
//  Feynman_Simulator
//
//  Created by Kun Chen on 11/2/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

#include "weight.h"
#include "component.h"
#include "utility/dictionary.h"
#include "module/parameter/parameter.h"
#include "gamma3.h"

using namespace std;
using namespace para;

weight::Weight::Weight(bool IsAllSymmetric)
{
    _IsAllSymmetric = IsAllSymmetric;
    Sigma = nullptr;
    Polar = nullptr;
    G = nullptr;
    W = nullptr;
}

weight::Weight::~Weight()
{
    delete Sigma;
    delete Polar;
    delete G;
    delete W;
}
/**
*  Build G, W, Sigma, Polar from file, you may use flag weight::GW and weight::SigmaPolar to control which group to load. Notice those in unflaged group will remain the same.
*
*  @param _flag     weight::GW to allocate memory for GW, weight::SigmaPolar to allocate memory for Sigma and Polar
*  @param Lat       Lattice
*  @param Beta      Beta
*  @param order     order
*/

bool weight::Weight::BuildNew(flag _flag, const ParaMC &para)
{
    //NormFactor only consider Vol, not beta, since beta can be changing during annealing
    Norm::NormFactor = para.Lat.Vol * para.Lat.SublatVol;

    if (para.Order == 0)
        ABORT("Order can not be zero!!!");
    //GW can only be loaded
    if (_flag & weight::SigmaPolar) {
        _AllocateSigmaPolar(para);
        Sigma->BuildNew();
        Polar->BuildNew();
    }
    return true;
}

void weight::Weight::Anneal(const ParaMC &para)
{
    G->Reset(para.Beta);
    W->Reset(para.Beta);
    Sigma->Reset(para.Beta);
    Polar->Reset(para.Beta);
}

bool weight::Weight::FromDict(const Dictionary &dict, flag _flag, const para::ParaMC &para)
{
    //NormFactor only consider Vol, not beta, since beta can be changing during annealing
    Norm::NormFactor = para.Lat.Vol * para.Lat.SublatVol;

    if (_flag & weight::GW) {
        _AllocateGW(para);
        G->FromDict(dict.Get<Dictionary>("G"));
        W->FromDict(dict.Get<Dictionary>("W"));
    }
    if (_flag & weight::SigmaPolar) {
        _AllocateSigmaPolar(para);
        Sigma->FromDict(dict.Get<Dictionary>("Sigma"));
        Polar->FromDict(dict.Get<Dictionary>("Polar"));
    }
    if (_flag & weight::GammaGW) {
        _AllocateGammaGW(para);
        if(dict.HasKey("GammaG")) {
            GammaG->WeightFromDict(dict.Get<Dictionary>("GammaG"));
        }else{
            LOG_WARNING("There is no GammaG Weight to read!\n I will initialze them with zeros.");
        }
        if(dict.HasKey("GammaW")) {
            GammaW->WeightFromDict(dict.Get<Dictionary>("GammaW"));
        }else{
            LOG_WARNING("There is no GammaW Weight to read!\n I will initialze them with zeros.");
            //GammaW will be allocated with zeros
        }
        G->GammaGWeight = GammaG;
        W->GammaWWeight = GammaW;
    }
    if (_flag & weight::GammaGWStatis) {
        ASSERT_ALLWAYS(GammaG!= nullptr&&GammaW!= nullptr, "GammaG and GammaW has to be initialized first!");
        if(dict.HasKey("GammaGStatis")&&dict.HasKey("GammaWStatis")){
            GammaG->StatisFromDict(dict.Get<Dictionary>("GammaGStatis"));
            GammaW->StatisFromDict(dict.Get<Dictionary>("GammaWStatis"));
        }else{
            LOG_WARNING("There is no GammaG or GammaW statistics to read");
            GammaG->ClearStatistics();
            GammaW->ClearStatistics();
        }
    }
    return true;
}
Dictionary weight::Weight::ToDict(flag _flag)
{
    Dictionary dict;
    if (_flag & weight::GW) {
        dict["G"] = G->ToDict();
        dict["W"] = W->ToDict();
    }
    if (_flag & weight::SigmaPolar) {
        dict["Sigma"] = Sigma->ToDict();
        dict["Polar"] = Polar->ToDict();
    }
    if (_flag & weight::GammaGW) {
        if(GammaG!= nullptr)
            dict["GammaG"] = GammaG->WeightToDict();
        if(GammaW!= nullptr)
            dict["GammaW"] = GammaW->WeightToDict();
    }
    if (_flag & weight::GammaGWStatis) {
        if(GammaG!= nullptr)
            dict["GammaGStatis"] = GammaG->StatisToDict();
        if(GammaW!= nullptr)
            dict["GammaWStatis"] = GammaW->StatisToDict();
    }
    return dict;
}

void weight::Weight::SetTest(const ParaMC &para)
{
    _AllocateGW(para);
    _AllocateSigmaPolar(para);
    G->BuildTest();
    W->BuildTest();
}

void weight::Weight::SetDiagCounter(const ParaMC &para)
{
    _AllocateGW(para);
    _AllocateSigmaPolar(para);
    G->BuildTest();
    W->BuildTest();
}

void weight::Weight::_AllocateGW(const ParaMC &para)
{
    //make sure old Sigma/Polar/G/W are released before assigning new memory
    delete G;
    auto symmetry = _IsAllSymmetric ? TauSymmetric : TauAntiSymmetric;
    G = new weight::GClass(para.Lat, para.Beta, para.MaxTauBin, symmetry);
    delete W;
    W = new weight::WClass(para.Lat, para.Beta, para.MaxTauBin);
}

void weight::Weight::_AllocateSigmaPolar(const ParaMC &para)
{
    auto symmetry = _IsAllSymmetric ? TauSymmetric : TauAntiSymmetric;
    delete Sigma;
    Sigma = new weight::SigmaClass(para.Lat, para.Beta, para.MaxTauBin, para.Order, symmetry);
    delete Polar;
    Polar = new weight::PolarClass(para.Lat, para.Beta, para.MaxTauBin, para.Order);
}

void weight::Weight::_AllocateGammaGW(const ParaMC &para)
{
    //make sure old Gamma/G/W are released before assigning new memory
    if(GammaG== nullptr)
        GammaG = new weight::GammaGClass(para.Lat, para.Beta, para.MaxTauBin);
    if(GammaW== nullptr)
        GammaW = new weight::GammaWClass(para.Lat, para.Beta, para.MaxTauBin);
}
