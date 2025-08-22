#include "LoopLogic/image.h"
#include "LoopLogic/State.h"
#include "GameLogic/RenderingBoard.h"
#include "GameLogic/Board.h"

void RenderChessPieces(SDL_Renderer *renderer, Bitboard boards[12]);
void RenderSelectedSquare(SDL_Renderer *renderer, Bitboard board);

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
SDL_Texture* PiecesTextures[12] = {NULL};
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

    // Pieces
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

    PiecesTextures[0]  = blackPawnTexture;
    PiecesTextures[1]  = blackKnightTexture;
    PiecesTextures[2]  = blackBishopTexture;
    PiecesTextures[3]  = blackRookTexture;
    PiecesTextures[4]  = blackKingTexture;
    PiecesTextures[5]  = blackQueenTexture;
    PiecesTextures[6]  = whitePawnTexture;
    PiecesTextures[7]  = whiteKnightTexture;
    PiecesTextures[8]  = whiteBishopTexture;
    PiecesTextures[9]  = whiteRookTexture;
    PiecesTextures[10] = whiteKingTexture;
    PiecesTextures[11] = whiteQueenTexture;
}

void RenderingBoard(SDL_Renderer *renderer, const GameData *gameData)
{
    SDL_RenderCopy(renderer, boardTexture, NULL, &WindowRect);
    //RenderSelectedSquare(renderer, gameData->selectedSquare);
    SDL_RenderCopy(renderer, boardIndexTexture, NULL, &WindowRect);
    //RenderChessPieces(renderer, gameData->board);
}

// private 
void RenderChessPieces(SDL_Renderer *renderer, Bitboard boards[12])
{
    int counter = 0;
    Bitboard current_board = boards[0];
    while (current_board) {
        int square = __builtin_ctzll(current_board); // index of least significant set bit
        
        cellRect.x = boardXPos + (square % 8) * CELL_WIDTH;
        cellRect.y = boardYPos + (square / 8) * CELL_HEIGHT;
        SDL_RenderCopy(renderer, PiecesTextures[counter], NULL, &cellRect);
        
        current_board &= (current_board - 1); // clear the least significant 1 bit
        while (current_board == 0 && counter <= 11)
        {
            current_board = boards[counter];
            counter++;
        }
    }
  
}
void RenderSelectedSquare(SDL_Renderer *renderer, Bitboard board)
{
    
    SDL_RenderCopy(renderer, BSOverlayTexture, NULL, &cellRect);
    SDL_RenderCopy(renderer, WSOverlayTexture, NULL, &cellRect);

    while (board) {
        int square = __builtin_ctzll(board); // index of least significant set bit
        int x = square % 8;
        int y = square / 8;

        cellRect.x = boardXPos + x * CELL_WIDTH;
        cellRect.y = boardYPos + SDL_YUV_CONVERSION_AUTOMATIC * CELL_HEIGHT;
        if (x % 2)
        {
            if (y % 2)
                SDL_RenderCopy(renderer, BSOverlayTexture, NULL, &cellRect);
            else
                SDL_RenderCopy(renderer, WSOverlayTexture, NULL, &cellRect);
        }
        else
        {
            if (y % 2)
                SDL_RenderCopy(renderer, WSOverlayTexture, NULL, &cellRect);
            else
                SDL_RenderCopy(renderer, BSOverlayTexture, NULL, &cellRect);
        }
        
        board &= (board - 1); // clear the least significant 1 bit
    }    
}
