#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "Utils.h"
#include "Position.h"

class MoveGenerator {
    public:
        MoveGenerator();
        Move legalMoves[218];
        int generateMoves(Position &position);

    private:
        void addMove(int& i, U8 from, U8 to, MoveType type, Piece captured, PieceType promotion);
        // PAWN MOVES
        // Pawn pushes
        // https://www.chessprogramming.org/Pawn_Pushes_(Bitboards)
        U64 pawnsAbleToPush(U64 pawns, U64 empty, Color color);
        U64 pawnsAbleToDoublePush(U64 pawns, U64 empty, Color color);
        void generatePawnPush(U64 pushes, int& i, Color color, bool isDoublePush);
        void generatePawnPushes(Position& position, int& i);
        // Pawn attacks
        // https://www.chessprogramming.org/Pawn_Attacks_(Bitboards) (didn't follow strictly but same idea)
        U64 arrPawnAttacks[2][64];
        void precalculatePawnAttacks();
        void generatePawnCaptures(Position& position, int& i);

        // KNIGHT MOVES
        // https://www.chessprogramming.org/Knight_Pattern
        U64 arrKnightMoves[64];
        void precalculateKnightMoves();
        void generateKnightMoves(Position &position, int &i);

        // KING MOVES, TODO: CASTLING
        // https://www.chessprogramming.org/King_Pattern
        U64 arrKingMoves[64];
        void precalculateKingMoves();
        void generateKingMoves(Position &position, int &i);

        // SLIDING PIECES
        // https://www.chessprogramming.org/Sliding_Piece_Attacks
        // https://www.chessprogramming.org/Looking_for_Magics
        // Mask relevant occupancy bits to form a key
        U64 arrBishopOccupancyMask[64];
        U64 arrRookOccupancyMask[64];
        void generateRelevantOccupancyMasks();
        U64 bishopOccupancyMask(int square);
        U64 rookOccupancyMask(int square);
        // Generated by counting bits of each square from the "generaterRelevantOccupancyMasks" method
        int arrBishopRelevantBits[64] = {
            6, 5, 5, 5, 5, 5, 5, 6,
            5, 5, 5, 5, 5, 5, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 5, 5, 5, 5, 5, 5,
            6, 5, 5, 5, 5, 5, 5, 6
        };
        int arrRookRelevantBits[64] = {
            12, 11, 11, 11, 11, 11, 11, 12,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            12, 11, 11, 11, 11, 11, 11, 12            
        };
        // Bishop and Rook attacks on the fly (slow method, only for magic number generation)
        U64 bishopMovesMapOnTheFly(int square, U64 occupancy);
        U64 rookMovesMapOnTheFly(int square, U64 occupancy);
        // Generating occupancy variations for bishop and rook masks
        // It's essentially counting in binary from 0 to 2^relevantBits in the mask
        U64 getOccupancyByIndex(int index, U64 mask);
        // Generating magic numbers!
        U64 findMagicNumber(int square, bool isBishop);
        void precalculateMagicNumbers();
        // Generated by "precalculateMagicNumbers" method
        U64 arrBishopMagicNumbers[64] = {
            18018831494946945, 1161957299758109698, 581004108297715712, 1442387733975697424, 10377459161226092552, 74943950047166472, 1153089248985907200, 9571253153235970, 
            2379171673321587200, 9376777030901888, 36319070387572800, 2594258241835794440, 39600135362576, 4755803578937376768, 4611695916218335232, 71485473563168, 
            2273807776088576, 145111655687651840, 13909367792836034632, 2452210066917441609, 13840689859076820992, 505564247108882438, 704289039266048, 7205830949362484226, 
            5139394927531008, 9800405085229940872, 1128100008068360, 433480260294566184, 9223517722212573188, 4613940017402761744, 2258397061851394, 2450108308577714304, 
            1207545585876337440, 9230128570168254984, 4611831227011301444, 35218732155140, 1588648084272317448, 585476761073566725, 144647356004172033, 9513894347142079015, 
            2254067623530560, 288306586856933379, 653587108402299904, 2254411472578560, 16140905497171788288, 2407261964209225760, 2306459015317561856, 1801723542132229392, 
            1153089248985907200, 72202734002110470, 3463270347938758784, 9223372155033766017, 9224568580657447026, 212240120741888, 1135830006792449, 1161957299758109698, 
            9571253153235970, 71485473563168, 292734049901137924, 4648277769703392320, 4615204601954697728, 9331459804491448576, 2379171673321587200, 18018831494946945 
        };
        U64 arrRookMagicNumbers[64] = {
            9979994641325359136, 882723120224935936, 72075323679705152, 1188954768392323200, 4791869588088424448, 324270168842436736, 1188950851398402560, 4755801768221344804, 
            563089556652544, 563089556652544, 140806216228864, 144396731772576000, 1027102258820548608, 1162491722677436928, 148900266975035524, 6896206739406976, 
            36037867991990336, 2350881479393804320, 36100265545304321, 2342726127035884288, 141287311280128, 577587204113826304, 9799837187239313672, 2199042162756, 
            4620833957320081408, 18014959004811330, 142945103659024, 144132799589777536, 216177182308302976, 577587204113826304, 9223374253066817537, 38421892668867585, 
            7241792736402079809, 4647855827821010944, 140806216228864, 1519964942965409922, 1027102258820548608, 1157988122940670984, 90090168916140545, 568484052336724, 
            648659908469817361, 45036271153741952, 9570220077350979, 282712196055048, 4398180761728, 1224983496724873344, 18031997171793928, 9223372468515897356, 
            324580780368527616, 563089556652544, 4615204601954697728, 180179221275017472, 216177182308302976, 1224983496724873344, 4611687702089171968, 4832503141951881344, 
            4621959876687437889, 38421407337562113, 4616330425369894922, 9227911937680998657, 10520971817022656514, 9228438896277262370, 2305844177478877316, 282597190173698
        };
        // Bishop and Rook moves
        U64 arrBishopMoves[64][512];
        U64 arrRookMoves[64][4096];
        void precalculateSliderMoves(bool isBishop);
        void generateSliderMoves(Position &position, int &i, bool isBishop);
};

#endif