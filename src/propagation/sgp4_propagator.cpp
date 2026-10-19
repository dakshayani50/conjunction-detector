#include "propagation/sgp4_propagator.hpp"

#include <iomanip>
#include <sstream>
#include <string>

#include "DateTime.h"
#include "Eci.h"
#include "SGP4.h"
#include "Tle.h"

namespace cdet {

StateVector propagate(
    const Satellite& satellite,
    const std::string& epoch) {

    std::ostringstream csv;

    csv << std::setprecision(17)
        << satellite.name << ","
        << satellite.intl_id << ","
        << satellite.epoch << ","
        << satellite.mean_motion << ","
        << satellite.eccentricity << ","
        << satellite.inclination << ","
        << satellite.raan << ","
        << satellite.arg_perigee << ","
        << satellite.mean_anomaly << ","
        << "0,"
        << "U,"
        << satellite.norad_id << ","
        << "0,"
        << "0,"
        << satellite.bstar << ","
        << satellite.mean_motion_dot << ","
        << satellite.mean_motion_ddot;

    const auto tle =
        libsgp4::Tle::FromCsv(csv.str());

    const libsgp4::SGP4 propagator(tle);

    const libsgp4::DateTime date(
        std::stoi(epoch.substr(0, 4)),
        std::stoi(epoch.substr(5, 2)),
        std::stoi(epoch.substr(8, 2)),
        std::stoi(epoch.substr(11, 2)),
        std::stoi(epoch.substr(14, 2)),
        std::stod(epoch.substr(17))
    );

    const libsgp4::Eci eci =
        propagator.FindPosition(date);

    const auto position = eci.Position();
    const auto velocity = eci.Velocity();

    return {
        position.x,
        position.y,
        position.z,
        velocity.x,
        velocity.y,
        velocity.z
    };
}

}  // namespace cdet