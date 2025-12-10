#include "server.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    int port = 8080;
    string docroot = "./www";
    
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 >= argc) break;
        
        string option = argv[i];
        string value = argv[i + 1];
        
        if (option == "--port") {
            port = stoi(value);
        } else if (option == "--docroot") {
            docroot = value;
        }
    }
    
    try {
        Server server(port, docroot);
        server.start();
    } catch (...) {
        cerr << "Error starting server" << endl;
        return 1;
    }
    
    return 0;
}
