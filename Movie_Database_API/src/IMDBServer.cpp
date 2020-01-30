#include <string>
#include <iostream>
#include "fstream"
#include "utility"
#include "IMDBServer.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
using namespace web::http::experimental::listener;

// Default constructor for IMDB server class
IMDBServer::IMDBServer(std::string& fileName)
{
    bool result = ParseFile(fileName);
    if(result)
    {
        std::cout << "File parsing is completed." << std::endl;
        InitServer();
    }
}

// Destructor for IMDB server that deallocates all dynamic memory
IMDBServer::~IMDBServer()
{
    for(auto movieItem: mMovie)
    {
        delete movieItem.second;
    }
}

//Initiates the server program
void IMDBServer::InitServer()
{
    http_listener listener("http://localhost:12345");
    listener.support(methods::GET, std::bind(&IMDBServer::Handle_Get, this, std::placeholders::_1));
    try
    {
        listener.open().wait();
        std::cout << "Starting server..." << std::endl;
        while(true){};
    }
    catch (std::exception const & e)
    {
        std::cout << e.what() << std::endl;
    }
}

// Handles the get request and calls the correct return function
void IMDBServer::Handle_Get(http_request request)
{
    std::string uriPath = request.relative_uri().path().substr(1);
    std::vector<std::string> items = Split(uriPath, '/');
    std::string action = items[1];
    std::string paramater = items[2];
    if(action == "id")
    {
        if(IDExists(paramater))
        {
            FoundID(request,paramater);
        }
        else
        {
            NotFound(request, action);
        }
    }
    else
    {
        paramater = FormatName(paramater);
        if(NameExists(paramater))
        {
            FoundName(request, paramater);
        }
        else
        {
            NotFound(request, action);
        }
    }
}

// Reformats the name uri to a regular name string
std::string IMDBServer::FormatName(std::string& nameUri)
{
    std::string movieName;
    std::string nextWord;
    std::vector<std::string> words = Split(nameUri, '%');
    for(int i=0; i<words.size(); i++)
    {
        if(i >= 1)
        {
            words[i] = words[i].substr(1);
            words[i][0] = ' ';
        }
        nextWord = words[i];
        movieName += nextWord;
    }
    return movieName;
}

// Return file not found error code 404 JSON File
void IMDBServer::NotFound(http_request& request, std::string& action)
{
    std::string outputString = "No movie with that ID found";
    if(action == "name")
    {
        outputString = "No movie by that name found";
    }
    json::value response;
    response["Error"] = json::value::string(outputString);
    request.reply(status_codes::NotFound, response);
}

// Return correct movie JSON file based on movie ID
void IMDBServer::FoundID(http_request& request, std::string& movieID)
{
    Movie * foundMovie = GetMovie(movieID);
    json::value response;
    json::value arr;
    std::vector<std::string> genres = foundMovie->m_Genres;
    for(int i =0; i<genres.size(); i++)
    {
        arr[i] = json::value::string(genres[i]);
    }
    response["Genres"] = arr;
    response["ID"] = json::value::string(foundMovie->m_ID);
    response["Runtime"] = json::value::number(foundMovie->m_RunTime);
    response["Title"] = json::value::string(foundMovie->m_Title);
    response["URL"] = json::value::string(foundMovie->m_URL);
    response["Year"] = json::value::number(foundMovie->m_Year);
    request.reply( status_codes::OK, response);
}

// Return correct movie JSON file based on movie name
void IMDBServer::FoundName(http_request& request, std::string& movieName)
{
    std::vector<std::string> movieIDs = mMovieID[movieName];
    json::value response;
    for(int i =0; i<movieIDs.size(); i++)
    {
        Movie * foundMovie = GetMovie(movieIDs[i]);
        json::value responseItem;
        json::value arr;
        std::vector<std::string> genres = foundMovie->m_Genres;
        for(int i =0; i<genres.size(); i++)
        {
            arr[i] = json::value::string(genres[i]);
        }
        responseItem["Genres"] = arr;
        responseItem["ID"] = json::value::string(foundMovie->m_ID);
        responseItem["Runtime"] = json::value::number(foundMovie->m_RunTime);
        responseItem["Title"] = json::value::string(foundMovie->m_Title);
        responseItem["URL"] = json::value::string(foundMovie->m_URL);
        responseItem["Year"] = json::value::number(foundMovie->m_Year);
        response[i] = responseItem;
    }
    request.reply( status_codes::OK, response);
}


