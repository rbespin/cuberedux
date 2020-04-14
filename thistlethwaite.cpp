/* Name: Robert Espinoza
 * Email: rbespinoza96@gmail.com
 * Description: The purpose of this program is to solve a scrambled Rubik's
 * cube utilizing Thistlethwaite's algorithm. This is the fifth attempt at
 * optimizing the cubes performance to perform with speed.
 * Sources:
 * https://www.doc.ic.ac.uk/teaching/distinguished-projects/2015/l.hoang.pdf?fbclid=IwAR0Xzs7hcgA3IrpyyGpRHFPse1APiu1FTulxuCd3ey3voUU1yJrBTMaRf98
 * https://www.jaapsch.net/puzzles/thistle.htm?fbclid=IwAR3FaRYYlOFQjzTxOfePSiQXv2CVFs0aAF1Bfmzx907t8Z1M0rStZYlmnLg#p19
 * https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite.txt
 */

#include <utility>
#include <string>
#include <vector> //vi, vc
#include <iostream> // cout 
#include <stdio.h>  // random
#include <stdlib.h> // random
#include <time.h> // random
#include <queue>
#include <map>
#include <list>
#include <ctime>
#include <fstream>


using namespace std;

//--- Cube will be predefined as a 20 element vector 
//--- <12 edge locations + orientations, 8 corner locations + orientations>
//--- A solved cube will have all cubies in their proper locations with 0
//orientation values, i.e.
//---

typedef vector<int> vi;
typedef vector<char> vc;

//--- Edge identifiers with 0 orientation
char UF = 0;
char UR = 2;
char UB = 4;
char UL = 6;
char FR = 8;
char FL = 10;
char BR = 12;
char BL = 14;
char DF = 16;
char DR = 18;
char DB = 20;
char DL = 22;

//--- Corner identifiers
char UFR = 0;
char UBR = 4;
char UBL = 8;
char UFL = 12;
char DFR = 16;
char DBR = 20;
char DBL = 24;
char DFL = 28;

vc initaliazedCube =
{UF,UR,UB,UL,FR,FL,BR,BL,DF,DR,DB,DL,UFR,UBR,UBL,UFL,DFR,DBR,DBL,DFL};

