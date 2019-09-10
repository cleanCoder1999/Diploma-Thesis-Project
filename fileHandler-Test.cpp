////
//// Created by lucas on 10/11/18.
////
//
//#include <stdlib.h>
////#include <iostream>
////#include <fstream>
//
//
//#include <sstream>
//
//#include "fileHandler.h"
//#include "disc.h"
//
//// alles funktioniert, c++ is nais
//int main(void)
//{
//    std::string filepath("/home/lucas/MMP-CrackTest/01_file");
//    std::fstream file;
//    FileHandler fh;
//    Disc d("Josef", "Maschine3");
//
//    d.setDiscType("CD");
//    d.setTestType("Stability");
//    std::string logFilePath = fh.createLogFile(d.createFilename());
//
//    std::cout << "filename: " << logFilePath << std::endl;
//
//    std::ostringstream data;
//    data << d.getDateTime() << "\n"
//         << d.getMachine()  << ", " << d.getWorker() << "\n"
//         << d.getDiscType() << ", " << d.getTestType() << "\n"
//         << "Fnom: " << d.getFNom() << ", Fmax: " << d.getFMax();
//
//    fh.writeInto(logFilePath, data.str(), false);
//
//
//    // open / close
//    /*
//    file = fh.openFile(filepath, "w");
//    fh.closeFile(file);
//    */
//    // write / read
///*
//    fh.writeInto(filepath, "alles funktioniert, c++ is nais", true);
//
//    std::string input(fh.readFrom(filepath));
//    std::cout << "input: " << input << std::endl;
//
//
//    if(fh.writeInto(filepath, "neeew dataaaa", true))
//        std::cout << ".writeTo() works" << std::endl;
//*/
//    /*
//    file.open(filepath.c_str(), std::ios::out | std::ios::trunc);
//    file << "neeew dataaaa" << std::endl;
//    file.close();
//    if(file.is_open())
//        std::cout << "something went wrong" << std::endl;
//    */
////    std::cout << fh.readFrom(filepath) << std::endl;
//
//
//    return EXIT_SUCCESS;
//}
//
