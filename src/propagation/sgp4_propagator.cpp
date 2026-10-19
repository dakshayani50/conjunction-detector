#include "propagation/sgp4_propagator.hpp"

#include <stdexcept>
#include <string>

#include "DateTime.h"
#include "Eci.h"
#include "SGP4.h"
#include "Tle.h"

namespace cdet {

StateVector propagate(const Satellite& satellite, const std::string& epoch) {
    const std::string csv =
        satellite.name + "," +
        satellite.intl_id + "," +
        satellite.epoch + "," +
        std::to_string(satellite.mean_motion) + "," +
        std::to_string(satellite.eccentricity) + "," +
        std::to_string(satellite.inclination) + "," +
        std::to_string(satellite.raan) + "," +
        std::to_string(satellite.arg_perigee) + "," +
        std::to_string(satellite.mean_anomaly) + "," +
        "0," +
        "U," +
        std::to_string(satellite.norad_id) + "," +
        "0," +
        "0," +
        std::to_string(satellite.bstar) + "," +
        std::to_string(satellite.mean_motion_dot) + "," +
        std::to_string(satellite.mean_motion_ddot);

    const auto tle = libsgp4::Tle::FromCsv(csv);
    const libsgp4::SGP4 propagator(tle);

    const libsgp4::DateTime date(
        std::stoi(epoch.substr(0, 4)),
        std::stoi(epoch.substr(5, 2)),
        std::stoi(epoch.substr(8, 2)),
        std::stoi(epoch.substr(11, 2)),
        std::stoi(epoch.substr(14, 2)),
        std::stod(epoch.substr(17))
    );

    const libsgp4::Eci eci = propagator.FindPosition(date);
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
