#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// Ride struct for storing ride history
struct Ride {
    std::string source;
    std::string destination;
    double distance;
    double fare;
    std::string timestamp;
};

// Driver class - manages driver information and ride history (Linked List simulation)
class Driver {
private:
    int id;
    std::string name;
    double rating;
    std::string currentLocation;
    bool isAvailable;
    std::vector<Ride> rideHistory;

public:
    Driver(int id, const std::string& name, double rating, const std::string& location)
        : id(id), name(name), rating(rating), currentLocation(location), isAvailable(true) {}
    
    int getId() const { return id; }
    std::string getName() const { return name; }
    double getRating() const { return rating; }
    std::string getCurrentLocation() const { return currentLocation; }
    bool getAvailability() const { return isAvailable; }
    const std::vector<Ride>& getRideHistory() const { return rideHistory; }
    
    void setAvailability(bool available) { isAvailable = available; }
    void setCurrentLocation(const std::string& location) { currentLocation = location; }
    void updateRating(double newRating) { rating = newRating; }
    void addRideToHistory(const Ride& ride) { rideHistory.push_back(ride); }
    
    double calculateDistanceTo(double x1, double y1, double x2, double y2) const {
        return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    }
};

// Passenger class
class Passenger {
private:
    int id;
    std::string name;
    std::string pickupLocation;
    std::string dropLocation;
    double rating;

public:
    Passenger(int id, const std::string& name, double rating)
        : id(id), name(name), rating(rating) {}
    
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPickupLocation() const { return pickupLocation; }
    std::string getDropLocation() const { return dropLocation; }
    double getRating() const { return rating; }
    
    void setPickupLocation(const std::string& location) { pickupLocation = location; }
    void setDropLocation(const std::string& location) { dropLocation = location; }
    void updateRating(double newRating) { rating = newRating; }
};

// RideRequest struct
struct RideRequest {
    int passengerId;
    std::string pickupLocation;
    std::string dropLocation;
    std::string timestamp;
    int assignedDriverId;
    bool isCompleted;
    
    RideRequest(int pId, const std::string& pickup, const std::string& drop)
        : passengerId(pId), pickupLocation(pickup), dropLocation(drop),
          assignedDriverId(-1), isCompleted(false) {}
};

// RideRequestQueue - Queue data structure for managing ride requests
class RideRequestQueue {
private:
    std::queue<RideRequest> requests;

public:
    void enqueue(const RideRequest& request) {
        requests.push(request);
    }
    
    RideRequest dequeue() {
        RideRequest req = requests.front();
        requests.pop();
        return req;
    }
    
    bool isEmpty() const {
        return requests.empty();
    }
    
    int size() const {
        return requests.size();
    }
};

// Graph class - represents the zoo map with locations and routes
class Graph {
private:
    std::map<std::string, std::vector<std::pair<std::string, double>>> adjacencyList;
    std::map<std::string, std::pair<double, double>> coordinates;

public:
    void addLocation(const std::string& location, double x, double y) {
        coordinates[location] = {x, y};
        if (adjacencyList.find(location) == adjacencyList.end()) {
            adjacencyList[location] = {};
        }
    }
    
    void addEdge(const std::string& from, const std::string& to, double distance) {
        adjacencyList[from].push_back({to, distance});
        adjacencyList[to].push_back({from, distance});
    }
    
    std::vector<std::string> dijkstraShortestPath(const std::string& start, const std::string& end) {
        std::map<std::string, double> distances;
        std::map<std::string, std::string> previous;
        std::map<std::string, bool> visited;
        
        for (const auto& loc : adjacencyList) {
            distances[loc.first] = std::numeric_limits<double>::max();
            visited[loc.first] = false;
        }
        
        distances[start] = 0;
        
        for (size_t i = 0; i < adjacencyList.size(); ++i) {
            std::string current;
            double minDist = std::numeric_limits<double>::max();
            
            for (const auto& loc : distances) {
                if (!visited[loc.first] && loc.second < minDist) {
                    minDist = loc.second;
                    current = loc.first;
                }
            }
            
            if (minDist == std::numeric_limits<double>::max()) break;
            
            visited[current] = true;
            
            for (const auto& neighbor : adjacencyList[current]) {
                double newDist = distances[current] + neighbor.second;
                if (newDist < distances[neighbor.first]) {
                    distances[neighbor.first] = newDist;
                    previous[neighbor.first] = current;
                }
            }
        }
        
        std::vector<std::string> path;
        std::string current = end;
        while (!current.empty()) {
            path.push_back(current);
            if (previous.find(current) != previous.end()) {
                current = previous[current];
            } else {
                break;
            }
        }
        
        std::reverse(path.begin(), path.end());
        return path;
    }
    
