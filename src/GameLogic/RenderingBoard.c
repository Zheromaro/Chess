#include "LoopLogic/image.h"
#include "LoopLogic/State.h"
#include "GameLogic/RenderingBoard.h"

void RenderChessPieces(SDL_Renderer *renderer, const Square board[NUM_ROW_COL ][NUM_ROW_COL ]);
void RenderSelectedSquare(SDL_Renderer *renderer, const Square* selectedSquare);

const float boardXPos = 9.2;
const float boardYPos = 9.2;
SDL_Rect cellRect = {.h = CELL_HEIGHT, .w = CELL_WIDTH, .x = 0, .y = 0};
SDL_Rect WindowRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SDL_Texture* CircleOverlayTexture = NULL;
SDL_Texture* BSOverlayTexture = NULL;
SDL_Texture* WSOverlayTexture = NULL;
SDL_Texture* boardTexture = NULL;
SDL_Texture* boardIndexTexture = NULL;
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
char CircleOverlayImgPath[] = "assets/img/CircleOverlay.png";
char BSOverlayImgPath[] = "assets/img/BlackSquareOverlay.png";
char WSOverlayImgPath[] = "assets/img/WhiteSquareOverlay.png";
char boardImgPath[] = "assets/img/Board.png";
char boardIndexImgPath[] = "assets/img/BoardIndex.png";
char blackPawnImgPath[] = "assets/img/blackPieces/BlackPawn.png";
char blackKnightImgPath[] = "assets/img/blackPieces/BlackKnight.png";
char blackBishopImgPath[] = "assets/img/blackPieces/BlackBishop.png";
char blackRookImgPath[] = "assets/img/blackPieces/BlackRook.png";
char blackKingImgPath[] = "assets/img/blackPieces/BlackKing.png";
char blackQueenImgPath[] = "assets/img/blackPieces/BlackQueen.png";
char whitePawnImgPath[] = "assets/img/whitePieces/WhitePawn.png";
char whiteKnightImgPath[] = "assets/img/whitePieces/WhiteKnight.png";
char whiteBishopImgPath[] = "assets/img/whitePieces/WhiteBishop.png";
char whiteRookImgPath[] = "assets/img/whitePieces/WhiteRook.png";
char whiteKingImgPath[] = "assets/img/whitePieces/WhiteKing.png";
char whiteQueenImgPath[] = "assets/img/whitePieces/WhiteQueen.png";

void GetXOImage(){
    CircleOverlayTexture = LoadTexture(CircleOverlayImgPath);
    BSOverlayTexture = LoadTexture(BSOverlayImgPath);
    WSOverlayTexture = LoadTexture(WSOverlayImgPath);
    boardTexture = LoadTexture(boardImgPath);
    boardIndexTexture = LoadTexture(boardIndexImgPath);
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
    RenderSelectedSquare(renderer, gameData->selectedSquare);
    SDL_RenderCopy(renderer, boardIndexTexture, NULL, &WindowRect);
    RenderChessPieces(renderer, gameData->board);
}

// private 
void RenderChessPieces(SDL_Renderer *renderer, const Square board[NUM_ROW_COL][NUM_ROW_COL])
{
    for (int row = 0; row < NUM_ROW_COL; row++)
    {
        for (int colom = 0; colom < NUM_ROW_COL; colom++)
        {
            if (board[row][colom].piece == NULL) continue;
            
            cellRect.x = boardXPos + row * CELL_WIDTH;
            cellRect.y = boardYPos + colom * CELL_HEIGHT;
            switch (board[row][colom].piece->pieceType)
            {
            case PAWN:
                if (board[row][colom].piece->pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackPawnTexture, NULL, &cellRect);
                else if (board[row][colom].piece->pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whitePawnTexture, NULL, &cellRect);
                break;
            case KNIGHT:
                if (board[row][colom].piece->pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackPawnTexture, NULL, &cellRect);
                else if (board[row][colom].piece->pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteKnightTexture, NULL, &cellRect);
                break;
            case BISHOP:
                if (board[row][colom].piece->pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackBishopTexture, NULL, &cellRect);
                else if (board[row][colom].piece->pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteBishopTexture, NULL, &cellRect);
                break;
            case ROOK:
                if (board[row][colom].piece->pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackRookTexture, NULL, &cellRect);
                else if (board[row][colom].piece->pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteRookTexture, NULL, &cellRect);
                break;
            case KING:
                if (board[row][colom].piece->pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackKingTexture, NULL, &cellRect);
                else if (board[row][colom].piece->pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteKingTexture, NULL, &cellRect);
                break;
            case QUEEN:
                if (board[row][colom].piece->pieceColor == BLACK)
                    SDL_RenderCopy(renderer, blackQueenTexture, NULL, &cellRect);
                else if (board[row][colom].piece->pieceColor == WHITE)
                    SDL_RenderCopy(renderer, whiteQueenTexture, NULL, &cellRect);
                break;
            }
        }
    }
    
}
void RenderSelectedSquare(SDL_Renderer *renderer, const Square* selectedSquare)
{
    if (selectedSquare == NULL) return;

    cellRect.x = boardXPos + selectedSquare->x * CELL_WIDTH;
    cellRect.y = boardYPos + selectedSquare->y * CELL_HEIGHT;

    if (selectedSquare->x % 2)
    {
        if (selectedSquare->y % 2)
            SDL_RenderCopy(renderer, BSOverlayTexture, NULL, &cellRect);
        else
            SDL_RenderCopy(renderer, WSOverlayTexture, NULL, &cellRect);
    }
    else
    {
        if (selectedSquare->y % 2)
            SDL_RenderCopy(renderer, WSOverlayTexture, NULL, &cellRect);
        else
            SDL_RenderCopy(renderer, BSOverlayTexture, NULL, &cellRect);
    }
    
}
