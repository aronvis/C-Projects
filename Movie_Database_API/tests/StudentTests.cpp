#include "catch.hpp"
#include "SrcMain.h"
#include <string>
#include <chrono>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// If you feel like adding tests, go for it...
TEST_CASE("Student tests", "[student]")
{
    const char* argv[]={
        "Input/title.basics.tsv"
    };
    ProcessCommandArgs(1, argv);
    REQUIRE(true);
}
