#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const int MAX_NODES = 100;
const int MAX_PLAYERS = 64;
const int MAX_TEAM_SIZE = 5;

// Forward declarations
class Player;
class Match;
class Team;

// ===============================
// GRAPH STRUCTURE FOR TOURNAMENT BRACKET
// ===============================

struct AdjNode {
    int matchVertex;
    string matchType;
    AdjNode* next;
    
    AdjNode(int vertex, string type) : matchVertex(vertex), matchType(type), next(nullptr) {}
};

struct TournamentGraph {
    int numMatches;
    AdjNode* adjLists[MAX_NODES];
    string matchNames[MAX_NODES];
    
    TournamentGraph(int matches = 0) {
        numMatches = matches;
        for (int i = 0; i < MAX_NODES; i++) {
            adjLists[i] = nullptr;
            matchNames[i] = "";
        }
    }
    
    void addMatchConnection(int srcMatch, int destMatch, string connectionType) {
        if (srcMatch < MAX_NODES && destMatch < MAX_NODES) {
            AdjNode* newNode = new AdjNode(destMatch, connectionType);
            newNode->next = adjLists[srcMatch];
            adjLists[srcMatch] = newNode;
        }
    }
    
    void setMatchName(int matchIndex, string matchID) {
        if (matchIndex < MAX_NODES) {
            matchNames[matchIndex] = matchID;
        }
    }
    
    void printTournamentBracket() {
        cout << "\n=== TOURNAMENT BRACKET STRUCTURE ===\n";
        for (int i = 0; i < numMatches; i++) {
            cout << "Match " << i << " (" << matchNames[i] << "):";
            AdjNode* temp = adjLists[i];
            while (temp != nullptr) {
                cout << " -> Match " << temp->matchVertex 
                     << "(" << temp->matchType << ")";
                temp = temp->next;
            }
            cout << endl;
        }
    }
    
    ~TournamentGraph() {
        for (int i = 0; i < MAX_NODES; i++) {
            AdjNode* temp = adjLists[i];
            while (temp != nullptr) {
                AdjNode* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }
};

// ===============================
// DOUBLY LINKED LIST NODE
// ===============================

template<typename T>
struct DoublyNode {
    T data;
    DoublyNode* next;
    DoublyNode* prev;
    
    DoublyNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

// ===============================
// QUEUE IMPLEMENTATION
// ===============================

template<typename T>
class Queue {
private:
    DoublyNode<T>* queueFront;
    DoublyNode<T>* queueRear;
    int count;

public:
    Queue() {
        queueFront = nullptr;
        queueRear = nullptr;
        count = 0;
    }
    
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    void enqueue(T elem) {
        DoublyNode<T>* newNode = new DoublyNode<T>(elem);
        
        if (isEmpty()) {
            queueFront = newNode;
            queueRear = newNode;
        } else {
            queueRear->next = newNode;
            newNode->prev = queueRear;
            queueRear = newNode;
        }
        count++;
    }
    
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty. Cannot delete element.");
        }
        
        DoublyNode<T>* tempNode = queueFront;
        T tempInfo = queueFront->data;
        
        queueFront = queueFront->next;
        
        if (queueFront == nullptr) {
            queueRear = nullptr;
        } else {
            queueFront->prev = nullptr;
        }
        
        delete tempNode;
        count--;
        
        return tempInfo;
    }
    
    bool isEmpty() {
        return (queueFront == nullptr);
    }
    
    int size() {
        return count;
    }
    
    T front() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return queueFront->data;
    }
    
    T rear() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return queueRear->data;
    }
    
    DoublyNode<T>* getHead() {
        return queueFront;
    }
};

// ===============================
// STACK IMPLEMENTATION
// ===============================

template<typename T>
class Stack {
private:
    DoublyNode<T>* head;
    int count;

public:
    Stack() { 
        this->head = nullptr; 
        this->count = 0;
    }
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    bool isEmpty() {
        return head == nullptr;
    }
    
    void push(T new_data) {
        DoublyNode<T>* new_node = new DoublyNode<T>(new_data);
        if (!new_node) {
            cout << "\nStack Overflow";
            return;
        }
        new_node->next = head;
        if (head != nullptr) {
            head->prev = new_node;
        }
        head = new_node;
        count++;
    }
    
    T pop() {
        if (this->isEmpty()) {
            throw runtime_error("Stack Underflow");
        } else {
            DoublyNode<T>* temp = head;
            T data = head->data;
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
            delete temp;
            count--;
            return data;
        }
    }
    
