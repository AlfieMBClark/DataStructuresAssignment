#ifndef TASK4_H
#define TASK4_H

#include "DataStructs.h"
#include "Entities.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

// ==================================================
// TASK 4: GAME RESULT LOGGING & PERFORMANCE HISTORY
// ==================================================

// Enhanced Match Result with detailed statistics
struct MatchResultDetails {
    string matchID;
    string team1ID;
    string team2ID;
    string winnerID;
    string loserID;
    int team1Score;
    int team2Score;
    string stage;
    string round;
    string date;
    string duration;
    string mvpPlayerID;
    
    MatchResultDetails() : team1Score(0), team2Score(0) {}
    
    MatchResultDetails(const Match& match, int score1, int score2, string d, string dur, string mvp)
        : matchID(match.matchID), team1ID(match.team1ID), team2ID(match.team2ID),
          winnerID(match.winnerID), team1Score(score1), team2Score(score2),
          stage(match.stage), round(match.round), date(d), duration(dur), mvpPlayerID(mvp) {
        
        loserID = (winnerID == team1ID) ? team2ID : team1ID;
    }
    
    string toString() {
        return matchID + "," + team1ID + "," + team2ID + "," + winnerID + "," +
               to_string(team1Score) + "," + to_string(team2Score) + "," + stage + "," +
               round + "," + date + "," + duration + "," + mvpPlayerID;
    }
};

// Player Performance Statistics
struct PlayerPerformanceStats {
    string playerID;
    string playerName;
    string teamID;
    int matchesPlayed;
    int matchesWon;
    int matchesLost;
    int totalScore;
    int mvpCount;
    double winRate;
    double averageScore;
    
    PlayerPerformanceStats() : matchesPlayed(0), matchesWon(0), matchesLost(0), 
                              totalScore(0), mvpCount(0), winRate(0.0), averageScore(0.0) {}
    
    PlayerPerformanceStats(string id, string name, string team)
        : playerID(id), playerName(name), teamID(team), matchesPlayed(0), matchesWon(0), 
          matchesLost(0), totalScore(0), mvpCount(0), winRate(0.0), averageScore(0.0) {}
    
    void updateStats() {
        if (matchesPlayed > 0) {
            winRate = (double)matchesWon / matchesPlayed * 100.0;
            averageScore = (double)totalScore / matchesPlayed;
        }
    }
    
    string toString() {
        return playerID + "," + playerName + "," + teamID + "," + 
               to_string(matchesPlayed) + "," + to_string(matchesWon) + "," + 
               to_string(matchesLost) + "," + to_string(totalScore) + "," +
               to_string(mvpCount) + "," + to_string(winRate) + "," + to_string(averageScore);
    }
};

// Team Performance Statistics
struct TeamPerformanceStats {
    string teamID;
    string teamName;
    int matchesPlayed;
    int matchesWon;
    int matchesLost;
    int totalScore;
    double winRate;
    double averageScore;
    
    TeamPerformanceStats() : matchesPlayed(0), matchesWon(0), matchesLost(0), 
                            totalScore(0), winRate(0.0), averageScore(0.0) {}
    
    TeamPerformanceStats(string id, string name)
        : teamID(id), teamName(name), matchesPlayed(0), matchesWon(0), 
          matchesLost(0), totalScore(0), winRate(0.0), averageScore(0.0) {}
    
    void updateStats() {
        if (matchesPlayed > 0) {
            winRate = (double)matchesWon / matchesPlayed * 100.0;
            averageScore = (double)totalScore / matchesPlayed;
        }
    }
};

// ===============================
// MAIN RESULT LOGGING CLASS
// ===============================

class GameResultLogger {
private:
    Stack<MatchResultDetails>* recentResults;        // Stack for recent match results (LIFO)
    Queue<MatchResultDetails>* matchHistory;         // Queue for chronological history (FIFO)
    PlayerPerformanceStats playerStats[MAX_PLAYERS]; // Array for player statistics
    TeamPerformanceStats teamStats[16];              // Array for team statistics
    int playerCount;
    int teamCount;
    int totalMatchesLogged;

public:
    GameResultLogger() {
        recentResults = new Stack<MatchResultDetails>();
        matchHistory = new Queue<MatchResultDetails>();
        playerCount = 0;
        teamCount = 0;
        totalMatchesLogged = 0;
    }
    
