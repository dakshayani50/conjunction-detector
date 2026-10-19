#include "time/epoch.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace cdet {

namespace {

std::time_t timegm_utc(std::tm* tm) {
#ifdef __APPLE__
    return timegm(tm);
#else
    return timegm(tm);
#endif
}

}  // namespace

std::string add_minutes(
    const std::string& epoch,
    double minutes) {

    if (epoch.size() < 19) {
        throw std::invalid_argument("Invalid epoch: " + epoch);
    }

    std::tm tm{};
    tm.tm_year = std::stoi(epoch.substr(0, 4)) - 1900;
    tm.tm_mon = std::stoi(epoch.substr(5, 2)) - 1;
    tm.tm_mday = std::stoi(epoch.substr(8, 2));
    tm.tm_hour = std::stoi(epoch.substr(11, 2));
    tm.tm_min = std::stoi(epoch.substr(14, 2));
    tm.tm_sec = std::stoi(epoch.substr(17, 2));

    const double seconds = minutes * 60.0;

    const std::time_t base = timegm_utc(&tm);
    const std::time_t shifted =
        base + static_cast<std::time_t>(seconds);

    std::tm result{};
    gmtime_r(&shifted, &result);

    std::ostringstream out;

    out << std::setfill('0')
        << std::setw(4) << result.tm_year + 1900 << "-"
        << std::setw(2) << result.tm_mon + 1 << "-"
        << std::setw(2) << result.tm_mday << "T"
        << std::setw(2) << result.tm_hour << ":"
        << std::setw(2) << result.tm_min << ":"
        << std::setw(2) << result.tm_sec;

    return out.str();
}

}  // namespace cdet