    T peek() {
        if (!isEmpty())
            return head->data;
        else {
            throw runtime_error("Stack is empty");
        }
    }
    
    int size() {
        return count;
    }
};

// ===============================
// PRIORITY QUEUE IMPLEMENTATION
// ===============================

template<typename T>
class PriorityQueue {
private:
    struct Node {
        T data;
        int priority;
        Node* next;
        
        Node(const T& d, int p) : data(d), priority(p), next(nullptr) {}
    };
    
    Node* head;
    int count;
    
public:
    PriorityQueue() : head(nullptr), count(0) {}
    
    ~PriorityQueue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void enqueue(const T& item, int priority) {
        Node* newNode = new Node(item, priority);
        
        if (!head || priority > head->priority) {
            newNode->next = head;
            head = newNode;
            count++;
            return;
        }
        
        Node* current = head;
        while (current->next && current->next->priority >= priority) {
            current = current->next;
        }
        
        newNode->next = current->next;
        current->next = newNode;
        count++;
    }
    
    T dequeue() {
        if (!head) {
            throw runtime_error("Priority Queue is empty!");
        }
        
        T result = head->data;
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
        
        return result;
    }
    
    T peek() {
        if (!head) {
            throw runtime_error("Priority Queue is empty!");
        }
        return head->data;
    }
    
    bool isEmpty() { 
        return head == nullptr; 
    }
    
    int size() {
        return count;
    }
};

// ===============================
// DFS AND BFS FOR TOURNAMENT TRAVERSAL
// ===============================

class TournamentTraversal {
private:
    TournamentGraph* graph;
    
    struct SimpleStack {
        int data[MAX_NODES];
        int top = -1;
        
        void push(int value) {
            if (top < MAX_NODES - 1) {
                data[++top] = value;
            }
        }
        
        int pop() {
            if (top >= 0) {
                return data[top--];
            }
            return -1;
        }
        
        bool isEmpty() {
            return top == -1;
        }
    };
    
    struct SimpleQueue {
        int data[MAX_NODES];
        int front = 0;
        int rear = 0;
        
        void enqueue(int value) {
            if (rear < MAX_NODES) {
                data[rear++] = value;
            }
        }
        
        int dequeue() {
            if (front < rear) {
                return data[front++];
            }
            return -1;
        }
        
        bool isEmpty() {
            return front == rear;
        }
    };
    
public:
    TournamentTraversal(TournamentGraph* g) : graph(g) {}
    
    void DFS(int start) {
        bool visited[MAX_NODES] = {false};
        SimpleStack stack;
        stack.push(start);
        
        cout << "\nDFS Tournament Bracket Traversal starting from match " << start << ": ";
        while (!stack.isEmpty()) {
            int current = stack.pop();
            if (!visited[current] && current < graph->numMatches) {
                cout << graph->matchNames[current] << " ";
                visited[current] = true;
                
                AdjNode* temp = graph->adjLists[current];
                while (temp) {
                    if (!visited[temp->matchVertex]) {
                        stack.push(temp->matchVertex);
                    }
                    temp = temp->next;
                }
            }
        }
        cout << endl;
    }
    
    void BFS(int start) {
        bool visited[MAX_NODES] = {false};
        SimpleQueue queue;
        queue.enqueue(start);
        visited[start] = true;
        
        cout << "BFS Tournament Bracket Traversal starting from match " << start << ": ";
        while (!queue.isEmpty()) {
            int current = queue.dequeue();
            if (current < graph->numMatches) {
                cout << graph->matchNames[current] << " ";
                
                AdjNode* temp = graph->adjLists[current];
                while (temp) {
                    if (!visited[temp->matchVertex]) {
                        queue.enqueue(temp->matchVertex);
                        visited[temp->matchVertex] = true;
                    }
                    temp = temp->next;
                }
            }
        }
        cout << endl;
    }
};

// ===============================
// PLAYER CLASS
// ===============================

class Player {
public:
    string playerID;
    string name;
    int ranking;
    string registrationType;
    bool checkedIn;
    string university;
    string teamID;
    string status;
    
    Player() : ranking(0), checkedIn(false), status("waiting") {}
    
    Player(string id, string playerName, int rank, string regType, string uni, string team = "")
    : playerID(id), name(playerName), ranking(rank), registrationType(regType),
      checkedIn(false), university(uni), status("waiting"), teamID(team) {}
    
