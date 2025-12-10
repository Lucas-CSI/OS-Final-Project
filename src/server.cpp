#include "server.h"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define close closesocket
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

using namespace std;

Server::Server(int port, const string& docroot)
    : port_(port), docroot_(docroot), server_fd_(-1) {
}

Server::~Server() {
    if (server_fd_ >= 0) {
        close(server_fd_);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

void Server::setup_socket() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    
    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);
    
    bind(server_fd_, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd_, 10);
}

void Server::handle_client(int client_fd) {
    char buffer[4096] = {0};
    recv(client_fd, buffer, sizeof(buffer), 0);
    
    string file_path = docroot_ + "/index.html";
    ifstream file(file_path, ios::binary);
    
    if (file.is_open()) {
        ostringstream oss;
        oss << file.rdbuf();
        string content = oss.str();
        
        string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + to_string(content.size()) + "\r\n";
        response += "Connection: close\r\n\r\n";
        response += content;
        
        send(client_fd, response.c_str(), response.size(), 0);
    } else {
        string error = "HTTP/1.1 404 Not Found\r\n\r\n";
        send(client_fd, error.c_str(), error.size(), 0);
    }
    
    close(client_fd);
}

void Server::start() {
    setup_socket();
    cout << "Server started on port " << port_ << endl;
    
    while (true) {
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        int client_fd = accept(server_fd_, (struct sockaddr*)&client_address, &addrlen);
        if (client_fd >= 0) {
            handle_client(client_fd);
        }
    }
}
