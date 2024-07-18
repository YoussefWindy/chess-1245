// src/main.cc

#include "../include/ai.h"

using namespace std;

int parsePlayer(string &s) {
	if (s == "human") return 0;
	else if (s.substr(0, 9) == "computer[" && s.back() == ']') {
		return stoi(s.substr(9, s.size() - 10));
	}
	else return -1;
}

bool verifyPiece(char c) {
	return c == 'b' || c == 'B' || c == 'r' || c == 'R' || c == 'n' || c == 'N'
		|| c == 'k' || c == 'K' || c == 'q' || c == 'Q' || c == 'p' || c == 'P';
}

int main() {
	double whiteWins = 0, blackWins = 0;
	string command, arg1, arg2;
	bool gameActive = false, defaultWhiteTurn = true, whiteTurn;
	bool text, graphics;
	Board board, defaultBoard;
	unique_ptr<AI> whiteAI, blackAI;
	// Andrew: declare graphical display
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
	// Pawns
	for (unsigned int i = 0; i < WIDTH; i++) {
		defaultBoard.addPiece<Pawn>(true, {1, i});
		defaultBoard.addPiece<Pawn>(false, {6, i});
	}
	cout << "Would you like to use a text-based display a graphical display, or both? (t/g/b): ";
	while (cin >> arg1) {
		if (arg1 == "t" || arg1 == "T") {
			text = true;
			graphics = false;
			cout << "Text-based display selected." << endl << defaultBoard << endl;
			break;
		} else if (arg1 == "g" || arg1 == "G") {
			text = false;
			graphics = true;
			cout << "Graphical display selected." << endl;
			// Andrew: initialize graphical display
			break;
		} else if (arg1 == "b" || arg1 == "B") {
			text = graphics = true;
			cout << "Both displays selected." << endl << defaultBoard << endl;
			// Andrew: initialize graphical display
			break;
		}
		cout << endl << "Please input \"t\" or \"g\"." << endl;
	}
	while (cin >> command) {
		if (gameActive) {
			if (text) cout << board << endl;
			if (graphics) {
				// Andrew: update graphical display
			}
		}
		cout << "Command: ";
		if (command == "game") {
			if (gameActive) {
				cerr << "Game is already active." << endl;
				continue;
			}
			cin >> arg1 >> arg2;
			int p1 = parsePlayer(arg1);
			int p2 = parsePlayer(arg2);
			if (p1 < 0 || p2 < 0) {
				cerr << "Please input either \"human\" or \"computer[1-4]\" for the players" << endl;
				continue;
			}
			whiteAI = p1 ? make_unique<AI>(board, true, p1) : nullptr;
			blackAI = p2 ? make_unique<AI>(board, false, p2) : nullptr;
			board = defaultBoard;
			whiteTurn = defaultWhiteTurn;
			gameActive = true;
			board.runCalculations(whiteTurn);
			cout << "The game has begun!" << endl;
		} else if (command == "resign") {
			if (!gameActive) {
				cerr << "Game is not active." << endl;
				continue;
			}
			(!whiteTurn ? whiteWins : blackWins) += 1;
			cout << (!whiteTurn ? "White" : "Black") << " wins!" << endl;
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
						if (tmp->canPromote()) {
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
						} else {
							board.movePiece({start, end});
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
			switch (board.runCalculations(whiteTurn)) {
				case 0:
					whiteWins += 0.5;
					blackWins += 0.5;
					gameActive = false;
					cout << "Stalemate!" << endl;
					break;
				case 1:
					cout << (whiteTurn ? "White" : "Black") << " is in check." << endl;
					break;
				case 2:
					(whiteTurn ? whiteWins : blackWins) += 1;
					gameActive = false;
					cout << "Checkmate! " << (whiteTurn ? "White" : "Black") << " wins!" << endl;
					break;
			}
			whiteTurn = !whiteTurn;
		} else if (command == "undo") {
			int num = 0;
			if (!(cin >> num)) continue;
			while (cin >> arg1) {
				cout << "Are you SURE you want to undo "
					<< num << " moves? (y/n): ";
				if (arg1 == "y" || arg1 == "Y") {
					whiteTurn = board.undoMoves(num) ? num % 2 ? !whiteTurn : whiteTurn : defaultWhiteTurn;
					break;
				} else if (arg1 == "n" || arg1 == "N") {
					break;
				}
				cout << endl << "Please input \"y\" or \"n\"." << endl;
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
								if (board.hasKing(white)) {
									cerr << "There cannot be two " << (white ? "white" : "black")
										 << " kings on the board at once!!" << endl;
								} else {
									board.addPiece<King>(white, p);
									break;
								}
							default:
								cerr << "Please input a valid piece type." << endl;
						}
					} catch (BadPosn &e) {
						cerr << "Please input valid board coordinates." << endl;
						continue;
					}
					cout << defaultBoard << endl;
				} else if (command == "-") { // remove a piece from the board
					cin >> arg1;
					try {
						Posn p{arg1};
						defaultBoard.removePiece(p);
						cout << defaultBoard << endl;
					} catch (BadPosn &e) {
						cerr << "Please input valid board coordinates." << endl;
						continue;
					}
				} else if (command == "=") { // change default starting colour
					cin >> arg1;
					if (arg1 == "white" || arg1 == "w") {
						defaultWhiteTurn = true;
						cout << "White will now have the first turn." << endl;
					} else if (arg1 == "black" || arg1 == "b") {
						defaultWhiteTurn = false;
						cout << "Black will now have the first turn." << endl;
					} else {
						cerr << "Please input a valid colour." << endl;
					}
				} else if (command == "clear") {
					for (auto space : defaultBoard) {
						if (space) defaultBoard.removePiece(space->getPosn());
					}
					cout << defaultBoard << endl;
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
	cout << endl << "Final Score:" << endl
		 << "White: " << whiteWins << endl
		 << "Black: " << blackWins << endl;
	return 0;
} // main
