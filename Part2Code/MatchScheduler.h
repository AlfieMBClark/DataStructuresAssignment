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
    Queue<Match>* pendingMatches;
    Stack<Match>* knockoutBracket;
    TournamentGraph* bracketGraph;
    TournamentTraversal* traversal;
    int matchCounter;
    int graphIndex;
    bool csvInitialized;

public:
    MatchScheduler() : matchCounter(1), graphIndex(0), csvInitialized(false) {
        pendingMatches = new Queue<Match>();
        knockoutBracket = new Stack<Match>();
        bracketGraph = new TournamentGraph();
        traversal = new TournamentTraversal(bracketGraph);
    }
    
    ~MatchScheduler() {
        delete pendingMatches;
        delete knockoutBracket;
        delete bracketGraph;
        delete traversal;
    }
    
    // Initialize CSV file for match results
    void initializeMatchCSV() {
        if (!csvInitialized) {
            CSVHandler::initializeMatchesCSV();
            csvInitialized = true;
        }
    }
    
    // Generate qualifier matches for teams
    void generateQualifierMatches(Queue<Team>* teams) {
        cout << "\n=== GENERATING TEAM QUALIFIER MATCHES ===\n";
        
        if (teams->isEmpty()) {
            cout << "No teams available for matches!\n";
            return;
        }
        
        // Initialize CSV file for match saving
        initializeMatchCSV();
        
        // Create a temporary copy to preserve original queue
        Queue<Team> tempTeams;
        while (!teams->isEmpty()) {
            tempTeams.enqueue(teams->dequeue());
        }
        
        // Restore original queue and create matches
        while (tempTeams.size() >= 2) {
            Team t1 = tempTeams.dequeue();
            Team t2 = tempTeams.dequeue();
            
            // Restore teams to original queue
            teams->enqueue(t1);
            teams->enqueue(t2);

            string matchID = "Q" + to_string(matchCounter++);
            Match match(matchID, t1.teamID, t2.teamID, "qualifier", "round-1");
            match.graphIndex = graphIndex;

            bracketGraph->setMatchName(graphIndex, matchID);
            bracketGraph->numMatches = max(bracketGraph->numMatches, graphIndex + 1);

            pendingMatches->enqueue(match);

            cout << "Created match " << matchID << " (Graph Index: " << graphIndex << "): "
                 << t1.teamName << " (Avg: " << t1.getAverageRanking() << ") vs " 
                 << t2.teamName << " (Avg: " << t2.getAverageRanking() << ")\n";

            graphIndex++;
        }
        
        // Handle odd team
        if (!tempTeams.isEmpty()) {
            Team byeTeam = tempTeams.dequeue();
            teams->enqueue(byeTeam);
            cout << "Team " << byeTeam.teamName << " receives a bye to next round.\n";
        }
        
        cout << "Generated " << (matchCounter - 1) << " qualifier matches.\n";
    }
    
    // Generate random scores using 13 vs 0-12 system
    void generateMatchScores(Match& match, int& score1, int& score2) {
        // Pseudo-random generation using match properties
        int randomSeed = (matchCounter * 7 + graphIndex * 3 + match.matchID.length() * 11) % 100;
        
        if (randomSeed % 2 == 0) {
            // Team 1 wins with 13
            score1 = 13;
            score2 = randomSeed % 13; // 0-12
        } else {
            // Team 2 wins with 13
            score1 = randomSeed % 13; // 0-12
            score2 = 13;
        }
    }
    
    // Find team by ID in team queue
    bool findTeam(Queue<Team>* allTeams, const string& teamID, Team& foundTeam) {
        DoublyNode<Team>* current = allTeams->getHead();
        while (current != nullptr) {
            if (current->data.teamID == teamID) {
                foundTeam = current->data;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Simulate individual match with score-based system
    bool simulateMatch(Match& match, Queue<Team>* allTeams, Team& winner, Team& loser) {
        Team team1, team2;
        
        // Find both teams
        if (!findTeam(allTeams, match.team1ID, team1) || 
            !findTeam(allTeams, match.team2ID, team2)) {
            cout << "Warning: Could not find teams for match " << match.matchID << "\n";
            return false;
        }
        
        // Generate scores using 13 vs 0-12 system
        int score1, score2;
        generateMatchScores(match, score1, score2);
        
        // Store scores in match
        match.setScores(score1, score2);
        
        // Determine winner and loser
        if (score1 > score2) {
            winner = team1;
            loser = team2;
        } else {
            winner = team2;
            loser = team1;
        }
        
        // Update match status
        match.winnerID = winner.teamID;
        match.status = "completed";
        
        // Update team statuses
        winner.status = "qualified";
        loser.status = "eliminated";
        
        // Display match result
        cout << "Match " << match.matchID << " completed:\n";
        cout << "  " << team1.teamName << " " << score1 << " - " << score2 << " " << team2.teamName << "\n";
        cout << "  Winner: " << winner.teamName << " (Score: " << (score1 > score2 ? score1 : score2) << ")\n";
        cout << "  Eliminated: " << loser.teamName << " (Score: " << (score1 > score2 ? score2 : score1) << ")\n";
        
        // Save match result immediately to CSV
        CSVHandler::saveMatchResult(match);
        
        return true;
    }
    
    // Simulate all pending matches with score-based system and immediate CSV saving
    Queue<Team>* simulateMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING TEAM MATCHES WITH SCORE SYSTEM & IMMEDIATE SAVING ===\n";
        Queue<Team>* winners = new Queue<Team>();

        if (pendingMatches->isEmpty()) {
            cout << "No matches to simulate!\n";
            return winners;
        }

        // Ensure CSV is initialized
        initializeMatchCSV();

        Queue<Match> matchesToProcess;
        int matchesSimulated = 0;
        
        // Process all pending matches
        while (!pendingMatches->isEmpty()) {
            Match match = pendingMatches->dequeue();
            match.status = "in-progress";

            Team winner, loser;
            
            // Simulate the match
            if (simulateMatch(match, allTeams, winner, loser)) {
                winners->enqueue(winner);
                matchesSimulated++;
                cout << "\n";
            }

            matchesToProcess.enqueue(match);
        }

        // Restore matches queue
        while (!matchesToProcess.isEmpty()) {
            pendingMatches->enqueue(matchesToProcess.dequeue());
        }
        
        cout << "Simulation complete: " << matchesSimulated << " matches played, " 
             << winners->size() << " teams advanced.\n";

        return winners;
    }
    
    // Generate knockout bracket matches
    void generateKnockoutMatches(Queue<Team>* qualifiedTeams, const string& stage = "knockout") {
        cout << "\n=== GENERATING " << stage << " BRACKET MATCHES ===\n";

        if (qualifiedTeams->isEmpty()) {
            cout << "No qualified teams for " << stage << " bracket!\n";
            return;
        }
        
        if (qualifiedTeams->size() < 2) {
            cout << "Not enough teams for " << stage << " matches (need at least 2).\n";
            return;
        }

        int prevMatchIndex = graphIndex > 0 ? graphIndex - 1 : -1;
        int matchesGenerated = 0;

        while (qualifiedTeams->size() >= 2) {
            Team t1 = qualifiedTeams->dequeue();
            Team t2 = qualifiedTeams->dequeue();

            string matchID = "K" + to_string(matchCounter++);
            Match match(matchID, t1.teamID, t2.teamID, stage, stage + "-round");
            match.graphIndex = graphIndex;

            bracketGraph->setMatchName(graphIndex, matchID);

            // Connect to previous matches in graph
            if (prevMatchIndex != -1) {
                bracketGraph->addMatchConnection(prevMatchIndex, graphIndex, "leads-to");
            }

            bracketGraph->numMatches = max(bracketGraph->numMatches, graphIndex + 1);
            knockoutBracket->push(match);

            cout << "Knockout match " << matchID << " (Graph Index: " << graphIndex << "): "
                 << t1.teamName << " (Avg: " << t1.getAverageRanking() << ") vs " 
                 << t2.teamName << " (Avg: " << t2.getAverageRanking() << ")\n";

            prevMatchIndex = graphIndex;
            graphIndex++;
            matchesGenerated++;
        }
        
        // Handle remaining team (gets bye to next round)
        if (!qualifiedTeams->isEmpty()) {
            Team byeTeam = qualifiedTeams->dequeue();
            cout << "Team " << byeTeam.teamName << " receives a bye to the next round.\n";
            qualifiedTeams->enqueue(byeTeam); // Put back for next round
        }
        
        cout << "Generated " << matchesGenerated << " " << stage << " matches.\n";
    }
    
    // Simulate knockout matches specifically
    Queue<Team>* simulateKnockoutMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING KNOCKOUT MATCHES ===\n";
        Queue<Team>* winners = new Queue<Team>();
        
        if (knockoutBracket->isEmpty()) {
            cout << "No knockout matches to simulate!\n";
            return winners;
        }
        
        Stack<Match> tempStack;
        int matchesSimulated = 0;
        
        // Process knockout matches (LIFO - most recent first)
        while (!knockoutBracket->isEmpty()) {
            Match match = knockoutBracket->pop();
            match.status = "in-progress";
            
            Team winner, loser;
            
            // Simulate the knockout match
            if (simulateMatch(match, allTeams, winner, loser)) {
                winners->enqueue(winner);
                matchesSimulated++;
                cout << "\n";
            }
            
            tempStack.push(match);
        }
        
        // Restore knockout bracket
        while (!tempStack.isEmpty()) {
            knockoutBracket->push(tempStack.pop());
        }
        
        cout << "Knockout simulation complete: " << matchesSimulated << " matches played, " 
             << winners->size() << " teams advanced.\n";
        
        return winners;
    }
    
    // Main method to generate knockout bracket
    void generateKnockoutBracket(Queue<Team>* qualifiedTeams) {
        generateKnockoutMatches(qualifiedTeams, "knockout");
    }
    
    // Display tournament bracket structure and traversal
    void displayBracketTraversal() {
        if (bracketGraph->numMatches > 0) {
            cout << "\n=== TOURNAMENT BRACKET TRAVERSAL ===\n";
            bracketGraph->printTournamentBracket();
            
            traversal->DFS(0);
            traversal->BFS(0);
        } else {
            cout << "\n=== NO BRACKET TO TRAVERSE ===\n";
            cout << "No matches were generated in the bracket.\n";
        }
    }
    
    // Get all matches (pending + completed)
    Queue<Match>* getAllMatches() {
        return pendingMatches;
    }
    
    // Get knockout bracket matches
    Stack<Match>* getKnockoutBracket() {
        return knockoutBracket;
    }
    
    // Display comprehensive bracket status
    void displayBracketStatus() {
        cout << "\n=== COMPREHENSIVE BRACKET STATUS ===\n";
        cout << "Pending matches: " << pendingMatches->size() << "\n";
        cout << "Knockout bracket size: " << knockoutBracket->size() << "\n";
        cout << "Graph matches: " << bracketGraph->numMatches << "\n";
        cout << "Next match counter: " << matchCounter << "\n";
        cout << "Current graph index: " << graphIndex << "\n";
        cout << "CSV initialized: " << (csvInitialized ? "Yes" : "No") << "\n";
        
        // Count completed matches
        int completedMatches = 0;
        DoublyNode<Match>* current = pendingMatches->getHead();
        while (current != nullptr) {
            if (current->data.status == "completed") {
                completedMatches++;
            }
            current = current->next;
        }
        cout << "Completed matches: " << completedMatches << "\n";
        cout << "=======================================\n";
    }
    
    // Reset tournament for new tournament
    void resetTournament() {
        // Clear all data structures
        while (!pendingMatches->isEmpty()) {
            pendingMatches->dequeue();
        }
        while (!knockoutBracket->isEmpty()) {
            knockoutBracket->pop();
        }
        
        // Reset counters
        matchCounter = 1;
        graphIndex = 0;
        csvInitialized = false;
        
        // Reset graph
        delete bracketGraph;
        delete traversal;
        bracketGraph = new TournamentGraph();
        traversal = new TournamentTraversal(bracketGraph);
        
        cout << "Tournament scheduler reset successfully.\n";
    }
    
    // Get tournament statistics
    void displayTournamentStats() {
        cout << "\n=== TOURNAMENT STATISTICS ===\n";
        
        // Analyze pending matches
        int qualifierMatches = 0, knockoutMatches = 0, completedMatches = 0;
        DoublyNode<Match>* current = pendingMatches->getHead();
        
        while (current != nullptr) {
            if (current->data.stage == "qualifier") qualifierMatches++;
            else if (current->data.stage == "knockout") knockoutMatches++;
            
            if (current->data.status == "completed") completedMatches++;
            current = current->next;
        }
        
        cout << "Qualifier matches: " << qualifierMatches << "\n";
        cout << "Knockout matches: " << knockoutMatches << "\n";
        cout << "Total matches generated: " << (matchCounter - 1) << "\n";
        cout << "Matches completed: " << completedMatches << "\n";
        cout << "Matches pending: " << (qualifierMatches + knockoutMatches - completedMatches) << "\n";
        cout << "Tournament progression: " << 
                (completedMatches * 100.0 / (qualifierMatches + knockoutMatches)) << "%\n";
        cout << "===============================\n";
    }
};

#endif // MATCH_SCHEDULER_H