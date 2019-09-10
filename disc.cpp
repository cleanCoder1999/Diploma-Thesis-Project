/*********************************************************************************************
*	project-name: 	_1_MMP-CrackTest
*	file: 			disc.cpp
*
*	copyright: Lucas Müllner
*	date of creation: 11.12.2018
*	description:
*		--> source file of class Disc
*
*********************************************************************************************/

#include "disc.h"

/*********************************************************************************************
*	function: Disc()
*
*	description:
*		--> constructor which instantiates worker and machine
*	parameter:
*		--> worker ... name of operator whose at the MMP-CrackTest
*		--> machine ... id of machine where the discs come from
*	return value:
*		--> n. a.
*
*********************************************************************************************/
Disc::Disc(const std::string& worker, const std::string& machine)
        : worker(worker), machine(machine)
{}

/*********************************************************************************************
*	function: setFNom()
*
*	description:
*		--> private setter method of member fNom
*	parameter:
*		--> fnom ... nominal force value (disc should withstand this force value)
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Disc::setFNom(float fnom)
{
    this->fNom = fnom;
}

/*********************************************************************************************
*	function: setFMax()
*
*	description:
*		--> private setter method of member fMax
*	parameter:
*		--> fmax ... maximum force value (disc shouldn't be able to withstand this force value)
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Disc::setFMax(float fmax)
{
    this->fMax = fmax;
}

/*********************************************************************************************
*	function: setDateTime()
*
*	description:
*		--> private setter method of member dateTime
*		    (loads current time and date into variable dateTime)
*	parameter:
*		--> n.a.
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Disc::setDateTime()
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

    this->dateTime = dateTime.str();

}

/*********************************************************************************************
*	function: gatherForceValues()
*
*	description:
*		--> private method for gathering force values from files (cd.txt, dvd.txt, bd.txt)
*		    (member fNom and fMax are internally set via private setters)
*	parameter:
*		--> n.a.
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Disc::gatherForceValues()
{
    int i;
    FileHandler handler;
    std::string buf;
    std::string subStringFnom, subStringFmax;

    // discTypes must consist of capital letters
    if(this->discType == "CD")
        buf = handler.readFrom(handler.getCDFilePath());
    else if(this->discType == "DVD")
        buf = handler.readFrom(handler.getDVDFilePath());
    else
        buf = handler.readFrom(handler.getBDFilePath());

    // find location of delimiter
    for(i = 0; i < buf.size(); ++i)
        if(buf[i] == ' ')
            break;

    if(i >= buf.size())
        i = static_cast<int>(buf.size())-1;

    if((i+1) == buf.size())
    {
        std::cerr << "[ERROR]: in function gatherForceValues: invalid file format" << std::endl;
        return false;
    }

    // create sub strings
    subStringFnom = buf.substr(0, static_cast<unsigned long>(i));
    subStringFmax = buf.substr(static_cast<unsigned long>(i+1), buf.size());

    // convert string to float
    this->setFNom(stof(subStringFnom));
    this->setFMax(stof(subStringFmax));

    return true;

}


/*********************************************************************************************
*	function: setFNom()
*
*	description:
*		--> public setter method of member discType
*		    (internal call of private method gatherForceValues())
*	parameter:
*		--> discType ... type of disc (CD, DVD, BD)
*	return value:
*		--> returnVal ... information whether everything worked fine (true) or not (false)
*
*********************************************************************************************/
bool Disc::setDiscType(const std::string& discType)
{
    if(discType != "CD" && discType != "DVD" && discType != "BD")
    {
        std::cerr << "[ERROR]: in function setDiscType: invalid discType" << std::endl;
        return false;
    }

    this->discType = discType;

    // get force values from files
    return this->gatherForceValues();
}

/*********************************************************************************************
*	function: setTestType()
*
*	description:
*		--> public setter method of member testType
*		    (internal call of private setter method setTimeDate())
*	parameter:
*		--> testType ... chosen type of test (stability, break, manual)
*	return value:
*		--> n.a.
*
*********************************************************************************************/
void Disc::setTestType(const std::string& testType)
{
    this->setDateTime();
    this->testType = testType;
}

/*********************************************************************************************
*	function: getWorker()
*
*	description:
*		--> getter method of member worker
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member worker
*
*********************************************************************************************/
std::string Disc::getWorker() const
{
    return this->worker;
}

/*********************************************************************************************
*	function: getMachine()
*
*	description:
*		--> getter method of member machine
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member machine
*
*********************************************************************************************/
std::string Disc::getMachine() const
{
    return this->machine;
}

/*********************************************************************************************
*	function: getDiscType()
*
*	description:
*		--> getter method of member discType
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member discType
*
*********************************************************************************************/
std::string Disc::getDiscType() const
{
    return this->discType;
}

/*********************************************************************************************
*	function: getFNom()
*
*	description:
*		--> getter method of member fNom
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member fNom
*
*********************************************************************************************/
float Disc::getFNom() const
{
    return this->fNom;
}

/*********************************************************************************************
*	function: getFMax()
*
*	description:
*		--> getter method of member fMax
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member fMax
*
*********************************************************************************************/
float Disc::getFMax() const
{
    return this->fMax;
}

/*********************************************************************************************
*	function: getTestType()
*
*	description:
*		--> getter method of member testType
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member testType
*
*********************************************************************************************/
std::string Disc::getTestType() const
{
    return this->testType;
}

/*********************************************************************************************
*	function: getDateTime()
*
*	description:
*		--> getter method of member dateTime
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... current value of member dateTime
*
*********************************************************************************************/
std::string Disc::getDateTime() const
{
    return this->dateTime;
}

/*********************************************************************************************
*	function: createFilename()
*
*	description:
*		--> public method for creating a filename containing the current date & time,
*		    name of worker at the MMP-CrackTest and the machine id which produced the discs
*	parameter:
*		--> n. a.
*	return value:
*		--> returnVal ... created filename
*
*********************************************************************************************/
std::string Disc::createFilename()
{
    std::ostringstream os;

    if(this->dateTime.empty())
    {
        std::cerr << "[ERROR]: in function createFilename: member timeDate is empty" << std::endl;
        return "";
    }

    os << this->dateTime << "_" << this->machine << "_" << this->worker << ".log";

    return os.str();
}