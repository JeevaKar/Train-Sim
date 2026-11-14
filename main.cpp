#include <iostream>
#include <string>
#include <vector>
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
        std::vector<station*> connections;
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
            row.push_back(cell);
        }
        data.push_back(row);
    }
    return data;
}

std::vector<station> read_stations(const std::string& filename, bool has_header = true) {
    std::vector<station> stations;
    std::vector<std::vector<std::string>> data = read_csv(filename, has_header);
    for (auto& row : data) {
        if (row.size() != 2) {
            log("WARNING: Row " + std::to_string(&row - &data[0]) + " does not have 2 columns. Skipping.");
        }
        else {
            station s;
            s.name = row[0];
            std::istringstream connections_stream(row[1]);
            std::string connection_name;
            while (std::getline(connections_stream, connection_name, ';')) {
                for (auto& existing_station : stations) {
                    if (existing_station.name == connection_name) {
                        s.connections.push_back(&existing_station);
                        existing_station.connections.push_back(&s);
                        break;
                    }
                }
            }
            stations.push_back(s);
        }
    }
    return stations;
}

std::vector<train> read_trains(const std::string& filename, std::vector<station>& stations, bool has_header = true) {
    std::vector<train> trains;
    std::vector<std::vector<std::string>> data = read_csv(filename, has_header);
    for (auto row : data) {
        if (row.size() != 7) {
            log("WARNING: Row " + std::to_string(&row - &data[0]) + " does not have 7 columns. Skipping.");
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
    }
    return trains;
}

int main() {
    log("Application started.");
    std::vector<station> stations = read_stations("C:\\Users\\Jekam\\Documents\\03-Projects\\Train Sim\\stations.csv");
    log("Number of stations read from CSV: " + std::to_string(stations.size()));
    std::vector<train> trains = read_trains("C:\\Users\\Jekam\\Documents\\03-Projects\\Train Sim\\trains.csv", stations);
    log("Number of trains read from CSV: " + std::to_string(trains.size()));
    log("Application exited successfully.");
    return 0;
}