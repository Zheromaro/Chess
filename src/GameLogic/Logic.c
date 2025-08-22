#include <stdio.h>
#include <stdlib.h>
#include "GameLogic/Logic.h"
#include "GameLogic/Board.h"

bool PlayerWon(GameData* data, Players player);
int MinMax(GameData* data, int depth, bool isMaximizing);

void ResetGame(GameData* data)
{
    data->GameState = STILL_PLAYING;
    data->currentPlayer = WHITE_PLAYER;
}
void SwitchPlayer(Players* current)
{
    if (*current == WHITE_PLAYER)
        *current = BLACK_PLAYER;
    else if (*current == BLACK_PLAYER)
        *current = WHITE_PLAYER;
}
void GetAIMove(GameData* data, int* bestRow, int* bestCol)
{
    // TODO: make it work for chess

    //*bestRow = -1;
    //*bestCol = -1;
    
    //bool isMaximizing = true;
    //if (data->currentPlayer == WHITE_PLAYER) isMaximizing = true;
    //else if (data->currentPlayer == BLACK_PLAYER) isMaximizing = false;

    //int bestScore = (isMaximizing) ? -1000 : 1000;
    //Pieces playerCell = (isMaximizing) ? X_CELL : O_CELL;

    //for (int row = 0; row < NUM_ROW_COL; row++)
    //{
    //    for (int col = 0; col < NUM_ROW_COL; col++)
    //    {
    //        if (data->board[row][col] == EMPTY)
    //        {
    //            // Simulate the move
    //            data->board[row][col] = playerCell;
    //            int score = MinMax(data, 10, !isMaximizing);

    //            // Undo the move
    //            data->board[row][col] = EMPTY;

    //            // Get best move
    //            if (isMaximizing && (score > bestScore) ||
    //                !isMaximizing && (score < bestScore))
    //            {
    //                bestScore = score;
    //                *bestRow = row;
    //                *bestCol = col;
    //            }
    //        }
    //    }
    //}
}
bool GameIsOver(GameData* data)
{
    // TODO: Add resign and agreed on draw
    if (PlayerWon(data, WHITE_PLAYER))
    {
        data->GameState = BLACK_PLAYER_WON;
        return true;
    }
    else if (PlayerWon(data, BLACK_PLAYER))
    {
        data->GameState = WHITE_PLAYER_WON;
        return true;
    }

    return false;
}

// private
bool PlayerWon(GameData* data, Players player)
{
    // TODO: Check if King is cheked
    // TODO: Check if King can't move

    return false;
}
int MinMax(GameData* data, int depth, bool isMaximizing)
{
    // TODO: make MinMax for chess

    //if (depth == 0 || GameIsOver(data))
    //    return data->GameState;
    //
    //int finalScore = (isMaximizing) ? -1005 : 1005;
    //Pieces playerCell = (isMaximizing) ? X_CELL : O_CELL;
    //
    //for (int row = 0; row < NUM_ROW_COL; row++)
    //{
    //    for (int colom = 0; colom < NUM_ROW_COL; colom++)
    //    {
    //        if (data->board[row][colom] == EMPTY)
    //        {
    //            data->board[row][colom] = playerCell;
    //            int score = MinMax(data, depth - 1, !isMaximizing);
    //            data->board[row][colom] = EMPTY;
    //            if (isMaximizing && (score > finalScore) ||
    //                !isMaximizing && (score < finalScore))
    //            {
    //                finalScore = score;
    //            }
    //        }
    //    }
    //}
    //return finalScore;
    
    return 0;
}
