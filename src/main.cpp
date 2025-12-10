#include "server.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    int port = 8080;
    string docroot = "./www";

    size_t threads = thread::hardware_concurrency();

    cout << "[DEBUG] Threads allocated: " << threads << endl;

    if (threads == 0) threads = 4;
    size_t queue_size = 128;

    for (int i = 1; i < argc; i += 2) {
        if (i + 1 >= argc) break;

        string option = argv[i];
        string value = argv[i + 1];

        if (option == "--port") {
            port = stoi(value);
        } else if (option == "--docroot") {
            docroot = value;
        } else if (option == "--threads") {
            threads = stoi(value);
        } else if (option == "--queue") {
            queue_size = stoi(value);
        }
    }

    try {
        Server server(port, docroot, threads, queue_size);
        server.start();
    } catch (const exception& e) {
        cerr << "Error starting server: " << e.what() << endl;
        return 1;
    }

    return 0;
}
