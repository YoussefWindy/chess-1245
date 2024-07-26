// src/ai.cc

#include "../include/ai.h"
#include <ctime>

const Move emptyMove = {{0, 0}, {0, 0}};

AI::AI(Board &b, bool w, int d): board{b}, colour{w}, difficulty{d} {
    std::srand(std::time(nullptr));
}

Move AI::think() const {
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
    unsigned int num = (colour ? board.whitePieces : board.blackPieces).size();
    std::shared_ptr<Piece> piece;
    do {
        piece = (colour ? board.whitePieces : board.blackPieces)[std::rand() % num];
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
            std::cerr << char('a' + it->oldPos.x) << it->oldPos.y + 1 << "-->" << char('a' + it->newPos.x) << it->newPos.y + 1 << "is " << (common ? "" : "NOT") << " common" << std::endl;
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
            if (!board[capturingMoves[i].newPos]) {
                value = std::max(value, 2);
            } else if (board[capturingMoves[i].newPos]->getIsProtected()) {
                for (auto piece: colour ? board.whitePieces : board.blackPieces) {
                    if (piece->canMoveTo(capturingMoves[i].newPos)) {
                        value = std::max(value, board[capturingMoves[i].newPos]->getValue() - piece->getValue());
                        std::cerr << "new value: " << value << std::endl;
                    }
                }
            } else {
                value = std::max(value, board[capturingMoves[i].newPos]->getValue());
            }
            std::cerr << "value: " << value << std::endl;
            if (value > (board[maxWorth.newPos] ? board[maxWorth.newPos]->getValue() : 2)) {
                std::cerr << "yep" << std::endl;
                maxWorth = capturingMoves[i];
            }
        }
    } else {
        maxWorth = checkingMoves.front();
        for (auto m: checkingMoves) {
            if (board[m.oldPos]->getValue() < board[maxWorth.oldPos]->getValue()) {
                maxWorth = m;
            }
        }
    }
    std::cerr << "maxWorth: " << char('a' + maxWorth.oldPos.x) << maxWorth.oldPos.y + 1 << "-->" << char('a' + maxWorth.newPos.x) << maxWorth.newPos.y + 1 << std::endl;
    if (board[maxWorth.oldPos]->getName() == (colour ? 'P' : 'p') && maxWorth.newPos.y == (colour ? HEIGHT - 1 : 0)) {
        maxWorth.promotion = 4;
    }
    return maxWorth;
}

Move AI::thinkAt3() const {
    threatenedPosns = calculateThreatenedPosns();
    Move tryCheck = thinkAt2();
    Move maxWorth = emptyMove;
    if (threatenedPosns.empty()) return tryCheck;
    std::cerr << "Heyy we're actually trying to be smart I guess" << std::endl;
    bool checks = !checkingMoves.empty();
    if (!capturingMoves.empty()) {
        std::cerr << 1 << std::endl;
        for (auto it = capturingMoves.begin(); it != capturingMoves.end(); it++) {
            if (board[(*it).newPos]->getValue() <= board[(*it).oldPos]->getValue() && board.check((*it).newPos, colour)) {
                capturingMoves.erase(it);
                it--;
            }
        }
    } else if (checks) {
        std::cerr << 2 << std::endl;
        for (auto it = checkingMoves.begin(); it != checkingMoves.end(); it++) {
            bool common = false;
            for (auto p: threatenedPosns) {
                if ((*it).oldPos == p) {
                    common = true;
                    break;
                }
            }
            if (!common || board.check((*it).newPos, colour)) {
                checkingMoves.erase(it);
                it--;
            }
        }
    }
    if (checkingMoves.empty() && capturingMoves.empty()) {
        std::cerr << 3 << std::endl;
        maxWorth = {threatenedPosns.front(), board[threatenedPosns.front()]->legalMoves[std::rand() % board[threatenedPosns.front()]->legalMoves.size()]};
        for (auto it = threatenedPosns.begin(); it != threatenedPosns.end(); it++) {
            std::cerr << "value: " << board[*it]->getValue() << std::endl;
            if (board[*it]->getValue() > board[maxWorth.oldPos]->getValue()) {
                std::cerr << "yep" << std::endl;
                maxWorth = {*it, board[threatenedPosns.front()]->legalMoves[std::rand() % board[threatenedPosns.front()]->legalMoves.size()]};
            }
        }
    }
    if (maxWorth == emptyMove) {
        std::cerr << "nvm we're playing dum lol" << std::endl;
        return tryCheck;
    }
    std::cerr << "maxWorth: " << char('a' + maxWorth.oldPos.x) << maxWorth.oldPos.y + 1 << "-->" << char('a' + maxWorth.newPos.x) << maxWorth.newPos.y + 1 << std::endl;
    if (board[maxWorth.oldPos]->getName() == (colour ? 'P' : 'p') && maxWorth.newPos.y == (colour ? HEIGHT - 1 : 0)) {
        maxWorth.promotion = 4;
    }
    return maxWorth;
}

