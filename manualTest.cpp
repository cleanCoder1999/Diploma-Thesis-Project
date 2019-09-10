//
// Created by lucas on 28/12/18.
//

#include "iTestTypes.h"


ManualTest::ManualTest() {}


bool ManualTest::runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj)
{
    disc->setTestType("manual test");

    std::cout << "[INFO]: in class ManualTest: in function runTest" << std::endl;
    std::cout << "\tName: " << disc->getWorker() << std::endl;
    std::cout << "\tMaschine: " << disc->getMachine() << std::endl;
    std::cout << "\tDiscType: " << disc->getDiscType() << std::endl;
    std::cout << "\tTestType: " << disc->getTestType() << std::endl;
    std::cout << "\n\tfile descriptor: " << slaveSocketTest << std::endl;

    while(futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        std::cout << "[MANUAL-TEST]: in whilee.... " << std::endl;
    }

    std::cout << "ending [MANUAL-TEST]...." << std::endl;







    if(!this->goToOrigin("ManualTest", 1))
    {
        std::cerr << "[ERROR]: in function runTest (ManualTest::runTest): couldn't go to origin (goToOrigin())" << std::endl;
        return false;
    }

    return true;
}
