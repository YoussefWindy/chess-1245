// src/main.cc

#include "../include/ai.h"

using namespace std;

Board defaultBoard;

int parsePlayer(string &s) {
	if (s == "human") return 0;
	else if (s == "computer[1]") return 1;
	else if (s == "computer[2]") return 2;
	else if (s == "computer[3]") return 3;
	else if (s == "computer[4]") return 4;
	else if (s == "computer[5]") return 5;
	else return -1;
}

bool verifyPiece(char c) {
	return c == 'b' || c == 'B' || c == 'r' || c == 'R' || c == 'n' || c == 'N'
		|| c == 'k' || c == 'K' || c == 'q' || c == 'Q' || c == 'p' || c == 'P';
}

template <typename T> void Board::addPiece(bool colour, const Posn &posn) {
	board[posn.x][posn.y] = make_shared<T>(colour, posn);
	(colour ? whitePieces : blackPieces).emplace_back(board[posn.x][posn.y]);
	if (is_same<T, King>::value) {
		(colour ? whiteKing : blackKing) = static_pointer_cast<King>(board[posn.x][posn.y]);
	}
}

template <typename T>
void Board::promote(bool colour, const Posn &posn) {
	deadPieces.emplace_back(board[posn.x][posn.y]);
	if (colour) {
		for (auto it = whitePieces.begin(); it != whitePieces.end(); it++) {
			if (it->get()->getPosn() == posn) {
				whitePieces.erase(it);
				break;
			}
		}
	} else {
		for (auto it = blackPieces.begin(); it != blackPieces.end(); it++) {
			if (it->get()->getPosn() == posn) {
				blackPieces.erase(it);
				break;
			}
		}
	}
	addPiece<T>(colour, {posn.x, posn.y + colour ? 1 : -1});
}