    std::vector<std::string> bfs(const std::string& start) {
        std::vector<std::string> result;
        std::map<std::string, bool> visited;
        std::queue<std::string> q;
        
        q.push(start);
        visited[start] = true;
        
        while (!q.empty()) {
            std::string node = q.front();
            q.pop();
            result.push_back(node);
            
            for (const auto& neighbor : adjacencyList[node]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        
        return result;
    }
    
    void dfs(const std::string& node, std::vector<std::string>& visited, std::vector<std::string>& result) {
        visited.push_back(node);
        result.push_back(node);
        
        for (const auto& neighbor : adjacencyList[node]) {
            if (std::find(visited.begin(), visited.end(), neighbor.first) == visited.end()) {
                dfs(neighbor.first, visited, result);
            }
        }
    }
    
    std::vector<std::string> dfsTraversal(const std::string& start) {
        std::vector<std::string> visited;
        std::vector<std::string> result;
        dfs(start, visited, result);
        return result;
    }
    
    double getDistance(const std::string& from, const std::string& to) const {
        for (const auto& neighbor : adjacencyList.at(from)) {
            if (neighbor.first == to) {
                return neighbor.second;
            }
        }
        return -1;
    }
    
    bool hasLocation(const std::string& location) const {
        return adjacencyList.find(location) != adjacencyList.end();
    }
};

// ============================================================================
// ALGORITHMS
// ============================================================================

class Sorting {
public:
    static void merge(std::vector<Driver*>& drivers, int left, int mid, int right,
                     const std::vector<double>& distances) {
        std::vector<Driver*> temp;
        int i = left, j = mid + 1;
        
        while (i <= mid && j <= right) {
            if (distances[i] <= distances[j]) {
                temp.push_back(drivers[i++]);
            } else {
                temp.push_back(drivers[j++]);
            }
        }
        
        while (i <= mid) temp.push_back(drivers[i++]);
        while (j <= right) temp.push_back(drivers[j++]);
        
        for (int i = left, k = 0; i <= right; ++i, ++k) {
            drivers[i] = temp[k];
        }
    }
    
    static void mergeSort(std::vector<Driver*>& drivers, int left, int right,
                         const std::vector<double>& distances) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(drivers, left, mid, distances);
            mergeSort(drivers, mid + 1, right, distances);
            merge(drivers, left, mid, right, distances);
        }
    }
    
    static int partition(std::vector<Driver*>& drivers, int left, int right,
                        const std::vector<double>& distances) {
        double pivot = distances[right];
        int i = left - 1;
        
        for (int j = left; j < right; ++j) {
            if (distances[j] < pivot) {
                ++i;
                std::swap(drivers[i], drivers[j]);
            }
        }
        
        std::swap(drivers[i + 1], drivers[right]);
        return i + 1;
    }
    
    static void quickSort(std::vector<Driver*>& drivers, int left, int right,
                         const std::vector<double>& distances) {
        if (left < right) {
            int pi = partition(drivers, left, right, distances);
            quickSort(drivers, left, pi - 1, distances);
            quickSort(drivers, pi + 1, right, distances);
        }
    }
};

class Searching {
public:
    static int binarySearch(const std::vector<Driver*>& drivers, int targetId) {
        int left = 0, right = drivers.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (drivers[mid]->getId() == targetId) {
                return mid;
            } else if (drivers[mid]->getId() < targetId) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return -1;
    }
    
