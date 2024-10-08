#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <queue>
#include <climits>
#include <algorithm>

// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

using namespace std;

// Structure to represent a user
struct User
{
    string username;
    string password;
};

// Structure to represent an item for the knapsack problem
struct Item
{
    string name;
    int weight;
    int value;
};

// Structure to represent a city and its connections
struct City
{
    string name;
    unordered_map<string, int> connections; // Connected cities and their distances
};

// Function prototypes
void registerUser();
bool loginUser(User &loggedInUser);
int knapsack(int capacity, const vector<Item> &items);
void saveKnapsackResult(const string &username, int capacity, int max_value);
void displayKnapsackResult(const string &username);
void addCityConnection(const string &city1, const string &city2, int distance);
void dijkstraShortestPath(const string &username);
void displayShortestPath(const string &username);
void RailAndRoadKnapSack(const string &username);
void displayRailandRoadKnapsackResult(const string &username);
// Global variables
unordered_map<string, City> citiesMap; // Map to store cities and their connections

// Function to register a new user
void registerUser()
{
    ofstream outfile("users.txt", ios::app);
    User newUser;

    cout << "Enter username: ";
    cin >> newUser.username;

    cout << "Enter password: ";
    cin >> newUser.password;

    outfile << newUser.username << " " << newUser.password << endl;

    cout << "User registered successfully!" << endl
         << endl;
    outfile.close();
}

// Function to login a user
bool loginUser(User &loggedInUser)
{
    ifstream infile("users.txt");
    User existingUser;
    string username, password;

    cout << "Enter username";
    cin >> username;

    cout << "Enter password: ";
    cin >> password;

    while (infile >> existingUser.username >> existingUser.password)
    {
        if (username == existingUser.username && password == existingUser.password)
        {
            cout << "Login successful!" << endl
                 << endl;
            loggedInUser = existingUser; // Store the logged-in user
            infile.close();
            return true;
        }
    }

    cout << "Invalid username or password." << endl
         << endl;
    infile.close();
    return false;
}

// Function to solve the knapsack problem
int knapsack(int capacity, const vector<Item> &items)
{
    int n = items.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i)
    {
        for (int w = 1; w <= capacity; ++w)
        {
            if (items[i - 1].weight <= w)
            {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - items[i - 1].weight] + items[i - 1].value);
            }
            else
            {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][capacity];
}

// Function to save knapsack result to a file
void saveKnapsackResult(const string &username, int capacity, int max_value)
{
    ofstream outfile(username + "_knapsack.txt");
    outfile << "Capacity: " << capacity << endl;
    outfile << "Maximum value: " << max_value << endl;
    outfile.close();
}

// Function to display knapsack result
void displayKnapsackResult(const string &username)
{
    ifstream infile(username + "_knapsack.txt");
    string line;

    if (infile.is_open())
    {
        cout << "Knapsack via airways Result for User: " << username << endl;
        while (getline(infile, line))
        {
            cout << line << endl
                 << endl;
        }
        infile.close();
    }
    else
    {
        cout << "No result found for Knapsack via airways for this user: " << username << endl
             << endl;
    }
}

// Function to add a connection between two cities
// The below function adds bidirectional connection between 2 cities
void addCityConnection(const string &city1, const string &city2, int distance)
{
    citiesMap[city1].connections[city2] = distance;
    citiesMap[city2].connections[city1] = distance;
}

