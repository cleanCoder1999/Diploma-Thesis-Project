/*********************************************************************************************
*	project-name: 	_1_MMP-CrackTest
*	file: 			socket.cpp
*
*	copyright: Lucas Müllner
*	date of creation: 11.12.2018
*	description:
*		--> source file of class Socket
*
*********************************************************************************************/

#include "socket.h"

/*********************************************************************************************
*	function: Socket()
*
*	description:
*		--> default constructor
 *		    every member of this->server (struct sockaddr_in) is set to 0 (zero)
*	parameter:
*		--> n. a.
*	return value:
*		--> n. a.
*
*********************************************************************************************/
Socket::Socket()
        : listenSocket{0}, slaveSocket{0}, structLen{sizeof(server)}
{

    // init promise and future object
    this->allocatePromiseAndFuture();

    // init struct
    memset(&this->server, 0, this->structLen);
}

/*
int Socket::getListenSocket() const {
    return this->listenSocket;
}

sockaddr_in Socket::getServerStruct() const {
    return this->server;
}*/
/*
int Socket::getSlaveSocket() const {
    return this->slaveSocket;
}
*/
/*
ITestTypes* Socket::getTestTypes() const
{
    return this->testType;
}
*/

/*********************************************************************************************
*	function: createSocket()
*
*	description:
*		--> private method for creating a socket
*		    kind of: setListenSocket()
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::createSocket()
{
    // create tcp socket
    int tmp = socket(AF_INET, SOCK_STREAM, 0);
    if(tmp < 0)
    {
        std::cerr << "[ERROR]: in function createSocket: couldn't create socket" << std::endl;
        return false;
    }

    this->listenSocket = tmp;
    return true;
}

/*********************************************************************************************
*	function: fillStruct()
*
*	description:
*		--> private method for filling member server (struct sockaddr_in)
*		    kind of: setServer()
*	parameter:
*		--> n.a.
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Socket::fillStruct()
{
    // must block access through port over ethernet and wifi
    // --> firewall rules
    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(PORT);
    this->server.sin_addr.s_addr = INADDR_ANY;
}

/*********************************************************************************************
*	function: bindSocket()
*
*	description:
*		--> private method for binding a socket (member listenSocket) to an internet
*           address (member server)
*           --> (IPv4)
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::bindSocket()
{
    if(this->server.sin_family == 0)
    {
        std::cerr << "[ERROR]: in function bindSocket: member server hasn't been filled previously" << std::endl;
        return false;
    }

    if(bind(this->listenSocket, (struct sockaddr*) &this->server, (socklen_t) this->structLen) < 0)
    {
        std::cerr << "[ERROR]: in function bindSocket: couldn't bind socket to address (IP and PORT)" << std::endl;
        return false;
    }

    return true;
}

/*********************************************************************************************
*	function: socketToString()
*
*	description:
*		--> private method for printing several pieces of information of member listenSocket
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... a formatted string containing the assigned port number
*	                      and ip address
*
*********************************************************************************************/
std::string Socket::socketToString()//int socket)
{
    std::ostringstream os;
    struct sockaddr_in tmp;

    if(getsockname(this->listenSocket, (struct sockaddr*) &tmp, (socklen_t*) &this->structLen) < 0)
    {
        std::cerr << "[ERROR]: in function socketToString: couldn't get values from member listenSocket" << std::endl;
        return "";
    }

    // convert in_addr.s_addr to uint32_t
    uint32_t byte4, byte3, byte2, byte1;

    byte4 = (ntohl(tmp.sin_addr.s_addr) & 0b11111111000000000000000000000000) >> 24;
    byte3 = (ntohl(tmp.sin_addr.s_addr) & 0b111111110000000000000000) >> 16;
    byte2 = (ntohl(tmp.sin_addr.s_addr) & 0b1111111100000000) >> 8;
    byte1 = ntohl(tmp.sin_addr.s_addr) & 0b11111111;

    os << "[INFO-SERVER]: assigned port number: " << ntohs(tmp.sin_port) << std::endl;

    if(tmp.sin_addr.s_addr == INADDR_ANY)
        os << "[INFO-SERVER]: assigning any ip address" << std::endl;
    else
        os << "[INFO-SERVER]: assigned ip address: " << byte4 << "."
           << byte3 << "."
           << byte2 << "."
           << byte1 << std::endl;

    return os.str();
}

