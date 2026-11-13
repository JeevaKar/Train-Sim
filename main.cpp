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
        std::vector<station> connections;
};

class train {
    public:
        std::string id;
        std::vector<station> location;
        station destination;
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

std::vector<station> read_stations(const std::string& filename) {
    std::vector<station> stations;
    station s1;
    s1.name = "Station A";
    station s2;
    s2.name = "Station B";
    s2.connections.push_back(s1);
    station s3;
    s3.name = "Station C";
    s3.connections.push_back(s2);
    stations.push_back(s1);
    stations.push_back(s2);
    stations.push_back(s3);
    return stations;
}

std::vector<train> read_trains(const std::string& filename) {
    std::vector<train> trains;
    train t1;
    t1.id = "T123";
    t1.location = {station{"Station A"}, station{"Station B"}};
    t1.destination = station{"Station C"};
    t1.max_speed = 120;
    t1.optimal_speed = 100;
    t1.type = TrainType::PASSENGER;
    trains.push_back(t1);
    return trains;
}

int main() {
    log("Application started.");
    std::vector<std::vector<std::string>> stations_data = read_csv("C:\\Users\\Jekam\\Documents\\03-Projects\\Train Sim\\stations.csv");
    log("Number of stations read from CSV: " + std::to_string(stations_data.size()));
    log("Application exited successfully.");
    return 0;
}