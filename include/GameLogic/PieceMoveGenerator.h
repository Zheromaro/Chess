#ifndef PIECE_MOVE_GENERATOR_H
#define PIECE_MOVE_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "GameLogic/Board.h"
#include "GameLogic/GameData.h"

Bitboard bpawn_moves(Players player, Board board, uint64_t square);
Bitboard wpawn_moves(Players player, Board board, uint64_t square);
Bitboard rook_moves(Players player, Board board, uint64_t square);
Bitboard bishop_moves(Players player, Board board, uint64_t square);
Bitboard knight_moves(Players player, Board board, uint64_t square);
Bitboard queen_moves(Players player, Board board, uint64_t square);
Bitboard king_moves(Players player, Board board, uint64_t square);

#endif