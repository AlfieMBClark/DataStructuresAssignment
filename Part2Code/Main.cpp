// ===== MAIN.CPP - TOURNAMENT SYSTEM =====
// Written by: Student Team
// Course: Data Structures 
// Assignment: Lab Work #2

#include "DataStructs.h"
#include "Entities.h"
#include "RegistrationManager.h"
#include "SpectatorManager.h"
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
        regManager = new RegistrationManager(100);
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
    


    void demonstrateTask3() {
    // 1) Create a SpectatorManager for a small venue (capacity 10)
    SpectatorManager manager(10);

    // 2) Construct a few Spectator objects
    //    (constructor signature: Spectator(id, name, email, type, [university]))
    Spectator vip1("S001", "Alice VIP",     "alice@vip.com",     "VIP");
    Spectator gen1("S002", "Bob General",   "bob@gen.com",       "General");
    Spectator inf1("S003", "Cara Influencer","cara@infl.com",    "Influencer");
    Spectator gen2("S004", "Dave Streamer", "dave@strm.com",     "General"); 
      // Note: “Streamer” here is treated as General. If you want to queue
      // streamers differently, adjust SpectatorType or manager logic accordingly.
    Spectator vip2("S005", "Eve VIP",       "eve@vip.com",       "VIP");
    Spectator inf2("S006", "Frank Influencer","frank@infl.com",  "Influencer");
    Spectator gen3("S007", "Grace General", "grace@gen.com",     "General");

    // 3) Register them with the manager (internally uses a priority queue)
    manager.registerSpectator(vip1);
    manager.registerSpectator(gen1);
    manager.registerSpectator(inf1);
    manager.registerSpectator(gen2);
    manager.registerSpectator(vip2);
    manager.registerSpectator(inf2);
    manager.registerSpectator(gen3);

    // 4) Process check-in until no more can be seated or queue empties
    cout << "\n--- Processing Check-Ins ---\n";
    while (manager.checkInSpectator()) {
        // Each call seats the highest-priority spectator until full or empty
    }

    // 5) Rotate two streaming slots (activate them in turn)
    cout << "\n--- Rotating Streaming Slots ---\n";
    manager.rotateStreamingSlot();
    manager.rotateStreamingSlot();

    // 6) If any spectators ended up in overflow (because venue was full),
    //    processOverflow() will seat them as soon as seats free. In this demo,
    //    no seats become free, but we call it to illustrate usage:
    cout << "\n--- Processing Overflow (if any) ---\n";
    manager.processOverflow();

    // 7) Finally, print out the entire manager status:
    //    • how many seats are filled / remaining
    //    • how many spectators remain in priority queue
    //    • how many in overflow
    //    • contents of priority queue and streaming slots
    cout << "\n--- FINAL SPECTATOR MANAGER STATUS ---\n";
    manager.displayStatus();

    // (End of Task 3 demo)
}

    // ===== UPDATED FUNCTIONS FOR 20-TEAM SUPPORT =====


    void initializeTournament() {
        // Remove the unnecessary check
        cout << "Creating teams...\n";
        
        // Always rebuild the teams
        teamQueue = createTeamsFromPlayers(checkedInPlayers);

        tournamentInitialized = true;
        cout << "Tournament initialized successfully!\n";
        cout << "Total teams created: " << teamQueue->size() << "\n";
    }
    
    // TASK 1: Match Scheduling & Player Progression
    void executeTask1() {
        cout << "\n========================================\n";
        cout << "   TASK 1: MATCH SCHEDULING & PLAYER PROGRESSION\n";
        cout << "========================================\n";
    
        if (tournamentInitialized != true) {
            cout << "ERROR: Run initialization first.\n";
            return;
        }
        
        //team count
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
                
                // Clean
                delete winners;
                winners = nullptr;
            }
            else if (choice == 3) {
                cout << "\n=== COMPLETE TOURNAMENT SIMULATION ===\n";
                
                // Reset
                cout << "Resetting tournament state...\n";
                matchScheduler->resetTournament();
                
                // Gen + sim qualifiers
                cout << "\n--- QUALIFIER ROUND ---\n";
                matchScheduler->generateQualifierMatches(teamQueue);
                Queue<Team>* qualifierWinners = matchScheduler->simulateMatches(teamQueue);
                
                cout << "\nQualifier Results:\n";
                cout << "Teams advancing to knockout stage: " << qualifierWinners->size() << "\n";
                
                //Knockout
                Queue<Team>* currentRound = qualifierWinners;
                int roundNumber = 1;
                int maxRounds = 10;
                
                while (currentRound->size() > 1 && roundNumber <= maxRounds) {
                    cout << "\n--- KNOCKOUT ROUND " << roundNumber << " ---\n";
                    cout << "Teams competing in this round: " << currentRound->size() << "\n";
                    
                    //queue for next round 
                    Queue<Team> roundTeams;
                    
                    // current round to tem
                    while (currentRound->isEmpty() == false) {
                        Team team = currentRound->dequeue();
                        roundTeams.enqueue(team);
                    }
                    
                    //round match
                    int matchCount = 0;
                    while (roundTeams.size() >= 2) {
                        Team team1 = roundTeams.dequeue();
                        Team team2 = roundTeams.dequeue();
                        

                        cout << "Match " << (matchCount + 1) << ": " << team1.teamName << " vs " << team2.teamName << "\n";
                        
                        //Winn
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
            cout << "1. Load Predefined Players\n";
            cout << "2. Register New Player\n";
            cout << "3. Process All Check-ins\n";
            cout << "4. Handle Player Withdrawal\n";
            cout << "5. Display Registration Status\n";
            cout << "6. View Checked-in Players\n";
            cout << "7. View Waiting Players\n";
            cout << "8. Export Players to CSV\n";
            cout << "9. Initialize Players\n";
            cout << "10. Back to Main Menu\n";
            cout << "Enter choice: ";
            cin >> choice;
            
            // Handle menu choice
            switch (choice) {

                case 1: {
                    // Load predefined players
                    cout << "\n=== LOADING PREDEFINED PLAYERS ===\n";
                        // Team 1 - APU Dragons (T1)
                        regManager->registerPlayer(Player("P001", "Alice Wong", 3, "early-bird", "APU", "T1"));
                        regManager->registerPlayer(Player("P002", "Bob Chen", 4, "regular", "APU", "T1"));
                        regManager->registerPlayer(Player("P003", "Carol Tan", 2, "early-bird", "APU", "T1"));
                        regManager->registerPlayer(Player("P004", "David Lim", 5, "wildcard", "APU", "T1"));
                        regManager->registerPlayer(Player("P005", "Eva Kumar", 3, "regular", "APU", "T1"));

                        // Team 2 - UM Tigers (T2)
                        regManager->registerPlayer(Player("P006", "Frank Lee", 4, "early-bird", "UM", "T2"));
                        regManager->registerPlayer(Player("P007", "Grace Ng", 5, "regular", "UM", "T2"));
                        regManager->registerPlayer(Player("P008", "Henry Goh", 3, "wildcard", "UM", "T2"));
                        regManager->registerPlayer(Player("P009", "Ivy Lau", 5, "early-bird", "UM", "T2"));
                        regManager->registerPlayer(Player("P010", "Jack Tan", 4, "regular", "UM", "T2"));

                        // Team 3 - USM Eagles (T3)
                        regManager->registerPlayer(Player("P011", "Kelly Ong", 1, "early-bird", "USM", "T3"));
                        regManager->registerPlayer(Player("P012", "Liam Chow", 2, "regular", "USM", "T3"));
                        regManager->registerPlayer(Player("P013", "Maya Singh", 2, "wildcard", "USM", "T3"));
                        regManager->registerPlayer(Player("P014", "Noah Kim", 3, "early-bird", "USM", "T3"));
                        regManager->registerPlayer(Player("P015", "Olivia Chen", 1, "regular", "USM", "T3"));

                        // Team 4 - MMU Wolves (T4)
                        regManager->registerPlayer(Player("P016", "Peter Yap", 5, "early-bird", "MMU", "T4"));
                        regManager->registerPlayer(Player("P017", "Quinn Lee", 5, "regular", "MMU", "T4"));
                        regManager->registerPlayer(Player("P018", "Rachel Teo", 5, "wildcard", "MMU", "T4"));
                        regManager->registerPlayer(Player("P019", "Sam Wong", 5, "early-bird", "MMU", "T4"));
                        regManager->registerPlayer(Player("P020", "Tina Lim", 5, "regular", "MMU", "T4"));

                        // Team 5 - UTAR Panthers (T5)
                        regManager->registerPlayer(Player("P021", "Uma Patel", 4, "early-bird", "UTAR", "T5"));
                        regManager->registerPlayer(Player("P022", "Victor Ng", 4, "regular", "UTAR", "T5"));
                        regManager->registerPlayer(Player("P023", "Wendy Koh", 3, "wildcard", "UTAR", "T5"));
                        regManager->registerPlayer(Player("P024", "Xavier Tan", 5, "early-bird", "UTAR", "T5"));
                        regManager->registerPlayer(Player("P025", "Yvonne Lee", 3, "regular", "UTAR", "T5"));

                        // Team 6 - INTI Sharks (T6)
                        regManager->registerPlayer(Player("P026", "Zara Ahmed", 1, "early-bird", "INTI", "T6"));
                        regManager->registerPlayer(Player("P027", "Adam Chong", 1, "regular", "INTI", "T6"));
                        regManager->registerPlayer(Player("P028", "Bella Tan", 1, "wildcard", "INTI", "T6"));
                        regManager->registerPlayer(Player("P029", "Chris Lim", 1, "early-bird", "INTI", "T6"));
                        regManager->registerPlayer(Player("P030", "Diana Wong", 1, "regular", "INTI", "T6"));

                        // Team 7 - HELP Hawks (T7)
                        regManager->registerPlayer(Player("P031", "Ethan Ong", 5, "early-bird", "HELP", "T7"));
                        regManager->registerPlayer(Player("P032", "Fiona Lau", 5, "regular", "HELP", "T7"));
                        regManager->registerPlayer(Player("P033", "George Kim", 4, "wildcard", "HELP", "T7"));
                        regManager->registerPlayer(Player("P034", "Hannah Chen", 5, "early-bird", "HELP", "T7"));
                        regManager->registerPlayer(Player("P035", "Ian Yap", 4, "regular", "HELP", "T7"));

                        // Team 8 - TAYLOR Titans (T8)
                        regManager->registerPlayer(Player("P036", "Julia Teo", 3, "early-bird", "TAYLOR", "T8"));
                        regManager->registerPlayer(Player("P037", "Kevin Ng", 3, "regular", "TAYLOR", "T8"));
                        regManager->registerPlayer(Player("P038", "Luna Koh", 2, "wildcard", "TAYLOR", "T8"));
                        regManager->registerPlayer(Player("P039", "Mike Tan", 3, "early-bird", "TAYLOR", "T8"));
                        regManager->registerPlayer(Player("P040", "Nina Lee", 3, "regular", "TAYLOR", "T8"));

                        cout << "Predefined players loaded successfully!\n";
                    
                    break;
                }
                case 2: {
                    // Register new player
                    string playerID, playerName, regType, university, teamID;
                    int ranking;
                    
                    cout << "\n=== REGISTER NEW PLAYER ===\n";
                    cout << "Enter Player ID (P041 and onwards): ";
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

                    cout << "Enter University:\n APU\n UM\n USM\n MMU\n UTAR\n INTI\n HELP\n TAYLOR\n: ";
                    cin >> university;
                    
                    cout << "Enter Team ID (T1 - T8 Depending on university):\n"
                    << "T1: APU Dragons\nT2: UM Tigers\nT3: USM Eagles\nT4: MMU Wolves\n"
                    << "T5: UTAR Panthers\nT6: INTI Sharks\nT7: HELP Hawks\nT8: TAYLOR Titans\n: ";

                    cin.ignore(); // clear buffer if needed

                    while (true) {
                        getline(cin, teamID);

                        if (teamID == "T1" || teamID == "T2" || teamID == "T3" || teamID == "T4" ||
                        teamID == "T5" || teamID == "T6" || teamID == "T7" || teamID == "T8") {
                            break; // Valid Team ID entered
                        } else {
                            cout << "Invalid Team ID! Please enter a valid option (T1-T8): ";
                        }
                    }

                    // Create and register player
                    Player newPlayer(playerID, playerName, ranking, regType, university, teamID);
                    regManager->registerPlayer(newPlayer);
    
                    cout << "Player registered successfully!\n";
                    break;
                }
                case 3: {
                    cout << "\n=== PROCESSING CHECK-INS ===\n";
                    regManager->processCheckIn();
                    checkedInPlayers = regManager->getCheckedInPlayers();
                    cout << "Check-in processing completed!\n";
                    break;
                }
                case 4: {
                    string playerID;
                    cout << "\n=== HANDLE PLAYER WITHDRAWAL ===\n";
                    cout << "Enter Player ID to withdraw: ";
                    cin >> playerID;
                    
                    regManager->handleWithdrawal(playerID);
                    initializeTournament();
                    break;
                }
                case 5: {
                    cout << "\n=== REGISTRATION STATUS ===\n";
                    regManager->displayStatus();
                    break;
                }
                case 6: {
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
                case 7: {
                    cout << "\n=== WAITING PLAYERS LIST ===\n";
                    if (regManager->getWaitingPlayers() != nullptr) {
                        DoublyNode<Player>* current = regManager->getWaitingPlayers()->getHead();
                        int playerNumber = 1;
                        
                        if (current == nullptr) {
                            cout << "No players waiting for check-in!\n";
                        } else {
                            cout << "List of waiting players:\n";
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
                        cout << "No waiting players data available!\n";
                    }
                    break;
                }
                case 8: {
                    cout << "\n=== EXPORT PLAYERS TO CSV ===\n";
                    if (checkedInPlayers != nullptr) {
                        CSVHandler::writePlayersToCSV("players.csv", checkedInPlayers);
                        cout << "Players exported successfully!\n";
                    } else {
                        cout << "No players to export!\n";
                    }
                    break;
                }
                case 9: {
                    cout << "\n=== INITIALIZING PLAYERS AND TEAMS ===\n";
                    initializeTournament();
                    break;
                }
                case 10: {
                    cout << "Returning to main menu...\n";
                    continueMenu = false;
                    break;
                }
                default: {
                    cout << "Invalid choice! Please try again.\n";
                    break;
                }
            }
            
            if (choice != 10) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
    
 // TASK 3: Live Stream & Spectator Queue Management
void executeTask3() {
    cout << "\n========================================\n";
    cout << "   TASK 3: LIVE STREAM & SPECTATOR QUEUE MANAGEMENT\n";
    cout << "========================================\n";

    SpectatorManager manager(10);  //Capacity
    
    // Initialize with some sample data for better demo
    loadSampleSpectators(manager);
    
    int choice = 0;
    bool continueMenu = true;

    while (continueMenu) {
        cout << "\n--- TASK 3 MENU ---\n";
        cout << "1. Register New Spectator\n";
        cout << "2. Process All Check-Ins (Seat Spectators)\n";
        cout << "3. Rotate One Streaming Slot\n";
        cout << "4. Process Overflowed Spectators\n";
        cout << "5. Display Spectator Management Status\n";
        cout << "6. Simulate Viewer Activity (Demo)\n";       
        cout << "7. Add Viewers to Specific Stream\n";          
        cout << "8. Display Streaming Status Only\n";            
        cout << "9. Back to Main Menu\n";                       
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                //new spectator by gathering their details
                string id, name, email, type;
                cout << "\n=== REGISTER NEW SPECTATOR ===\n";
                cout << "Enter Spectator ID: ";
                cin >> id;
                cout << "Enter Spectator Name: ";
                cin.ignore();           
                getline(cin, name);
                cout << "Enter Email: ";
                getline(cin, email);
                cout << "Enter Spectator Type (VIP/Influencer/General): ";
                getline(cin, type);

                // Construct and register
                Spectator newSpec(id, name, email, type);
                manager.registerSpectator(newSpec);

                cout << "Spectator registered successfully!\n";
                break;
            }
            case 2: {
                // Process
                cout << "\n=== PROCESSING CHECK-INS ===\n";
                int processed = 0;
                while (manager.checkInSpectator()) {
                    processed++;
                }
                cout << "Check-ins complete. Processed " << processed << " spectators.\n";
                break;
            }
            case 3: {
                cout << "\n=== ROTATING ONE STREAMING SLOT ===\n";
                manager.rotateStreamingSlot();
                manager.displayStreamingStatus();
                break;
            }
            case 4: {
                //spectators overflow, seat them now if seats freed
                cout << "\n=== PROCESSING OVERFLOWED SPECTATORS ===\n";
                manager.processOverflow();
                break;
            }
            case 5: {
                // Display current status (seatings, queues, streaming slots)
                cout << "\n=== SPECTATOR MANAGEMENT STATUS ===\n";
                manager.displayStatus();
                break;
            }
            case 6: {
                // NEW: Simulate viewer activity for demo
                cout << "\n=== SIMULATING VIEWER ACTIVITY ===\n";
                manager.simulateViewerActivity();
                cout << "Viewer activity simulation complete!\n";
                break;
            }
            case 7: {
                // NEW: Add viewers to specific platform
                cout << "\n=== ADD VIEWERS TO STREAM ===\n";
                string platform;
                int viewers;
                cout << "Enter platform (YouTube/Twitch/Facebook): ";
                cin >> platform;
                cout << "Enter number of viewers to add: ";
                cin >> viewers;
                
                manager.addViewersToStream(platform, viewers);
                break;
            }
            case 8: {
                // NEW: Display only streaming information
                cout << "\n=== STREAMING STATUS ===\n";
                manager.displayStreamingStatus();
                break;
            }
            case 9: {
                // Exit Task 3 menu
                cout << "Returning to main menu...\n";
                continueMenu = false;
                break;
            }
            default: {
                cout << "Invalid choice! Please enter a number between 1 and 9.\n";
                break;
            }
        }

        if (choice != 9) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}

// Helper function to load sample spectators for demo
void loadSampleSpectators(SpectatorManager& manager) {
    cout << "\n=== Loading Sample Spectators for Demo ===\n";
    
    //sample spectators
    Spectator vip1("VIP001", "Alice", "alice@email.com", "VIP");
    Spectator vip3("VIP003", "Leclerc", "alice@email.com", "VIP");
    Spectator vip2("VIP002", "Eve", "eve@email.com", "VIP");
    Spectator inf1("INF001", "Cara", "cara@email.com", "Influencer");
    Spectator inf2("INF002", "Frank", "frank@email.com", "Influencer");
    Spectator gen1("GEN001", "Bob", "bob@email.com", "General");
    Spectator gen2("GEN002", "Dave", "dave@email.com", "General");
    Spectator gen3("GEN003", "Grace", "grace@email.com", "General");
    Spectator gen4("GEN004", "Lewis", "lewis@email.com", "General");
    Spectator gen5("GEN005", "Max", "max@email.com", "General");
    
    // Register them
    manager.registerSpectator(vip1);
    manager.registerSpectator(gen1);
    manager.registerSpectator(inf1);
    manager.registerSpectator(vip2);
    manager.registerSpectator(vip3);
    manager.registerSpectator(gen2);
    manager.registerSpectator(inf2);
    manager.registerSpectator(gen3);
    manager.registerSpectator(gen4);
    manager.registerSpectator(gen5);

    
    cout << "Sample spectators loaded successfully!\n";
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
    
    //tournament data
    void exportAllData() {
        cout << "\n=== SAveing ALL TOURNAMENT DATA ===\n";
        
        if (tournamentInitialized == false) {
            cout << "Cannot export data without initialized tournament.\n";
            return;
        }
        
  
        CSVHandler::exportAllData(checkedInPlayers, matchScheduler->getAllMatches(), teamQueue);
        CSVHandler::writeMatchSummaryCSV("match_summary.csv", matchScheduler->getAllMatches());
        resultLogger->exportResultsToCSV();
        
        cout << "\n All data exported!\n";
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

                if (player.status == "eliminated") {
                    currentPlayer = currentPlayer->next;
                    continue;
                }      
            
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
        Queue<Match>* allMatches = matchScheduler->getAllMatches();
        
        // Check if matches to sim
        if (allMatches->isEmpty() == true) {
            cout << "ERROR: No matches to simulate!\n";
            cout << "Please generate matches first using Task 1.\n";
            return;
        }
        
        // total match count
        int totalMatches = allMatches->size();
        cout << "Processing " << totalMatches << " matches for logging...\n";
        
        if (totalMatches == 0) {
            cout << "No matches found to log.\n";
            return;
        }
        
        Match matchArray[50]; 
        int matchCount = 0;
        
        // Extract
        while (!allMatches->isEmpty() && matchCount < 50) {
            matchArray[matchCount] = allMatches->dequeue();
            matchCount++;
        }
        
        cout << "Extracted " << matchCount << " matches for processing.\n";
        
        // Process
        int matchesLogged = 0;
        for (int i = 0; i < matchCount; i++) {
            Match currentMatch = matchArray[i];
            
            cout << "Processing match " << (i + 1) << ": " << currentMatch.matchID << "\n";
            
            //log completed
            if (currentMatch.status == "completed") {
                try {
                    // Gen match details
                    string matchDate = "2025-01-" + to_string(15 + (i + 1));
                    string matchDuration = to_string(25 + ((i + 1) % 20)) + " minutes";
                    
                    // Select MVP player ID
                    string mvpPlayerID = generateMVPPlayerID(currentMatch.winnerID, i + 1);
                    
                    // Log 
                    resultLogger->logMatchResult(currentMatch, currentMatch.team1Score, currentMatch.team2Score,  matchDate,  matchDuration, mvpPlayerID, checkedInPlayers, allTeams);
                    
                    matchesLogged++;
                    cout << "Logged match " << (i + 1) << ": " << currentMatch.matchID << " (MVP: " << mvpPlayerID << ")\n";
                    
                } catch (const exception& e) {
                    cout << "Error logging match " << currentMatch.matchID << ": " << e.what() << "\n";
                    continue; 
                }
            } else {
                cout << "Skipping match " << currentMatch.matchID << " (status: " << currentMatch.status << ")\n";
            }
            
            if (i % 3 == 2) {
                cout << "--- Processed " << (i + 1) << " matches so far ---\n";
            }
        }
        
        // Restore matches
        for (int i = 0; i < matchCount; i++) {
            allMatches->enqueue(matchArray[i]);
        }
        
        cout << "\n=== MATCH LOGGING SUMMARY ===\n";
        cout << "Total matches processed: " << matchCount << "\n";
        cout << "Matches logged successfully: " << matchesLogged << "\n";
        cout << "Match logging completed!\n";
    }

    string selectMVPPlayer(const string& winnerTeamID, int matchNumber, Queue<Team>* allTeams) {
        //get winner
        DoublyNode<Team>* currentTeam = allTeams->getHead();
        
        while (currentTeam != nullptr) {
            Team team = currentTeam->data;
            
            if (team.teamID == winnerTeamID) {
                //select player
                if (team.playerCount > 0) {
                    int mvpIndex = matchNumber % team.playerCount;
                    return team.players[mvpIndex].playerID;
                }
                break;
            }
            currentTeam = currentTeam->next;
        }
        
        //Gen MVP ID randopm
        return generateMVPPlayerID(winnerTeamID, matchNumber);
    }
    
    //gen MVP rando
    string generateMVPPlayerID(const string& teamID, int matchNumber) {
        int teamNumber = 1;
        if (teamID.length() >= 2 && teamID[0] == 'T') {
            try {
                string numberPart = teamID.substr(1);
                teamNumber = stoi(numberPart);
                
                // Valid team number range
                if (teamNumber < 1 || teamNumber > 20) {
                    teamNumber = 1;
                }
            } catch (const exception& e) {
                teamNumber = 1;
            }
        }
        
        
        int basePlayerID = ((teamNumber - 1) * 5) + 1;
        // Select player fro team
        int playerOffset = matchNumber % 5; 
        int mvpPlayerID = basePlayerID + playerOffset;
        
        string mvpID = "P";
        if (mvpPlayerID < 10) {
            mvpID += "00" + to_string(mvpPlayerID);
        } else if (mvpPlayerID < 100) {
            mvpID += "0" + to_string(mvpPlayerID);
        } else {
            mvpID += to_string(mvpPlayerID);
        }
        
        if (mvpPlayerID < 1 || mvpPlayerID > 100) {
            return "P001"; 
        }
        
        return mvpID;
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
        if (tournamentInitialized == false) {
            cout << "Tournament not initialized. Initializing now...\n";
            initializeTournament();
        }
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
        int maxRounds = 10;
        
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
        
        cout << "\n--- STEP 3: TOURNAMENT RESULTS SUMMARY  ---\n";
        cout << "=== TOURNAMENT COMPLETED SUCCESSFULLY! ===\n";
        cout << " Total teams: 20\n";
        cout << " Total players: 100\n"; 
        cout << " Qualification matches: 10\n";
        cout << " Knockout rounds: " << (roundNumber - 1) << "\n";
        cout << " Tournament duration: Complete\n";

        cout << "\n--- STEP 4: DATA EXPORT  ---\n";
        try {
            if (checkedInPlayers && !checkedInPlayers->isEmpty()) {
                CSVHandler::writePlayersToCSV("players.csv", checkedInPlayers);
            }
            if (teamQueue && !teamQueue->isEmpty()) {
                CSVHandler::writeTeamsToCSV("teams.csv", teamQueue);
            }
            cout << " Basic tournament data exported successfully!\n";
        } catch (const exception& e) {
            cout << " Data export skipped: " << e.what() << "\n";
        }
        
        // Clean
        if (currentRound != qualifierWinners) {
            delete currentRound;
        }
        delete qualifierWinners;
        
        cout << "\n COMPLETE TOURNAMENT FINISHED SUCCESSFULLY! \n";
    }
};

// ===============================
// MAIN FUNCTION
// ===============================

int main() {
    try {
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