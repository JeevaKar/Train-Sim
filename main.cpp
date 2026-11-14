#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream> 
#include <sstream>
#include <ctime>
#include <iomanip>

enum class TrainType {
    FREIGHT,
    PASSENGER,
    HIGH_SPEED
};

class station {
    public:
        std::string name;
        float latitude;
        float longitude;
        std::vector<station*> connections;

        std::string to_string() const {
            std::string result = "Station: " + name + ", Lat: " + std::to_string(latitude) + ", Long: " + std::to_string(longitude) + ", Connections: ";
            for (const auto& conn : connections) {
                result += conn->name + " ";
            }
            return result;
        }
};

class train {
    public:
        std::string id;
        std::vector<station*> location;
        station* destination;
        int max_speed;
        int optimal_speed;
        TrainType type;
};

int log(const std::string& message) {
    std::ofstream log_file("C:\\Users\\Jekam\\Documents\\03-Projects\\Train Sim\\output.log", std::ios_base::app);
    if (!log_file.is_open()) {
        std::cerr << "Error: Could not open log file." << std::endl;
        return -1;
    }
    const std::time_t now = std::time(nullptr);
    const std::tm* utc_time = std::gmtime(&now);
    if (utc_time == nullptr) {
        std::cerr << "Error: Could not get the current UTC time." << std::endl;
        return -1;
    }
    log_file << std::put_time(utc_time, "%Y-%m-%d %H:%M:%S UTC") << " - " << message << std::endl;
    return 0;
}

std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = value.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = value.find_last_not_of(whitespace);
    return value.substr(first, last - first + 1);
}

std::vector<std::vector<std::string>> read_csv(const std::string& filename, bool has_header = true) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        log("Error: Could not open file " + filename);
        return data;
    }
    std::string line;
    if (has_header) {
        std::getline(file, line); // Skip header line
    }
    while (getline(file, line)) {
        std::vector<std::string> row;
        std::string cell;
        std::istringstream line_stream(line);
        while (std::getline(line_stream, cell, ',')) {
            row.push_back(trim(cell));
        }
        data.push_back(row);
    }
    return data;
}

std::vector<station> read_stations(const std::string& filename, bool has_header = true) {
    std::vector<station> stations;
    std::vector<std::vector<std::string>> pending_connections;
    std::vector<std::vector<std::string>> data = read_csv(filename, has_header);
    int i = 1;
    for (auto& row : data) {
        if (row.size() != 4) {
            log("WARNING: Row " + std::to_string(i) + " does not have 4 columns. Skipping.");
        }
        else {
            station s;
            s.name = row[0];
            s.latitude = std::stof(row[1]);
            s.longitude = std::stof(row[2]);
            std::vector<std::string> connection_names;
            std::istringstream connections_stream(row[3]);
            std::string connection_name;
            while (std::getline(connections_stream, connection_name, ';')) {
                connection_name = trim(connection_name);
                if (!connection_name.empty()) {
                    connection_names.push_back(connection_name);
                }
            }
            stations.push_back(s);
            pending_connections.push_back(connection_names);
        }
        i++;
    }

    auto add_connection = [](station& from, station* to) {
        if (std::find(from.connections.begin(), from.connections.end(), to) == from.connections.end()) {
            from.connections.push_back(to);
        }
    };
    for (std::size_t station_index = 0; station_index < stations.size(); ++station_index) {
        for (const auto& connection_name : pending_connections[station_index]) {
            bool found = false;
            for (std::size_t target_index = 0; target_index < stations.size(); ++target_index) {
                if (stations[target_index].name == connection_name) {
                    add_connection(stations[station_index], &stations[target_index]);
                    add_connection(stations[target_index], &stations[station_index]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                log("WARNING: Connection '" + connection_name + "' for station '" +
                    stations[station_index].name + "' was not found.");
            }
        }
    }
    return stations;
}

std::vector<train> read_trains(const std::string& filename, std::vector<station>& stations, bool has_header = true) {
    std::vector<train> trains;
    std::vector<std::vector<std::string>> data = read_csv(filename, has_header);
    int i = 1;
    for (auto row : data) {
        if (row.size() != 7) {
            log("WARNING: Row " + std::to_string(i) + " does not have 7 columns. Skipping.");
        }
        else {
            train t;
            t.id = row[0];
            for (auto& station : stations) {
                if (station.name == row[1]) {
                    t.location.insert(t.location.begin(), &station);
                }
                if (station.name == row[2]) {
                    t.location.push_back(&station);
                }
                if (station.name == row[3]) {
                    t.destination = &station;
                }
            }
            t.max_speed = std::stoi(row[4]);
            t.optimal_speed = std::stoi(row[5]);
            t.type = TrainType(std::stoi(row[6]));
            trains.push_back(t);
        }
        i++;
    }
    return trains;
}

int main() {
    log("Application started.");
    std::vector<station> stations = read_stations("C:\\Users\\Jekam\\Documents\\03-Projects\\Train Sim\\stations.csv");
    log("Number of stations read from CSV: " + std::to_string(stations.size()));
    for (const auto& current_station : stations) {
        log("Station details: " + current_station.to_string());
    }
    std::vector<train> trains = read_trains("C:\\Users\\Jekam\\Documents\\03-Projects\\Train Sim\\trains.csv", stations);
    log("Number of trains read from CSV: " + std::to_string(trains.size()));
    log("Application exited successfully.");
    return 0;
}