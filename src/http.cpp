#include "http.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define stat _stat
#else
#include <unistd.h>
#include <limits.h>
#endif

using namespace std;

bool HttpHandler::parse_request(const string& raw_request, HttpRequest& request) {
    return false;
}

string HttpHandler::build_response(const HttpResponse& response) {
    return "";
}

string HttpHandler::get_content_type(const string& file_path) {
    return "";
}

bool HttpHandler::is_safe_path(const string& path, const string& docroot) {
    return false;
}

string HttpHandler::read_file(const string& file_path) {
    return "";
}

HttpResponse HttpHandler::handle_get(const HttpRequest& request, const string& docroot) {
    HttpResponse response;
    return response;
}

HttpResponse HttpHandler::handle_head(const HttpRequest& request, const string& docroot) {
    HttpResponse response;
    return response;
}

HttpResponse HttpHandler::handle_error(int status_code, const string& message) {
    HttpResponse response;
    return response;
}