    ~GameResultLogger() {
        delete recentResults;
        delete matchHistory;
    }
    
    // Find player statistics index
    int findPlayerStats(const string& playerID) {
        for (int i = 0; i < playerCount; i++) {
            if (playerStats[i].playerID == playerID) {
                return i;
            }
        }
        return -1;
    }
    
    // Find team statistics index
    int findTeamStats(const string& teamID) {
        for (int i = 0; i < teamCount; i++) {
            if (teamStats[i].teamID == teamID) {
                return i;
            }
        }
        return -1;
    }
    
    // Create or update player statistics
    void updatePlayerStatistics(const MatchResultDetails& result, Queue<Player>* allPlayers) {
        // Get team rosters for this match
        DoublyNode<Player>* current = allPlayers->getHead();
        
        while (current != nullptr) {
            Player player = current->data;
            
            // Check if player is in either team
            if (player.teamID == result.team1ID || player.teamID == result.team2ID) {
                int index = findPlayerStats(player.playerID);
                
                // Create new player stats if not found
                if (index == -1) {
                    playerStats[playerCount] = PlayerPerformanceStats(player.playerID, player.name, player.teamID);
                    index = playerCount++;
                }
                
                // Update match count
                playerStats[index].matchesPlayed++;
                
                // Update wins/losses
                if (player.teamID == result.winnerID) {
                    playerStats[index].matchesWon++;
                    // Add score (winner gets their team's score)
                    if (result.team1ID == result.winnerID) {
                        playerStats[index].totalScore += result.team1Score;
                    } else {
                        playerStats[index].totalScore += result.team2Score;
                    }
                } else {
                    playerStats[index].matchesLost++;
                    // Add score (loser gets their team's score)
                    if (result.team1ID == player.teamID) {
                        playerStats[index].totalScore += result.team1Score;
                    } else {
                        playerStats[index].totalScore += result.team2Score;
                    }
                }
                
                // Check if MVP
                if (player.playerID == result.mvpPlayerID) {
                    playerStats[index].mvpCount++;
                }
                
                // Update calculated stats
                playerStats[index].updateStats();
            }
            
            current = current->next;
        }
    }
    
    // Update team statistics
    void updateTeamStatistics(const MatchResultDetails& result, Queue<Team>* allTeams) {
        // Update team1 stats
        int team1Index = findTeamStats(result.team1ID);
        if (team1Index == -1) {
            // Find team name from allTeams
            DoublyNode<Team>* current = allTeams->getHead();
            while (current != nullptr) {
                if (current->data.teamID == result.team1ID) {
                    teamStats[teamCount] = TeamPerformanceStats(result.team1ID, current->data.teamName);
                    team1Index = teamCount++;
                    break;
                }
                current = current->next;
            }
        }
        
        if (team1Index != -1) {
            teamStats[team1Index].matchesPlayed++;
            teamStats[team1Index].totalScore += result.team1Score;
            if (result.team1ID == result.winnerID) {
                teamStats[team1Index].matchesWon++;
            } else {
                teamStats[team1Index].matchesLost++;
            }
            teamStats[team1Index].updateStats();
        }
        
        // Update team2 stats
        int team2Index = findTeamStats(result.team2ID);
        if (team2Index == -1) {
            // Find team name from allTeams
            DoublyNode<Team>* current = allTeams->getHead();
            while (current != nullptr) {
                if (current->data.teamID == result.team2ID) {
                    teamStats[teamCount] = TeamPerformanceStats(result.team2ID, current->data.teamName);
                    team2Index = teamCount++;
                    break;
                }
                current = current->next;
            }
        }
        
        if (team2Index != -1) {
            teamStats[team2Index].matchesPlayed++;
            teamStats[team2Index].totalScore += result.team2Score;
            if (result.team2ID == result.winnerID) {
                teamStats[team2Index].matchesWon++;
            } else {
                teamStats[team2Index].matchesLost++;
            }
            teamStats[team2Index].updateStats();
        }
    }
    
