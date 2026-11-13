#include <iostream>
#include <string>
#include <vector>

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
    std::vector<train> trains = read_trains("trains.txt");
    std::cout << trains[0].id << " is traveling from " << trains[0].location[0].name << " to " << trains[0].destination.name << std::endl;
}