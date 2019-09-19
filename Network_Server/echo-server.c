#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <cstring>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <thread>
#include <iostream>
#include <utility>
#include <mutex>
#include <unordered_map>
#include <fstream>
#include <sys/time.h>
#include <time.h>
#include <openssl/md5.h>

void get_timestamp(char timestamp_buf[32])
{
    struct timeval now;
    char time_buf[26];
    int i;

    gettimeofday(&now, NULL);
    strcpy(time_buf, ctime(&now.tv_sec));
    for (i=0; i < 11; i++) {
        timestamp_buf[i] = time_buf[i];
    }
    timestamp_buf[11] = time_buf[20];
    timestamp_buf[12] = time_buf[21];
    timestamp_buf[13] = time_buf[22];
    timestamp_buf[14] = time_buf[23];
    for (i=15; i < 24; i++) {
        timestamp_buf[i] = time_buf[i-5];
    }
    sprintf(&timestamp_buf[24], ".%06d", (int)now.tv_usec);
}

struct connectionObj 
{
    connectionObj(int connectNum, int socket_fd, std::thread * threadptr, double kbSent): connectNum(connectNum), 
    socket_fd(socket_fd), threadptr(threadptr), kbSent(kbSent) {};
    int connectNum;
    int socket_fd;
    std::thread * threadptr;
    double kbSent;
    std::string IPaddress;
    std::string port;
    std::string path;
    int fileSize;
};

// struct bucketFilter
// {

// };

struct logfileObj
{
    logfileObj(std::unordered_map<std::string,std::string> *startup): fileOutput((*startup)["logfile"])
    {
        char timestamp_buf[32];
        get_timestamp(timestamp_buf);
        fileOutput << "Server started at [" << timestamp_buf << "]\nServer listening on port "; 
        fileOutput << (*startup)["port"] << "\nServer root at \'" << (*startup)["rootdir"]<< "\'\n";
        fileOutput.flush();
    }
    void insert(std::string line)
    {
        if(fileOutput.is_open())
        {
            fileOutput << line;
            fileOutput.flush();
        }
        else
        {
            std::cerr << "Log file has been closed prematurely" << std::endl;
            exit(-1);
        }
    }
    ~logfileObj()
    {
        fileOutput.close();
    }
    std::ofstream fileOutput;
};

struct serializationBox
{
    serializationBox(int curr_connection_number, bool finished, std::unordered_map<std::string,std::string> *startup): 
    finished(finished), next_connection_number(curr_connection_number), logfile(new logfileObj(startup)) {};
    std::mutex m_;
    std::unordered_map<int,connectionObj*> clients;
    bool finished;
    int next_connection_number;
    logfileObj *logfile;
    ~serializationBox()
    {
        delete logfile;
    }
    // std::unordered_map<std::string,bucketFilter*> buckets;
};

void createPID(std::string fileName)
{
    std::ofstream fileOutput(fileName);
    if(fileOutput.is_open())
    {
        int PIDValue = (int) getpid();
        fileOutput << PIDValue << '\n';
        fileOutput.close();
    }
    else
    {
        std::cerr << "PID file was not opened successfully" << std::endl;
        exit(-1);
    }
}

/**
 * Use this code to create a master socket to be used by a server.
 *
 * You should be able to use this function as it.
 *
 * @param port_number_str - port number of the well-known/welcome port.
 * @param debug - non-zero means print to stderr where the server is listening.
 */
static
int create_master_socket(const char *port_number_str) 
{
    int socket_fd = (-1);
    int reuse_addr = 1;
    struct addrinfo hints; 
    struct addrinfo* res = NULL;
    signal(SIGPIPE, SIG_IGN);
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_NUMERICSERV|AI_ADDRCONFIG;
    getaddrinfo("localhost", port_number_str, &hints, &res);
    socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == (-1)) {
        perror("socket() system call");
        exit(-1);
    }
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (void*)(&reuse_addr), sizeof(int));
    if(bind(socket_fd, res->ai_addr, res->ai_addrlen) == (-1)) 
    {
        perror("bind() system call");
        exit(-1);
    }
    listen(socket_fd, 2);
    return socket_fd;
}

