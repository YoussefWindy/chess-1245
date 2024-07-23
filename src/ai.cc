// src/ai.cc

#include "../include/ai.h"

const Move emptyMove = {{0, 0}, {0, 0}};

AI::AI(Board &b, bool w, int d): boardRef{b}, boardState{b}, colour{w}, difficulty{d} {}

Move AI::think() const {
    boardState = boardRef; // Reset thinking state
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
            return thinkAt4();
    }
}

Move AI::thinkAt1() const {
    int num = (colour ? boardState.whitePieces : boardState.blackPieces).size();
    std::shared_ptr<Piece> piece;
    do {
        piece = (colour ? boardState.whitePieces : boardState.blackPieces)[std::rand() % num];
    } while (!piece->canMove());
    return {piece->getPosn(), piece->getLegalMoves()[std::rand() % piece->getLegalMoves().size()]};
}

Move AI::thinkAt2() const {
    checkingMoves = calculateCheckingMoves(false);
    capturingMoves = calculateCapturingMoves();
    if (checkingMoves.empty() && capturingMoves.empty()) {
        return thinkAt1();
    }
    Move maxWorth = emptyMove;
    if (!checkingMoves.empty() && !capturingMoves.empty()) {
        for (auto it = capturingMoves.begin(); it != capturingMoves.end(); it++) {
            bool common = false;
            for (auto m: checkingMoves) if (m == *it) common = true;
            if (!common) {
                capturingMoves.erase(it);
                it--;
            }
        }
    }
    if (!capturingMoves.empty()) {
        maxWorth = capturingMoves.front();
        for (unsigned int i = 1; i < capturingMoves.size(); i++) {
            int value = 0;
            if (boardState[capturingMoves[i].newPos]->getIsProtected()) {
                for (auto piece: colour ? boardState.whitePieces : boardState.blackPieces) {
                    if (piece->canMoveTo(capturingMoves[i].newPos)) {
                        value = std::max(value, boardState[capturingMoves[i].newPos]->getValue() - piece->getValue());
                    }
                }
            } else {
                value = std::max(value, boardState[capturingMoves[i].newPos]->getValue());
            }
            if (value > boardState[maxWorth.newPos]->getValue()) {
                maxWorth = capturingMoves[i];
            }
        }
    } else {
        maxWorth = checkingMoves.front();
        for (auto m: checkingMoves) {
            if (boardState[m.oldPos]->getValue() < boardState[maxWorth.oldPos]->getValue()) {
                maxWorth = m;
            }
        }
    }
    return maxWorth;
}

Move AI::thinkAt3() const {
    Move tryCheck = thinkAt2();
    Move maxWorth = emptyMove;
    threatenedPosns = calculateThreatenedPosns();
    if (!checkingMoves.empty() || threatenedPosns.empty()) return tryCheck;
    // Fill in
    if ((!capturingMoves.empty() || maxWorth == emptyMove) && (capturingMoves.empty() || boardState[maxWorth.oldPos]->getValue() < boardState[tryCheck.newPos]->getValue())) {
        return tryCheck;
    } else {
        return maxWorth;
    }
}

Move AI::thinkAt4() const {
    // Fill in
    return emptyMove;
}

const std::vector<Posn> AI::calculateThreatenedPosns() const {
    std::vector<Posn> tmp;
    for (auto piece: !colour ? boardState.whitePieces : boardState.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            if (boardState[posn] && boardState[posn]->canMove()) {
                tmp.emplace_back(posn);
            }
        }
    }
    return tmp;
}

const std::vector<Move> AI::calculateCapturingMoves() const {
    std::vector<Move> tmp;
    for (auto piece: colour ? boardState.whitePieces : boardState.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            if (boardState[posn]) {
                tmp.emplace_back(piece->getPosn(), posn);
            }
        }
    }
    return tmp;
}

const std::vector<Move> AI::calculateCheckingMoves(bool checkmate) const {
    std::vector<Move> tmp;
    for (auto piece: colour ? boardState.whitePieces : boardState.blackPieces) {
        for (Posn posn: piece->getLegalMoves()) {
            boardState.movePiece({piece->getPosn(), posn});
            if ((!checkmate && boardState.check((colour ? boardState.whiteKing : boardState.blackKing)->getPosn(), colour))
              || (checkmate && boardState.checkmate(colour))) {
                tmp.emplace_back(boardState.log.back());
            }
            boardState.undoMoves();
        }
    }
    return tmp;
}
