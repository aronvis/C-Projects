#include "catch.hpp"
#include "RleData.h"
#include "RleFile.h"
#include "SrcMain.h"

// Helper function declarations (don't change these)
extern bool BuffersAreSame(const char* expected, const char* actual, size_t size);
extern bool RunCompressionTest(char* test, size_t testSize,
							   char* expected, size_t expectedSize);
extern bool RunDecompressionTest(char* test, size_t testSize,
								 char* expected, size_t expectedSize);
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);

TEST_CASE("RLE Compression", "[student]")
{
    // Test 1 - Passed
    SECTION("Basic positive run")
    {
        char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
        "ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
        char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
        "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
        "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
        "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
        "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
        "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
        "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
        "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";

        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 2
    SECTION("Empty space run")
    {
        char test[] = "     \n      ";
        char expected[] = "\x05" " " "\x01" "\n" "\x06" " ";
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 3
    SECTION("Single Run")
    {
        char test[] = "c";
        char expected[] = "\x01" "c";
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 4
    SECTION("Double Single Run")
    {
        char test[] = "abcabc";
        char expected[] = "\xFA" "a" "b" "c" "a" "b" "c";
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 5
    SECTION("Negative, positive, negative Run")
    {
        char test[] = "abccba";
        char expected[] = "\xFE" "a" "b" "\x02" "c" "\xFE" "b" "a";
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 6
    SECTION("Capital in single character run")
    {
        char test[] = "Aabc";
        char expected[] = "\xFC" "A" "a" "b" "c";
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 7
    SECTION("Single run with space")
    {
        char test[] = "AbB Bc";
        char expected[] = "\xFA" "A" "b" "B" " " "B" "c";
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 8
    SECTION("Odd character run")
    {
        char test[] = "/><~!@#$%^&*()_+{}|[]";
        char expected[] = "\xEB" "/" ">" "<" "~" "!" "@" "#" "$" "%" "^" "&" "*" "(" ")" "_" "+" "{" "}" "|" "[" "]";
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 9
    SECTION("Massive single Run")
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        char expected[] = "\x7F" "a" "\x21" "a";
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 10
    SECTION("Two large negative runs followed by positive run")
    {
        char test[171];
        for(int i = 0; i <= 169; i++)
        {
            if(i >= 160)
            {
                test[i] = 'a';
            }
            else
            {
                test[i] = i;
            }
        }
        char expected[165];
        int testIndex = 0;
        for(int i = 0; i <= 163; i++)
        {
            if(i == 0)
            {
                expected[i] = -127;
            }
            else if(((i >= 1) && (i <=127)) || ((i >= 129) && (i<= 161)) || (i == 163))
            {
                expected[i] = test[testIndex];
                testIndex++;
            }
            else if(i == 128)
            {
                expected[i] = -33;
            }
            else
            {
                expected[i] = 10;
            }
        }
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
}

TEST_CASE("RLE Decompression", "[student]")
{
    // Test 1
    SECTION("Basic positive run")
    {
        char test[] = "\x28" "x";
        char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 2
    SECTION("Basic negative run")
    {
        char test[] = "\xFD" "a" "b" "c" "\xFD" "a" "b" "c";
        char expected[] = "abcabc";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 3
    SECTION("Positive and negative run")
    {
        char test[] = "\xFE" "a" "b" "\x02" "c" "\xFE" "b" "a";
        char expected[] = "abccba";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    // Test 4
    SECTION("run")
    {
        char test[] = "\x05" " " "\x01" "\n" "\x06" " ";
        char expected[] = "     \n      ";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
}

TEST_CASE("Command line arguments", "[student]")
{
    SECTION("instructions.txt.rl1")
    {
        const char* argv[] = {
            "tests/tests",
            "data/instructions.txt.rl1"
        };
        ProcessCommandArgs(2, argv);
        bool result = CheckFileMD5("data/instructions.txt", "5e042d2e4bf014e714dd7cfc4dc25aab");
        REQUIRE(result);
    }
}

TEST_CASE("File compression", "[student]")
{
    RleFile r;
    SECTION("rle.bmp")
    {
        r.CreateArchive("data/rle.bmp");
        bool result = CheckFileMD5("data/rle.bmp.rl1", "f2a9d8425d53c664e45d9eb1b53137b9");
        REQUIRE(result);
    }
    SECTION("pic.jpg")
    {
        r.CreateArchive("data/pic.jpg");
        bool result = CheckFileMD5("data/pic.jpg.rl1", "0bbf2a5109b30d79939d2061ea8c74aa");
        REQUIRE(result);
    }
    SECTION("Conquest.ogg")
    {
        r.CreateArchive("data/Conquest.ogg");
        bool result = CheckFileMD5("data/Conquest.ogg.rl1", "ec29ff368ec5100bfba22635ddc5ba5c");
        REQUIRE(result);
    }
}

TEST_CASE("File decompression", "[student]")
{
    RleFile r;
    SECTION("works.bmp.rl1")
    {
        r.ExtractArchive("data/works.bmp.rl1");
        bool result = CheckFileMD5("data/works.bmp", "8baad647acefae2f8c36ee111582a875");
        REQUIRE(result);
    }
    SECTION("xkcd.bmp.rl1")
    {
        r.ExtractArchive("data/xkcd.bmp.rl1");
        bool result = CheckFileMD5("data/xkcd.bmp", "a4d7efa89d47a94a76f8050dd8d60cd0");
        REQUIRE(result);
    }
    SECTION("logo.png.rl1")
    {
        r.ExtractArchive("data/logo.png.rl1");
        bool result = CheckFileMD5("data/logo.png", "95403309460632f60efa647ef59b78fc");
        REQUIRE(result);
    }
}

