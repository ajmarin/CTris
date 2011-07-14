/**
 * Configurations
 */
#ifndef CONFIGURATIONS
#define CONFIGURATIONS
const int BLOCK_COLS = 10;
const int BLOCK_ROWS = 20;
const int BLOCK_SIZE = 20;
const int BLOCKS_PER_PIECE = 4;
const int COORDS_PER_BLOCK = 2;
const int NUMBER_OF_PIECES = 7;
const int ROTATIONS = 4;
const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 400;
const int TOP_LEFT_X = 15;
const int TOP_LEFT_Y = 15;

const int PiecePositions[NUMBER_OF_PIECES][ROTATIONS][BLOCKS_PER_PIECE][COORDS_PER_BLOCK] =
{
	{ // I piece
		{{0,2},{1,2},{2,2},{3,2}},
		{{2,1},{2,2},{2,3},{2,4}},
		{{0,2},{1,2},{2,2},{3,2}},
		{{2,1},{2,2},{2,3},{2,4}}
	},
	{ // L piece
		{{0,2},{1,2},{2,2},{2,3}},
		{{2,2},{2,3},{2,4},{3,2}},
		{{2,1},{2,2},{3,2},{4,2}},
		{{1,2},{2,0},{2,1},{2,2}}
	},
	{ // O piece
		{{1,1},{1,2},{2,1},{2,2}},
		{{1,1},{1,2},{2,1},{2,2}},
		{{1,1},{1,2},{2,1},{2,2}},
		{{1,1},{1,2},{2,1},{2,2}}
	},
	{ // R piece
		{{0,2},{1,2},{2,1},{2,2}},
		{{1,2},{2,2},{2,3},{2,4}},
		{{2,2},{2,3},{3,2},{4,2}},
		{{2,0},{2,1},{2,2},{3,2}}
	},
	{ // S piece
		{{1,2},{1,3},{2,1},{2,2}},
		{{1,2},{2,2},{2,3},{3,3}},
		{{1,2},{1,3},{2,1},{2,2}},
		{{1,2},{2,2},{2,3},{3,3}}
	},
	{ // T piece
		{{1,2},{2,1},{2,2},{2,3}},
		{{1,2},{2,2},{2,3},{3,2}},
		{{2,1},{2,2},{2,3},{3,2}},
		{{1,2},{2,1},{2,2},{3,2}}
	},
	{ // Z piece
		{{1,1},{1,2},{2,2},{2,3}},
		{{1,3},{2,2},{2,3},{3,2}},
		{{1,1},{1,2},{2,2},{2,3}},
		{{1,3},{2,2},{2,3},{3,2}}
	}
};
#endif
