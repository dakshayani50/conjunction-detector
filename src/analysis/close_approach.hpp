#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "ingest/omm.hpp"

namespace cdet {

struct CloseApproach {
    std::size_t first_index = 0;
    std::size_t second_index = 0;

    std::string epoch;
    double miss_distance_km = 0.0;
};

std::vector<CloseApproach> find_close_approaches(
    const std::vector<Satellite>& satellites,
    const std::vector<std::size_t>& candidate_indices,
    const std::string& start_epoch,
    int steps,
    double step_minutes,
    double threshold_km);

}  // namespace cdet