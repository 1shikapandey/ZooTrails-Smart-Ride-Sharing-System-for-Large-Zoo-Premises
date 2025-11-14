#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include <vector>

using namespace std;

#define MAX_DRIVERS 50
#define MAX_PASSENGERS 50
#define MAX_RIDES 100

// Ride info (Array element)
struct Ride {
    string source, destination, timestamp;
};

// Driver info (Array + Linked list simulation)
class Driver {
public:
    int id;
    string name, currentLocation;
    double rating;
    bool isAvailable;
    Ride rideHistory[MAX_RIDES];
    int rideCount;

    Driver() {}
    Driver(int i, string n, double r, string loc) {
        id = i; name = n; rating = r; currentLocation = loc;
        isAvailable = true; rideCount = 0;
    }
    void addRide(Ride ride) { if (rideCount < MAX_RIDES) rideHistory[rideCount++] = ride; }
};

// Passenger info (Array element)
class Passenger {
public:
    int id;
    string name, pickup, drop;
    Passenger() {}
    Passenger(int i, string n) { id = i; name = n; }
};

// Queue for ride requests
struct RideRequest {
    int passengerId;
    string pickup, drop, timestamp;
    int assignedDriverId;
    bool completed;
    RideRequest() {}
    RideRequest(int pId, string p, string d) {
        passengerId = pId; pickup = p; drop = d;
        assignedDriverId = -1; completed = false;
    }
};

class RideQueue {
private:
    queue<RideRequest> q;
public:
    void enqueue(RideRequest r) { q.push(r); }
    bool empty() const { return q.empty(); }
    RideRequest dequeue() { RideRequest r = q.front(); q.pop(); return r; }
    int size() const { return q.size(); }
};

// Graph for locations
class Graph {
private:
    map<string, vector<pair<string, double>>> adj;
public:
    void addEdge(string u, string v, double d) {
        adj[u].push_back({v, d});
        adj[v].push_back({u, d});
    }

