#pragma once

#include <cstddef>
#include <vector>

#include "propagation/sgp4_propagator.hpp"

namespace cdet {

struct ConjunctionCandidate {
    std::size_t first_index = 0;
    std::size_t second_index = 0;
    double distance_km = 0.0;
};

std::vector<ConjunctionCandidate> screen_conjunctions(
    const std::vector<StateVector>& states,
    double threshold_km);

}  // namespace cdet