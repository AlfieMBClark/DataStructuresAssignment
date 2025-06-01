  // (In a real system, you’d track exactly which seats freed;
                    // here we’ll just push back “dummy” labels to keep size in sync.)
                    // If you want to keep track of exact seat numbers, you'd need a map
                    // from seatNumber->Spectator and pull from there. For this demo,
                    // we assume availableSeats was a stack of unused seat IDs,
                    // so “departures” new seats appear at the top.