/**
 * Call accept() on the master socket to wait for a clients to connect.
 * If no clients connects, the function would block indefinitely
 *         (unless an error occurs).
 * If a clients connects, this function would create a new socket file descriptor
 *         for communicating with the clients.
 *
 * You should be able to use this function as it.
 * 
 * The accept function will return -1 with errno == EINTR if accept was enable
 *      to make a connection while it should be making a connection
 *
 * The accept function will return -1 without errno == EINTR if the master socket
 *      if the master socket is closed and shutdown, which will exit the function
 *      and return -1
 *
 * @param master_socket_fd - master socket created by create_master_socket().
 */
static
int my_accept(const int master_socket_fd)
{
    int newsockfd = (-1);
    struct sockaddr_in cli_addr;
    unsigned int clilen = sizeof(cli_addr);
    while (newsockfd == (-1)) 
    {
        newsockfd = accept(master_socket_fd, (struct sockaddr *)(&cli_addr), &clilen);
        if (newsockfd == (-1) && errno == EINTR) continue;
        if (newsockfd == (-1)) break;
    }
    return newsockfd;
}

static
void usage()
{
    fprintf(stderr, "./Configuration file\n");
    exit(-1);
}

static
int read_a_line(int socket_fd, char *buf, int buf_sz)
{
    int lineLength = 0;
    int bytes_received = read(socket_fd, buf, 1);
    if(bytes_received > 0)
    {
        lineLength++;
        while((buf[lineLength-1] != '\n') && (lineLength <=buf_sz-1) && (bytes_received == 1))
        {
            bytes_received = read(socket_fd, buf+lineLength, 1);
            if(bytes_received > 0)
                lineLength++;
        } 
        buf[lineLength] = '\0';
    }
    return lineLength;
}

static
int get_file_size(const char *path) 
{
    struct stat stat_buf;
    if(stat(path, &stat_buf) != 0) 
    {
        return (-1); 
    }
    return (int)(stat_buf.st_size); 
}

static
void HexDump(unsigned char *buf, int len, std::string &md5value)
{
    static char hexchar[]="0123456789abcdef";
    for (int i=0; i < len; i++) 
    {
        unsigned char ch=buf[i];
        int hi_nibble=(int)(unsigned int)((ch>>4)&0x0f);
        int lo_nibble=(int)(unsigned int)(ch&0x0f);
        md5value += hexchar[hi_nibble];
        md5value += hexchar[lo_nibble];
    }
}

static
void md5_calc(std::string &md5value, std::string url)
{
    int fd = (-1), bytes_read = 0;
    char buf[1];
    unsigned char md5_buf[MD5_DIGEST_LENGTH];
    MD5_CTX md5_ctx;
    if ((fd=open(url.c_str(),O_RDONLY)) == (-1)) 
    {
        fprintf(stderr, "Cannot open '%s' for reading.\n", url.c_str());
        exit(-1);
    }
    MD5_Init(&md5_ctx);
    while ((bytes_read=(int)read(fd, buf, sizeof(buf))) > 0) 
    {
        MD5_Update(&md5_ctx, buf, bytes_read);
    }
    MD5_Final(md5_buf, &md5_ctx);
    HexDump(md5_buf, sizeof(md5_buf), md5value);
    close(fd);
}

static
void parseRequestline(std::string requestLine, std::string &response, int &fileSize, std::string &url, int &responseCode, std::string &md5value)
{
    std::stringstream ss(requestLine);
    std::string method;
    std::string temp;
    std::string version;
    ss >> method >> temp >> version;
    if(temp[0] != '/')
    {
        url += '/' + temp;
    }
    else
        url += temp;
    fileSize = get_file_size(url.c_str());
    if(method != "GET")
    {
        responseCode = 405;
        fileSize = 72;
        response = version + " 405 Method Not Allowed\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: 72\r\n"  
                            "\r\n<html><head></head><body><h1>405 Method Not Allowed</h1></body></html>\r\n";
    }
    else if(version.substr(0,7) !="HTTP/1.")
    {
        responseCode = 400;
        fileSize = 65;
        response = version + " 400 Bad Request\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: 65\r\n" 
                            "\r\nhtml><head></head><body><h1>400 Bad Request</h1></body></html>\r\n";
    }
    else if(fileSize == -1)
    {
        responseCode = 404;
        fileSize = 63;
        response = version + " 404 Not Found\r\nConnection: close\r\nServer: pa4 (vischjag@usc.edu)\r\nContent-Type: text/html\r\nContent-Length: 63\r\n" 
                            "\r\n<html><head></head><body><h1>404 Not Found</h1></body></html>\r\n";
    }
    else
    {
        responseCode = 200;
        md5_calc(md5value, url);
        response = version + " 200 OK\r\nConnection: close\r\nServer: pa4 (vischjag@usc.edu)\r\nContent-Type: application/octet-stream\r\nContent-Length: " + 
        std::to_string(fileSize) + "\r\nContent-MD5: " + md5value + "\r\n\r\n";
    }
}

