// src/ai.cc

#include "../include/ai.h"
#include <ctime>

const Move emptyMove = {{0, 0}, {0, 0}};

AI::AI(Board &b, bool w, int d): boardRef{b}, boardState{b}, colour{w}, difficulty{d} {
    std::srand(std::time(nullptr));
}

Move AI::think() const {
    boardState = boardRef; // Reset thinking state
    boardState.runCalculations();
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
    unsigned int num = (colour ? boardState.whitePieces : boardState.blackPieces).size();
    std::shared_ptr<Piece> piece;
    do {
        piece = (colour ? boardState.whitePieces : boardState.blackPieces)[std::rand() % num];
        std::cerr << piece->getName() << std::endl;
    } while (!piece->canMove());
    return {piece->getPosn(), piece->legalMoves[std::rand() % piece->legalMoves.size()],
      (piece->getName() == (colour ? 'P' : 'p') && piece->getY() == (colour ? HEIGHT - 2 : 1)) ? 4U : 0U};
}

Move AI::thinkAt2() const {
    std::cerr << "start level 2 checking for checks" << std::endl;
    checkingMoves = calculateCheckingMoves(false);
    std::cerr << "start checkig for capturs" << std::endl;
    capturingMoves = calculateCapturingMoves();
    std::cerr << "yay" << std::endl;
    if (checkingMoves.empty() && capturingMoves.empty()) {
        std::cerr << "we gave up lol" << std::endl;
        return thinkAt1();
    }
    Move maxWorth = emptyMove;
    if (!checkingMoves.empty() && !capturingMoves.empty()) {
        std::cerr << 1 << std::endl;
        for (auto it = capturingMoves.begin(); it != capturingMoves.end(); it++) {
            bool common = false;
            for (auto m: checkingMoves) if (m == *it) common = true;
            std::cerr << char('a' + it->oldPos.x) << it->oldPos.y + 1 << "-->" << char('a' + it->oldPos.x) << it->oldPos.y + 1 << "is " << (common ? "" : "NOT") << " common" << std::endl;
            if (!common) {
                capturingMoves.erase(it);
                it--;
            }
        }
    }
    if (!capturingMoves.empty()) {
        std::cerr << 2 << std::endl;
        maxWorth = capturingMoves.front();
        for (unsigned int i = 1; i < capturingMoves.size(); i++) {
            int value = 0;
            if (!boardState[capturingMoves[i].newPos]) {
                value = std::max(value, 2);
            } else if (boardState[capturingMoves[i].newPos]->getIsProtected()) {
                for (auto piece: colour ? boardState.whitePieces : boardState.blackPieces) {
                    if (piece->canMoveTo(capturingMoves[i].newPos)) {
                        value = std::max(value, boardState[capturingMoves[i].newPos]->getValue() - piece->getValue());
                        std::cerr << "new value: " << value << std::endl;
                    }
                }
            } else {
                value = std::max(value, boardState[capturingMoves[i].newPos]->getValue());
            }
            std::cerr << "value: " << value << std::endl;
            if (value > (boardState[maxWorth.newPos] ? boardState[maxWorth.newPos]->getValue() : 2)) {
                std::cerr << "yep" << std::endl;
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
    std::cerr << "maxWorth: " << char('a' + maxWorth.oldPos.x) << maxWorth.oldPos.y + 1 << "-->" << char('a' + maxWorth.oldPos.x) << maxWorth.oldPos.y + 1 << std::endl;
    if (boardState[maxWorth.oldPos]->getName() == (colour ? 'P' : 'p') && maxWorth.newPos.y == (colour ? HEIGHT - 1 : 0)) {
        maxWorth.promotion = 4;
    }
    return maxWorth;
}

Move AI::thinkAt3() const {
    threatenedPosns = calculateThreatenedPosns();
    Move tryCheck = thinkAt2();
    Move maxWorth = emptyMove;
    if (threatenedPosns.empty()) return tryCheck;
    bool checks = !checkingMoves.empty();
    int value = 0;
    if (checks && !capturingMoves.empty()) {
        for (auto it = capturingMoves.begin(); it != capturingMoves.end(); it++) {
            bool common = false;
            for (auto p: threatenedPosns) if (p == (*it).oldPos) common = true;
            if (!common) {
                capturingMoves.erase(it);
                it--;
            }
        }
    } else {
        for (auto it = checkingMoves.begin(); it != checkingMoves.end(); it++) {
            bool common = false;
            for (auto p: threatenedPosns) if (p == (*it).oldPos) common = true;
            if (!common) {
                checkingMoves.erase(it);
                it--;
            }
        }
    }
    for (auto m: checkingMoves) {

    }
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
        for (Posn posn: piece->legalMoves) {
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
        for (Posn posn: piece->legalMoves) {
            std::cerr << char('a' + piece->getX()) << piece->getY() + 1 << "-->" << char('a' + posn.x) << posn.y + 1 << std::endl;
            if (boardState[posn] || (piece->getName() == (colour ? 'P' : 'p') && abs(piece->getX() - posn.x))) {
                if (boardState[posn] && boardState[posn]->getColour() == colour) std::cerr << "HEYHEYHEYHEY STOP" << std::endl;
                else std::cerr << "^^^^^^^ a good thing" << std::endl;
                tmp.emplace_back(piece->getPosn(), posn);
            }
        }
    }
    std::cerr << "Captures found: " << tmp.size() << std::endl;
    return tmp;
}

const std::vector<Move> AI::calculateCheckingMoves(bool checkmate) const {
    std::vector<Move> tmp;
    for (unsigned int i = 0; i < (colour ? boardState.whitePieces : boardState.blackPieces).size(); i++) { // can't use ranged for
        auto piece = (colour ? boardState.whitePieces : boardState.blackPieces)[i]; // loops here because, due to the nature of this
        for (unsigned int j = 0; j < piece->legalMoves.size(); j++) { // function, iterators for the pieces vectors are not consistent
            Posn posn = piece->legalMoves[j];
            std::cerr << char('a' + piece->getX()) << piece->getY() + 1 << "-->" << char('a' + posn.x) << posn.y + 1 << std::endl;
            unsigned int promotion = 0;
            if (piece->getName() == (colour ? 'P' : 'p')) {
                std::shared_ptr<Pawn> tmp = std::static_pointer_cast<Pawn>(piece);
                promotion = (tmp->canPromote() ? 4 : 0);
            }
            boardState.movePiece({piece->getPosn(), posn, promotion});
            int result = boardState.runCalculations();
            if ((!checkmate && result == 1) || result == 2) {
                std::cerr << "interesting thing here" << std::endl;
                tmp.emplace_back(boardState.log.back());
            }
            std::cerr << "undo" << std::endl;
            boardState.undoMoves();
        }
    }
    return tmp;
}
