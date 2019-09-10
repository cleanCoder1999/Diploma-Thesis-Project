////
//// Created by lucas on 18/12/18.
////
//
//#include "socket.h"
//
//int main()
//{
//    Socket s;
//    char msgBuffer[256];
//    std::string endStr = "close connection";
//
///*
//    if(s.createSocket())
//        std::cout << "socket has been created, descriptor: " << s.getListenSocket() << std::endl;
//
//    s.fillStruct();
//
//    if(s.bindSocket())
//        std::cout << "socket has been bound" << std::endl;
//
//    if(s.markAsListen())
//        std::cout << "socket has been marked as listen socket\n" << std::endl;
//
//    std::cout << s.socketToString(s.getListenSocket());
//
//    bool cond{true};
//    while(cond)
//    {
//        if(s.acceptRequest())
//            std::cout << "created slave" << std::endl;
//
//        if(s.readFromClient())
//           std::cout << "\nreading worked without any errors" << std::endl;
//
//    }
//*/
////    std::string str = "btnAbr";
////    std::string str = "btnStaLucas Müllner,Maschine13";
////    std::string str = "btnNul";
////    std::string str = "btnPar";
////    std::string str = "btnPsB";
////    std::string str = "btnBTe";
////    std::string str = "btnFer";
//
////    std::cout << s.filter(str) << std::endl;
////    std::cout << str << std::endl;
//
//    bool cond = true;
//    s.run(cond);
////    Disc d("josef", "13");
////    d.setDiscType("DVD");
//
//    return EXIT_SUCCESS;
//}