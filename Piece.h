/**
 * Piece
 */
#include "Block.h"
#include "Config.h"
#include "Defines.h"
#ifndef PIECE_CLASS
#define PIECE_CLASS
class Piece {
	Block* parts[4];
	Block* projection[4];
	int row, col, type, rot;
	static Piece* instance;
	Piece(int r, int c, int type, int rotation);
	void UpdateProjection();
public:
	static Piece* CreatePiece();
	bool CanMove(int drow, int dcol);
	bool CanRotate();
	void Fix();
	bool Move(int drow, int dcol);
	void Rotate();
};
#endif
