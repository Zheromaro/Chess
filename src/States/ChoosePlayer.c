#include <SDL2/SDL.h>
#include "image.h"
#include "audio.h"
#include "text.h"
#include "appSettings.h"
#include "GameLogic/GameData.h"
#include "State.h"

SDL_Rect WindowRect3   = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Texture* choosePlayerTexture = NULL;

char choosePlayerImageFile[] = "assets/img/ChoosePlayer.png";

void ChoosePlayerEnter(void* contextTable[])
{
    choosePlayerTexture = LoadTexture(choosePlayerImageFile);
}
void ChoosePlayerProcessInput(SDL_Event event)
{  
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_KP_0:
            popState();
            break;
        case SDLK_KP_1:
            pushState(&VScpuState, "P", PLAYER_O);
            break;
        case SDLK_KP_2:
            pushState(&VScpuState, "P", PLAYER_X);
            break;
        }
        break;
    }
}
void ChoosePlayerRender(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, choosePlayerTexture, NULL, &WindowRect3);
}

State choosePlayerState = {
    .enter = ChoosePlayerEnter,
    .processInput = ChoosePlayerProcessInput,
    .render = ChoosePlayerRender
};