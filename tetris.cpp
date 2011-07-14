/*
 * tetris.cpp
 *
 *  Created on: 05/07/2011
 *      Author: Marin
 */

#include "SDL/SDL.h"
#include "Color.h"
#include "Defines.h"
#include "Piece.h"
#include "GameControl.h"
#include "SurfaceControl.h"
#include <time.h>


/**
 * Color list
 */
const Uint32 BLUE = COLOR(0, 0, 255);
const Uint32 CYAN = COLOR(0, 255, 255);
const Uint32 GREEN = COLOR(0, 255, 0);
const Uint32 MAGENTA = COLOR(255, 0, 255);
const Uint32 ORANGE = COLOR(255, 165, 0);
const Uint32 RED = COLOR(255, 0, 0);
const Uint32 YELLOW = COLOR(255, 255, 0);
const Uint32 BLACK = COLOR(0, 0, 0);
const Color colors[8] = {BLACK, BLUE, CYAN, GREEN, MAGENTA, ORANGE, RED, YELLOW};

SDL_Surface* SurfaceControl::screen = NULL;
GameControl* GameControl::instance = NULL;
Piece* Piece::instance = NULL;

SDL_Surface* SurfaceControl::GetScreen(){
	if(screen == NULL)
		screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
	return screen;
}

/**
 * Color class method definitions
 */
Uint32 Color::Darker(Uint32 color){
	Uint8 R, G, B;
	SDL_GetRGB(color, GET_SCREEN()->format, &R, &G, &B);
	return COLOR(MAX(R - 50, 0), MAX(G - 50, 0), MAX(B - 50, 0));
}

Uint32 Color::Brighter(Uint32 color){
	Uint8 R, G, B;
	SDL_GetRGB(color, GET_SCREEN()->format, &R, &G, &B);
	return COLOR(MIN(R + 50, 255), MIN(G + 50, 255), MIN(B + 50, 255));
}

/**
 * Block class method definitions
 */
Block::Block(int r = 0, int c = 0, int t = 0): col(c), row(r), type(t) {
	moving = false;
}

void Block::Draw(int pType = -1){
	SDL_Rect rect = {col * BLOCK_SIZE + TOP_LEFT_X, row * BLOCK_SIZE + TOP_LEFT_Y, BLOCK_SIZE, BLOCK_SIZE};
	if(pType == -1){
		SDL_FillRect(GET_SCREEN(), &rect, BLACK);
		rect.h = rect.w = BLOCK_SIZE - 2;
		rect.x += 1;
		rect.y += 1;
		SDL_FillRect(GET_SCREEN(), &rect, colors[type].c);
	} else {
		SDL_FillRect(GET_SCREEN(), &rect, colors[pType].c);
		rect.h = rect.w = BLOCK_SIZE - 2;
		rect.x += 1;
		rect.y += 1;
		SDL_FillRect(GET_SCREEN(), &rect, BLACK);
	}

}

void Block::Clear(){
	type = 0;
	moving = false;
}

bool Block::CanMove(int drow, int dcol){
	if(type == 0 || (!drow && !dcol)) return false;
	int dr = row + drow, dc = col + dcol;
	if(dr < 0 || dr >= BLOCK_ROWS || dc < 0 || dc >= BLOCK_COLS) return false;
	Block* dest = GameControl::GetInstance()->GetBlockAt(dr, dc);
	return ((dest->type == 0) || dest->moving);
}
Block* Block::Move(int drow, int dcol){
	if(!CanMove(drow, dcol)) return this;
	Block* src = this;
	Block* dest = GameControl::GetInstance()->GetBlockAt(row + drow, col + dcol);
	dest->type = src->type;
	dest->moving = src->moving;
	src->Clear();
	return dest;
}

/**
 * Piece method definitions
 */
Piece::Piece(int r, int c, int t, int rotation):
	row(r), col(c), type(t+1), rot(rotation){
	FOR(i, BLOCKS_PER_PIECE){
		int drow = PiecePositions[t][rotation][i][0];
		int dcol = PiecePositions[t][rotation][i][1];
		parts[i] = GameControl::GetInstance()->GetBlockAt(row + drow, col + dcol);
		parts[i]->SetMoving(true);
		parts[i]->SetType(type);
		projection[i] = NULL;
	}
}
Piece* Piece::CreatePiece(){
	srand(rand());
	int nType = rand() % 7;
	int nRot = rand() % 4;
	instance = new Piece(0, 4, nType, nRot);
	instance->UpdateProjection();
	return instance;
}
bool Piece::CanMove(int drow, int dcol){
	bool canMove = true;
	FOR(i, 4) canMove &= parts[i]->CanMove(drow, dcol);
	return canMove;
}
bool Piece::CanRotate(){
	int nRot = (rot + 1) % 4, t = type - 1;
	bool can = true;
	FOR(i, BLOCKS_PER_PIECE){
		int nrow = row + PiecePositions[t][nRot][i][0];
		int ncol = col + PiecePositions[t][nRot][i][1];
		Block* b = GameControl::GetInstance()->GetBlockAt(nrow, ncol);
		can &= (b->GetType() == 0 || b->IsMoving());
	}
	return can;
}
void Piece::Rotate(){
	if(!CanRotate()) return;
	int nRot = (rot + 1) % ROTATIONS, t = type - 1;
	FOR(i, BLOCKS_PER_PIECE) parts[i]->Clear();
	FOR(i, BLOCKS_PER_PIECE){
		int nrow = row + PiecePositions[t][nRot][i][0];
		int ncol = col + PiecePositions[t][nRot][i][1];
		parts[i] = GameControl::GetInstance()->GetBlockAt(nrow, ncol);
		parts[i]->SetType(type);
		parts[i]->SetMoving(true);
	}
	rot = nRot;
	UpdateProjection();
}
void Piece::Fix(){
	FOR(i, BLOCKS_PER_PIECE){
		parts[i]->SetMoving(false);
		if(projection[i] != NULL) projection[i]->Draw();
	}
}
bool Piece::Move(int drow, int dcol){
	if(!CanMove(drow, dcol)) return false;
	if(dcol < 0)
		FOR(i, 4) parts[i] = parts[i]->Move(drow, dcol);
	else
		FORD(i, 4) parts[i] = parts[i]->Move(drow, dcol);
	row += drow;
	col = MIN(MAX(col + dcol, 0), BLOCK_COLS - 5);
	if(dcol){
		UpdateProjection();
	}
	return true;
}
void Piece::UpdateProjection(){
	int down = 1;
	bool project = true;
	while(CanMove(down, 0)) ++down;
	if(--down){
		FOR(i, BLOCKS_PER_PIECE){
			if(projection[i] != NULL) projection[i]->Draw();
			projection[i] = GameControl::GetInstance()->GetBlockAt(parts[i]->GetRow() + down, parts[i]->GetCol());
			project &= !projection[i]->IsMoving();
		}
		if(project) FOR(i, BLOCKS_PER_PIECE) projection[i]->Draw(type);
	}
}

