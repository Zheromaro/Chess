#include <stdbool.h>
#include <SDL2/SDL.h>
#include "LoopLogic/image.h"
#include "LoopLogic/audio.h"
#include "LoopLogic/State.h"
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
    .currentPlayer = WHITE_PLAYER
};
Players cpuIs = WHITE_PLAYER;

void VScpuEnter(void *contextTable[]) 
{
    GetXOImage();
    ResetGame(&VScpuData);
    if (contextTable[0])
        cpuIs = *((Players*)contextTable[0]);

    if (cpuIs == WHITE_PLAYER)
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
        
    bool succeed = ClickOnPiece(data, row, colum);
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
    ClickOnPiece(data, row, colum); 
    SwitchPlayer(&(data->currentPlayer));
    
    if (GameIsOver(data))
        pushState(&gameOverState, "G P",  data->GameState, cpuIs);
}