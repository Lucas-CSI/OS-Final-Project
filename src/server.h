#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "threadpool.h"
#include "blocking_queue.h"

using namespace std;

class Server {
private:
    int port_;
    string docroot_;
    int server_fd_;

    ThreadPool pool_;
    BlockingQueue<int> connection_queue_;

    void setup_socket();
    void handle_client(int client_fd);

public:
    Server(int port,
           const string& docroot,
           size_t thread_count,
           size_t queue_size);

    ~Server();
    void start();
};

#endif // SERVER_H
