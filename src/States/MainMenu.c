#include <SDL2/SDL.h>
#include "LoopLogic/image.h"
#include "LoopLogic/audio.h"
#include "LoopLogic/text.h"
#include "LoopLogic/appSettings.h"
#include "GameLogic/GameData.h"
#include "LoopLogic/State.h"

SDL_Rect WindowRect4 = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Texture* mainMenu = NULL;

char mainMenuImageFile[] = "assets/img/MainMenu.png";

void MainMenuEnter() 
{
    mainMenu = LoadTexture(mainMenuImageFile);
}
void MainMenuProcessInput(SDL_Event event) 
{  
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_KP_1:
            pushState(&twoPlayerState, "");
            break;
        case SDLK_KP_2:
            printf("Still in working on it\n");
            break;
        case SDLK_KP_3:
            printf("Still in Thinking :{\n");
            break;
        break;
        }
    }
}
void MainMenuRender(SDL_Renderer *renderer) 
{
    SDL_RenderCopy(renderer, mainMenu, NULL, &WindowRect4);
}

State mainMenuState = {
    .enter = MainMenuEnter,
    .processInput = MainMenuProcessInput,
    .render = MainMenuRender
};