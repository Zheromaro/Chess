#include <SDL2/SDL.h>
#include "image.h"
#include "audio.h"
#include "State.h"
#include "GameLogic/GameData.h"
#include "GameLogic/Logic.h"
#include "GameLogic/RenderingBoard.h"

void Move(GameData *data, int row, int colum);

GameData TowPlayerData = {
    .GameState = STILL_PLAYING,
    .currentPlayer = WHITE_PLAYER
};

void TwoPlayerEnter(void *contextTable[]) 
{
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
        ClickOnPiece(&TowPlayerData,
             event.button.x / CELL_WIDTH,
             event.button.y / CELL_HEIGHT);
        break;
    }
}
void TwoPlayerRender(SDL_Renderer *renderer)
{
    RenderingBoard(renderer, &TowPlayerData);
}

State twoPlayerState = {
    .enter = TwoPlayerEnter,
    .processInput = TwoPlayerProcessInput,
    .render = TwoPlayerRender
};

void Move(GameData *data, int row, int colum)
{
    bool succeed = ClickOnPiece(data, row, colum);
    if (succeed)
    {
        SwitchPlayer(&(data->currentPlayer));
        if (GameIsOver(data))
            pushState(&gameOverState, "G", data->GameState);
    }
}