// ===== MATCH SCHEDULER HEADER FILE =====
// Student Implementation for TASK 1
// Match Scheduling & Player Progression

#ifndef MATCH_SCHEDULER_H
#define MATCH_SCHEDULER_H

#include "DataStructs.h"
#include "Entities.h"
#include "TournamentGraph.h"
#include "CSVHandler.h"
#include <iostream>
#include <algorithm>

using namespace std;

// ===============================
// TASK 1: MATCH SCHEDULING & PLAYER PROGRESSION
// ===============================

class MatchScheduler {
private:
    Queue<Match>* pendingMatches;        // Queue pending matches
    Stack<Match>* knockoutBracket;       // Stack fknockout matches
    TournamentGraph* bracketGraph;       // Graph tournament structure
    TournamentTraversal* traversal;      // For DFS and BFS traversal
    
    // Counter variables
    int matchCounter;                    //gen unique match ID
    int graphIndex;                      //graph indexing
    bool csvInitialized;                 // CSV status
    
    // Helper variables
    bool isSystemReady;
    int totalMatchesGenerated;
    int simulationCounter;              

public:
    MatchScheduler() {
        matchCounter = 1;                //match numbering from 1
        graphIndex = 0;                  //graph index from 0
        csvInitialized = false;          
        isSystemReady = true;            
        totalMatchesGenerated = 0;       
        simulationCounter = 0;           
        
        // data structures
        pendingMatches = new Queue<Match>();
        knockoutBracket = new Stack<Match>();
        bracketGraph = new TournamentGraph();
        traversal = new TournamentTraversal(bracketGraph);
        
        cout << "MatchScheduler initialized successfully.\n";
    }
    
    // Destructor - clean up memory
    ~MatchScheduler() {
        delete pendingMatches;
        delete knockoutBracket;
        delete bracketGraph;
        delete traversal;
        
        // Set pointers to null for safety
        pendingMatches = nullptr;
        knockoutBracket = nullptr;
        bracketGraph = nullptr;
        traversal = nullptr;
    }
    
    //initialize CSV file
    void initializeMatchCSV() {
        if (csvInitialized == false) {
            CSVHandler::initializeMatchesCSV();
            csvInitialized = true;
            cout << "CSV file initialized for match results.\n";
        } else {
            cout << "CSV file already initialized.\n";
        }
    }
    
