#include <stdbool.h>
#include <SDL2/SDL.h>
#include "image.h"
#include "audio.h"
#include "State.h"
#include "GameLogic/GameData.h"
#include "GameLogic/Logic.h"
#include "GameLogic/RenderingBoard.h"

void PlayerMove(GameData *data, int row, int colum);
void CPUMove(GameData *data);

const float AI_TIMER_END = 0.5f; 
float aiTimer = 0.0f; // Seconds passed since AI turn started
bool aiThinking = false;

GameData VScpuData = {
    .GameState = STILL_PLAYING,
    .board = {EMPTY_CELL},
    .currentPlayer = PLAYER_X
};
XOPlayers cpuIs = PLAYER_X;

void VScpuEnter(void *contextTable[]) 
{
    GetXOImage();
    ResetGame(&VScpuData);
    if (contextTable[0])
        cpuIs = *((XOPlayers*)contextTable[0]);

    if (cpuIs == PLAYER_X)
    {
        CPUMove(&VScpuData);
    }
    
}
void VScpuProcessInput(SDL_Event event)
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
            PlayerMove(&VScpuData, 0, 2);
            break;
        case SDLK_KP_2:
            PlayerMove(&VScpuData, 1, 2);
            break; 
        case SDLK_KP_3:
            PlayerMove(&VScpuData, 2, 2);
            break; 
        case SDLK_KP_4:
            PlayerMove(&VScpuData, 0, 1);
            break; 
        case SDLK_KP_5:
            PlayerMove(&VScpuData, 1, 1);
            break; 
        case SDLK_KP_6:
            PlayerMove(&VScpuData, 2, 1);
            break; 
        case SDLK_KP_7:
            PlayerMove(&VScpuData, 0, 0);
            break;
        case SDLK_KP_8:
            PlayerMove(&VScpuData, 1, 0);
            break;
        case SDLK_KP_9:
            PlayerMove(&VScpuData, 2, 0);
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        PlayerMove(&VScpuData,
                    event.button.x / CELL_WIDTH,
                    event.button.y / CELL_HEIGHT);
        break;
    }
}
void VScpuUpdate(float delta_time)
{
    if (aiThinking) {
        aiTimer += delta_time; // Add frame time to timer

        if (aiTimer >= AI_TIMER_END) { // 2 seconds passed
            CPUMove(&VScpuData);
            aiThinking = false;
        }
    }
}
void VScpuRender(SDL_Renderer *renderer)
{
    RenderingBoard(renderer, &VScpuData);
}

State VScpuState = {
    .enter = VScpuEnter,
    .processInput = VScpuProcessInput,
    .update = VScpuUpdate,
    .render = VScpuRender
};

void PlayerMove(GameData *data, int row, int colum)
{
    if (aiThinking)
        return;
        
    bool succeed = ClickOnCell(data, row, colum);
    if (succeed)
    {
        SwitchPlayer(&(data->currentPlayer));
        if (GameIsOver(data))
            pushState(&gameOverState, "G P", data->GameState, cpuIs);
        
        aiThinking = true;
        aiTimer = 0.0f;
    }
}
void CPUMove(GameData *data)
{
    int row = -1;
    int colum = -1;
    GetAIMove(data, &row, &colum);
    ClickOnCell(data, row, colum); 
    SwitchPlayer(&(data->currentPlayer));
    
    if (GameIsOver(data))
        pushState(&gameOverState, "G P",  data->GameState, cpuIs);
}