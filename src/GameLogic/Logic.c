#include <stdio.h>
#include <stdlib.h>
#include "GameLogic/Logic.h"

Piece* NewPiece(PieceType type, PieceColor color);
void InitBoard(GameData* data);
bool PlayerWon(GameData* data, Players player);
bool Stalemate(Square board[NUM_ROW_COL][NUM_ROW_COL]);
int MinMax(GameData* data, int depth, bool isMaximizing);

void ResetGame(GameData* data)
{
    data->GameState = STILL_PLAYING;
    data->currentPlayer = WHITE_PLAYER;
    InitBoard(data);
}
void SwitchPlayer(Players* current)
{
    if (*current == WHITE_PLAYER)
        *current = BLACK_PLAYER;
    else if (*current == BLACK_PLAYER)
        *current = WHITE_PLAYER;
}
bool ClickOnPiece(GameData* data, int row, int colum)
{
    Square square = data->board[row][colum];
    Players player = data->currentPlayer;

    data->selectedSquare = &(data->board[row][colum]);
    if (square.piece == NULL) return false;
    if (square.piece->pieceColor == BLACK && player == WHITE_PLAYER) return false;
    if (square.piece->pieceColor == WHITE && player == BLACK_PLAYER) return false;
    
    // TODO: deselect old selected piece
    // TODO: select the new piece
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
    else if (Stalemate(data->board))
    {
        data->GameState = DRAW;
        return true;
    }

    return false;
}

// private
void InitBoard(GameData* data)
{
    data->selectedSquare = NULL;
    for (int row = 0; row < NUM_ROW_COL; row++)
    {
        for (int colom = 0; colom < NUM_ROW_COL; colom++)
        {
            data->board[row][colom].x = row;
            data->board[row][colom].y = colom;
            if (colom == 1 || colom == 6)
            {
                Piece* piece = NewPiece(PAWN, (colom == 1) ? BLACK : WHITE);
                data->board[row][colom].piece = piece;
            }
            else if (colom == 0 || colom == 7)
            {
                Piece* piece = NewPiece(0 , (colom == 0) ? BLACK : WHITE);
                switch (row)
                {
                case 0:
                case 7:
                    piece->pieceType = ROOK;
                    break;
                case 1:
                case 6:
                    piece->pieceType = KNIGHT;
                    break;
                case 2:
                case 5:
                    piece->pieceType = BISHOP;
                    break;
                case 3:
                    piece->pieceType = QUEEN;
                    break;
                case 4:
                    piece->pieceType = KING;
                    break;
                default:
                    break;
                }
                data->board[row][colom].piece = piece;
            }
            
        }
    }
}
bool PlayerWon(GameData* data, Players player)
{
    // TODO: Check if King is cheked
    // TODO: Check if King can't move

    return false;
}
bool Stalemate(Square board[NUM_ROW_COL][NUM_ROW_COL])
{
    // TODO: Check if King can't move
    // TODO: Check if no other piece can move

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
Piece* NewPiece(PieceType type, PieceColor color)
{
    Piece* result = malloc(sizeof(Piece));
    if (result == NULL) exit(EXIT_FAILURE);

    result->pieceType = type;
    result->pieceColor = color;
    return result;
}