/*
 * Should always call beter_write() instead of write()!
 * Return 0 if successful.
 * Return -1 if there is an error.
 *                     
 */
int better_write(int fd, char *buf, int bytes_to_write)
{
    while (bytes_to_write > 0) {
        int bytes_written = write(fd, buf, bytes_to_write);

        if (bytes_written > 0) {
            bytes_to_write -= bytes_written;
            buf += bytes_written;
        } else if (bytes_written == (-1)) {
            if (errno == EINTR) {
                continue;
            }
            return (-1);
        }
    }
    return 0;
}

/**
 * This is the function you need to change to change the behavior of your server!
 *
 * After every 1000 bits other threads can be run
 * The connection ends when:
 * 1) The user kills the socket (socket_fd = -1)
 * 2) All the data has been send (socket_fd = -2)
 * 3) The user quits the program (socket_fd = -1)
 *
 * @param newsockfd - socket that can be used to "talk" (i.e., read/write) to the clients.
 */
static
void talk_to_clients(int connectionNumber, serializationBox *databox, std::unordered_map<std::string,std::string> *startup)
{
    struct sockaddr_in cs_addr;
    socklen_t cs_len = (socklen_t)sizeof(cs_addr);
    int bytes_received = 0;
    char buf[1000];
    std::vector<std::string> httpRequest;
    std::string requestLine;
    std::string connectionHeader;
    do
    {
        databox->m_.lock();
        bytes_received = read_a_line(databox->clients[connectionNumber]->socket_fd, buf, sizeof(buf)); 
        databox->m_.unlock();
        if(bytes_received > 2)
        {
            requestLine = buf;
            httpRequest.push_back(requestLine);
            memset(buf,0,sizeof(buf));
        }
    }
    while((bytes_received > 2) && (buf[0] != '\r') && (buf[1] != '\n'));
    if(httpRequest.size() > 0)
    {
        char timestamp_buf[32];
        get_timestamp(timestamp_buf);
        databox->m_.lock();
        connectionHeader = "[" + std::to_string(databox->clients[connectionNumber]->connectNum) + "]\t";
        getpeername(databox->clients[connectionNumber]->socket_fd, (struct sockaddr *)(&cs_addr), &cs_len);
        databox->clients[connectionNumber]->IPaddress = inet_ntoa(cs_addr.sin_addr);
        databox->clients[connectionNumber]->port = std::to_string((int)htons((uint16_t)(cs_addr.sin_port & 0x0ffff)));
        std::string connectionDetails = "[";
        connectionDetails += timestamp_buf;
        connectionDetails += "] Client connected from " + databox->clients[connectionNumber]->IPaddress + 
        ":" + databox->clients[connectionNumber]->port + "\n";
        databox->logfile->insert(connectionHeader + connectionDetails);
        for(size_t i=0; i<httpRequest.size(); i++)
        {
             databox->logfile->insert(connectionHeader + httpRequest[i]);
        }
        databox->logfile->insert("\n");
        databox->m_.unlock();
        int fileSize;
        std::string url = (*startup)["rootdir"];
        std::string response;
        int responseCode;
        std::string md5value;
        parseRequestline(httpRequest[0], response, fileSize, url, responseCode, md5value);
        char message[response.length()];
        std::strcpy(message, response.c_str());
        std::string responseMessage;
        databox->m_.lock();
        databox->clients[connectionNumber]->path = url.substr((*startup)["rootdir"].length());
        databox->clients[connectionNumber]->fileSize = fileSize;
        databox->logfile->insert(connectionHeader + "Requested path: \'" + databox->clients[connectionNumber]->path + "\'\n");
        databox->logfile->insert(connectionHeader + "Content-Length: " +  std::to_string(databox->clients[connectionNumber]->fileSize) + "\n");
        if(responseCode == 200)
        {
            databox->logfile->insert(connectionHeader + "Content-Type: application/octet-stream\n");
            databox->logfile->insert(connectionHeader + "Content-MD5: " + md5value + "\n");
        }
        else
        {
            if(responseCode == 400)
                responseMessage = "400 Bad Request\n";
            else if(responseCode == 404)
                responseMessage = "404 Not Found\n";
            else
                responseMessage = "405 Method Not Allowed\n";
            databox->logfile->insert(connectionHeader + "Content-Type: text/html\n");
            databox->logfile->insert(connectionHeader + "ERROR: " + responseMessage);
        }
        better_write(databox->clients[connectionNumber]->socket_fd,message,sizeof(message));
        databox->m_.unlock();
        if(responseCode == 200)
        {
            int bytes_read = 0;
            int fd = open(url.c_str(), O_RDONLY);
            if(fd < 0){
                exit(EXIT_FAILURE);
            }
            else
            {
                do
                {
                    memset(buf,0,sizeof(buf));
                    bytes_read = read(fd, buf, sizeof(buf));
                    databox->m_.lock();
                    if(databox->clients[connectionNumber]->socket_fd == -1)
                    {
                        bytes_read = 0;
                        databox->m_.unlock();
                    }
                    if(bytes_read > 0)
                    {
                        better_write(databox->clients[connectionNumber]->socket_fd, buf, bytes_read);
                        databox->clients[connectionNumber]->kbSent += (bytes_read/1000.0);
                        databox->m_.unlock();
                        usleep(1000000);
                    }
                    else
                        databox->m_.unlock();
                }
                while(bytes_read > 0);
                close(fd);
            }
        }
    }
    char timestamp_buf[32];
    databox->m_.lock();
    if(databox->clients[connectionNumber]->socket_fd >= 0)
    {
        shutdown(databox->clients[connectionNumber]->socket_fd, SHUT_RDWR);
        close(databox->clients[connectionNumber]->socket_fd);
        databox->clients[connectionNumber]->socket_fd = -2;
        get_timestamp(timestamp_buf);
        std::string connectionDetails = "[";
        connectionDetails += timestamp_buf;
        connectionDetails += "] Done.\n";
        databox->logfile->insert(connectionHeader + connectionDetails);
    }
    databox->m_.unlock();
}

