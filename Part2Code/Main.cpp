// ===== MAIN.CPP - TOURNAMENT SYSTEM =====
// Written by: Student Team
// Course: Data Structures 
// Assignment: Lab Work #2

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
    // Class members
    RegistrationManager* regManager;
    MatchScheduler* matchScheduler;
    GameResultLogger* resultLogger;
    Queue<Player>* checkedInPlayers;
    Queue<Team>* teamQueue;
    bool tournamentInitialized;
    
    int userChoice;

public:
    TournamentSystem() {
        regManager = new RegistrationManager(40);
        matchScheduler = new MatchScheduler();
        resultLogger = new GameResultLogger();
        checkedInPlayers = nullptr;
        teamQueue = nullptr;
        tournamentInitialized = false;
        userChoice = 0; // Initialize user choice
    }
    
    // Destructor
    ~TournamentSystem() {
        delete regManager;
        delete matchScheduler;
        delete resultLogger;
        if (teamQueue) {
            delete teamQueue;
        }
        regManager = nullptr;
        matchScheduler = nullptr;
        resultLogger = nullptr;
        teamQueue = nullptr;
    }
    

    void initializeTournament() {
        // Check if already initialized
        if (tournamentInitialized == true) {
            cout << "Tournament already initialized!\n";
            return;
        }
        
        cout << "\n=== INITIALIZING TOURNAMENT WITH SAMPLE DATA ===\n";
        
        Player players[40]; 
        
        // Team 1 - APU Dragons (T1) - Average ranking team
        players[0] = Player("P001", "Alice Wong", 3, "early-bird", "APU", "T1");
        players[1] = Player("P002", "Bob Chen", 4, "regular", "APU", "T1");
        players[2] = Player("P003", "Carol Tan", 2, "early-bird", "APU", "T1");
        players[3] = Player("P004", "David Lim", 5, "wildcard", "APU", "T1");
        players[4] = Player("P005", "Eva Kumar", 3, "regular", "APU", "T1");
        
        // Team 2 - UM Tigers (T2) - Lower ranking team
        players[5] = Player("P006", "Frank Lee", 4, "early-bird", "UM", "T2");
        players[6] = Player("P007", "Grace Ng", 5, "regular", "UM", "T2");
        players[7] = Player("P008", "Henry Goh", 3, "wildcard", "UM", "T2");
        players[8] = Player("P009", "Ivy Lau", 5, "early-bird", "UM", "T2");
        players[9] = Player("P010", "Jack Tan", 4, "regular", "UM", "T2");
        
        // Team 3 - USM Eagles (T3) - High ranking team
        players[10] = Player("P011", "Kelly Ong", 1, "early-bird", "USM", "T3");
        players[11] = Player("P012", "Liam Chow", 2, "regular", "USM", "T3");
        players[12] = Player("P013", "Maya Singh", 2, "wildcard", "USM", "T3");
        players[13] = Player("P014", "Noah Kim", 3, "early-bird", "USM", "T3");
        players[14] = Player("P015", "Olivia Chen", 1, "regular", "USM", "T3");
        
        // Team 4 - MMU Wolves (T4) - Lowest ranking team
        players[15] = Player("P016", "Peter Yap", 5, "early-bird", "MMU", "T4");
        players[16] = Player("P017", "Quinn Lee", 5, "regular", "MMU", "T4");
        players[17] = Player("P018", "Rachel Teo", 5, "wildcard", "MMU", "T4");
        players[18] = Player("P019", "Sam Wong", 5, "early-bird", "MMU", "T4");
        players[19] = Player("P020", "Tina Lim", 5, "regular", "MMU", "T4");
        
        // Team 5 - UTAR Panthers (T5) - Average-low ranking team
        players[20] = Player("P021", "Uma Patel", 4, "early-bird", "UTAR", "T5");
        players[21] = Player("P022", "Victor Ng", 4, "regular", "UTAR", "T5");
        players[22] = Player("P023", "Wendy Koh", 3, "wildcard", "UTAR", "T5");
        players[23] = Player("P024", "Xavier Tan", 5, "early-bird", "UTAR", "T5");
        players[24] = Player("P025", "Yvonne Lee", 3, "regular", "UTAR", "T5");
        
        // Team 6 - INTI Sharks (T6) - Highest ranking team (all rank 1!)
        players[25] = Player("P026", "Zara Ahmed", 1, "early-bird", "INTI", "T6");
        players[26] = Player("P027", "Adam Chong", 1, "regular", "INTI", "T6");
        players[27] = Player("P028", "Bella Tan", 1, "wildcard", "INTI", "T6");
        players[28] = Player("P029", "Chris Lim", 1, "early-bird", "INTI", "T6");
        players[29] = Player("P030", "Diana Wong", 1, "regular", "INTI", "T6");
        
        // Team 7 - HELP Hawks (T7) - Low ranking team
        players[30] = Player("P031", "Ethan Ong", 5, "early-bird", "HELP", "T7");
        players[31] = Player("P032", "Fiona Lau", 5, "regular", "HELP", "T7");
        players[32] = Player("P033", "George Kim", 4, "wildcard", "HELP", "T7");
        players[33] = Player("P034", "Hannah Chen", 5, "early-bird", "HELP", "T7");
        players[34] = Player("P035", "Ian Yap", 4, "regular", "HELP", "T7");
        
        // Team 8 - TAYLOR Titans (T8) - Average ranking team
        players[35] = Player("P036", "Julia Teo", 3, "early-bird", "TAYLOR", "T8");
        players[36] = Player("P037", "Kevin Ng", 3, "regular", "TAYLOR", "T8");
        players[37] = Player("P038", "Luna Koh", 2, "wildcard", "TAYLOR", "T8");
        players[38] = Player("P039", "Mike Tan", 3, "early-bird", "TAYLOR", "T8");
        players[39] = Player("P040", "Nina Lee", 3, "regular", "TAYLOR", "T8");
        
        // Register
        cout << "Registering players...\n";
        for (int i = 0; i < 40; i++) {
            regManager->registerPlayer(players[i]);
            if (i % 10 == 9) {
                cout << "Registered " << (i + 1) << " players so far...\n";
            }
        }
        
        // Process check-ins
        cout << "Processing check-ins...\n";
        regManager->processCheckIn();
        checkedInPlayers = regManager->getCheckedInPlayers();
        
        // teams
        cout << "Creating teams...\n";
        teamQueue = createTeamsFromPlayers(checkedInPlayers);
        
        tournamentInitialized = true;
        cout << "Tournament initialized successfully!\n";
        cout << "Total teams created: " << teamQueue->size() << "\n";
    }
    
    // TASK 1: Match Scheduling & Player Progression
     // TASK 1: Match Scheduling & Player Progression
    void executeTask1() {
        // Print header
        cout << "\n========================================\n";
        cout << "   TASK 1: MATCH SCHEDULING & PLAYER PROGRESSION\n";
        cout << "========================================\n";
        
        // Check if tournament is ready
        if (tournamentInitialized != true) {
            cout << "ERROR: Tournament not initialized! Please run initialization first.\n";
            return;
        }
        
        // Check team count
        if (teamQueue->size() < 2) {
            cout << "ERROR: Not enough teams for matches!\n";
            cout << "Need at least 2 teams, but only have " << teamQueue->size() << " teams.\n";
            return;
        }
        
        // Menu loop
        int choice = 0;
        bool keepRunning = true;
        
        while (keepRunning == true) {
            cout << "\n--- TASK 1 MENU ---\n";
            cout << "1. Generate Qualifier Matches\n";
            cout << "2. Simulate Qualifier Round\n";
            cout << "3. Generate & Simulate Complete Tournament\n";
            cout << "4. Display Tournament Bracket (Graph Traversal)\n";
            cout << "5. Show Tournament Statistics\n";
            cout << "6. Reset Tournament Matches\n";
            cout << "7. Back to Main Menu\n";
            cout << "Enter choice: ";
            cin >> choice;
            
            // Process user choice
            if (choice == 1) {
                cout << "\n=== GENERATING QUALIFIER MATCHES ===\n";
                matchScheduler->generateQualifierMatches(teamQueue);
                cout << "Qualifier matches generated successfully!\n";
            }
            else if (choice == 2) {
                cout << "\n=== SIMULATING QUALIFIER ROUND ===\n";
                Queue<Team>* winners = matchScheduler->simulateMatches(teamQueue);
                cout << "\nQualifier simulation complete!\n";
                cout << "Number of teams advanced: " << winners->size() << "\n";
                
                // Clean up memory
                delete winners;
                winners = nullptr;
            }
            else if (choice == 3) {
                cout << "\n=== COMPLETE TOURNAMENT SIMULATION ===\n";
                
                // Reset first to ensure clean state
                cout << "Resetting tournament state...\n";
                matchScheduler->resetTournament();
                
                // Generate and simulate qualifiers
                cout << "\n--- QUALIFIER ROUND ---\n";
                matchScheduler->generateQualifierMatches(teamQueue);
                Queue<Team>* qualifierWinners = matchScheduler->simulateMatches(teamQueue);
                
                cout << "\nQualifier Results:\n";
                cout << "Teams advancing to knockout stage: " << qualifierWinners->size() << "\n";
                
                // Run knockout rounds until we have a champion
                Queue<Team>* currentRound = qualifierWinners;
                int roundNumber = 1;
                int maxRounds = 10; // Safety limit to prevent infinite loops
                
                while (currentRound->size() > 1 && roundNumber <= maxRounds) {
                    cout << "\n--- KNOCKOUT ROUND " << roundNumber << " ---\n";
                    cout << "Teams competing in this round: " << currentRound->size() << "\n";
                    
                    // Create new queue for next round winners
                    Queue<Team> roundTeams;
                    
                    // Move all teams from current round to temporary queue
                    while (currentRound->isEmpty() == false) {
                        Team team = currentRound->dequeue();
                        roundTeams.enqueue(team);
                    }
                    
                    // Generate matches for this specific round
                    int matchCount = 0;
                    while (roundTeams.size() >= 2) {
                        Team team1 = roundTeams.dequeue();
                        Team team2 = roundTeams.dequeue();
                        
                        // Simulate single match
                        cout << "Match " << (matchCount + 1) << ": " 
                             << team1.teamName << " vs " << team2.teamName << "\n";
                        
                        //Winner
                        Team winner;
                        Team loser;
                        int score1, score2;
                        int randomSeed = (roundNumber * 17 + matchCount * 23 + currentRound->size() * 31) % 1000;

                        if (randomSeed % 2 == 0) {
                            score1 = 13;
                            score2 = randomSeed % 13;
                        } else {
                            score1 = randomSeed % 13;
                            score2 = 13;
                        }

                        if (score1 > score2) {
                            winner = team1;
                        } else {
                            winner = team2;
                        }
                        
                        cout << "Winner: " << winner.teamName 
                             << " (Avg Rank: " << winner.getAverageRanking() << ")\n";
                        cout << "Eliminated: " << loser.teamName 
                             << " (Avg Rank: " << loser.getAverageRanking() << ")\n\n";
                        
                        // Adva winne
                        currentRound->enqueue(winner);
                        matchCount++;
                    }
                    
                    // odd num team
                    if (roundTeams.isEmpty() == false) {
                        Team byeTeam = roundTeams.dequeue();
                        cout << "Bye: " << byeTeam.teamName << " advances automatically\n";
                        currentRound->enqueue(byeTeam);
                    }
                    
                    cout << "Round " << roundNumber << " complete.\n";
                    cout << "Teams advancing to next round: " << currentRound->size() << "\n";
                    roundNumber = roundNumber + 1;
                }
                
                //campeones
                if (currentRound->size() == 1) {
                    Team champion = currentRound->front();
                    cout << "\n";
                    cout << "   TOURNAMENT CHAMPION \n";
                    cout << "      " << champion.teamName << "\n";
                    cout << "  Average Team Ranking: " << champion.getAverageRanking() << "\n";
                } else {
                    cout << "\nTournament ended with " << currentRound->size() 
                         << " teams remaining.\n";
                }
                
                // Memory cleanup
                delete qualifierWinners;
                qualifierWinners = nullptr;
            }
            else if (choice == 4) {
                cout << "\n=== TOURNAMENT BRACKET & GRAPH TRAVERSAL ===\n";
                matchScheduler->displayBracketTraversal();
                matchScheduler->displayBracketStatus();
            }
            else if (choice == 5) {
                cout << "\n=== TOURNAMENT STATISTICS ===\n";
                matchScheduler->displayTournamentStats();
            }
            else if (choice == 6) {
                cout << "\n=== RESETTING TOURNAMENT ===\n";
                matchScheduler->resetTournament();
                cout << "Tournament matches have been reset!\n";
            }
            else if (choice == 7) {
                cout << "Returning to main menu...\n";
                keepRunning = false;
            }
            else {
                cout << "Invalid choice! Please enter a number between 1 and 7.\n";
            }
            
            if (choice != 7) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
    
    // TASK 2: Tournament Registration & Player Queueing
    void executeTask2() {
        cout << "\n========================================\n";
        cout << "   TASK 2: TOURNAMENT REGISTRATION & PLAYER QUEUEING\n";
        cout << "========================================\n";
        
        int choice = 0;
        bool continueMenu = true;
        
        while (continueMenu) {
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
            
            // Handle menu choice
            switch (choice) {
                case 1: {
                    // Register new player
                    string playerID, playerName, regType, university, teamID;
                    int ranking;
                    
                    cout << "\n=== REGISTER NEW PLAYER ===\n";
                    cout << "Enter Player ID: ";
                    cin >> playerID;
                    
                    cout << "Enter Player Name: ";
                    cin.ignore(); // Clear input buffer
                    getline(cin, playerName);
                    
                    cout << "Enter Ranking (1-5): ";
                    cin >> ranking;
                    
                    // Validate ranking
                    while (ranking < 1 || ranking > 5) {
                        cout << "Invalid ranking! Please enter a number between 1 and 5: ";
                        cin >> ranking;
                    }
                    
                    cout << "Enter Registration Type (early-bird/regular/wildcard): ";
                    cin >> regType;
                    
                    cout << "Enter University: ";
                    cin >> university;
                    
                    cout << "Enter Team ID (optional, press Enter to skip): ";
                    cin.ignore();
                    getline(cin, teamID);
                    
                    // Create and register player
                    Player newPlayer(playerID, playerName, ranking, regType, university, teamID);
                    regManager->registerPlayer(newPlayer);
                    
                    cout << "Player registered successfully!\n";
                    break;
                }
                case 2: {
                    cout << "\n=== PROCESSING CHECK-INS ===\n";
                    regManager->processCheckIn();
                    checkedInPlayers = regManager->getCheckedInPlayers();
                    cout << "Check-in processing completed!\n";
                    break;
                }
                case 3: {
                    string playerID;
                    cout << "\n=== HANDLE PLAYER WITHDRAWAL ===\n";
                    cout << "Enter Player ID to withdraw: ";
                    cin >> playerID;
                    
                    regManager->handleWithdrawal(playerID);
                    break;
                }
                case 4: {
                    cout << "\n=== REGISTRATION STATUS ===\n";
                    regManager->displayStatus();
                    break;
                }
                case 5: {
                    cout << "\n=== CHECKED-IN PLAYERS LIST ===\n";
                    if (checkedInPlayers != nullptr) {
                        DoublyNode<Player>* current = checkedInPlayers->getHead();
                        int playerNumber = 1;
                        
                        if (current == nullptr) {
                            cout << "No players checked in yet!\n";
                        } else {
                            cout << "List of checked-in players:\n";
                            cout << "No. | Player ID | Name | Team | Ranking\n";
                            cout << "----+-----------+------+------+--------\n";
                            
                            while (current != nullptr) {
                                Player player = current->data;
                                cout << playerNumber << "   | " << player.playerID 
                                     << "     | " << player.name << " | " << player.teamID 
                                     << "   | " << player.ranking << "\n";
                                current = current->next;
                                playerNumber++;
                            }
                        }
                    } else {
                        cout << "No checked-in players data available!\n";
                    }
                    break;
                }
                case 6: {
                    cout << "\n=== EXPORT PLAYERS TO CSV ===\n";
                    if (checkedInPlayers != nullptr) {
                        CSVHandler::writePlayersToCSV("players.csv", checkedInPlayers);
                        cout << "Players exported successfully!\n";
                    } else {
                        cout << "No players to export!\n";
                    }
                    break;
                }
                case 7: {
                    cout << "Returning to main menu...\n";
                    continueMenu = false;
                    break;
                }
                default: {
                    cout << "Invalid choice! Please try again.\n";
                    break;
                }
            }
            
            if (choice != 7) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
    
    // TASK 3: Live Stream & Spectator Queue Management (Placeholder)
    void executeTask3() {
        cout << "\n========================================\n";
        cout << "   TASK 3: LIVE STREAM & SPECTATOR QUEUE MANAGEMENT\n";
        cout << "========================================\n";
        
        cout << "📝 NOTE: This is a placeholder for Task 3 functionality.\n";
        cout << "\nThis task would typically include:\n";
        cout << "  • VIP seating priority queue management\n";
        cout << "  • Spectator overflow handling with circular queues\n";
        cout << "  • Live streaming slot allocation system\n";
        cout << "  • Queue management for different spectator categories\n";
        cout << "\n🔧 Implementation Note:\n";
        cout << "   Each team member should implement their assigned task.\n";
        cout << "   This placeholder shows where Task 3 functionality would go.\n";
        
        cout << "\nPress Enter to return to main menu...";
        cin.ignore();
        cin.get();
    }
    
    // TASK 4: Game Result Logging & Performance History
    void executeTask4() {
        cout << "\n========================================\n";
        cout << "   TASK 4: GAME RESULT LOGGING & PERFORMANCE HISTORY\n";
        cout << "========================================\n";
        
        // Check if tournament is initialized
        if (tournamentInitialized == false) {
            cout << "ERROR: Tournament not initialized!\n";
            cout << "Please run initialization first (Option 0 from main menu).\n";
            cout << "\nPress Enter to return...";
            cin.ignore();
            cin.get();
            return;
        }
        
        int choice = 0;
        bool stayInMenu = true;
        
        while (stayInMenu == true) {
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
            
            // Process menu choice
            switch (choice) {
                case 1: {
                    cout << "\n=== SIMULATE & LOG MATCHES ===\n";
                    simulateAndLogMatches(teamQueue);
                    break;
                }
                case 2: {
                    int numberOfResults;
                    cout << "\n=== RECENT MATCH RESULTS ===\n";
                    cout << "How many recent results to display? ";
                    cin >> numberOfResults;
                    
                    // Validate input
                    if (numberOfResults <= 0) {
                        numberOfResults = 5; // Default value
                        cout << "Invalid input. Showing 5 results.\n";
                    }
                    
                    resultLogger->displayRecentResults(numberOfResults);
                    break;
                }
                case 3: {
                    cout << "\n=== COMPLETE MATCH HISTORY ===\n";
                    resultLogger->displayMatchHistory();
                    break;
                }
                case 4: {
                    cout << "\n=== PLAYER PERFORMANCE STATISTICS ===\n";
                    resultLogger->displayPlayerStats();
                    break;
                }
                case 5: {
                    cout << "\n=== TEAM PERFORMANCE STATISTICS ===\n";
                    resultLogger->displayTeamStats();
                    break;
                }
                case 6: {
                    cout << "\n=== EXPORT RESULTS TO CSV ===\n";
                    resultLogger->exportResultsToCSV();
                    cout << "Export completed!\n";
                    break;
                }
                case 7: {
                    cout << "\n=== STATISTICS SUMMARY ===\n";
                    resultLogger->displayStatisticsSummary();
                    break;
                }
                case 8: {
                    cout << "Returning to main menu...\n";
                    stayInMenu = false;
                    break;
                }
                default: {
                    cout << "Invalid choice! Please enter a number between 1 and 8.\n";
                    break;
                }
            }
            
            if (choice != 8) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
    
    //export all tournament data
    void exportAllData() {
        cout << "\n=== EXPORTING ALL TOURNAMENT DATA ===\n";
        
        // Check if tournament is initialized
        if (tournamentInitialized == false) {
            cout << "ERROR: Tournament not initialized!\n";
            cout << "Cannot export data without initialized tournament.\n";
            return;
        }
        
        cout << "Starting data export process...\n";
        
        // Export all data using CSV handler
        CSVHandler::exportAllData(checkedInPlayers, matchScheduler->getAllMatches(), teamQueue);
        
        // Export match summary
        CSVHandler::writeMatchSummaryCSV("match_summary.csv", matchScheduler->getAllMatches());
        
        // Export results from Task 4
        resultLogger->exportResultsToCSV();
        
        cout << "\n All data exported successfully!\n";
        cout << "Files created in current directory:\n";
        cout << "  • players.csv - Player information\n";
        cout << "  • matches.csv - Match results with scores\n";
        cout << "  • teams.csv - Team information\n";
        cout << "  • match_summary.csv - Formatted match summary\n";
        cout << "  • match_results.csv - Detailed match results\n";
        cout << "  • player_performance.csv - Player performance statistics\n";
        
        cout << "\nYou can open these files in Excel or any text editor.\n";
    }
    
    // create teams from players
    Queue<Team>* createTeamsFromPlayers(Queue<Player>* players) {
        cout << "\n=== CREATING TEAMS FROM REGISTERED PLAYERS ===\n";
        
        Queue<Team>* teamsQueue = new Queue<Team>();
        
        //organize players
        Player teamPlayers[8][MAX_TEAM_SIZE]; // 8 teams, 5 players each
        int teamPlayerCounts[8];
        string teamNames[8];
        
        // Init arrays
        for (int i = 0; i < 8; i++) {
            teamPlayerCounts[i] = 0;
            teamNames[i] = "";
        }
        
        // Group players by  ID
        DoublyNode<Player>* currentPlayer = players->getHead();
        
        while (currentPlayer != nullptr) {
            Player player = currentPlayer->data;
            
            // Check player has team ID
            if (player.teamID.empty() == false && player.teamID.length() >= 2) {
                int teamIndex = -1;
                
                // Determine team index based on team ID
                if (player.teamID == "T1") {
                    teamIndex = 0;
                } else if (player.teamID == "T2") {
                    teamIndex = 1;
                } else if (player.teamID == "T3") {
                    teamIndex = 2;
                } else if (player.teamID == "T4") {
                    teamIndex = 3;
                } else if (player.teamID == "T5") {
                    teamIndex = 4;
                } else if (player.teamID == "T6") {
                    teamIndex = 5;
                } else if (player.teamID == "T7") {
                    teamIndex = 6;
                } else if (player.teamID == "T8") {
                    teamIndex = 7;
                }
                
                // Add player to team if valid index and space available
                if (teamIndex >= 0 && teamIndex < 8 && teamPlayerCounts[teamIndex] < MAX_TEAM_SIZE) {
                    teamPlayers[teamIndex][teamPlayerCounts[teamIndex]] = player;
                    teamPlayerCounts[teamIndex] = teamPlayerCounts[teamIndex] + 1;
                    
                    // Set team name if not already set
                    if (teamNames[teamIndex] == "") {
                        if (player.university == "APU") {
                            teamNames[teamIndex] = "APU Dragons";
                        } else if (player.university == "UM") {
                            teamNames[teamIndex] = "UM Tigers";
                        } else if (player.university == "USM") {
                            teamNames[teamIndex] = "USM Eagles";
                        } else if (player.university == "MMU") {
                            teamNames[teamIndex] = "MMU Wolves";
                        } else if (player.university == "UTAR") {
                            teamNames[teamIndex] = "UTAR Panthers";
                        } else if (player.university == "INTI") {
                            teamNames[teamIndex] = "INTI Sharks";
                        } else if (player.university == "HELP") {
                            teamNames[teamIndex] = "HELP Hawks";
                        } else if (player.university == "TAYLOR") {
                            teamNames[teamIndex] = "TAYLOR Titans";
                        } else {
                            teamNames[teamIndex] = player.university + " Team";
                        }
                    }
                }
            }
            
            currentPlayer = currentPlayer->next;
        }
        
        // Create complete teams (only teams with exactly 5 players)
        int teamsCreated = 0;
        for (int i = 0; i < 8; i++) {
            if (teamPlayerCounts[i] == MAX_TEAM_SIZE) {
                // Create team object
                Team newTeam("T" + to_string(i + 1), teamNames[i]);
                
                cout << "Creating team: " << newTeam.teamName << " (" << newTeam.teamID << ")\n";
                cout << "  Players: ";
                
                // Add all players to team
                for (int j = 0; j < MAX_TEAM_SIZE; j++) {
                    bool playerAdded = newTeam.addPlayer(teamPlayers[i][j]);
                    if (playerAdded) {
                        cout << teamPlayers[i][j].name;
                        if (j < MAX_TEAM_SIZE - 1) {
                            cout << ", ";
                        }
                    }
                }
                
                cout << "\n  Average Ranking: " << newTeam.getAverageRanking() << "\n\n";
                
                // Add team to queue
                teamsQueue->enqueue(newTeam);
                teamsCreated++;
            } else {
                // Incomplete team
                if (teamPlayerCounts[i] > 0) {
                    cout << "Warning: Team " << teamNames[i] << " has only " 
                         << teamPlayerCounts[i] << " players (need " << MAX_TEAM_SIZE << ")\n";
                }
            }
        }
        
        cout << "Team creation completed!\n";
        cout << "Total complete teams created: " << teamsCreated << "\n";
        return teamsQueue;
    }
    

    void simulateAndLogMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING & LOGGING DETAILED MATCH RESULTS ===\n";
        
        //all matches
        Queue<Match>* allMatches = matchScheduler->getAllMatches();
        
        // Check matches to simulate
        if (allMatches->isEmpty() == true) {
            cout << "ERROR: No matches to simulate!\n";
            cout << "Please generate matches first using Task 1.\n";
            return;
        }
        
        // temp queue to preserve original matches
        Queue<Match> tempMatches;
        int matchNumber = 1;
        int matchesLogged = 0;
        
        cout << "Processing matches for logging...\n";
        
        // Process
        while (allMatches->isEmpty() == false) {
            Match currentMatch = allMatches->dequeue();
            
            //log completed matches
            if (currentMatch.status == "completed") {
                // Gen  details
                string matchDate = "2025-01-" + to_string(15 + matchNumber);
                string matchDuration = to_string(25 + (matchNumber % 20)) + " minutes";
                
                // Select MVP player ID based on winning team
                string mvpPlayerID = "P001"; // Default
                
                if (currentMatch.winnerID == "T1") {
                    mvpPlayerID = "P" + to_string((matchNumber % 5) + 1);
                } else if (currentMatch.winnerID == "T2") {
                    mvpPlayerID = "P" + to_string((matchNumber % 5) + 6);
                } else if (currentMatch.winnerID == "T3") {
                    mvpPlayerID = "P" + to_string((matchNumber % 5) + 11);
                } else if (currentMatch.winnerID == "T4") {
                    mvpPlayerID = "P" + to_string((matchNumber % 5) + 16);
                } else if (currentMatch.winnerID == "T5") {
                    mvpPlayerID = "P" + to_string((matchNumber % 5) + 21);
                } else if (currentMatch.winnerID == "T6") {
                    mvpPlayerID = "P" + to_string((matchNumber % 5) + 26);
                } else if (currentMatch.winnerID == "T7") {
                    mvpPlayerID = "P" + to_string((matchNumber % 5) + 31);
                } else if (currentMatch.winnerID == "T8") {
                    mvpPlayerID = "P" + to_string((matchNumber % 5) + 36);
                }
                
                // Log
                resultLogger->logMatchResult(currentMatch, 
                                           currentMatch.team1Score, 
                                           currentMatch.team2Score, 
                                           matchDate, 
                                           matchDuration, 
                                           mvpPlayerID, 
                                           checkedInPlayers, 
                                           allTeams);
                
                matchesLogged++;
                
                cout << "Logged match " << matchNumber << ": " 
                     << currentMatch.matchID << "\n";
            }
            
            // Add match back to temporary queue
            tempMatches.enqueue(currentMatch);
            matchNumber++;
        }
        
        // Restore original matches queue
        while (tempMatches.isEmpty() == false) {
            Match match = tempMatches.dequeue();
            allMatches->enqueue(match);
        }
        
        cout << "\nMatch logging completed!\n";
        cout << "Total matches logged: " << matchesLogged << "\n";
    }
    
   
    void showMainMenu() {
        int menuChoice = 0;
        bool keepRunning = true;
        
        // Main menu loop
        while (keepRunning == true) {
            cout << "\n";
            cout << "================================================================\n";
            cout << "      ASIA PACIFIC UNIVERSITY ESPORTS CHAMPIONSHIP\n";
            cout << "                MANAGEMENT SYSTEM\n";
            cout << "================================================================\n";
            
        
            if (tournamentInitialized == true) {
                cout << "Status: Tournament Initialized\n";
            } else {
                cout << "Status: Tournament Not Initialized\n";
            }
            
            if (teamQueue != nullptr) {
                cout << "Teams: " << teamQueue->size() << " teams ready\n";
            } else {
                cout << "Teams: 0 teams\n";
            }
            
            cout << "================================================================\n";
            cout << "MENU OPTIONS:\n";
            cout << "0. Initialize Tournament (Run this first!)\n";
            cout << "1. TASK 1: Match Scheduling & Player Progression\n";
            cout << "2. TASK 2: Tournament Registration & Player Queueing\n";
            cout << "3. TASK 3: Live Stream & Spectator Queue Management\n";
            cout << "4. TASK 4: Game Result Logging & Performance History\n";
            cout << "5. Export All Data to CSV Files\n";
            cout << "6. Run Complete Tournament (Automatic Mode)\n";
            cout << "7. Exit Tournament System\n";
            cout << "================================================================\n";
            cout << "Please enter your choice (0-7): ";
            cin >> menuChoice;
  
            if (menuChoice == 0) {
                cout << "\n=== INITIALIZING TOURNAMENT ===\n";
                initializeTournament();
            }
            else if (menuChoice == 1) {
                executeTask1();
            }
            else if (menuChoice == 2) {
                executeTask2();
            }
            else if (menuChoice == 3) {
                executeTask3();
            }
            else if (menuChoice == 4) {
                executeTask4();
            }
            else if (menuChoice == 5) {
                exportAllData();
            }
            else if (menuChoice == 6) {
                runCompleteTournament();
            }
            else if (menuChoice == 7) {
                cout << "\n=== EXITING TOURNAMENT SYSTEM ===\n";
                cout << "Thank you for using the APU Esports Championship System!\n";
                cout << "Goodbye!\n";
                keepRunning = false;
            }
            else {
                cout << "\nERROR: Invalid choice!\n";
                cout << "Please enter a number between 0 and 7.\n";
            }
            

            if (menuChoice != 7 && menuChoice != 0) {
                cout << "\nPress Enter to return to main menu...";
                cin.ignore();
                cin.get();
            }
        }
    }
    
   
    void runCompleteTournament() {
        cout << "\n=== RUNNING COMPLETE TOURNAMENT AUTOMATICALLY ===\n";
        
        // Check and initialize if needed
        if (tournamentInitialized == false) {
            cout << "Tournament not initialized. Initializing now...\n";
            initializeTournament();
        }
        
        // Reset tournament
        cout << "Resetting tournament state...\n";
        matchScheduler->resetTournament();
        
        // Gen & ism qualifier
        cout << "\n--- STEP 1: EXECUTING TASK 1 (QUALIFIERS) ---\n";
        matchScheduler->generateQualifierMatches(teamQueue);
        Queue<Team>* qualifierWinners = matchScheduler->simulateMatches(teamQueue);
        
        cout << "\nQualifier stage completed!\n";
        cout << "Teams advancing to knockout stage: " << qualifierWinners->size() << "\n";
        
        // Knockout stage
        cout << "\n--- STEP 2: KNOCKOUT STAGE ---\n";
        Queue<Team>* currentRound = qualifierWinners;
        int roundNumber = 1;
        int maxRounds = 10; // Safety limit
        
        // Continue unt champion
        while (currentRound->size() > 1 && roundNumber <= maxRounds) {
            cout << "\n=== KNOCKOUT ROUND " << roundNumber << " ===\n";
            cout << "Teams in this round: " << currentRound->size() << "\n";
            
            //queue round winners
            Queue<Team>* nextRoundWinners = new Queue<Team>();
            
            // Pair teams + ply matches
            while (currentRound->size() >= 2) {
                Team team1 = currentRound->dequeue();
                Team team2 = currentRound->dequeue();
                
                cout << "Match: " << team1.teamName << " vs " << team2.teamName << " -> ";
                
                // Gen scores 
                int score1, score2;
                int randomSeed = (roundNumber * 29 + currentRound->size() * 37) % 1000;
                
                if (randomSeed % 2 == 0) {
                    score1 = 13;
                    score2 = randomSeed % 13;
                } else {
                    score1 = randomSeed % 13;
                    score2 = 13;
                }
                
                // Determine winner
                Team matchWinner;
                if (score1 > score2) {
                    matchWinner = team1;
                    cout << team1.teamName << " " << score1 << "-" << score2 << " " << team2.teamName << " -> Winner: " << team1.teamName << "\n";
                } else {
                    matchWinner = team2;
                    cout << team1.teamName << " " << score1 << "-" << score2 << " " << team2.teamName << " -> Winner: " << team2.teamName << "\n";
                }
                
                nextRoundWinners->enqueue(matchWinner);
            }
        

            if (currentRound->isEmpty() == false) {
                Team byeTeam = currentRound->dequeue();
                cout << "Bye: " << byeTeam.teamName << " advances automatically\n";
                nextRoundWinners->enqueue(byeTeam);
            }
            
            // Clean round
            if (currentRound != qualifierWinners) {
                delete currentRound;
            }
            
            //next 
            currentRound = nextRoundWinners;
            
            cout << "Round " << roundNumber << " completed. " 
                 << currentRound->size() << " teams advance.\n";
            roundNumber++;
        }
        
        //tournchampion
        if (currentRound->size() == 1) {
            Team champion = currentRound->front();
            cout << "\n";
            cout << " TOURNAMENT CHAMPION \n";
            cout << "         " << champion.teamName << "\n";
            cout << "    Average Team Ranking: " << champion.getAverageRanking() << "\n";
            cout << "    Champion Players: ";
            
            for (int i = 0; i < champion.playerCount; i++) {
                cout << champion.players[i].name;
                if (i < champion.playerCount - 1) {
                    cout << ", ";
                }
            }
        } else {
            cout << "\nTournament ended with " << currentRound->size() 
                 << " teams remaining.\n";
        }
        
        // Log results
        cout << "\n--- STEP 3: EXECUTING TASK 4 (LOGGING RESULTS) ---\n";
        simulateAndLogMatches(teamQueue);
        
        // Display final tournament statistics
        cout << "\n--- STEP 4: TOURNAMENT SUMMARY ---\n";
        resultLogger->displayStatisticsSummary();
        resultLogger->displayTeamStats();
        
  
        cout << "\n--- STEP 5: EXPORTING DATA ---\n";
        exportAllData();
        
        //clean
        if (currentRound != qualifierWinners) {
            delete currentRound;
        }
        delete qualifierWinners;
        
        cout << "\n✅ COMPLETE TOURNAMENT FINISHED SUCCESSFULLY! ✅\n";
        cout << "All data has been exported to CSV files.\n";
    }
};

// ===============================
// MAIN FUNCTION
// ===============================

int main() {
    try {
        cout << "=== STARTING TOURNAMENT SYSTEM ===\n";
        cout << "Asia Pacific University Esports Championship Management System\n";
        cout << "Loading...\n\n";
        
        TournamentSystem* tournament = new TournamentSystem();
        
        tournament->showMainMenu();
        
        delete tournament;
        tournament = nullptr;
        
    } catch (const exception& error) {
        cout << "\n AN ERROR OCCURRED \n";
        cout << "Error Details: " << error.what() << endl;
        return 1; 
    }
    
    cout << "\nProgram terminated normally.\n";
    return 0;
}