/**
 * GameControl method definitions
 */
Block* GameControl::GetBlockAt(int row, int col){
	return gameMatrix[row][col];
}
void GameControl::Init(){
	FOR(row, BLOCK_ROWS) FOR(col, BLOCK_COLS){
		gameMatrix[row][col] = new Block(row, col, 0);
		oldTypes[row][col] = 0;
	}
}
GameControl* GameControl::GetInstance(){
	if(instance == NULL) instance = new GameControl();
	return instance;
}
void GameControl::ClearGameMatrix(){
	FOR(row, BLOCK_ROWS) FOR(col, BLOCK_COLS){
		gameMatrix[row][col]->SetType(0);
		gameMatrix[row][col]->SetMoving(false);
	}
}
void GameControl::MoveAllAboveDown(int r){
	FOR(col, BLOCK_COLS) gameMatrix[r][col]->Clear(), oldTypes[r][col] = -1;
	FORD(row, r) FOR(col, BLOCK_COLS){
		gameMatrix[row][col]->Move(1, 0);
		oldTypes[row][col] = -1;
	}
}
bool GameControl::CheckRow(int r){
	int count = 0;
	FOR(col, BLOCK_COLS) count += gameMatrix[r][col]->GetType() > 0;
	if(count == BLOCK_COLS) MoveAllAboveDown(r);
	return count == BLOCK_COLS;
}
void GameControl::GameLoop(){
	SDL_Event event;
	SDL_Surface* screen = GET_SCREEN();
	SDL_Rect border = {TOP_LEFT_X-1, TOP_LEFT_Y-1, BLOCK_COLS * BLOCK_SIZE + 2, BLOCK_ROWS * BLOCK_SIZE + 2};
	SDL_FillRect(screen, &border, COLOR(210, 250, 210));
	border.x += 1; border.y += 1;
	border.h -= 2; border.w -= 2;
	SDL_FillRect(screen, &border, BLACK);
	Piece* currentPiece = Piece::CreatePiece();


	int downCounter = 0, isKeyDown;
	for(bool playing = true; playing; downCounter = (downCounter + 1) % 5){
		while(SDL_PollEvent(&event)){
			// Non-keyboard events are ignored
			if(event.type != SDL_KEYUP && event.type != SDL_KEYDOWN) continue;
			isKeyDown = event.type == SDL_KEYDOWN;
			switch(event.key.keysym.sym){
				case SDLK_F2:
					if(isKeyDown) StartGame(); break;
				case SDLK_LEFT:
					dcol = -isKeyDown; break;
				case SDLK_RIGHT:
					dcol = isKeyDown; break;
				case SDLK_UP:
					if(isKeyDown && !paused) currentPiece->Rotate(); break;
				case SDLK_p:
				case SDLK_PAUSE:
					if(isKeyDown) TogglePause(); break;
				case SDLK_ESCAPE:
					playing = false; break;
				case SDLK_SPACE:
					if(isKeyDown && !paused){
						while(currentPiece->Move(1,0));
						currentPiece->Fix();
						FORD(row, BLOCK_ROWS) row += CheckRow(row);
						currentPiece = Piece::CreatePiece();
					}
					break;
				default: break;
			}
		}
		if(!paused){ // Moves only happen if not paused
			// Left-Right movement
			if(dcol) currentPiece->Move(0, dcol);
			// Falling (gravity)
			if(!downCounter && !currentPiece->Move(1, 0)){
				currentPiece->Fix();
				FORD(row, BLOCK_ROWS) row += CheckRow(row);
				currentPiece = Piece::CreatePiece();
			}
			/* Redraw procedure */
			FOR(row, BLOCK_ROWS){
				FOR(col, BLOCK_COLS){
					Block* b = gameMatrix[row][col];
					if(b->GetType() != oldTypes[row][col]) b->Draw();
					oldTypes[row][col] = b->GetType();
				}
			}
			/* Update screen */
			SDL_Flip(screen);
		}
		SDL_Delay(100);
	}
}
void GameControl::StartGame(){
	ClearGameMatrix();
	paused = false;
	score = 0;
}

int main( int argc, char* args[] ) {
	srand( time(NULL) );
	SDL_Init( SDL_INIT_EVERYTHING );
	GameControl* gc = GameControl::GetInstance();
	gc->GameLoop();
	SDL_Quit();
	return 0;
}
