#include <SDL2/SDL.h>
#include "LoopLogic/image.h"
#include "LoopLogic/audio.h"
#include "LoopLogic/State.h"
#include "GameLogic/Board.h"
#include "GameLogic/MoveAction.h"
#include "GameLogic/BoardRendering.h"

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
        if (event.key.keysym.sym == SDLK_SPACE)
            popState();
        break;
    case SDL_MOUSEBUTTONDOWN:
        int row = event.button.y / CELL_HEIGHT; // invert y
        int col = event.button.x / CELL_WIDTH;
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
