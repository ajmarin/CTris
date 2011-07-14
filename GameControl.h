/**
 * GameControl
 */
#include "Block.h"
#include "Config.h"
#ifndef GAMECONTROL_CLASS
#define GAMECONTROL_CLASS
class GameControl {
	static GameControl *instance;
	int dcol, score;
	bool paused;
	Block* gameMatrix[BLOCK_ROWS][BLOCK_COLS];
	int oldTypes[BLOCK_ROWS][BLOCK_COLS];
	GameControl(){ Init(); }
	bool CheckRow(int row);
	void ClearGameMatrix();
	void Init();
	void MoveAllAboveDown(int row);
public:
	void GameLoop();
	Block* GetBlockAt(int row, int col);
	static GameControl* GetInstance();
	void PerformRotation();
	void StartGame();
	void TogglePause(){ paused = !paused; }
};
#endif
