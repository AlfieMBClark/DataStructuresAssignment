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
    if (tournamentInitialized == true) {
        cout << "Tournament already initialized!\n";
        return;
    }
    
    cout << "\n=== INITIALIZING TOURNAMENT WITH 20 TEAMS (100 PLAYERS) ===\n";
    
    Player players[100]; 
        // TEAM 1 (T1) - APU Dragons
        players[0] = Player("P001", "Alice Wong", 3, "early-bird", "APU", "T1");
        players[1] = Player("P002", "Bob Chen", 4, "regular", "APU", "T1");
        players[2] = Player("P003", "Carol Tan", 2, "early-bird", "APU", "T1");
        players[3] = Player("P004", "David Lim", 5, "wildcard", "APU", "T1");
        players[4] = Player("P005", "Eva Kumar", 3, "regular", "APU", "T1");
        
        // TEAM 2 (T2) - UM Tigers
        players[5] = Player("P006", "Frank Lee", 4, "early-bird", "UM", "T2");
        players[6] = Player("P007", "Grace Ng", 5, "regular", "UM", "T2");
        players[7] = Player("P008", "Henry Goh", 3, "wildcard", "UM", "T2");
        players[8] = Player("P009", "Ivy Lau", 5, "early-bird", "UM", "T2");
        players[9] = Player("P010", "Jack Tan", 4, "regular", "UM", "T2");
        
        // TEAM 3 (T3) - USM Eagles
        players[10] = Player("P011", "Kelly Ong", 1, "early-bird", "USM", "T3");
        players[11] = Player("P012", "Liam Chow", 2, "regular", "USM", "T3");
        players[12] = Player("P013", "Maya Singh", 2, "wildcard", "USM", "T3");
        players[13] = Player("P014", "Noah Kim", 3, "early-bird", "USM", "T3");
        players[14] = Player("P015", "Olivia Chen", 1, "regular", "USM", "T3");
        
        // TEAM 4 (T4) - MMU Wolves
        players[15] = Player("P016", "Peter Yap", 5, "early-bird", "MMU", "T4");
        players[16] = Player("P017", "Quinn Lee", 5, "regular", "MMU", "T4");
        players[17] = Player("P018", "Rachel Teo", 5, "wildcard", "MMU", "T4");
        players[18] = Player("P019", "Sam Wong", 5, "early-bird", "MMU", "T4");
        players[19] = Player("P020", "Tina Lim", 5, "regular", "MMU", "T4");
        
        // TEAM 5 (T5) - UTAR Panthers
        players[20] = Player("P021", "Uma Patel", 4, "early-bird", "UTAR", "T5");
        players[21] = Player("P022", "Victor Ng", 4, "regular", "UTAR", "T5");
        players[22] = Player("P023", "Wendy Koh", 3, "wildcard", "UTAR", "T5");
        players[23] = Player("P024", "Xavier Tan", 5, "early-bird", "UTAR", "T5");
        players[24] = Player("P025", "Yvonne Lee", 3, "regular", "UTAR", "T5");
        
        // TEAM 6 (T6) - INTI Sharks
        players[25] = Player("P026", "Zara Ahmed", 1, "early-bird", "INTI", "T6");
        players[26] = Player("P027", "Adam Chong", 1, "regular", "INTI", "T6");
        players[27] = Player("P028", "Bella Tan", 1, "wildcard", "INTI", "T6");
        players[28] = Player("P029", "Chris Lim", 1, "early-bird", "INTI", "T6");
        players[29] = Player("P030", "Diana Wong", 1, "regular", "INTI", "T6");
        
        // TEAM 7 (T7) - HELP Hawks
        players[30] = Player("P031", "Ethan Ong", 5, "early-bird", "HELP", "T7");
        players[31] = Player("P032", "Fiona Lau", 5, "regular", "HELP", "T7");
        players[32] = Player("P033", "George Kim", 4, "wildcard", "HELP", "T7");
        players[33] = Player("P034", "Hannah Chen", 5, "early-bird", "HELP", "T7");
        players[34] = Player("P035", "Ian Yap", 4, "regular", "HELP", "T7");
        
        // TEAM 8 (T8) - TAYLOR Titans
        players[35] = Player("P036", "Julia Teo", 3, "early-bird", "TAYLOR", "T8");
        players[36] = Player("P037", "Kevin Ng", 3, "regular", "TAYLOR", "T8");
        players[37] = Player("P038", "Luna Koh", 2, "wildcard", "TAYLOR", "T8");
        players[38] = Player("P039", "Mike Tan", 3, "early-bird", "TAYLOR", "T8");
        players[39] = Player("P040", "Nina Lee", 3, "regular", "TAYLOR", "T8");
        
        // TEAM 9 (T9) - UKM Warriors
        players[40] = Player("P041", "Oscar Lim", 2, "early-bird", "UKM", "T9");
        players[41] = Player("P042", "Penny Wong", 4, "regular", "UKM", "T9");
        players[42] = Player("P043", "Quincy Tan", 3, "wildcard", "UKM", "T9");
        players[43] = Player("P044", "Ruby Chen", 1, "early-bird", "UKM", "T9");
        players[44] = Player("P045", "Steve Ng", 4, "regular", "UKM", "T9");
        
        // TEAM 10 (T10) - UPM Leopards
        players[45] = Player("P046", "Tiffany Koh", 5, "early-bird", "UPM", "T10");
        players[46] = Player("P047", "Ulrich Lee", 3, "regular", "UPM", "T10");
        players[47] = Player("P048", "Vera Patel", 2, "wildcard", "UPM", "T10");
        players[48] = Player("P049", "William Goh", 4, "early-bird", "UPM", "T10");
        players[49] = Player("P050", "Xenia Lau", 5, "regular", "UPM", "T10");
        
        // TEAM 11 (T11) - UTM Stallions
        players[50] = Player("P051", "Yusuf Kim", 3, "early-bird", "UTM", "T11");
        players[51] = Player("P052", "Zoe Yap", 1, "regular", "UTM", "T11");
        players[52] = Player("P053", "Aaron Teo", 4, "wildcard", "UTM", "T11");
        players[53] = Player("P054", "Bianca Ong", 2, "early-bird", "UTM", "T11");
        players[54] = Player("P055", "Carlos Singh", 5, "regular", "UTM", "T11");
        
        // TEAM 12 (T12) - UMS Phoenix
        players[55] = Player("P056", "Daphne Chow", 4, "early-bird", "UMS", "T12");
        players[56] = Player("P057", "Edward Tan", 3, "regular", "UMS", "T12");
        players[57] = Player("P058", "Felicia Wong", 1, "wildcard", "UMS", "T12");
        players[58] = Player("P059", "Gary Lim", 5, "early-bird", "UMS", "T12");
        players[59] = Player("P060", "Hazel Chen", 2, "regular", "UMS", "T12");
        
        // TEAM 13 (T13) - UNIMAS Lions
        players[60] = Player("P061", "Ivan Ng", 2, "early-bird", "UNIMAS", "T13");
        players[61] = Player("P062", "Jessica Koh", 4, "regular", "UNIMAS", "T13");
        players[62] = Player("P063", "Keith Lee", 3, "wildcard", "UNIMAS", "T13");
        players[63] = Player("P064", "Linda Ahmed", 1, "early-bird", "UNIMAS", "T13");
        players[64] = Player("P065", "Marcus Goh", 5, "regular", "UNIMAS", "T13");
        
        // TEAM 14 (T14) - UITM Cobras
        players[65] = Player("P066", "Natasha Lau", 3, "early-bird", "UITM", "T14");
        players[66] = Player("P067", "Oliver Kim", 4, "regular", "UITM", "T14");
        players[67] = Player("P068", "Patricia Yap", 2, "wildcard", "UITM", "T14");
        players[68] = Player("P069", "Quentin Teo", 5, "early-bird", "UITM", "T14");
        players[69] = Player("P070", "Rebecca Ong", 1, "regular", "UITM", "T14");
        
        // TEAM 15 (T15) - UCSI Eagles
        players[70] = Player("P071", "Sebastian Chow", 4, "early-bird", "UCSI", "T15");
        players[71] = Player("P072", "Tanya Tan", 3, "regular", "UCSI", "T15");
        players[72] = Player("P073", "Ursula Wong", 5, "wildcard", "UCSI", "T15");
        players[73] = Player("P074", "Vincent Lim", 2, "early-bird", "UCSI", "T15");
        players[74] = Player("P075", "Wendy Chen", 1, "regular", "UCSI", "T15");
        
        // TEAM 16 (T16) - SUNWAY Bears
        players[75] = Player("P076", "Xavier Ng", 2, "early-bird", "SUNWAY", "T16");
        players[76] = Player("P077", "Yasmin Koh", 4, "regular", "SUNWAY", "T16");
        players[77] = Player("P078", "Zachary Lee", 3, "wildcard", "SUNWAY", "T16");
        players[78] = Player("P079", "Amelia Ahmed", 5, "early-bird", "SUNWAY", "T16");
        players[79] = Player("P080", "Brandon Goh", 1, "regular", "SUNWAY", "T16");
        
        // TEAM 17 (T17) - MONASH Wolves
        players[80] = Player("P081", "Chloe Lau", 3, "early-bird", "MONASH", "T17");
        players[81] = Player("P082", "Derek Kim", 4, "regular", "MONASH", "T17");
        players[82] = Player("P083", "Emily Yap", 2, "wildcard", "MONASH", "T17");
        players[83] = Player("P084", "Felix Teo", 5, "early-bird", "MONASH", "T17");
        players[84] = Player("P085", "Grace Ong", 1, "regular", "MONASH", "T17");
        
        // TEAM 18 (T18) - CURTIN Sharks
        players[85] = Player("P086", "Hugo Chow", 4, "early-bird", "CURTIN", "T18");
        players[86] = Player("P087", "Iris Tan", 3, "regular", "CURTIN", "T18");
        players[87] = Player("P088", "Jake Wong", 1, "wildcard", "CURTIN", "T18");
        players[88] = Player("P089", "Kara Lim", 5, "early-bird", "CURTIN", "T18");
        players[89] = Player("P090", "Leo Chen", 2, "regular", "CURTIN", "T18");
        
        // TEAM 19 (T19) - SEGI Panthers
        players[90] = Player("P091", "Maya Ng", 2, "early-bird", "SEGI", "T19");
        players[91] = Player("P092", "Nathan Koh", 3, "regular", "SEGI", "T19");
        players[92] = Player("P093", "Ophelia Lee", 4, "wildcard", "SEGI", "T19");
        players[93] = Player("P094", "Paul Ahmed", 1, "early-bird", "SEGI", "T19");
        players[94] = Player("P095", "Quinn Goh", 5, "regular", "SEGI", "T19");
        
        // TEAM 20 (T20) - BINARY Hawks
        players[95] = Player("P096", "Rachel Lau", 3, "early-bird", "BINARY", "T20");
        players[96] = Player("P097", "Sam Kim", 4, "regular", "BINARY", "T20");
        players[97] = Player("P098", "Tara Yap", 2, "wildcard", "BINARY", "T20");
        players[98] = Player("P099", "Ulysses Teo", 5, "early-bird", "BINARY", "T20");
        players[99] = Player("P100", "Violet Ong", 1, "regular", "BINARY", "T20");
        
        cout << "Registering all 100 players...\n";
        for (int i = 0; i < 100; i++) {  // FIXED: Changed from 40 to 100
            regManager->registerPlayer(players[i]);
            if (i % 20 == 19) {  // Progress update every 20 players
                cout << "Registered " << (i + 1) << " players so far...\n";
            }
        }
        
        // Process check-ins and create teams
        cout << "Processing check-ins for all 100 players...\n";
        regManager->processCheckIn();
        checkedInPlayers = regManager->getCheckedInPlayers();
        cout << "Creating teams from 100 players...\n";
        teamQueue = createTeamsFromPlayers(checkedInPlayers);
        
        tournamentInitialized = true;
        cout << "Tournament initialized successfully with 20 teams!\n";
        cout << "Total teams created: " << teamQueue->size() << "\n";
        cout << "Total players registered: 100\n";
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
    
    Queue<Team>* createTeamsFromPlayers(Queue<Player>* players) {
        cout << "\n=== CREATING TEAMS FROM REGISTERED PLAYERS (AUTOMATIC) ===\n";
        Queue<Team>* teamsQueue = new Queue<Team>();
        Player teamPlayers[MAX_TEAMS][MAX_TEAM_SIZE];
        int teamPlayerCounts[MAX_TEAMS];
        string teamNames[MAX_TEAMS];
        
        for (int i = 0; i < MAX_TEAMS; i++) {
            teamPlayerCounts[i] = 0;
            teamNames[i] = "";
        }
        
        cout << "Scanning players and organizing into teams...\n";
        
        // Group by their team ID
        DoublyNode<Player>* currentPlayer = players->getHead();
        while (currentPlayer != nullptr) {
            Player player = currentPlayer->data;
            
            // has a valid team ID
            if (player.teamID.empty() == false && player.teamID.length() >= 2) {
                int teamIndex = -1;
                
                // team num from teamId
                if (player.teamID[0] == 'T' && player.teamID.length() >= 2) {
                    string numberPart = player.teamID.substr(1);
                    
                    try {
                        int teamNumber = stoi(numberPart);
                        if (teamNumber >= 1 && teamNumber <= MAX_TEAMS) {
                            teamIndex = teamNumber - 1;
                        }
                    } catch (const exception& e) {
                        cout << "Warning: Invalid team ID format: " << player.teamID << "\n";
                    }
                }
                
                // Add player to team
                if (teamIndex >= 0 && teamIndex < MAX_TEAMS && teamPlayerCounts[teamIndex] < MAX_TEAM_SIZE) {
                    teamPlayers[teamIndex][teamPlayerCounts[teamIndex]] = player;
                    teamPlayerCounts[teamIndex] = teamPlayerCounts[teamIndex] + 1;
                    
                    cout << "  Assigned " << player.name << " to team " << player.teamID << "\n";
                    
                    // Auto team name gen
                    if (teamNames[teamIndex] == "") {
                        teamNames[teamIndex] = generateTeamName(player.university, teamIndex + 1);
                    }
                } else {
                    cout << "Warning: Could not assign " << player.name 
                         << " to team " << player.teamID << " (invalid index or team full)\n";
                }
            } else {
                cout << "Warning: Player " << player.name << " has no valid team ID\n";
            }
            
            currentPlayer = currentPlayer->next;
        }
        
        // Create complete teams
        int teamsCreated = 0;
        cout << "\nCreating complete teams:\n";
        
        for (int i = 0; i < MAX_TEAMS; i++) {
            if (teamPlayerCounts[i] == MAX_TEAM_SIZE) {
                Team newTeam("T" + to_string(i + 1), teamNames[i]);
                cout << " Creating team: " << newTeam.teamName << " (" << newTeam.teamID << ")\n";
                cout << "  Players: ";
                
                //players to team
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
                
                // Add queue
                teamsQueue->enqueue(newTeam);
                teamsCreated++;
            } else if (teamPlayerCounts[i] > 0) {
                // Incomplete team
                cout << "  Incomplete team T" << (i + 1) << " (" << teamNames[i] 
                     << "): has " << teamPlayerCounts[i] << " players (needs " 
                     << MAX_TEAM_SIZE << ")\n";
            }
        }
        
        cout << "\n=== TEAM CREATION SUMMARY ===\n";
        cout << "Total complete teams created: " << teamsCreated << "\n";
        cout << "Teams ready for tournament: " << teamsQueue->size() << "\n";
        
        return teamsQueue;
    }
    
    string generateTeamName(const string& university, int teamNumber) {
        // Predefined team names for known universities
        if (university == "APU") {
            return "APU Dragons";
        } else if (university == "UM") {
            return "UM Tigers";
        } else if (university == "USM") {
            return "USM Eagles";
        } else if (university == "MMU") {
            return "MMU Wolves";
        } else if (university == "UTAR") {
            return "UTAR Panthers";
        } else if (university == "INTI") {
            return "INTI Sharks";
        } else if (university == "HELP") {
            return "HELP Hawks";
        } else if (university == "TAYLOR") {
            return "TAYLOR Titans";
        } else if (university == "UKM") {
            return "UKM Warriors";
        } else if (university == "UPM") {
            return "UPM Leopards";
        } else if (university == "UTM") {
            return "UTM Stallions";
        } else if (university == "UMS") {
            return "UMS Phoenix";
        } else if (university == "UNIMAS") {
            return "UNIMAS Lions";
        } else if (university == "UITM") {
            return "UITM Cobras";
        } else if (university == "UCSI") {
            return "UCSI Eagles";
        } else if (university == "SUNWAY") {
            return "SUNWAY Bears";
        } else if (university == "MONASH") {
            return "MONASH Wolves";
        } else if (university == "CURTIN") {
            return "CURTIN Sharks";
        } else if (university == "SEGI") {
            return "SEGI Panthers";
        } else if (university == "BINARY") {
            return "BINARY Hawks";
        } else {
            // team name for unknown
            return university + " Team " + to_string(teamNumber);
        }
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