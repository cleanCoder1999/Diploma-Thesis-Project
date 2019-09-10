/*********************************************************************************************
*	project-name: 	_1_MMP-CrackTest
*	file: 			fileHandler.cpp
*
*	copyright: Lucas Müllner
*	date of creation: 10.11.2018
*	description:
*		--> source file of class FileHandler
*
*********************************************************************************************/

#include "fileHandler.h"

/*********************************************************************************************
*	function: FileHandler()
*
*	description:
*		--> default constructor
*	parameter:
*		--> n. a.
*	return value:
*		--> n. a.
*
*********************************************************************************************/
FileHandler::FileHandler() {}

/*********************************************************************************************
*	function: openFile()
*
*	description:
*		--> private method for opening a file through std::fstream class
*	parameter:
*		--> filepath ... location of the file which will be opened
 *		--> mode ... decides whether to write (w, wd) or to read (r) from file
*	return value:
*		--> returnVal ... stream which is connected to the open file
*
*********************************************************************************************/
std::fstream FileHandler::openFile(const std::string& filepath, std::string mode)
{

    std::fstream file;

    // append and write
    if(mode == "w")
        file.open(filepath.c_str(), std::ios::out | std::ios::app);
        // truncate and write
    else if(mode == "wd")
        file.open(filepath.c_str(), std::ios::out | std::ios::trunc);
        // read
    else if(mode == "r")
        file.open(filepath.c_str(), std::ios::in);
    else
    {
        std::cerr << "[ERROR]: in function openFile: parameter missing" << std::endl;
        return (std::fstream) nullptr;
    }

    if(file.fail())
    {
        std::cerr << "[ERROR]: in function openFile: couldn't open file" << std::endl;
        return (std::fstream) nullptr;
    }

    return file;
}

/*********************************************************************************************
*	function: closeFile()
*
*	description:
*		--> private method for closing a file through std::fstream class
*	parameter:
*		--> file ... std::fstream/file that should be closed
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool FileHandler::closeFile(std::fstream& file)
{
    file.close();

    if(file.is_open())
    {
        std::cerr << "[ERROR]: in function closeFile: couldn't close file" << std::endl;
        return false;
    }

    return true;
}

/*********************************************************************************************
*	function: getCDFilePath()
*
*	description:
*		--> getter method of member cdFilePath
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member cdFilePath
*
*********************************************************************************************/
std::string FileHandler::getCDFilePath()  const
{
    return this->cdFilePath;
}

/*********************************************************************************************
*	function: getDVDFilePath()
*
*	description:
*		--> getter method of member dvdFilePath
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member dvdFilePath
*
*********************************************************************************************/
std::string FileHandler::getDVDFilePath()  const
{
    return this->dvdFilePath;
}

/*********************************************************************************************
*	function: getBDFilePath()
*
*	description:
*		--> getter method of member bdFilePath
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member bdFilePath
*
*********************************************************************************************/
std::string FileHandler::getBDFilePath()  const
{
    return this->bdFilePath;
}

/*********************************************************************************************
*	function: getLogFolderPath()
*
*	description:
*		--> getter method of member logFolderPath
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member logFolderPath
*
*********************************************************************************************/
std::string FileHandler::getLogFolderPath()  const
{
    return this->logFolderPath;
}

/*********************************************************************************************
*	function: getShutdownLogFilePath()
*
*	description:
*		--> getter method of member shutdownLogFilePath
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member shutdownLogFilePath
*
*********************************************************************************************/
std::string FileHandler::getShutDownLogFilePath() const
{
    return this->shutDownLogFilePath;
}

/*********************************************************************************************
*	function: getStartingPointFilePath()
*
*	description:
*		--> getter method of member startingPointFilePath
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member startingPointFilePath
*
*********************************************************************************************/
std::string FileHandler::getStartingPointFilePath() const
{
    return this->startingPointFilePath;
}

/*********************************************************************************************
*	function: readFrom()
*
*	description:
*		--> public method for reading from a certain file
*	parameter:
*		--> filepath ... location of the file which is used for reading
*	return value:
*		--> returnVal ... data which has been read
*
*********************************************************************************************/
// buffer size has to be considered when reading from files with big data
std::string FileHandler::readFrom(const std::string& filepath)
{
    std::fstream file = this->openFile(filepath, "r");
    char buffer[255];

    // read
    file.getline(buffer, sizeof(buffer));

    if(buffer == nullptr)
    {
        std::cerr << "[ERROR]: in function readFrom: couldn't read from file" << std::endl;
        return (std::string) nullptr;
    }

    if(!this->closeFile(file))
    {
        std::cerr << "[ERROR]: in function readFrom: couldn't close file" << std::endl;
        return (std::string) nullptr;
    }

    return (std::string) buffer;

}

/*********************************************************************************************
*	function: writeInto()
*
*	description:
*		--> public method for writing into a certain file
*	parameter:
*		--> filepath ... location of the file which is used for writing
*		--> data ... string which is going to be written into the file
*		--> tuncate ... decides whether the current content of the file should be
*		                overwritten(true) or not (false)
*	return value:
*		--> returnVal ... information if everything worked fine(true) or not(false)
*
*********************************************************************************************/
bool FileHandler::writeInto(const std::string& filepath,
                            const std::string& data,
                            const bool truncate)
{
    std::fstream file;

    if(truncate)
        file  = this->openFile(filepath, "wd");
    else
        file = this->openFile(filepath, "w");

    // write
    file << data << std::endl;

    if(file.fail())
    {
        std::cerr << "[ERROR]: in function writeTo: couldn't write to file" << std::endl;
        return false;
    }

    if(!this->closeFile(file))
    {
        std::cerr << "[ERROR]: in function writeTo: couldn't close file" << std::endl;
        return false;
    }

    return true;
}

/*********************************************************************************************
*	function: createLogFile()
*
*	description:
*		--> public method for creating a log-file
*	parameter:
*		--> filename ... name of log-file that should be created
*	return value:
*		--> returnVal ... file path of created file
*
*********************************************************************************************/
std::string FileHandler::createLogFile(const std::string& filename)
{
    std::string path = this->getLogFolderPath() + "/" + filename;
    return this->openFile(path, "w") ? path : "";
}

/*********************************************************************************************
*	function: getShutDownString()
*
*	description:
*		--> public method for creating a string with information about the shutdown matters
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... copy of created string
*
*********************************************************************************************/
std::string FileHandler::getShutDownString()
{
    std::ostringstream dateTime;

    // initialize struct time_t with current time
    std::time_t t = std::time(0);
    std::tm* time_ptr = std::localtime(&t);

    dateTime << (time_ptr->tm_year + 1900)
             << "-" << (time_ptr->tm_mon + 1)
             << "-" << time_ptr->tm_mday
             << "-" << time_ptr->tm_hour
             << ":" << time_ptr->tm_min
             << ":" << time_ptr->tm_sec;

    return dateTime.str();
}