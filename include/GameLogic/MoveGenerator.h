#ifndef PIECE_MOVE_GENERATOR_H
#define PIECE_MOVE_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "GameLogic/Board.h"

Bitboard pawn_moves(Players player, Board board, short square);
Bitboard rook_moves(Players player, Board board, short square);
Bitboard bishop_moves(Players player, Board board, short square);
Bitboard knight_moves(Players player, Board board, short square);
Bitboard queen_moves(Players player, Board board, short square);
Bitboard king_moves(Players player, Board board, short square);

#endif