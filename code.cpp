#include <bits/stdc++.h>
using namespace std;



class ListNode {
public:
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class LinkedList {
public:
    ListNode *head;
    LinkedList() : head(NULL) {}
    void insert(int x) {
        ListNode *node = new ListNode(x);
        if (head == NULL) {
            head = node;
        } else {
            ListNode *temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = node;
        }
    }
    void print() {
        ListNode *temp = head;
        while (temp != NULL) {
            cout << temp->val << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

class Queue {
public:
    LinkedList *list;
    Queue() : list(new LinkedList()) {}
    void enqueue(int x) {
        list->insert(x);
    }
    void dequeue() {
        if (list->head == NULL) {
            cout << "Queue is empty" << endl;
        } else {
            list->head = list->head->next;
        }
    }
    void print() {
        list->print();
    }
};

class Stack {
public:
    LinkedList *list;
    Stack() : list(new LinkedList()) {}
    void push(int x) {
        list->insert(x);
    }
    bool empty() {
        return list->head == NULL;
    }
    int top() {
        if (list->head == NULL) {
            cout << "Stack is empty" << endl;
            return -1;
        } else {
            return list->head->val;
        }
    }
    void pop() {
        if (list->head == NULL) {
            cout << "Stack is empty" << endl;
        } else {
            list->head = list->head->next;
        }
    }
    void print() {
        list->print();
    }
};

void printAdjacencyList(vector<int> adj[], int n) {
    for (int i=0; i<n; ++i) {
        cout << i << " -> ";
        for (int i : adj[i]) {
            cout << i << ", ";
        }
        cout << endl;
    }
}

void printGraphWithEdges(int **edges, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << edges[i][j] << ", ";
        }
        cout << endl;
    }
}

void bfs(int **edges, int n, int sv, bool *visited) {
    Queue *q = new Queue();
    q->enqueue(sv);
    visited[sv] = true;
    while (q->list->head != NULL) {
        int front = q->list->head->val;
        q->dequeue();
        cout << front << " ";
        for (int i = 0; i < n; i++) {
            if (i == front) {
                continue;
            }
            if (edges[front][i] == 1 && !visited[i]) {
                q->enqueue(i);
                visited[i] = true;
            }
        }
    }
}

const int N = 1e5;
vector<int> adj[N], reverseAdj[N];
stack<int> st;
int who[N], degree[N]={0};
bool visited[N], visited2[N];
//Stack st;

void dfs1(int u) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs1(v);
        }
    }
    st.push(u);
}

void dfs2(int u, int r) { // r is representative
    visited2[u] = true;
    who[u] = r;
    for (int v : reverseAdj[u]) {
        if (!visited2[v]) {
            dfs2(v, r);
        }
    }
}

vector<string> solve(vector<string> airports, vector<vector<string>> routes, string newAirport) {
    /*
    Step 1: process input
    Go through the airports assign each airport a unique integer
    Create adjecency list vector<int> adj[n]
    Create map<string, int> mp
    */
    int n = airports.size();
    map<string, int> mp;

    // assign each airport an id
    for (int i = 0; i < n; ++i) {
        mp[airports[i]] = i;
    }

    // go through the routes and fill the adjecency list
    for (vector<string> v : routes) {
        adj[mp[v[0]]].push_back(mp[v[1]]); 
    }

    // strongly connected components
    // create reverse/inverse adjecency list
    for (int i = 0; i < n; i++) {
        for (int j : adj[i]) {
            reverseAdj[j].push_back(i);
        }
    }

    // dfs on the original graph
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    // dfs on the reverse graph
    // find the representative of each SCC

    while(!st.empty()) {
        int u = st.top();
        st.pop();

        if (!visited2[u]) {
            dfs2(u, u);
        }
    }

    // find the indegree of each airport
    for (int i=0; i<n; i++) {
        for (int j : adj[i]) {
            if (who[i] != who[j]) {
                degree[who[j]]++;
            }
        }
    }

    vector<string> ans;

    for (int i=0; i<n; i++) {
        // find the airport that has no incoming edges and is the representative of its SCC and is not the new airport
        if (who[i] == i && degree[i] == 0) {
            ans.push_back(airports[i]);
        }
    }

    return ans;
}