    void generateQualifierMatches(Queue<Team>* teams) {
        cout << "\n=== GENERATING TEAM QUALIFIER MATCHES ===\n";
        
        if (teams->isEmpty() == true) {
            cout << "ERROR: No teams available for match generation!\n";
            return;
        }
        
        // Check min teams
        if (teams->size() < 2) {
            cout << "ERROR: Need at least 2 teams for matches!\n";
            cout << "Current team count: " << teams->size() << "\n";
            return;
        }
        
        initializeMatchCSV();
        
        //tempo queue 
        Queue<Team> tempTeams;
        int originalTeamCount = teams->size();
        
        cout << "Processing " << originalTeamCount << " teams for qualifier matches...\n";
        
        // Move teams to temp queue
        while (teams->isEmpty() == false) {
            Team team = teams->dequeue();
            tempTeams.enqueue(team);
        }
        
        // Generate matches
        int matchesCreated = 0;
        while (tempTeams.size() >= 2) {
            Team team1 = tempTeams.dequeue();
            Team team2 = tempTeams.dequeue();
            
            // Restore teams to original queue
            teams->enqueue(team1);
            teams->enqueue(team2);

            //unique match ID
            string matchID = "Q" + to_string(matchCounter);
            matchCounter = matchCounter + 1; 
            
            Match newMatch(matchID, team1.teamID, team2.teamID, "qualifier", "round-1");
            newMatch.graphIndex = graphIndex;

            // Add to graph
            bracketGraph->setMatchName(graphIndex, matchID);
            if (bracketGraph->numMatches < (graphIndex + 1)) {
                bracketGraph->numMatches = graphIndex + 1;
            }

            // Add match
            pendingMatches->enqueue(newMatch);

            cout << "✓ Created match " << matchID << " (Graph Index: " << graphIndex << "):\n";
            cout << "  Team 1: " << team1.teamName << " (Avg Rank: " << team1.getAverageRanking() << ")\n";
            cout << "  Team 2: " << team2.teamName << " (Avg Rank: " << team2.getAverageRanking() << ")\n";
            cout << "\n";

            // Inc counter
            graphIndex++;
            matchesCreated++;
            totalMatchesGenerated++;
        }
        
        // Handle odd team
        if (tempTeams.isEmpty() == false) {
            Team byeTeam = tempTeams.dequeue();
            teams->enqueue(byeTeam);
            cout << "🎯 Team " << byeTeam.teamName << " receives a bye to the next round.\n";
        }
        
        cout << "\nQualifier match generation completed!\n";
        cout << "Total matches created: " << matchesCreated << "\n";
        cout << "Matches in pending queue: " << pendingMatches->size() << "\n";
    }
    
    
    void generateMatchScores(Match& match, int& score1, int& score2) {
        simulationCounter++;
        
        int baseRandom = simulationCounter * 7919; // Large prime number
        baseRandom += matchCounter * 4567;         // Another prime
        baseRandom += graphIndex * 2347;           // Another prime
        
        // Add team ID variations
        for (int i = 0; i < match.team1ID.length(); i++) {
            baseRandom += match.team1ID[i] * (i + 17);
        }
        for (int i = 0; i < match.team2ID.length(); i++) {
            baseRandom += match.team2ID[i] * (i + 29);
        }
        
        // Create final random value
        int randomValue = baseRandom % 10000;
        
        //winner determ
        if (randomValue % 2 == 0) {
            // Team 1 wins with 13
            score1 = 13;
            score2 = (randomValue / 13) % 13; 
        } else {
            // Team 2 wins with 13  
            score1 = (randomValue / 17) % 13;
            score2 = 13;
        }
        
        if (score1 == 13 && score2 == 13) {
            score2 = 12;
        }
        
        // one winner with 13
        if (score1 != 13 && score2 != 13) {
            if (simulationCounter % 2 == 0) {
                score1 = 13;
            } else {
                score2 = 13;
            }
        }
        
        
        // cout << "  DEBUG Counter: " << simulationCounter << ", Random: " << randomValue 
        //      << ", Scores: " << score1 << "-" << score2 << "\n";
    }
    
    // find team by ID in queue
    bool findTeam(Queue<Team>* allTeams, const string& teamID, Team& foundTeam) {
        // head 
        DoublyNode<Team>* currentNode = allTeams->getHead();
        
        // Search
        while (currentNode != nullptr) {
            Team currentTeam = currentNode->data;
            
            // Check if match
            if (currentTeam.teamID == teamID) {
                foundTeam = currentTeam; // Copy team data
                return true; // Team found
            }
            
            currentNode = currentNode->next; // next node
        }
        
        return false;
    }
    
