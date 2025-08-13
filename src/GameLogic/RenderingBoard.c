#include "image.h"
#include "State.h"
#include "GameLogic/RenderingBoard.h"

void RenderGrid(SDL_Renderer *renderer, const SDL_Color *color);
void RenderCells(SDL_Renderer *renderer, const XOCellStates board[NUM_ROW_COL ][NUM_ROW_COL ]);

const SDL_Color GRID_COLOR = {.r = 255, .g = 255, .b = 255};
SDL_Rect cellRect = {.h = CELL_HEIGHT, .w = CELL_WIDTH, .x = 0, .y = 0};
SDL_Texture* XTexture = NULL;
SDL_Texture* OTexture = NULL;
char XImageFile[] = "assets/img/X.png";
char OImageFile[] = "assets/img/O.png";

void GetXOImage(){
    XTexture = LoadTexture(XImageFile);
    OTexture = LoadTexture(OImageFile);
}

void RenderingBoard(SDL_Renderer *renderer, const GameData *gameData)
{
    RenderGrid(renderer, &GRID_COLOR);
    RenderCells(renderer, gameData->board);
}

// private 
void RenderGrid(SDL_Renderer *renderer, const SDL_Color *color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    for (int i = 0; i < NUM_ROW_COL; i++)
    {
        SDL_RenderDrawLine(renderer,
                           i * CELL_WIDTH, 0,
                           i * CELL_WIDTH, WINDOW_HEIGHT);
        SDL_RenderDrawLine(renderer,
                           0           , i * CELL_HEIGHT,
                           WINDOW_WIDTH, i * CELL_HEIGHT);
    }
    
}
void RenderCells(SDL_Renderer *renderer, const XOCellStates board[NUM_ROW_COL ][NUM_ROW_COL ])
{
    for (int row = 0; row < NUM_ROW_COL; row++)
    {
        for (int colom = 0; colom < NUM_ROW_COL; colom++)
        {
            cellRect.x = row * CELL_WIDTH;
            cellRect.y = colom * CELL_HEIGHT;
            if (board[row][colom] == X_CELL)
            {
                SDL_RenderCopy(renderer, XTexture, NULL, &cellRect);
            }
            else if (board[row][colom] == O_CELL)
            {
                SDL_RenderCopy(renderer, OTexture, NULL, &cellRect);
            }
        }
    }
    
}