void driverMenu(vector<string> &airports, vector<vector<string>> &routes) {
    cout << "1. Add a new airport" << endl;
    cout << "2. Add a new route" << endl;
    cout << "3. Show needed airports" << endl;
    cout << "4. Show all airports" << endl;
    cout << "5. Show all routes" << endl;
    cout << "6. Exit" << endl;

    int choice;
    cin >> choice;

    switch (choice) {
        case 1: {
            cout << "\n";
            cout << "Enter the name of the airport" << endl;
            string airport;
            cin >> airport;
            airports.push_back(airport);
            // export to file
            ofstream fout("airports.txt");
            for (string s : airports) {
                fout << s << endl;
            }
            break;
        }
        case 2: {
            cout << "\n";
            cout << "Enter the name of the airports" << endl;
            if (airports.size() == 0) {
                cout << "No airports exist" << endl;
                break;
            string airport1, airport2;
            cin >> airport1 >> airport2;
            routes.push_back({airport1, airport2});
            // export to file
            ofstream fout("routes.txt");
            for (vector<string> v : routes) {
                fout << v[0] << " " << v[1] << endl;
            }
            break;
        }
        case 3: {
            cout << "\n";
            cout << "Enter the name of the new airport" << endl;
            string newAirport;
            cin >> newAirport;
            vector<string> ans = solve(airports, routes, newAirport);
            cout << "The best places to get licensed in are - ";
            for (string s : ans) {
                cout << s << " ";
            }
            cout << endl;
            break;
        }
        case 4: {
            cout << "\n";
            for (string s : airports) {
                cout << s << endl;
            }
            cout << endl;
            break;
        }
        case 5: {
            cout << "\n";
            for (vector<string> v : routes) {
                cout << v[0] << " ---> " << v[1] << endl;
            }
            cout << endl;
            break;
        }
        case 6: {
            cout << "\n";
            cout << "Exiting" << endl;
            exit(0);
            break;
        }
        default: {
            cout << "\n";
            cout << "Invalid choice" << endl;
            break;
        }
    }
}
}

int main() {
    //vector<string> airports = {"BGI", "CDG", "DEL", "DOH", "DSM", "EWR", "EYW", "HND", "ICN", "JFK", "LGA", "LHR", "ORD", "SAN", "SFO", "SIN", "TLV", "BUD"};
    //vector<vector<string>> routes = {{"DSM", "ORD"}, {"ORD", "BGI"}, {"BGI", "LGA"}, {"SIN", "CDG"}, {"CDG", "SIN"}, {"CDG", "BUD"}, {"DEL", "DOH"}, {"DEL", "CDG"}, {"TLV", "DEL"}, {"EWR", "HND"}, {"HND", "ICN"}, {"HND", "JFK"}, {"ICN", "JFK"}, {"JFK", "LGA"}, {"EYW", "LHR"}, {"LHR", "SFO"}, {"SFO", "SAN"}, {"SFO", "DSM"}, {"SAN", "EYW"}};

    // import airports and routes from file
    vector<string> airports;
    vector<vector<string>> routes;

    ifstream file("airports.txt");
    string line;
    while (getline(file, line)) {
        // ignore empty lines
        if (line == "") {
            continue;
        }
        airports.push_back(line);
    }
    
    ifstream file2("routes.txt");
    string line2;
    while (getline(file2, line2)) {
        stringstream ss(line2);
        string word;
        vector<string> v;
        // ignore empty lines
        if (line2 == "") {
            continue;
        }
        while (ss >> word) {
            v.push_back(word);
        }
        routes.push_back(v);
    }
    
    while (true) {
        driverMenu(airports, routes);
    }
        
        

    //vector<string> ans = solve(airports, routes, "LGA");
    //for (string s : ans) {
    //    cout << s << " ";
    //}
    //cout << endl;

    return 0;
}
