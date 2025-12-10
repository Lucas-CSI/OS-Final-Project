#include "server.h"
#include "http.h"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


using namespace std;

Server::Server(int port, const string& docroot)
    : port_(port), docroot_(docroot), server_fd_(-1) {
}

Server::~Server() {
    if (server_fd_ >= 0) {
        close(server_fd_);
    }
}

void Server::setup_socket() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR,
               (char*)&opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    bind(server_fd_, (sockaddr*)&address, sizeof(address));
    listen(server_fd_, 10);
}

void Server::handle_client(int client_fd) {
    char buffer[8192];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        close(client_fd);
        return;
    }

    buffer[bytes_read] = '\0';
    string raw_request(buffer);

    HttpRequest request;
    HttpResponse response;

    if (!HttpHandler::parse_request(raw_request, request)) {
        response = HttpHandler::handle_error(400, "Bad Request");
    } else if (request.method == "GET") {
        response = HttpHandler::handle_get(request, docroot_);
    } else if (request.method == "HEAD") {
        response = HttpHandler::handle_head(request, docroot_);
    } else {
        response = HttpHandler::handle_error(405, "Method Not Allowed");
    }

    response.headers["Connection"] = "close";

    string http_response = HttpHandler::build_response(response);
    send(client_fd, http_response.c_str(), http_response.size(), 0);

    close(client_fd);
}

void Server::start() {
    setup_socket();
    cout << "Server started on port " << port_ << endl;

    while (true) {
        sockaddr_in client_address{};
        socklen_t addrlen = sizeof(client_address);

        int client_fd = accept(server_fd_,
                               (sockaddr*)&client_address,
                               &addrlen);
        if (client_fd >= 0) {
            handle_client(client_fd);
        }
    }
}