vc applyMove(int move, vc state){
   // 0,6,12 - R[0]
   // 1,7,13 - L[1]
   // 2,8,14 - F[2]
   // 3,9,15 - B[3]
   // 4,10,16 - U[4]
   // 5,11,17 - D[5]

   // Basic properties:

   // Edge orientation 0 to be defined as an edge that can be returned to its
   // proper location and orientation with an even amount of <U/D> operations

   // Edge orientation 1 to be defined as otherwise

   // Edge orientations flip for only <U,D> operations

   // Corner orientation to be defined as GOOD, or 0, if there exists an L/R
   // sticker facing either L/R face

   // Corner orientation to be defined as 1 if L/R sticker requires a single
   // clockwise cubie twist to return to L/R face

   // Corner orientation to be defined as 2 if L/R sticker requires a single 
   // counter-clockwise cube twist to return to L/R face

   // Corner orientations flip for only <F,B,U,D> operations

   if( move == 0 ){ // R

	  // update edges
	  swap( state[4], state[1] );
	  swap( state[4], state[6] );
	  swap( state[4], state[9] );

	  // update corners
	  swap( state[16], state[12]);
	  swap( state[16], state[13]);
	  swap( state[16], state[17]);

	  return state;

   }

   if( move == 1 ){ // L 

	  // update edges
	  swap( state[7], state[3] );
	  swap( state[7], state[5] );
	  swap( state[7], state[11] );

	  // update corners
	  swap( state[14], state[15] );
	  swap( state[14], state[19] );
	  swap( state[14], state[18] );

	  return state;

   }
   if( move == 2 ){ // F

	  // update edges
	  swap( state[5], state[0] );
	  swap( state[5], state[4] );
	  swap( state[5], state[8] );

	  // update corners orientations
	  state[12] = ((state[12] >> 2) << 2) | ((int(state[12] & 3) + 2) % 3) ;
	  state[16] = ((state[16] >> 2) << 2) | ((int(state[16] & 3) + 1) % 3) ;
	  state[19] = ((state[19] >> 2) << 2) | ((int(state[19] & 3) + 2) % 3) ;
	  state[15] = ((state[15] >> 2) << 2) | ((int(state[15] & 3) + 1) % 3) ;

	  // update corners
	  swap( state[15], state[12] );
	  swap( state[15], state[16] );
	  swap( state[15], state[19] );

	  return state;

   }

   if( move == 3 ){ // B
	  // update edges
	  swap( state[6], state[2] );
	  swap( state[6], state[7] );
	  swap( state[6], state[10] );

	  // update corners orientations
	  // 1+1;2+2;6+1;5+2;
	  state[13] = ((state[13] >> 2) << 2) | ((int(state[13] & 3) + 1) % 3) ;
	  state[14] = ((state[14] >> 2) << 2) | ((int(state[14] & 3) + 2) % 3) ;
	  state[18] = ((state[18] >> 2) << 2) | ((int(state[18] & 3) + 1) % 3) ;
	  state[17] = ((state[17] >> 2) << 2) | ((int(state[17] & 3) + 2) % 3) ;

	  // update corners
	  swap( state[17], state[13] );
	  swap( state[17], state[14] );
	  swap( state[17], state[18] );

	  return state;

   }

   if( move == 4 ){ // U 
	  // update edge orientations
	  state[0] = ((state[0] >> 1) << 1) | ((int(state[0] & 1) + 1) % 2) ;
	  state[1] = ((state[1] >> 1) << 1) | ((int(state[1] & 1) + 1) % 2) ;
	  state[2] = ((state[2] >> 1) << 1) | ((int(state[2] & 1) + 1) % 2) ;
	  state[3] = ((state[3] >> 1) << 1) | ((int(state[3] & 1) + 1) % 2) ;

	  // update corner orientations
	  state[12] = ((state[12] >> 2) << 2) | ((int(state[12] & 3) + 1) % 3) ;
	  state[13] = ((state[13] >> 2) << 2) | ((int(state[13] & 3) + 2) % 3) ;
	  state[14] = ((state[14] >> 2) << 2) | ((int(state[14] & 3) + 1) % 3) ;
	  state[15] = ((state[15] >> 2) << 2) | ((int(state[15] & 3) + 2) % 3) ;

	  // update edge locations
	  swap( state[1], state[0] );
	  swap( state[1], state[3] );
	  swap( state[1], state[2] );

	  // update corner locations
	  swap( state[13], state[12] );
	  swap( state[13], state[15] );
	  swap( state[13], state[14] );

	  return state;

   }

   if( move == 5 ){ // D
	  // update edge orientations
	  state[8] = ((state[8] >> 1) << 1) | ((int(state[8] & 1) + 1) % 2) ;
	  state[9] = ((state[9] >> 1) << 1) | ((int(state[9] & 1) + 1) % 2) ;
	  state[10] = ((state[10] >> 1) << 1) | ((int(state[10] & 1) + 1) % 2) ;
	  state[11] = ((state[11] >> 1) << 1) | ((int(state[11] & 1) + 1) % 2) ;

	  // update corner orientations
	  state[16] = ((state[16] >> 2) << 2) | ((int(state[16] & 3) + 2) % 3) ;
	  state[17] = ((state[17] >> 2) << 2) | ((int(state[17] & 3) + 1) % 3) ;
	  state[18] = ((state[18] >> 2) << 2) | ((int(state[18] & 3) + 2) % 3) ;
	  state[19] = ((state[19] >> 2) << 2) | ((int(state[19] & 3) + 1) % 3) ;

	  // update edge locations
	  swap( state[11], state[8] );
	  swap( state[11], state[9] );
	  swap( state[11], state[10] );

	  // update corner locations
	  swap( state[19], state[16] );
	  swap( state[19], state[17] );
	  swap( state[19], state[18] );

	  return state;

   }


   if( move == 6 ){ // R2
	  // update edge locations
	  swap( state[1], state[9] );
	  swap( state[4], state[6] );

	  // update corner orientations
	  swap( state[12], state[17] );
	  swap( state[13], state[16] );

	  return state;

   }

   if( move == 7 ){ // L2
	  // update edge locations
	  swap( state[3], state[11] );
	  swap( state[5], state[7] );

	  // update corner orientations
	  swap( state[14], state[19] );
	  swap( state[15], state[18] );
	  return state;

   }
   if( move == 8 ){ // F2
	  // update edge locations
	  swap( state[0], state[8] );
	  swap( state[4], state[5] );

	  // update corner orientations
	  swap( state[12], state[19] );
	  swap( state[15], state[16] );

	  return state;
   }
   if( move == 9 ){ // B2
	  // update edge locations
	  swap( state[2], state[10] );
	  swap( state[6], state[7] );

	  // update corner orientations
	  swap( state[13], state[18] );
	  swap( state[14], state[17] );

	  return state;
   }
   if( move == 10 ){ // U2
	  // update edge locations
	  swap( state[0], state[2] );
	  swap( state[1], state[3] );

	  // update corner orientations
	  swap( state[12], state[14] );
	  swap( state[13], state[15] );

	  return state;

   }
   if( move == 11 ){ // D2
	  // update edge locations
	  swap( state[8], state[10] );
	  swap( state[9], state[11] );

	  // update corner orientations
	  swap( state[16], state[18] );
	  swap( state[17], state[19] );

	  return state;

   }
   if( move == 12 ){ // R' (R3)

	  // update edges
	  swap( state[6], state[1] );
	  swap( state[6], state[4] );
	  swap( state[6], state[9] );

	  // update corners
	  swap( state[13], state[12]);
	  swap( state[13], state[16]);
	  swap( state[13], state[17]);

	  return state;
   }
   if( move == 13 ){ // L' (L3)

	  // update edges
	  swap( state[5], state[3] );
	  swap( state[5], state[7] );
	  swap( state[5], state[11] );

	  // update corners
	  swap( state[19], state[15]);
	  swap( state[19], state[14]);
	  swap( state[19], state[18]);

	  return state;


   }
   if( move == 14 ){ // F'
	  // update edges
	  swap( state[4], state[0] );
	  swap( state[4], state[5] );
	  swap( state[4], state[8] );

	  // update corners orientations
	  state[12] = ((state[12] >> 2) << 2) | ((int(state[12] & 3) + 2) % 3) ;
	  state[16] = ((state[16] >> 2) << 2) | ((int(state[16] & 3) + 1) % 3) ;
	  state[19] = ((state[19] >> 2) << 2) | ((int(state[19] & 3) + 2) % 3) ;
	  state[15] = ((state[15] >> 2) << 2) | ((int(state[15] & 3) + 1) % 3) ;

	  // update corners
	  swap( state[16], state[12] );
	  swap( state[16], state[15] );
	  swap( state[16], state[19] );

	  return state;
   }
   if( move == 15 ){ // B' (B3)
	  // update edges
	  swap( state[7], state[2] );
	  swap( state[7], state[6] );
	  swap( state[7], state[10] );

	  // update corners orientations
	  state[13] = ((state[13] >> 2) << 2) | ((int(state[13] & 3) + 1) % 3) ;
	  state[14] = ((state[14] >> 2) << 2) | ((int(state[14] & 3) + 2) % 3) ;
	  state[18] = ((state[18] >> 2) << 2) | ((int(state[18] & 3) + 1) % 3) ;
	  state[17] = ((state[17] >> 2) << 2) | ((int(state[17] & 3) + 2) % 3) ;

	  // update corners
	  swap( state[14], state[13] );
	  swap( state[14], state[17] );
	  swap( state[14], state[18] );

	  return state;
   }
   if( move == 16 ){ // U' (U3)
	  // update edge orientations
	  state[0] = ((state[0] >> 1) << 1) | ((int(state[0] & 1) + 1) % 2) ;
	  state[1] = ((state[1] >> 1) << 1) | ((int(state[1] & 1) + 1) % 2) ;
	  state[2] = ((state[2] >> 1) << 1) | ((int(state[2] & 1) + 1) % 2) ;
	  state[3] = ((state[3] >> 1) << 1) | ((int(state[3] & 1) + 1) % 2) ;

	  // update corner orientations
	  state[12] = ((state[12] >> 2) << 2) | ((int(state[12] & 3) + 1) % 3) ;
	  state[13] = ((state[13] >> 2) << 2) | ((int(state[13] & 3) + 2) % 3) ;
	  state[14] = ((state[14] >> 2) << 2) | ((int(state[14] & 3) + 1) % 3) ;
	  state[15] = ((state[15] >> 2) << 2) | ((int(state[15] & 3) + 2) % 3) ;

	  // update edge locations
	  swap( state[3], state[0] );
	  swap( state[3], state[1] );
	  swap( state[3], state[2] );

	  // update corner locations
	  swap( state[15], state[12] );
	  swap( state[15], state[13] );
	  swap( state[15], state[14] );

	  return state;
   }
   if( move == 17 ){ // D' (D3)
	  // update edge orientations
	  state[8] = ((state[8] >> 1) << 1) | ((int(state[8] & 1) + 1) % 2) ;
	  state[9] = ((state[9] >> 1) << 1) | ((int(state[9] & 1) + 1) % 2) ;
	  state[10] = ((state[10] >> 1) << 1) | ((int(state[10] & 1) + 1) % 2) ;
	  state[11] = ((state[11] >> 1) << 1) | ((int(state[11] & 1) + 1) % 2) ;

	  // update corner orientations
	  state[16] = ((state[16] >> 2) << 2) | ((int(state[16] & 3) + 2) % 3) ;
	  state[17] = ((state[17] >> 2) << 2) | ((int(state[17] & 3) + 1) % 3) ;
	  state[18] = ((state[18] >> 2) << 2) | ((int(state[18] & 3) + 2) % 3) ;
	  state[19] = ((state[19] >> 2) << 2) | ((int(state[19] & 3) + 1) % 3) ;

	  // update edge locations
	  swap( state[9], state[8] );
	  swap( state[9], state[11] );
	  swap( state[9], state[10] );

	  // update corner locations
	  swap( state[17], state[16] );
	  swap( state[17], state[19] );
	  swap( state[17], state[18] );

	  return state;
   }
   return state;
}

