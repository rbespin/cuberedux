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
#include <unordered_map>
#include <algorithm>


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

vector<vc> twists = { { 0,1,2,3,4,5,6,7 }, { 0,1,2,3,6,7,4,5 }, { 0,1,5,4,3,2,6,7 },
   { 0,1,5,4,6,7,3,2 }, { 0,1,7,6,3,2,4,5 }, { 0,1,7,6,4,5,3,2 }, 
   { 0,3,2,1,4,7,6,5 }, { 0,3,2,1,6,5,4,7 }, { 0,3,5,6,1,2,4,7 }, 
   { 0,3,5,6,4,7,1,2 }, { 0,3,7,4,1,2,6,5 }, { 0,3,7,4,6,5,1,2 }, 
   { 0,4,2,6,1,5,3,7 }, { 0,4,2,6,3,7,1,5 }, { 0,4,5,1,3,7,6,2 }, 
   { 0,4,5,1,6,2,3,7 }, { 0,4,7,3,1,5,6,2 }, { 0,4,7,3,6,2,1,5 }, 
   { 0,6,2,4,1,7,3,5 }, { 0,6,2,4,3,5,1,7 }, { 0,6,5,3,1,7,4,2 }, 
   { 0,6,5,3,4,2,1,7 }, { 0,6,7,1,3,5,4,2 }, { 0,6,7,1,4,2,3,5 }, 
   { 2,1,0,3,4,7,6,5 }, { 2,1,0,3,6,5,4,7 }, { 2,1,5,6,3,0,4,7 }, 
   { 2,1,5,6,4,7,3,0 }, { 2,1,7,4,3,0,6,5 }, { 2,1,7,4,6,5,3,0 }, 
   { 2,3,0,1,4,5,6,7 }, { 2,3,0,1,6,7,4,5 }, { 2,3,5,4,1,0,6,7 }, 
   { 2,3,5,4,6,7,1,0 }, { 2,3,7,6,1,0,4,5 }, { 2,3,7,6,4,5,1,0 }, 
   { 2,4,0,6,1,7,3,5 }, { 2,4,0,6,3,5,1,7 }, { 2,4,5,3,1,7,6,0 }, 
   { 2,4,5,3,6,0,1,7 }, { 2,4,7,1,3,5,6,0 }, { 2,4,7,1,6,0,3,5 }, 
   { 2,6,0,4,1,5,3,7 }, { 2,6,0,4,3,7,1,5 }, { 2,6,5,1,3,7,4,0 }, 
   { 2,6,5,1,4,0,3,7 }, { 2,6,7,3,1,5,4,0 }, { 2,6,7,3,4,0,1,5 }, 
   { 5,1,0,4,3,7,6,2 }, { 5,1,0,4,6,2,3,7 }, { 5,1,2,6,3,7,4,0 }, 
   { 5,1,2,6,4,0,3,7 }, { 5,1,7,3,4,0,6,2 }, { 5,1,7,3,6,2,4,0 }, 
   { 5,3,0,6,1,7,4,2 }, { 5,3,0,6,4,2,1,7 }, { 5,3,2,4,1,7,6,0 }, 
   { 5,3,2,4,6,0,1,7 }, { 5,3,7,1,4,2,6,0 }, { 5,3,7,1,6,0,4,2 }, 
   { 5,4,0,1,3,2,6,7 }, { 5,4,0,1,6,7,3,2 }, { 5,4,2,3,1,0,6,7 }, 
   { 5,4,2,3,6,7,1,0 }, { 5,4,7,6,1,0,3,2 }, { 5,4,7,6,3,2,1,0 }, 
   { 5,6,0,3,1,2,4,7 }, { 5,6,0,3,4,7,1,2 }, { 5,6,2,1,3,0,4,7 }, 
   { 5,6,2,1,4,7,3,0 }, { 5,6,7,4,1,2,3,0 }, { 5,6,7,4,3,0,1,2 }, 
   { 7,1,0,6,3,5,4,2 }, { 7,1,0,6,4,2,3,5 }, { 7,1,2,4,3,5,6,0 }, 
   { 7,1,2,4,6,0,3,5 }, { 7,1,5,3,4,2,6,0 }, { 7,1,5,3,6,0,4,2 }, 
   { 7,3,0,4,1,5,6,2 }, { 7,3,0,4,6,2,1,5 }, { 7,3,2,6,1,5,4,0 }, 
   { 7,3,2,6,4,0,1,5 }, { 7,3,5,1,4,0,6,2 }, { 7,3,5,1,6,2,4,0 }, 
   { 7,4,0,3,1,2,6,5 }, { 7,4,0,3,6,5,1,2 }, { 7,4,2,1,3,0,6,5 }, 
   { 7,4,2,1,6,5,3,0 }, { 7,4,5,6,1,2,3,0 }, { 7,4,5,6,3,0,1,2 }, 
   { 7,6,0,1,3,2,4,5 }, { 7,6,0,1,4,5,3,2 }, { 7,6,2,3,1,0,4,5 }, 
   { 7,6,2,3,4,5,1,0 }, { 7,6,5,4,1,0,3,2 }, { 7,6,5,4,3,2,1,0 }  };

