//
// Created by lucas on 28/12/18.
//

#include "iTestTypes.h"


BreakTest::BreakTest() {}

/*
bool BreakTest::runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj)
{
    disc->setTestType("break test");

    std::cout << "[INFO]: in class BreakTest: in function runTest" << std::endl;
    std::cout << "\tName: " << disc->getWorker() << std::endl;
    std::cout << "\tMaschine: " << disc->getMachine() << std::endl;
    std::cout << "\tDiscType: " << disc->getDiscType() << std::endl;
    std::cout << "\tTestType: " << disc->getTestType() << std::endl;
    std::cout << "\n\tfile descriptor: " << slaveSocketTest << std::endl;


    while(futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        std::cout << "[BREAK-TEST]: in whilee.... " << std::endl;
    }

    std::cout << "ending [BREAK-TEST]...." << std::endl;



    if(!this->goToOrigin("BreakTest", 1))
    {
        std::cerr << "[ERROR]: in function runTest (BreakTest::runTest): couldn't go to origin (goToOrigin())" << std::endl;
        return false;
    }

    return true;
}
*/

bool BreakTest::runTest(int slaveSocketTest, Disc *disc, std::future<void> &futureObj)
{

    disc->setTestType("break test");

    FileHandler fileHandler;
    std::string logFilePath = fileHandler.createLogFile(disc->createFilename());

    if(!this->reachStartingPoint(logFilePath, *disc, futureObj))
    {
        std::cerr << "[ERROR]: in function runTest (" << disc->getTestType() << "::runTest): couldn't reach starting point" << std::endl;
        return false;
    }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// testroutine
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    float additionalDistance{0.0};
    // average values of values which will be written into a logFile
    float avgUMotor_FB{0.0};
    float avgUKMD_FB{0.0};
    float avgUMotor_OUT{0.0};
    float avgForce{0.0};
    // compare value
    float previousUMotor_FB{0.0};
    int cntStandStill{0};

    while(futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        //[additionalDistance] = mm
        additionalDistance += 2.0f;
        this->setDistance(this->getStartingPoint() + additionalDistance);

        // stop routine if the motor has been deflected to its maximum
        //--> 100mm ... 3,27V
        if(this->getDistance() >= 102)//100)
        {
            std::ostringstream finishedRoutine;
            finishedRoutine << "\n[INFO]: motor has already been deflected to its maximum (100mm); however, did not reach value of maximum force (Fmax)\n"
                            << "[STATUS]: NOT OK" << std::endl;
            // exit while
            break;
        }

        // make a step
//        if(i2c.writeToDac(this->getDistance(), 0))
//        {
//            std::cerr << "[ERROR]: in function runTest (StabilityTest::runTest): couldn't make a step" << std::endl;
//        return false;
//        }

        // wait for 0,2s
        //--> too short duration?
        //--> used minimum velocity (10mm/s) for calculations
        usleep((useconds_t) (2.0f / 10.0f) * 1000000);

        // start measurements
        for(int i = 0; i < 10; ++i)
        {
            avgUMotor_FB    += 0.0f;//i2c.readAdcMotor_FB();
            avgUKMD_FB      += 0.0f;//i2c.readAdcKMD_FB();
            avgForce        += 0.0f;//i2c.getForce();

            // calculating the average value of member elongation and uMotor_OUT isn't necessary
            //--> all values needed for calculating the value of member elongation and uMotor_OUT are constant over a certain time period

            // frequency of measuring (10Hz)
            usleep((useconds_t) (0.1f * 1000000));
        }

        // calculate elongation
        this->calcElongation();
        // set member uMotor_OUT
        this->setUMotor_OUT(0.0f);//i2c.getOutputVoltage());

        // calculate average values for member variables of abstract class ITestTypes
        this->setUMotor_FB(avgUMotor_FB/10.0f);
        this->setUKMD_FB(avgUKMD_FB/10.0f);
        this->setForce(avgForce/10.0f);

//######################################################################################################################
        // if uMotor_FB[n-1] <= uMotor_FB[n] --> motor stagnates
        // previousUMotor_FB mustn't equal 0.0
        if(     (previousUMotor_FB + this->calcUMotor_FBTolerance(this->getStepTolerance()) ) >= this->getUMotor_FB()   // (U[n-1] + dU) >= U[n]
                &&  (previousUMotor_FB - this->calcUMotor_FBTolerance(this->getStepTolerance()) ) <= this->getUMotor_FB()   // (U[n-1] - dU) <= U[n]
                &&  (previousUMotor_FB != 0.0f))
            ++cntStandStill;
//######################################################################################################################

        // check if the disc broke
        if(this->getUKMD_FB() < 200 * pow(10.0, -3.0))
            this->setElongation(0.0f);

        // transmit values of members to the logFile and the client
        this->writeToLogFile(logFilePath);
        this->writeToClient(slaveSocketTest);

        // check if value of force has already reached fMax
        if(this->getForce() >= disc->getFMax())
        {
            std::ostringstream finishedRoutine;
            finishedRoutine << "\n[INFO]: reached value of maximum force (Fmax)\n"
                            << "[STATUS]: OK" << std::endl;

            fileHandler.writeInto(logFilePath, finishedRoutine.str(), false);
            // exit while
            break;
        }

        // check if the motor has already been stagnating for two while cycles
        if(cntStandStill > 1) //cntStandStill >= 1)
        {
            std::ostringstream finishedRoutine;
            finishedRoutine << "\n[INFO]: did not reach value of maximum force (Fmax)\n"
                            << "[STATUS]: NOT OK" << std::endl;

            fileHandler.writeInto(logFilePath, finishedRoutine.str(), false);
            // exit while
            break;
        }

        // load current value of member uMotor_FB into variable previousUMotor_FB
        //-->uMotor_FB[n-1] = uMotor_FB[n]
        previousUMotor_FB = this->getUMotor_FB();

    }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//    this->goToOrigin("StabilityTest", 3);

        return true;
}