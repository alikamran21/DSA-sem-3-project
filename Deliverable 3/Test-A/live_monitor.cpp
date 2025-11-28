// string last = circularQueue.getLast();
// string now  = circularQueue.getCurrent();

// bool ok = graph.isValidTransition(last, now);

// if (!ok) {
//     cout << "[ALERT] Illegal sequence: " << last << " → " << now << endl;
//     trigger_shutdown();
// }

// function that connets circular queue to graph transitions 



// marking functions that stores all the anomilaies being detected

// string last = cq.getLastAction();
// string now  = cq.getCurrentAction();

// int scoreAdd = 0;

// // SEQUENCE CHECK
// if (!graph.isValidTransition(last, now)) 
//     markAdd += 3;

// // PROFILE CHECK
// if (profile->isRare(now))
//     markAdd += 2;

// // TIMING CHECK
// if (isTimingWeird(now))
//     markAdd += 1;

// // Update live score
// markinging.addMark(scoreAdd);

// // Decision:
// if (marking.shouldShutdown()) {
//     shutdownSystem();
// }