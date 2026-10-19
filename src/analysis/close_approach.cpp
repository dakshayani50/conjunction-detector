#include "analysis/close_approach.hpp"
#include "time/epoch.hpp"

#include <cmath>
#include <limits>
#include <vector>

#include "propagation/sgp4_propagator.hpp"

namespace cdet {

namespace {

double distance_km(const StateVector& a, const StateVector& b) {
    const double dx = a.x_km - b.x_km;
    const double dy = a.y_km - b.y_km;
    const double dz = a.z_km - b.z_km;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

}  // namespace

std::vector<CloseApproach> find_close_approaches(
    const std::vector<Satellite>& satellites,
    const std::vector<std::size_t>& candidate_indices,
    const std::string& start_epoch,
    int steps,
    double step_minutes,
    double threshold_km) {

    std::vector<CloseApproach> results;

    if (steps <= 0 || step_minutes <= 0.0 || threshold_km <= 0.0) {
        return results;
    }

    for (std::size_t a = 0; a < candidate_indices.size(); ++a) {
        for (std::size_t b = a + 1; b < candidate_indices.size(); ++b) {

            const std::size_t i = candidate_indices[a];
            const std::size_t j = candidate_indices[b];

            if (i >= satellites.size() || j >= satellites.size()) {
                continue;
            }

            double minimum_distance =
                std::numeric_limits<double>::max();

            std::string closest_epoch = start_epoch;

            for (int step = 0; step < steps; ++step) {

                const std::string current_epoch =
                    add_minutes(start_epoch, step * step_minutes);

                const auto state_a =
                    propagate(satellites[i], current_epoch);

                const auto state_b =
                    propagate(satellites[j], current_epoch);

                const double distance =
                    distance_km(state_a, state_b);

                if (distance < minimum_distance) {
                    minimum_distance = distance;
                    closest_epoch = current_epoch;
                }
            }

            if (minimum_distance <= threshold_km) {
                results.push_back({
                    i,
                    j,
                    closest_epoch,
                    minimum_distance
                });
            }
        }
    }

    return results;
}

}  // namespace cdet