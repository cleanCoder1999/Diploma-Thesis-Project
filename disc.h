/*********************************************************************************************
*	project-name: 	_1_MMP-CrackTest
*	file: 			disc.h
*
*	copyright: Lucas Müllner
*	date of creation: 11.12.2018
*	description:
*		--> header file of class Disc
*
*********************************************************************************************/

#ifndef INC_1_MMP_CRACKTEST_DISC_H
#define INC_1_MMP_CRACKTEST_DISC_H

//#include <iostream>
//#include <string>
//#include <sstream>
//#include <ctime>

#include "fileHandler.h"

/*********************************************************************************************
*	class-name: 	Disc
*	member:
*		--> worker ... name of operator whose at the MMP-CrackTest
*		--> machine ... id of machine where the discs come from
*		--> discType ... type of disc (CD, DVD, BD)
*		--> fMax ... maximum force value (disc shouldn't be able to withstand this force value)
*		--> fNom ... nominal force value (disc should withstand this force value)
*		--> testType ... chosen type of test (stability, break, manual)
*		--> dateTime ... time and date when a test starts
*	inheritance:
*		--> super-class: 	n. a.
*		--> sub-class:		n. a.
*	private-methods:
*		--> gatherForceValues ... referring to disc.cpp
*		--> setFMax ... referring to disc.cpp
*		--> setFNom ... referring to disc.cpp
*		--> setDateTime ... referring to disc.cpp
*
*********************************************************************************************/
class Disc {

private:
    // first window
    std::string worker;
    std::string machine;

    // second window
    std::string discType;
    float fMax{0.0};
    float fNom{0.0};

    // third window
    std::string testType;
    std::string dateTime{""};

    // gather values from files
    bool gatherForceValues();
    // set member fMax and fNom
    void setFMax(float fmax);
    void setFNom(float fnom);

    // set date time
    void setDateTime();

public:
    // constructor
    Disc(const std::string& worker, const std::string& machine);
    Disc()=delete;

    // setters
    bool setDiscType(const std::string& discType);
    void setTestType(const std::string& testType);


    // getters
    std::string getWorker() const;            // gather information from socket before calling this method
    std::string getMachine() const;           // gather information from socket before calling this method

    std::string getDiscType() const;          // gather information from socket before calling this method
    float getFMax() const;
    float getFNom() const;

    std::string getTestType() const;          // gather information from socket before calling this method
    std::string getDateTime() const;          // gather information from socket before calling this method

    // public methods
    std::string createFilename();
};


#endif //INC_1_MMP_CRACKTEST_DISC_H
