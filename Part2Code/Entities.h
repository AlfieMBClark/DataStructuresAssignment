// ===== FIXED ENTITIES.H =====
#ifndef ENTITIES_H
#define ENTITIES_H

#include "DataStructs.h"
#include <string>
#include <sstream>
#include <vector>

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

// ===============================
// SPECTATOR CLASS
// ===============================

class Spectator {
private:
    string spectatorID;
    string name;
    string email;
    string spectatorType;  // vip,influencer,general
    int    priority;       // Higher number → higher priority
    string seatNumber;
    bool   checkedIn;
    string university;
    string ticketType;

public:
    // Default constructor
    Spectator()
      : spectatorID(""),
        name(""),
        email(""),
        spectatorType("General"),
        priority(1),
        seatNumber(""),
        checkedIn(false),
        university(""),
        ticketType("standard")
    {}

    // Parameterized constructor
    Spectator(const string& id,
              const string& spectatorName,
              const string& mail,
              const string& type,
              const string& uni = "")
      : spectatorID(id),
        name(spectatorName),
        email(mail),
        spectatorType(type),
        priority(1),          // will be overwritten by setPriorityByType()
        seatNumber(""),
        checkedIn(false),
        university(uni),
        ticketType("standard")
    {
        setPriorityByType();
    }

    //getters
    int    getPriority()      const { return priority;       }
    string getName()          const { return name;           }
    string getEmail()         const { return email;          }
    string getSpectatorType() const { return spectatorType;  }
    string getSeatNumber()    const { return seatNumber;     }
    bool   isCheckedIn()      const { return checkedIn;      }
    string getSpectatorID()   const { return spectatorID;    }
    string getUniversity()    const { return university;     }
    string getTicketType()    const { return ticketType;     }

    //setters
    void setSeatNumber(const string& seat) { seatNumber   = seat;      }
    void setCheckedIn(bool status)         { checkedIn    = status;    }
    void setTicketType(const string& t)    { ticketType   = t;         }
    void setUniversity(const string& u)    { university   = u;         }

    // Fill 'priority' based on spectatorType
    void setPriorityByType() {
        if      (spectatorType == "VIP")        priority = 3;
        else if (spectatorType == "Influencer") priority = 2;
        else                                    priority = 1; // General
    }

    //for csv format, if used 
    string toString() const {
        return spectatorID    + "," +
               name           + "," +
               email          + "," +
               spectatorType  + "," +
               to_string(priority)  + "," +
               (seatNumber.empty() ? "none" : seatNumber) + "," +
               (checkedIn ? "true" : "false") + "," +
               (university.empty() ? "none" : university) + "," +
               ticketType;
    }

    // Print a human‐readable summary to stdout
    void displaySpectator() const {
        cout << "ID: " << spectatorID
             << " | Name: " << name
             << " | Type: " << spectatorType
             << " | Priority: " << priority;
        if (!seatNumber.empty()) {
            cout << " | Seat: " << seatNumber;
        }
        cout << " | Checked In: " << (checkedIn ? "Yes" : "No");
        if (!university.empty()) {
            cout << " | Univ: " << university;
        }
        cout << " | Ticket: " << ticketType << "\n";
    }
};

// ===============================
// STREAMING SLOT CLASS
// ===============================

class StreamingSlot {
private:
    string slotID;
    string streamerName;
    string platform;        // Twitch,YouTube,Facebook
    int    viewerCapacity;
    int    currentViewers;
    bool   isActive;
    string streamKey;
    string status;
    string assignedMatch;

public:
    //Default constructor
    StreamingSlot()
      : slotID(""),
        streamerName(""),
        platform(""),
        viewerCapacity(1000),
        currentViewers(0),
        isActive(false),
        streamKey(""),
        status("inactive"),
        assignedMatch("")
    {}

    //Parameterized constructor
    StreamingSlot(const string& id,
                  const string& streamer,
                  const string& plat,
                  int capacity)
      : slotID(id),
        streamerName(streamer),
        platform(plat),
        viewerCapacity(capacity),
        currentViewers(0),
        isActive(false),
        streamKey(""),
        status("inactive"),
        assignedMatch("")
    {}

    //getters
    string getSlotID()       const { return slotID; }
    string getStreamerName() const { return streamerName; }
    string getPlatform()     const { return platform; }
    int    getViewerCapacity() const { return viewerCapacity; }
    int    getCurrentViewers() const { return currentViewers; }
    bool   getIsActive()     const { return isActive; }
    string getStreamKey()    const { return streamKey; }
    string getStatus()       const { return status; }
    string getAssignedMatch() const { return assignedMatch; }

    //setters 
    void setActive(bool activeStatus) {
        isActive = activeStatus;
        status = activeStatus ? "active" : "inactive";
    }

    void setCurrentViewers(int viewers) {
        if (viewers <= viewerCapacity) {
            currentViewers = viewers;
        }
    }

    void setStreamKey(const string& key) {
        streamKey = key;
    }

    void assignMatch(const string& matchID) {
        assignedMatch = matchID;
        if (!matchID.empty()) {
            setActive(true);
        }
    }

    
    bool canAcceptViewers() const {
        return isActive && (currentViewers < viewerCapacity);
    }

    int getAvailableCapacity() const {
        return viewerCapacity - currentViewers;
    }

    // Return a comma‐separated line if you ever need to save to CSV
    string toString() const {
        return slotID + "," +
               streamerName + "," +
               platform + "," +
               to_string(viewerCapacity) + "," +
               to_string(currentViewers) + "," +
               (isActive ? "true" : "false") + "," +
               streamKey + "," +
               status + "," +
               assignedMatch;
    }

    // Print a human‐readable summary
    void displaySlot() const {
        cout << "Slot ID: " << slotID
             << " | Streamer: " << streamerName
             << " | Platform: " << platform
             << " | Viewers: " << currentViewers << "/" << viewerCapacity
             << " | Active: " << (isActive ? "Yes" : "No");
        if (!assignedMatch.empty()) {
            cout << " | Match: " << assignedMatch;
        }
        cout << endl;
    }
};


// ===============================
// VIEWING SESSION CLASS
// ===============================

class ViewingSession {
public:
    string sessionID;
    string spectatorID;
    string streamingSlotID;
    string startTime;
    string endTime;
    string sessionType;     // "live,replay,highlight
    bool isActive;
    int duration;          // in minutes
    
    ViewingSession() : isActive(false), duration(0), sessionType("live") {}
    
    ViewingSession(string sessID, string specID, string slotID, string type = "live")
        : sessionID(sessID), spectatorID(specID), streamingSlotID(slotID),
          sessionType(type), isActive(true), duration(0), startTime(""), endTime("") {}
    
    void startSession(string time) {
        startTime = time;
        isActive = true;
    }
    
    void endSession(string time) {
        endTime = time;
        isActive = false;
        // Calculate duration if needed
    }
    
    bool getIsActive() const {
        return isActive;
    }
    
    string toString() const {
        return sessionID + "," + spectatorID + "," + streamingSlotID + "," +
               startTime + "," + endTime + "," + sessionType + "," +
               (isActive ? "true" : "false") + "," + to_string(duration);
    }
};

#endif // ENTITIES_H

