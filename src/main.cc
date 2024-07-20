// src/main.cc

#include "../include/ai.h"

using namespace std;

bool text, graphics;

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

void display(Board &board, XWindow &xw) {
	if (text) cout << board << endl;
	if (graphics) {
		xw.drawBoard(board);
	}
}

int main() {
	double whiteWins = 0, blackWins = 0;
	string command, arg1, arg2;
	bool gameActive = false, defaultWhiteTurn = true, whiteTurn;
	Board board, defaultBoard;
	unique_ptr<AI> whiteAI, blackAI;
	XWindow xw = XWindow(1000, 800);
	// Initial default board
	// White pieces
	defaultBoard.addPiece<Rook>(true, {"a1"});
	defaultBoard.addPiece<Knight>(true, {"b1"});
	defaultBoard.addPiece<Bishop>(true, {"c1"});
	defaultBoard.addPiece<Queen>(true, {"d1"});
	defaultBoard.addPiece<King>(true, {"e1"});
	defaultBoard.addPiece<Bishop>(true, {"f1"});
	defaultBoard.addPiece<Knight>(true, {"g1"});
	defaultBoard.addPiece<Rook>(true, {"h1"});
	// Black pieces
	defaultBoard.addPiece<Rook>(false, {"a8"});
	defaultBoard.addPiece<Knight>(false, {"b8"});
	defaultBoard.addPiece<Bishop>(false, {"c8"});
	defaultBoard.addPiece<Queen>(false, {"d8"});
	defaultBoard.addPiece<King>(false, {"e8"});
	defaultBoard.addPiece<Bishop>(false, {"f8"});
	defaultBoard.addPiece<Knight>(false, {"g8"});
	defaultBoard.addPiece<Rook>(false, {"h8"});
	// Pawns
	for (unsigned int i = 0; i < WIDTH; i++) {
		defaultBoard.addPiece<Pawn>(true, {i, 1});
		defaultBoard.addPiece<Pawn>(false, {i, HEIGHT - 2});
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
			break;
		} else if (arg1 == "b" || arg1 == "B") {
			text = graphics = true;
			cout << "Both displays selected." << endl << defaultBoard << endl;
			break;
		}
		cout << endl << "Please input \"t\" or \"g\"." << endl;
	}
	cout << "Command: ";
	while (cin >> command) {
		if (command == "game") {
			if (gameActive) {
				cerr << "Game is already active." << endl;
				continue;
			}
			cin >> arg1 >> arg2;
			int p1 = parsePlayer(arg1);
			int p2 = parsePlayer(arg2);
			if (p1 < 0 || p2 < 0) {
				cerr << "Please input either \"human\" or \"computer[X]\" for the players" << endl;
				continue;
			}
			whiteAI = (p1 ? make_unique<AI>(board, true, p1) : nullptr);
			blackAI = (p2 ? make_unique<AI>(board, false, p2) : nullptr);
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
				board.movePiece(whiteTurn, whiteAI->think());
			} else if (!whiteTurn && blackAI) {
				board.movePiece(whiteTurn, blackAI->think());
			} else {
				cin >> arg1 >> arg2;
				try {
					Posn start{arg1}, end{arg2};
					board.movePiece(whiteTurn, {start, end});
					if ((whiteTurn && board[end]->getName() == 'P') || (!whiteTurn && board[end]->getName() == 'p')) {
						shared_ptr<Pawn> tmp = static_pointer_cast<Pawn>(board[end]);
						if (tmp->canPromote()) {
							char piece;
							cin >> piece;
							bool white = 'B' <= piece && piece <= 'R';
							if (white != whiteTurn) {
								cerr << "Please input a valid piece type to promote into." << endl;
								whiteTurn = !whiteTurn;
							} else {
								piece -= (white ? ('A' - 'a') : 0);
								switch (piece) {
									case 'n':
										board.promote(white, {start, end}, 1);
										break;
									case 'b':
										board.promote(white, {start, end}, 2);
										break;
									case 'r':
										board.promote(white, {start, end}, 3);
										break;
									case 'q':
										board.promote(white, {start, end}, 4);
										break;
									default:
										cerr << "Please input a valid piece type to promote into." << endl;
										whiteTurn = !whiteTurn;
								}
							}
						}
					}
				} catch (BadPosn &e) {
					cerr << "Please input valid board coordinates." << endl
						 << (whiteTurn ? "White" : "Black" ) << "'s move: ";
					continue;
				} catch (BadMove &e) {
					cerr << "Please input a valid move action." << endl
						 << (whiteTurn ? "White" : "Black" ) << "'s move: ";
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
					whiteTurn = (board.undoMoves(num) ? num % 2 ? !whiteTurn : whiteTurn : defaultWhiteTurn);
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
			display(defaultBoard, xw);
			while (cin >> command) {
				if (command == "+") { // add a piece to the board
					char piece;
					cin >> piece >> arg1;
					if (!verifyPiece(piece)) {
						cerr << "Please input a valid piece name." << endl << "Command: ";
						continue;
					}
					try {
						Posn p{arg1};
						defaultBoard.removePiece(p);
						bool white = 'B' <= piece && piece <= 'R';
						piece -= (white ? ('A' - 'a') : 0);
						switch (piece) {
							case 'p':
								defaultBoard.addPiece<Pawn>(white, p);
								break;
							case 'n':
								defaultBoard.addPiece<Knight>(white, p);
								break;
							case 'b':
								defaultBoard.addPiece<Bishop>(white, p);
								break;
							case 'r':
								defaultBoard.addPiece<Rook>(white, p);
								break;
							case 'q':
								defaultBoard.addPiece<Queen>(white, p);
								break;
							case 'k':
								if (defaultBoard.hasKing(white)) {
									cerr << "There cannot be two " << (white ? "white" : "black")
										 << " kings on the board at once!!" << endl;
									break;
								} else {
									defaultBoard.addPiece<King>(white, p);
									break;
								}
						}
					} catch (BadPosn &e) {
						cerr << "Please input valid board coordinates." << endl;
					}
					display(defaultBoard, xw);
				} else if (command == "-") { // remove a piece from the board
					cin >> arg1;
					try {
						Posn p{arg1};
						defaultBoard.removePiece(p);
						display(defaultBoard, xw);
					} catch (BadPosn &e) {
						cerr << "Please input valid board coordinates." << endl;
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
					// Iterate over every space on the board
					for (auto space : defaultBoard) {
						// If a piece exists in that square, remove it
						if (space) defaultBoard.removePiece(space->getPosn());
					}
					display(defaultBoard, xw);
				} else if (command == "done") { // valid board setup
					try {
						defaultBoard.validate();
						display(defaultBoard, xw);
						cout << "Board setup successful." << endl;
						break;
					} catch (BadSetup &e) {
						cerr << "Board setup invalid: " << e.why() << endl;
					}
				} else { // invalid command
					cerr << "Please input a valid setup command." << endl;
				} // switch
				cerr << "Command: ";
			} // while
		} else if (command == "replay") {
			if (gameActive) {
				cerr << "You must not be currently playing a game to enter replay mode." << endl
					 << (whiteTurn ? "White" : "Black") << "'s turn: ";
					 continue;
			} else if (!(whiteWins || blackWins)) {
				cerr << "You must have completed a game to enter replay mode." << endl << "Command: ";
				continue;
			}
			// replay mode stuff here
		} else {
			cerr << "Please input a valid command." << endl;
		} // switch
		if (gameActive) {
			display(board, xw);
			cerr << (whiteTurn ? "White" : "Black") << "'s turn: ";
		} else {
			cerr << "Command: ";
		}
	} // while
	cout << endl << "Final Score:" << endl
		 << "White: " << whiteWins << endl
		 << "Black: " << blackWins << endl;
	return 0;
} // main
