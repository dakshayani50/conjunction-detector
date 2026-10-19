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

    if (steps <= 0 ||
        step_minutes <= 0.0 ||
        threshold_km <= 0.0) {
        return results;
    }

    /*
     * Store the best result found for every satellite pair.
     *
     * Instead of:
     *
     *   pair A-B -> propagate A, propagate B
     *   pair A-C -> propagate A, propagate C
     *   pair B-C -> propagate B, propagate C
     *
     * we propagate every satellite once at each time step:
     *
     *   propagate A
     *   propagate B
     *   propagate C
     *
     * and then compare those already-computed states.
     */

    const std::size_t n = candidate_indices.size();

    struct PairResult {
        double minimum_distance =
            std::numeric_limits<double>::max();

        std::string closest_epoch;
    };

    std::vector<PairResult> best_results(
        n * n);

    for (int step = 0; step < steps; ++step) {

        const std::string current_epoch =
            add_minutes(
                start_epoch,
                step * step_minutes);

        // Propagate every satellite exactly once for this epoch.
        std::vector<StateVector> states;
        states.reserve(n);

        for (const std::size_t index : candidate_indices) {

            if (index >= satellites.size()) {
                states.push_back({});
                continue;
            }

            states.push_back(
                propagate(
                    satellites[index],
                    current_epoch));
        }

        // Compare the already-propagated states.
        for (std::size_t a = 0; a < n; ++a) {

            const std::size_t i =
                candidate_indices[a];

            if (i >= satellites.size()) {
                continue;
            }

            for (std::size_t b = a + 1; b < n; ++b) {

                const std::size_t j =
                    candidate_indices[b];

                if (j >= satellites.size()) {
                    continue;
                }

                const double distance =
                    distance_km(
                        states[a],
                        states[b]);

                PairResult& best =
                    best_results[a * n + b];

                if (distance < best.minimum_distance) {
                    best.minimum_distance = distance;
                    best.closest_epoch = current_epoch;
                }
            }
        }
    }

    // Convert qualifying pair results into the public result format.
    for (std::size_t a = 0; a < n; ++a) {

        const std::size_t i =
            candidate_indices[a];

        if (i >= satellites.size()) {
            continue;
        }

        for (std::size_t b = a + 1; b < n; ++b) {

            const std::size_t j =
                candidate_indices[b];

            if (j >= satellites.size()) {
                continue;
            }

            const PairResult& best =
                best_results[a * n + b];

            if (best.minimum_distance <= threshold_km) {

                results.push_back({
                    i,
                    j,
                    best.closest_epoch,
                    best.minimum_distance
                });
            }
        }
    }

    return results;
}

}  // namespace cdet