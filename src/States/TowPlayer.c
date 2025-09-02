#include <SDL2/SDL.h>
#include "LoopLogic/State.h"
#include "GameLogic/Board.h"
#include "GameLogic/MoveAction.h"
#include "GameLogic/BoardRendering.h"
#include "GameConstant.h"

Board towPlayerBoard;
Players currentPlayer = WHITE_PLAYER;

void TwoPlayerEnter(void *contextTable[]) 
{
    init_board(&towPlayerBoard);
    GetXOImage();
}
void TwoPlayerProcessInput(SDL_Event event)
{  
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_SPACE:
            popState();
            break;
        case SDLK_f:
            towPlayerBoard.fliped = !towPlayerBoard.fliped;
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        int col = event.button.x / CELL_WIDTH;
        int row = (towPlayerBoard.fliped) 
                  ? (8-(event.button.y / CELL_HEIGHT))
                  : (event.button.y / CELL_HEIGHT);
        
        click_on_square(&towPlayerBoard, &currentPlayer, SQUARE(row, col));
        break;
    }
}
void TwoPlayerRender(SDL_Renderer *renderer)
{
    BoardRendering(renderer, towPlayerBoard);
}

State twoPlayerState = {
    .enter = TwoPlayerEnter,
    .processInput = TwoPlayerProcessInput,
    .render = TwoPlayerRender
};