    bool simulateMatch(Match& match, Queue<Team>* allTeams, Team& winner, Team& loser) {
        Team team1, team2;
        
        simulationCounter++;
        
        // Find both teams
        bool team1Found = findTeam(allTeams, match.team1ID, team1);
        bool team2Found = findTeam(allTeams, match.team2ID, team2);
        
        // Check both teams found
        if (team1Found == false || team2Found == false) {
            cout << "WARNING: Could not find teams for match " << match.matchID << "\n";
            if (team1Found == false) {
                cout << "  Missing team: " << match.team1ID << "\n";
            }
            if (team2Found == false) {
                cout << "  Missing team: " << match.team2ID << "\n";
            }
            return false;
        }
        
        // Gen scores
        int team1Score, team2Score;
        generateMatchScores(match, team1Score, team2Score);
        
        // Store scores
        match.setScores(team1Score, team2Score);
        
       //det winner
        if (team1Score > team2Score) {
            winner = team1;
            loser = team2;
        } else if (team2Score > team1Score) {
            winner = team2;
            loser = team1;
        } else {
            cout << "WARNING: Tie game! Using team ranking as tiebreaker.\n";
            if (team1.getAverageRanking() < team2.getAverageRanking()) {
                winner = team1;
                loser = team2;
            } else {
                winner = team2;
                loser = team1;
            }
        }
        
        // Update match  winner information
        match.winnerID = winner.teamID;
        match.status = "completed";
        
        // Update team statuses
        winner.status = "qualified";
        loser.status = "eliminated";
        
        cout << " Match " << match.matchID << " simulation completed:\n";
        cout << "  " << team1.teamName << " " << team1Score 
             << " - " << team2Score << " " << team2.teamName << "\n";
        cout << "   Winner: " << winner.teamName 
             << " (Score: " << (winner.teamID == team1.teamID ? team1Score : team2Score) << ")\n";
        cout << "   Eliminated: " << loser.teamName 
             << " (Score: " << (loser.teamID == team1.teamID ? team1Score : team2Score) << ")\n";
        
        // Save match result to CSV file immediately
        CSVHandler::saveMatchResult(match);
        
        return true; 
    }
    
    // Main func to sim matches
    Queue<Team>* simulateMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING TEAM MATCHES WITH SCORE SYSTEM ===\n";
        
        //queue to store winners
        Queue<Team>* winners = new Queue<Team>();

        // Check matches to simulate
        if (pendingMatches->isEmpty() == true) {
            cout << "No matches available to simulate!\n";
            return winners;
        }

        // CSV initialized for  results
        initializeMatchCSV();

        Queue<Match> matchesToProcess;
        int matchesSimulated = 0;
        int totalMatches = pendingMatches->size();
        
        cout << "Starting simulation of " << totalMatches << " matches...\n\n";
        
        // Process  match
        while (pendingMatches->isEmpty() == false) {
            Match currentMatch = pendingMatches->dequeue();
            currentMatch.status = "in-progress"; 

            Team matchWinner, matchLoser;
            
            // Sim
            bool simulationSuccess = simulateMatch(currentMatch, allTeams, matchWinner, matchLoser);
            
            if (simulationSuccess == true) {
                // Add winner 
                winners->enqueue(matchWinner);
                matchesSimulated++;
                cout << "\n"; 
            } else {
                cout << " Failed to simulate match " << currentMatch.matchID << "\n";
            }

            // Add match back to temp queue
            matchesToProcess.enqueue(currentMatch);
        }

        while (matchesToProcess.isEmpty() == false) {
            Match match = matchesToProcess.dequeue();
            pendingMatches->enqueue(match);
        }
        
        cout << "=== SIMULATION SUMMARY ===\n";
        cout << "Total matches simulated: " << matchesSimulated << " out of " << totalMatches << "\n";
        cout << "Teams advancing to next round: " << winners->size() << "\n";
        cout << "Simulation process completed!\n";