/*********************************************************************************************
*	function: markAsListen()
*
*	description:
*		--> private method for marking member listenSocket as listen socket
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::markAsListen()
{
    // vary backlog size if necessary
    if(listen(this->listenSocket, 5) < 0)
    {
        std::cerr << "[ERROR]: in function markAsListen: couldn't mark member listenSocket as listen socket" << std::endl;
        return false;
    }

    return true;
}

/*********************************************************************************************
*	function: acceptRequest()
*
*	description:
*		--> private method for accepting a client request and creating a slave socket
*           (member slaveSocket) afterwards
*           kind of: setSlaveSocket()
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::acceptRequest()
{
    struct sockaddr tmp;
    int sockTmp = accept(listenSocket, &tmp, (socklen_t*) &this->structLen);

    if(sockTmp < 0)
    {
        std::cerr << "[ERROR]: in function acceptRequest: couldn't create slave socket" << std::endl;
        return false;
    }

    this->slaveSocket = sockTmp;

    return true;
}


//****************************************
// methods used in Socket::run()
// --> these methods use the self-written
//     classes disc.h and fileHandler.h
//****************************************
/*********************************************************************************************
*	function: saveForceParams()
*
*	description:
*		--> private method for delimiting a string into several pieces (substrings);
*		    these substrings represent force values (fNom, fMax) which are going to
*		    be written into the preset-files (cd/dvd/bd.txt)
*	parameter:
*		--> str ... string to be delimited
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::saveForceParams(const std::string& str)
{
    FileHandler fileHandler;
    // message: fNomCD fMaxCD:fNomDVD fMaxDVD:fNomBD fMaxBD
    std::string message = str;

    if(message.empty())
    {
        std::cerr << "[ERROR]: in function saveForceParams: message is empty, no values to save in cd/dvd/bd-files" << std::endl;
        return false;
    }

    unsigned long delimiter = message.find(':');
    std::string data = message.substr(0, delimiter);

    for(int i = 0; i < 3; ++i)
    {
        if(i == 0)
        {
            if(!fileHandler.writeInto(fileHandler.getCDFilePath(), data, true))
                break;
        }
        else if(i == 1)
        {
            if(!fileHandler.writeInto(fileHandler.getDVDFilePath(), data, true))
                break;
        }
        else if(i == 2)
        {
            if(!fileHandler.writeInto(fileHandler.getBDFilePath(), data, true))
                break;

            return true;
        }

        message = message.substr((delimiter+1), (message.size()-1));
        delimiter = message.find(':');
        data = message.substr(0, delimiter);
    }

    std::cerr << "[ERROR]: in function saveForceParams: couldn't write to cd/dvd/bd-file" << std::endl;
    return false;

}

/*********************************************************************************************
*	function: loadPresetInto()
*
*	description:
*		--> private method to set the right discType in order to gather the correct
*		    force values
*	parameter:
*		--> disc ... pointer of Disc-object
*		--> preset ... a string which consists of CD, DVD or BD
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::loadPresetInto(Disc *disc, const std::string &preset)
{
    bool cond = false;
    if(preset == "CD")
    {
        if(!disc->setDiscType("CD"))
            cond = true;
    }
    else if(preset == "DVD")
    {
        if(!disc->setDiscType("DVD"))
            cond = true;
    }

    else if(preset == "BD")
    {
        if(!disc->setDiscType("BD"))
            cond = true;
    }

    if(cond)
    {
        std::cerr << "[ERROR]: in function loadPresetInto: couldn't set discType" << std::endl;
        return false;
    }

    return true;
}

/*********************************************************************************************
*	function: saveLengthConstants()
*
*	description:
*		--> private method to load and save origin point (Nullpunkt) and
*		    step-tolerance (minimale Schrittweite des Motors) of the motor
*	parameter:
*		--> str ... string which contains the motors origin point and step-tolerance
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::saveLengthConstants(const std::string& str)
{
    FileHandler fileHandler;

    if(!fileHandler.writeInto(fileHandler.getStartingPointFilePath(), str, true))
    {
        std::cerr << "[ERROR]: in function saveLengthConstants: couldn't write to originPoint-file" << std::endl;
        return false;
    }

    return true;
}

/*********************************************************************************************
*	function: sendForeParams()
*
*	description:
*		--> private method to send parameter of force to the client
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::sendForceLengthParams()
{
    FileHandler fileHandler;
    std::ostringstream os;

    // fNom fMax --> 12.3 45.8:12.3 45.8:12.3 45.8
    os << fileHandler.readFrom(fileHandler.getCDFilePath()) << ":"
       << fileHandler.readFrom(fileHandler.getDVDFilePath()) << ":"
       << fileHandler.readFrom(fileHandler.getBDFilePath()) << ":"
       << fileHandler.readFrom(fileHandler.getStartingPointFilePath());

    std::string str = os.str();

    std::cout << "ForceLength-C-String: " << str.c_str() << std::endl;
    std::cout << "ForceLength-String length: " << str.size() << std::endl;

    int writtenBytes = (int) write(this->slaveSocket, str.c_str(), str.size());

    if(writtenBytes < 0)
    {
        std::cerr << "[ERROR]: in function sendForeParams: couldn't write to client" << std::endl;
        return false;
    }

    return true;
}

/*********************************************************************************************
*	function: sendPresetsFrom()
*
*	description:
*		--> private method to send chosen presets (fNom and fMax of a certain disc)
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
/*
bool Socket::sendPresetsFrom(Disc* disc)
{
    std::ostringstream os;
    std::string str;

    disc->setDiscType("DVD");

    os << disc->getFNom() << " "
       << disc->getFMax();

    str = os.str();

    std::cout << "Preset-C-String: " << str.c_str() << std::endl;
    std::cout << "Preset-String length: " << str.size() << std::endl;

    int writtenBytes = (int) write(this->slaveSocket, str.c_str(), str.size());

    if(writtenBytes < 0)
    {
        std::cerr << "[ERROR]: in function sendPresetsFrom: couldn't write to client" << std::endl;
        return false;
    }

    return true;
}
*/

