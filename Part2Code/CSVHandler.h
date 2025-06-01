#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include "DataStructs.h"
#include "Entities.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Forward declarations to avoid circular dependencies
class Match;
class Player;
class Team;

// ===============================
// CSV HANDLER CLASS
// ===============================

class CSVHandler {
private:
    static bool matchesHeaderWritten;

public:
    static void writePlayersToCSV(const string& filename, Queue<Player>* players) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        
        // Write CSV header
        file << "PlayerID,Name,Ranking,RegistrationType,CheckedIn,University,TeamID,Status\n";
        
        // Create temporary queue to preserve original queue
        Queue<Player> tempQueue;
        int playerCount = 0;
        
        // Process all players
        while (!players->isEmpty()) {
            Player player = players->dequeue();
            file << player.toString() << "\n";
            tempQueue.enqueue(player);
            playerCount++;
        }
        
        // Restore original queue
        while (!tempQueue.isEmpty()) {
            players->enqueue(tempQueue.dequeue());
        }
        
        file.close();
        cout << "Successfully exported " << playerCount << " players to " << filename << endl;
    }
    
    // Initialize matches CSV file with header
    static void initializeMatchesCSV(const string& filename = "matches.csv") {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create matches file " << filename << endl;
            return;
        }
        
        // Write header
        file << "MatchID,Team1ID,Team2ID,Stage,Status,WinnerID,Round,Team1Score,Team2Score\n";
        file.close();
        matchesHeaderWritten = true;
        cout << "Initialized matches CSV file: " << filename << endl;
    }
    
    // Save single match result immediately after completion
    static void saveMatchResult(const Match& match, const string& filename = "matches.csv") {
        // Initialize file if header hasn't been written
        if (!matchesHeaderWritten) {
            initializeMatchesCSV(filename);
        }
        
        ofstream file(filename, ios::app); // Append mode
        if (!file.is_open()) {
            cout << "Error: Could not open matches file " << filename << endl;
            return;
        }
        
        // Write match data manually to avoid toString() dependency issues
        file << match.matchID << ","
             << match.team1ID << ","
             << match.team2ID << ","
             << match.stage << ","
             << match.status << ","
             << match.winnerID << ","
             << match.round << ","
             << match.team1Score << ","
             << match.team2Score << "\n";
        
        file.close();
        
        cout << "✓ Match " << match.matchID << " result saved to " << filename << endl;
    }
    
    static void writeMatchesToCSV(const string& filename, Queue<Match>* matches) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        
        // Updated header to include scores
        file << "MatchID,Team1ID,Team2ID,Stage,Status,WinnerID,Round,Team1Score,Team2Score\n";
        
        Queue<Match> tempQueue;
        int matchCount = 0;
        
        while (!matches->isEmpty()) {
            Match match = matches->dequeue();
            
            // Write match data manually instead of using toString()
            file << match.matchID << ","
                 << match.team1ID << ","
                 << match.team2ID << ","
                 << match.stage << ","
                 << match.status << ","
                 << match.winnerID << ","
                 << match.round << ","
                 << match.team1Score << ","
                 << match.team2Score << "\n";
            
            tempQueue.enqueue(match);
            matchCount++;
        }
        
        while (!tempQueue.isEmpty()) {
            matches->enqueue(tempQueue.dequeue());
        }
        
        file.close();
        cout << "Successfully exported " << matchCount << " matches with scores to " << filename << endl;
    }
    
    static void writeTeamsToCSV(const string& filename, Queue<Team>* teams) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        
        file << "TeamID,TeamName,PlayerCount,AverageRanking,Status\n";
        
        Queue<Team> tempQueue;
        int teamCount = 0;
        
        while (!teams->isEmpty()) {
            Team team = teams->dequeue();
            file << team.teamID << "," << team.teamName << "," 
                 << team.playerCount << "," << team.getAverageRanking() 
                 << "," << team.status << "\n";
            tempQueue.enqueue(team);
            teamCount++;
        }
        
        while (!tempQueue.isEmpty()) {
            teams->enqueue(tempQueue.dequeue());
        }
        
        file.close();
        cout << "Successfully exported " << teamCount << " teams to " << filename << endl;
    }
    
    // Additional utility function to export all data at once
    static void exportAllData(Queue<Player>* players, Queue<Match>* matches, Queue<Team>* teams) {
        cout << "\n=== EXPORTING ALL TOURNAMENT DATA TO CSV ===\n";
        writePlayersToCSV("players.csv", players);
        writeMatchesToCSV("matches.csv", matches);
        writeTeamsToCSV("teams.csv", teams);
        cout << "All tournament data exported successfully!\n";
    }
    
    // Export match results summary with scores
    static void writeMatchSummaryCSV(const string& filename, Queue<Match>* matches) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        
        file << "Match Summary with Score Details\n";
        file << "MatchID,Winner,Loser,Score,Stage,Round\n";
        
        Queue<Match> tempQueue;
        
        while (!matches->isEmpty()) {
            Match match = matches->dequeue();
            
            if (match.status == "completed") {
                string winner = match.winnerID;
                string loser = (match.winnerID == match.team1ID) ? match.team2ID : match.team1ID;
                string scoreDisplay = to_string(match.team1Score) + "-" + to_string(match.team2Score);
                
                file << match.matchID << "," << winner << "," << loser << "," 
                     << scoreDisplay << "," << match.stage << "," << match.round << "\n";
            }
            
            tempQueue.enqueue(match);
        }
        
        // Restore queue
        while (!tempQueue.isEmpty()) {
            matches->enqueue(tempQueue.dequeue());
        }
        
        file.close();
        cout << "Match summary with scores exported to " << filename << endl;
    }
};

// Initialize static member
bool CSVHandler::matchesHeaderWritten = false;

#endif // CSV_HANDLER_H