    // Dijkstra (Graph + Array-like maps)
    vector<string> dijkstra(string start, string end) {
        map<string, double> dist;
        map<string, string> prev;
        map<string, bool> visited;

        for (auto& node : adj) {
            dist[node.first] = numeric_limits<double>::max();
            visited[node.first] = false;
        }
        dist[start] = 0;

        for (size_t i = 0; i < adj.size(); ++i) {
            string u;
            double minDist = numeric_limits<double>::max();
            for (auto& p : dist)
                if (!visited[p.first] && p.second < minDist) {
                    minDist = p.second;
                    u = p.first;
                }
            if (minDist == numeric_limits<double>::max()) break;
            visited[u] = true;

            for (auto& nb : adj[u]) {
                double newDist = dist[u] + nb.second;
                if (newDist < dist[nb.first]) {
                    dist[nb.first] = newDist;
                    prev[nb.first] = u;
                }
            }
        }

        vector<string> path;
        string curr = end;
        while (!curr.empty()) {
            path.push_back(curr);
            if (prev.find(curr) != prev.end()) curr = prev[curr];
            else break;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

// Sorting + Searching (Array-based)
class Util {
public:
    static void quickSort(Driver* arr[], double dist[], int l, int r) {
        if (l < r) {
            double pivot = dist[r];
            int i = l - 1;
            for (int j = l; j < r; ++j) {
                if (dist[j] < pivot) {
                    i++;
                    swap(dist[i], dist[j]);
                    swap(arr[i], arr[j]);
                }
            }
            swap(dist[i + 1], dist[r]);
            swap(arr[i + 1], arr[r]);
            quickSort(arr, dist, l, i);
            quickSort(arr, dist, i + 2, r);
        }
    }

    static int binarySearch(Driver* arr[], int n, int id) {
        int l = 0, r = n - 1;
        while (l <= r) {
            int m = (l + r) / 2;
            if (arr[m]->id == id) return m;
            if (arr[m]->id < id) l = m + 1;
            else r = m - 1;
        }
        return -1;
    }
};

// Ride-sharing system (Array + Queue + Graph)
class RideSharingSystem {
private:
    Driver* drivers[MAX_DRIVERS];
    Passenger* passengers[MAX_PASSENGERS];
    int driverCount, passengerCount;
    RideQueue rideQueue;
    Graph cityMap;

public:
    RideSharingSystem() { driverCount = passengerCount = 0; }

    void addDriver(int id, string name, double rating, string loc) {
        if (driverCount < MAX_DRIVERS)
            drivers[driverCount++] = new Driver(id, name, rating, loc);
    }

    void addPassenger(int id, string name) {
        if (passengerCount < MAX_PASSENGERS)
            passengers[passengerCount++] = new Passenger(id, name);
    }

    Driver* getDriver(int id) {
        int idx = Util::binarySearch(drivers, driverCount, id);
        return (idx != -1) ? drivers[idx] : nullptr;
    }

    void addRoute(string u, string v, double d) { cityMap.addEdge(u, v, d); }

    void requestRide(int pId, string pickup, string drop) {
        rideQueue.enqueue(RideRequest(pId, pickup, drop));
        cout << "Ride queued for passenger " << pId << endl;
    }

    void assignRide() {
        if (rideQueue.empty()) {
            cout << "No pending rides.\n";
            return;
        }

        RideRequest req = rideQueue.dequeue();
        if (driverCount == 0) {
            cout << "No drivers.\n";
            return;
        }

        Driver* best = drivers[0];
        for (int i = 1; i < driverCount; ++i)
            if (drivers[i]->isAvailable && drivers[i]->rating > best->rating)
                best = drivers[i];

        best->isAvailable = false;
        vector<string> route = cityMap.dijkstra(req.pickup, req.drop);
        Ride ride = {req.pickup, req.drop, "2024-01-01"};
        best->addRide(ride);

        cout << "Ride assigned to " << best->name << " (ID: " << best->id << ")\n";
    }

    void displayDrivers() {
        cout << "\nDrivers:\n";
        for (int i = 0; i < driverCount; ++i)
            cout << "ID: " << drivers[i]->id << " | Name: " << drivers[i]->name
                 << " | Rating: " << drivers[i]->rating
                 << " | Loc: " << drivers[i]->currentLocation << endl;
    }

    void displayQueue() { cout << "\nPending rides: " << rideQueue.size() << endl; }

    void displayHistory(int id) {
        Driver* d = getDriver(id);
        if (!d) { cout << "Driver not found.\n"; return; }
        cout << "\nRide history of " << d->name << ":\n";
        for (int i = 0; i < d->rideCount; ++i)
            cout << "From " << d->rideHistory[i].source << " To " << d->rideHistory[i].destination << endl;
    }
};

int main() {
    RideSharingSystem sys;

    sys.addDriver(101, "Ravi", 4.5, "Main Gate");
    sys.addDriver(102, "Anita", 4.9, "North Gate");
    sys.addDriver(103, "Karan", 4.2, "Reptile House");

    sys.addPassenger(1, "John");
    sys.addPassenger(2, "Sarah");

    sys.addRoute("Main Gate", "North Gate", 0.8);
    sys.addRoute("Main Gate", "Reptile House", 0.9);
    sys.addRoute("North Gate", "Reptile House", 1.2);

    int choice;
    do {
        cout << "\n1. Display Drivers\n2. Request Ride\n3. Assign Ride\n4. Show Queue\n5. Driver History\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) sys.displayDrivers();
        else if (choice == 2) {
            int pid; string p, d;
            cout << "Passenger ID: "; cin >> pid;
            cout << "Pickup: "; cin >> p;
            cout << "Drop: "; cin >> d;
            sys.requestRide(pid, p, d);
        }
        else if (choice == 3) sys.assignRide();
        else if (choice == 4) sys.displayQueue();
        else if (choice == 5) {
            int id; cout << "Driver ID: "; cin >> id;
            sys.displayHistory(id);
        }
    } while (choice != 0);

    return 0;
}
