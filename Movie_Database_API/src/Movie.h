#pragma once
#include <vector>
#include <string>

struct Movie
{
    Movie(std::vector<std::string>& genres, std::string& ID, int runTime, std::string& title, std::string url, int year);
    std::vector<std::string> m_Genres;
    std::string m_ID;
    int m_RunTime;
    std::string m_Title;
    std::string m_URL;
    int m_Year;
};