Move AI::thinkAt4() const {
    // Priority 1: checkmate
    std::cerr << "start level 4 checking for checkmates" << std::endl;
    checkingMoves = calculateCheckingMoves(true);
    if (!checkingMoves.empty()) {
        std::cerr << "CHECKMATE BABY WHOOO" << std::endl;
        return checkingMoves.front();
    }
    // Priority 2: promote
    for (auto piece: colour ? board.whitePieces : board.blackPieces) {
        if (piece->getValue() == 1) {
            std::shared_ptr<Pawn> tmp = std::static_pointer_cast<Pawn>(piece);
            if (tmp->canPromote()) {
                std::cerr << "we're PROMOTING" << std::endl;
                return {piece->getPosn(), piece->legalMoves.back(), 4};
            }
        }
    }
    // Priority 3: castle
    for (auto posn: colour ? board.whiteKing->legalMoves : board.blackKing->legalMoves) {
        if (posn.x - (colour ? board.whiteKing->getX() : board.blackKing->getX()) == 2) { // always check kingside castle first :)
            return {colour ? board.whiteKing->getPosn() : board.blackKing->getPosn(), posn};
        } else if ((colour ? board.whiteKing->getX() : board.blackKing->getX()) - posn.x == 2) { // queenside
            return {colour ? board.whiteKing->getPosn() : board.blackKing->getPosn(), posn};
        }
    }
    // Priority 4: en passant
    for (auto piece: colour ? board.whitePieces : board.blackPieces) {
        if (piece->getValue() == 1) {
            for (auto posn: piece->legalMoves) {
                if (abs(posn.x - piece->getX()) == 1 && !board[posn]) {
                    return {piece->getPosn(), posn};
                }
            }
        }
    }
    // Priority 5: level 3 move
    return thinkAt3();
}

const std::vector<Posn> AI::calculateThreatenedPosns() const {
    std::vector<Posn> tmp;
    for (auto piece: !colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->legalMoves) {
            if (board[posn] && board[posn]->canMove()) tmp.emplace_back(posn);
        }
    }
    return tmp;
}

const std::vector<Move> AI::calculateCapturingMoves() const {
    std::vector<Move> tmp;
    for (auto piece: colour ? board.whitePieces : board.blackPieces) {
        for (Posn posn: piece->legalMoves) {
            std::cerr << char('a' + piece->getX()) << piece->getY() + 1 << "-->" << char('a' + posn.x) << posn.y + 1 << std::endl;
            if (board[posn] || (piece->getName() == (colour ? 'P' : 'p') && abs(piece->getX() - posn.x))) {
                if (board[posn] && board[posn]->getColour() == colour) std::cerr << "HEYHEYHEYHEY STOP" << std::endl;
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
    Board boardCopy = board; // Create board for messing around with
    boardCopy.runCalculations();
    for (unsigned int i = 0; i < (colour ? boardCopy.whitePieces : boardCopy.blackPieces).size(); i++) { // can't use ranged for
        auto piece = (colour ? boardCopy.whitePieces : boardCopy.blackPieces)[i]; // loops here because, due to the nature of this
        for (unsigned int j = 0; j < piece->legalMoves.size(); j++) { // function, iterators for the pieces vectors are not consistent
            Posn posn = piece->legalMoves[j];
            std::cerr << char('a' + piece->getX()) << piece->getY() + 1 << "-->" << char('a' + posn.x) << posn.y + 1 << std::endl;
            unsigned int promotion = 0;
            if (piece->getName() == (colour ? 'P' : 'p')) {
                std::shared_ptr<Pawn> tmp = std::static_pointer_cast<Pawn>(piece);
                promotion = (tmp->canPromote() ? 4 : 0);
            }
            boardCopy.movePiece({piece->getPosn(), posn, promotion});
            int result = boardCopy.runCalculations();
            if ((!checkmate && result == 1) || result == 2) {
                std::cerr << "interesting thing here" << std::endl;
                tmp.emplace_back(boardCopy.log.back());
            }
            std::cerr << "undo" << std::endl;
            boardCopy.undoMoves();
        }
    }
    return tmp;
}
