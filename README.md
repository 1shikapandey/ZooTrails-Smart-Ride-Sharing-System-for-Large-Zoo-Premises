# 🦓 ZooTrails: Smart Ride-Sharing System for Large Zoo Premises

**ZooTrails** is a **C++-based ride-sharing simulation** designed for large zoological parks like *The Rajiv Gandhi Zoological Park*, where attractions are spread across vast areas. The system models a network of locations inside the zoo and intelligently manages ride requests, driver assignments, and optimal routes using multiple **data structures and algorithms**.

---

## 🚀 Overview

ZooTrails simulates a **smart mobility management system** that connects passengers (visitors) with available drivers (electric vehicle operators) across a zoo map.
It leverages **graph algorithms** for route optimization, **queues** for ride management, and **sorting/searching** for efficient driver selection.

---

## 🧩 Core Features

* **Dynamic Ride Request Handling** using a queue-based system.
* **Driver & Passenger Management** via object-oriented class structures.
* **Shortest Path Computation** using Dijkstra’s algorithm on a weighted graph.
* **Graph Traversal** using BFS and DFS for exploration and connectivity checks.
* **Driver Sorting and Searching** with merge sort, quick sort, and binary search.
* **Ride History and Ratings System** for drivers.
* **Extensible Architecture** designed for future real-world deployment.

---

## 🧠 Data Structures Used

| Data Structure                          | Used In                                | Purpose                                           |
| --------------------------------------- | -------------------------------------- | ------------------------------------------------- |
| **Array / Vector**                      | Driver & Passenger Lists, Ride History | To store and manage multiple records dynamically. |
| **Queue**                               | RideRequestQueue                       | To handle ride requests in FIFO order.            |
| **Map (Adjacency List)**                | Graph Representation                   | To model zoo locations and paths efficiently.     |
| **Linked List (Simulation via Vector)** | Ride History                           | To maintain sequential driver ride history.       |
| **Graph**                               | City/Zoo Map                           | To represent routes between different zoo points. |

---

## ⚙️ Algorithms Implemented

| Algorithm                      | Application        | Description                                                   |
| ------------------------------ | ------------------ | ------------------------------------------------------------- |
| **Dijkstra’s Algorithm**       | Route Optimization | Finds the shortest path between pickup and drop locations.    |
| **Breadth-First Search (BFS)** | Connectivity Check | Traverses all reachable locations from a given node.          |
| **Depth-First Search (DFS)**   | Path Exploration   | Recursively explores all connected paths.                     |
| **Merge Sort**                 | Driver Sorting     | Sorts drivers by computed distances efficiently (O(n log n)). |
| **Quick Sort**                 | Driver Sorting     | Alternative fast sorting technique (average O(n log n)).      |
| **Binary Search**              | Driver Lookup      | Searches drivers by ID efficiently (O(log n)).                |
| **Linear Search**              | Driver Lookup      | Searches drivers by name.                                     |

---

## 🧱 System Architecture

**Class Structure:**

* `Driver` – encapsulates driver data, availability, rating, and ride history.
* `Passenger` – manages passenger details and ride preferences.
* `RideRequest` – represents individual ride requests.
* `RideRequestQueue` – queue-based management of ride requests.
* `Graph` – models zoo locations and routes.
* `Sorting` & `Searching` – utility classes implementing algorithmic operations.
* `RideSharingSystem` – integrates all components into a functioning simulation.

---

## 🧮 Workflow

1. **Initialization**

   * Add drivers, passengers, and zoo locations.
   * Define routes (edges) between locations.

2. **Ride Request**

   * Passenger submits a ride request.
   * Request added to queue.

3. **Ride Assignment**

   * System fetches available drivers.
   * Drivers are sorted and best match assigned (based on rating or proximity).

4. **Path Calculation**

   * Dijkstra’s algorithm finds the optimal route.
   * Distance and fare computed dynamically.

5. **Completion**

   * Ride stored in driver’s history.
   * Driver marked available again.

---

## 🧰 Technologies & Concepts

* **Language:** C++ (OOP, STL)
* **Paradigm:** Object-Oriented Programming
* **Core Concepts:** Encapsulation, Abstraction, Composition
* **STL Containers:** `vector`, `map`, `queue`
* **Algorithmic Design:** Sorting, Searching, Graph Theory, Pathfinding

---

## 🧩 Sample Zoo Map

The system models a realistic zoo layout with nodes and edges:

```
Main Gate --0.8--> North Gate --1.6--> Bird Sanctuary --1.4--> Elephant Enclosure
   |                               
   0.9
   |
Reptile House --1.1--> Cafeteria --1.5--> Elephant Enclosure
   |
   1.2
   |
Restrooms
```

---

## 📈 Example Output

```
=== All Drivers ===
ID: 101 | Name: Ravi | Rating: 4.5 | Location: Main Gate
ID: 102 | Name: Anita | Rating: 4.9 | Location: North Gate
...

Ride request queued for passenger 1
Ride request queued for passenger 2

Shortest route from Main Gate to Cafeteria:
Main Gate -> Recreation Centre -> Cafeteria ->
```

---

## 📚 Complexity Analysis

| Operation            | Data Structure       | Time Complexity |
| -------------------- | -------------------- | --------------- |
| Enqueue/Dequeue Ride | Queue                | O(1)            |
| Search Driver by ID  | Binary Search        | O(log n)        |
| Sort Drivers         | Merge/Quick Sort     | O(n log n)      |
| Find Shortest Path   | Dijkstra’s Algorithm | O(V²)           |
| Traverse Locations   | BFS / DFS            | O(V + E)        |

---

## 🔮 Future Enhancements

* Integrate real GPS or coordinate data for live mapping.
* Implement priority-based driver selection (distance + rating).
* Add a persistence layer (file or database) for ride history.
* Build a visualization dashboard (C++ + Qt / Web-based frontend).



