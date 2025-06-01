#include "DataStructs.h"
#include "Entities.h"
#include "RegistrationManager.h"
#include "MatchScheduler.h"
#include "CSVHandler.h"
#include "Task4.h"  
#include <iostream>
#include <exception>

using namespace std;

// =======================
// TOURNAMENT SYSTEM CLASS
// =======================

class TournamentSystem {
private:
    RegistrationManager* regManager;
    MatchScheduler* matchScheduler;
    GameResultLogger* resultLogger;  

public:
    TournamentSystem() {
        regManager = new RegistrationManager(40);
        matchScheduler = new MatchScheduler();
        resultLogger = new GameResultLogger();  
    }
    
    ~TournamentSystem() {
        delete regManager;
        delete matchScheduler;
        delete resultLogger;  
    }
    
    // Simulate match results
    void simulateAndLogMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING MATCHES WITH DETAILED RESULT LOGGING ===\n";
        
        Queue<Match>* allMatches = matchScheduler->getAllMatches();
        
        if (allMatches->isEmpty()) {
            cout << "No matches to simulate!\n";
            return;
        }
        
        // Create temporary queue to preserve original
        Queue<Match> tempMatches;
        int matchNum = 1;
        
        while (!allMatches->isEmpty()) {
            Match match = allMatches->dequeue();
            
            // Simulate detailed match results
            int score1 = 15 + (rand() % 16);  // Random score between 15-30
            int score2 = 10 + (rand() % 16);  // Random score between 10-25
            
            // Determine winner based on team strength 
            Team team1, team2;
            bool found1 = false, found2 = false;
            
            DoublyNode<Team>* current = allTeams->getHead();
            while (current != nullptr) {
                if (current->data.teamID == match.team1ID) {
                    team1 = current->data;
                    found1 = true;
                }
                if (current->data.teamID == match.team2ID) {
                    team2 = current->data;
                    found2 = true;
                }
                current = current->next;
            }
            
            if (found1 && found2) {
                // Winner is team with lower average ranking (better team)
                if (team1.getAverageRanking() < team2.getAverageRanking()) {
                    match.winnerID = team1.teamID;
                    score1 += 5;  // Winner bonus
                } else {
                    match.winnerID = team2.teamID;
                    score2 += 5;  // Winner bonus
                }
                
                match.status = "completed";
                
                // Generate realistic match details for Task 4
                string date = "2025-01-" + to_string(15 + matchNum);
                string duration = to_string(25 + (rand() % 20)) + " minutes";
                
                // Select random MVP from winning team
                string mvpPlayerID = "P" + to_string((rand() % 5) + 1 + 
                    ((match.winnerID == "T1") ? 0 : 
                     (match.winnerID == "T2") ? 5 : 
                     (match.winnerID == "T3") ? 10 : 
                     (match.winnerID == "T4") ? 15 : 
                     (match.winnerID == "T5") ? 20 : 
                     (match.winnerID == "T6") ? 25 : 
                     (match.winnerID == "T7") ? 30 : 35));
                
                // Use TASK 4 to log the detailed result
                resultLogger->logMatchResult(match, score1, score2, date, duration, mvpPlayerID, 
                                           regManager->getCheckedInPlayers(), allTeams);
                
                cout << "Match " << matchNum << " simulated and logged!\n";
                matchNum++;
            }
            
            tempMatches.enqueue(match);
        }
        
