/*********************************************************************************************
*	project-name: 	_1_MMP-CrackTest
*	file: 			fileHandler.h
*
*	copyright: Lucas Müllner
*	date of creation: 10.11.2018
*	description:
*		--> header file of class FileHandler
*
*********************************************************************************************/

#ifndef INC_000_MMP_CRACKTEST_FILEHANDLER_H
#define INC_000_MMP_CRACKTEST_FILEHANDLER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

/*********************************************************************************************
*	class-name: 	FileHandler
*	member:
*		--> cdFilePath ... location of the cd.txt-file
*		--> dvdFilePath ... location of the dvd.txt-file
*		--> bdFilePath ... location of the bd.txt-file
*		--> logFilePath ... location of the log.txt-file
*	inheritance:
*		--> super-class: 	n. a.
*		--> sub-class:		n. a.
*	private-methods:
*		--> openFile ... referring to fileHandler.cpp
*		--> closeFile ... referring to fileHandler.cpp
*
*********************************************************************************************/
class FileHandler {


private:
    std::string cdFilePath{"/home/lucas/MMP-CrackTest/cd.txt"};//"/home/lucas/MMP-CrackTest/cd.txt"};
    std::string dvdFilePath{"/home/lucas/MMP-CrackTest/dvd.txt"};//"/home/lucas/MMP-CrackTest/dvd.txt"};
    std::string bdFilePath{"/home/lucas/MMP-CrackTest/bd.txt"};//"/home/lucas/MMP-CrackTest/bd.txt"};
    std::string startingPointFilePath{"/home/lucas/MMP-CrackTest/startingPoint.txt"};///home/pi/Documents/MMP-CrackTest/"/home/lucas/MMP-CrackTest/originPoint.txt"};

    std::string logFolderPath{"/home/lucas/MMP-CrackTest/logFolder"};//"/home/lucas/MMP-CrackTest/"};
    std::string shutDownLogFilePath{"/home/lucas/MMP-CrackTest/shutdown.log"};//"/home/lucas/MMP-CrackTest/shutdown.log"};


    // std::string justierungFilePath;

    // open / close
    std::fstream openFile(const std::string& filepath, std::string mode);
    bool closeFile(std::fstream& file);


public:

    FileHandler();

    // getters
    std::string getCDFilePath() const;
    std::string getDVDFilePath() const;
    std::string getBDFilePath() const;
    std::string getLogFolderPath() const;
    std::string getShutDownLogFilePath() const;
    std::string getStartingPointFilePath() const;
    //std::string getJUSTFilePath();


    // create
    std::string createLogFile(const std::string& filename);
    std::string getShutDownString();

    // read / write
    std::string readFrom(const std::string& filepath);
    bool writeInto(const std::string& filepath, const std::string& data, const bool truncate);

};


#endif //INC_000_MMP_CRACKTEST_FILEHANDLER_H