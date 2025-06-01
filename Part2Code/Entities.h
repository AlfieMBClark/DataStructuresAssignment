// ===== FIXED ENTITIES.H =====
#ifndef ENTITIES_H
#define ENTITIES_H

#include "DataStructs.h"
#include <string>
#include <sstream>

using namespace std;

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
               registrationType + "," + (checkedIn ? "true" : "false") + "," + 
               university + "," + teamID + "," + status;
    }
};

// ===============================
// TEAM CLASS
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
    int team1Score;  // Added missing score fields
    int team2Score;  // Added missing score fields

    Match() : graphIndex(-1), team1Score(0), team2Score(0) {}

    Match(string id, string t1, string t2, string stg, string rnd = "round-1")
        : matchID(id), team1ID(t1), team2ID(t2), stage(stg), status("pending"), 
          round(rnd), graphIndex(-1), team1Score(0), team2Score(0) {}

    // Added missing setScores method
    void setScores(int score1, int score2) {
        team1Score = score1;
        team2Score = score2;
    }

    string toString() {
        return matchID + "," + team1ID + "," + team2ID + "," + stage + "," + 
               status + "," + winnerID + "," + round + "," + 
               to_string(team1Score) + "," + to_string(team2Score);
    }
};

#endif // ENTITIES_H

