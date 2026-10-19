#include <iostream>
#include <stdexcept>
#include <vector>

#include "analysis/close_approach.hpp"
#include "core.hpp"
#include "ingest/omm.hpp"
#include "propagation/sgp4_propagator.hpp"

int main(int argc, char** argv) {
    std::cout << "conjunction detector "
              << cdet::version() << "\n";

    if (argc < 2) {
        std::cerr << "usage: conjunction <catalog.csv>\n";
        return 1;
    }

    try {
        auto satellites = cdet::load_catalog_csv(argv[1]);

        std::cout << "loaded "
                  << satellites.size()
                  << " satellites\n";

        if (satellites.size() < 2) {
            std::cerr << "need at least two satellites\n";
            return 1;
        }

        // Use a small subset for the first end-to-end run.
        const std::size_t sample_size =
            std::min<std::size_t>(satellites.size(), 100);

        std::vector<std::size_t> candidate_indices;

        for (std::size_t i = 0; i < sample_size; ++i) {
            candidate_indices.push_back(i);
        }

        std::cout << "screening "
                  << candidate_indices.size()
                  << " satellites\n";

        const std::string start_epoch =
            satellites[0].epoch;

        const auto results =
            cdet::find_close_approaches(
                satellites,
                candidate_indices,
                start_epoch,
                6,
                10.0,
                50.0
            );

        std::cout << "close approaches found: "
                  << results.size() << "\n";

        for (const auto& result : results) {
            const auto& first =
                satellites[result.first_index];

            const auto& second =
                satellites[result.second_index];

            std::cout << "\nCONJUNCTION CANDIDATE\n";
            std::cout << "  A: " << first.name
                      << " (NORAD " << first.norad_id << ")\n";
            std::cout << "  B: " << second.name
                      << " (NORAD " << second.norad_id << ")\n";
            std::cout << "  epoch: "
                      << result.epoch << "\n";
            std::cout << "  miss distance: "
                      << result.miss_distance_km
                      << " km\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "error: "
                  << e.what() << "\n";
        return 1;
    }

    return 0;
}