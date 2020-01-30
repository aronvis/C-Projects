#include "SrcMain.h"
#include "IMDBServer.h"
#include <memory>
#include <string>

void ProcessCommandArgs(int argc, const char* argv[])
{
    if(argc >= 2)
    {
        std::string fileName = argv[1];
        std::unique_ptr<IMDBServer> server(new IMDBServer(fileName));
    }
}


