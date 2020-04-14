bool DLS( vc & startState, vc & goalID, int limit, map<vc,int> & lastMove,
	  map<vc,vc> & predecessor, int phase ){
   vc oldState = startState;
   vc oldID = id(oldState, phase);
   if( oldID == goalID ){
	  // We have found a solution, and our tables are already initialized
	  return true;
   }
   if( limit <= 0 ){
	  return false; // no solution found for this depth
   }
   // for each edge of src, we recurse with DLS for max depth
   vi moveSet = applicableMoves[phase];
   for(int i = 0; i < moveSet.size(); i++ ){
	  int move = moveSet[i];
	  vc newState = applyMove(move, oldState);
	  vc newID = id(newState, phase);

	  // dummy statement
	  if( DLS(newState, goalID, limit-1, lastMove, predecessor, phase) == true){
		 return true;
	  }


	  /*	  if( newID == goalID ){
			  lastMove[newID] = move;
			  predecessor[newID] = oldID;
	  // We have found a solution, and our tables are already initialized
	  return true;
	  }

	  // if we have not seen this id before, we add it to ID's seen
	  if( predecessor[newID].size() == 0 ){
	  lastMove[newID] = move;
	  predecessor[newID] = oldID;
	  // recur at this ID for all moves
	  if( DLS(newState, goalID, limit-1, lastMove, predecessor, phase) == true){
	  return true;
	  }
	  } */
   }
   return false; // no solution found for such depth
}


vi IDDFS ( vc src, vc target, int limit, int phase ){
   vc startID = id(src, phase);
   vc goalID = id(target, phase);
   // initialize tables for IDDFS
   map<vc, int> lastMove;
   map<vc, vc> predecessor;
   if( startID == goalID ){
	  cout << "Cube already in phase!" << endl;
	  vi path;
	  return path;
   }
   for(int i = 0; i <= limit; i++){
	  cout << "Checking depth: " << i << endl;
	  if( DLS( src, goalID, limit, lastMove, predecessor, phase ) == true ){
		 cout << "solution found at depth: " << i << endl;
		 vi path;
		 /* while( goalID != startID ){
			path.insert(path.begin(), lastMove[goalID]);
			goalID = predecessor[goalID];
			} */
		 return path;
	  }
	  if( DLS( src, goalID, limit, lastMove, predecessor, phase ) == false ){
		 cout << "No solution found for depth: " << i << ", moving on..." << endl;
	  }
   }
   cout << "No path found for maximum depth: " << limit << endl;
   vi path;
   return path;
}