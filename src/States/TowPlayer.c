#include <SDL2/SDL.h>
#include "image.h"
#include "audio.h"
#include "State.h"
#include "GameLogic/GameData.h"
#include "GameLogic/Logic.h"
#include "GameLogic/RenderingBoard.h"

void Move(GameData *data, int row, int colum);

GameData TowPlayerData = {
    .GameState = PLAYER_O_WON,
    .board = {EMPTY_CELL},
    .currentPlayer = PLAYER_X
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

        case SDLK_KP_1:
            Move(&TowPlayerData, 0, 2);
            break;
        case SDLK_KP_2:
            Move(&TowPlayerData, 1, 2);
            break; 
        case SDLK_KP_3:
            Move(&TowPlayerData, 2, 2);
            break; 
        case SDLK_KP_4:
            Move(&TowPlayerData, 0, 1);
            break; 
        case SDLK_KP_5:
            Move(&TowPlayerData, 1, 1);
            break; 
        case SDLK_KP_6:
            Move(&TowPlayerData, 2, 1);
            break; 
        case SDLK_KP_7:
            Move(&TowPlayerData, 0, 0);
            break;
        case SDLK_KP_8:
            Move(&TowPlayerData, 1, 0);
            break;
        case SDLK_KP_9:
            Move(&TowPlayerData, 2, 0);
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        Move(&TowPlayerData,
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
    bool succeed = ClickOnCell(data, row, colum);
    if (succeed)
    {
        SwitchPlayer(&(data->currentPlayer));
        if (GameIsOver(data))
            pushState(&gameOverState, "G", data->GameState);
    }
}