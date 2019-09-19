#include "SrcMain.h"
#include <string>
#include "RleFile.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	if(argc > 1)
    {
        std::string fileName = argv[1];
        std::string extension = fileName.substr(fileName.length()-4);
        RleFile * archive = new RleFile();
        if(extension == ".rl1")
        {
            archive->ExtractArchive(fileName);
        }
        else
        {
            archive->CreateArchive(fileName);
        }
        delete archive;
    }
}
