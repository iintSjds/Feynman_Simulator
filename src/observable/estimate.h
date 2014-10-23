//
//  Estimate.h
//  Feynman_Simulator
//
//  Created by Kun Chen on 10/14/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

#ifndef __Feynman_Simulator__Estimate__
#define __Feynman_Simulator__Estimate__

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../utility/cnpy.h"
#include "../utility/convention.h"
#include "../utility/complex.h"

/**
*  \brief estimate with mean value and standard error
*/
template <typename T>
class Estimate {
  public:
    Estimate();
    Estimate(const T &m, const T &e);
    T Mean;
    T Error;
    friend std::ostream &operator<<(std::ostream &, const Estimate &);
};

/**
*  \brief maintain a history of observables, where an Estimate object can be calculated
*/

template <typename T>
class Estimator {
  private:
    T _accumulator;
    real _norm;
    real _ratio;
    Estimate<T> _value;
    void _update();

  public:
    std::vector<T> _history;
    Estimator();
    Estimator(std::string);
    std::string Name;
    void Measure(const T &);
    void AddStatistics();
    Estimate<T> Estimate();
    double Ratio();
    bool LoadState(cnpy::npz_t);
    void SaveState(const std::string &FileName, std::string Mode = "a");
    void ClearStatistics();
    void SqueezeStatistics(real factor);
};

/**
*  \brief an extension to the std::vector<Estimator<T>> to contain Estimator<T>
*/

template <typename T>
class EstimatorBundle {
  private:
    typedef Estimator<T> EstimatorT;
    std::vector<EstimatorT> _EstimatorVector;
    std::unordered_map<std::string, EstimatorT *> _EstimatorMap;

  public:
    void AddEstimator(const std::string);
    void AddEstimator(const EstimatorT &);
    void RemoveEstimator(const std::string);
    void AddStatistics();
    int HowMany();
    bool LoadState(const std::string &FileName);
    void SaveState(const std::string &FileName, std::string Mode = "a");
    EstimatorT &operator[](int);
    EstimatorT &operator[](std::string);
    void ClearStatistics();
    void SqueezeStatistics(real factor);
};

#endif /* defined(__Feynman_Simulator__Estimate__) */
