//
//  main.cpp
//  Fermion_Simulator
//
//  Created by Kun Chen on 10/2/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

/********************** include files *****************************************/
#include <iostream>
#include "definition.h"
#include "initialization.h"
#include "markov/markov.h"
#include "markov/markov_monitor.h"
using namespace std;

void MonteCarlo(Jobs::JobsMC &Job);
void Dyson(Jobs::JobsDyson &Job);
int main(int argc, const char *argv[])
{
    //initialize LOGGER
    LOGGER_CONF("", "MC", Logger::file_on | Logger::screen_on, INFO, INFO);

    InitGlobalUtility();
    RunTest();
    //    InitEveryOneNeedsIt();
    string InputFile = "infile/_in_MC_1";

    switch (Jobs::GetJobsType(InputFile)) {
        case Jobs::MC:
            MonteCarlo(Jobs::JobsMC(InputFile));
        case Jobs::DYSON:
            Dyson(Jobs::JobsDyson(InputFile));
    }
    return 0;
}

void MonteCarlo(Jobs::JobsMC &Job)
{
    EnvMonteCarlo PaddyField(Job);
    Markov GrassHopper(&PaddyField);
    MarkovMonitor Scarecrow(&PaddyField);
    while (PaddyField.Counter < 10000) {
        PaddyField.Counter++;
        //        GrassHopper.Hop(10);

        Scarecrow.Measure();

        if (PaddyField.Counter % 10) {
            //            Env.AddStatistics();
            Scarecrow.ReWeightEachOrder();
        }
        if (PaddyField.Counter % 100000) {
            PaddyField.SaveState();
        }
        if (PaddyField.Counter % 20) {
            Scarecrow.Annealing();
        }
    }
}

void Dyson(Jobs::JobsDyson &Job)
{
    EnvDyson Env(Job);
}
