////
//// Created by lucas on 28/12/18.
////
//
//#include <thread>
//#include <future>
////#include <chrono>
//
//
//
//#include "socket.h"
////#include "iTestTypes.h"
////#include <iostream>
//
////void easyFunc()
////{
////    ITestTypes* testType{nullptr};
////    testType = new StabilityTest();
////
////    int fd = 3;
////    Disc* disc = new Disc("Lucas Müllner", "Maschine121112");
////
////    testType->runTest(fd, disc);
////}
//
//int main()
//{
//    int fd{3};
//    Socket s;
//    Disc* disc = new Disc("Heribert", "Maschine1212");
//    //ITestTypes* testTypes{nullptr};
//
//    //std::thread thready(testType->runTest, fd, disc);
//    //std::thread thready(easyFunc);
//
//    std::promise<void> exitSignal;
//    std::future<void> futureObj = exitSignal.get_future();
//
//
//    std::thread thrd1 = std::thread([&] {
//
//        std::future_status status = futureObj.wait_for(std::chrono::milliseconds(1));
//
//        if(status == std::future_status::ready)
//            std::cout << "status: ready" << std::endl;
//        else
//            std::cout << "not ready" << std::endl;
//
//    });
//
//
////    s.sendPresetsFrom(disc);
////    s.sendForceLengthParams();
//
///*
//    std::thread thrd2 = std::thread([&] {
//        s.createStabilityTest();
//        //s.getTestTypes()->runTest(fd, disc, futureObj);
//
//    });
//
//    usleep(100000);
//    exitSignal.set_value();
//*/
//
//    //s.killTestThread();
//
//
//    thrd1.join();
////    thrd2.join();
//
//
//
//    return EXIT_SUCCESS;
//}