// Console menu sets the socket_fd of a killed connection to -1
// When quit is entered all sockets are killed including the master socket
// The menu only ends when the user presses quit
void executeMenu(int master_socket_fd, serializationBox *databox)
{
    std::string value;
    std::cout << "> ";
    std::getline(std::cin, value);
    if(value.substr(0,4) == "info") 
    {
        std::string option;
        int connectionNumber;
        std::stringstream ss(value);
        ss >> option;
        if(value.length() == 6)
        {
            ss >> connectionNumber;
            databox->m_.lock(); 
            auto find = databox->clients.find(connectionNumber);
            if(find != databox->clients.end())
            {
                std::cout << "[" << databox->clients[connectionNumber]->connectNum << "]\tClient at: ";
                std::cout << databox->clients[connectionNumber]->IPaddress << ":" << databox->clients[connectionNumber]->port <<std::endl;
                std::cout << "\tPath: " << databox->clients[connectionNumber]->path << std::endl;
                std::cout << "\tContent-Length: " << databox->clients[connectionNumber]->fileSize << std::endl;
                std::cout << "\tStart-time: [" << "]" << std::endl;
                std::cout << "\tShaper-Params: B="<< ", P="<< ", MAXR="<< "tokens/s, DIAL="<< "%, r="<< "tokens/s" << std::endl;
                std::cout << "\tSent: BC bytes (" << "%), time elapsed: " <<" sec" << std::endl;
            }
            else
                std::cout << "No such connection: " << connectionNumber << std::endl; 
            databox->m_.unlock();
        }
        else
        {
            databox->m_.lock();
            if(databox->clients.size() > 0)
            {
                for(auto it=databox->clients.begin(); it!=databox->clients.end(); it++)
                {
                    std::cout << "[" << it->second->connectNum << "]\tClient at: ";
                    std::cout << it->second->IPaddress << ":" << it->second->port <<std::endl;
                    std::cout << "\tPath: " << it->second->path << std::endl;
                    std::cout << "\tContent-Length: " << it->second->fileSize << std::endl;
                    std::cout << "\tStart-time: [" << "]" << std::endl;
                    std::cout << "\tShaper-Params: B=" <<", P=" <<", MAXR=" <<"tokens/s, DIAL=" <<"%, r="<< "tokens/s" << std::endl;
                    std::cout << "\tSent: BC bytes (" << "%), time elapsed: " <<" sec" << std::endl;
                }
            }
            else
                std::cout << "No active connection" << std::endl; 
            databox->m_.unlock();           
        }
        executeMenu(master_socket_fd,databox);
    }
    else if((value.substr(0,5) == "close") && (value.length() >= 7))
    {
        std::string option;
        int connectionNumber;
        std::stringstream ss(value);
        ss >> option >> connectionNumber;
        databox->m_.lock();
        auto find = databox->clients.find(connectionNumber);
        if(find == databox->clients.end())
        {
            std::cout << "No such connection: "<< connectionNumber << std::endl; 
        }
        else
        {
            std::cout << "Closing connection " << connectionNumber << " ..." << std::endl;
            int socket_fd = databox->clients[connectionNumber]->socket_fd;
            shutdown(socket_fd, SHUT_RDWR);
            close(socket_fd);
            databox->clients[connectionNumber]->socket_fd = -1;
            char timestamp_buf[32];
            get_timestamp(timestamp_buf);
            std::string connectionDetails = "[" + std::to_string(connectionNumber) + "]\t[";
            connectionDetails += timestamp_buf;
            connectionDetails += "] Terminated at user's request.\n";
            databox->logfile->insert(connectionDetails);
        }
        databox->m_.unlock();
        executeMenu(master_socket_fd,databox);
    }
    else if((value == "quit") || (std::cin.eof()))
    {
        databox->m_.lock();
        databox->finished = true;
        if(!std::cin.eof())
        {   
            for(auto it=databox->clients.begin(); it!=databox->clients.end(); it++)
            {
                int socket_fd = it->second->socket_fd;
                if(socket_fd >= 0)
                {
                    shutdown(socket_fd, SHUT_RDWR);
                    close(socket_fd);
                    it->second->socket_fd = -1;
                }
            }
            shutdown(master_socket_fd, SHUT_RDWR);
            close(master_socket_fd);
            char timestamp_buf[32];
            get_timestamp(timestamp_buf);
            std::string connectionClosed = "Server shutdown at [";
            connectionClosed += timestamp_buf;
            connectionClosed += "]\n";
            databox->logfile->insert(connectionClosed);
        }
        databox->m_.unlock();
    }
    else
    {
        std::cout << "Command not recognized.  Valid commands are: " << std::endl;
        std::cout << "\tinfo #\n\tinfo all\n\tdail # percent\n\tclose #\n\tquit" << std::endl;
        executeMenu(master_socket_fd,databox);
    }
}