int main() {
	double whiteWins = 0, blackWins = 0;
	string command, arg1, arg2;
	bool gameActive = false, defaultWhiteTurn = true, whiteTurn;
	Board board;
	AI *whiteAI, *blackAI;

	// Initial default board
	// White pieces
	defaultBoard.addPiece<Rook>(true, {0, 0});
	defaultBoard.addPiece<Knight>(true, {0, 1});
	defaultBoard.addPiece<Bishop>(true, {0, 2});
	defaultBoard.addPiece<Queen>(true, {0, 3});
	defaultBoard.addPiece<King>(true, {0, 4});
	defaultBoard.addPiece<Bishop>(true, {0, 5});
	defaultBoard.addPiece<Knight>(true, {0, 6});
	defaultBoard.addPiece<Rook>(true, {0, 7});

	// Black pieces
	defaultBoard.addPiece<Rook>(false, {7, 0});
	defaultBoard.addPiece<Knight>(false, {7, 1});
	defaultBoard.addPiece<Bishop>(false, {7, 2});
	defaultBoard.addPiece<Queen>(false, {7, 3});
	defaultBoard.addPiece<King>(false, {7, 4});
	defaultBoard.addPiece<Bishop>(false, {7, 5});
	defaultBoard.addPiece<Knight>(false, {7, 6});
	defaultBoard.addPiece<Rook>(false, {7, 7});

	cout << board << endl << endl;

	board.movePiece({{6, 4}, {4, 4}});

	cout << board << endl << endl;

	board.movePiece({{7, 7}, {6, 7}});

	cout << board << endl;
  
	// Pawns
	for (unsigned int i = 0; i < WIDTH; i++) {
		defaultBoard.addPiece<Pawn>(true, {1, i});
		defaultBoard.addPiece<Pawn>(false, {6, i});
	}

	while (cin >> command) {
		cout << board << endl;

		if (command == "game") {
			if (gameActive) {
				cerr << "Game is already active." << endl;
				continue;
			}
			cin >> arg1 >> arg2;
			int p1 = parsePlayer(arg1), p2 = parsePlayer(arg2);
			if (p1 < 0 || p2 < 0) continue;
			whiteAI = p1 ? new AI{board, true, p1} : nullptr;
			blackAI = p2 ? new AI{board, false, p2} : nullptr;
			board = defaultBoard;
			whiteTurn = defaultWhiteTurn;
			gameActive = true;
		} else if (command == "resign") {
			if (!gameActive) {
				cerr << "Game is not active." << endl;
				continue;
			}
			// stuff
			gameActive = false;
		} else if (command == "move") {
			if (!gameActive) {
				cerr << "Game is not active." << endl;
				continue;
			}
			if (whiteTurn && whiteAI) {
				board.movePiece(whiteAI->think());
			} else if (!whiteTurn && blackAI) {
				board.movePiece(blackAI->think());
			} else {
				cin >> arg1 >> arg2;
				try {
					Posn start{arg1}, end{arg2};
					if (board[start] && (board[start]->getName() == 'P' || board[start]->getName() == 'p')) {
						shared_ptr<Pawn> tmp = static_pointer_cast<Pawn>(board[start]);
						if (tmp->canPromote() && end.y == (start.y + tmp->getColour() ? 1 : -1)) {
							char piece;
							cin >> piece;
							bool white = 'B' <= piece && piece <= 'R';
							piece -= white ? ('A' - 'a') : 0;
							switch (piece) {
								case 'n':
									board.promote<Knight>(white, tmp->getPosn());
									break;
								case 'b':
									board.promote<Bishop>(white, tmp->getPosn());
									break;
								case 'r':
									board.promote<Rook>(white, tmp->getPosn());
									break;
								case 'q':
									board.promote<Queen>(white, tmp->getPosn());
									break;
								default:
									cerr << "Please input a valid piece type to promote into." << endl;
									whiteTurn = !whiteTurn;
							}
						}
					} else {
						board.movePiece({start, end});
					}
				} catch (BadPosn &e) {
					cerr << "Please input valid board coordinates." << endl;
					continue;
				} catch (BadMove &e) {
					cerr << "Please input a valid move action." << endl;
					continue;
				}
			}
			whiteTurn = !whiteTurn;
		} else if (command == "undo") {
			int num = 0;
			if (!(cin >> num)) continue;
			bool confirm = false;
			while (cin >> arg1) {
				cout << "Are you SURE you want to undo "
					<< num << " moves? (y/n): ";
				if (arg1 == "y" || arg1 == "Y") {
					confirm = true;
					break;
				} else if (arg1 == "n" || arg1 == "N") {
					break;
				}
				cout << endl << "Please input \"y\" or \"n\"." << endl;
			}
			if (confirm) {
				board.undoMoves(num);
			}
		} else if (command == "setup") {
			if (gameActive) {
				cerr << "Game is already active." << endl;
				continue;
			}
			while (cin >> command) {
				if (command == "+") { // add a piece to the board
					char piece;
					cin >> piece >> arg1;
					if (!verifyPiece(piece)) {
						cerr << "Please input a valid piece name." << endl;
						continue;
					}
					try {
						Posn p{arg1};
						bool white = 'B' <= piece && piece <= 'R';
						piece -= white ? ('A' - 'a') : 0;
						switch (piece) {
							case 'p':
								board.addPiece<Pawn>(white, p);
								break;
							case 'n':
								board.addPiece<Knight>(white, p);
								break;
							case 'b':
								board.addPiece<Bishop>(white, p);
								break;
							case 'r':
								board.addPiece<Rook>(white, p);
								break;
							case 'q':
								board.addPiece<Queen>(white, p);
								break;
							case 'k':
								board.addPiece<King>(white, p);
								break;
						}
					} catch (BadPosn &e) {
						cerr << "Please input valid board coordinates." << endl;
						continue;
					}
					cout << defaultBoard << endl;
				} else if (command == "-") { // remove a piece from the board
					cin >> arg1;
					Posn p{arg1};
					if (!p.validate()) {
						cerr << "Please input valid board coordinates." << endl;
						continue;
					}
					defaultBoard.removePiece(p);
					cout << defaultBoard << endl;
				} else if (command == "=") { // change default starting colour
					cin >> arg1;
					if (arg1 == "white") {
						defaultWhiteTurn = true;
						cout << "White will now have the first turn." << endl;
					} else if (arg1 == "black") {
						defaultWhiteTurn = false;
						cout << "Black will now have the first turn." << endl;
					} else {
						cerr << "Please input a valid colour." << endl;
					}
				} else if (command == "done" && defaultBoard.validate()) { // valid board setup
					break;
				} else if (command == "done") { // invalid board setup
					cerr << "Please create a valid chess setup." << endl;
				} else { // invalid command
					cerr << "Please input a valid setup command." << endl;
				} // switch
			} // while
		} else {
			cerr << "Please input a valid command." << endl;
		} // switch
	} // while

	cout << "Final Score:" << endl
		 << "White: " << whiteWins << endl
		 << "Black: " << blackWins << endl;
	return 0;
} // main
