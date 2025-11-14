#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int GRID_ROWS;
extern const int GRID_COLS;
extern const int CELL_SIZE;

enum CellState { EMPTY, OBSTACLE, VISITED, PATH, START, END };
extern std::vector<std::vector<CellState>> grid;

const SDL_Rect btnBFS = {SCREEN_WIDTH + 10, 20, 100, 30};
const SDL_Rect btnDFS = {SCREEN_WIDTH + 10, 60, 100, 30};
const SDL_Rect btnReset = {SCREEN_WIDTH + 10, 100, 100, 30};

extern int startRow, startCol;
extern int endRow, endCol;

extern const int dRow[4];
extern const int dCol[4];

// Forward declarations for shared functions
bool isValid(int row, int col);
void drawGrid(SDL_Renderer* renderer);
void animateRender(SDL_Renderer* renderer);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);
void drawButtons(SDL_Renderer* renderer, TTF_Font* font);
void drawPath(SDL_Renderer* renderer, const std::vector<std::vector<std::pair<int, int>>>& parent);