        return winners;
    }
    
    void generateKnockoutMatches(Queue<Team>* qualifiedTeams, const string& stage = "knockout") {
        cout << "\n=== GENERATING " << stage << " BRACKET MATCHES ===\n";

        // Validate input
        if (qualifiedTeams->isEmpty() == true) {
            cout << "ERROR: No qualified teams for " << stage << " bracket!\n";
            return;
        }
        
        if (qualifiedTeams->size() < 2) {
            cout << "ERROR: Not enough teams for " << stage << " matches.\n";
            cout << "Need at least 2 teams, but have " << qualifiedTeams->size() << "\n";
            return;
        }

        // Track prev match for graph
        int previousMatchIndex = -1;
        if (graphIndex > 0) {
            previousMatchIndex = graphIndex - 1;
        }
        
        int matchesGenerated = 0;
        cout << "Creating " << stage << " matches from " << qualifiedTeams->size() << " teams...\n\n";


        while (qualifiedTeams->size() >= 2) {
            Team team1 = qualifiedTeams->dequeue();
            Team team2 = qualifiedTeams->dequeue();

            // match ID
            string matchID = "K" + to_string(matchCounter);
            matchCounter++;
            
            // match object
            Match knockoutMatch(matchID, team1.teamID, team2.teamID, stage, stage + "-round");
            knockoutMatch.graphIndex = graphIndex;

            // Add to graph
            bracketGraph->setMatchName(graphIndex, matchID);

            // prev matches to graph
            if (previousMatchIndex != -1) {
                bracketGraph->addMatchConnection(previousMatchIndex, graphIndex, "leads-to");
            }

            // Update grap
            if (bracketGraph->numMatches <= graphIndex) {
                bracketGraph->numMatches = graphIndex + 1;
            }
            
            // Add to knockout bracket stack
            knockoutBracket->push(knockoutMatch);

            cout << " Knockout match " << matchID << " (Graph Index: " << graphIndex << "):\n";
            cout << "  " << team1.teamName << " (Avg Rank: " << team1.getAverageRanking() << ")\n";
            cout << "  vs\n";
            cout << "  " << team2.teamName << " (Avg Rank: " << team2.getAverageRanking() << ")\n\n";


            previousMatchIndex = graphIndex;
            graphIndex++;
            matchesGenerated++;
            totalMatchesGenerated++;
        }
        
        // Handle remaining
        if (qualifiedTeams->isEmpty() == false) {
            Team byeTeam = qualifiedTeams->dequeue();
            cout << "🎯 Team " << byeTeam.teamName << " gets a bye to the next round.\n";
            // team next round
            qualifiedTeams->enqueue(byeTeam);
        }
        
        cout << "\nKnockout match generation completed!\n";
        cout << "Matches generated for " << stage << " stage: " << matchesGenerated << "\n";
        cout << "Knockout bracket size: " << knockoutBracket->size() << "\n";
    }
    
    Queue<Team>* simulateKnockoutMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING KNOCKOUT MATCHES ===\n";
        
        Queue<Team>* knockoutWinners = new Queue<Team>();
        
        // Check knockout matches to sim
        if (knockoutBracket->isEmpty() == true) {
            cout << "No knockout matches available to simulate!\n";
            return knockoutWinners;
        }
        
        //temp stack
        Stack<Match> tempStack;
        int matchesSimulated = 0;
        int totalKnockoutMatches = knockoutBracket->size();
        
        cout << "Processing " << totalKnockoutMatches << " knockout matches...\n\n";
        
        // knockout matches
        while (knockoutBracket->isEmpty() == false) {
            Match knockoutMatch = knockoutBracket->pop();
            knockoutMatch.status = "in-progress"; // Update match status
            
            Team matchWinner, matchLoser;
            
            bool simulationResult = simulateMatch(knockoutMatch, allTeams, matchWinner, matchLoser);
            
            if (simulationResult == true) {
                // Adv winne
                knockoutWinners->enqueue(matchWinner);
                matchesSimulated++;
                cout << "\n"; 
            } else {
                cout << " Failed to simulate knockout match " << knockoutMatch.matchID << "\n";
            }
            
            // temporary stack
            tempStack.push(knockoutMatch);
        }
        
        // Restore knockout bracket
        while (tempStack.isEmpty() == false) {
            Match match = tempStack.pop();
            knockoutBracket->push(match);
        }
        
        cout << "=== KNOCKOUT SIMULATION SUMMARY ===\n";
        cout << "Knockout matches simulated: " << matchesSimulated << " out of " << totalKnockoutMatches << "\n";
        cout << "Teams advancing from knockout stage: " << knockoutWinners->size() << "\n";
        cout << "Knockout simulation completed!\n";
        
        return knockoutWinners;
    }
    
    // Main wrapper function to generate knockout bracket
    void generateKnockoutBracket(Queue<Team>* qualifiedTeams) {
        // Call the detailed knockout generation function
        generateKnockoutMatches(qualifiedTeams, "knockout");
    }
    
    // Function to display tournament bracket structure and traversal algorithms
    void displayBracketTraversal() {
        cout << "\n=== TOURNAMENT BRACKET TRAVERSAL ALGORITHMS ===\n";
        
        // Check if there are matches in the bracket
        if (bracketGraph->numMatches > 0) {
            cout << "Tournament bracket contains " << bracketGraph->numMatches << " matches.\n";
            
            //bracket structure
            bracketGraph->printTournamentBracket();
            
            //DFS 
            cout << "\n--- GRAPH TRAVERSAL ALGORITHMS ---\n";
            traversal->DFS(0);
            
            //BFS
            traversal->BFS(0);
            
            cout << "\nBracket traversal completed.\n";
        } else {
            cout << "=== NO TOURNAMENT BRACKET TO TRAVERSE ===\n";
            cout << "No matches have been generated in the bracket yet.\n";
            cout << "Please generate matches first using the match scheduling functions.\n";
        }
    }
    
    // retrieve matches
    Queue<Match>* getAllMatches() {
        return pendingMatches;
    }
    
    //retrieve knockout bracket
    Stack<Match>* getKnockoutBracket() {
        return knockoutBracket;
    }
    

    void displayBracketStatus() {
        cout << "\n=== COMPREHENSIVE TOURNAMENT BRACKET STATUS ===\n";
        
        cout << "MATCH STATISTICS:\n";
        cout << "  Pending matches in queue: " << pendingMatches->size() << "\n";
        cout << "  Knockout matches in stack: " << knockoutBracket->size() << "\n";
        cout << "  Total matches in graph: " << bracketGraph->numMatches << "\n";
        cout << "  Next match ID number: " << matchCounter << "\n";
        cout << "  Current graph index: " << graphIndex << "\n";
        cout << "  Total matches generated: " << totalMatchesGenerated << "\n";
        
        cout << "\nSYSTEM STATUS:\n";
        if (csvInitialized == true) {
            cout << "  CSV file status: Initialized\n";
        } else {
            cout << "  CSV file status: Not initialized\n";
        }
        
        if (isSystemReady == true) {
            cout << "  Match scheduler status: Ready\n";
        } else {
            cout << "  Match scheduler status: Not ready\n";
        }
        
        int completedMatches = 0;
        int pendingMatchCount = 0;
        DoublyNode<Match>* currentMatch = pendingMatches->getHead();
        
        while (currentMatch != nullptr) {
            if (currentMatch->data.status == "completed") {
                completedMatches++;
            } else if (currentMatch->data.status == "pending") {
                pendingMatchCount++;
            }
            currentMatch = currentMatch->next;
        }
        
        cout << "\nMATCH STATUS BREAKDOWN:\n";
        cout << "  Completed matches: " << completedMatches << "\n";
        cout << "  Pending matches: " << pendingMatchCount << "\n";
        
        // Calcu progress percentage
        int totalMatches = completedMatches + pendingMatchCount;
        if (totalMatches > 0) {
            double progressPercentage = (double)completedMatches / totalMatches * 100.0;
            cout << "  Tournament progress: " << progressPercentage << "%\n";
        } else {
            cout << "  Tournament progress: 0% (no matches generated)\n";
        }
        
        cout << "=======================================\n";
    }
    
    //reset 
    void resetTournament() {
        cout << "\n=== RESETTING TOURNAMENT SCHEDULER ===\n";
        
        // Clear pending 
        int pendingCount = pendingMatches->size();
        while (pendingMatches->isEmpty() == false) {
            Match match = pendingMatches->dequeue();
        }
        cout << "Cleared " << pendingCount << " pending matches.\n";
        
        // Clear knockout
        int knockoutCount = knockoutBracket->size();
        while (knockoutBracket->isEmpty() == false) {
            Match match = knockoutBracket->pop();
        }
        cout << "Cleared " << knockoutCount << " knockout matches.\n";
        
        matchCounter = 1;
        graphIndex = 0;
        csvInitialized = false;
        totalMatchesGenerated = 0;
        
        delete bracketGraph;
        delete traversal;
        bracketGraph = new TournamentGraph();
        traversal = new TournamentTraversal(bracketGraph);
        
        cout << "Reset match counter to: " << matchCounter << "\n";
        cout << "Reset graph index to: " << graphIndex << "\n";
        cout << "Reset CSV initialization flag.\n";
        cout << "Created new tournament graph structure.\n";
        cout << "Tournament scheduler has been reset successfully!\n";
    }
    

    void displayTournamentStats() {
        cout << "\n=== DETAILED TOURNAMENT STATISTICS ===\n";
        
        // counter
        int qualifierMatches = 0;
        int knockoutMatches = 0;
        int completedMatches = 0;
        int pendingMatches = 0;
        int inProgressMatches = 0;
        
        //pending matches
        DoublyNode<Match>* currentMatch = this->pendingMatches->getHead();
        
        cout << "Analyzing match data...\n";
        
        while (currentMatch != nullptr) {
            Match match = currentMatch->data;
            
            // Count by stage
            if (match.stage == "qualifier") {
                qualifierMatches++;
            } else if (match.stage == "knockout") {
                knockoutMatches++;
            }
            
            // Count by status
            if (match.status == "completed") {
                completedMatches++;
            } else if (match.status == "pending") {
                pendingMatches++;
            } else if (match.status == "in-progress") {
                inProgressMatches++;
            }
            
            currentMatch = currentMatch->next;
        }
        
        //  match type stats
        cout << "\nMATCH TYPE BREAKDOWN:\n";
        cout << "  Qualifier matches: " << qualifierMatches << "\n";
        cout << "  Knockout matches: " << knockoutMatches << "\n";
        cout << "  Other matches: " << (totalMatchesGenerated - qualifierMatches - knockoutMatches) << "\n";
        
        //status stats
        cout << "\nMATCH STATUS BREAKDOWN:\n";
        cout << "  Completed matches: " << completedMatches << "\n";
        cout << "  Pending matches: " << pendingMatches << "\n";
        cout << "  In-progress matches: " << inProgressMatches << "\n";
        

        cout << "\nGENERATION STATISTICS:\n";
        cout << "  Total matches generated: " << totalMatchesGenerated << "\n";
        cout << "  Next match ID will be: " << matchCounter << "\n";
        cout << "  Current graph index: " << graphIndex << "\n";
        
        // display progress
        int totalKnownMatches = qualifierMatches + knockoutMatches;
        if (totalKnownMatches > 0) {
            double completionRate = (double)completedMatches / totalKnownMatches * 100.0;
            cout << "\nTOURNAMENT PROGRESS:\n";
            cout << "  Overall completion: " << completionRate << "%\n";
            
            if (completionRate == 100.0) {
                cout << "  Status: Tournament completed!\n";
            } else if (completionRate >= 50.0) {
                cout << "  Status: Tournament in progress (over halfway)\n";
            } else if (completionRate > 0.0) {
                cout << "  Status: Tournament just started\n";
            } else {
                cout << "  Status: Tournament ready to begin\n";
            }
        } else {
            cout << "\nTOURNAMENT PROGRESS:\n";
            cout << "  Overall completion: 0% (no matches generated yet)\n";
            cout << "  Status: 📋 Waiting for match generation\n";
        }
        
        cout << "===============================\n";
    }
};

#endif 