/*********************************************************************************************
*	function: createStabilityTest()
*
*	description:
*		--> private method for creating a dynamic object of class StabilityTest
*		    via member testType (ITestTypes*)
*	parameter:
*		--> n.a.
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Socket::createStabilityTest()
{
    // following call doesn't return any value
    // --> try-catch
    try {
        this->testType = new StabilityTest();
    }
    catch (std::bad_alloc& e) {
        std::cerr << "[ERROR]: in function createStabilityThread: couldn't allocate StabilityTest (" << e.what() << ")" << std::endl;
    }
    catch (...) {
        std::cerr << "[ERROR]: in function createStabilityThread: any exception" << std::endl;
    }
}

/*********************************************************************************************
*	function: createBreakTest()
*
*	description:
*		--> private method for creating a dynamic object of class BreakTest
*		    via member testType (ITestTypes*)
*	parameter:
*		--> n.a.
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Socket::createBreakTest()
{
    // following call doesn't return any value
    // --> try-catch
    try {
        this->testType = new BreakTest();
    }
    catch (std::bad_alloc& e) {
        std::cerr << "[ERROR]: in function createBreakTest: couldn't allocate BreakTest (" << e.what() << ")" << std::endl;
    }
    catch (...) {
        std::cerr << "[ERROR]: in function createBreakTest: any exception" << std::endl;
    }
}

/*********************************************************************************************
*	function: createManualTest()
*
*	description:
*		--> private method for creating a dynamic object of class ManualTest
*		    via member testType (ITestTypes*)
*	parameter:
*		--> n.a.
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Socket::createManualTest()
{
    // following call doesn't return any value
    // --> try-catch
    try {
        this->testType = new ManualTest();
    }
    catch (std::bad_alloc& e) {
        std::cerr << "[ERROR]: in function createManualTest: couldn't allocate ManualTest (" << e.what() << ")" << std::endl;
    }
    catch (...) {
        std::cerr << "[ERROR]: in function createManualTest: any exception" << std::endl;
    }
}

/*********************************************************************************************
*	function: killTestThread()
*
*	description:
*		--> private method to free memory used by member testType and
*		    to stop the execution of member testThread
*	parameter:
*		--> n.a.
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Socket::killTestThread()//std::thread& thread)
{
    // following calls don't return any values
    // --> try-catch
    try {

        this->exitSignal->set_value();

        this->testThread.join();

        delete this->testType;
        this->testType = nullptr;

        // free memory used by exitSignal (std::promise<void>) and futureObj (std::future<void>)
        // in order to prevent a future_error caused by a second call of set_value()
        delete this->exitSignal;
        delete this->futureObj;
        this->exitSignal = nullptr;
        this->futureObj = nullptr;

    }
    catch (std::bad_alloc& e) {
        std::cerr << "[ERROR]: in function killTestThread: couldn't delete testType (" << e.what() << ")" << std::endl;
    }
    catch (std::system_error& e) {
        std::cerr << "[ERROR]: in function killTestThread: couldn't detach or destroy thread (" << e.what() << ")" << std::endl;
    }
    catch (std::future_error& e) {
        std::cerr << "[ERROR]: in function killTestThread: couldn't make future-object ready (" << e.what() << ")" << std::endl;
    }
    catch (...) {
        std::cerr << "[ERROR]: in function killTestThread: any exception" << std::endl;
    }
}

/*********************************************************************************************
*	function: isJoinable()
*
*	description:
*		--> private method to check if member testThread (std::thread) is still executing
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether member testThread is still executing (true)
*		                  or not (false)
*
*********************************************************************************************/
/*
bool Socket::isJoinable()//std::thread& thread)
{
    /*
    std::cout << "in isJoinable: Thread-ID (before join): " << thread.get_id() << std::endl;
    try {
        this->testThread.join();
    }
    catch (std::system_error& e) {
        std::cerr << "[ERROR]: in function isJoinable: couldn't join thread (" << e.what() << ")" << std::endl;
    }
    catch (...) {
        std::cerr << "[ERROR]: in function createStabilityThread: any exception" << std::endl;
    }

    // thread id of non-executing thread
    std::cout << "in isJoinable: Thread-ID (after join): " << thread.get_id() << std::endl;
    *
    if(this->testThread.joinable())
    {
        std::cerr << "[ERROR]: in function isJoinable: thread is still executing" << std::endl;
        return true;
    }

    return false;
}
*/

