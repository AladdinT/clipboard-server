#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <regex> 

class MyServer
{
private:
    int port;
    bool    is_listening;
    int serverSocket;
public:
    
    // RULE OF ZERO
    MyServer() = delete;
    MyServer(int&  portNumber);
    ~MyServer();
    
    // Methods
    int startListening();
    int stopListening();
    int acceptClient();
    static std::string httpGetRequestUrl(int& clientSocket);
    

private:
    
};