    static int linearSearch(const std::vector<Driver*>& drivers, const std::string& name) {
        for (size_t i = 0; i < drivers.size(); ++i) {
            if (drivers[i]->getName() == name) {
                return i;
            }
        }
        return -1;
    }
    
    static std::vector<Driver*> findAvailableDrivers(const std::vector<Driver*>& drivers) {
        std::vector<Driver*> available;
        for (Driver* driver : drivers) {
            if (driver->getAvailability()) {
                available.push_back(driver);
            }
        }
        return available;
    }
};

// ============================================================================
// RIDE SHARING SYSTEM
// ============================================================================

class RideSharingSystem {
private:
    std::vector<Driver*> drivers;
    std::vector<Passenger*> passengers;
    RideRequestQueue rideQueue;
    Graph cityMap;
    std::map<int, int> rideAssignments;

public:
    RideSharingSystem() {}
    
    ~RideSharingSystem() {
        for (Driver* driver : drivers) delete driver;
        for (Passenger* passenger : passengers) delete passenger;
    }
    
    void addDriver(int id, const std::string& name, double rating, const std::string& location) {
        drivers.push_back(new Driver(id, name, rating, location));
    }
    
    Driver* getDriver(int id) {
        int index = Searching::binarySearch(drivers, id);
        return (index != -1) ? drivers[index] : nullptr;
    }
    
    std::vector<Driver*> getAllDrivers() const { return drivers; }
    
    void addPassenger(int id, const std::string& name, double rating) {
        passengers.push_back(new Passenger(id, name, rating));
    }
    
    Passenger* getPassenger(int id) {
        for (Passenger* passenger : passengers) {
            if (passenger->getId() == id) return passenger;
        }
        return nullptr;
    }
    
    void addLocationToMap(const std::string& location, double x, double y) {
        cityMap.addLocation(location, x, y);
    }
    
    void addRouteToMap(const std::string& from, const std::string& to, double distance) {
        cityMap.addEdge(from, to, distance);
    }
    
    void requestRide(int passengerId, const std::string& pickup, const std::string& drop) {
        RideRequest request(passengerId, pickup, drop);
        rideQueue.enqueue(request);
        std::cout << "Ride request queued for passenger " << passengerId << std::endl;
    }
    
    void assignRideToDriver(const RideRequest& request) {
        std::vector<Driver*> availableDrivers = Searching::findAvailableDrivers(drivers);
        
        if (availableDrivers.empty()) {
            std::cout << "No available drivers!" << std::endl;
            return;
        }
        
        std::vector<double> distances(availableDrivers.size());
        for (size_t i = 0; i < availableDrivers.size(); ++i) {
            distances[i] = i;
        }
        
        Driver* bestDriver = availableDrivers[0];
        for (Driver* driver : availableDrivers) {
            if (driver->getRating() > bestDriver->getRating()) {
                bestDriver = driver;
            }
        }
        
        bestDriver->setAvailability(false);
        
        std::vector<std::string> route = findShortestRoute(request.pickupLocation, request.dropLocation);
        double distance = route.size() * 2.5;
        double fare = distance * 10;
        
        Ride ride{request.pickupLocation, request.dropLocation, distance, fare, "2024-01-01"};
        bestDriver->addRideToHistory(ride);
        
        std::cout << "Ride assigned to driver: " << bestDriver->getName() << " (ID: " << bestDriver->getId() << ")" << std::endl;
    }
    
    std::vector<std::string> findShortestRoute(const std::string& from, const std::string& to) {
        return cityMap.dijkstraShortestPath(from, to);
    }
    
    std::vector<Driver*> findNearbyDrivers(const std::string& location, double radius) {
        std::vector<Driver*> nearby;
        for (Driver* driver : drivers) {
            if (driver->getCurrentLocation() == location && driver->getAvailability()) {
                nearby.push_back(driver);
            }
        }
        return nearby;
    }
    
