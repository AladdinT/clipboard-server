#include "myserver.hpp"

// use namespace
std::string readImageFile(const std::string& filename);
std::string readHtmlFile(const std::string& filename);

void clipboardHandler(int& clientSocket);
void screenshotHandler(int& clientSocket);
void homeHandler(int& clientSocket);
int takeScreenShot();

int main() {
    // Initialize  server
    int portNumber = 8080;
    MyServer pcServer(portNumber);
    if( pcServer.startListening() != 0)
    {
        perror("Error listening on socket");
        return 1;
    }

    while (true) 
    {
        int clientSocket = pcServer.acceptClient();
        std::string url = MyServer::httpGetRequestUrl(clientSocket);
      
        if ( url == "/clipboard")
        {
            clipboardHandler(clientSocket);
        }
        else if ( url == "/prtscr")
        {
            screenshotHandler(clientSocket);
        }
        else if( url == "")
        {
            
        }
        else
        {
            clipboardHandler(clientSocket);
        }

        // Close the client socket
        close(clientSocket);
    }

    return 0;
}


// Function to read the image file into a string
std::string readImageFile(const std::string& filename) 
{
    std::cout << __PRETTY_FUNCTION__ << " called\n";

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return "";
    }

    // Read the file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
std::string readHtmlFile(const std::string& filename) {
    std::ifstream htmlFileDesc(filename);
    std::stringstream buffer;
    buffer << htmlFileDesc.rdbuf();
    std::string htmlContent = buffer.str();
    return htmlContent;
}

int takeScreenShot()
{
    std::cout << __PRETTY_FUNCTION__ << " called\n";
    // TODO : save screenshot to pictures and retrieve it from there
    const char* command = "DISPLAY=:0 scrot -o -F screenshot.png";
    
    int result = std::system(command);
    
    if (result == 0) {
        std::cout << "Screenshot saved as screenshot.png" << std::endl;
    } else {
        std::cerr << "Failed to take a screenshot." << std::endl;
        return -1;
    }

    return 0;
}

void screenshotHandler(int& clientSocket)
{
    std::cout << __PRETTY_FUNCTION__ << " called\n";
    if(takeScreenShot() != 0)
    {
        // TODO handle error
    }
    // Prepare an HTTP response with an image
    std::string imageContent = readImageFile("screenshot.png");

    if (!imageContent.empty()) {
        std::string httpResponse = "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: image/jpeg\r\n";
        httpResponse += "Content-Length: " + std::to_string(imageContent.size() + 50 ) + "\r\n\r\n";
        httpResponse += imageContent;


        // Send the HTTP response with the image
        send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
    } else {
        // Error: Could not read the image file
        std::string errorResponse = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        send(clientSocket, errorResponse.c_str(), errorResponse.length(), 0);
    }

}

void homeHandler(int& clientSocket)
{
    return;
}

void clipboardHandler(int& clientSocket)
{
    std::cout << __PRETTY_FUNCTION__ << " called\n";
    char buffer[128];
    std::string result = "";
    
    // Open the command for reading
    FILE* pipe = popen("xclip -o", "r");
    if (!pipe) {
        std::cerr << "Error opening pipe!" << std::endl;
        return;
    }

    // Read the output from the command
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    // Close the pipe
    pclose(pipe);

    if (!result.empty()) {
        std::string httpResponse = "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: text/html\r\n";
        httpResponse += "Content-Length: " + std::to_string(result.size() + 1024) + "\r\n\r\n";
        httpResponse += "\
        <!DOCTYPE html><html>\
        <style>.button-1 {background-color: #EA4C89;color: #FFFFFF; font-size:18px; font-weight: 900;\
        height: 40px;line-height: 20px;padding: 10px 16px;text-align: center;}</style>\
        <body><div style='text-align:center;'> <h1> Requests </h1>\
        <button class=\"button-1\" onclick=\"window.location.href='/clipboard'\">Refresh Clipboard</button>   \
        <button class=\"button-1\" onclick=\"window.location.href='/prtscr'\">Screen Shot</button><br></div> \
        <pre>" + result + "</pre></body></html>";
        

        // Send the HTTP response with the image
        send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
    } else {
        // Error: Could not read the image file
        std::string errorResponse = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        send(clientSocket, errorResponse.c_str(), errorResponse.length(), 0);
    }

}