        // Restore matches queue
        while (!tempMatches.isEmpty()) {
            allMatches->enqueue(tempMatches.dequeue());
        }
    }
    
    // TASK 4 MENU FUNCTIONS
    void showTask4Menu() {
        int choice;
        do {
            cout << "\n========= TASK 4: GAME RESULT LOGGING & PERFORMANCE HISTORY =========\n";
            cout << "1. View Recent Match Results (Stack - LIFO)\n";
            cout << "2. View Complete Match History (Queue - FIFO)\n";
            cout << "3. Display Player Performance Statistics\n";
            cout << "4. Display Team Performance Statistics\n";
            cout << "5. Export Results to CSV\n";
            cout << "6. View Statistics Summary\n";
            cout << "7. Back to Main Menu\n";
            cout << "=====================================================================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1:
                    resultLogger->displayRecentResults();
                    break;
                case 2:
                    resultLogger->displayMatchHistory();
                    break;
                case 3:
                    resultLogger->displayPlayerStats();
                    break;
                case 4:
                    resultLogger->displayTeamStats();
                    break;
                case 5:
                    resultLogger->exportResultsToCSV();
                    break;
                case 6:
                    resultLogger->displayStatisticsSummary();
                    break;
                case 7:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
            
            if (choice != 7) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
            
        } while (choice != 7);
    }
    
    // Create teams from players grouped by teamID 
    Queue<Team>* createTeamsFromPlayers(Queue<Player>* players) {
        cout << "\n=== CREATING TEAMS FROM PLAYERS (GROUPED BY TEAM ID) ===\n";
        
        Queue<Team>* teamQueue = new Queue<Team>();
        
        // Create array to hold players for each team
        Player teamPlayers[8][MAX_TEAM_SIZE];
        int teamPlayerCounts[8] = {0};
        string teamNames[8] = {"", "", "", "", "", "", "", ""};
        
        // Group players by teamID
        DoublyNode<Player>* current = players->getHead();
        while (current != nullptr) {
            Player player = current->data;
            
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
        
        // Create complete teams
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
            }
        }
        
        cout << "Created " << teamQueue->size() << " complete teams for tournament.\n";
        return teamQueue;
    }
    
    // Main tournament execution
    void runTournament() {
        cout << "=== ASIA PACIFIC UNIVERSITY ESPORTS CHAMPIONSHIP ===\n";
        cout << "=== FEATURING ADVANCED RESULT LOGGING SYSTEM (TASK 4) ===\n\n";
    
        // Create sample players with pre-assigned team IDs
        Player players[] = {
            // Team 1 - APU Dragons (T1)
            Player("P001", "Alice Wong", 12, "early-bird", "APU", "T1"),
            Player("P002", "Bob Chen", 15, "regular", "APU", "T1"),
            Player("P003", "Carol Tan", 8, "early-bird", "APU", "T1"),
            Player("P004", "David Lim", 20, "wildcard", "APU", "T1"),
            Player("P005", "Eva Kumar", 10, "regular", "APU", "T1"),
            
            // Team 2 - UM Tigers (T2)
            Player("P006", "Frank Lee", 18, "early-bird", "UM", "T2"),
            Player("P007", "Grace Ng", 22, "regular", "UM", "T2"),
            Player("P008", "Henry Goh", 14, "wildcard", "UM", "T2"),
            Player("P009", "Ivy Lau", 25, "early-bird", "UM", "T2"),
            Player("P010", "Jack Tan", 16, "regular", "UM", "T2"),
            
            // Team 3 - USM Eagles (T3)
            Player("P011", "Kelly Ong", 5, "early-bird", "USM", "T3"),
            Player("P012", "Liam Chow", 7, "regular", "USM", "T3"),
            Player("P013", "Maya Singh", 9, "wildcard", "USM", "T3"),
            Player("P014", "Noah Kim", 11, "early-bird", "USM", "T3"),
            Player("P015", "Olivia Chen", 6, "regular", "USM", "T3"),
            
            // Team 4 - MMU Wolves (T4)
            Player("P016", "Peter Yap", 30, "early-bird", "MMU", "T4"),
            Player("P017", "Quinn Lee", 28, "regular", "MMU", "T4"),
            Player("P018", "Rachel Teo", 32, "wildcard", "MMU", "T4"),
            Player("P019", "Sam Wong", 26, "early-bird", "MMU", "T4"),
            Player("P020", "Tina Lim", 29, "regular", "MMU", "T4")
        };
        
        // Register players 
        cout << "=== PLAYER REGISTRATION (TASK 2) ===\n";
        for (int i = 0; i < 20; i++) {
            regManager->registerPlayer(players[i]);
        }
        
        regManager->processCheckIn();
        regManager->displayStatus();
        
        // Create teams from players
        Queue<Player>* checkedInPlayers = regManager->getCheckedInPlayers();
        Queue<Team>* teamQueue = createTeamsFromPlayers(checkedInPlayers);
        
        if (teamQueue->size() >= 2) {
            // Generate matches (using friend's Task 1 system)
            cout << "\n=== MATCH SCHEDULING (TASK 1) ===\n";
            matchScheduler->generateQualifierMatches(teamQueue);
            
            Queue<Team>* winners = matchScheduler->simulateMatches(teamQueue);
            
            if (winners->size() >= 2) {
                matchScheduler->generateKnockoutBracket(winners);
            }
            
            // TASK 4 - SIMULATE AND LOG DETAILED RESULTS
            cout << "\n=== ADVANCED RESULT LOGGING (YOUR TASK 4) ===\n";
            simulateAndLogMatches(teamQueue);
            
            // Display the results using Task 4 system
            cout << "\n=== TASK 4 DEMONSTRATION ===\n";
            resultLogger->displayRecentResults(3);
            resultLogger->displayPlayerStats();
            resultLogger->displayTeamStats();
            resultLogger->displayStatisticsSummary();
            
            delete winners;
        }
        
        // Show Task 4 interactive menu
        showTask4Menu();
        
        // Export all data
        cout << "\n=== EXPORTING ALL DATA ===\n";
        CSVHandler::exportAllData(checkedInPlayers, matchScheduler->getAllMatches(), teamQueue);
        resultLogger->exportResultsToCSV();
        
        delete teamQueue;
    }
};

// ===============================
// MAIN FUNCTION
// ===============================

int main() {
    try {
        cout << "=================================================================\n";
        cout << "    ASIA PACIFIC UNIVERSITY ESPORTS CHAMPIONSHIP SYSTEM\n";
        cout << "    FEATURING ALL 4 TASKS WITH ADVANCED DATA STRUCTURES\n";
        cout << "=================================================================\n";
        cout << "Task 1: Match Scheduling & Player Progression (Alfie)\n";
        cout << "Task 2: Tournament Registration & Player Queueing (Hadi)\n";
        cout << "Task 3: Live Stream & Spectator Queue Management (Stanlie)\n";
        cout << "Task 4: Game Result Logging & Performance History (Badr)\n";
        cout << "=================================================================\n";
        
        TournamentSystem tournament;
        tournament.runTournament();
        
        cout << "\n=================================================================\n";
        cout << "               TOURNAMENT SYSTEM COMPLETED\n";
        cout << "=================================================================\n";
        cout << "📊 YOUR TASK 4 CONTRIBUTIONS:\n";
        cout << "   • Stack-based Recent Results (LIFO access)\n";
        cout << "   • Queue-based Match History (FIFO chronological)\n";
        cout << "   • Player Performance Tracking\n";
        cout << "   • Team Statistics Analysis\n";
        cout << "   • CSV Export for Tournament Analysis\n";
        cout << "=================================================================\n";
        
    } catch (const exception& e) {
        cout << "\n=== ERROR OCCURRED ===\n";
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    cout << "\nThank you for using the Tournament Management System!\n";
    return 0;
}