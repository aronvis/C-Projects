#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

void RleFile::CreateArchive(const std::string& source)
{
    // Reads the file in binary mode
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[static_cast<unsigned int>(size)];
        file.seekg(0, std::ios::beg);
        file.read(memblock, size);
        file.close();
        // Compresses the contents of the file
        mRleData.Compress(memblock, static_cast<size_t>(size));
        delete[] memblock;
        // Save data items to file header
        mHeader.mFileName = source;
        mHeader.mFileNameLength = mHeader.mFileName.length();
        mHeader.mFileSize = size;
        // Opens a file for writing in binary mode
        std::ofstream arc(mHeader.mFileName + ".rl1", std::ios::out|std::ios::binary|std::ios::trunc);
        if(arc.is_open())
        {
            arc.write(mHeader.mSig, 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)), 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.mFileNameLength)),1);
            arc.write(mHeader.mFileName.c_str(),mHeader.mFileNameLength);
            arc.write(mRleData.mData, mRleData.mSize);
            arc.close();
        }
    }
    else
    {
        std::cout << source << " was not found." << std::endl;
    }
}

void RleFile::ExtractArchive(const std::string& source)
{
    // Reads the file in binary mode
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[static_cast<unsigned int>(size)];
        file.seekg(0, std::ios::beg);
        file.read(memblock, size);
        file.close();
        if(memblock[0] != '\0')
        {
            mHeader.mFileSize = *(reinterpret_cast<int*>(&memblock[4]));
            mHeader.mFileNameLength = *(reinterpret_cast<char*>(&memblock[8]));
            int fileNameLength = static_cast<unsigned int>(mHeader.mFileNameLength);
            mHeader.mFileName = source.substr(0,fileNameLength);
            //Decompresses the contents of the file
            size_t inputSize = static_cast<size_t>(size) - 9 - fileNameLength;
            mRleData.Decompress(&memblock[9+fileNameLength],inputSize, static_cast<size_t>(mHeader.mFileSize));
            delete[] memblock;
            std::ofstream arc(mHeader.mFileName, std::ios::out|std::ios::binary|std::ios::trunc);
            if(arc.is_open())
            {
                arc.write(mRleData.mData, mHeader.mFileSize);
                arc.close();
            }
        }
        else
        {
            std::cout << source << " was empty" << std::endl;
        }
    }
    else
    {
        std::cout << source << " was not found." << std::endl;
    }
}