    void displayAllDrivers() const {
        std::cout << "\n=== All Drivers ===" << std::endl;
        for (const Driver* driver : drivers) {
            std::cout << "ID: " << driver->getId() << " | Name: " << driver->getName() 
                      << " | Rating: " << driver->getRating() << " | Location: " << driver->getCurrentLocation() << std::endl;
        }
    }
    
    void displayRideQueue() const {
        std::cout << "\n=== Ride Queue ===" << std::endl;
        std::cout << "Pending requests: " << rideQueue.size() << std::endl;
    }
    
    void displayDriverRideHistory(int driverId) const {
        Driver* driver = const_cast<RideSharingSystem*>(this)->getDriver(driverId);
        if (!driver) {
            std::cout << "Driver not found!" << std::endl;
            return;
        }
        
        std::cout << "\n=== Ride History for " << driver->getName() << " ===" << std::endl;
        const auto& history = driver->getRideHistory();
        for (const auto& ride : history) {
            std::cout << "From: " << ride.source << " To: " << ride.destination 
                      << " | Distance: " << ride.distance << " km | Fare: $" << ride.fare << std::endl;
        }
    }
};

// ============================================================================
// MAIN - DEMONSTRATION
// ============================================================================

int main() {
    RideSharingSystem system;
    
    // Add drivers at zoo locations
    system.addDriver(101, "Ravi", 4.5, "Main Gate");
    system.addDriver(102, "Anita", 4.9, "North Gate");
    system.addDriver(103, "Karan", 4.2, "Reptile House");
    system.addDriver(104, "Priya", 4.7, "Cafeteria");
    system.addDriver(105, "Amit", 4.4, "Restrooms");
    system.addDriver(106, "Neha", 4.8, "Recreation Centre");
    system.addDriver(107, "Vikram", 4.3, "Bird Sanctuary");
    system.addDriver(108, "Sneha", 4.6, "Elephant Enclosure");
    
    // Add passengers
    system.addPassenger(1, "John", 4.8);
    system.addPassenger(2, "Sarah", 4.6);
    
    // Add zoo locations to map
    system.addLocationToMap("Main Gate", 0, 0);
    system.addLocationToMap("North Gate", 5, 5);
    system.addLocationToMap("Reptile House", 10, 5);
    system.addLocationToMap("Cafeteria", 15, 10);
    system.addLocationToMap("Restrooms", 2, 10);
    system.addLocationToMap("Recreation Centre", 8, 8);
    system.addLocationToMap("Bird Sanctuary", 5, 15);
    system.addLocationToMap("Elephant Enclosure", 12, 15);
    
    // Add routes between zoo locations
    system.addRouteToMap("Main Gate", "North Gate", 0.8);
    system.addRouteToMap("Main Gate", "Reptile House", 0.9);
    system.addRouteToMap("North Gate", "Restrooms", 1.2);
    system.addRouteToMap("Reptile House", "Cafeteria", 1.1);
    system.addRouteToMap("Main Gate", "Recreation Centre", 0.6);
    system.addRouteToMap("Recreation Centre", "Restrooms", 1.0);
    system.addRouteToMap("Recreation Centre", "Cafeteria", 0.9);
    system.addRouteToMap("Restrooms", "Bird Sanctuary", 1.3);
    system.addRouteToMap("Cafeteria", "Elephant Enclosure", 1.5);
    system.addRouteToMap("Bird Sanctuary", "Elephant Enclosure", 1.4);
    system.addRouteToMap("North Gate", "Bird Sanctuary", 1.6);
    system.addRouteToMap("Reptile House", "Elephant Enclosure", 1.2);
    
    // Display all drivers
    system.displayAllDrivers();
    
    // Request rides
    system.requestRide(1, "Main Gate", "Cafeteria");
    system.requestRide(2, "North Gate", "Elephant Enclosure");
    
    // Display queue
    system.displayRideQueue();
    
    // Find shortest route
    std::vector<std::string> route = system.findShortestRoute("Main Gate", "Cafeteria");
    std::cout << "\nShortest route from Main Gate to Cafeteria: ";
    for (const auto& location : route) {
        std::cout << location << " -> ";
    }
    std::cout << std::endl;
    
    return 0;
}
