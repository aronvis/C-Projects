#include "Movie.h"

Movie::Movie(std::vector<std::string>& genres, std::string& ID, int runTime, std::string& title, std::string url, int year): m_Genres(genres),m_ID(ID), m_RunTime(runTime), m_Title(title), m_URL(url), m_Year(year){}