    int getPriority() {
        if (registrationType == "early-bird") return 3;
        if (registrationType == "wildcard") return 2;
        return 1;
    }
    
    string toString() {
        return playerID + "," + name + "," + to_string(ranking) + "," + 
               registrationType + "," + (checkedIn ? "true" : "false") + "," + university + "," + status;
    }
};

// ===============================
// TEAM CLASS (FIXED)
// ===============================

class Team {
public:
    string teamID;
    string teamName;
    string status;
    Player players[MAX_TEAM_SIZE];
    int playerCount;

    Team() {
        playerCount = 0;
        status = "active";
        teamID = "";
        teamName = "";
    }

    Team(string id, string name) {
        teamID = id;
        teamName = name;
        playerCount = 0;
        status = "active";
    }

    // Add player to team
    bool addPlayer(const Player& player) {
        if (playerCount < MAX_TEAM_SIZE) {
            players[playerCount] = player;
            playerCount++;
            return true;
        }
        return false;
    }

    // Calculate average ranking
    double getAverageRanking() const {
        if (playerCount == 0) return 0.0;
        int total = 0;
        for (int i = 0; i < playerCount; ++i) {
            total += players[i].ranking;
        }
        return (double)total / playerCount;
    }

    bool isComplete() const {
        return playerCount == MAX_TEAM_SIZE;
    }

    string toString() {
        stringstream ss;
        ss << "TeamID: " << teamID << ", TeamName: " << teamName 
           << ", AvgRanking: " << getAverageRanking() << ", Players: " << playerCount;
        return ss.str();
    }
};

// ===============================
// MATCH CLASS
// ===============================

class Match {
public:
    string matchID;
    string team1ID;
    string team2ID;
    string stage;
    string status;
    string winnerID;
    string round;
    int graphIndex;

    Match() : graphIndex(-1) {}

    Match(string id, string t1, string t2, string stg, string rnd = "round-1")
        : matchID(id), team1ID(t1), team2ID(t2), stage(stg), status("pending"), 
          round(rnd), graphIndex(-1) {}

    string toString() {
        return matchID + "," + team1ID + "," + team2ID + "," + stage + "," + 
               status + "," + winnerID + "," + round;
    }
};

// ===============================
// CSV HANDLER CLASS
// ===============================

class CSVHandler {
public:
    static void writePlayersToCSV(const string& filename, Queue<Player>* players) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        
        file << "PlayerID,Name,Ranking,RegistrationType,CheckedIn,University,Status\n";
        
        Queue<Player> tempQueue;
        while (!players->isEmpty()) {
            Player player = players->dequeue();
            file << player.toString() << "\n";
            tempQueue.enqueue(player);
        }
        
        while (!tempQueue.isEmpty()) {
            players->enqueue(tempQueue.dequeue());
        }
        
        file.close();
    }
    
    static void writeMatchesToCSV(const string& filename, Queue<Match>* matches) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        
        file << "MatchID,Team1ID,Team2ID,Stage,Status,WinnerID,Round\n";
        
        Queue<Match> tempQueue;
        while (!matches->isEmpty()) {
            Match match = matches->dequeue();
            file << match.toString() << "\n";
            tempQueue.enqueue(match);
        }
        
        while (!tempQueue.isEmpty()) {
            matches->enqueue(tempQueue.dequeue());
        }
        
        file.close();
    }
};

// ===============================
// TASK 2: TOURNAMENT REGISTRATION & PLAYER QUEUEING
// ===============================

class RegistrationManager {
private:
    PriorityQueue<Player>* registrationQueue;
    Queue<Player>* checkedInPlayers;
    Queue<Player>* waitlist;
    int maxCapacity;

public:
    RegistrationManager(int capacity = 64) : maxCapacity(capacity) {
        registrationQueue = new PriorityQueue<Player>();
        checkedInPlayers = new Queue<Player>();
        waitlist = new Queue<Player>();
    }
    
    ~RegistrationManager() {
        delete registrationQueue;
        delete checkedInPlayers;
        delete waitlist;
    }
    
    void registerPlayer(Player player) {
        int priority = player.getPriority();
        registrationQueue->enqueue(player, priority);
        cout << "Player " << player.name << " registered with " 
             << player.registrationType << " priority (Level: " << priority << ").\n";
    }
    
