#ifndef SERVER_H
#define SERVER_H

#include <string>

using namespace std;

class Server {
private:
    int port_;
    string docroot_;
    int server_fd_;
    
    void setup_socket();
    void handle_client(int client_fd);

public:
    Server(int port, const string& docroot);
    ~Server();
    
    void start();
};

#endif // SERVER_H
