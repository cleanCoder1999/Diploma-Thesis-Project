//
// Created by lucas on 05/02/19.
//

#include "iTestTypes.h"

//###################################################################################################
// setters
//###################################################################################################
void ITestTypes::setUMotor_FB(float uMotor_FB)
{
    this->uMotor_FB = uMotor_FB;
}

void ITestTypes::setUKMD_FB(float uKMD_FB)
{
    this->uKMD_FB = uKMD_FB;
}

void ITestTypes::setUMotor_OUT(float uMotor_OUT)
{
    this->uMotor_OUT = uMotor_OUT;
}

void ITestTypes::setDistance(float distance)
{
    this->distance = distance;
}

void ITestTypes::setForce(float force)
{
    this->force = force;
}

void ITestTypes::setElongation(float elongation)
{
    this->elongation = elongation;
}

//###################################################################################################
// getters
//###################################################################################################
float ITestTypes::getUMotor_FB()
{
    return this->uMotor_FB;
}

float ITestTypes::getUKMD_FB()
{
    return this->uKMD_FB;
}

float ITestTypes::getForce()
{
    return this->force;
}

float ITestTypes::getDistance()
{
    return this->distance;
}

/*
float ITestTypes::getUMotor_OUT()
{
    return this->uMotor_OUT;
}

float ITestTypes::getElongation()
{
    return this->elongation;
}
*/

//###################################################################################################
// public methods
//###################################################################################################
float ITestTypes::getStartingPoint()
{
    FileHandler fileHandler;
    // gather startingPoint from startingPoint.txt
    std::string buf = fileHandler.readFrom(fileHandler.getStartingPointFilePath());
    if(buf.empty())
    {
        std::cerr << "[ERROR]: in function getStartingPoint: couldn't read from startingPoint.txt" << std::endl;
        return -1.0f;
    }

    unsigned long delimiter = buf.find(' ');
    buf = buf.substr(0, delimiter);

    // convert string to float
    return std::stof(buf);
}

float ITestTypes::getStepTolerance()
{
    FileHandler fileHandler;
    // gather stepTolerance from startingPoint.txt
    std::string buf = fileHandler.readFrom(fileHandler.getStartingPointFilePath());
    if(buf.empty())
    {
        std::cerr << "[ERROR]: in function getStartingPoint: couldn't read from startingPoint.txt" << std::endl;
        return -1.0f;
    }

    unsigned long delimiter = buf.find(' ');
    buf = buf.substr(delimiter+1, buf.size()-1);

    // convert string to float
    return std::stof(buf);
}

//######################################################################################################################
//######################################################################################################################
float ITestTypes::calcUMotor_FBTolerance(float stepTolerance)
{
    return (-0.033f*stepTolerance + 3.3f);
}
//######################################################################################################################

void ITestTypes::calcElongation()
{
    const float bolzenAufsatzLength{/*
 *
 *
 * MEASURE BOLZEN-AUFSATZ
 * --> ohne Phase am Bolzen messen
 *
 */ 63.0};
    FileHandler fileHandler;
    float startingPoint;
    // Motor Auslenkung
    float deflection{0.0};

    startingPoint = this->getStartingPoint();

    deflection += bolzenAufsatzLength;
    // distance must hold the current value of deflection of the motor
    deflection += this->distance;

    // Dehnung der Disc
    this->elongation = deflection - startingPoint;

    if(this->elongation < 0.0)
        this->elongation = 0.0;

}

float ITestTypes::round(float val)
{
    /*
     * 55.236 * 100 = 5523.6
     * 5523.6 + 0.5 = 5524.1
     * (float) ((int) 5524.1 / 100) = 55.24
     */
    return (float) ((int) (val * 100 + 0.5)) / 100;
}

