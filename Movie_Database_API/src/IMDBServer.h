#pragma once
#include "Movie.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <cpprest/http_listener.h>
using namespace web;
using namespace web::http;

class IMDBServer
{
public:
    // Constructors
    IMDBServer(std::string& fileName);
    IMDBServer(const IMDBServer & server) = default;
    IMDBServer& operator= (const IMDBServer & server) = default;
    IMDBServer(IMDBServer && server) = default;
    IMDBServer& operator= (IMDBServer && server) = default;
    ~IMDBServer();
    // Rest server functionality
    void InitServer();
    void Handle_Get(http_request request);
    void NotFound(http_request& request, std::string& action);
    void FoundID(http_request& request, std::string& movieID);
    void FoundName(http_request& request, std::string& movieName);
    std::string FormatName(std::string& nameUri);
    // Parsing functions
    bool ParseFile(std::string& fileName);
    int StringToInt(std::string& content);
    std::string GetURL(std::string& ID);
    std::vector<std::string> Split(std::string content, char delim);
    void InsertMovie(std::string& ID, Movie* movie);
    void InsertID(std::string& movieName, std::string& movieID);
    bool IDExists(std::string& movieID);
    bool NameExists(std::string& movieName);
    // Getter functions used to get data for JSON File
    Movie* GetMovie(std::string& movieID);
    std::vector<std::string>* GetMovieID(std::string& movieName);
private:
    // Maps unique movie id -> movie*
    std::unordered_map<std::string, Movie*> mMovie;
    // Maps non-unique movie title -> vector of movie IDs
    std::unordered_map<std::string, std::vector<std::string>> mMovieID;
};
