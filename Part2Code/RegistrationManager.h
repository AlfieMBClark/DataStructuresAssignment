#ifndef REGISTRATION_MANAGER_H
#define REGISTRATION_MANAGER_H

#include "DataStructs.h"
#include "Entities.h"
#include <iostream>
#include <map>

using namespace std;

// ===============================
// TASK 2: TOURNAMENT REGISTRATION & PLAYER QUEUEING
// ===============================

class RegistrationManager {
private:
    PriorityQueue<Player>* registrationQueue;
    Queue<Player>* checkedInPlayers;
    Queue<Player>* waitlist;
    int maxCapacity;

public:
    RegistrationManager(int capacity = 64) : maxCapacity(capacity) {
        registrationQueue = new PriorityQueue<Player>();
        checkedInPlayers = new Queue<Player>();
        waitlist = new Queue<Player>();
    }
    
    ~RegistrationManager() {
        delete registrationQueue;
        delete checkedInPlayers;
        delete waitlist;
    }
    
    void registerPlayer(Player player) {
        int priority = player.getPriority();
        registrationQueue->enqueue(player, priority);
        cout << "Player " << player.name << " registered with " 
             << player.registrationType << " priority (Level: " << priority << ").\n";
    }
    
    void processCheckIn() {
        cout << "\n=== PROCESSING PLAYER CHECK-INS ===\n";

        // Track number of checked-in players per team
        map<string, int> teamCounts; // teamID -> count

        while (!registrationQueue->isEmpty() && checkedInPlayers->size() < maxCapacity) {
            Player player = registrationQueue->dequeue();
            string teamID = player.teamID;

            // Allow only 5 players per team
            if (!teamID.empty() && teamCounts[teamID] >= MAX_TEAM_SIZE) {
                player.status = "waiting";
                waitlist->enqueue(player);
                cout << "Added to waitlist: " << player.name << " (team " << teamID << ") - Team full\n";
                continue;
            }

            // Check in player
            player.checkedIn = true;
            player.status = "active";
            checkedInPlayers->enqueue(player);
            teamCounts[teamID]++; // Increment team count

            cout << "Checked in: " << player.name << " (" 
                << player.registrationType << ", team " << teamID << ") - Status: " << player.status << "\n";
        }

        // Add any remaining players to waitlist
        while (!registrationQueue->isEmpty()) {
            Player player = registrationQueue->dequeue();
            player.status = "waiting";
            waitlist->enqueue(player);
            cout << "Added to waitlist: " << player.name << " - Status: " << player.status << "\n";
        }

        cout << "Check-in complete. Checked in: " << checkedInPlayers->size() 
            << ", Waitlisted: " << waitlist->size() << "\n";
    }
    
    void handleWithdrawal(string playerID) {
        cout << "\n=== HANDLING PLAYER WITHDRAWAL ===\n";

        DoublyNode<Player>* current = checkedInPlayers->getHead();
        bool found = false;
        string withdrawnTeam = "";

        while (current != nullptr && !found) {
            if (current->data.playerID == playerID) {
                withdrawnTeam = current->data.teamID;  // Capture the team of the withdrawn player
                cout << "Player " << current->data.name << " from Team " << withdrawnTeam << " has been withdrawn.\n";
                found = true;

                // Remove the player 
                checkedInPlayers->removeNode(current);
                break;
            }
            current = current->next;
        }

        if (found) {
            // Find a replacement from waitlist with matching team
            bool replacementFound = false;
            int waitlistSize = waitlist->size();  // Prevent infinite loop

            for (int i = 0; i < waitlistSize; i++) {
                Player potentialReplacement = waitlist->dequeue();

                if (potentialReplacement.teamID == withdrawnTeam) {
                    potentialReplacement.checkedIn = true;
                    potentialReplacement.status = "active";
                    checkedInPlayers->enqueue(potentialReplacement);
                    cout << "Replacement: " << potentialReplacement.name << " (Team " << potentialReplacement.teamID << ") moved from waitlist to active participants.\n";
                    replacementFound = true;
                    break;
                } else {
                    // Put the player back into waitlist if not matching
                    waitlist->enqueue(potentialReplacement);
                }
            }

            if (!replacementFound) {
                cout << "No replacement available in waitlist for Team " << withdrawnTeam << ".\n";
            }
        } else {
            cout << "Player with ID " << playerID << " not found in active participants.\n";
        }
    }
    
    Queue<Player>* getCheckedInPlayers() {
        return checkedInPlayers;
    }
    Queue<Player>* getWaitingPlayers() {
        return waitlist;
    }
    
    void displayStatus() {
        cout << "\n=== REGISTRATION STATUS ===\n";
        cout << "Checked-in players: " << checkedInPlayers->size() << "\n";
        cout << "Waitlisted players: " << waitlist->size() << "\n";
        cout << "Remaining slots: " << (maxCapacity - checkedInPlayers->size()) << "\n";
    }
};

#endif // REGISTRATION_MANAGER_H