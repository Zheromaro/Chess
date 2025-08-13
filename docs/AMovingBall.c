#include <SDL2/SDL_image.h>
#include "mainLayer.h"
#include "constants.h"

struct ball
{
    float x;
    float y;
    float width;
    float height;
} ball;

struct dir
{
    float x;
    float y;
}dir;

int followMouse = 0;

void Setup() {
    ball.width = 15;
    ball.height = 15;
    ball.x = WINDOW_HEIGHT/2 - ball.height/2;
    ball.y = WINDOW_WIDTH/2 - ball.width/2;

    dir.x = 0;
    dir.y = 0;
}
void ProcessInput(SDL_Event event) {
    
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            dir.y = -1;
            break;
        
        case SDLK_s:
            dir.y = 1;
            break;
            
        case SDLK_d:
            dir.x = 1;
            break;
            
        case SDLK_a:
            dir.x = -1;
            break;
        }
    break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            dir.y = 0;
            break;
        
        case SDLK_s:
            dir.y = 0;
            break;
            
        case SDLK_d:
            dir.x = 0;
            break;
            
        case SDLK_a:
            dir.x = 0;
            break;
        }
    break;
    case SDL_MOUSEBUTTONUP:
        switch (event.button.button)
        {
        case SDL_BUTTON_RIGHT:
            ball.x = event.button.x;
            ball.y = event.button.y;
        break;
        case SDL_BUTTON_LEFT:
            if(followMouse == 0) { 
                followMouse = 1;
            } else { 
                followMouse = 0;
            }
        break;

        }
    break;
    case SDL_MOUSEMOTION:
        if(followMouse) {
            ball.x = event.motion.x;
            ball.y = event.motion.y;
        }
    break;
    }
}
void Update(float delta_time) {
    ball.x += dir.x;
    ball.y += dir.y;
}
void Render(SDL_Renderer *renderer) {
    SDL_Rect ball_rect = {
        ball.x, 
        ball.y, 
        ball.width, 
        ball.height
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

}
