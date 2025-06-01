#include "DataStructs.h"
#include "Entities.h"
#include "RegistrationManager.h"
#include "MatchScheduler.h"
#include "CSVHandler.h"
#include <iostream>
#include <exception>

using namespace std;

// ===============================
// MAIN TOURNAMENT SYSTEM CLASS
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
    
    // Create teams from players grouped by teamID
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
    
    void runTournament() {
        cout << "=== ASIA PACIFIC UNIVERSITY ESPORTS CHAMPIONSHIP ===\n";
        cout << "=== USING GRAPH-BASED TOURNAMENT BRACKET SYSTEM ===\n\n";
    
        // Create players with pre-assigned team IDs
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
            Player("P020", "Tina Lim", 29, "regular", "MMU", "T4"),
            
            // Team 5 - UTAR Panthers (T5)
            Player("P021", "Uma Patel", 17, "early-bird", "UTAR", "T5"),
            Player("P022", "Victor Ng", 19, "regular", "UTAR", "T5"),
            Player("P023", "Wendy Koh", 13, "wildcard", "UTAR", "T5"),
            Player("P024", "Xavier Tan", 21, "early-bird", "UTAR", "T5"),
            Player("P025", "Yvonne Lee", 15, "regular", "UTAR", "T5"),
            
            // Team 6 - INTI Sharks (T6)
            Player("P026", "Zara Ahmed", 3, "early-bird", "INTI", "T6"),
            Player("P027", "Adam Chong", 4, "regular", "INTI", "T6"),
            Player("P028", "Bella Tan", 2, "wildcard", "INTI", "T6"),
            Player("P029", "Chris Lim", 1, "early-bird", "INTI", "T6"),
            Player("P030", "Diana Wong", 5, "regular", "INTI", "T6"),
            
            // Team 7 - HELP Hawks (T7)
            Player("P031", "Ethan Ong", 24, "early-bird", "HELP", "T7"),
            Player("P032", "Fiona Lau", 27, "regular", "HELP", "T7"),
            Player("P033", "George Kim", 23, "wildcard", "HELP", "T7"),
            Player("P034", "Hannah Chen", 26, "early-bird", "HELP", "T7"),
            Player("P035", "Ian Yap", 25, "regular", "HELP", "T7"),
            
            // Team 8 - TAYLOR Titans (T8)
            Player("P036", "Julia Teo", 35, "early-bird", "TAYLOR", "T8"),
            Player("P037", "Kevin Ng", 33, "regular", "TAYLOR", "T8"),
            Player("P038", "Luna Koh", 31, "wildcard", "TAYLOR", "T8"),
            Player("P039", "Mike Tan", 34, "early-bird", "TAYLOR", "T8"),
            Player("P040", "Nina Lee", 36, "regular", "TAYLOR", "T8")
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

        // Save to CSV using the complete CSVHandler
        cout << "\n=== SAVING TO CSV FILES ===\n";
        CSVHandler::exportAllData(checkedInPlayers, matchScheduler->getAllMatches(), teamQueue);

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
        cout << "=================================================================\n";
        
        TournamentSystem tournament;
        tournament.runTournament();
        
        cout << "\n=================================================================\n";
        cout << "               TOURNAMENT SYSTEM COMPLETED\n";
        cout << "=================================================================\n";
        cout << "=================================================================\n";
        cout << "📊 TOURNAMENT STATISTICS:\n";
        cout << "   • 40 Players across 8 Universities\n";
        cout << "   • 8 Teams (5 players each)\n";
        cout << "   • Multiple tournament rounds (Qualifiers → Knockouts)\n";
        cout << "   • Graph-based bracket with " << MAX_NODES << " node capacity\n";
        cout << "   • Priority-based registration with 3 tiers\n";
        cout << "=================================================================\n";
        
    } catch (const exception& e) {
        cout << "\n=== ERROR OCCURRED ===\n";
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    cout << "\n SYSTEM SHUTDOWN SUCCESSFUL\n";
    cout << "Thank you for using the Tournament Management System!\n";
    return 0;
}