    void processCheckIn() {
        cout << "\n=== PROCESSING PLAYER CHECK-INS ===\n";
        
        while (!registrationQueue->isEmpty() && checkedInPlayers->size() < maxCapacity) {
            Player player = registrationQueue->dequeue();
            player.checkedIn = true;
            player.status = "active";
            checkedInPlayers->enqueue(player);
            
            cout << "Checked in: " << player.name << " (" 
                 << player.registrationType << ") - Status: " << player.status << "\n";
        }
        
        while (!registrationQueue->isEmpty()) {
            Player player = registrationQueue->dequeue();
            player.status = "waiting";
            waitlist->enqueue(player);
            cout << "Added to waitlist: " << player.name << " - Status: " << player.status << "\n";
        }
        
        cout << "Check-in complete. Checked in: " << checkedInPlayers->size() 
             << ", Waitlisted: " << waitlist->size() << "\n";
    }
    
    void handleWithdrawal(string playerID) {
        cout << "\n=== HANDLING PLAYER WITHDRAWAL ===\n";
        
        DoublyNode<Player>* current = checkedInPlayers->getHead();
        bool found = false;
        
        while (current != nullptr && !found) {
            if (current->data.playerID == playerID) {
                current->data.status = "eliminated";
                cout << "Player " << current->data.name << " has been marked as eliminated.\n";
                found = true;
            }
            current = current->next;
        }
        
        if (!waitlist->isEmpty()) {
            Player replacement = waitlist->dequeue();
            replacement.checkedIn = true;
            replacement.status = "active";
            checkedInPlayers->enqueue(replacement);
            
            cout << "Replacement: " << replacement.name 
                 << " moved from waitlist to active participants.\n";
        }
        
        if (!found) {
            cout << "Player with ID " << playerID << " not found in active participants.\n";
        }
    }
    
    Queue<Player>* getCheckedInPlayers() {
        return checkedInPlayers;
    }
    
    void displayStatus() {
        cout << "\n=== REGISTRATION STATUS ===\n";
        cout << "Checked-in players: " << checkedInPlayers->size() << "\n";
        cout << "Waitlisted players: " << waitlist->size() << "\n";
        cout << "Remaining slots: " << (maxCapacity - checkedInPlayers->size()) << "\n";
    }
};

// ===============================
// TASK 1: MATCH SCHEDULING & PLAYER PROGRESSION (FIXED)
// ===============================

class MatchScheduler {
private:
    Queue<Match>* pendingMatches;
    Stack<Match>* knockoutBracket;
    TournamentGraph* bracketGraph;
    TournamentTraversal* traversal;
    int matchCounter;
    int graphIndex;

public:
    MatchScheduler() : matchCounter(1), graphIndex(0) {
        pendingMatches = new Queue<Match>();
        knockoutBracket = new Stack<Match>();
        bracketGraph = new TournamentGraph();
        traversal = new TournamentTraversal(bracketGraph);
    }
    
    ~MatchScheduler() {
        delete pendingMatches;
        delete knockoutBracket;
        delete bracketGraph;
        delete traversal;
    }
    
    // FIXED: Generate qualifier matches for teams
    void generateQualifierMatches(Queue<Team>* teams) {
        cout << "\n=== GENERATING TEAM QUALIFIER MATCHES ===\n";
        
        if (teams->isEmpty()) {
            cout << "No teams available for matches!\n";
            return;
        }
        
        // Create a temporary copy to preserve original queue
        Queue<Team> tempTeams;
        while (!teams->isEmpty()) {
            tempTeams.enqueue(teams->dequeue());
        }
        
        // Restore original queue and create matches
        while (tempTeams.size() >= 2) {
            Team t1 = tempTeams.dequeue();
            Team t2 = tempTeams.dequeue();
            
            // Restore teams to original queue
            teams->enqueue(t1);
            teams->enqueue(t2);

            string matchID = "Q" + to_string(matchCounter++);
            Match match(matchID, t1.teamID, t2.teamID, "qualifier", "round-1");
            match.graphIndex = graphIndex;

            bracketGraph->setMatchName(graphIndex, matchID);
            bracketGraph->numMatches = max(bracketGraph->numMatches, graphIndex + 1);

            pendingMatches->enqueue(match);

            cout << "Created match " << matchID << " (Graph Index: " << graphIndex << "): "
                 << t1.teamName << " (Avg: " << t1.getAverageRanking() << ") vs " 
                 << t2.teamName << " (Avg: " << t2.getAverageRanking() << ")\n";

            graphIndex++;
        }
        
        // Handle odd team
        if (!tempTeams.isEmpty()) {
            Team byeTeam = tempTeams.dequeue();
            teams->enqueue(byeTeam);
            cout << "Team " << byeTeam.teamName << " receives a bye to next round.\n";
        }
    }
    
