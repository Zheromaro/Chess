#include <SDL2/SDL.h>
#include "LoopLogic/image.h"
#include "LoopLogic/audio.h"
#include "LoopLogic/State.h"
#include "GameLogic/GameData.h"
#include "GameLogic/Board.h"
#include "GameLogic/MoveAction.h"
#include "GameLogic/RenderingBoard.h"

//void Move(GameData *data, int row, int colum);

GameData TowPlayerData = {
    .GameState = STILL_PLAYING
};
Board towPlayerBoard;

void TwoPlayerEnter(void *contextTable[]) 
{
    init_board(&towPlayerBoard);
    ResetGame(&TowPlayerData);
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
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        int row = event.button.y / CELL_HEIGHT; // invert y
        int col = event.button.x / CELL_WIDTH;

        click_on_square(&towPlayerBoard, WHITE_PLAYER, SQUARE(row, col));
        break;
    }
}
void TwoPlayerRender(SDL_Renderer *renderer)
{
    RenderingBoard(renderer, towPlayerBoard);
}

State twoPlayerState = {
    .enter = TwoPlayerEnter,
    .processInput = TwoPlayerProcessInput,
    .render = TwoPlayerRender
};

//void Move(GameData *data, int row, int colum)
//{
//    bool succeed = ClickOnPiece(data, row, colum);
//    if (succeed)
//    {
//        SwitchPlayer(&(data->currentPlayer));
//        if (GameIsOver(data))
//            pushState(&gameOverState, "G", data->GameState);
//    }
//}