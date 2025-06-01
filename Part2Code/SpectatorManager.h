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
    // Method 1: Simulate viewer activity for demo purposes
void simulateViewerActivity() {
        cout << "\n=== Simulating Viewer Activity ===" << endl;

        // === 1) STREAMING ACTIVITY UPDATE ===
        // We'll pull every slot out of the circular queue, update its viewer count if it's active,
        // then push it back in the same order.
        vector<StreamingSlot> tempSlots;
        while (!streamingSlots.isEmpty()) {
            tempSlots.push_back(streamingSlots.dequeue());
        }

        for (auto &slot : tempSlots) {
            if (slot.getIsActive()) {
                int viewerCount = 0;
                if (slot.getPlatform() == "YouTube") {
                    viewerCount = 2000 + (rand() % 1000);  // 2000–2999 viewers
                } else if (slot.getPlatform() == "Twitch") {
                    viewerCount = 1500 + (rand() % 800);   // 1500–2299 viewers
                } else if (slot.getPlatform() == "Facebook") {
                    viewerCount =  800 + (rand() % 600);   //  800–1399 viewers
                }
                slot.setCurrentViewers(viewerCount);
                cout << "Updated " << slot.getPlatform()
                     << " stream: " << viewerCount << " viewers" << endl;
            }
            // Re‐enqueue back into the circular queue
            streamingSlots.enqueue(slot);
        }

        // === 2) SPECTATOR DEPARTURE SIMULATION ===
        cout << "\n--- Spectator Movement ---" << endl;

        // Decide how many spectators leave randomly (up to 3 each time)
        if (currentOccupancy > 0) {
            int departures = rand() % 4; 
            if (departures > currentOccupancy) {
                departures = currentOccupancy;
            }

            // Free up that many seats
            if (departures > 0) {
                currentOccupancy -= departures;
                // Return those seat labels to availableSeats
                for (int i = 0; i < departures; i++) {
                    // (In a real system, you’d track exactly which seats freed;
                    // here we’ll just push back “dummy” labels to keep size in sync.)
                    // If you want to keep track of exact seat numbers, you'd need a map
                    // from seatNumber->Spectator and pull from there. For this demo,
                    // we assume availableSeats was a stack of unused seat IDs,
                    // so “departures” new seats appear at the top.
                    availableSeats.push_back("FreedSeat"); 
                      // ← Replace "FreedSeat" logic with actual seat tracking if needed
                }

                cout << departures << " spectator(s) left the venue." << endl;
                cout << "Seats freed up: " << departures << endl;
            } else {
                cout << "No spectators left during this simulation." << endl;
            }

            // As soon as seats free up, automatically seat any waiting spectators:
            if (departures > 0 && (!spectatorQueue.isEmpty() || !overflowQueue.isEmpty())) {
                cout << "\n--- Auto‐processing waiting spectators ---" << endl;
                int processed = 0;

                // First, seat from the priority queue (VIPs, Influencers, etc.)
                while (!spectatorQueue.isEmpty() && currentOccupancy < totalCapacity) {
                    Spectator vip = spectatorQueue.dequeue();
                    // Assign a newly freed seat:
                    string assignedSeat = availableSeats.back();
                    availableSeats.pop_back();
                    vip.setSeatNumber(assignedSeat);
                    vip.setCheckedIn(true);
                    currentOccupancy++;
                    processed++;
                    cout << "VIP/Influencer " << vip.getName() 
                         << " got a seat (" << assignedSeat << ")!" << endl;
                }

                // Then, seat from the overflow queue (General spectators in FIFO order)
                while (!overflowQueue.isEmpty() && currentOccupancy < totalCapacity) {
                    Spectator s = overflowQueue.dequeue();
                    string assignedSeat = availableSeats.back();
                    availableSeats.pop_back();
                    s.setSeatNumber(assignedSeat);
                    s.setCheckedIn(true);
                    currentOccupancy++;
                    processed++;
                    cout << "Overflow spectator " << s.getName() 
                         << " got a seat (" << assignedSeat << ")!" << endl;
                }

                if (processed > 0) {
                    cout << "Total spectators seated from waiting lists: " 
                         << processed << endl;
                }
            }
        }

        cout << "\nViewer activity simulation complete!" << endl;
    }