    // FIXED: Simulate matches with proper team lookup
    Queue<Team>* simulateMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING TEAM MATCHES ===\n";
        Queue<Team>* winners = new Queue<Team>();

        if (pendingMatches->isEmpty()) {
            cout << "No matches to simulate!\n";
            return winners;
        }

        // Create lookup map for teams
        Queue<Team> lookup;
        DoublyNode<Team>* current = allTeams->getHead();
        while (current != nullptr) {
            lookup.enqueue(current->data);
            current = current->next;
        }

        Queue<Match> matchesToProcess;
        while (!pendingMatches->isEmpty()) {
            Match match = pendingMatches->dequeue();
            match.status = "in-progress";

            Team team1, team2;
            bool found1 = false, found2 = false;

            // Find teams in lookup
            DoublyNode<Team>* lookupCurrent = lookup.getHead();
            while (lookupCurrent != nullptr) {
                if (lookupCurrent->data.teamID == match.team1ID) {
                    team1 = lookupCurrent->data;
                    found1 = true;
                }
                if (lookupCurrent->data.teamID == match.team2ID) {
                    team2 = lookupCurrent->data;
                    found2 = true;
                }
                lookupCurrent = lookupCurrent->next;
            }

            if (found1 && found2) {
                double avg1 = team1.getAverageRanking();
                double avg2 = team2.getAverageRanking();

                Team winner = (avg1 < avg2) ? team1 : team2;
                Team loser  = (avg1 < avg2) ? team2 : team1;

                match.winnerID = winner.teamID;
                match.status = "completed";

                winner.status = "qualified";
                loser.status = "eliminated";

                winners->enqueue(winner);

                cout << "Match " << match.matchID << " completed:\n";
                cout << "  Winner: " << winner.teamName << " (Avg Rank: " << avg1 << ")\n";
                cout << "  Eliminated: " << loser.teamName << " (Avg Rank: " << avg2 << ")\n";
            } else {
                cout << "Warning: Could not find teams for match " << match.matchID << "\n";
            }

            matchesToProcess.enqueue(match);
        }

        // Restore matches queue
        while (!matchesToProcess.isEmpty()) {
            pendingMatches->enqueue(matchesToProcess.dequeue());
        }

        return winners;
    }
    
    // FIXED: Generate knockout bracket
    void generateKnockoutBracket(Queue<Team>* qualifiedTeams) {
        cout << "\n=== GENERATING TEAM KNOCKOUT BRACKET ===\n";

        if (qualifiedTeams->isEmpty()) {
            cout << "No qualified teams for knockout bracket!\n";
            return;
        }

        int prevMatchIndex = -1;

        while (qualifiedTeams->size() >= 2) {
            Team t1 = qualifiedTeams->dequeue();
            Team t2 = qualifiedTeams->dequeue();

            string matchID = "K" + to_string(matchCounter++);
            Match match(matchID, t1.teamID, t2.teamID, "knockout", "knockout-round");
            match.graphIndex = graphIndex;

            bracketGraph->setMatchName(graphIndex, matchID);

            if (prevMatchIndex != -1) {
                bracketGraph->addMatchConnection(prevMatchIndex, graphIndex, "leads-to");
            }

            bracketGraph->numMatches = max(bracketGraph->numMatches, graphIndex + 1);
            knockoutBracket->push(match);

            cout << "Knockout match " << matchID << " (Graph Index: " << graphIndex << "): "
                 << t1.teamName << " (Avg: " << t1.getAverageRanking() << ") vs " 
                 << t2.teamName << " (Avg: " << t2.getAverageRanking() << ")\n";

            prevMatchIndex = graphIndex;
            graphIndex++;
        }
        
        cout << "Knockout bracket generated with " << knockoutBracket->size() << " matches.\n";
    }
    
    void displayBracketTraversal() {
        if (bracketGraph->numMatches > 0) {
            cout << "\n=== TOURNAMENT BRACKET TRAVERSAL ===\n";
            bracketGraph->printTournamentBracket();
            
            traversal->DFS(0);
            traversal->BFS(0);
        } else {
            cout << "\n=== NO BRACKET TO TRAVERSE ===\n";
            cout << "No matches were generated in the bracket.\n";
        }
    }
    
    Queue<Match>* getAllMatches() {
        return pendingMatches;
    }
    
    void displayBracketStatus() {
        cout << "\n=== BRACKET STATUS ===\n";
        cout << "Pending matches: " << pendingMatches->size() << "\n";
        cout << "Knockout bracket size: " << knockoutBracket->size() << "\n";
        cout << "Graph matches: " << bracketGraph->numMatches << "\n";
    }
};

