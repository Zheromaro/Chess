#ifndef CLICK_ON_BOARD_H
#define CLICK_ON_BOARD_H

#include <stdbool.h>
#include "GameLogic/GameData.h"
#include "GameLogic/Board.h"

void ResetGame(GameData* data);
void SwitchPlayer(Players* current);
bool GameIsOver(GameData* data);
void GetAIMove(GameData* data, int* bestRow, int* bestCol);

#endif