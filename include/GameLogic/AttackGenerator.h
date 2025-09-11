#ifndef PIECE_ATTACK_GENERATOR_H
#define PIECE_ATTACK_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "GameLogic/Board.h"

Bitboard pawns_attack(Players player, Board board);
Bitboard rooks_attack(Players player, Board board);
Bitboard bishops_attack(Players player, Board board);
Bitboard knights_attack(Players player, Board board);
Bitboard queens_attack(Players player, Board board);
Bitboard king_attack(Players player, Board board);

#endif