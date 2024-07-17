// src/ai.cc

#include "../include/ai.h"

Move empty = {{0, 0}, {0, 0}};

AI::AI(Board &b, bool w, int d): boardRef{b}, board{b},
  colour{w}, difficulty{d}, noLevel2{false}, noLevel3{false} {}

Move AI::think() const {
    noLevel2 = noLevel3 = false;
    board = boardRef; // Reset thinking state
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
    noLevel2 = true;
    int num = (colour ? board.whitePieces : board.blackPieces).size();
    std::shared_ptr<Piece> piece;
    do {
        piece = piece = (colour ? board.whitePieces : board.blackPieces)[std::rand() % num];
    } while (piece->getLegalMoves().empty());
    return {piece->getPosn(), piece->getLegalMoves()[std::rand() % piece->getLegalMoves().size()]};
}

Move AI::thinkAt2() const {
    Move maxWorth = empty;
    for (auto piece: colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            board.movePiece({piece->getPosn(), posn});
            if (!checkFound && board.deadPieces.size() > boardRef.deadPieces.size() &&
              board.deadPieces.back()->getValue() > boardRef[maxWorth.newPos]->getValue()) {
                maxWorth = board.log.back();
            }
            if (colour ? board.check(board.blackKing->getPosn(), false) : board.check(board.whiteKing->getPosn(), true)
              && (maxWorth == empty || board.deadPieces.size() > boardRef.deadPieces.size()
              || boardRef[maxWorth.newPos]->getValue() > board[posn]->getValue())) {
                maxWorth = board.log.back();
                checkFound = true;
            }
            board.undoMoves(1);
        }
    }
    if (maxWorth == empty) {
        noLevel2 = true;
        return thinkAt1();
    } else {
        return maxWorth;
    }
}

Move AI::thinkAt3() const {
    Move tryCapture = thinkAt2();
    if (checkFound) return tryCapture;
    Move maxWorth = empty;
    for (auto piece: !colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            std::shared_ptr<Piece> tmp = board[posn];
            if (tmp && tmp->canMove() && (maxWorth == empty ||
              board[maxWorth.oldPos]->getValue() < tmp->getValue())) {
                maxWorth = {posn, tmp->getLegalMoves()[std::rand() % tmp->getLegalMoves().size()]};
            }
        }
    }
    if (noLevel2 && !(maxWorth == empty)) {
        return maxWorth;
    } else if ((noLevel2 && maxWorth == empty) || board[maxWorth.oldPos]->getValue() < board[tryCapture.newPos]->getValue()) {
        noLevel3 = noLevel2;
        return tryCapture;
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

const std::vector<Move> AI::calculateCheckingMoves() const {
    std::vector<Move> tmp;
    for (auto piece: colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            board.movePiece({piece->getPosn(), posn});
            if (board.check((colour ? board.whiteKing : board.blackKing)->getPosn(), colour)) {
                tmp.emplace_back(board.log.back());
            }
            board.undoMoves(1);
        }
    }
    return tmp;
}

const std::vector<Move> AI::calculateCheckmatingMoves() const {
    std::vector<Move> tmp;
    for (auto piece: colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            board.movePiece({piece->getPosn(), posn});
            if (colour ? board.checkmate(colour) : board.checkmate(colour)) {
                tmp.emplace_back(board.log.back());
            }
            board.undoMoves(1);
        }
    }
    return tmp;
}
