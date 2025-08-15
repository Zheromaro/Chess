#ifndef CLICK_ON_BOARD_H
#define CLICK_ON_BOARD_H

#include <stdbool.h>
#include "GameLogic/GameData.h"

void ResetGame(GameData* data);
void SwitchPlayer(Players* current);
bool ClickOnPiece(GameData* data, int row, int colum);
bool GameIsOver(GameData* data);
void GetAIMove(GameData* data, int* bestRow, int* bestCol);

#endif