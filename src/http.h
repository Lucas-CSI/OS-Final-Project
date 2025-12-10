#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <map>

using namespace std;

struct HttpRequest {
    string method;
    string path;
    string version;
    map<string, string> headers;
    string body;
};

struct HttpResponse {
    int status_code;
    string status_text;
    map<string, string> headers;
    string body;
};

class HttpHandler {
public:
    static bool parse_request(const string& raw_request, HttpRequest& request);
    static string build_response(const HttpResponse& response);
    static string get_content_type(const string& file_path);
    static bool is_safe_path(const string& path, const string& docroot);
    static string read_file(const string& file_path);
    static HttpResponse handle_get(const HttpRequest& request, const string& docroot);
    static HttpResponse handle_head(const HttpRequest& request, const string& docroot);
    static HttpResponse handle_error(int status_code, const string& message);
};

#endif // HTTP_H
