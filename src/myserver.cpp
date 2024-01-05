#include "myserver.hpp"

MyServer::MyServer(int& portNumber) 
: port(portNumber)
{
    // Create a socket
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    // Bind the socket to local address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons( this->port );
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(this->serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error binding socket");
        exit(1);
    }

}

int MyServer::startListening()
{
    // Listen for incoming connections
    if (listen(this->serverSocket, 10) == -1) {
        perror("Error listening on socket");
        return 1;
    }

    std::cout << "HTTP server listening on port " << this->port << std::endl;
    return 0;
}

int MyServer::stopListening()
{
    // Listen for incoming connections
    if (listen(this->serverSocket, 0) == -1) {
        perror("Error listening on socket");
        return 1;
    }

    std::cout << "HTTP server stopped listening on port "  << this->port  << std::endl;
    return 0;
}

int MyServer::acceptClient()
{
    // Accept incoming connections
    int clientSocket = accept(this->serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        perror("Error accepting connection");
        return -1;
    }
    std::cout << __PRETTY_FUNCTION__ << clientSocket << "\n";

    return clientSocket;
}

std::string MyServer::httpGetRequestUrl(int& clientSocket)
{
    // Read the HTTP request
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead <= 0) {
        perror("Error reading request");
        close(clientSocket);
        return "";
    }

    // Parsing url from the GET request 
    std::regex requestRegex("^([A-Z]+)\\s+([^\\s]+)\\s+HTTP/1\\.[01]");
    std::cmatch match;
    std::string url = "/";

    if (std::regex_search(buffer, match, requestRegex)) 
    {
        if( match[1] == "GET" )
        {
            url = match[2];
        }
    } 
    else 
    {
        std::cerr << "Failed to parse GET request" << std::endl;
    }

    std::cout << "req-" << match[1] << "-" << match[2] <<"-\n";
    return url;
}

MyServer::~MyServer()
{
    // Close serverSocket
    close(this->serverSocket);
}