/*********************************************************************************************
*	function: allocatePromiseAndFuture()
*
*	description:
*		--> private method to allocate memory for member exitSignal and futureObj
*	parameter:
*		--> n.a.
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Socket::allocatePromiseAndFuture()
{
    try {
        this->exitSignal = new std::promise<void>();
        this->futureObj = new std::future<void>();
        *this->futureObj = this->exitSignal->get_future();
    }
    catch (std::bad_alloc& e)
    {
        std::cerr << "[ERROR]: in function allocatePromiseAndFuture: couldn't allocate exitSignal and futureObj (" << e.what() << ")" << std::endl;
    }
}

/*********************************************************************************************
*	function: filter()
*
*	description:
*		--> private method which delimits a string in a name-String and a data-String;
*		    the name-String is going to be compared with constant strings, these constant
*		    strings represent button names in the UI
*	parameter:
*		--> msgFromClient ... represents a message from a client
*	return value:
*		--> returnVal ... id (identification) of a certain button in the UI
*
*********************************************************************************************/
int Socket::filter(std::string& msgFromClient)
{

    /* string format:
     * btn<Name>
     * ################
     *
     * btnSta ... Start Button
     * btnNul ... Nullpunkt speichern (...)
     * btnPar ... Parameter speichern (fNom, fMax)
     * btnPrS ... Preset (CD,DVD,BD)
     * btnSTe ... Stabilitätstest
     * btnBTe ... Breaktest
     * btnMTe ... manual test
     * btnAbr ... Abbruch Button
     * btnFer ... Fertig Button (manual test)
     * btnLoO ... Log-Out Button
     */

    // button name
    std::string btnName = msgFromClient.substr(0,6);

    // additional data (operator, machine, fNom, ...)
    msgFromClient = msgFromClient.substr(6, msgFromClient.size() - 1);


    std::cout << "Button-Name: " << btnName << std::endl;
    std::cout << "Button-Value: " << msgFromClient << std::endl;

    if(btnName == "btnAbr")
        return 1;
    else if(btnName == "btnSta")
        return 2;

    else if(btnName == "btnNul")
        return 3;
    else if(btnName == "btnPar")
        return 4;

    else if(btnName == "btnPrS")
        return 5;

    else if(btnName == "btnSTe")
        return 6;
    else if(btnName == "btnBTe")
        return 7;
    else if(btnName == "btnMTe")
        return 8;

    else if(btnName == "btnFer")
        return 9;
    else if(btnName == "btnLoO")
        return 10;

    else
        return -1;  // default case
}


