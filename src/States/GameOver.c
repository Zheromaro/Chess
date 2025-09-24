#include <SDL2/SDL.h>
#include "GameConstant.h"
#include "LoopLogic/image.h"
#include "LoopLogic/audio.h"
#include "LoopLogic/text.h"
#include "LoopLogic/appSettings.h"
#include "LoopLogic/State.h"
#include "GameLogic/Board.h"

ChessGameStates gameState = WHITE_CHECKMATED;  
SDL_Rect WindowRect2 = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Texture* BlackWinMenuTexture = NULL;
SDL_Texture* WhiteWinMenuTexture = NULL;
SDL_Texture* StalemateMenuTexture = NULL;

char BlackWinMenuImageFile[] = "assets/img/Menus/BlackWinMenu.png";
char WhiteWinMenuImageFile[] = "assets/img/Menus/WhiteWinMenu.png";
char StalemateWinMenuImageFile[] = "assets/img/Menus/StalemateMenu.png";
char WinMusicFile[] = "assets/sound/01 - Detective Conan Main Theme.mp3";
char LoseMusicFile[] = "assets/sound/04 - Conan's Dream.mp3";

void GameOverEnter(void* contextTable[])
{
    BlackWinMenuTexture = LoadTexture(BlackWinMenuImageFile);
    WhiteWinMenuTexture = LoadTexture(WhiteWinMenuImageFile);
    StalemateMenuTexture = LoadTexture(StalemateWinMenuImageFile);

    gameState = *((ChessGameStates*)contextTable[0]);
    Players* cpu = ((Players*)contextTable[1]);

    if (gameState == STALEMATE)
        audioPlayMusic(LoseMusicFile);
    else
    {
        if (cpu && *cpu == WHITE_PLAYER && gameState == BLACK_CHECKMATED)
            audioPlayMusic(LoseMusicFile);
        else if (cpu && *cpu == BLACK_PLAYER && gameState == WHITE_CHECKMATED)
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
        case SDLK_KP_1:
            popState();
            break;
        case SDLK_KP_2:
            popToState(mainMenuState);
            break;
        }
        break;
    }
}
void GameOverRender(SDL_Renderer *renderer)
{
    switch (gameState)
    {
    case BLACK_CHECKMATED:
        SDL_RenderCopy(renderer, WhiteWinMenuTexture, NULL, &WindowRect2);
        break;
    case WHITE_CHECKMATED:
        SDL_RenderCopy(renderer, BlackWinMenuTexture, NULL, &WindowRect2);
        break;
    case STALEMATE:
        SDL_RenderCopy(renderer, StalemateMenuTexture, NULL, &WindowRect2);
        break;
    }
}

State gameOverState = {
    .enter = GameOverEnter,
    .exit = GameOverExit,
    .processInput = GameOverProcessInput,
    .render = GameOverRender
};