// ===== UPDATED MAIN.CPP WITH MENU SYSTEM =====
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
// TOURNAMENT SYSTEM CLASS WITH MENU
// =======================

class TournamentSystem {
private:
    RegistrationManager* regManager;
    MatchScheduler* matchScheduler;
    GameResultLogger* resultLogger;
    Queue<Player>* checkedInPlayers;
    Queue<Team>* teamQueue;
    bool tournamentInitialized;

public:
    TournamentSystem() {
        regManager = new RegistrationManager(40);
        matchScheduler = new MatchScheduler();
        resultLogger = new GameResultLogger();
        checkedInPlayers = nullptr;
        teamQueue = nullptr;
        tournamentInitialized = false;
    }
    
    ~TournamentSystem() {
        delete regManager;
        delete matchScheduler;
        delete resultLogger;
        if (teamQueue) delete teamQueue;
    }
    
    // Initialize tournament with sample data
    void initializeTournament() {
        if (tournamentInitialized) {
            cout << "Tournament already initialized!\n";
            return;
        }
        
        cout << "\n=== INITIALIZING TOURNAMENT WITH SAMPLE DATA ===\n";
        
        // Create sample players with rankings 1-5
        Player players[] = {
            // Team 1 - APU Dragons (T1)
            Player("P001", "Alice Wong", 3, "early-bird", "APU", "T1"),
            Player("P002", "Bob Chen", 4, "regular", "APU", "T1"),
            Player("P003", "Carol Tan", 2, "early-bird", "APU", "T1"),
            Player("P004", "David Lim", 5, "wildcard", "APU", "T1"),
            Player("P005", "Eva Kumar", 3, "regular", "APU", "T1"),
            
            // Team 2 - UM Tigers (T2)
            Player("P006", "Frank Lee", 4, "early-bird", "UM", "T2"),
            Player("P007", "Grace Ng", 5, "regular", "UM", "T2"),
            Player("P008", "Henry Goh", 3, "wildcard", "UM", "T2"),
            Player("P009", "Ivy Lau", 5, "early-bird", "UM", "T2"),
            Player("P010", "Jack Tan", 4, "regular", "UM", "T2"),
            
            // Team 3 - USM Eagles (T3)
            Player("P011", "Kelly Ong", 1, "early-bird", "USM", "T3"),
            Player("P012", "Liam Chow", 2, "regular", "USM", "T3"),
            Player("P013", "Maya Singh", 2, "wildcard", "USM", "T3"),
            Player("P014", "Noah Kim", 3, "early-bird", "USM", "T3"),
            Player("P015", "Olivia Chen", 1, "regular", "USM", "T3"),
            
            // Team 4 - MMU Wolves (T4)
            Player("P016", "Peter Yap", 5, "early-bird", "MMU", "T4"),
            Player("P017", "Quinn Lee", 5, "regular", "MMU", "T4"),
            Player("P018", "Rachel Teo", 5, "wildcard", "MMU", "T4"),
            Player("P019", "Sam Wong", 5, "early-bird", "MMU", "T4"),
            Player("P020", "Tina Lim", 5, "regular", "MMU", "T4"),
            
            // Team 5 - UTAR Panthers (T5)
            Player("P021", "Uma Patel", 4, "early-bird", "UTAR", "T5"),
            Player("P022", "Victor Ng", 4, "regular", "UTAR", "T5"),
            Player("P023", "Wendy Koh", 3, "wildcard", "UTAR", "T5"),
            Player("P024", "Xavier Tan", 5, "early-bird", "UTAR", "T5"),
            Player("P025", "Yvonne Lee", 3, "regular", "UTAR", "T5"),
            
            // Team 6 - INTI Sharks (T6)
            Player("P026", "Zara Ahmed", 1, "early-bird", "INTI", "T6"),
            Player("P027", "Adam Chong", 1, "regular", "INTI", "T6"),
            Player("P028", "Bella Tan", 1, "wildcard", "INTI", "T6"),
            Player("P029", "Chris Lim", 1, "early-bird", "INTI", "T6"),
            Player("P030", "Diana Wong", 1, "regular", "INTI", "T6"),
            
            // Team 7 - HELP Hawks (T7)
            Player("P031", "Ethan Ong", 5, "early-bird", "HELP", "T7"),
            Player("P032", "Fiona Lau", 5, "regular", "HELP", "T7"),
            Player("P033", "George Kim", 4, "wildcard", "HELP", "T7"),
            Player("P034", "Hannah Chen", 5, "early-bird", "HELP", "T7"),
            Player("P035", "Ian Yap", 4, "regular", "HELP", "T7"),
            
            // Team 8 - TAYLOR Titans (T8)
            Player("P036", "Julia Teo", 3, "early-bird", "TAYLOR", "T8"),
            Player("P037", "Kevin Ng", 3, "regular", "TAYLOR", "T8"),
            Player("P038", "Luna Koh", 2, "wildcard", "TAYLOR", "T8"),
            Player("P039", "Mike Tan", 3, "early-bird", "TAYLOR", "T8"),
            Player("P040", "Nina Lee", 3, "regular", "TAYLOR", "T8")
        };
        
        // Register all players
        for (int i = 0; i < 40; i++) {
            regManager->registerPlayer(players[i]);
        }
        
        // Process check-ins
        regManager->processCheckIn();
        checkedInPlayers = regManager->getCheckedInPlayers();
        teamQueue = createTeamsFromPlayers(checkedInPlayers);
        
        tournamentInitialized = true;
        cout << "Tournament initialized successfully with " << teamQueue->size() << " teams!\n";
    }
    
