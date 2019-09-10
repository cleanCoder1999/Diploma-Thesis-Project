
#include "socket.h"

int main()
{
    Socket sock;

    FileHandler handler;
    std::ostringstream shutDown;
    bool cond = true;

    if(!sock.run())//cond))
    {
        std::cerr << "[ERROR]: in function main: error while running run() (Socket::run())"
                  << std::endl;
    }

    shutDown << "[INFO]: logged shut down on "
                << handler.getShutDownString()
                << std::endl;

    handler.writeInto(handler.getShutDownLogFilePath(), shutDown.str(), false);
    std::cout << "shutting down..." << std::endl;

    return EXIT_SUCCESS;
}



// ***********************************
//  2304 lines (29.12.2018)
//  --> tests aren't implemented yet
//      (stability, break, manual)
// ************************************/