    // Log a completed match result
    void logMatchResult(const Match& completedMatch, int score1, int score2, 
                       string date, string duration, string mvpPlayerID,
                       Queue<Player>* allPlayers, Queue<Team>* allTeams) {
        
        MatchResultDetails result(completedMatch, score1, score2, date, duration, mvpPlayerID);
        
        // Add to recent results stack (LIFO - most recent on top)
        recentResults->push(result);
        
        // Add to match history queue (FIFO - chronological order)
        matchHistory->enqueue(result);
        
        // Update player and team statistics
        updatePlayerStatistics(result, allPlayers);
        updateTeamStatistics(result, allTeams);
        
        totalMatchesLogged++;
        
        cout << "\n=== MATCH RESULT LOGGED ===\n";
        cout << "Match ID: " << result.matchID << "\n";
        cout << "Teams: " << result.team1ID << " (" << result.team1Score << ") vs " 
             << result.team2ID << " (" << result.team2Score << ")\n";
        cout << "Winner: " << result.winnerID << "\n";
        cout << "MVP: " << result.mvpPlayerID << "\n";
        cout << "Duration: " << result.duration << "\n";
        cout << "===========================\n";
    }
    
    // Display recent match results (Stack - LIFO)
    void displayRecentResults(int count = 5) {
        cout << "\n=== RECENT MATCH RESULTS (Latest First) ===\n";
        
        if (recentResults->isEmpty()) {
            cout << "No match results logged yet!\n";
            return;
        }
        
        Stack<MatchResultDetails> tempStack;
        int displayed = 0;
        
        // Pop from stack and display, then restore
        while (!recentResults->isEmpty() && displayed < count) {
            MatchResultDetails result = recentResults->pop();
            
            cout << (displayed + 1) << ". Match " << result.matchID 
                 << " (" << result.stage << ")\n";
            cout << "   " << result.team1ID << " (" << result.team1Score << ") vs " 
                 << result.team2ID << " (" << result.team2Score << ")\n";
            cout << "   Winner: " << result.winnerID << " | MVP: " << result.mvpPlayerID 
                 << " | Date: " << result.date << "\n\n";
            
            tempStack.push(result);
            displayed++;
        }
        
        // Restore stack
        while (!tempStack.isEmpty()) {
            recentResults->push(tempStack.pop());
        }
        
        cout << "==========================================\n";
    }
    
    // Display match history (Queue - FIFO chronological)
    void displayMatchHistory() {
        cout << "\n=== MATCH HISTORY (Chronological Order) ===\n";
        
        if (matchHistory->isEmpty()) {
            cout << "No match history available!\n";
            return;
        }
        
        Queue<MatchResultDetails> tempQueue;
        int count = 1;
        
        while (!matchHistory->isEmpty()) {
            MatchResultDetails result = matchHistory->dequeue();
            
            cout << count << ". " << result.matchID << " - " << result.stage 
                 << " (" << result.date << ")\n";
            cout << "   Result: " << result.team1ID << " " << result.team1Score 
                 << "-" << result.team2Score << " " << result.team2ID << "\n";
            cout << "   Winner: " << result.winnerID << " | Duration: " << result.duration << "\n\n";
            
            tempQueue.enqueue(result);
            count++;
        }
        
        // Restore queue
        while (!tempQueue.isEmpty()) {
            matchHistory->enqueue(tempQueue.dequeue());
        }
        
        cout << "=========================================\n";
    }
    