    // TASK 1: Match Scheduling & Player Progression
    void executeTask1() {
        cout << "\n========================================\n";
        cout << "   TASK 1: MATCH SCHEDULING & PLAYER PROGRESSION\n";
        cout << "========================================\n";
        
        if (!tournamentInitialized) {
            cout << " Tournament not initialized! Please run initialization first.\n";
            return;
        }
        
        if (teamQueue->size() < 2) {
            cout << " Not enough teams for matches!\n";
            return;
        }
        
        int choice;
        do {
            cout << "\n--- TASK 1 MENU ---\n";
            cout << "1. Generate Qualifier Matches\n";
            cout << "2. Simulate Qualifier Round\n";
            cout << "3. Generate & Simulate Knockout Rounds\n";
            cout << "4. Display Tournament Bracket (Graph Traversal)\n";
            cout << "5. Show Tournament Statistics\n";
            cout << "6. Reset Tournament Matches\n";
            cout << "7. Back to Main Menu\n";
            cout << "Enter choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    cout << "\n=== GENERATING QUALIFIER MATCHES ===\n";
                    matchScheduler->generateQualifierMatches(teamQueue);
                    break;
                }
                case 2: {
                    cout << "\n=== SIMULATING QUALIFIER ROUND ===\n";
                    Queue<Team>* winners = matchScheduler->simulateMatches(teamQueue);
                    cout << "\nQualifier complete! " << winners->size() << " teams advanced.\n";
                    delete winners;
                    break;
                }
                case 3: {
                    cout << "\n=== COMPLETE TOURNAMENT SIMULATION ===\n";
                    
                    // First, reset tournament to ensure clean state
                    matchScheduler->resetTournament();
                    
                    // Generate and simulate qualifiers
                    cout << "\n--- QUALIFIER ROUND ---\n";
                    matchScheduler->generateQualifierMatches(teamQueue);
                    Queue<Team>* qualifierWinners = matchScheduler->simulateMatches(teamQueue);
                    
                    cout << "\nQualifier Results: " << qualifierWinners->size() << " teams advance\n";
                    
                    // Run knockout rounds until champion
                    Queue<Team>* currentRound = qualifierWinners;
                    int round = 1;
                    
                    while (currentRound->size() > 1 && round <= 10) { // Safety limit
                        cout << "\n--- KNOCKOUT ROUND " << round << " ---\n";
                        cout << "Teams in this round: " << currentRound->size() << "\n";
                        
                        // Generate knockout matches for current round
                        string stageName = "knockout-round-" + to_string(round);
                        
                        // Create temporary queue for this round's matches
                        Queue<Team> roundTeams;
                        while (!currentRound->isEmpty()) {
                            roundTeams.enqueue(currentRound->dequeue());
                        }
                        
                        // Generate matches for this specific round
                        while (roundTeams.size() >= 2) {
                            Team t1 = roundTeams.dequeue();
                            Team t2 = roundTeams.dequeue();
                            
                            // Simulate single match
                            cout << "Match: " << t1.teamName << " vs " << t2.teamName << "\n";
                            
                            // Simple winner determination (team with better avg ranking wins)
                            Team winner = (t1.getAverageRanking() < t2.getAverageRanking()) ? t1 : t2;
                            cout << "Winner: " << winner.teamName << "\n";
                            
                            currentRound->enqueue(winner);
                        }
                        
                        // Handle bye team
                        if (!roundTeams.isEmpty()) {
                            Team byeTeam = roundTeams.dequeue();
                            cout << "Bye: " << byeTeam.teamName << " advances automatically\n";
                            currentRound->enqueue(byeTeam);
                        }
                        
                        cout << "Round " << round << " complete. " << currentRound->size() << " teams advance.\n";
                        round++;
                    }
                    
                    // Announce champion
                    if (currentRound->size() == 1) {
                        Team champion = currentRound->front();
                        cout << "\n\n TOURNAMENT CHAMPION \n";
                        cout << "         " << champion.teamName << "\n";
                        cout << "    Average Ranking: " << champion.getAverageRanking() << "\n";
                        cout << "\n";
                    } else {
                        cout << "\nTournament ended with " << currentRound->size() << " teams remaining.\n";
                    }
                    
                    delete qualifierWinners;
                    break;
                }
                case 4: {
                    cout << "\n=== TOURNAMENT BRACKET & GRAPH TRAVERSAL ===\n";
                    matchScheduler->displayBracketTraversal();
                    matchScheduler->displayBracketStatus();
                    break;
                }
                case 5: {
                    matchScheduler->displayTournamentStats();
                    break;
                }
                case 6: {
                    matchScheduler->resetTournament();
                    cout << "Tournament matches reset!\n";
                    break;
                }
                case 7:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
            
            if (choice != 7) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        } while (choice != 7);
    }
    
    // TASK 2: Tournament Registration & Player Queueing
    void executeTask2() {
        cout << "\n========================================\n";
        cout << "   TASK 2: TOURNAMENT REGISTRATION & PLAYER QUEUEING\n";
        cout << "========================================\n";
        
        int choice;
        do {
            cout << "\n--- TASK 2 MENU ---\n";
            cout << "1. Register New Player\n";
            cout << "2. Process All Check-ins\n";
            cout << "3. Handle Player Withdrawal\n";
            cout << "4. Display Registration Status\n";
            cout << "5. View Checked-in Players\n";
            cout << "6. Export Players to CSV\n";
            cout << "7. Back to Main Menu\n";
            cout << "Enter choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    string id, name, regType, university, teamID;
                    int ranking;
                    
                    cout << "Enter Player ID: ";
                    cin >> id;
                    cout << "Enter Name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter Ranking (1-5): ";
                    cin >> ranking;
                    cout << "Enter Registration Type (early-bird/regular/wildcard): ";
                    cin >> regType;
                    cout << "Enter University: ";
                    cin >> university;
                    cout << "Enter Team ID (optional): ";
                    cin >> teamID;
                    
                    Player newPlayer(id, name, ranking, regType, university, teamID);
                    regManager->registerPlayer(newPlayer);
                    break;
                }
                case 2: {
                    regManager->processCheckIn();
                    checkedInPlayers = regManager->getCheckedInPlayers();
                    break;
                }
                case 3: {
                    string playerID;
                    cout << "Enter Player ID to withdraw: ";
                    cin >> playerID;
                    regManager->handleWithdrawal(playerID);
                    break;
                }
                case 4: {
                    regManager->displayStatus();
                    break;
                }
                case 5: {
                    cout << "\n=== CHECKED-IN PLAYERS ===\n";
                    if (checkedInPlayers) {
                        DoublyNode<Player>* current = checkedInPlayers->getHead();
                        int count = 1;
                        while (current != nullptr) {
                            Player p = current->data;
                            cout << count++ << ". " << p.name << " (" << p.playerID 
                                 << ") - Team: " << p.teamID << " - Rank: " << p.ranking << "\n";
                            current = current->next;
                        }
                    } else {
                        cout << "No players checked in yet!\n";
                    }
                    break;
                }
                case 6: {
                    if (checkedInPlayers) {
                        CSVHandler::writePlayersToCSV("players.csv", checkedInPlayers);
                    } else {
                        cout << "No players to export!\n";
                    }
                    break;
                }
                case 7:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
            
            if (choice != 7) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        } while (choice != 7);
    }
    
    // TASK 3: Live Stream & Spectator Queue Management
    void executeTask3() {
        cout << "\n========================================\n";
        cout << "   TASK 3: LIVE STREAM & SPECTATOR QUEUE MANAGEMENT\n";
        cout << "========================================\n";
        cout << " Note: Task 3 implementation would include:\n";
        cout << "   • VIP seating priority queue\n";
        cout << "   • Spectator overflow management\n";
        cout << "   • Live streaming slot allocation\n";
        cout << "   • Circular queue for rotating spectators\n";
        cout << "\n🔧 This is a placeholder for Task 3 functionality.\n";
        cout << "   Each team member should implement their assigned task.\n";
        
        cout << "\nPress Enter to return to main menu...";
        cin.ignore();
        cin.get();
    }
    
    // TASK 4: Game Result Logging & Performance History
    void executeTask4() {
        cout << "\n========================================\n";
        cout << "   TASK 4: GAME RESULT LOGGING & PERFORMANCE HISTORY\n";
        cout << "========================================\n";
        
        if (!tournamentInitialized) {
            cout << " Tournament not initialized! Please run initialization first.\n";
            cout << "\nPress Enter to return...";
            cin.ignore();
            cin.get();
            return;
        }
        
        int choice;
        do {
            cout << "\n--- TASK 4 MENU ---\n";
            cout << "1. Simulate & Log All Matches\n";
            cout << "2. View Recent Match Results (Stack - LIFO)\n";
            cout << "3. View Complete Match History (Queue - FIFO)\n";
            cout << "4. Display Player Performance Statistics\n";
            cout << "5. Display Team Performance Statistics\n";
            cout << "6. Export Results to CSV\n";
            cout << "7. View Statistics Summary\n";
            cout << "8. Back to Main Menu\n";
            cout << "Enter choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    simulateAndLogMatches(teamQueue);
                    break;
                }
                case 2: {
                    int count;
                    cout << "How many recent results to display? ";
                    cin >> count;
                    resultLogger->displayRecentResults(count);
                    break;
                }
                case 3: {
                    resultLogger->displayMatchHistory();
                    break;
                }
                case 4: {
                    resultLogger->displayPlayerStats();
                    break;
                }
                case 5: {
                    resultLogger->displayTeamStats();
                    break;
                }
                case 6: {
                    resultLogger->exportResultsToCSV();
                    break;
                }
                case 7: {
                    resultLogger->displayStatisticsSummary();
                    break;
                }
                case 8:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
            
            if (choice != 8) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        } while (choice != 8);
    }
    
    // Export all tournament data
    void exportAllData() {
        cout << "\n=== EXPORTING ALL TOURNAMENT DATA ===\n";
        
        if (!tournamentInitialized) {
            cout << " Tournament not initialized!\n";
            return;
        }
        
        CSVHandler::exportAllData(checkedInPlayers, matchScheduler->getAllMatches(), teamQueue);
        CSVHandler::writeMatchSummaryCSV("match_summary.csv", matchScheduler->getAllMatches());
        resultLogger->exportResultsToCSV();
        
        cout << " All data exported successfully!\n";
        cout << "Files created:\n";
        cout << "  players.csv\n";
        cout << "  matches.csv\n";
        cout << "  teams.csv\n";
        cout << "  match_summary.csv\n";
        cout << "  match_results.csv\n";
        cout << "  player_performance.csv\n";
    }
    
    // Create teams from players grouped by teamID
    Queue<Team>* createTeamsFromPlayers(Queue<Player>* players) {
        cout << "\n=== CREATING TEAMS FROM PLAYERS ===\n";
        
        Queue<Team>* teams = new Queue<Team>();
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
                
                cout << "✓ " << team.teamName << " (" << team.teamID << ") - Avg Rank: ";
                for (int j = 0; j < MAX_TEAM_SIZE; j++) {
                    team.addPlayer(teamPlayers[i][j]);
                }
                cout << team.getAverageRanking() << "\n";
                
                teams->enqueue(team);
            }
        }
        
        cout << "Created " << teams->size() << " complete teams.\n";
        return teams;
    }
    
    // Simulate and log matches for Task 4
    void simulateAndLogMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING & LOGGING DETAILED MATCH RESULTS ===\n";
        
        Queue<Match>* allMatches = matchScheduler->getAllMatches();
        
        if (allMatches->isEmpty()) {
            cout << " No matches to simulate! Generate matches first.\n";
            return;
        }
        
        Queue<Match> tempMatches;
        int matchNum = 1;
        
        while (!allMatches->isEmpty()) {
            Match match = allMatches->dequeue();
            
            if (match.status == "completed") {
                // Generate realistic match details
                string date = "2025-01-" + to_string(15 + matchNum);
                string duration = to_string(25 + (matchNum % 20)) + " minutes";
                
                // Select random MVP
                string mvpPlayerID = "P" + to_string((matchNum % 5) + 1 + 
                    ((match.winnerID == "T1") ? 0 : 
                     (match.winnerID == "T2") ? 5 : 
                     (match.winnerID == "T3") ? 10 : 
                     (match.winnerID == "T4") ? 15 : 
                     (match.winnerID == "T5") ? 20 : 
                     (match.winnerID == "T6") ? 25 : 
                     (match.winnerID == "T7") ? 30 : 35));
                
                // Log the detailed result
                resultLogger->logMatchResult(match, match.team1Score, match.team2Score, 
                                           date, duration, mvpPlayerID, 
                                           checkedInPlayers, allTeams);
                matchNum++;
            }
            
            tempMatches.enqueue(match);
        }
        
        // Restore matches queue
        while (!tempMatches.isEmpty()) {
            allMatches->enqueue(tempMatches.dequeue());
        }
        
        cout  << (matchNum - 1) << " matches logged successfully!\n";
    }
    
    // Main menu system
    void showMainMenu() {
        int choice;
        do {
            cout << "\n";
            cout << "================================================================\n";
            cout << "      ASIA PACIFIC UNIVERSITY ESPORTS CHAMPIONSHIP\n";
            cout << "                MANAGEMENT SYSTEM\n";
            cout << "================================================================\n";
            cout << "Status: " << (tournamentInitialized ? "✅ Initialized" : " Not Initialized") << "\n";
            cout << "Teams: " << (teamQueue ? to_string(teamQueue->size()) : "0") << "\n";
            cout << "================================================================\n";
            cout << "0. Initialize Tournament\n";
            cout << "1. TASK 1: Match Scheduling & Player Progression\n";
            cout << "2. TASK 2: Tournament Registration & Player Queueing\n";
            cout << "3. TASK 3: Live Stream & Spectator Queue Management\n";
            cout << "4. TASK 4: Game Result Logging & Performance History\n";
            cout << "5. Export All Data to CSV\n";
            cout << "6. Run Complete Tournament (Auto)\n";
            cout << "7. Exit System\n";
            cout << "================================================================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch (choice) {
                case 0:
                    initializeTournament();
                    break;
                case 1:
                    executeTask1();
                    break;
                case 2:
                    executeTask2();
                    break;
                case 3:
                    executeTask3();
                    break;
                case 4:
                    executeTask4();
                    break;
                case 5:
                    exportAllData();
                    break;
                case 6:
                    runCompleteTournament();
                    break;
                case 7:
                    cout << "Exiting tournament system. Goodbye!\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
            
            if (choice != 7 && choice != 0) {
                cout << "\nPress Enter to return to main menu...";
                cin.ignore();
                cin.get();
            }
            
        } while (choice != 7);
    }
    
    // Run complete tournament automatically
    void runCompleteTournament() {
        cout << "\n=== RUNNING COMPLETE TOURNAMENT AUTOMATICALLY ===\n";
        
        if (!tournamentInitialized) {
            cout << "Initializing tournament first...\n";
            initializeTournament();
        }
        
        // Reset tournament state
        matchScheduler->resetTournament();
        
        // Task 1: Generate and simulate qualifier matches
        cout << "\n--- EXECUTING TASK 1: QUALIFIERS ---\n";
        matchScheduler->generateQualifierMatches(teamQueue);
        Queue<Team>* qualifierWinners = matchScheduler->simulateMatches(teamQueue);
        
        cout << "\nQualifier complete! " << qualifierWinners->size() << " teams advance to knockouts.\n";
        
        // Knockout rounds with proper progression
        cout << "\n--- KNOCKOUT STAGE ---\n";
        Queue<Team>* currentRound = qualifierWinners;
        int round = 1;
        
        while (currentRound->size() > 1 && round <= 10) { // Safety limit
            cout << "\n=== KNOCKOUT ROUND " << round << " ===\n";
            cout << "Teams competing: " << currentRound->size() << "\n";
            
            Queue<Team>* nextRound = new Queue<Team>();
            
            // Pair teams and determine winners
            while (currentRound->size() >= 2) {
                Team t1 = currentRound->dequeue();
                Team t2 = currentRound->dequeue();
                
                cout << "Match: " << t1.teamName << " vs " << t2.teamName << " - ";
                
                // Winner determination: team with better (lower) average ranking wins
                Team winner = (t1.getAverageRanking() < t2.getAverageRanking()) ? t1 : t2;
                Team loser = (winner.teamID == t1.teamID) ? t2 : t1;
                
                cout << "Winner: " << winner.teamName << "\n";
                nextRound->enqueue(winner);
            }
            
            // Handle bye team (odd number of teams)
            if (!currentRound->isEmpty()) {
                Team byeTeam = currentRound->dequeue();
                cout << "Bye: " << byeTeam.teamName << " advances automatically\n";
                nextRound->enqueue(byeTeam);
            }
            
            // Move to next round
            if (currentRound != qualifierWinners) delete currentRound;
            currentRound = nextRound;
            
            cout << "Round " << round << " complete. " << currentRound->size() << " teams advance.\n";
            round++;
        }
        
        // Announce champion
        if (currentRound->size() == 1) {
            Team champion = currentRound->front();
            cout << "\n    TOURNAMENT CHAMPION \n";
            cout << "         " << champion.teamName << "\n";
            cout << "    Average Team Ranking: " << champion.getAverageRanking() << "\n";
            cout << "    Players: ";
            for (int i = 0; i < champion.playerCount; i++) {
                cout << champion.players[i].name;
                if (i < champion.playerCount - 1) cout << ", ";
            }
        } else {
            cout << "\nTournament ended with " << currentRound->size() << " teams remaining.\n";
        }
        
        // Task 4: Log all results
        cout << "\n--- EXECUTING TASK 4: LOGGING RESULTS ---\n";
        simulateAndLogMatches(teamQueue);
        
        // Display final statistics
        cout << "\n--- TOURNAMENT SUMMARY ---\n";
        resultLogger->displayStatisticsSummary();
        resultLogger->displayTeamStats();
        
        // Export all data
        cout << "\n--- EXPORTING DATA ---\n";
        exportAllData();
        
        // Cleanup
        if (currentRound != qualifierWinners) delete currentRound;
        delete qualifierWinners;
        
        cout << "\n TOURNAMENT COMPLETED SUCCESSFULLY! \n";
    }
};

// ===============================
// MAIN FUNCTION
// ===============================

int main() {
    try {
        cout << "Starting Asia Pacific University Esports Championship System...\n";
        
        TournamentSystem tournament;
        tournament.showMainMenu();
        
    } catch (const exception& e) {
        cout << "\n ERROR OCCURRED \n";
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}