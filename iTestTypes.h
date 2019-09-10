//
// Created by lucas on 28/12/18.
//

#ifndef INC_1_MMP_CRACKTEST_ITESTTYPES_H
#define INC_1_MMP_CRACKTEST_ITESTTYPES_H

#include <unistd.h>
#include <future>
#include <cmath>

#include "disc.h"
//#include "i2c.h"


//**************************************************************
// Strategy Pattern
//**************************************************************
// interface --> ITestTypes
class ITestTypes {

private:

    // voltages
    float uMotor_FB{0.0};
    float uKMD_FB{0.0};
    float uMotor_OUT{0.0};

    // distance
    float distance{0.0};

    // force
    float force{0.0};
    // elongation
    float elongation{0.0};

    float round(float val);

public:
    virtual ~ITestTypes() {}

    void setUMotor_FB(float uMotor_FB);
    void setUKMD_FB(float uKMD_FB);
    void setUMotor_OUT(float uMotor_OUT);
    void setDistance(float distance);
    void setForce(float force);
    // elongation ... Dehnung
    void calcElongation();
    void setElongation(float elongation);

    float getUMotor_FB();
    float getUKMD_FB();
//    float getUMotor_OUT();
    float getDistance();
    float getForce();
//    float getElongation();

    float getStartingPoint();
    float getStepTolerance();
    float calcUMotor_FBTolerance(float stepTolerance);



    virtual bool runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj)=0;

    bool reachStartingPoint(const std::string& logFilePath, const Disc disc, std::future<void>& futureObj);

    bool writeToLogFile(const std::string& logFilePath);
    bool writeToClient(const int slaveSocketTest);
    bool goToOrigin(const std::string& testType, unsigned int nrOfCall);

};


class StabilityTest : public ITestTypes {

public:
    StabilityTest();

    bool runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj) override;

};

class BreakTest : public ITestTypes {

public:
    BreakTest();

    bool runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj) override;

};

class ManualTest : public ITestTypes {

public:
    ManualTest();

    bool runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj) override;

};

#endif //INC_1_MMP_CRACKTEST_ITESTTYPES_H
