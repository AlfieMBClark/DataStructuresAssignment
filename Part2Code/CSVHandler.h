#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include "DataStructs.h"
#include "Entities.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
    
    static void writeMatchesToCSV(const string& filename, Queue<Match>* matches) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        
        file << "MatchID,Team1ID,Team2ID,Stage,Status,WinnerID,Round\n";
        
        Queue<Match> tempQueue;
        int matchCount = 0;
        
        while (!matches->isEmpty()) {
            Match match = matches->dequeue();
            file << match.toString() << "\n";
            tempQueue.enqueue(match);
            matchCount++;
        }
        
        while (!tempQueue.isEmpty()) {
            matches->enqueue(tempQueue.dequeue());
        }
        
        file.close();
        cout << "Successfully exported " << matchCount << " matches to " << filename << endl;
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
};

#endif // CSV_HANDLER_H