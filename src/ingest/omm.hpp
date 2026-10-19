#pragma once

#include <string>
#include <vector>

namespace cdet {

struct Satellite {
    std::string name;
    std::string intl_id;
    std::string epoch;

    int norad_id = 0;

    double mean_motion = 0.0;
    double eccentricity = 0.0;
    double inclination = 0.0;
    double raan = 0.0;
    double arg_perigee = 0.0;
    double mean_anomaly = 0.0;

    double bstar = 0.0;
    double mean_motion_dot = 0.0;
    double mean_motion_ddot = 0.0;
};

std::vector<Satellite> load_catalog_csv(const std::string& path);

} // namespace cdet