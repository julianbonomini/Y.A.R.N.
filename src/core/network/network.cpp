#include "../utils/network.hpp"
#include "../utils/execute_utils.hpp"


std::string Network::getIp() {
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