// Method 2: Add viewers to specific platform
void addViewersToStream(const string& platform, int numViewers) {
    vector<StreamingSlot> tempSlots;
    bool found = false;
    
    // Extract all slots
    while (!streamingSlots.isEmpty()) {
        tempSlots.push_back(streamingSlots.dequeue());
    }
    
    // Find and update the specific platform
    for (auto& slot : tempSlots) {
        if (slot.getPlatform() == platform && slot.getIsActive()) {
            int newViewerCount = slot.getCurrentViewers() + numViewers;
            slot.setCurrentViewers(newViewerCount);
            cout << "Added " << numViewers << " viewers to " << platform 
                 << " (Total: " << newViewerCount << ")" << endl;
            found = true;
            break;
        }
        
        // Put slot back
        streamingSlots.enqueue(slot);
    }
    
    // Put remaining slots back
    for (auto& slot : tempSlots) {
        if (!(slot.getPlatform() == platform && found)) {
            streamingSlots.enqueue(slot);
        }
    }
    
    if (!found) {
        cout << "Platform " << platform << " not found or not active" << endl;
    }
}

// Method 3: Enhanced rotation that also updates viewers
void rotateStreamingSlotWithViewers() {
    if (!streamingSlots.isEmpty()) {
        StreamingSlot slot = streamingSlots.dequeue();
        slot.setActive(true);
        
        // Add realistic viewer count when activating
        int baseViewers = 500 + (rand() % 1500); // 500-2000 base viewers
        slot.setCurrentViewers(baseViewers);
        
        cout << "Activated streaming slot: " << slot.getStreamerName() 
             << " on " << slot.getPlatform() 
             << " with " << baseViewers << " viewers" << endl;
             
        streamingSlots.enqueue(slot);
    }
}

// Method 4: Initialize slots with some starting viewers
void initializeStreamingSlotsWithViewers() {
    // Clear existing slots first if needed
    while (!streamingSlots.isEmpty()) {
        streamingSlots.dequeue();
    }
    
    StreamingSlot slot1("1", "APU_Official", "YouTube", 5000);
    slot1.setActive(true);
    slot1.setCurrentViewers(2500); // Start with some viewers
    
    StreamingSlot slot2("2", "EsportsHighlights", "Twitch", 3000);
    slot2.setActive(true);
    slot2.setCurrentViewers(1800);
    
    StreamingSlot slot3("3", "GameMaster_Live", "Facebook", 2000);
    slot3.setActive(true);
    slot3.setCurrentViewers(950);
    
    streamingSlots.enqueue(slot1);
    streamingSlots.enqueue(slot2);
    streamingSlots.enqueue(slot3);
    
    cout << "Initialized streaming slots with viewer counts" << endl;
}

// Method 5: Enhanced display with viewer information
void displayStreamingStatus() {
    cout << "\n--- Streaming Slots Status ---" << endl;
    
    vector<StreamingSlot> tempSlots;
    
    // Extract and display all slots
    while (!streamingSlots.isEmpty()) {
        StreamingSlot slot = streamingSlots.dequeue();
        cout << "Slot ID: " << slot.getSlotID() 
             << " | Streamer: " << slot.getStreamerName() 
             << " | Platform: " << slot.getPlatform() 
             << " | Viewers: " << slot.getCurrentViewers() << "/" << slot.getViewerCapacity()
             << " | Active: " << (slot.getIsActive() ? "Yes" : "No") << endl;
        
        tempSlots.push_back(slot);
    }
    
    // Put slots back
    for (const auto& slot : tempSlots) {
        streamingSlots.enqueue(slot);
    }
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
    StreamingSlot slot1("1", "APU_Official", "YouTube", 5000);
    slot1.setActive(true);
    slot1.setCurrentViewers(2500); // ← ADD THIS LINE
    
    StreamingSlot slot2("2", "EsportsHighlights", "Twitch", 3000);
    slot2.setActive(true);
    slot2.setCurrentViewers(1800); // ← ADD THIS LINE
    
    StreamingSlot slot3("3", "GameMaster_Live", "Facebook", 2000);
    slot3.setActive(true);
    slot3.setCurrentViewers(950);  // ← ADD THIS LINE
    
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