// Parses the file and returns true if the parsing was successful
bool IMDBServer::ParseFile(std::string& fileName)
{
    std::ifstream inputFile(fileName);
    if(inputFile.is_open())
    {
        std::string uselessItem;
        // The first line is not needed
        std::getline(inputFile, uselessItem);
        std::string movieID;
        std::string titleType;
        std::string primaryTitle;
        int startYear;
        int runTime;
        std::vector<std::string> genres;
        std::string url;
        std::string startYearString;
        std::string runTimeString;
        std::string genreString;
        while(!inputFile.eof())
        {
            std::getline(inputFile, movieID, '\t');
            std::getline(inputFile, titleType, '\t');
            // Skip Storage of data row
            if((IDExists(movieID)) || (titleType != "movie"))
            {
                getline(inputFile, uselessItem);
                continue;
            }
            std::getline(inputFile, primaryTitle, '\t');
            std::getline(inputFile, uselessItem, '\t');
            std::getline(inputFile, uselessItem, '\t');
            std::getline(inputFile, startYearString, '\t');
            std::getline(inputFile, uselessItem, '\t');
            std::getline(inputFile, runTimeString, '\t');
            std::getline(inputFile, genreString);
            // Reformats the strings in file into the correct format
            startYear = StringToInt(startYearString);
            runTime = StringToInt(runTimeString);
            genres = Split(genreString, ',');
            url = GetURL(movieID);
            Movie * movie= new Movie(genres, movieID, runTime, primaryTitle, url, startYear);
            InsertMovie(movieID, movie);
            InsertID(primaryTitle, movieID);
        }
        return true;
    }
    std::cout << fileName << " was not found." << std::endl;
    return false;
}

// Converts any string value inside the
int IMDBServer::StringToInt(std::string& content)
{
    int output = 0;
    if(content[1] != 'N')
    {
        output = stoi(content);
    }
    return output;
}

// Returns the URL based on the movieID of the movie
std::string IMDBServer::GetURL(std::string& movieID)
{
    std::string url = "https://www.imdb.com/title/" + movieID + "/";
    return url;
}

// Returns a vector of genres based in the genres string
std::vector<std::string> IMDBServer::Split(std::string content, char delim)
{
    int numChars = content.size();
    std::vector<std::string> itemList;
    std::string item;
    int delimIndex = -1;
    int stringLength = 0;
    for(int i=0; i<=numChars; i++)
    {
        if((content[i] == delim) || (i == numChars))
        {
            stringLength = i - delimIndex - 1;
            if(i == numChars)
            {
                stringLength++;
            }
            item = content.substr(delimIndex+1, stringLength);
            itemList.emplace_back(item);
            delimIndex = i;
        }
    }
    return itemList;
}

// Inserts a movie movieID as key and the movie class itself as value into the mMovie hashmap
void IMDBServer::InsertMovie(std::string& movieID, Movie* movie)
{
    mMovie[movieID] = movie;
}

// Inserts a movie movieID as key and the movie class itself as value into the mMovie hashmap
void IMDBServer::InsertID(std::string& movieName, std::string& movieID)
{
    if(mMovieID.find(movieName) == mMovieID.end())
    {
        std::vector<std::string> movieIDS;
        movieIDS.emplace_back(movieID);
        mMovieID[movieName] = movieIDS;
    }
    else
    {
        mMovieID[movieName].emplace_back(movieID);
    }
}

// Returns true if the movie movieID already exists inside our database
bool IMDBServer::IDExists(std::string& movieID)
{
    bool result = mMovie.find(movieID) != mMovie.end();
    return result;
}

// Returns true if the movie name already exists inside our database
bool IMDBServer::NameExists(std::string& movieName)
{
    bool result = mMovieID.find(movieName) != mMovieID.end();
    return result;
}

// Returns the movie if its movie movieID exits
Movie * IMDBServer::GetMovie(std::string &movieID)
{
    if(!IDExists(movieID))
    {
        return nullptr;
    }
    return mMovie[movieID];
}

// Returns a vector of movie IDs for a given movie title
std::vector<std::string>* IMDBServer::GetMovieID(std::string &movieName)
{
    if(!NameExists(movieName))
    {
        return nullptr;
    }
    return &mMovieID[movieName];
}
