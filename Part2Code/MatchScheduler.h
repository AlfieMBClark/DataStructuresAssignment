#ifndef MATCH_SCHEDULER_H
#define MATCH_SCHEDULER_H


#include "DataStructs.h"
#include "Entities.h"
#include "TournamentGraph.h"
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

public:
    MatchScheduler() : matchCounter(1), graphIndex(0) {
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
    
    // Generate qualifier matches for teams
    void generateQualifierMatches(Queue<Team>* teams) {
        cout << "\n=== GENERATING TEAM QUALIFIER MATCHES ===\n";
        
        if (teams->isEmpty()) {
            cout << "No teams available for matches!\n";
            return;
        }
        
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
    }
    
    // Simulate matches with proper team lookup
    Queue<Team>* simulateMatches(Queue<Team>* allTeams) {
        cout << "\n=== SIMULATING TEAM MATCHES ===\n";
        Queue<Team>* winners = new Queue<Team>();

        if (pendingMatches->isEmpty()) {
            cout << "No matches to simulate!\n";
            return winners;
        }

        // Create lookup map for teams
        Queue<Team> lookup;
        DoublyNode<Team>* current = allTeams->getHead();
        while (current != nullptr) {
            lookup.enqueue(current->data);
            current = current->next;
        }

        Queue<Match> matchesToProcess;
        while (!pendingMatches->isEmpty()) {
            Match match = pendingMatches->dequeue();
            match.status = "in-progress";

            Team team1, team2;
            bool found1 = false, found2 = false;

            // Find teams in lookup
            DoublyNode<Team>* lookupCurrent = lookup.getHead();
            while (lookupCurrent != nullptr) {
                if (lookupCurrent->data.teamID == match.team1ID) {
                    team1 = lookupCurrent->data;
                    found1 = true;
                }
                if (lookupCurrent->data.teamID == match.team2ID) {
                    team2 = lookupCurrent->data;
                    found2 = true;
                }
                lookupCurrent = lookupCurrent->next;
            }

            if (found1 && found2) {
                double avg1 = team1.getAverageRanking();
                double avg2 = team2.getAverageRanking();

                Team winner = (avg1 < avg2) ? team1 : team2;
                Team loser  = (avg1 < avg2) ? team2 : team1;

                match.winnerID = winner.teamID;
                match.status = "completed";

                winner.status = "qualified";
                loser.status = "eliminated";

                winners->enqueue(winner);

                cout << "Match " << match.matchID << " completed:\n";
                cout << "  Winner: " << winner.teamName << " (Avg Rank: " << avg1 << ")\n";
                cout << "  Eliminated: " << loser.teamName << " (Avg Rank: " << avg2 << ")\n";
            } else {
                cout << "Warning: Could not find teams for match " << match.matchID << "\n";
            }

            matchesToProcess.enqueue(match);
        }

        // Restore matches queue
        while (!matchesToProcess.isEmpty()) {
            pendingMatches->enqueue(matchesToProcess.dequeue());
        }

        return winners;
    }
    
    // Generate knockout bracket
    void generateKnockoutBracket(Queue<Team>* qualifiedTeams) {
        cout << "\n=== GENERATING TEAM KNOCKOUT BRACKET ===\n";

        if (qualifiedTeams->isEmpty()) {
            cout << "No qualified teams for knockout bracket!\n";
            return;
        }

        int prevMatchIndex = -1;

        while (qualifiedTeams->size() >= 2) {
            Team t1 = qualifiedTeams->dequeue();
            Team t2 = qualifiedTeams->dequeue();

            string matchID = "K" + to_string(matchCounter++);
            Match match(matchID, t1.teamID, t2.teamID, "knockout", "knockout-round");
            match.graphIndex = graphIndex;

            bracketGraph->setMatchName(graphIndex, matchID);

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
        }
        
        cout << "Knockout bracket generated with " << knockoutBracket->size() << " matches.\n";
    }
    
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
    
    Queue<Match>* getAllMatches() {
        return pendingMatches;
    }
    
    void displayBracketStatus() {
        cout << "\n=== BRACKET STATUS ===\n";
        cout << "Pending matches: " << pendingMatches->size() << "\n";
        cout << "Knockout bracket size: " << knockoutBracket->size() << "\n";
        cout << "Graph matches: " << bracketGraph->numMatches << "\n";
    }
};

#endif // MATCH_SCHEDULER_H