// Dijkstra's algorithm for finding the shortest path between two cities
void dijkstraShortestPath(const string &username)
{
    unordered_map<string, int> distance; // Shortest Distance from startCity to other cities
    unordered_map<string, string> prev;  // Previous city in the shortest path or preceding city in the shortest path
    // priortize city on the basis of the distance in the reverse order i.e. citites with shorter distance are given higher priority
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    // User input for cities and connections
    int numCities, numConnections;
    cout << "Enter the number of cities: ";
    cin >> numCities;
    cout << "Enter the number of connections: ";
    cin >> numConnections;

    for (int i = 0; i < numCities; ++i)
    {
        string cityName;
        cout << "Enter city " << i + 1 << " name: ";
        cin >> cityName;
        citiesMap[cityName].name = cityName; // Add city to the map
    }

    for (int i = 0; i < numConnections; ++i)
    {
        string city1, city2;
        int distance;
        cout << "Enter connection " << i + 1 << " (City1 City2 Distance): ";
        cin >> city1 >> city2 >> distance;
        addCityConnection(city1, city2, distance); // Add connection between cities
    }

    string startCity, endCity;
    cout << "Enter the starting city: ";
    cin >> startCity;
    cout << "Enter the destination city: ";
    cin >> endCity;

    for (const auto &city : citiesMap)
    {
        distance[city.first] = INT_MAX; // Initially setting shortest distance from starting city to each city as infinite in the distance map
        prev[city.first] = "";          // previous city to next city's has been empty string initially
    }

    // For example
    // distance[A] = INT_MAX;  // Initially, the shortest distance from the starting city to A is unknown
    // distance[B] = INT_MAX;  // Similarly for B
    // distance[C] = INT_MAX;  // Similarly for C

    // prev[A] = "";  // No shortest path to A discovered yet
    // prev[B] = "";  // No shortest path to B discovered yet
    // prev[C] = "";  // No shortest path to C discovered yet

    distance[startCity] = 0; // distance from starting city to itself is set as 0
    pq.push({0, startCity});

    while (!pq.empty())
    {
        string currentCity = pq.top().second; // this line adds name of the city with the shortest dist. from the starting city
        pq.pop();                             // city with shortest distance is removed from the priority queue

        for (const auto &neighbor : citiesMap[currentCity].connections)
        {
            int newDist = distance[currentCity] + neighbor.second;
            if (newDist < distance[neighbor.first])
            {
                distance[neighbor.first] = newDist;
                prev[neighbor.first] = currentCity;
                pq.push({newDist, neighbor.first});
            }
        }
    }

    // Output the shortest path
    vector<string> shortestPath;
    string current = endCity;
    while (!prev[current].empty())
    {
        shortestPath.push_back(current);
        current = prev[current];
    }
    shortestPath.push_back(startCity);

    // Save shortest path to a file
    ofstream outfile(username + "_shortest_path.txt");
    outfile << "Shortest path from " << startCity << " to " << endCity << ": ";
    for (int i = shortestPath.size() - 1; i >= 0; --i)
    {
        outfile << shortestPath[i];
        if (i > 0)
            outfile << " -> ";
    }
    outfile << " (Distance: " << distance[endCity] << " units)" << endl;
    outfile.close();

    cout << "Shortest path from " << startCity << " to " << endCity << " saved to file." << endl;
}

// Function to display the saved shortest path from file
void displayShortestPath(const string &username)
{
    ifstream infile(username + "_shortest_path.txt");
    string line;

    if (infile.is_open())
    {
        cout << "Shortest path saved for User: " << username << endl;
        while (getline(infile, line))
        {
            cout << line << endl;
        }
        infile.close();
    }
    else
    {
        cout << "No saved shortest path found for this user." << endl;
    }
}

// Define maximum number of nodes in the network
#define MAX_NODES 100

// Define infinity as maximum possible flow
#define INF 1000000000

// Define a structure to represent an edge in the network
struct Edge
{
    int v, capacity, reverseEdge;
};

// Global variables
vector<Edge> graph[MAX_NODES];
int parent[MAX_NODES];

// Function to add an edge to the network
void addEdge(int u, int v, int capacity)
{
    Edge edge1 = {v, capacity, static_cast<int>(graph[v].size())};
    Edge edge2 = {u, 0, static_cast<int>(graph[u].size())};
    graph[u].push_back(edge1);
    graph[v].push_back(edge2);
}

// Function to find an augmenting path using BFS
bool bfs(int source, int sink, int nodes)
{
    bool visited[MAX_NODES];
    memset(visited, false, sizeof(visited));
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (Edge &e : graph[u])
        {
            int v = e.v;
            if (!visited[v] && e.capacity > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return visited[sink];
}

// Function to find the maximum flow in the network
int fordFulkerson(int source, int sink, int nodes)
{
    int maxFlow = 0;

    while (bfs(source, sink, nodes))
    {
        int pathFlow = INF;
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            for (Edge &e : graph[u])
            {
                if (e.v == v)
                {
                    pathFlow = min(pathFlow, e.capacity);
                    break;
                }
            }
        }
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            for (Edge &e : graph[u])
            {
                if (e.v == v)
                {
                    e.capacity -= pathFlow;
                    graph[e.v][e.reverseEdge].capacity += pathFlow;
                    break;
                }
            }
        }
        maxFlow += pathFlow;
    }

    return maxFlow;
}

// Function to print the path for the maximum flow
void printPath(int sink)
{
    vector<int> path;
    for (int v = sink; v != -1; v = parent[v])
    {
        path.push_back(v);
    }
    cout << "Path for maximum flow: ";
    for (int i = path.size() - 1; i >= 0; --i)
    {
        cout << path[i];
        if (i > 0)
            cout << " -> ";
    }
    cout << endl;
}

