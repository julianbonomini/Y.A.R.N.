#include "execute_utils.hpp"
#include <array>
#include <cstdio>
#include <sys/statvfs.h>
#include <string>
#include <fstream>
#include <iostream>

#ifdef __linux__
    #include <sys/statvfs.h>
    #include <filesystem>
    #include <ifaddrs.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#endif


std::string ExecuteUtils::execCommand(const std::string &cmd) {
    std::array<char, 128> buffer; {
    };
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) return "Error opening pipe";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string ExecuteUtils::getIp() {
#if defined(__APPLE__)
    std::string ip = ExecuteUtils::execCommand("ipconfig getifaddr en0");
    return ip;
#elif defined(__linux__)
    std::string ip = execCommand("hostname -I");
    return ip;
#else
    std::cout << "Unknown platform\n";
    return "Unkown platform";
#endif
}

std::string ExecuteUtils::getCpuModel() {
#ifdef __linux__
    std::ifstream cpuInfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuInfo, line)) {
        if (line.find("model name") != std::string::npos || line.find("Hardware") != std::string::npos) {
            auto colon = line.find(":");
            return line.substr(colon + 2);
        }
    }
    return "Unknown CPU";
#elif defined(__APPLE__)
    char buffer[256];
    size_t size = sizeof(buffer);
    sysctlbyname("machdep.cpu.brand_string", &buffer, &size, NULL, 0);
    return std::string(buffer);
#else
    return "Unsupported OS";
#endif
}

std::string ExecuteUtils::getRam() {
#ifdef __linux__
    std::ifstream memInfo("/proc/meminfo");
    std::string line;
    while (std::getline(memInfo, line)) {
        if (line.find("MemTotal") != std::string::npos) {
            auto colon = line.find(":");
            std::string kbStr = line.substr(colon + 1);
            int kb = std::stoi(kbStr);
            int gb = kb / (1024 * 1024.0) + 0.5;
            return std::to_string(gb) + " GB";
        }
    }
    return "Unknown RAM";
#elif defined(__APPLE__)
    int64_t memBytes;
    size_t len = sizeof(memBytes);
    sysctlbyname("hw.memsize", &memBytes, &len, NULL, 0);
    int gb = memBytes / (1024 * 1024 * 1024);
    return std::to_string(gb) + " GB";
#else
    return "Unsupported OS";
#endif
}

std::string ExecuteUtils::getStorageUsage(const std::string &path = "/") {
#ifdef __linux__
    struct statvfs stat;
    if (statvfs(path.c_str(), &stat) != 0) return "Unknown";

    auto total = stat.f_blocks * stat.f_frsize;
    auto free = stat.f_bfree * stat.f_frsize;
    auto used = total - free;

    auto toGB = [](unsigned long long bytes) {
        return bytes / (1024 * 1024 * 1024);
    };

    return std::to_string(toGB(used)) + "GB of " + std::to_string(toGB(total)) + "GB";
#elif defined(__APPLE__)
    struct statvfs stat;
    if (statvfs(path.c_str(), &stat) != 0) return "Unknown";

    unsigned long long total = stat.f_blocks * stat.f_frsize;
    unsigned long long free = stat.f_bfree * stat.f_frsize;
    unsigned long long used = total - free;

    auto toGB = [](unsigned long long bytes) {
        return bytes / (1024 * 1024 * 1024);
    };

    return std::to_string(toGB(used)) + "GB of " + std::to_string(toGB(total)) + "GB";
#else
    return "Unsupported OS";
#endif
}

std::string ExecuteUtils::getUptime() {
#ifdef __linux__
    std::ifstream uptimeFile("/proc/uptime");
    double uptimeSeconds;
    uptimeFile >> uptimeSeconds;

    int days = uptimeSeconds / (60 * 60 * 24);
    int hours = ((int)uptimeSeconds % (60 * 60 * 24)) / (60 * 60);

    return std::to_string(days) + " days, " + std::to_string(hours) + " hours";
#elif defined(__APPLE__)
    struct timeval boottime;
    size_t size = sizeof(boottime);
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};
    time_t now;
    time(&now);

    if (sysctl(mib, 2, &boottime, &size, NULL, 0) != -1 && boottime.tv_sec != 0) {
        time_t uptime = now - boottime.tv_sec;
        int days = uptime / (60 * 60 * 24);
        int hours = (uptime % (60 * 60 * 24)) / (60 * 60);
        return std::to_string(days) + " days, " + std::to_string(hours) + " hours";
    }

    return "Unknown Uptime";
#else
    return "Unsupported OS";
#endif
}

std::string ExecuteUtils::getNetworkStatus() {
#ifdef __linux__
    std::string result;
    std::string basePath = "/sys/class/net/";

    for (const auto& entry : std::filesystem::directory_iterator(basePath)) {
        std::string iface = entry.path().filename();
        std::ifstream operstate(entry.path() / "operstate");
        std::string state;
        if (operstate >> state) {
            std::string label;

            if (iface.find("eth") == 0) label = "Ethernet";
            else if (iface.find("wlan") == 0) label = "Wi-Fi";
            else continue;

            result += label + ": " + (state == "up" ? "Connected" : "Disconnected") + "\n";
        }
    }

    return result.empty() ? "No network interfaces detected" : result;
#elif defined(__APPLE__)
    return "Network status not implemented on macOS"; // Can be implemented with SystemConfiguration framework in Obj-C
#else
    return "Unsupported OS";
#endif
}
