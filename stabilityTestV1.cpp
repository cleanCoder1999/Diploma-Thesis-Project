////
//// Created by lucas on 28/12/18.
////
//
//#include "iTestTypes.h"
//
//StabilityTest::StabilityTest() {}
//
///*
//bool StabilityTest::runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj)
//{
//    disc->setTestType("stability test");
//
//    std::cout << "[INFO]: in class StabilityTest: in function runTest" << std::endl;
//    std::cout << "\tName: " << disc->getWorker() << std::endl;
//    std::cout << "\tMaschine: " << disc->getMachine() << std::endl;
//    std::cout << "\tDiscType: " << disc->getDiscType() << std::endl;
//    std::cout << "\tTestType: " << disc->getTestType() << std::endl;
//    std::cout << "\n\tfile descriptor: " << slaveSocketTest << std::endl;
//
//
//    return true;
//}
//*/
//
//bool StabilityTest::runTest(int slaveSocketTest, Disc* disc, std::future<void>& futureObj)
//{
//    disc->setTestType("stability test");
//
//    FileHandler handler;
//    std::ostringstream logFileHeader;
//    std::string logFilePath = handler.createLogFile(disc->createFilename());
//
////    I2C i2c;
//
//    logFileHeader << disc->getDateTime() << "\n"
//                  << disc->getWorker() << ", " << disc->getMachine() << "\n"
//                  << disc->getDiscType() << ", " << disc->getTestType() << "\n\n"
//
//                  << "Motor-Input-Voltage | Motor-FB-Voltage | Distance | KMD-FB-Voltage | Force\n";
//
//
//    if(handler.writeInto(logFilePath, logFileHeader.str(), true))
//        std::cout << "writeInto(logFile) worked fine..." << std::endl;
//
//
//
//
//    bool returnVal{false};
//    float motorVoltage_FB{0.0};
//    std::ostringstream os;
//    int i{0};
//
//    // 0mm 		--> 3.3V
//    // 100mm	--> 0V
//    // ==> 1mm = 0.033V
//    while(futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout || i < 3)
//    {
//        //motorVoltage_FB = i2c.readAdcMotor_FB();
//
//        if(3.1 < motorVoltage_FB || i == 0)
//        {
//            if(!this->writeToLogFile(logFilePath) || !this->writeToClient(slaveSocketTest))
//                return false;
//
//            //returnVal = i2c.writeToDacMotor(50.0, 0);
//            // wait for three seconds
//            std::cout << "wait a second..." << std::endl;
//            usleep(3000000);
//            i = 1;
//        }
//        else if((1.50 < motorVoltage_FB && motorVoltage_FB < 1.70) || i == 1)
//        {
//            if(!this->writeToLogFile(logFilePath))
//                return false;
//
//            //returnVal = i2c.writeToDacMotor(100.0, 0);
//            // wait for three seconds
//            std::cout << "wait a second..." << std::endl;
//            usleep(3000000);
//            i = 2;
//        }
//        else if((motorVoltage_FB < 0.165) || i == 2)
//        {
//            if(!this->writeToLogFile(logFilePath))
//                return false;
//
//            //returnVal = i2c.writeToDacMotor(0.0, 0);
//            // wait for three seconds
//            std::cout << "wait a second..." << std::endl;
//            usleep(3000000);
//            i = 3;
//        }
//
//
//        if(!returnVal)
//            std::cout << "writing to DAC (MCP4716) worked fine..." << std::endl;
//        else
//        {
//            std::cerr << "[ERROR]: in function runTest (StabilityTest::runTest): couldn't write to DAC" << std::endl;
//            return false;
//        }
//
//        os.str("");
//    }
//
////    if(!this->goToOrigin())
////    {
////        std::cerr << "[ERROR]: in function runTest (StabilityTest::runTest): couldn't go to origin (goToOrigin())" << std::endl;
////        return false;
////    }
//
//
//    return true;
//}
