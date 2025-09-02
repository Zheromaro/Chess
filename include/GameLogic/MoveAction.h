#ifndef MOVE_ACTION_H
#define MOVE_ACTION_H

#include <stdint.h>
#include <stdbool.h>
#include "GameLogic/Board.h"

void click_on_square(Board* board, Players* player, short square_index);

#endif