int statesVisited = 0;
int statesTotal = 0;

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
	  /*	  for( int i = 12; i < 20; i++ ){
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
			  } */

	  for( int i = 12; i < 20; i++ ){
		 // Making sure 1,3,4,6 are in 1,3,4,6
		 if( (state[i]&28) == UBR ||
			   (state[i]&28) == UFL ||
			   (state[i]&28) == DFR ||
			   (state[i]&28) == DBL ){
			id.push_back(i);
		 }
	  } 
	  /*	  // Track the pairs 1,3; 4,6 and make sure they're paired
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
			  */
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

   if( phase == 5 ){
	  vc check;
	  /*	  for( int i = 12; i < 20; i++ ){
			  check.push_back(state[i]&28);
			  } */
	  // gathering potential corner locations
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == UFR ) {
			check.push_back(i-12);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == UBR ) {
			check.push_back(i-12);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == UBL ) {
			check.push_back(i-12);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == UFL ) {
			check.push_back(i-12);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == DFR ) {
			check.push_back(i-12);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == DBR ) {
			check.push_back(i-12);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == DBL ) {
			check.push_back(i-12);
		 }
	  }
	  for( int i = 12; i < 20; i++ ){
		 if( (state[i]&28) == DFL ) {
			check.push_back(i-12);
		 }
	  }

/*	  for( int i = 0; i < check.size(); i++ ){
		 cout << check[i] << " ";
	  }cout << endl; */

	  vc id;
	  //return check;
	  
	  // check if one of 96 permutations
 	  if( find( twists.begin(), twists.end(), check ) != twists.end() ){
		 id.push_back(1);
	  }
	  else{
		 id.push_back(0);
	  } 
/*	  bool setcheck = false;
	  for( int i = 0; i < twists.size(); i++ ){
		 if( check == twists[i] ){
			cout << "reached" << endl;
			setcheck = true;
		 }
	  }
	  if( setcheck == true ){
		 id.push_back(1);
	  }
	  else{
		 id.push_back(0);
	  }
 */
	  return id; 
   }


   return state;
}

vector<vi> applicableMoves{
   { 0 }, // g0 group
	  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}, // moves to transition to g1 // phase 1
	  {0,1,2,3,6,7,8,9,10,11,12,13,14,15}, // moves to transition to g2 // phase 2
	  {0,1,8,9,10,11,12,13}, // moves to transition to g3 // phase 3
	  {6,7,8,9,10,11}, // moves to transition to g4 // phase 4
	  {6,7,8,9,10,11} // moves to transition to nothing, test // phase 5
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
/*
   bool DLS( vc & startState, vc & goalID, int limit, map<vc,int> & lastMove,
   map<vc,vc> & predecessor, int phase, map<vc,int> & visited, 
   int direction, vi & path){ */
