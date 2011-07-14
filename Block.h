/**
 * Block
 */
#ifndef BLOCK_CLASS
#define BLOCK_CLASS
class Block {
	int col, row, type;
	bool moving;
public:
	Block(int r, int c, int t);
	bool CanMove(int drow, int dcol);
	void Clear();
	void Draw(int pType);
	bool IsMoving(){ return moving; }
	int GetType(){ return type; }
	int GetRow(){ return row; }
	int GetCol(){ return col; }
	Block* Move(int drow, int dcol);
	void SetCol(int c){ col = c; }
	void SetMoving(bool flag){ moving = flag; }
	void SetRow(int r){ row = r; }
	void SetType(int t){ type = t; }
};
#endif
