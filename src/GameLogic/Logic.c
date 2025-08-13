#include <stdio.h>
#include "GameLogic/Logic.h"

void InitBoard(GameData* data);
bool PlayerWon(GameData* data, XOPlayers player);
bool BoardIsFull(XOCellStates board[NUM_ROW_COL][NUM_ROW_COL]);
int MinMax(GameData* data, int depth, bool isMaximizing);

void ResetGame(GameData* data)
{
    data->GameState = STILL_PLAYING;
    data->currentPlayer = PLAYER_X;
    InitBoard(data);
}
void SwitchPlayer(XOPlayers* current)
{
    if (*current == PLAYER_X)
        *current = PLAYER_O;
    else if (*current == PLAYER_O)
        *current = PLAYER_X;
}
bool ClickOnCell(GameData* data, int row, int colum)
{
    if (data->board[row][colum] == EMPTY_CELL)
    {
        if (data->currentPlayer == PLAYER_X)
        {
            data->board[row][colum] = X_CELL;
        }
        else if (data->currentPlayer == PLAYER_O)
        {
            data->board[row][colum] = O_CELL;
        }
        return true;
    }
    return false;
}
void GetAIMove(GameData* data, int* bestRow, int* bestCol)
{
    *bestRow = -1;
    *bestCol = -1;
    
    bool isMaximizing = true;
    if (data->currentPlayer == PLAYER_X) isMaximizing = true;
    else if (data->currentPlayer == PLAYER_O) isMaximizing = false;

    int bestScore = (isMaximizing) ? -1000 : 1000;
    XOCellStates playerCell = (isMaximizing) ? X_CELL : O_CELL;

    for (int row = 0; row < NUM_ROW_COL; row++)
    {
        for (int col = 0; col < NUM_ROW_COL; col++)
        {
            if (data->board[row][col] == EMPTY_CELL)
            {
                // Simulate the move
                data->board[row][col] = playerCell;
                int score = MinMax(data, 10, !isMaximizing);

                // Undo the move
                data->board[row][col] = EMPTY_CELL;

                // Get best move
                if (isMaximizing && (score > bestScore) ||
                    !isMaximizing && (score < bestScore))
                {
                    bestScore = score;
                    *bestRow = row;
                    *bestCol = col;
                }
            }
        }
    }
}
bool GameIsOver(GameData* data)
{
    if (PlayerWon(data, PLAYER_X))
    {
        data->GameState = PLAYER_X_WON;
        return true;
    }
    else if (PlayerWon(data, PLAYER_O))
    {
        data->GameState = PLAYER_O_WON;
        return true;
    }
    else if (BoardIsFull(data->board))
    {
        data->GameState = DRAW;
        return true;
    }

    return false;
}

// private
void InitBoard(GameData* data)
{
    for (int row = 0; row < NUM_ROW_COL; row++)
    {
        for (int colom = 0; colom < NUM_ROW_COL; colom++)
        {
            data->board[row][colom] = EMPTY_CELL;
        }
    }
}
bool PlayerWon(GameData* data, XOPlayers player)
{
    XOCellStates playerCell = (player == PLAYER_X) ? X_CELL : O_CELL;

    // Check rows and columns
    for (int i = 0; i < NUM_ROW_COL; i++)
    {
        if ((data->board[i][0] == playerCell && data->board[i][1] == playerCell && data->board[i][2] == playerCell) ||
            (data->board[0][i] == playerCell && data->board[1][i] == playerCell && data->board[2][i] == playerCell))
        {
            return true;
        }
    }

    // Check diagonals
    if ((data->board[0][0] == playerCell && data->board[1][1] == playerCell && data->board[2][2] == playerCell) ||
        (data->board[0][2] == playerCell && data->board[1][1] == playerCell && data->board[2][0] == playerCell))
    {
        return true;
    }

    return false;
}
bool BoardIsFull(XOCellStates board[NUM_ROW_COL][NUM_ROW_COL])
{
    for (int row = 0; row < NUM_ROW_COL; row++)
    {
        for (int colom = 0; colom < NUM_ROW_COL; colom++)
        {
            if (board[row][colom] == EMPTY_CELL) return false;
        }
    }

    return true;
}
int MinMax(GameData* data, int depth, bool isMaximizing)
{
    if (depth == 0 || GameIsOver(data))
        return data->GameState;
    
    int finalScore = (isMaximizing) ? -1005 : 1005;
    XOCellStates playerCell = (isMaximizing) ? X_CELL : O_CELL;
    
    for (int row = 0; row < NUM_ROW_COL; row++)
    {
        for (int colom = 0; colom < NUM_ROW_COL; colom++)
        {
            if (data->board[row][colom] == EMPTY_CELL)
            {
                data->board[row][colom] = playerCell;
                int score = MinMax(data, depth - 1, !isMaximizing);
                data->board[row][colom] = EMPTY_CELL;
                if (isMaximizing && (score > finalScore) ||
                    !isMaximizing && (score < finalScore))
                {
                    finalScore = score;
                }
            }
        }
    }
    return finalScore;
}
