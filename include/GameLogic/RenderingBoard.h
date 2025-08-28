#ifndef RENDERING_BOARD_H
#define RENDERING_BOARD_H

#include <SDL2/SDL.h>
#include "GameLogic/GameData.h"
#include "GameLogic/Board.h"

void GetXOImage();
void RenderingBoard(SDL_Renderer *renderer, Board board);

#endif