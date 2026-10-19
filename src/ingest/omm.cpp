#include "ingest/omm.hpp"

#include <fstream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace cdet {
namespace {

// Split one CSV line into fields. Commas inside quotes don't split.
std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> fields;
    std::string cur;
    bool in_quotes = false;

    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            fields.push_back(cur);
            cur.clear();
        } else if (c != '\r') {
            cur += c;
        }
    }

    fields.push_back(cur);
    return fields;
}

// Build a key from the orbital elements used by SGP4.
std::string orbital_key(const Satellite& s) {
    return s.epoch + "|" +
           std::to_string(s.mean_motion) + "|" +
           std::to_string(s.eccentricity) + "|" +
           std::to_string(s.inclination) + "|" +
           std::to_string(s.raan) + "|" +
           std::to_string(s.arg_perigee) + "|" +
           std::to_string(s.mean_anomaly) + "|" +
           std::to_string(s.bstar) + "|" +
           std::to_string(s.mean_motion_dot) + "|" +
           std::to_string(s.mean_motion_ddot);
}

}  // namespace

std::vector<Satellite> load_catalog_csv(const std::string& path) {
    std::ifstream in(path);

    if (!in) {
        throw std::runtime_error("cannot open " + path);
    }

    std::string line;

    if (!std::getline(in, line)) {
        throw std::runtime_error("empty file: " + path);
    }

    // Map each column name to its position.
    const std::vector<std::string> header = split_csv_line(line);

    std::map<std::string, std::size_t> col;

    for (std::size_t i = 0; i < header.size(); ++i) {
        col[header[i]] = i;
    }

    for (const char* name :
         {
             "OBJECT_NAME",
             "OBJECT_ID",
             "EPOCH",
             "MEAN_MOTION",
             "ECCENTRICITY",
             "INCLINATION",
             "RA_OF_ASC_NODE",
             "ARG_OF_PERICENTER",
             "MEAN_ANOMALY",
             "NORAD_CAT_ID",
             "BSTAR",
             "MEAN_MOTION_DOT",
             "MEAN_MOTION_DDOT"
         }) {

        if (!col.count(name)) {
            throw std::runtime_error(
                std::string("missing column: ") + name);
        }
    }

    std::vector<Satellite> sats;

    // Prevent identical orbital records from being treated
    // as separate conjunction candidates.
    std::set<std::string> seen_orbits;

    std::size_t line_no = 1;

    while (std::getline(in, line)) {
        ++line_no;

        if (line.empty() || line == "\r") {
            continue;
        }

        const std::vector<std::string> f = split_csv_line(line);

        if (f.size() < header.size()) {
            throw std::runtime_error(
                "line " + std::to_string(line_no) +
                ": too few fields");
        }

        try {
            Satellite s;

            s.name = f[col["OBJECT_NAME"]];
            s.intl_id = f[col["OBJECT_ID"]];
            s.epoch = f[col["EPOCH"]];

            s.norad_id =
                std::stoi(f[col["NORAD_CAT_ID"]]);

            s.mean_motion =
                std::stod(f[col["MEAN_MOTION"]]);

            s.eccentricity =
                std::stod(f[col["ECCENTRICITY"]]);

            s.inclination =
                std::stod(f[col["INCLINATION"]]);

            s.raan =
                std::stod(f[col["RA_OF_ASC_NODE"]]);

            s.arg_perigee =
                std::stod(f[col["ARG_OF_PERICENTER"]]);

            s.mean_anomaly =
                std::stod(f[col["MEAN_ANOMALY"]]);

            s.bstar =
                std::stod(f[col["BSTAR"]]);

            s.mean_motion_dot =
                std::stod(f[col["MEAN_MOTION_DOT"]]);

            s.mean_motion_ddot =
                std::stod(f[col["MEAN_MOTION_DDOT"]]);

            const std::string key = orbital_key(s);

            if (!seen_orbits.insert(key).second) {
                continue;
            }

            sats.push_back(std::move(s));

        } catch (const std::logic_error&) {
            throw std::runtime_error(
                "line " + std::to_string(line_no) +
                ": bad number");
        }
    }

    return sats;
}

}  // namespace cdet