// src/ai.cc

#include "../include/ai.h"

Move empty = {{0, 0}, {0, 0}};

AI::AI(Board &b, bool w, int d): boardRef{b}, board{b},
  colour{w}, difficulty{d}, noLevel2{false}, noLevel3{false} {}

Move AI::think() const {
    noLevel2 = noLevel3 = false;
    board = boardRef; // Reset thinking state
    checkingMoves.clear();
    capturingMoves.clear();
    switch (difficulty) {
        case 1:
            return thinkAt1();
        case 2:
            return thinkAt2();
        case 3:
            return thinkAt3();
        default:
            return thinkAtX(difficulty);
    }
}

Move AI::thinkAt1() const {
    int num = (colour ? board.whitePieces : board.blackPieces).size();
    std::shared_ptr<Piece> piece;
    do {
        piece = piece = (colour ? board.whitePieces : board.blackPieces)[std::rand() % num];
    } while (piece->getLegalMoves().empty());
    return {piece->getPosn(), piece->getLegalMoves()[std::rand() % piece->getLegalMoves().size()]};
}

Move AI::thinkAt2() const {
    checkingMoves = calculateCheckingMoves(false);
    capturingMoves = calculateCapturingMoves();
    if (checkingMoves.empty() && capturingMoves.empty()) {
        noLevel2 = true;
        return thinkAt1();
    } else if (checkingMoves.empty()) {
        Move maxWorth = capturingMoves.front();
        for (int i = 1, value = 0; i < capturingMoves.size(); i++, value = 0) {
            if (board[capturingMoves[i].newPos]->getIsProtected()) {
                for (auto piece: colour ? board.whitePieces : board.blackPieces) {
                    if (piece->canMoveTo(capturingMoves[i].newPos)) {
                        value = std::max(value, board[capturingMoves[i].newPos]->getValue() - piece->getValue());
                    }
                }
            } else {
                value = std::max(value, board[capturingMoves[i].newPos]->getValue());
            }
            if (value > board[maxWorth.newPos]->getValue()) {
                maxWorth = capturingMoves[i];
            }
        }
        return maxWorth;
    }
}

Move AI::thinkAt3() const {
    Move tryCheck = thinkAt2();
    if (!checkingMoves.empty()) return tryCheck;
    Move maxWorth = empty;

    if (noLevel2 && !(maxWorth == empty)) {
        return maxWorth;
    } else if ((noLevel2 && maxWorth == empty) || board[maxWorth.oldPos]->getValue() < board[tryCheck.newPos]->getValue()) {
        noLevel3 = noLevel2;
        return tryCheck;
    } else {
        return maxWorth;
    }
}

Move AI::thinkAtX(int x) const {
    // Fill in
    return empty;
}

const std::vector<Posn> AI::calculateThreatenedPosns() const {
    std::vector<Posn> tmp;
    for (auto piece: !colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            if (board[posn] && board[posn]->canMove()) {
                tmp.emplace_back(posn);
            }
        }
    }
    return tmp;
}

const std::vector<Move> AI::calculateCapturingMoves() const {
    std::vector<Move> tmp;
    for (auto piece: colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            if (board[posn]) {
                tmp.emplace_back(piece->getPosn(), posn);
            }
        }
    }
    return tmp;
}

const std::vector<Move> AI::calculateCheckingMoves(bool checkmate) const {
    std::vector<Move> tmp;
    for (auto piece: colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            board.movePiece({piece->getPosn(), posn});
            if ((!checkmate && board.check((colour ? board.whiteKing : board.blackKing)->getPosn(), colour))
              || (checkmate && board.checkmate(colour))) {
                tmp.emplace_back(board.log.back());
            }
            board.undoMoves(1);
        }
    }
    return tmp;
}