// Define the integer change to result in an inverse move upon inputting
int inverse(int move){
   if( move <= 5 ){
	  move = move+12;
	  return move;
   }
   if( move >= 12 ){
	  move = move - 12;
	  return move;
   }
   else{
	  return move;
   }
}

// Apply random moves 0-17 to a state and return the path
vi scramble(int number_of_moves, vc & state){
   int random;
   srand(time(NULL));
   vi path;

   for( int i = 0; i < number_of_moves; i++){
	  random = rand()%18;
	  state = applyMove(random, state);
	  path.push_back(random);
   }
   return path;
}

//--- Gives the relevant info at the current phase the cube is currently in
//--- These are the only relevant information pieces for that particular
//phase. Elements in the return vector will 'track' the pieces for the breadth
//first search.
vc id(vc & state, int phase){

   //--- Phase 1
   // fix edge orientations
   if( phase == 1 ){
	  vc id;
	  for(int i = 0; i < 12; i++ ){
		 id.push_back(state[i] & 1);
	  }
	  return id;
   }

   //--- Phase 2
   // fix corner orientations, M-Slice
   if( phase == 2 ){
	  vc id;

	  // ensuring edge orientations are zero
	/*  for( int i = 0; i < 12; i++){
		 id.push_back(state[i]&1);
	  } */
	  // corner orientations to be zero
	  for( int i = 12; i < 20; i++){
		 id.push_back(state[i]&3);
	  }
	  /*	  // Ensuring corners have L/R stickers facing L/R faces
	  // This is how Thistlethwaite defines an oriented corner.
	  // Corners 1,3,4,6 will naturally fall into place if corners 0,2,5,7 are
	  // put into their proper groups
	  for( int i = 12; i < 20; i++ ){
	  if( (state[i] & 28) == UFR ||
	  (state[i] & 28) == UBL ||
	  (state[i] & 28) == DBR ||
	  (state[i] & 28) == DFL ){
	  id.push_back(i);
	  }
	  } // we can also try & 28 for (11100) to forget least 2 significant bits
	   */

	  // M-Slice (slice between L/R face)
	  // push back indices of 0,2,8,10 to track 
	  for( int i = 0; i < 12; i++ ){
		 if( ((state[i]&30) == UF) || 
			   ((state[i]&30) == UB) || 
			   ((state[i]&30) == DF) || 
			   ((state[i]&30) == DB) ){
			id.push_back(i);
		 }
	  } // we can also try ( & 30 ) for (11110) to forget least 2 significant
	  //bits
	  return id;
   }

   //--- Phase 3
   //E-Slice, S-Slice, corner tetrads
   if( phase == 3 ){
	  vc id;

	  //----------------------------------------------

	  // E-slice: 4,5,6,7
	  for( int i = 0; i < 12; i++ ){
		 if( ((state[i]&30) == FR) || 
			   ((state[i]&30) == FL) || 
			   ((state[i]&30) == BR) || 
			   ((state[i]&30) == BL) ){
			id.push_back(i);
		 }
	  }
	  // S-Slice cubies will naturally fall into place with E-slice cubies put
	  // into place

	  //--- Ensuring even parity with tetrad edges. 
	  int tetrad_edge = 0;
	  for( int i=0; i<12; i++ ){
		 for( int j=i+1; j<12; j++ ){
			// If state[i] > state[j], then state[i] is in an incorrect cubie
			// location. If there are an odd number of these, then the cube is
			// unsolvable with g4 group moves. Therefore, we must ensure even
			// parity for corner tetrad pair swaps.
			if( (state[i]&30) > (state[j]&30)){
			   tetrad_edge++;
			}
		 }
	  }
	  if( (tetrad_edge%2) == 0 ){
		 id.push_back(tetrad_edge);
	  } 


	  //-----------------------------------------------------------

	  // Corner orbits:
	  // UFR and UBL can only be in position 0, 2, 5, 7 
	  // UBR and UFL can only be in position 1, 3, 4, 6
	  // DFR and DBL can only be in position 1, 3, 4, 6
	  // DBR and DFL can only be in position 0, 2, 5, 7

	  //------ Placing corners into proper tetrads
	  // Making sure 0,2,5,7 are in 0.2.5.7
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == UFR ||
			   (state[i]&28) == UBL ||
			   (state[i]&28) == DBR ||
			   (state[i]&28) == DFL ){
			id.push_back(i);
		 }
	  }

	  // Ensuring that UFR and UBL, DBR and DFL are paired
	  // Track the pairs 0,2; 5,7
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == UFR ||
			   (state[i]&28) == UBL ){
			id.push_back(i);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == DBR ||
			   (state[i]&28) == DFL ){
			id.push_back(i);
		 }
	  }

	  for( int i = 12; i < 20; i++ ){
		 // Making sure 1,3,4,6 are in 1,3,4,6
		 if( (state[i]&28) == UBR ||
			   (state[i]&28) == UFL ||
			   (state[i]&28) == DFR ||
			   (state[i]&28) == DBL ){
			id.push_back(i);
		 }
	  } 
	  // Track the pairs 1,3; 4,6 and make sure they're paired
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == UBR ||
			   (state[i]&28) == UFL ){
			id.push_back(i);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == DFR ||
			   (state[i]&28) == DBL ){
			id.push_back(i);
		 }
	  }  

	  //--- Ensuring even parity with tetrad pairs. 
	  int tetrad = 0;
	  for( int i=12; i<20; i++ ){
		 for( int j=i+1; j<20; j++ ){
			// If state[i] > state[j], then state[i] is in an incorrect cubie
			// location. If there are an odd number of these, then the cube is
			// unsolvable with g4 group moves. Therefore, we must ensure even
			// parity for corner tetrad pair swaps.
			if( (state[i]&28) > (state[j]&28)){
			   tetrad++;
			}
		 }
	  }
	  if( (tetrad%2) == 0 ){
		 id.push_back(tetrad);
	  }

	  //-----------------------------------------------------------
	  return id;
   } 

   //--- Phase 4
   // Fix remaining cubes with proper group moves
   if( phase == 4 ){

	  return state;
   }

   return state;
}

