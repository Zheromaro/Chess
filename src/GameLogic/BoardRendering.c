#include "LoopLogic/image.h"
#include "GameLogic/BoardRendering.h"
#include "GameConstant.h"

void RenderSelectedSquare(SDL_Renderer *renderer, short selectedSquare, bool fliped);
void RenderKingChecked(SDL_Renderer *renderer, Board board, bool fliped);
void RenderChessPieces(SDL_Renderer *renderer, Board board);
void RenderAvailableSquare(SDL_Renderer *renderer, Bitboard availableSquare, bool fliped);
void RenderPromotionMenu(SDL_Renderer *renderer, short promotoin_sq_index);

const float boardXPos = 9.2;
const float boardYPos = 9.2;
SDL_Rect cellRect = {.h = CELL_HEIGHT, .w = CELL_WIDTH, .x = 0, .y = 0};
SDL_Rect WindowRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SDL_Texture* boardTexture = NULL;
SDL_Texture* boardIndexTexture = NULL;
SDL_Texture* promotionMenuTexture = NULL;
SDL_Texture* CircleOverlayTexture = NULL;
SDL_Texture* BSOverlayTexture = NULL;
SDL_Texture* WSOverlayTexture = NULL;
SDL_Texture* BSDangerTexture = NULL;
SDL_Texture* WSDangerTexture = NULL;
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
char boardImgPath[] =      "assets/img/Board.png";
char boardIndexImgPath[] = "assets/img/BoardIndex.png";
char promotionMenuImgPath[] = "assets/img/Menus/PromotionMenu.png";
char CircleOverlayImgPath[] = "assets/img/feedbackImg/CircleOverlay.png";
char BSOverlayImgPath[] =     "assets/img/feedbackImg/BlackSquareOverlay.png";
char WSOverlayImgPath[] =     "assets/img/feedbackImg/WhiteSquareOverlay.png";
char BSDangerImgPath[] =      "assets/img/feedbackImg/BlackSquareDanger.png";
char WSDangerImgPath[] =      "assets/img/feedbackImg/WhiteSquareDanger.png";
char blackPawnImgPath[] =   "assets/img/blackPieces/BlackPawn.png";
char blackKnightImgPath[] = "assets/img/blackPieces/BlackKnight.png";
char blackBishopImgPath[] = "assets/img/blackPieces/BlackBishop.png";
char blackRookImgPath[] =   "assets/img/blackPieces/BlackRook.png";
char blackKingImgPath[] =   "assets/img/blackPieces/BlackKing.png";
char blackQueenImgPath[] =  "assets/img/blackPieces/BlackQueen.png";
char whitePawnImgPath[] =   "assets/img/whitePieces/WhitePawn.png";
char whiteKnightImgPath[] = "assets/img/whitePieces/WhiteKnight.png";
char whiteBishopImgPath[] = "assets/img/whitePieces/WhiteBishop.png";
char whiteRookImgPath[] =   "assets/img/whitePieces/WhiteRook.png";
char whiteKingImgPath[] =   "assets/img/whitePieces/WhiteKing.png";
char whiteQueenImgPath[] =  "assets/img/whitePieces/WhiteQueen.png";

void GetXOImage(){
    boardTexture = LoadTexture(boardImgPath);
    boardIndexTexture = LoadTexture(boardIndexImgPath);
    promotionMenuTexture = LoadTexture(promotionMenuImgPath);
    CircleOverlayTexture = LoadTexture(CircleOverlayImgPath);
    BSOverlayTexture = LoadTexture(BSOverlayImgPath);
    WSOverlayTexture = LoadTexture(WSOverlayImgPath);
    BSDangerTexture = LoadTexture(BSDangerImgPath);
    WSDangerTexture = LoadTexture(WSDangerImgPath);

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

void BoardRendering(SDL_Renderer *renderer, Board board)
{
    SDL_RenderCopy(renderer, boardTexture, NULL, &WindowRect);
    RenderKingChecked(renderer, board, board.fliped);
    if (board.promotion_square_index == -1)
        RenderSelectedSquare(renderer, board.cliked_square_index, board.fliped);
    SDL_RenderCopy(renderer, boardIndexTexture, NULL, &WindowRect);
    RenderChessPieces(renderer, board);
    if (board.promotion_square_index == -1)
        RenderAvailableSquare(renderer, board.available_moves, board.fliped);
    RenderPromotionMenu(renderer, board.promotion_square_index);
}

// private
void RenderSelectedSquare(SDL_Renderer *renderer, short selectedSquare, bool fliped)
{
    if (selectedSquare > 0 && selectedSquare < 64)
    {
        int file = (selectedSquare % 8);
        int rank = (fliped)? (7 - (selectedSquare / 8)) : (selectedSquare / 8);
        cellRect.x = boardXPos + file * CELL_WIDTH;
        cellRect.y = boardYPos + rank * CELL_HEIGHT;
        if (selectedSquare % 2)
            SDL_RenderCopy(renderer, BSOverlayTexture, NULL, &cellRect);
        else
            SDL_RenderCopy(renderer, WSOverlayTexture, NULL, &cellRect);
    } 
}
void RenderKingChecked(SDL_Renderer *renderer, Board board, bool fliped)
{
    short king_square = 0;
    if (board.state == BLACK_CHECKED && board.pieces[BLACK_PLAYER][KING] != 0)
    {
        king_square = __builtin_ctzll(board.pieces[BLACK_PLAYER][KING]); // __builtin_ctzll() <- don't accept zero
    }
    else if (board.state == WHITE_CHECKED && board.pieces[WHITE_PLAYER][KING] != 0)
    {
        king_square = __builtin_ctzll(board.pieces[WHITE_PLAYER][KING]);
    }

    if (king_square)
    {
        int file = (king_square % 8);
        int rank = (fliped)? (7 - (king_square / 8)) : (king_square / 8);
        cellRect.x = boardXPos + file * CELL_WIDTH;
        cellRect.y = boardYPos + rank * CELL_HEIGHT;
        if (king_square % 2)
            SDL_RenderCopy(renderer, BSDangerTexture, NULL, &cellRect);
        else
            SDL_RenderCopy(renderer, WSDangerTexture, NULL, &cellRect);
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
        
        int file = (i % 8);
        int rank = (board.fliped)? (7 - (i / 8)) : (i / 8);
        cellRect.x = boardXPos + file * CELL_WIDTH;
        cellRect.y = boardYPos + rank * CELL_HEIGHT;

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
void RenderAvailableSquare(SDL_Renderer *renderer, Bitboard availableSquare, bool fliped)
{
    if (!availableSquare) return;
    
    for (int i = 0; i < 64; i++) {
        uint64_t mask = 1ULL << i;     // make a mask with only bit i set
        bool sq = (availableSquare & mask) ? 1 : 0;
        
        int file = (i % 8);
        int rank = (fliped)? (7 - (i / 8)) : (i / 8);

        cellRect.x = boardXPos + file * CELL_WIDTH;
        cellRect.y = boardYPos + rank * CELL_HEIGHT;

        if (sq)
            SDL_RenderCopy(renderer, CircleOverlayTexture, NULL, &cellRect);
    }
}
void RenderPromotionMenu(SDL_Renderer *renderer, short promotoin_sq_index)
{
    if (promotoin_sq_index != -1)
        SDL_RenderCopy(renderer, promotionMenuTexture, NULL, NULL);
}
