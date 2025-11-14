#include <SDL.h>
#include <iostream> 
#include <queue>   
#include <stack>   
#include <vector>  
#include <utility> 
#include "globals.hpp"

// Breadth-First Search algorithm
void bfs(SDL_Renderer* renderer) {
    if (startRow == -1 || endRow == -1) {
        std::cerr << "Start or end point not set." << std::endl;
        return;
    }

    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> visited(GRID_ROWS, std::vector<bool>(GRID_COLS, false));
    std::vector<std::vector<std::pair<int, int>>> parent(GRID_ROWS, std::vector<std::pair<int, int>>(GRID_COLS, {-1, -1}));

    q.push({startRow, startCol});
    visited[startRow][startCol] = true;

    bool found = false;
    while (!q.empty() && !found) {
        auto [row, col] = q.front();
        q.pop();

        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            int newRow = row + dRow[i];
            int newCol = col + dCol[i];

            if (isValid(newRow, newCol) && !visited[newRow][newCol]) {
                visited[newRow][newCol] = true;
                parent[newRow][newCol] = {row, col};
                q.push({newRow, newCol});

                // Mark visited cells (excluding start/end)
                if (grid[newRow][newCol] == EMPTY) {
                    grid[newRow][newCol] = VISITED;
                }
                animateRender(renderer);

                // Check if the end is reached
                if (newRow == endRow && newCol == endCol) {
                    found = true;
                    break;
                }
            }
        }
    }

    // Draw the path if found
    if (found) {
        drawPath(renderer, parent);
    }
}

// Depth-First Search algorithm
void dfs(SDL_Renderer* renderer) {
    if (startRow == -1 || endRow == -1) {
        std::cerr << "Start or end point not set." << std::endl;
        return;
    }

    std::stack<std::pair<int, int>> stk;
    std::vector<std::vector<bool>> visited(GRID_ROWS, std::vector<bool>(GRID_COLS, false));
    std::vector<std::vector<std::pair<int, int>>> parent(GRID_ROWS, std::vector<std::pair<int, int>>(GRID_COLS, {-1, -1}));

    stk.push({startRow, startCol});
    visited[startRow][startCol] = true;
    bool found = false;

    while (!stk.empty() && !found) {
        auto [row, col] = stk.top();
        stk.pop();

        // Check if the end is reached (DFS explores deeper first)
        if (row == endRow && col == endCol) {
             found = true;
             // Need to reconstruct path here as well, similar to BFS
             // The current parent tracking works for reconstruction after finding the end
        }

        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            int newRow = row + dRow[i];
            int newCol = col + dCol[i];

            if (isValid(newRow, newCol) && !visited[newRow][newCol]) {
                visited[newRow][newCol] = true;
                parent[newRow][newCol] = {row, col};
                stk.push({newRow, newCol});

                // Mark visited cells (excluding start/end)
                 if (grid[newRow][newCol] == EMPTY) {
                    grid[newRow][newCol] = VISITED;
                }
                animateRender(renderer);

                // If found during neighbor exploration, break inner loop
                if (newRow == endRow && newCol == endCol){
                    found = true;
                    break;
                }
            }
        }
    }

    // Draw the path if found
    if (found) {
        drawPath(renderer, parent);
    }
}
