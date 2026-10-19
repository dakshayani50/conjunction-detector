#pragma once

#include <string>

#include "ingest/omm.hpp"

namespace cdet {

struct StateVector {
    double x_km = 0.0;
    double y_km = 0.0;
    double z_km = 0.0;

    double vx_km_s = 0.0;
    double vy_km_s = 0.0;
    double vz_km_s = 0.0;
};

StateVector propagate(const Satellite& satellite, const std::string& epoch);

}  // namespace cdet
