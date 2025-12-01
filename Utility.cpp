#include "Utility.h"
#include <cstdlib>
#include <string>

// CREDIT TO: Matus Novak @ https://matusnovak.com/blog/cpp_tips_open_in_os/
// Original code is slightly different, this was taken from him 
// and changed to take normal strings instead of using fmt::format

// Detect OS and choose the correct command
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
static const char* openUrlCommand = "start";
#elif defined(__APPLE__)
static const char* openUrlCommand = "open";
#elif defined(__linux__)
static const char* openUrlCommand = "xdg-open";
#else
#error "Unknown or unsupported operating system"
#endif

void openWebBrowser(const std::string& url) {
    std::string cmd = std::string(openUrlCommand) + " " + url;
    std::system(cmd.c_str());
}