vector<vi> applicableMoves{
   { 0 }, // g0 group
	  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}, // moves to transition to g1 // phase 1
	  {0,1,2,3,6,7,8,9,10,11,12,13,14,15}, // moves to transition to g2 // phase 2
	  {0,1,8,9,10,11,12,13}, // moves to transition to g3 // phase 3
	  {6,7,8,9,10,11} // moves to transition to g4 // phase 4
};

// Bidirectional Breadth First Search
vi BDBFS(vc & startState, vc goalState, int phase){

   // compute start state ID, goal state ID
   vc startID = id(startState, phase);
   vc goalID = id(goalState, phase);

   // initialize queues for forward and backward search
   // queue of states
   queue<vc> q;
   q.push(startState);
   q.push(goalState);

   // initialize tables for BFS
   map<vc, int> direction;
   map<vc, int> lastMove;
   map<vc, vc> predecessor;

   // initialize directions for starting states
   direction[startID] = 1;
   direction[goalID] = 2;

   // Already in phase, return
   if( startID == goalID ){
	  vi retPath;
	  return retPath;
   }

   // begin BFS for particular phase
   while(!q.empty()){

	  // get information from queue
	  vc oldState = q.front();
	  q.pop();
	  vc oldID = id(oldState, phase);
	  int& oldDir = direction[oldID];

	  // Get appropriate moveset for group
	  vi moveSet = applicableMoves[phase];
	  for( int i = 0; i < moveSet.size(); i++ ){
		 int move = moveSet[i];

		 vc newState = applyMove(move, oldState);
		 vc newID = id(newState, phase);
		 int& newDir = direction[newID];

		 //--- newDir == 0 if it is a new direction
		 //--- newDir == 1 if we have seen this from the forward search
		 //--- newDir == 2 if if we have seen this from the backward search
		 //--- if newDir > 0, and newDir != oldDir, then we have found a
		 //connecting path from forward and backward search

		 if( (newDir > 0) && (newDir != oldDir) ){

			// if newDir is 1, we are coming from backwards search
			if( newDir == 1 ){

			   vi path;
			   // rebuild path from newID -> startID
			   while( newID != startID ){
				  path.insert(path.begin(), lastMove[newID]);
				  newID = predecessor[newID];
			   }

			   // Applying connecting move
			   path.push_back(inverse(move));

			   // rebuild path from oldID -> goalID
			   while( oldID != goalID ){
				  path.push_back(inverse(lastMove[oldID]));
				  oldID = predecessor[oldID];
			   }

			   // Applying path to input starting state
			   for( int i = 0; i < path.size(); i++ ){
				  startState = applyMove(path[i], startState);
			   }
			   return path;

			}
			// we are coming from forward search
			if( newDir == 2 ){

			   vi path;

			   // rebuild path from oldID -> startID
			   while( oldID != startID ){
				  path.insert(path.begin(), lastMove[oldID]);
				  oldID = predecessor[oldID];
			   }

			   // Applying connecting move
			   path.push_back((move));

			   // rebuild path from newID -> goalID
			   while( newID != goalID ){
				  path.push_back(inverse(lastMove[newID]));
				  newID = predecessor[newID];
			   }


			   // Applying path to input starting state
			   for( int i = 0; i < path.size(); i++ ){
				  startState = applyMove(path[i], startState);
			   }
			   return path;

			}
		 }

		 // only insert into queue if we have not seen this ID
		 if( ! newDir ){
			q.push(newState);
			newDir = oldDir;
			lastMove[newID] = move;
			predecessor[newID] = oldID;
		 }
	  }
   }

   // safeguard for returning
   vi path;
   return path;
}

