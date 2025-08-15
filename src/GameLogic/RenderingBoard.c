#include "image.h"
#include "State.h"
#include "GameLogic/RenderingBoard.h"

void RenderCells(SDL_Renderer *renderer, const SquareStates board[NUM_ROW_COL ][NUM_ROW_COL ]);

SDL_Rect cellRect = {.h = CELL_HEIGHT, .w = CELL_WIDTH, .x = 0, .y = 0};
SDL_Rect WindowRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SDL_Texture* boardTexture = NULL;
SDL_Texture* blackPawnTexture = NULL;
SDL_Texture* blackKnightTexture = NULL;
SDL_Texture* blackBishopTexture = NULL;
SDL_Texture* blackRookTexture = NULL;
SDL_Texture* blackKingTexture = NULL;
SDL_Texture* blackQueenTexture = NULL;
SDL_Texture* whitePawnTexture = NULL;
SDL_Texture* whiteKnightTexture = NULL;
SDL_Texture* whiteBishopTexture = NULL;
SDL_Texture* whiteRookTexture = NULL;
SDL_Texture* whiteKingTexture = NULL;
SDL_Texture* whiteQueenTexture = NULL;
char boardImgPath[] = "assets/img/ChessBoard.png";
char blackPawnImgPath[] = "assets/img/blackPieces/blackPawn.png";
char blackKnightImgPath[] = "assets/img/blackPieces/blackKnight.png";
char blackBishopImgPath[] = "assets/img/blackPieces/blackBishop.png";
char blackRookImgPath[] = "assets/img/blackPieces/blackRook.png";
char blackKingImgPath[] = "assets/img/blackPieces/blackKing.png";
char blackQueenImgPath[] = "assets/img/blackPieces/blackQueen.png";
char whitePawnImgPath[] = "assets/img/whitePieces/whitePawn.png";
char whiteKnightImgPath[] = "assets/img/whitePieces/whiteKnight.png";
char whiteBishopImgPath[] = "assets/img/whitePieces/whiteBishop.png";
char whiteRookImgPath[] = "assets/img/whitePieces/whiteRook.png";
char whiteKingImgPath[] = "assets/img/whitePieces/whiteKing.png";
char whiteQueenImgPath[] = "assets/img/whitePieces/whiteQueen.png";

void GetXOImage(){
    boardTexture = LoadTexture(boardImgPath);
    blackPawnTexture = LoadTexture(blackPawnImgPath);
    blackKnightTexture = LoadTexture(blackKnightImgPath);
    blackBishopTexture = LoadTexture(blackBishopImgPath);
    blackRookTexture = LoadTexture(blackRookImgPath);
    blackKingTexture = LoadTexture(blackKingImgPath);
    blackQueenTexture = LoadTexture(blackQueenImgPath);
    whitePawnTexture = LoadTexture(whitePawnImgPath);
    whiteKnightTexture = LoadTexture(whiteKnightImgPath);
    whiteBishopTexture = LoadTexture(whiteBishopImgPath);
    whiteRookTexture = LoadTexture(whiteRookImgPath);
    whiteKingTexture = LoadTexture(whiteKingImgPath);
    whiteQueenTexture = LoadTexture(whiteQueenImgPath);
}

void RenderingBoard(SDL_Renderer *renderer, const GameData *gameData)
{
    SDL_RenderCopy(renderer, boardTexture, NULL, &WindowRect);
    RenderCells(renderer, gameData->board);
}

// private 
void RenderCells(SDL_Renderer *renderer, const SquareStates board[NUM_ROW_COL ][NUM_ROW_COL ])
{
    for (int row = 0; row < NUM_ROW_COL; row++)
    {
        for (int colom = 0; colom < NUM_ROW_COL; colom++)
        {
            if (board[row][colom].isEmpty == true) continue;
            
            cellRect.x = row * CELL_WIDTH;
            cellRect.y = colom * CELL_HEIGHT;
            switch (board[row][colom].piece)
            {
            case PAWN:
                if (board[row][colom].pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackPawnTexture, NULL, &cellRect);
                else if (board[row][colom].pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whitePawnTexture, NULL, &cellRect);
                break;
            case KNIGHT:
                if (board[row][colom].pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackPawnTexture, NULL, &cellRect);
                else if (board[row][colom].pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteKnightTexture, NULL, &cellRect);
                break;
            case BISHOP:
                if (board[row][colom].pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackBishopTexture, NULL, &cellRect);
                else if (board[row][colom].pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteBishopTexture, NULL, &cellRect);
                break;
            case ROOK:
                if (board[row][colom].pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackRookTexture, NULL, &cellRect);
                else if (board[row][colom].pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteRookTexture, NULL, &cellRect);
                break;
            case KING:
                if (board[row][colom].pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackKingTexture, NULL, &cellRect);
                else if (board[row][colom].pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteKingTexture, NULL, &cellRect);
                break;
            case QUEEN:
                if (board[row][colom].pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackQueenTexture, NULL, &cellRect);
                else if (board[row][colom].pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteQueenTexture, NULL, &cellRect);
                break;
            }
        }
    }
    
}