// static, global variables
static char msgBuffer[200]{""};
static std::string message("");

/*********************************************************************************************
*	function: readFromClient()
*
*	description:
*		--> private method which handles the communication with a client
*		    with the aid of a socket
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::readFromClient()
{
    int readBytes{1};
    ::message = "";
/*
    //std::cout << "sizeof(msgBuffer): " << sizeof(::msgBuffer) << std::endl;
    do {

        // read (don't forget the string-end character '\0')
        readBytes = (int) read(this->slaveSocket, ::msgBuffer, sizeof(::msgBuffer));

        // error while reading
        if(readBytes < 0)
        {
            std::cerr << "[ERROR]: in function readFromClient: couldn't read from client" << std::endl;
            return false;
        }

        else if(readBytes == 0)
            std::cout << "######################### TRANSMISSION DONE #########################" << std::endl;

            // print received data onto console
        else
        {
            //std::cout << "whole message before: " << ::message << std::endl;
            //std::cout << "readBytes: " << readBytes << " ...\n\t" << ::msgBuffer << std::endl;
            ::message += ::msgBuffer;
            std::cout << "\nwhole message after read(): " << ::message << std::endl;
        }

    } while(readBytes != 0);
*/
//#######################################################################################################################################
    readBytes = (int) read(this->slaveSocket, ::msgBuffer, sizeof(::msgBuffer));

    // error while reading
    if(readBytes < 0)
    {
        std::cerr << "[ERROR]: in function readFromClient: couldn't read from client" << std::endl;
        return false;
    }

    ::message = ::msgBuffer;

//#######################################################################################################################################

    // make message buffer empty
    memset(::msgBuffer, 0, sizeof(::msgBuffer));

    //****************************************
    // !!! ::message mustn't be made empty !!!
    //****************************************

    return true;
}

