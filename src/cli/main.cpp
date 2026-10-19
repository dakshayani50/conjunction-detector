#include <iostream>
#include <stdexcept>

#include "core.hpp"
#include "ingest/omm.hpp"

int main(int argc, char** argv) {
    std::cout << "conjunction detector " << cdet::version() << "\n";
    if (argc > 1) {
        try {
            auto sats = cdet::load_catalog_csv(argv[1]);
            std::cout << "loaded " << sats.size() << " satellites\n";
            if (!sats.empty()) {
                std::cout << "first: " << sats[0].name << " (NORAD "
                          << sats[0].norad_id << ")\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "error: " << e.what() << "\n";
            return 1;
        }
    }
    return 0;
}