vector<string> movesString{ "R", "L", "F", "B", "U", "D", "R2", "L2", "F2", 
   "B2", "U2", "D2", "R'", "L'", "F'", "B'", "U'", "D'" };

// Add phase paths into a single string
void build_path( vi path, string & build ){
   for( int i = 0; i < path.size(); i++ ){
	  build += ( movesString[path[i]] + " " );
   }
}

bool DLS( vc & startState, vc & goalID, int limit, map<vc,int> & lastMove,
	  map<vc,vc> & predecessor, int phase, map<vc,vc> & visited){

   // get oldState, oldID
   vc oldState = startState;
   vc oldID = id(oldState, phase);

   // if we have arrived at goalID, return true
   if( oldID == goalID ){
	  // our maps should have been initialized by now
	  return true;
   }

   // if we have reached depth, we return false since no sol found
   if( limit <= 0 ){
	  return false;
   }

   vi moveSet = applicableMoves[phase];
   for( int i = 0; i < moveSet.size(); i++ ){
	  int move = moveSet[i];
	  //move = (move*19)%moveSet.size();

	  // get newState, perform DLS at next depth
	  vc newState = applyMove(move, oldState);
	  vc newID = id(newState,phase);

	  // we recur at this ID if we have not seen it before
	  // recur at this ID for all moves
	  //	  if( visited[newID].size() == 0 ){
	  //		 visited[newID].push_back(limit);
	  if( DLS(newState, goalID, (limit-1), lastMove, predecessor, phase,
			   visited) == true){
		 cout << move << " ";
		 lastMove[newID] = move;
		 predecessor[newID] = oldID;
		 return true;
	  }
	  //	  }
	  //	  else{
	  //		 if( int((visited[newID])[0]) > limit ){
	  //			(visited[newID])[0] = limit;
	  //		 }
	  //	  }
   }

   return false;

}