bool DLS( vc & sourceState, vc & targetID, int limit, 
	  map<vc,int> & lastMove, map<vc,vc> & predecessor, int phase, 
	  map<vc,int> & direction, vc & interID, int & interMove, 
	  int & interDir, vc & predInterID, int prevDir){

   // get oldState, oldID
   vc oldState = sourceState;
   vc oldID = id(oldState, phase);
   int & oldDir = direction[oldID];

   // if we have reached depth, we return false since no sol found, 
   // and mark it as visited since it is the shortest solution to this ID
   if( limit <= 0 ){

	  statesVisited++;
	  // if we have found a solution, return true
	  if( oldID == targetID ){

		 cout << "solution found on search" << endl;
		 // if interDir is -1, we found solution with search
		 interDir = -1;
		 return true;
	  }
	  if( direction[oldID] == 0 ){
		 direction[oldID] = prevDir;
		 return false;
	  }
	  // if an intersection is found, initialize intersection map
	  if( (oldDir != 0 ) && (prevDir != oldDir) ){
		 //cout << "solution found at intersection" << endl;
		 // if interDir is not -1 or 0, we found solution with intersection
		 interMove = (lastMove[oldID]);
		 interID = oldID;
		 predInterID = predecessor[oldID];
		 interDir = oldDir;
		 return true;
	  }
	  return false;
   }

   vi moveSet = applicableMoves[phase];
   for( int i = 0; i < moveSet.size(); i++ ){
	  int move = moveSet[i];

	  // get newState, perform DLS at next depth
	  vc newState = applyMove(move, oldState);
	  vc newID = id(newState,phase);
	  int & newDir = direction[newID];

	  bool recursion = DLS( newState, targetID, (limit-1), lastMove,
			predecessor, phase, direction, interID, interMove, interDir,
			predInterID, oldDir );
	  lastMove[newID] = move;
	  predecessor[newID] = oldID;
	  direction[newID] = oldDir;

	  if( recursion == true ){
		 return true;
	  }
   }
   return false;
}

vi IDDFS ( vc src, vc target, int limit, int phase ){

   // initialize maps and preserving intersection variables
   map<vc,int> lastMove;
   map<vc,vc> predecessor;
   map<vc,int> direction;
   vc interID;
   int interMove;
   vc predInterID;
   vi path;
   int interDir;


   // get goalID
   vc goalID = id(target,phase);
   vc startID = id(src,phase);

   // initialize directions
   direction[goalID] = 2;
   direction[startID] = 1;

   // return if we are already in phase
   if( startID == goalID ){
	  cout << "already in phase" << endl;
	  return path;
   }

   // perform DLS iteratively at an increasing depth level
   for( int i = 0; i <= limit; i++ ){

	  cout << "trying iteration at depth: " << i << endl;

	  // perform DLS forward. If true is returned,
	  // we rebuild from goalID->startID
	  //

	  if( DLS(src, goalID, i, lastMove,
			   predecessor, phase, direction, interID,interMove,
			   interDir, predInterID, 1) == true){
		 if( interDir == -1 ){
			// if true, we have found a solution on forward search
			cout << endl;
			cout << "solution found on forward search at depth: " << i << endl;
			cout << direction.size() << " states stored" << endl;
			while( goalID != startID ){
			   path.insert(path.begin(),(lastMove[goalID]));
			   goalID = predecessor[goalID];
			}
			return path;
		 }
		 if( interDir == 2 ){
			cout << "Solution found at intersection during forward search" << endl;

			while( interID != startID ){
			   path.insert(path.begin(), lastMove[interID]);
			   interID = predecessor[interID];
			}
			path.push_back(inverse(interMove));
			while( predInterID != goalID ){
			   path.push_back(inverse(lastMove[predInterID]));
			   predInterID = predecessor[predInterID];
			} 
			cout << path.size() << endl; 
			return path; 
		 }
	  } 

	  // perform DLS backward. If true is returned and path is not zero, 
	  // we rebuild from startID->goalID
	  if( DLS(target, startID, i, lastMove,
			   predecessor, phase, direction, interID,interMove,
			   interDir, predInterID, 2) == true){
		 // if interDir is -1, we found solution on search
		 if( interDir == -1 ){
			cout << endl;
			cout << "Solution found on backward search at depth: " << i << endl;
			cout << direction.size() << " states stored" << endl;
			while( startID != goalID ){
			   path.push_back(inverse(lastMove[startID]));
			   startID = (predecessor)[startID];
			} 
			cout << "Solution found at depth: " << i << endl;
			return path;
		 }
		 if( interDir == 1 ){
			cout << "Solution found at intersection during backward search" << endl;
			// rebuild path
			while( interID != goalID ){
			   path.push_back(inverse(lastMove[interID]));
			   interID = predecessor[interID];
			}

			path.insert(path.begin(), interMove);

			while( predInterID != startID ){
			   path.insert(path.begin(), lastMove[predInterID]);
			   predInterID = predecessor[predInterID];
			}
			return path;
		 } 
	  } 
	  else{
		 cout << "States visited: " << statesVisited << endl;
		 cout << direction.size() << endl;
		 statesVisited = 0;
		 statesTotal += statesVisited;
	  }
   }
   return path;
}

