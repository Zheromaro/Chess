#include "LoopLogic/image.h"
#include "LoopLogic/State.h"
#include "GameLogic/RenderingBoard.h"

void RenderSelectedSquare(SDL_Renderer *renderer, Bitboard selectedSquare);
void RenderChessPieces(SDL_Renderer *renderer, Board board);
void RenderAvailableSquare(SDL_Renderer *renderer, Bitboard availableSquare);

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
}

void RenderingBoard(SDL_Renderer *renderer, Board board)
{
    SDL_RenderCopy(renderer, boardTexture, NULL, &WindowRect);
    RenderSelectedSquare(renderer, board.cliked_square_index);
    SDL_RenderCopy(renderer, boardIndexTexture, NULL, &WindowRect);
    RenderChessPieces(renderer, board);
    RenderAvailableSquare(renderer, board.available_moves);
}

// private
void RenderSelectedSquare(SDL_Renderer *renderer, Bitboard selectedSquare)
{
    if (selectedSquare)
    {
        cellRect.x = boardXPos + (selectedSquare % 8) * CELL_WIDTH;
        cellRect.y = boardYPos + (selectedSquare / 8) * CELL_HEIGHT;
        if (selectedSquare % 2)
            SDL_RenderCopy(renderer, BSOverlayTexture, NULL, &cellRect);
        else
            SDL_RenderCopy(renderer, WSOverlayTexture, NULL, &cellRect);
    } 
}
void RenderChessPieces(SDL_Renderer *renderer, Board board)
{
    for (int i = 0; i < 64; i++) {
        uint64_t mask = 1ULL << i;     // make a mask with only bit i set
        bool bpawn   = (board.pieces[BLACK_PLAYER][PAWN]   & mask) ? 1 : 0;  // check if that bit is set
        bool bknight = (board.pieces[BLACK_PLAYER][KNIGHT] & mask) ? 1 : 0;
        bool bbishop = (board.pieces[BLACK_PLAYER][BISHOP] & mask) ? 1 : 0;
        bool brook   = (board.pieces[BLACK_PLAYER][ROOK]   & mask) ? 1 : 0;
        bool bqueen  = (board.pieces[BLACK_PLAYER][QUEEN]  & mask) ? 1 : 0;
        bool bking   = (board.pieces[BLACK_PLAYER][KING]   & mask) ? 1 : 0;
        bool wpawn   = (board.pieces[WHITE_PLAYER][PAWN]   & mask) ? 1 : 0;
        bool wknight = (board.pieces[WHITE_PLAYER][KNIGHT] & mask) ? 1 : 0;
        bool wbishop = (board.pieces[WHITE_PLAYER][BISHOP] & mask) ? 1 : 0;
        bool wrook   = (board.pieces[WHITE_PLAYER][ROOK]   & mask) ? 1 : 0;
        bool wqueen  = (board.pieces[WHITE_PLAYER][QUEEN]  & mask) ? 1 : 0;
        bool wking   = (board.pieces[WHITE_PLAYER][KING]   & mask) ? 1 : 0;
        
        cellRect.x = boardXPos + (i % 8) * CELL_WIDTH;
        cellRect.y = boardYPos + (7 - (i / 8)) * CELL_HEIGHT;

        if (bpawn)
            SDL_RenderCopy(renderer, blackPawnTexture, NULL, &cellRect);
        if (bknight)
            SDL_RenderCopy(renderer, blackKnightTexture, NULL, &cellRect);
        if (bbishop)
            SDL_RenderCopy(renderer, blackBishopTexture, NULL, &cellRect);
        if (brook)
            SDL_RenderCopy(renderer, blackRookTexture, NULL, &cellRect);
        if (bqueen)
            SDL_RenderCopy(renderer, blackQueenTexture, NULL, &cellRect);
        if (bking)
            SDL_RenderCopy(renderer, blackKingTexture, NULL, &cellRect);
        if (wpawn)
            SDL_RenderCopy(renderer, whitePawnTexture, NULL, &cellRect);
        if (wknight)
            SDL_RenderCopy(renderer, whiteKnightTexture, NULL, &cellRect);
        if (wbishop)
            SDL_RenderCopy(renderer, whiteBishopTexture, NULL, &cellRect);
        if (wrook)
            SDL_RenderCopy(renderer, whiteRookTexture, NULL, &cellRect);
        if (wqueen)
            SDL_RenderCopy(renderer, whiteQueenTexture, NULL, &cellRect);
        if (wking)
            SDL_RenderCopy(renderer, whiteKingTexture, NULL, &cellRect);
    }
}
void RenderAvailableSquare(SDL_Renderer *renderer, Bitboard availableSquare)
{
    if (!availableSquare) return;
    
    for (int i = 0; i < 64; i++) {
        uint64_t mask = 1ULL << i;     // make a mask with only bit i set
        bool sq = (availableSquare & mask) ? 1 : 0;
        
        cellRect.x = boardXPos + (i % 8) * CELL_WIDTH;
        cellRect.y = boardYPos + (i / 8) * CELL_HEIGHT;

        if (sq)
            SDL_RenderCopy(renderer, CircleOverlayTexture, NULL, &cellRect);
    }
}
