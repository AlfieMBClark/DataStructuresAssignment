#ifndef REGISTRATION_MANAGER_H
#define REGISTRATION_MANAGER_H

#include "DataStructs.h"
#include "Entities.h"
#include <iostream>

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
        
        while (!registrationQueue->isEmpty() && checkedInPlayers->size() < maxCapacity) {
            Player player = registrationQueue->dequeue();
            player.checkedIn = true;
            player.status = "active";
            checkedInPlayers->enqueue(player);
            
            cout << "Checked in: " << player.name << " (" 
                 << player.registrationType << ") - Status: " << player.status << "\n";
        }
        
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
        
        while (current != nullptr && !found) {
            if (current->data.playerID == playerID) {
                current->data.status = "eliminated";
                cout << "Player " << current->data.name << " has been marked as eliminated.\n";
                found = true;
            }
            current = current->next;
        }
        
        if (!waitlist->isEmpty()) {
            Player replacement = waitlist->dequeue();
            replacement.checkedIn = true;
            replacement.status = "active";
            checkedInPlayers->enqueue(replacement);
            
            cout << "Replacement: " << replacement.name 
                 << " moved from waitlist to active participants.\n";
        }
        
        if (!found) {
            cout << "Player with ID " << playerID << " not found in active participants.\n";
        }
    }
    
    Queue<Player>* getCheckedInPlayers() {
        return checkedInPlayers;
    }
    
    void displayStatus() {
        cout << "\n=== REGISTRATION STATUS ===\n";
        cout << "Checked-in players: " << checkedInPlayers->size() << "\n";
        cout << "Waitlisted players: " << waitlist->size() << "\n";
        cout << "Remaining slots: " << (maxCapacity - checkedInPlayers->size()) << "\n";
    }
};

#endif // REGISTRATION_MANAGER_H