// function to calculate all paths and solutions for a phase
map<vc,vi> generate_database( int phase ){

   // completed cube to iterate through all moves
   vc cube = {UF,UR,UB,UL,FR,FL,BR,BL,DF,DR,DB,DL,
	  UFR,UBR,UBL,UFL,DFR,DBR,DBL,DFL};
   vc solvedID = id(cube,phase);
   vi path;

   // solutions map for <ID, solution path> pairs
   map<vc,vi> solutions;
   solutions[solvedID] = path;

   // queue to iterate through all ID's
   queue<vc> states;
   states.push(cube);

   vi moveSet = applicableMoves[phase];
   while( !(states.empty()) ){

	  vc oldState = states.front();
	  states.pop();
	  vc oldID = id(oldState, phase);

	  for( int i = 0; i < moveSet.size(); i++ ){

		 int move = moveSet[i];

		 vc newState = applyMove( move, oldState );
		 vc newID = id( newState, phase );

		 if( (solutions[newID].size() == 0) && (newID != solvedID) ){
			vi newPath(solutions[oldID]);
			newPath.insert(newPath.begin(), inverse(move));
			solutions[newID] = newPath;
			states.push(newState);
		 }
	  }
   }
   return solutions;
}


int main(int argc, char** argv){

   vc cube = 
   {UF,UR,UB,UL,FR,FL,BR,BL,DF,DR,DB,DL,UFR,UBR,UBL,UFL,DFR,DBR,DBL,DFL};

   vc goalCube = 
   {UF,UR,UB,UL,FR,FL,BR,BL,DF,DR,DB,DL,UFR,UBR,UBL,UFL,DFR,DBR,DBL,DFL};

   // map<vc,vi> phase_1_database = generate_database(1);

   //cout << phase_1_database.size() << endl;

   //map<vc,vi> phase_2_database = generate_database(2);

   //cout << phase_2_database.size() << endl;

   // map<vc,vi> phase_3_database = generate_database(3);

   //cout << phase_3_database.size() << endl;

   //   map<vc,vi> phase_4_database = generate_database(4);

   // cout << phase_4_database.size() << endl;
   /*   map<vc,vi> phase_5_database = generate_database(5);

		cout << phase_5_database.size() << endl;

		map<vc, vi>::iterator it;
		cout << "vc twists = { ";

		for ( it = phase_5_database.begin(); it != phase_5_database.end(); it++ )
		{
		vc first = it->first;
		vi second = it->second;
		cout << "{ ";
		for( int i = 0; i < first.size(); i++){
		if( i != first.size()-1 ){
		cout << int(first[i])-12 << ",";
		}
		else{
		cout << int(first[i])-12;
		}
		}

		cout << " }, ";
		}
		cout << " };" << endl;

		cout << endl;
		vc cornerSet = {UFR,UBR,UBL,UFL,DFR,DBR,DBL,DFL};

		cout << int(cornerSet[1]) << endl; */
   //vi scramble_path = scramble(25,cube);
   cube = applyMove(8,cube);
   cube = applyMove(1, cube);
   cube = applyMove(13, cube);
   cube = applyMove(7,cube);
   vc phase5 = id(cube, 5);

   for( int i = 0; i < phase5.size(); i++ ){
	  cout << int(phase5[i]) << " ";
   }
   cout << endl;


   /*   for( int i = 0; i < cube.size(); i++){
		if( i < 12 ){ cout << int(cube[i])/2 << " ";}
		else{ cout << int(cube[i])/4 << " ";}
		}cout << endl; 



		vi scramble_path = scramble(25, cube);
		string sp;
		build_path(scramble_path, sp);

		int phase = 1;
		int depth = 10;

		vi totPath;

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

   totPath.insert(totPath.end(),path.begin(), path.end());

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

   totPath.insert(totPath.end(),path.begin(), path.end());

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

   build_path(path, build);

   cout << build << endl;  

   totPath.insert(totPath.end(),path.begin(), path.end());

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
   build_path(path, build);

   totPath.insert(totPath.end(),path.begin(), path.end());

   cout << build << endl;  
   cout << endl;
   cout << endl;

   for( int i = 0; i < cube.size(); i++){
	  if( i < 12 ){
		 cout << int(cube[i]) << " ";
	  }
	  else{ 
		 cout << int(cube[i]) << " ";
	  }
   } cout << endl;

   cout << "Scramble path: " << endl;
   cout << sp << endl;
   cout << endl;
   cout << "Solution path: "<< endl;
   cout << build << endl;
   cout << "Solution path length: " << totPath.size() << endl;

   */
	  // Scramble the cube
	  /////////////////

	  /*   vi scramble_path = scramble(30, cube);
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