// ===============================
// MAIN TOURNAMENT SYSTEM (FIXED)
// ===============================

class TournamentSystem {
private:
    RegistrationManager* regManager;
    MatchScheduler* matchScheduler;

public:
    TournamentSystem() {
        regManager = new RegistrationManager(40); // Capacity for 8 teams × 5 players
        matchScheduler = new MatchScheduler();
    }
    
    ~TournamentSystem() {
        delete regManager;
        delete matchScheduler;
    }
    
    // FIXED: Create teams from players grouped by teamID
    Queue<Team>* createTeamsFromPlayers(Queue<Player>* players) {
        cout << "\n=== CREATING TEAMS FROM PLAYERS (GROUPED BY TEAM ID) ===\n";
        
        Queue<Team>* teamQueue = new Queue<Team>();
        
        // Create array to hold players for each team
        Player teamPlayers[8][MAX_TEAM_SIZE];  // Support up to 8 teams
        int teamPlayerCounts[8] = {0};
        string teamNames[8] = {"", "", "", "", "", "", "", ""};
        
        // Group players by teamID
        DoublyNode<Player>* current = players->getHead();
        while (current != nullptr) {
            Player player = current->data;
            
            // Extract team number from teamID (e.g., "T1" -> 0, "T2" -> 1, etc.)
            if (!player.teamID.empty() && player.teamID.length() >= 2) {
                int teamIndex = -1;
                if (player.teamID == "T1") teamIndex = 0;
                else if (player.teamID == "T2") teamIndex = 1;
                else if (player.teamID == "T3") teamIndex = 2;
                else if (player.teamID == "T4") teamIndex = 3;
                else if (player.teamID == "T5") teamIndex = 4;
                else if (player.teamID == "T6") teamIndex = 5;
                else if (player.teamID == "T7") teamIndex = 6;
                else if (player.teamID == "T8") teamIndex = 7;
                
                if (teamIndex >= 0 && teamIndex < 8 && teamPlayerCounts[teamIndex] < MAX_TEAM_SIZE) {
                    teamPlayers[teamIndex][teamPlayerCounts[teamIndex]] = player;
                    teamPlayerCounts[teamIndex]++;
                    
                    // Set team name based on university
                    if (teamNames[teamIndex].empty()) {
                        if (player.university == "APU") teamNames[teamIndex] = "APU Dragons";
                        else if (player.university == "UM") teamNames[teamIndex] = "UM Tigers";
                        else if (player.university == "USM") teamNames[teamIndex] = "USM Eagles";
                        else if (player.university == "MMU") teamNames[teamIndex] = "MMU Wolves";
                        else if (player.university == "UTAR") teamNames[teamIndex] = "UTAR Panthers";
                        else if (player.university == "INTI") teamNames[teamIndex] = "INTI Sharks";
                        else if (player.university == "HELP") teamNames[teamIndex] = "HELP Hawks";
                        else if (player.university == "TAYLOR") teamNames[teamIndex] = "TAYLOR Titans";
                        else teamNames[teamIndex] = player.university + " Team";
                    }
                }
            }
            current = current->next;
        }
        
        // Create complete teams (only teams with exactly 5 players)
        for (int i = 0; i < 8; i++) {
            if (teamPlayerCounts[i] == MAX_TEAM_SIZE) {
                Team team("T" + to_string(i + 1), teamNames[i]);
                
                cout << "Creating " << team.teamName << " (" << team.teamID << ") with players: ";
                for (int j = 0; j < MAX_TEAM_SIZE; j++) {
                    team.addPlayer(teamPlayers[i][j]);
                    cout << teamPlayers[i][j].name << " ";
                }
                cout << "(Avg Rank: " << team.getAverageRanking() << ")\n";
                
                teamQueue->enqueue(team);
            } else if (teamPlayerCounts[i] > 0) {
                cout << "Incomplete team " << teamNames[i] << " (" << teamPlayerCounts[i] << "/5 players) - not included in tournament\n";
            }
        }
        
        cout << "Created " << teamQueue->size() << " complete teams for tournament.\n";
        return teamQueue;
    }
    
