//
// Created by lucas on 06/02/19.
//

#include "iTestTypes.h"

StabilityTest::StabilityTest() {}


bool StabilityTest::runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj)
{

    disc->setTestType("stability test");

    FileHandler fileHandler;
    std::string logFilePath = fileHandler.createLogFile(disc->createFilename());

/*
    std::ostringstream logFileHeader;
    logFileHeader << disc->getDateTime()    << "\n"
                  << disc->getWorker()      << ", " << disc->getMachine() << "\n"
                  << disc->getDiscType()    << ", " << disc->getTestType() << "\n\n";

//    I2C i2c;
//    if(!i2c.goToStartingPoint(this->getStartingPoint()))
//    {
//        std::cerr << "[ERROR]: in function runTest (StabilityTest::runTest): couldn't go to starting point" << std::endl;
//        return false;
//    }

    bool reachedStartingPoint{true};
    bool stopWatchDog{false};
//    bool returnVal_goToOrigin{true};
    // look for any troubles while motor drives to startingPoint
    std::thread watchDog_KMD = std::thread( [&] {
        while(!stopWatchDog && (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout))
        {
            this->setUKMD_FB(0.0);//i2c.readAdcKMD_FB());
            if(this->getUKMD_FB() > 200 * pow(10.0, -3.0))
            {
                reachedStartingPoint = false;
                return;
            }
            // frequency of measuring (10Hz)
            usleep((useconds_t)(0.1f * 1000000));
        }
        return;
    });
    // wait for 5s, until the motor has reached the StartingPoint
    // max speed: 25mm/s; used a velocity of 20mm/s for the calculations
//    usleep( (useconds_t) ((this->getStartingPoint()/20) * 1000000));
    usleep(5000000);
    stopWatchDog = true;
    watchDog_KMD.join();

    if(!reachedStartingPoint)
    {
        logFileHeader << "[ERROR]: some troubles occurred while the motor was driving to startingPoint" << std::endl;
        fileHandler.writeInto(logFilePath, logFileHeader.str(), true);

//        this->goToOrigin("StabilityTest", 1);
        return false;
    }


    this->setUKMD_FB(10.0);//i2c.readAdcKMD_FB());
    // lower than 200mV
    if(this->getUKMD_FB() < 200 * pow(10.0, -3.0))
    {
        logFileHeader << "[ERROR]: did not reach startingPoint; please make sure that the value of startingPoint in startingPoint.txt is correct" << std::endl;
        fileHandler.writeInto(logFilePath, logFileHeader.str(), true);

//        this->goToOrigin("StabilityTest", 2);
        return false;
    }

    logFileHeader   << "[INFO]: the motor reached the startingPoint without any troubles\n"
                    << "Motor-Input-Voltage [V] | Motor-FB-Voltage [V] | Elongation [mm] | KMD-FB-Voltage [V] | Force [N]\n";
    fileHandler.writeInto(logFilePath, logFileHeader.str(), true);
*/

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

    int j{0};

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
            finishedRoutine << "\n[INFO]: motor has already been deflected to its maximum (100mm); however, did not reach value of nominal force (Fnom)\n"
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
            avgUMotor_FB    += 0.074f + (float) i * 0.1f + (float) j * 0.1f;//i2c.readAdcMotor_FB();
            avgUKMD_FB      += 0.2f + (float) (i%2) * 0.1f;//i2c.readAdcKMD_FB();
            avgForce        += 1.0f + (float) i * 0.3f;//i2c.getForce();

            // calculating the average value of member elongation and uMotor_OUT isn't necessary
            //--> all values needed for calculating the value of member elongation and uMotor_OUT are constant over a certain time period

            // frequency of measuring (10Hz)
            usleep((useconds_t) (0.1f * 1'000'000));
        }

        // calculate elongation
        this->calcElongation();
        // set member uMotor_OUT
        this->setUMotor_OUT(avgUMotor_FB*(5.0f/3.0f));//i2c.getOutputVoltage());

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
        if(this->getUKMD_FB() < 200.0f * (float) pow(10.0, -3.0))
            this->setElongation(0.0f);

        // transmit values of members to the logFile and the client
        this->writeToLogFile(logFilePath);
        this->writeToClient(slaveSocketTest);

        // check if value of force has already reached fNom
        if(this->getForce() >= disc->getFNom())
        {
            std::ostringstream finishedRoutine;
            finishedRoutine << "\n[INFO]: reached value of nominal force (Fnom)\n"
                            << "[STATUS]: OK" << std::endl;

            fileHandler.writeInto(logFilePath, finishedRoutine.str(), false);
            // exit while
            break;
        }

        // check if the motor has already been stagnating for two while cycles
        if(cntStandStill > 1) //cntStandStill >= 1)
        {
            std::ostringstream finishedRoutine;
            finishedRoutine << "\n[INFO]: did not reach value of nominal force (Fnom)\n"
                            << "[STATUS]: NOT OK" << std::endl;

            fileHandler.writeInto(logFilePath, finishedRoutine.str(), false);
            // exit while
            break;
        }

        // load current value of member uMotor_FB into variable previousUMotor_FB
        //-->uMotor_FB[n-1] = uMotor_FB[n]
        previousUMotor_FB = this->getUMotor_FB();
        ++j;

    }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//    this->goToOrigin("StabilityTest", 3);
    return true;
}