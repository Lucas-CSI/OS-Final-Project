#include "http.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

using namespace std;

bool HttpHandler::parse_request(const string& raw_request, HttpRequest& request) {
    istringstream stream(raw_request);
    string line;

    if (!getline(stream, line))
        return false;
    if (!line.empty() && line.back() == '\r') 
        line.pop_back();

    istringstream request_line(line);
    if (!(request_line >> request.method >> request.path >> request.version)) {
        return false;
    }

    while (getline(stream, line)) {
        if (line == "\r" || line.empty()) 
            break;
        if (!line.empty() && line.back() == '\r') 
            line.pop_back();

        size_t colon = line.find(':');
        if (colon == string::npos) 
            return false;

        string key = line.substr(0, colon);
        string value = line.substr(colon + 1);

        value.erase(0, value.find_first_not_of(" \t"));
        request.headers[key] = value;
    }

    return true;
}

string HttpHandler::build_response(const HttpResponse& response) {
    ostringstream out;

    out << "HTTP/1.1 " << response.status_code << " " << response.status_text << "\r\n";

    for (const auto& h : response.headers) {
        out << h.first << ": " << h.second << "\r\n";
    }

    out << "\r\n";
    out << response.body;

    return out.str();
}


string HttpHandler::get_content_type(const string& file_path) {
    size_t dot = file_path.find_last_of('.');
    if (dot == string::npos)
        return "application/octet-stream";

    string ext = file_path.substr(dot + 1);
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == "html" || ext == "htm") return "text/html";
    if (ext == "css")  return "text/css";
    if (ext == "js")   return "application/javascript";
    if (ext == "png")  return "image/png";
    if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
    if (ext == "gif")  return "image/gif";
    if (ext == "txt")  return "text/plain";

    return "application/octet-stream";
}


bool HttpHandler::is_safe_path(const string& path, const string& docroot) {
    char resolved_path[PATH_MAX];
    char resolved_root[PATH_MAX];

    string full_path = docroot + path;

    if (!realpath(full_path.c_str(), resolved_path)) 
        return false;
    if (!realpath(docroot.c_str(), resolved_root)) 
        return false;

    return string(resolved_path).find(resolved_root) == 0;
}

string HttpHandler::read_file(const string& file_path) {
    ifstream file(file_path, ios::binary);
    if (!file) return "";

    ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

HttpResponse HttpHandler::handle_get(const HttpRequest& request, const string& docroot) {
    HttpResponse response;

    if (!is_safe_path(request.path, docroot)) {
        return handle_error(403, "Forbidden");
    }

    string file_path = docroot + request.path;

    if (file_path == "./www/")
        file_path = "./www/index.html";
    struct stat st;

    if (stat(file_path.c_str(), &st) != 0 || S_ISDIR(st.st_mode)) {
        return handle_error(404, "Not Found");
    }

    response.body = read_file(file_path);
    response.status_code = 200;
    response.status_text = "OK";

    response.headers["Content-Type"] = get_content_type(file_path);
    response.headers["Content-Length"] = to_string(response.body.size());

    return response;
}

HttpResponse HttpHandler::handle_head(const HttpRequest& request, const string& docroot) {
    HttpResponse response = handle_get(request, docroot);

    response.body.clear();
    response.headers["Content-Length"] = "0";

    return response;
}

HttpResponse HttpHandler::handle_error(int status_code, const string& message) {
    HttpResponse response;

    response.status_code = status_code;
    response.status_text = message;
    response.body = "<h1>" + to_string(status_code) + " " + message + "</h1>";

    response.headers["Content-Type"] = "text/html";
    response.headers["Content-Length"] = to_string(response.body.size());

    return response;
}