// This function will iterate through the top for loop until the program 
//      has quit or max num connection is reached
// This function joins threads of connections that have closed 
//      and removes those connection objects along the way
void joinThreads(serializationBox *databox)
{
    bool finished = false;
    std::vector<connectionObj *> closedConnections;
    while(finished == false)
    {
        usleep(250000);
        databox->m_.lock();
        if(databox->finished == false)
        {
            for(auto it=databox->clients.begin(); it!=databox->clients.end(); it++)
            {
                if(it->second->socket_fd < 0)
                {
                    closedConnections.push_back(it->second);
                }
            }
            databox->m_.unlock();
            for(size_t i=0; i<closedConnections.size(); i++)
            {
                closedConnections[i]->threadptr->join();
                databox->m_.lock();
                delete closedConnections[i]->threadptr;
                if(closedConnections[i]->socket_fd == -1)
                    std::cout << "Connection " << closedConnections[i]->connectNum << " is closed" << std::endl;
                databox->clients.erase(closedConnections[i]->connectNum); 
                delete closedConnections[i];
                databox->m_.unlock();
            }
            closedConnections.clear();
        }
        else
        {
            databox->m_.unlock();
            finished = true;
        }
    }
    databox->m_.lock();
    for(auto it=databox->clients.begin(); it!=databox->clients.end(); it++)
    {
        if(it->second->socket_fd >= 0)
        {
            shutdown(it->second->socket_fd, SHUT_RDWR);
            close(it->second->socket_fd);
            it->second->socket_fd = -1;
        }
        databox->m_.unlock();
        it->second->threadptr->join();
        databox->m_.lock();
        delete it->second->threadptr;
        if(it->second->socket_fd == -1)
                    std::cout << "Connection " << it->second->connectNum << " is closed" << std::endl;
        delete it->second;
    }
    databox->clients.clear();
    databox->m_.unlock();
}

