#include <vector>
#include "position.h"
#include "move.h"
#include "bitboard.h"
#include "movegen/pawn.h"

void generateWhitePawnMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard pawns = pos.whitePawns;

    Bitboard empty   = ~(pos.whitePieces() | pos.blackPieces());
    Bitboard enemies = pos.blackPieces();

    Bitboard p = pawns;

    while (p)
    {
        int from = bb::popLSB(p);
        Bitboard fromBB = 1ULL << from;

        // -------------------------
        // single push
        // -------------------------
        Bitboard one = (fromBB << 8) & empty;

        if (one)
        {

            Bitboard targets = (fromBB << 8) & empty;

            while (targets)
            {
                int to = bb::popLSB(targets);

                if (to / 8 == 7)
                {
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
                }
                else
                {
                    moves.push_back({from, to, QUIET, NONE});
                }
            }
            
            // Promotion check
            if (to / 8 == 7)
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    QUEEN
                });

                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    ROOK
                });

                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    BISHOP
                });

                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    KNIGHT
                });
            }
            else
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    QUIET,
                    NONE
                });
            }

            // -------------------------
            // double push (rank 2 only)
            // -------------------------
            if (from / 8 == 1)
            {
                Bitboard two = (fromBB << 16) & empty;
                Bitboard between = (fromBB << 8);

                if (two && (between & empty))
                {
                    int to2 = bb::lsb(two);
                    moves.push_back({(uint8_t)from, (uint8_t)to2, DOUBLE_PUSH, NONE});
                }
            }
        }

        // -------------------------
        // captures
        // -------------------------

        // left capture (up-left)
        Bitboard capLeft = (fromBB << 7) & enemies & ~FILE_H;
        if (capLeft)
        {
            Bitboard targets = (fromBB << 8) & empty;
                    
            while (targets)
            {
                int to = bb::popLSB(targets);

                if (to / 8 == 7)
                {
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
                }
                else
                {
                    moves.push_back({from, to, QUIET, NONE});
                }
            }

            // Promotion check
            if (to / 8 == 7)
            {
                moves.push_back({ 
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    QUEEN
                });
                
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    ROOK
                });
                
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    BISHOP
                });
                
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    KNIGHT
                });
            }
            else
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    CAPTURE,
                    NONE
                });
            }

        }

        // right capture (up-right)
        Bitboard capRight = (fromBB << 9) & enemies & ~FILE_A;
        if (capRight)
        {
            Bitboard targets = (fromBB << 8) & empty;
                    
            while (targets)
            {
                int to = bb::popLSB(targets);

                if (to / 8 == 7)
                {
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, QUEEN});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, ROOK});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, BISHOP});
                    moves.push_back(Move{(uint8_t)from, (uint8_t)to, PROMOTION, KNIGHT});
                }
                else
                {
                    moves.push_back({from, to, QUIET, NONE});
                }
            }

            // Promotion check
            if (to / 8 == 7)
            {
                moves.push_back({ 
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    QUEEN
                });
                
                moves.push_back({ 
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    ROOK
                }); 
                
                moves.push_back({ 
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    BISHOP
                });
                
                moves.push_back({ 
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    KNIGHT
                });
            }   
            else
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    CAPTURE,
                    NONE
                });  
            }
        }

        if (pos.enPassantSquare != -1)
        {
            if (from + 7 == pos.enPassantSquare &&
                (fromBB & ~FILE_A))
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)pos.enPassantSquare,
                    EN_PASSANT,
                    NONE
                });
            }

            if (from + 9 == pos.enPassantSquare &&
                (fromBB & ~FILE_H))
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)pos.enPassantSquare,
                    EN_PASSANT,
                    NONE
                });
            }
        }
    }
}

void generateBlackPawnMoves(const Position& pos, std::vector<Move>& moves)
{
    Bitboard pawns = pos.blackPawns;

    Bitboard empty   = ~(pos.whitePieces() | pos.blackPieces());
    Bitboard enemies = pos.whitePieces();

    Bitboard p = pawns;

    while (p)
    {
        int from = bb::popLSB(p);
        Bitboard fromBB = 1ULL << from;

        // -------------------------
        // single push
        // -------------------------
        Bitboard one = (fromBB >> 8) & empty;

        if (one)
        {
            int to = bb::lsb(one);
            
            // Promotion check
            if (to / 8 == 0)
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    QUEEN
                });
         
                moves.push_back({
                    (uint8_t)from, 
                    (uint8_t)to,
                    PROMOTION,
                    ROOK
                });
            
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to, 
                    PROMOTION,
                    BISHOP
                });
                    
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to, 
                    PROMOTION,
                    KNIGHT
                });
            }
            else   
            {       
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    CAPTURE,  
                    NONE  
                });
            }  

            // -------------------------
            // double push (rank 7 only)
            // -------------------------
            if (from / 8 == 6)
            {
                Bitboard two = (fromBB >> 16) & empty;
                Bitboard between = (fromBB >> 8);

                if (two && (between & empty))
                {
                    int to2 = bb::lsb(two);
                    moves.push_back({(uint8_t)from, (uint8_t)to2, DOUBLE_PUSH, NONE});
                }
            }
        }

        // -------------------------
        // captures
        // -------------------------

        // down-left
        Bitboard capLeft = (fromBB >> 9) & enemies & ~FILE_H;
        if (capLeft)
        {
            int to = bb::lsb(capLeft);
            
            // Promotion check
            if (to / 8 == 0)
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    QUEEN
                });
         
                moves.push_back({
                    (uint8_t)from, 
                    (uint8_t)to,
                    PROMOTION,
                    ROOK
                });
            
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to, 
                    PROMOTION,
                    BISHOP
                });
                    
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to, 
                    PROMOTION,
                    KNIGHT
                });
            }
            else   
            {       
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    CAPTURE,  
                    NONE  
                });
            }  
        }

        // down-right
        Bitboard capRight = (fromBB >> 7) & enemies & ~FILE_A;
        if (capRight)
        {
            int to = bb::lsb(capRight);

            // Promotion check
            if (to / 8 == 0)
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    PROMOTION,
                    QUEEN
                });
         
                moves.push_back({
                    (uint8_t)from, 
                    (uint8_t)to,
                    PROMOTION,
                    ROOK
                });
            
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to, 
                    PROMOTION,
                    BISHOP
                });
                    
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to, 
                    PROMOTION,
                    KNIGHT
                });
            }
            else   
            {       
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)to,
                    CAPTURE,  
                    NONE  
                });
            }  
        }

        if (pos.enPassantSquare != -1)
        {
            if (from - 7 == pos.enPassantSquare &&
                (fromBB & ~FILE_H))
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)pos.enPassantSquare,
                    EN_PASSANT,
                    NONE
                });
            }

            if (from - 9 == pos.enPassantSquare &&
                (fromBB & ~FILE_A))
            {
                moves.push_back({
                    (uint8_t)from,
                    (uint8_t)pos.enPassantSquare,
                    EN_PASSANT,
                    NONE
                });
            }
        }
    }
}
