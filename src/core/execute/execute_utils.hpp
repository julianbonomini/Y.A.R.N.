#ifndef EXECUTE_UTILS_H
#define EXECUTE_UTILS_H


#include <string>

class ExecuteUtils {
public:
    static std::string getResourcePath(const std::string& relativePath);

    static std::string execCommand(const std::string& cmd);

    static std::string getIp();

    static std::string getCpuModel();

    static std::string getCpuUsage();

    static std::string getGpuModel();

    static std::string getRam();

    static std::string getStorageUsage(const std::string &path);

    static std::string getUptime();

    static std::string getNetworkStatus();
};

#endif //EXECUTE_UTILS_H