/*********************************************************************************************
*	function: run()
*
*	description:
*		--> public method: it's the main function of the whole project;
*		                   every self-written class and method is used in Socket::run()
*	parameter:
*		--> cond ... condition for executing the while-loop in Socket::run()
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Socket::run()//bool& cond)
{
    bool cond{true};
    int id{};
    Disc* disc{nullptr};
    FileHandler fileHandler;

    //**************************************************************
    // init
    // --> socket, sockaddr_in, bind, listen
    //**************************************************************
    if(!this->createSocket())   return false;

    this->fillStruct();

    if(!this->bindSocket())     return false;

    if(!this->markAsListen())   return false;

    std::cout << this->socketToString();

    //**************************************************************
    // accept and loop
    // --> accept, read
    //**************************************************************
    while(true)//cond)
    {
        // create slave socket
        if(!this->acceptRequest())   return false;

        // read from client via slave socket
        if(!this->readFromClient())
        {
            close(this->slaveSocket);
            return false;
        }

        //std::cout << "after readFromClient()" << std::endl;

        id = this->filter(::message);
        std::cout << "######################### FILTER DONE #########################" << std::endl;

        /*
         * --> prüfen, ob ein Aufruf der close()-Fkt (close(slaveSocket)), nachdem der Deskriptor
         *      dem slaveSocketTest zugewiesen wurde, ein Problem bei der Kommunikation (write())
         *      während eines Tests verursacht
         */

        switch (id)
        {
            /*
             * der Button "erneuter Test" verwendet dieselbe ID
             * wie die einzelnen Test Modis
             */
//#######################################################################################################################################
//#######################################################################################################################################
//                CREATE SHUTDOWN CASE (case 11)
//                --> system call (sudo poweroff); determined as shutdown command in terminal
//#######################################################################################################################################
//#######################################################################################################################################
                // ABBRUCH (bis vordefinierte Parameter)
                // --> works
            case 1:
            {
                std::cout << "------------------------------------- ABBRUCH (case 1) -------------------------------------" << std::endl;
                // write to log-file
                // --> ABBRUCH
                if(this->testThread.joinable())
                {
                    this->killTestThread();
                    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~ KILLED THREAD ~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
                }

//#######################################################################################################################################
//                GoToOrigin go = GoToOrigin();
//                if(!go.run())
//                    return false;
//#######################################################################################################################################
                break;
            }

                // START
                // --> works
            case 2:
            {
                std::cout << "------------------------------------- START (case 2) -------------------------------------" << std::endl;
                // additional info: Operator, Maschine
                // ::message: operatorName,machine
                unsigned long delimiterPos = ::message.find(',');
                unsigned long lastElemPos = (::message.size()-1);
                // exception handling
                try {
                    disc = new Disc(::message.substr(0, delimiterPos), ::message.substr((delimiterPos+1), lastElemPos));
                }
                catch (std::bad_alloc& e) {
                    std::cerr << "[ERROR]: in function run: couldn't allocate disc (" << e.what() << ")" << std::endl;
                }

                if(!this->sendForceLengthParams())
                    return false;

                break;
            }

                // Nullpunkt festlegen --> Pop-Up
                // --> works
            case 3:
            {
                std::cout << "------------------------------------- save ORIGIN and STEP-TOLERANCE (case 3) -------------------------------------" << std::endl;
                // ::message: "originPoint stepTolerance"
                // ... --> fileHandler
                if(!this->saveLengthConstants(::message))
                    return false;
                break;
            }

                // Parameter ändern --> Parameter SPEICHERN
                // --> works
            case 4:
            {
                std::cout << "------------------------------------- save FORCE-PARAMS (case 4) -------------------------------------" << std::endl;
                // every single parameter (CD: fNom,fMax; DVD: ...)
                // ::message: fNomCD fMaxCD:fNomDVD fMaxDVD:fNomBD fMaxBD
                if(!this->saveForceParams(::message))
                    return false;
                break;
            }

                // Presets (CD, DVD, BD)
                // --> works
            case 5:
            {
                std::cout << "------------------------------------- load PRESETS into DISC object (case 5) -------------------------------------" << std::endl;
                if(!this->loadPresetInto(disc, ::message))
                    return false;

                if(this->futureObj == nullptr && this->exitSignal == nullptr)
                    this->allocatePromiseAndFuture();

                break;
            }

                // stability test
                // --> works, however, runTest() must still be implemented
            case 6:
            {
                std::cout << "------------------------------------- STABILITY TEST (case 6) -------------------------------------" << std::endl;
                //disc->setTestType("stability test");
                if(this->testThread.joinable())
                    this->killTestThread();


                testThread = std::thread([&] {
                    this->createStabilityTest();

                    this->slaveSocketTest = this->slaveSocket;
                    //close(this->slaveSocket);
                    this->slaveSocket = 0;
                    if(!this->testType->runTest(this->slaveSocketTest, disc, *this->futureObj))
                        std::cerr << "[ERROR]: in lambda-function: error while runTest (StabilityTest::runTest())" << std::endl;

                    return;
                });
                std::cout << "[INFO]: thread id: " << testThread.get_id() << std::endl;

                break;
            }

                // break test
                // --> works, however, runTest() must be implemented still
            case 7:
            {
                std::cout << "------------------------------------- BREAK TEST (case 7) -------------------------------------" << std::endl;
                //disc->setTestType("break test");
                if(this->testThread.joinable())
                    this->killTestThread();

                // lambda-function
                testThread = std::thread([&] {
                    this->createBreakTest();

                    this->slaveSocketTest = this->slaveSocket;
                    //close(this->slaveSocket);
                    this->slaveSocket = 0;
                    if(!this->testType->runTest(this->slaveSocketTest, disc, *this->futureObj))
                        std::cerr << "[ERROR]: in lambda-function: error while runTest (BreakTest::runTest())" << std::endl;

                    return;
                });
                std::cout << "[INFO]: thread id: " << testThread.get_id() << std::endl;

                break;
            }

                // manual test
                // --> works, however, runTest() must be implemented still
            case 8:
            {
                std::cout << "------------------------------------- MANUAL TEST (case 8) -------------------------------------" << std::endl;
                //disc->setTestType("manual test");
                if(this->testThread.joinable())//this->isJoinable())
                    this->killTestThread();

                testThread = std::thread([&] {
                    this->createManualTest();

                    this->slaveSocketTest = this->slaveSocket;
                    //close(slaveSocket);
                    this->slaveSocket = 0;
                    if(!this->testType->runTest(this->slaveSocketTest, disc, *this->futureObj))
                        std::cerr << "[ERROR]: in lambda-function: error while runTest (ManualTest::runTest())" << std::endl;

                    return;
                });
                std::cout << "[INFO]: thread id " << testThread.get_id() << std::endl;

                break;
            }

                // fertig
                // --> "kill" thread of manual test
                // --> should work
            case 9:
            {
                std::cout << "------------------------------------- manual test DONE (case 9) -------------------------------------" << std::endl;
                // !!! how to stop logging in a good way? !!!

                if(this->testThread.joinable())
                    this->killTestThread();


//#######################################################################################################################################
//                GoToOrigin go = GoToOrigin();
//                if(!go.run())
//                    return false;
//#######################################################################################################################################
                break;
            }

                // log out
            case 10:
            {
                std::cout << "------------------------------------- LOG OUT (case 10) -------------------------------------" << std::endl;
                // exception handling
                try {
                    delete disc;
                    disc = nullptr;
                }
                catch (std::bad_alloc& e) {
                    std::cerr << "[ERROR]: in function run (case 10): couldn't delete disc (" << e.what() << ")" << std::endl;
                }
                //cond = false;
                break;
            }

            default:
            {
                std::cout << "------------------------------------- DEFAULT case -------------------------------------" << std::endl;
                std::cerr << "[ERROR]: in function run: wrong btnName" << std::endl;
                return false;
            }
        }

        ::message = "";
//#######################################################################################################################################
        // connection to client must be closed in order to read and write data within one while()-cycle
        // --> the connection must be maintained so that data (fNom, fMax, ...) can be sent (to GUI) and displayed at GUI
//        close(this->slaveSocket);
//#######################################################################################################################################

    }

    // if user didn't log out
    if(disc != nullptr)
        delete disc;

    if(this->testType != nullptr)
        delete testType;

    if(this->testThread.joinable())
        this->killTestThread();

    close(this->slaveSocket);
    close(this->listenSocket);

    return true;
}