    void runTournament() {
        cout << "=== ASIA PACIFIC UNIVERSITY ESPORTS CHAMPIONSHIP ===\n";
        cout << "=== USING GRAPH-BASED TOURNAMENT BRACKET SYSTEM ===\n\n";
    
        // Create players with pre-assigned team IDs
        Player players[] = {
            // Team 1 - APU Dragons (T1)
            Player("P001", "Alice Wong", 5, "early-bird", "APU", "T1"),
            Player("P002", "Bob Chen", 4, "regular", "APU", "T1"),
            Player("P003", "Carol Tan", 3, "early-bird", "APU", "T1"),
            Player("P004", "David Lim", 2, "wildcard", "APU", "T1"),
            Player("P005", "Eva Kumar", 5, "regular", "APU", "T1"),
            
            // Team 2 - UM Tigers (T2)
            Player("P006", "Frank Lee", 3, "early-bird", "UM", "T2"),
            Player("P007", "Grace Ng", 2, "regular", "UM", "T2"),
            Player("P008", "Henry Goh", 1, "wildcard", "UM", "T2"),
            Player("P009", "Ivy Lau", 1, "early-bird", "UM", "T2"),
            Player("P010", "Jack Tan", 1, "regular", "UM", "T2"),
            
            // Team 3 - USM Eagles (T3)
            Player("P011", "Kelly Ong", 3, "early-bird", "USM", "T3"),
            Player("P012", "Liam Chow", 3, "regular", "USM", "T3"),
            Player("P013", "Maya Singh", 4, "wildcard", "USM", "T3"),
            Player("P014", "Noah Kim", 3, "early-bird", "USM", "T3"),
            Player("P015", "Olivia Chen", 1, "regular", "USM", "T3"),
            
            // Team 4 - MMU Wolves (T4)
            Player("P016", "Peter Yap", 5, "early-bird", "MMU", "T4"),
            Player("P017", "Quinn Lee", 5, "regular", "MMU", "T4"),
            Player("P018", "Rachel Teo", 5, "wildcard", "MMU", "T4"),
            Player("P019", "Sam Wong", 4, "early-bird", "MMU", "T4"),
            Player("P020", "Tina Lim", 5, "regular", "MMU", "T4"),
            
            // Team 5 - UTAR Panthers (T5)
            Player("P021", "Uma Patel", 2, "early-bird", "UTAR", "T5"),
            Player("P022", "Victor Ng", 3, "regular", "UTAR", "T5"),
            Player("P023", "Wendy Koh", 4, "wildcard", "UTAR", "T5"),
            Player("P024", "Xavier Tan", 5, "early-bird", "UTAR", "T5"),
            Player("P025", "Yvonne Lee", 4, "regular", "UTAR", "T5"),
            
            // Team 6 - INTI Sharks (T6)
            Player("P026", "Zara Ahmed", 2, "early-bird", "INTI", "T6"),
            Player("P027", "Adam Chong", 4, "regular", "INTI", "T6"),
            Player("P028", "Bella Tan", 2, "wildcard", "INTI", "T6"),
            Player("P029", "Chris Lim", 1, "early-bird", "INTI", "T6"),
            Player("P030", "Diana Wong", 5, "regular", "INTI", "T6"),
            
            // Team 7 - HELP Hawks (T7)
            Player("P031", "Ethan Ong", 1, "early-bird", "HELP", "T7"),
            Player("P032", "Fiona Lau", 1, "regular", "HELP", "T7"),
            Player("P033", "George Kim", 1, "wildcard", "HELP", "T7"),
            Player("P034", "Hannah Chen", 1, "early-bird", "HELP", "T7"),
            Player("P035", "Ian Yap", 1, "regular", "HELP", "T7"),
            
            // Team 8 - TAYLOR Titans (T8)
            Player("P036", "Julia Teo", 3, "early-bird", "TAYLOR", "T8"),
            Player("P037", "Kevin Ng", 3, "regular", "TAYLOR", "T8"),
            Player("P038", "Luna Koh", 3, "wildcard", "TAYLOR", "T8"),
            Player("P039", "Mike Tan", 3, "early-bird", "TAYLOR", "T8"),
            Player("P040", "Nina Lee", 3, "regular", "TAYLOR", "T8")
        };
        
        // Register all players
        cout << "=== PLAYER REGISTRATION ===\n";
        for (int i = 0; i < 40; i++) {
            regManager->registerPlayer(players[i]);
        }
        
        // Process check-ins
        regManager->processCheckIn();
        regManager->displayStatus();
        
        // Get checked-in players
        Queue<Player>* checkedInPlayers = regManager->getCheckedInPlayers();
        
        // Create teams from players grouped by teamID
        Queue<Team>* teamQueue = createTeamsFromPlayers(checkedInPlayers);
        
        if (teamQueue->size() >= 2) {
            // Generate qualifier matches (team vs team)
            matchScheduler->generateQualifierMatches(teamQueue);

            // Simulate team matches
            Queue<Team>* winners = matchScheduler->simulateMatches(teamQueue);

            // Generate knockout bracket for winning teams
            if (winners->size() >= 2) {
                matchScheduler->generateKnockoutBracket(winners);
            } else {
                cout << "\nNot enough winning teams for knockout bracket.\n";
            }

            // Display bracket structure and traversal
            matchScheduler->displayBracketTraversal();
            matchScheduler->displayBracketStatus();

            // Test withdrawal
            cout << "\n=== TESTING PLAYER WITHDRAWAL ===\n";
            regManager->handleWithdrawal("P004");

            // Display team ranking analysis
            displayTeamRankingAnalysis(teamQueue);

            delete winners;
        } else {
            cout << "\nNot enough teams formed for tournament!\n";
        }

        // Save to CSV
        cout << "\n=== SAVING TO CSV FILES ===\n";
        CSVHandler::writePlayersToCSV("players.csv", checkedInPlayers);
        CSVHandler::writeMatchesToCSV("matches.csv", matchScheduler->getAllMatches());
        cout << "Data saved to players.csv and matches.csv\n";

        delete teamQueue;
    }
    