// Creates a hashmap that stores the section names as keys and an inner hashmap as value. 
// The inner hashmap stores the key value pairs within each section
void parseFile(std::string fileName, std::unordered_map<std::string,std::unordered_map<std::string,std::string>> & data)
{
    std::ifstream inputFile(fileName);
    if(inputFile.is_open())
    {
        std::string currentSection;
        std::string line;
        std::unordered_map<std::string,std::string> dataMap;
        int counter = 0;
        while(std::getline(inputFile,line))
        {
            if((line[0] != ';') && (!line.empty()))
            {
                if(line[0] == '[')
                {
                    if(counter > 0)
                    {
                        data[currentSection] = dataMap;
                        dataMap.clear();
                    }
                    line = line.substr(1,line.length()-2);
                    currentSection = line;
                    counter++;
                }
                else
                {
                    std::string key = line.substr(0,line.find('='));
                    std::string value = line.substr(line.find('=')+1);
                    dataMap[key] = value;
                }
            }
        }
        data[currentSection] = dataMap;
        dataMap.clear();
    }
    else
        std::cout << fileName << " was not found." << std::endl;
}

// When the master_socket is closed and shutdown when my_accept returns -1
// If too many connections get created, it will behave as if the user pressed quit
int main(int argc, char *argv[])
{
    int master_socket_fd = (-1);
    if(argc != 2) 
        usage();
    std::unordered_map<std::string,std::unordered_map<std::string,std::string>> configdata;
    parseFile(argv[1], configdata);
    master_socket_fd = create_master_socket(configdata["startup"]["port"].c_str());
    bool finished = false;
    createPID(configdata["startup"]["pidfile"]);
    serializationBox *databox = new serializationBox(0,false,&configdata["startup"]);
    std::thread * menuthread = new std::thread(executeMenu, master_socket_fd, databox);
    std::thread * jointhread = new std::thread(joinThreads, databox);
    while(finished == false)
    {                                                                                                         
        int newsockfd = (-1);
        newsockfd = my_accept(master_socket_fd);
        if(newsockfd == -1)
            finished = true;
        else
        {
            databox->m_.lock();
            databox->next_connection_number++;
            int connectionNumber = databox->next_connection_number; 
            if(connectionNumber <= 100000) 
            {
                connectionObj * newObj = new connectionObj(connectionNumber, newsockfd, NULL,0.0);
                newObj->threadptr = new std::thread(talk_to_clients, connectionNumber, databox, &configdata["startup"]);
                databox->clients[connectionNumber] = newObj;
                databox->m_.unlock();
            }
            else
            {
                std::cout << "100,000 connections served.  Proceed with auto-shutdown..." << std::endl;
                for(auto it=databox->clients.begin(); it!=databox->clients.end(); it++)
                {
                    int socket_fd = it->second->socket_fd;
                    if(socket_fd >= 0)
                    {
                        shutdown(socket_fd, SHUT_RDWR);
                        close(socket_fd);
                        it->second->socket_fd = -1;
                    }
                }
                shutdown(master_socket_fd, SHUT_RDWR);
                close(master_socket_fd);
                char timestamp_buf[32];
                get_timestamp(timestamp_buf);
                std::string connectionClosed = "Server shutdown at [";
                connectionClosed += timestamp_buf;
                connectionClosed += "]\n";
                databox->logfile->insert(connectionClosed);
                finished = true;
                std::cin.setstate(std::basic_istream<char>::eofbit);
                databox->m_.unlock();
            }
        }
    }
    jointhread->join();
    std::cout << "Console thread terminated" << std::endl;
    menuthread->join();
    delete menuthread;
    delete jointhread;
    delete databox;
    return 0;
}