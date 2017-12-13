//
//  component_bundle.h
//  Fermion_Simulator
//
//  Created by Kun Chen on 10/7/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

#ifndef __Fermion_Simulator__component_bundle__
#define __Fermion_Simulator__component_bundle__

#include "component.h"
#include <string>

class RandomFactory;
namespace diag {

const int MAX_BUNDLE = 2 * MAX_ORDER;
template <typename T>
class Bundle {
  private:
    T *_component_name[MAX_BUNDLE];
    T _component_bundle[MAX_BUNDLE];
    std::string _bundle_name;
    int _available_space;

  public:
    std::string BundleName();
    Bundle(std::string);
    T *Add();      //CAUTION: This function returns the address of the added object
    void Add(T *); //Add a copy of component to bundle
    void Remove(T *target);
    void Remove(int);
    void Recover(int step);
    //step>0: Recover successive "step" steps of removing T*
    //step<0: Recover successive "step" steps of adding T*
    T &operator[](name);
    T *operator()(name);
    int HowMany();
    T *RandomPick(RandomFactory &RNG);
    bool Exist(T *target);
};
}
#endif /* defined(__Fermion_Simulator__component_bundle__) */