    // Display team ranking analysis
    void displayTeamRankingAnalysis(Queue<Team>* teams) {
        cout << "\n=== TEAM RANKING ANALYSIS ===\n";
        
        // Find best and worst average rankings
        double bestAvg = 999.0, worstAvg = 0.0;
        string bestTeam = "", worstTeam = "";
        
        DoublyNode<Team>* current = teams->getHead();
        while (current != nullptr) {
            Team team = current->data;
            if (team.getAverageRanking() < bestAvg) {
                bestAvg = team.getAverageRanking();
                bestTeam = team.teamName;
            }
            if (team.getAverageRanking() > worstAvg) {
                worstAvg = team.getAverageRanking();
                worstTeam = team.teamName;
            }
            current = current->next;
        }
        
        cout << "Best Team (Lowest Avg Ranking): " << bestTeam << " (" << bestAvg << ")\n";
        cout << "Weakest Team (Highest Avg Ranking): " << worstTeam << " (" << worstAvg << ")\n";
        
        // Display all teams with rankings
        cout << "\nAll Teams by Average Ranking:\n";
        cout << "Rank | Team Name        | Avg Ranking | University\n";
        cout << "-----|------------------|-------------|----------\n";
        
        current = teams->getHead();
        int rank = 1;
        while (current != nullptr) {
            Team team = current->data;
            cout << " " << rank << "   | " << team.teamName;
            // Padding for team name
            for (int j = team.teamName.length(); j < 16; j++) cout << " ";
            cout << " | " << team.getAverageRanking() << "        | ";
            
            // Get university from first player
            if (team.playerCount > 0) {
                cout << team.players[0].university;
            }
            cout << "\n";
            
            current = current->next;
            rank++;
        }
    }
};

// ===============================
// MAIN FUNCTION
// ===============================

int main() {
    try {
        cout << "=================================================================\n";
        cout << "    ASIA PACIFIC UNIVERSITY ESPORTS CHAMPIONSHIP SYSTEM\n";
        cout << "          Graph-Based Tournament Management\n";
        cout << "=================================================================\n";
        
        TournamentSystem tournament;
        tournament.runTournament();
        
        cout << "\n=================================================================\n";
        cout << "               TOURNAMENT SYSTEM COMPLETED\n";
        cout << "=================================================================\n";
        
    } catch ( exception& e) {
        cout << "\n=== ERROR OCCURRED ===\n";
        cout << "Error: " << e.what();
        return 1;
    }
    
    return 0;
}