#ifndef RENDERING_BOARD_H
#define RENDERING_BOARD_H

#include <SDL2/SDL.h>
#include "GameConstant.h"
#include "GameLogic/Board.h"

void GetXOImage();
void BoardRendering(SDL_Renderer *renderer, Board board);

#endif