//######################################################################################################################
//######################################################################################################################
bool ITestTypes::reachStartingPoint(const std::string &logFilePath, const Disc disc, std::future<void>& futureObj)
{
    FileHandler fileHandler;
    std::ostringstream logFileHeader;

    logFileHeader << disc.getDateTime()    << "\n"
                  << disc.getWorker()      << ", " << disc.getMachine() << "\n"
                  << disc.getDiscType()    << ", " << disc.getTestType() << "\n\n";

//    I2C i2c;
//    if(!i2c.goToStartingPoint(this->getStartingPoint()))
//    {
//        std::cerr << "[ERROR]: in function reachStartingPoint (" << disc.getTestType() << "::runTest): couldn't go to starting point" << std::endl;
//        return false;
//    }



    bool reachedStartingPoint{true};
    bool stopWatchDog{false};
//    bool returnVal_goToOrigin{true};
    // look for any troubles while motor drives to startingPoint
    std::thread watchDog_KMD = std::thread( [&] {
        while(!stopWatchDog && (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout))
        {
            this->setUKMD_FB(190.0f * 0.001f);//i2c.readAdcKMD_FB());
            if(this->getUKMD_FB() > 200.0f * (float) pow(10.0, -3.0))
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
    usleep(5 * 1'000'000);
    stopWatchDog = true;
    watchDog_KMD.join();

    if(!reachedStartingPoint)
    {
        logFileHeader << "[ERROR]: some troubles occurred while the motor was driving to startingPoint" << std::endl;
        fileHandler.writeInto(logFilePath, logFileHeader.str(), true);

//        this->goToOrigin(disc.getTestType(), 1);
        return false;
    }

    this->setUKMD_FB(400.0f * 0.001f);//i2c.readAdcKMD_FB());
    // lower than 200mV
    if(this->getUKMD_FB() < 200.0f * (float) pow(10.0, -3.0))
    {
        logFileHeader << "[ERROR]: did not reach startingPoint; please make sure that the value of startingPoint in startingPoint.txt is correct" << std::endl;
        fileHandler.writeInto(logFilePath, logFileHeader.str(), true);

//        this->goToOrigin(disc.getTestType(), 2);
        return false;
    }

    logFileHeader   << "[INFO]: the motor reached the startingPoint without any troubles\n"
                    << "Motor-Input-Voltage [V] | Motor-FB-Voltage [V] | Elongation [mm] | KMD-FB-Voltage [V] | Force [N]\n"
                    << "-------------------------------------------------------------------------------------------------\n";
    fileHandler.writeInto(logFilePath, logFileHeader.str(), true);

    return true;
}
//######################################################################################################################

bool ITestTypes::writeToLogFile(const std::string& logFilePath)
{
    std::ostringstream os;
    FileHandler fileHandler;

    // ####################################
    // round each values
    // ####################################
    os << "           " << this->round(this->uMotor_FB)    << "         " << " | "
       << "          " << this->round(this->uMotor_OUT)    << "        " << " | "

//#########################################################################################################################
       << "       " << this->round(this->elongation)       << "       " << " | "
//#########################################################################################################################

       << "        " << this->round(this->uKMD_FB)         << "        " << " | "
       << "   " << this->round(this->force);

    if(!fileHandler.writeInto(logFilePath, os.str(), false))
    {
        std::cerr << "[ERROR]: in function writeToLogFile: couldn't write to log file" << std::endl;
        return false;
    }
    return true;
}

bool ITestTypes::writeToClient(const int slaveSocketTest)
{
    std::ostringstream os;
    float elongation    = this->round(this->elongation);
    float force         = this->round(this->force);

    os << force << " " << elongation;
    std::string str = os.str();

    //int writtenBytes = (int) write(slaveSocketTest, str.c_str(), str.size());

//    if(writtenBytes < 0)
//    {
//        std::cerr << "[ERROR]: in function writeToClient: couldn't write to client" << std::endl;
//        return false;
//    }
    std::cout << "[INFO]: wrote: " << str << " to client" << std::endl;

    return true;
}

bool ITestTypes::goToOrigin(const std::string& testType, unsigned int nrOfCall)
{
//    I2C i2c;
//    if(i2c.goToOrigin()
//    {
//        std::cerr << "[ERROR]: in function goToOrigin ("<< testType <<"::runTest): couldn't go to origin (#"<< nrOfCall <<" goToOrigin())" << std::endl;
//        return false;
//    }
//    return true;
    return false;
}