    // Display player performance statistics
    void displayPlayerStats() {
        cout << "\n=== PLAYER PERFORMANCE STATISTICS ===\n";
        
        if (playerCount == 0) {
            cout << "No player statistics available!\n";
            return;
        }
        
        cout << "Rank |Player ID| Name           | Team | Matches | W/L   | Win%   | Avg Score | MVP\n";
        cout << "-----|---------|----------------|------|---------|-------|--------|-----------|----\n";
        
        // Simple bubble sort by win rate
        for (int i = 0; i < playerCount - 1; i++) {
            for (int j = 0; j < playerCount - i - 1; j++) {
                if (playerStats[j].winRate < playerStats[j + 1].winRate) {
                    PlayerPerformanceStats temp = playerStats[j];
                    playerStats[j] = playerStats[j + 1];
                    playerStats[j + 1] = temp;
                }
            }
        }
        
        for (int i = 0; i < min(10, playerCount); i++) {
            PlayerPerformanceStats& player = playerStats[i];
            cout << " " << (i + 1) << "   | " << player.playerID << "    | ";
            
            // Pad name to 14 characters
            cout << player.playerName;
            for (int j = player.playerName.length(); j < 14; j++) cout << " ";
            
            cout << " | " << player.teamID << "   | " << player.matchesPlayed 
                 << "       | " << player.matchesWon << "/" << player.matchesLost 
                 << "   | " << player.winRate << "%   | " << player.averageScore 
                 << "        | " << player.mvpCount << "\n";
        }
        
        cout << "=====================================\n";
    }
    
    // Display team performance statistics
    void displayTeamStats() {
        cout << "\n=== TEAM PERFORMANCE STATISTICS ===\n";
        
        if (teamCount == 0) {
            cout << "No team statistics available!\n";
            return;
        }
        
        cout << "Rank | Team ID | Team Name          | Matches | W/L   | Win%   | Avg Score\n";
        cout << "-----|---------|--------------------|---------|-------|--------|---------\n";
        
        // Sort by win rate
        for (int i = 0; i < teamCount - 1; i++) {
            for (int j = 0; j < teamCount - i - 1; j++) {
                if (teamStats[j].winRate < teamStats[j + 1].winRate) {
                    TeamPerformanceStats temp = teamStats[j];
                    teamStats[j] = teamStats[j + 1];
                    teamStats[j + 1] = temp;
                }
            }
        }
        
        for (int i = 0; i < teamCount; i++) {
            TeamPerformanceStats& team = teamStats[i];
            cout << " " << (i + 1) << "   | " << team.teamID << "      | ";
            
            // Pad team name
            cout << team.teamName;
            for (int j = team.teamName.length(); j < 18; j++) cout << " ";
            
            cout << " | " << team.matchesPlayed << "       | " 
                 << team.matchesWon << "/" << team.matchesLost << "   | " 
                 << team.winRate << "%   | " << team.averageScore << "\n";
        }
        
        cout << "==================================\n";
    }
    
    // Export results to CSV
    void exportResultsToCSV() {
        // Export match results
        ofstream file("match_results.csv");
        if (file.is_open()) {
            file << "MatchID,Team1ID,Team2ID,WinnerID,Team1Score,Team2Score,Stage,Round,Date,Duration,MVP\n";
            
            Queue<MatchResultDetails> tempQueue;
            while (!matchHistory->isEmpty()) {
                MatchResultDetails result = matchHistory->dequeue();
                file << result.toString() << "\n";
                tempQueue.enqueue(result);
            }
            
            // Restore queue
            while (!tempQueue.isEmpty()) {
                matchHistory->enqueue(tempQueue.dequeue());
            }
            
            file.close();
            cout << "Match results exported to match_results.csv\n";
        }
        
        // Export player stats
        ofstream playerFile("player_performance.csv");
        if (playerFile.is_open()) {
            playerFile << "PlayerID,PlayerName,TeamID,MatchesPlayed,MatchesWon,MatchesLost,TotalScore,MVPCount,WinRate,AverageScore\n";
            
            for (int i = 0; i < playerCount; i++) {
                playerFile << playerStats[i].toString() << "\n";
            }
            
            playerFile.close();
            cout << "Player performance exported to player_performance.csv\n";
        }
    }
    
    // Get statistics summary
    void displayStatisticsSummary() {
        cout << "\n=== TOURNAMENT STATISTICS SUMMARY ===\n";
        cout << "Total matches logged: " << totalMatchesLogged << "\n";
        cout << "Players tracked: " << playerCount << "\n";
        cout << "Teams tracked: " << teamCount << "\n";
        cout << "Recent results available: " << recentResults->size() << "\n";
        cout << "Match history entries: " << matchHistory->size() << "\n";
        cout << "====================================\n";
    }
};

#endif // TASK4_H