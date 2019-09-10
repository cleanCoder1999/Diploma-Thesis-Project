/*********************************************************************************************
*	project-name: 	_1_MMP-CrackTest
*	file: 			socket.h
*
*	copyright: Lucas Müllner
*	date of creation: 11.12.2018
*	description:
*		--> header file of class Socket
*
*********************************************************************************************/

#ifndef INC_1_MMP_CRACKTEST_SOCKET_H
#define INC_1_MMP_CRACKTEST_SOCKET_H

//#include <string>
//#include <sstream>
//#include <iostream>

#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <new>
#include <thread>

//#include "disc.h"
//#include "fileHandler.h"
#include "iTestTypes.h"

#define PORT 25565

/*********************************************************************************************
*	class-name: 	Socket
*	member:
*		--> testType ... base class pointer of class ITestTypes (stability, break, manual)
*		--> testThread ... thread which is used for running ITestTypes::runTest()
*		--> server ... this struct contains necessary pieces of information for the listenSocket
*		--> listenSocket ... descriptor of the listenSocket
*		--> slaveSocket ... descriptor of the slaveSocket
*		--> slaveSocketTest ... descriptor of the slaveSocketTest
*		--> structLen ... length of struct sockaddr_in (server)*
*	inheritance:
*		--> super-class: 	n. a.
*		--> sub-class:		n. a.
*	private-methods:
*		--> createSocket ... referring to socket.cpp
*		--> fillStruct ... referring to socket.cpp
*		--> bindSocket ... referring to socket.cpp
*		--> socketToString ... referring to socket.cpp
*		--> markAsListen ... referring to socket.cpp
*		--> acceptRequest ... referring to socket.cpp
*		--> doWhile ... referring to socket.cpp
*		--> filter ... referring to socket.cpp
*		--> saveForceParams ... referring to socket.cpp
*		--> loadPresetInto ... referring to socket.cpp
*		--> isJoinable ... referring to socket.cpp
*		--> killTestThread ... referring to socket.cpp
*		--> createStabilityTest ... referring to socket.cpp
*		--> createBreakTest ... referring to socket.cpp
*		--> createManualTest ... referring to socket.cpp
*		--> allocatePromiseAndFuture ... referring to socket.cpp
*
*********************************************************************************************/
class Socket {

private:

    // base class pointer (polymorphism)
    ITestTypes* testType;//{nullptr};
    // thread
    std::thread testThread;
    std::promise<void>* exitSignal;
    std::future<void>* futureObj;
    // socket necessities
    struct sockaddr_in server;

    int listenSocket;//{0};
    int slaveSocket;//{0};
    int slaveSocketTest;      // slave which handles the communication during
                              // a test mode (stability, break, manual)
    int structLen;//{0};

    // private methods
    bool createSocket();                            // kind of setListenSocket
    void fillStruct();                              // return bool?
    bool bindSocket();

    std::string socketToString();//int socket);
    bool markAsListen();
    bool acceptRequest();                           // kind of setSlaveSocket
    bool readFromClient();

    // file-handling
    int filter(std::string& msgFromClient);
    bool saveForceParams(const std::string& str);
    bool saveLengthConstants(const std::string& str);
    bool loadPresetInto(Disc *disc, const std::string &preset);


    bool sendForceLengthParams();
    bool sendPresetsFrom(Disc* disc);


    /*ITestTypes* getTestTypes()
    {
        return this->testType;
    }*/
    // multi-threading
    bool isJoinable();
    void allocatePromiseAndFuture();
    void killTestThread();


    void createStabilityTest();
    void createBreakTest();
    void createManualTest();


    // *************************************************
    //ITestTypes* getTestTypes() const;
    //int getListenSocket() const;
    //sockaddr_in getServerStruct() const;
    //int getSlaveSocket() const;
    // *************************************************

public:
    Socket();
    bool run();//bool& cond);

};


#endif //INC_1_MMP_CRACKTEST_SOCKET_H
