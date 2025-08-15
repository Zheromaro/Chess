#include <SDL2/SDL.h>
#include "image.h"
#include "audio.h"
#include "text.h"
#include "appSettings.h"
#include "GameLogic/GameData.h"
#include "State.h"

ChessGameStates gameState = DRAW;  
SDL_Rect WindowRect2 = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Texture* XWonMenuTexture = NULL;
SDL_Texture* OWonMenuTexture = NULL;
SDL_Texture* DrawMenuTexture = NULL;

char XWonMenuImageFile[] = "assets/img/XPlayerWin.png";
char OWonMenuImageFile[] = "assets/img/OPlayerWin.png";
char DrawMenuImageFile[] = "assets/img/Draw.png";
char WinMusicFile[] = "assets/sound/01 - Detective Conan Main Theme.mp3";
char LoseMusicFile[] = "assets/sound/04 - Conan's Dream.mp3";

void GameOverEnter(void* contextTable[])
{
    XWonMenuTexture = LoadTexture(XWonMenuImageFile);
    OWonMenuTexture = LoadTexture(OWonMenuImageFile);
    DrawMenuTexture = LoadTexture(DrawMenuImageFile);

    gameState = *((ChessGameStates*)contextTable[0]);
    Players* cpu = ((Players*)contextTable[1]);
    
    if (gameState == DRAW)
        audioPlayMusic(LoseMusicFile);
    else
    {
        if (cpu && *cpu == WHITE_PLAYER && gameState == BLACK_PLAYER_WON)
            audioPlayMusic(LoseMusicFile);
        else if (cpu && *cpu == BLACK_PLAYER && gameState == WHITE_PLAYER_WON)
            audioPlayMusic(LoseMusicFile);
        else
            audioPlayMusic(WinMusicFile);
    }
}
void GameOverExit()
{
    audioPauseMusic();
}
void GameOverProcessInput(SDL_Event event)
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
            popToState(&mainMenuState);
            break;
        }
        break;
    }
}
void GameOverRender(SDL_Renderer *renderer)
{
    switch (gameState)
    {
    case BLACK_PLAYER_WON:
        SDL_RenderCopy(renderer, XWonMenuTexture, NULL, &WindowRect2);
        break;
    case WHITE_PLAYER_WON:
        SDL_RenderCopy(renderer, OWonMenuTexture, NULL, &WindowRect2);
        break;
    case DRAW:
        SDL_RenderCopy(renderer, DrawMenuTexture, NULL, &WindowRect2);
        break;
    }
}

State gameOverState = {
    .enter = GameOverEnter,
    .exit = GameOverExit,
    .processInput = GameOverProcessInput,
    .render = GameOverRender
};