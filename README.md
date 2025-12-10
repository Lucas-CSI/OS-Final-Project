# CSI 4337 - Multithreaded Web Server

A multithreaded HTTP/1.1 web server implementation in C++11 with thread pool, bounded blocking queue, and backpressure handling.

## Build Instructions

### Windows
1. Install MinGW-w64 or MSYS2 (includes g++ and make)
2. Open a terminal and navigate to the project directory
3. Build using:
   ```bash
   mingw32-make
   ```
   Or if `make` is in your PATH:
   ```bash
   make
   ```

### Linux/Mac
1. Ensure g++ and make are installed (usually pre-installed)
2. Open a terminal and navigate to the project directory
3. Build using:
   ```bash
   make
   ```

The Makefile automatically detects your operating system and sets the appropriate compiler flags.

## Run Instructions

Start the web server with customizable options:

**Windows:**
```bash
.\webserver.exe --port 8080 --docroot ./www --threads 8 --log server.log
```

**Linux/Mac:**
```bash
./webserver --port 8080 --docroot ./www --threads 8 --log server.log
```

Then, open your browser and navigate to:

```
http://localhost:8080/
```

## Command Line Options

- `--port <port>` - Port number to listen on (default: 8080)
- `--docroot <path>` - Document root directory (default: ./www)
- `--threads <num>` - Number of worker threads (default: 8)
- `--log <file>` - Log file path (default: server.log)

## Features Implemented

- ✅ HTTP/1.1 support for GET and HEAD requests
- ✅ Thread pool with bounded blocking queue (backpressure returns HTTP 503)
- ✅ Safe path handling to prevent directory traversal attacks
- ✅ Static file serving with Content-Type and Content-Length headers
- ✅ Synchronized logging with timestamps (server.log)
- ✅ Socket read/write timeouts for robustness

## Project Structure

```
CSI4337_GroupProject/
├── src/
│   ├── main.cpp           # Entry point, CLI argument parsing
│   ├── server.cpp/h       # Server class, socket handling
│   ├── threadpool.cpp/h   # Thread pool implementation
│   ├── blocking_queue.h   # Bounded blocking queue
│   └── http.cpp/h         # HTTP request/response handling
├── www/
│   └── index.html         # Sample HTML file
├── makefile               # Build configuration
└── README.md              # This file
```

## Load Testing

You can test server performance with ApacheBench (ab) or a similar tool:

```bash
ab -n 200 -c 20 http://127.0.0.1:8080/
```

## Notes

- Default document root is `./www`, which includes a sample `index.html`
- The server returns HTTP 503 when the request queue is full (backpressure)
- All requests are logged to the specified log file with timestamps

## Possible Extensions (Optional)

- Add support for POST requests and implement a simple `/echo` endpoint
- Implement caching for frequently accessed small files
- Add HTTPS support using OpenSSL
- Provide a monitoring dashboard (text-based or GUI)

## Requirements

- C++11 compatible compiler (g++ or clang++)
- pthread library (usually included with compiler)
- **Windows:** 
  - MinGW-w64 or MSYS2 (includes g++, make, and Winsock2)
  - Winsock2 library (ws2_32.lib) - included with MinGW
- **Linux/Mac:** 
  - Standard POSIX sockets (included in system)

## Cross-Platform Support

This project is designed to compile and run on both Windows and Linux/Mac. The code uses platform detection (`#ifdef _WIN32`) to automatically:
- Include the correct socket libraries (Winsock2 on Windows, POSIX on Linux/Mac)
- Use the correct file system functions
- Link the appropriate libraries

**Note:** `_WIN32` is defined on both 32-bit and 64-bit Windows, so the code works on all Windows architectures.

