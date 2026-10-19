#include "screening/conjunction_screen.hpp"

#include <cmath>

namespace cdet {

namespace {

double distance_km(const StateVector& a, const StateVector& b) {
    const double dx = a.x_km - b.x_km;
    const double dy = a.y_km - b.y_km;
    const double dz = a.z_km - b.z_km;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

}  // namespace

std::vector<ConjunctionCandidate> screen_conjunctions(
    const std::vector<StateVector>& states,
    double threshold_km) {

    std::vector<ConjunctionCandidate> candidates;

    if (threshold_km <= 0.0) {
        return candidates;
    }

    for (std::size_t i = 0; i < states.size(); ++i) {
        for (std::size_t j = i + 1; j < states.size(); ++j) {
            const double distance = distance_km(states[i], states[j]);

            if (distance <= threshold_km) {
                candidates.push_back({
                    i,
                    j,
                    distance
                });
            }
        }
    }

    return candidates;
}

}  // namespace cdet