vi IDDFS ( vc src, vc target, int limit, int phase ){

   // initialize maps 
   map<vc,int> lastMove;
   map<vc,vc> predecessor;
   map<vc,vc> visited; // <stateID, depth discovered>

   // get goalID
   vc goalID = id(target,phase);
   vc startID = id(src,phase);

   // return if we are already in phase
   if( startID == goalID ){
	  cout << "already in phase" << endl;
	  vi path;
	  return path;
   }

   // perform DLS iteratively at an increasing depth level
   for( int i = 0; i <= limit; i++ ){
	  if(DLS( src, goalID, i, lastMove, predecessor, phase, visited) == true){

		 // rebuild path
		 vi path;
		 while( goalID != startID ){
			path.insert(path.begin(), (lastMove)[goalID]);
			goalID = (predecessor)[goalID];
		 } 
		 cout << endl;
		 cout << "Solution found at depth: " << i << endl;
		 return path;
	  }
	  else{
		 // lastMove.clear();
		 // predecessor.clear();
		 visited.clear(); // might not be necessary
		 cout << "no sol reachable..." << endl;
		 cout << "trying next iteration at depth: " << i << endl;
	  }
   }

   vi path;
   return path;
}

int main(int argc, char** argv){

   vc cube = 
   {UF,UR,UB,UL,FR,FL,BR,BL,DF,DR,DB,DL,UFR,UBR,UBL,UFL,DFR,DBR,DBL,DFL};

   vc goalCube = 
   {UF,UR,UB,UL,FR,FL,BR,BL,DF,DR,DB,DL,UFR,UBR,UBL,UFL,DFR,DBR,DBL,DFL};


   for( int i = 0; i < cube.size(); i++){
	  if( i < 12 ){ cout << int(cube[i])/2 << " ";}
	  else{ cout << int(cube[i])/4 << " ";}
   }cout << endl; 

   

   vi scramble_path = scramble(15, cube);

   int phase = 1;
   int depth = 100;

   string build; // complete solution string

   // function to convert int moves to string literals
  // build_path(path, build);

   vi path;
   path = IDDFS(cube, goalCube, depth, phase);
   for( int i = 0; i < path.size(); i++){
	  cube = applyMove(path[i], cube);
   }

   for( int i = 0; i < cube.size(); i++){
	  if( i < 12 ){
		 cout << int(cube[i]&1) << " ";
	  }
	  else{ 
		 cout << int(cube[i])/4 << " ";
	  }
   }
   cout << endl;

   build_path(path, build);

   cout << build << endl;

   phase = 2;
   path = IDDFS(cube, goalCube, depth, phase);
   for( int i = 0; i < path.size(); i++){
	  cube = applyMove(path[i], cube);
   }

   for( int i = 0; i < cube.size(); i++){
	  if( i < 12 ){
		 cout << int(cube[i]&1) << " ";
	  }
	  else{ 
		 cout << int(cube[i]&3) << " ";
	  }
   }
   cout << endl; 

   build_path(path, build);

   cout << build << endl; 
/*
   phase = 3;
   path = IDDFS(cube, goalCube, depth, phase);
   for( int i = 0; i < path.size(); i++){
	  cube = applyMove(path[i], cube);
   }

   for( int i = 0; i < cube.size(); i++){
	  if( i < 12 ){
		 cout << int(cube[i]&1) << " ";
	  }
	  else{ 
		 cout << int(cube[i]&3) << " ";
	  }
   }
   cout << endl; 

   phase = 4;
   path = IDDFS(cube, goalCube, depth, phase);
   for( int i = 0; i < path.size(); i++){
	  cube = applyMove(path[i], cube);
   }

   for( int i = 0; i < cube.size(); i++){
	  if( i < 12 ){
		 cout << int(cube[i]&1) << " ";
	  }
	  else{ 
		 cout << int(cube[i]&3) << " ";
	  }
   }
   cout << endl; 
*/


/*

   // Scramble the cube
   /////////////////
   
	  vi scramble_path = scramble(30, cube);
	  string sp;
   //build_path(scramble_path, sp);

   // begin solving cube by iteratively going through the 4 phases
   vi path; // intermediate phase solution
   string build; // complete solution string

   int phase = 1;
   path = BDBFS( cube, goalCube, phase );
   build_path(path, build);
   for( int i = 0; i < cube.size(); i++){
   if( i < 12 ){
   cout << int(cube[i]&1) << " ";
   }
   else{ 
   cout << int(cube[i]&3) << " ";
   }
   } cout << endl;

   phase = 2;
   path = BDBFS( cube, goalCube, phase );
   build_path(path, build);
   for( int i = 0; i < cube.size(); i++){
   if( i < 12 ){
   cout << int(cube[i]&1) << " ";
   }
   else{ 
   cout << int(cube[i]&3) << " ";
   }
   } cout << endl;
   /////////////
	*/
   /*
	  phase = 3;
	  path = BDBFS( cube, goalCube, phase );
	  build_path(path, build);

	  phase = 4;
	  path = BDBFS( cube, goalCube, phase );
	  build_path(path, build);

   // Print scramble path, solution
   cout << sp << endl;
   cout << build << endl;
	*/
   /*   int phase = 1;

		vi path;
		string build;

		cube = applyMove(5, cube);
		cube = applyMove(4, cube);

		cube = applyMove(3, cube);
		cube = applyMove(1, cube);

		path = IDDFS(cube, goalCube, 4, phase);
		build_path(path, build);
	*/
   /*   phase++;
		path = IDDFS(cube, goalCube, 50, phase);
		build_path(path, build);

		phase++;
		path = IDDFS(cube, goalCube, 50, phase);
		build_path(path, build);

		phase++;
		path = IDDFS(cube, goalCube, 50, phase);
		build_path(path, build);
	*/
   // cout << sp << endl;
   //` cout << build << endl;





   /*   int phase = 3;

   //  initializeCube = applyMove(2, initializeCube);
   //  initializeCube = applyMove(inverse(2), initializeCube);

   vi scramble_one = scramble(1000, initializeCube);

   reverse(scramble_one.begin(), scramble_one.end());
   for( int i = 0; i < scramble_one.size(); i++){
   initializeCube = applyMove(inverse(scramble_one[i]), initializeCube);
   }

   for( int i = 0; i < initializeCube.size(); i++){
   cout << int(initializeCube[i]) << " ";
   }cout << endl;

   vc phase2 = id(initializeCube, phase);

   for( int i = 0; i < phase2.size(); i++){
   cout << int(phase2[i]) << " ";
   } cout << endl;  
	*/

}
