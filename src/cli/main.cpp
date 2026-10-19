
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "core.hpp"
#include "ingest/omm.hpp"
#include "propagation/sgp4_propagator.hpp"

int main(int argc, char** argv) {
    std::cout << "conjunction detector " << cdet::version() << "\n";

    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <catalog.csv>\n";
        return 1;
    }

    try {
        const auto sats = cdet::load_catalog_csv(argv[1]);

        std::cout << "loaded " << sats.size() << " satellites\n";

        if (sats.empty()) {
            return 0;
        }

        const auto& satellite = sats[0];

        std::cout << "propagating: " << satellite.name
                  << " (NORAD " << satellite.norad_id << ")\n";

        const auto state = cdet::propagate(satellite, satellite.epoch);

        std::cout << std::fixed << std::setprecision(6);

        std::cout << "position (km): "
                  << state.x_km << ", "
                  << state.y_km << ", "
                  << state.z_km << "\n";

        std::cout << "velocity (km/s): "
                  << state.vx_km_s << ", "
                  << state.vy_km_s << ", "
                  << state.vz_km_s << "\n";

    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