void main_fordfulkersion()
{
    int nodes, edges;
    cout << "Enter the number of nodes and edges: ";
    cin >> nodes >> edges;

    for (int i = 0; i < edges; i++)
    {
        int u, v, capacity;
        cout << "Enter source, destination, and capacity of edge " << i + 1 << ": ";
        cin >> u >> v >> capacity;
        addEdge(u, v, capacity);
    }

    int source, sink;
    cout << "Enter source and sink nodes: ";
    cin >> source >> sink;

    int maxFlow = fordFulkerson(source, sink, nodes);
    cout << "Maximum flow in the network is: " << maxFlow << endl;

    printPath(sink);
}
void RailAndRoadKnapSack(const string &username) // function to obtain maximum possible value using dynamic programming (tabulation)
{
    int W, n;                                      // declaring of variables for storing maximum possible weight and total number of items
    cout << "Input weight limit of the vehicle: "; // Message for user display
    cin >> W;                                      // Inputting Maximum Possible Weight
    cout << "Input number of items: ";             // Message for user display
    cin >> n;                                      // Inputting Number of Different Items
    int wt[n];                                     // Initializing all elements of the array with zero
    int val[n];                                    // Initializing all elements of the array with zero
    cout << "Input weight of the items: ";         // Message for user display
    for (int i = 0; i < n; i++)                    // loop for inputting weights of items
        cin >> wt[i];                              // inputting weights of items
    cout << "Input value of the items: ";          // Message for user display
    for (int i = 0; i < n; i++)                    // loop for inputting values of items
        cin >> val[i];                             // inputting value of items
    int i, w;                                      // declaring the index variable i and weight considered variable w
    int K[n + 1][W + 1];                           // declaring 2D array for dynamic programming using tabulation and initializing it with zero
    for (i = 0; i <= n; i++)                       // outer loop for traversing different rows of the 2D array
    {
        for (w = 0; w <= W; w++) // inner loop for traversing different columns of a particular row of the 2D array
        {
            if (i == 0 || w == 0)                                                                                                            // case when no item or no weight is considered
                K[i][w] = 0;                                                                                                                 // initializing first row and first column with zero
            else if (wt[i - 1] <= w)                                                                                                         // when the ith item can be part of the total solution
                K[i][w] = ((val[i - 1] + K[i - 1][w - wt[i - 1]]) > (K[i - 1][w])) ? (val[i - 1] + K[i - 1][w - wt[i - 1]]) : (K[i - 1][w]); // taking maximum value generating choice from taking or not tsking the ith item
            else                                                                                                                             // when the ith item cannot be the part of total solution
                K[i][w] = K[i - 1][w];                                                                                                       // copying the value of the just above cell i.e. not considering the ith item to be part of the solution
        }
    }
    int result = K[n][W];                                                    // the maximum possible value is stored in result variable
    cout << "The maximum value that can be obtained is: " << result << endl; // the maximum possible value is displayed to user
    w = W;                                                                   // assigning the variable with value of max weight for retracing the steps to obtain the items considered for obtaining this maximum possible value
    string Itemsconsidered;
    string temp;
    cout << "The items considered for obtaining the maximum value according to their weight are:"; // Message for user display
    for (i = n; i > 0 && result > 0; i--)                                                          // loop to retrace the steps to obtain the items considered for obtaining this maximum possible value
    {
        if (result == K[i - 1][w]) // if ith item was not considered for obtaining this maximum possible value
            continue;              // move to the just above cell
        else                       // if ith item was considered for obtaining this maximum possible value
        {
            cout << " " << wt[i - 1]; // display the considered ith item to user
            temp = to_string(wt[i - 1]);
            Itemsconsidered.append(temp);
            Itemsconsidered.append(" ");
            result = result - val[i - 1]; // modify the checking variable to the value of leftover value from the obtained maximum possible value
            w = w - wt[i - 1];            // modify the weight considered variable to the leftover weight after considering the ith item to be part of the solution to obatain the maximum possible value
        }
    }
    cout << endl;
    ofstream outfile(username + "__knapsack.txt");
    outfile << "Capacity: " << W << endl;
    outfile << "Maximum value: " << K[n][W] << endl;
    outfile << "Items considered: " << Itemsconsidered << endl;
    outfile.close();
}
void displayRailandRoadKnapsackResult(const string &username)
{
    ifstream infile(username + "__knapsack.txt");
    string line;

    if (infile.is_open())
    {
        cout << "Knapsack via Rail or Road Result for User: " << username << endl;
        while (getline(infile, line))
        {
            cout << line << endl
                 << endl;
        }
        infile.close();
    }
    else
    {
        cout << "No result found for Knapsack via Rail or Road for this user: " << username << endl
             << endl;
    }
}
int main()
{
    cout << CYAN << "Welcome to :" << endl;
    cout << "             " << RED << " ***** "
         << "   " << GREEN << "  *****  "
         << "   " << YELLOW << "*   *"
         << "   " << BLUE << "  ***** " << MAGENTA << "   "
         << " *    *"
         << "   " << CYAN << "  *****  " << RESET << endl;
    cout << "             " << RED << " *    *"
         << "   " << GREEN << " *     *"
         << "   " << YELLOW << " *  * "
         << "   " << BLUE << " *     " << MAGENTA << "   "
         << "  *    *"
         << "   " << CYAN << " *     *" << RESET << endl;
    cout << "             " << RED << " ***** "
         << "   " << GREEN << " *******"
         << "   " << YELLOW << " ***   "
         << "   " << BLUE << " ***** " << MAGENTA << "   "
         << " ******"
         << "   " << CYAN << " *******" << RESET << endl;
    cout << "             " << RED << " *   * "
         << "   " << GREEN << " *     *"
         << "   " << YELLOW << " *   * "
         << "   " << BLUE << "     * " << MAGENTA << "   "
         << " *    *"
         << "   " << CYAN << " *     *" << RESET << endl;
    cout << "             " << RED << " *    *"
         << "   " << GREEN << " *     *"
         << "   " << YELLOW << " *    *"
         << "   " << BLUE << " ***** " << MAGENTA << "   "
         << " *    *"
         << "   " << CYAN << " *     *" << RESET << endl;
    cout << "\n\n"
         << "           " << RED << "Resource " << GREEN << " Allocation " << YELLOW << "for Keeping" << BLUE << " Sovereignty" << MAGENTA << " and Harmony " << CYAN << "Attained";
    cout << "\n\n";
    system("pause");
    system("cls");
    cout << RESET;
    system("color 02");
    int choice;
    User loggedInUser;
    system("pause");

    // Adding city connections (example connections)
    addCityConnection("CityA", "CityB", 10);
    addCityConnection("CityA", "CityC", 15);
    addCityConnection("CityB", "CityD", 20);
    addCityConnection("CityC", "CityD", 25);
    system("cls");
    do
    {
        cout << "1. Register\n2. Login\n3. Exit\n";
        cout << "\nEnter the option you want to choose: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            registerUser();
            break;
        case 2:
            if (loginUser(loggedInUser))
            {
                // User logged in successfully
            }
            break;
        case 3:
            cout << "Exiting program.\n"
                 << "Thankyou for using R.A.K.S.H.A." << endl;
            exit(0);
        default:
            cout << "Invalid choice. Try again." << endl;
        }
        system("pause");
        system("cls");
    } while (choice != 2);
    system("cls");
    cout << "Welcome user: " << loggedInUser.username << endl;
    system("pause");
    system("cls");
    do
    {
        cout << "1. Knapsack for airways Program\n2. View Knapsack Result for airways\n3. Dijkstra's Algorithm for shortest path\n4. Display Shortest Path\n5. Maximum Flow In A Railway Network\n6. Knapsack for Rail and Road\n7. Display Rail and Road Knapsack Results \n8. Exit\n";
        cout << "\nEnter the option you want to choose: ";
        cin >> choice;
        system("cls");
        switch (choice)
        {

        case 1:
            if (!loggedInUser.username.empty())
            {
                int numItems, capacity;
                vector<Item> items;

                cout << "Enter the number of items: ";
                cin >> numItems;

                cout << "Enter the capacity of the Knapsack via airways: ";
                cin >> capacity;

                for (int i = 0; i < numItems; ++i)
                {
                    Item newItem;
                    cout << "Enter item " << i + 1 << " name, weight, and value: ";
                    cin >> newItem.name >> newItem.weight >> newItem.value;
                    items.push_back(newItem);
                }

                int max_value = knapsack(capacity, items);

                cout << "Maximum value that can be carried in the Knapsack via airways with capacity " << capacity << " : " << max_value << endl;

                saveKnapsackResult(loggedInUser.username, capacity, max_value);
                cout << "Result saved for Knapsack via airways for this user: " << loggedInUser.username << endl;
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 2:
            if (!loggedInUser.username.empty())
            {
                displayKnapsackResult(loggedInUser.username);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 3:
            if (!loggedInUser.username.empty())
            {
                dijkstraShortestPath(loggedInUser.username);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 4:
            if (!loggedInUser.username.empty())
            {
                displayShortestPath(loggedInUser.username);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;

        case 5:
            if (!loggedInUser.username.empty())
            {
                main_fordfulkersion();
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 6:
            if (!loggedInUser.username.empty())
            {
                RailAndRoadKnapSack(loggedInUser.username);
                cout << "Result saved for Knapsack via rail or road for this user: " << loggedInUser.username << endl;
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 7:
            if (!loggedInUser.username.empty())
            {
                displayRailandRoadKnapsackResult(loggedInUser.username);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 8:
            cout << "Exiting program.\n"
                 << "Thankyou for using R.A.K.S.H.A." << endl;
            exit(0);
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
