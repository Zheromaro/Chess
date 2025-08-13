#include <stdbool.h>
#include <SDL2/SDL.h>
#include <Stack.h>
#include "GameLogic/GameData.h"
#include "State.h"
#include "appSettings.h"

#define MAX_CONTEXT_NUM 10

void ResetContext(void* contextTable[MAX_CONTEXT_NUM], int *contextCount);

CallbackObj_LL callObj = {NULL, NULL, NULL};
Stack* stateStack = NULL;
State* current = NULL;
State* holdState = NULL;
int contextCount = 0;
void* contextTable[MAX_CONTEXT_NUM] = {NULL};

SDL_Event event; // ProcessInput
int last_frame_time = 0; // Update

// Main Func
bool StateInit()
{
    stateStack = NewStack(sizeof(State), callObj);
    ResetContext(contextTable ,&contextCount);
    
    return true;
}
void StateUpdate() // <- Enter & Exit
{
    if (holdState)
    {
        if (current && current->exit)
            ((PExit)current->exit)();

        current = holdState;
        if (current && current->enter)
            ((PEnter)holdState->enter)(contextTable);

        holdState = NULL;
        ResetContext(contextTable ,&contextCount);
    }
}
void StateRelease()
{
    ResetContext(contextTable ,&contextCount);
    while (holdState != NULL)
    {
        if (holdState && holdState->exit)
            ((PExit)holdState->exit)();
        
        holdState = Pop(stateStack);
    }
}

// State management
void pushState(State* newState, const char* format, ...)
{
    // Get Context
    va_list args;
    va_start(args, format);
    void* val = NULL;

    while (*format != '\0') {
        val = NULL;
        switch (*format) {
            case 'b':
            case 'i': {
                val = malloc(sizeof(int));
                *(int*)val = va_arg(args, int);
                break;
            }
            case 'd': {
                val = malloc(sizeof(double));
                *(double*)val = va_arg(args, double); // float promoted to double
                break;
            }
            case 's': {
                char* src = va_arg(args, char*);
                val = malloc(strlen(src) + 1);
                strcpy(val, src);
                break;
            }
            case 'G': {
                val = malloc(sizeof(XOGameStates));
                *(XOGameStates*)val = va_arg(args, XOGameStates);
                break;
            }
            case 'P': {
                val = malloc(sizeof(XOPlayers));
                *(XOPlayers*)val = va_arg(args, XOPlayers);
                break;
            }
            case ' ': 
                break;
            case '%': 
                printf("format specifier of %% is not supported");
                break;
            default:
                printf("Unknown format specifier: %c\n", *format);
        }
        
        if (val)
        {
            contextTable[contextCount] = val;
            contextCount++;
        }
        format++;
    }
    va_end(args);

    // Change State
    if (current)
        Puch(stateStack, current);
    
    holdState = newState;
}
void popState()
{
    holdState = Pop(stateStack);
    if (!holdState) printf("The state stack is empty!\n");
}
void popToState(State* dist)
{
    holdState = Pop(stateStack);
    while (holdState != NULL && holdState != dist)
    {
        holdState = Pop(stateStack);
    }

    if (!holdState) printf("The state stack is empty!\n");
}

// Game loop functions
void ReloadState()
{
    if (current && current->reloading)
        ((PReloading)current->reloading)();
}
void ProcessInput()
{
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT ||
        event.key.keysym.sym == SDLK_ESCAPE) CloseGame();

    if (current && current->processInput)
        ((PProcessInput)current->processInput)(event); 
}
void Update()
{
    int time_to_wait = SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);
    
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    // calling Update
    if (current && current->update)
        ((PUpdate)current->update)(delta_time);
}
void Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // here we start drawing our game 
    if (current && current->render)
        ((PRender)current->render)(renderer);

    SDL_RenderPresent(renderer);   
}

// private
void ResetContext(void* contextTable[MAX_CONTEXT_NUM], int *contextCount)
{
    for (int i = 0; i < MAX_CONTEXT_NUM; i++)
    {
        if (contextTable[i])
        {
            free(contextTable[i]);
            contextTable[i] = NULL;
        }
    }
    *contextCount = 0;
}
