#ifndef SPECTATOR_MANAGER_H
#define SPECTATOR_MANAGER_H

#include "DataStructs.h"
#include "Entities.h"
#include <iostream>
#include <vector>    // for std::vector
#include <string>    // for std::string
using namespace std;

class SpectatorManager {
private:
    PriorityQueue<Spectator> spectatorQueue;     // For priority-based seating
    CircularQueue<StreamingSlot> streamingSlots; // For rotating streaming slots
    Queue<Spectator> overflowQueue;              // For audience overflow
    vector<string> availableSeats;
    int totalCapacity;
    int currentOccupancy;

public:
    SpectatorManager(int capacity = 500)
      : streamingSlots(10), totalCapacity(capacity), currentOccupancy(0)
    {
        initializeSeats();
        initializeStreamingSlots();
    }

    // Initialize available seats
    void initializeSeats() {
        availableSeats.clear();
        for (int i = 1; i <= totalCapacity; i++) {
            availableSeats.push_back("S" + to_string(i));
        }
    }

    // Initialize streaming slots
    void initializeStreamingSlots() {
        // Note: constructor expects string IDs, not int
        StreamingSlot slot1("1", "APU_Official", "YouTube", 5000);
        StreamingSlot slot2("2", "EsportsHighlights", "Twitch", 3000);
        StreamingSlot slot3("3", "GameMaster_Live", "Facebook", 2000);
        
        streamingSlots.enqueue(slot1);
        streamingSlots.enqueue(slot2);
        streamingSlots.enqueue(slot3);
    }

    // Register spectator with priority
    void registerSpectator(const Spectator& spectator) {
        spectatorQueue.enqueue(spectator, spectator.getPriority());
        cout << "Spectator " << spectator.getName() << " registered with priority " 
             << spectator.getPriority() << endl;
    }

    // Process spectator check-in with seating
    bool checkInSpectator() {
        if (spectatorQueue.isEmpty()) {
            cout << "No spectators in queue" << endl;
            return false;
        }

        if (currentOccupancy >= totalCapacity) {
            Spectator spectator = spectatorQueue.dequeue();
            overflowQueue.enqueue(spectator);
            cout << "Venue full! " << spectator.getName() 
                 << " moved to overflow queue" << endl;
            return false;
        }

        Spectator spectator = spectatorQueue.dequeue();
        string assignedSeat = availableSeats.back();
        availableSeats.pop_back();
        
        spectator.setSeatNumber(assignedSeat);
        spectator.setCheckedIn(true);
        currentOccupancy++;

        cout << "Checked in: " << spectator.getName() 
             << " | Seat: " << assignedSeat 
             << " | Type: " << spectator.getSpectatorType() << endl;
        return true;
    }

    // Manage streaming slot rotation
    void rotateStreamingSlot() {
        if (!streamingSlots.isEmpty()) {
            StreamingSlot slot = streamingSlots.dequeue();
            slot.setActive(true);
            cout << "Activated streaming slot: " << slot.getStreamerName() 
                 << " on " << slot.getPlatform() << endl;
            streamingSlots.enqueue(slot);
        }
    }

    // Handle overflow queue
    void processOverflow() {
        cout << "\n=== Processing Overflow Queue ===" << endl;
        while (!overflowQueue.isEmpty() && currentOccupancy < totalCapacity) {
            Spectator spectator = overflowQueue.dequeue();
            string assignedSeat = availableSeats.back();
            availableSeats.pop_back();
            
            spectator.setSeatNumber(assignedSeat);
            spectator.setCheckedIn(true);
            currentOccupancy++;

            cout << "Overflow processed: " << spectator.getName() 
                 << " | Seat: " << assignedSeat << endl;
        }
    }

    // Display system status
    void displayStatus() {
        cout << "\n=== SPECTATOR MANAGEMENT STATUS ===" << endl;
        cout << "Venue Capacity: " << currentOccupancy << "/" << totalCapacity << endl;
        cout << "Available Seats: " << availableSeats.size() << endl;
        cout << "Spectators in Priority Queue: " << spectatorQueue.size() << endl;
        cout << "Spectators in Overflow: " << overflowQueue.size() << endl;
        
        cout << "\n--- Streaming Slots ---" << endl;
        streamingSlots.display();
    }

    // Getters for integration
    int getCurrentOccupancy() const { return currentOccupancy; }
    int getTotalCapacity()   const { return totalCapacity;   }
    bool hasOverflow()       const { return !overflowQueue.isEmpty(); }
};

#endif // SPECTATOR_MANAGER_H