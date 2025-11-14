#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream> 
#include <string>  
#include "globals.hpp"
#include "algorithms.hpp"

// Constants for screen and grid dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_ROWS = 20;
const int GRID_COLS = 20;
const int CELL_SIZE = SCREEN_WIDTH / GRID_COLS;
const int SIDEBAR_WIDTH = 120; 

// Global grid representing the maze
std::vector<std::vector<CellState>> grid(GRID_ROWS, std::vector<CellState>(GRID_COLS, EMPTY));

// Start and end point coordinates
int startRow = -1, startCol = -1;
int endRow = -1, endCol = -1;

// Directions for movement (Up, Down, Left, Right)
const int dRow[] = {-1, 1, 0, 0};
const int dCol[] = {0, 0, -1, 1};

// Function to check if a cell is valid and not an obstacle
bool isValid(int row, int col) {
    return row >= 0 && row < GRID_ROWS && col >= 0 && col < GRID_COLS && grid[row][col] != OBSTACLE;
}

// Function to draw the grid
void drawGrid(SDL_Renderer* renderer) {
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLS; ++j) {
            SDL_Rect cell = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            switch (grid[i][j]) {
                case EMPTY:    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break; // White
                case OBSTACLE: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;     // Black
                case VISITED:  SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255); break; // Light Blue
                case PATH:     SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;     // Blue
                case START:    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;     // Green
                case END:      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;     // Red
            }
            SDL_RenderFillRect(renderer, &cell);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
}

// Function to render and animate the grid update
void animateRender(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderClear(renderer);
    drawGrid(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(25); // Small delay for visualization
}

// Function to draw text on the renderer
void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color color = {0, 0, 0}; // Black color
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Error creating text surface: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Error creating text texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Function to draw the buttons
void drawButtons(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 0, 255); // Yellowish color
    SDL_RenderFillRect(renderer, &btnBFS);
    SDL_RenderFillRect(renderer, &btnDFS);
    SDL_RenderFillRect(renderer, &btnReset);

    drawText(renderer, font, "BFS", btnBFS.x + 35, btnBFS.y + 5);
    drawText(renderer, font, "DFS", btnDFS.x + 35, btnDFS.y + 5);
    drawText(renderer, font, "Reset", btnReset.x + 30, btnReset.y + 5);
}

// Function to reconstruct and draw the path
void drawPath(SDL_Renderer* renderer, const std::vector<std::vector<std::pair<int, int>>>& parent) {
    int row = endRow, col = endCol;
    // Trace back from end to start using parent pointers
    while (parent[row][col] != std::make_pair(startRow, startCol)) {
        std::tie(row, col) = parent[row][col];
        // Mark visited cells on the path as PATH
        if (grid[row][col] == VISITED) {
            grid[row][col] = PATH;
        }
        animateRender(renderer); // Animate path drawing
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL and TTF
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Error in SDL Initialization: " << SDL_GetError() << std::endl;
        return 1;
    }
    if(TTF_Init() < 0) {
        std::cerr << "Error in TTF Initialization: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Pathfinding Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH + SIDEBAR_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 16);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    // Main application loop
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point mousePoint = {e.button.x, e.button.y};
                // Check for button clicks
                if (SDL_PointInRect(&mousePoint, &btnBFS)) {
                    for(int i=0; i<GRID_ROWS; ++i) {
                        for(int j=0; j<GRID_COLS; ++j) {
                            if(grid[i][j] == VISITED || grid[i][j] == PATH) {
                                grid[i][j] = EMPTY;
                            }
                        }
                    }
                    if(startRow != -1 && startCol != -1) grid[startRow][startCol] = START;
                    if(endRow != -1 && endCol != -1) grid[endRow][endCol] = END;

                    bfs(renderer);
                } else if (SDL_PointInRect(&mousePoint, &btnDFS)) {
                     // Reset grid state before running algorithm
                     for(int i=0; i<GRID_ROWS; ++i) {
                        for(int j=0; j<GRID_COLS; ++j) {
                            if(grid[i][j] == VISITED || grid[i][j] == PATH) {
                                grid[i][j] = EMPTY;
                            }
                        }
                    }
                    if(startRow != -1 && startCol != -1) grid[startRow][startCol] = START;
                    if(endRow != -1 && endCol != -1) grid[endRow][endCol] = END;

                    dfs(renderer);
                } else if (SDL_PointInRect(&mousePoint, &btnReset)){
                    // Reset grid and start/end points
                    grid = std::vector<std::vector<CellState>>(GRID_ROWS, std::vector<CellState>(GRID_COLS, EMPTY));
                    startRow = -1;
                    startCol = -1;
                    endRow = -1;
                    endCol = -1;
                } else {
                    // Handle grid cell clicks
                    int row = e.button.y / CELL_SIZE;
                    int col = e.button.x / CELL_SIZE;
                    if (row < GRID_ROWS && col < GRID_COLS){
                        // Toggle obstacle with left click, remove with right click
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            grid[row][col] = OBSTACLE;
                        } else if (e.button.button == SDL_BUTTON_RIGHT) {
                             grid[row][col] = EMPTY;
                        }
                        // If removing obstacle from start/end, reset their positions
                        if(row == startRow && col == startCol){
                            startRow = -1;
                            startCol = -1;
                        }
                        if(row == endRow && col == endCol){
                            endRow = -1;
                            endCol = -1;
                        }
                    }
                }
            } else if (e.type == SDL_KEYDOWN){
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                int row = mouseY / CELL_SIZE;
                int col = mouseX / CELL_SIZE;

                if (row < GRID_ROWS && col < GRID_COLS) { // Ensure key press is within grid area
                    if(e.key.keysym.sym == SDLK_s){
                        // Set start point
                        if(startRow != -1 && startCol != -1){
                            grid[startRow][startCol] = EMPTY;
                        }
                        if(row == endRow && col == endCol){
                            endRow = -1;
                            endCol = -1;
                        }
                        startRow = row;
                        startCol = col;
                        grid[row][col] = START;
                    } else if(e.key.keysym.sym == SDLK_e){
                        // Set end point
                        if(endRow != -1 && endCol != -1){
                             // Clear previous end point if it wasn't an obstacle
                            if(grid[endRow][endCol] != OBSTACLE) grid[endRow][endCol] = EMPTY;
                        }
                        if(row == startRow && col == startCol){
                            startRow = -1;
                            startCol = -1;
                        }
                        endRow = row;
                        endCol = col;
                        grid[row][col] = END; 
                    }
                }
            }
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);

        drawGrid(renderer);
        drawButtons(renderer, font);

        SDL_RenderPresent(renderer);
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}