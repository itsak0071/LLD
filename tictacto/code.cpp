
#include <bits/stdc++.h>
using namespace std;

enum class PieceType { X, O };
enum class GameStatus { DRAW, WIN };

class PlayingPiece {
public:
    PieceType type;

    PlayingPiece(PieceType t) {
        type = t;
    }

    virtual ~PlayingPiece() {}
};

class PlayingPieceX : public PlayingPiece {
public:
    PlayingPieceX() : PlayingPiece(PieceType::X) {}
};

class PlayingPieceO : public PlayingPiece {
public:
    PlayingPieceO() : PlayingPiece(PieceType::O) {}
};

class Player {
public:
    string name;
    PlayingPiece* piece;

    Player(string n, PlayingPiece* p) {
        name = n;
        piece = p;
    }
};

class Board {
public:
    int n;
    vector<vector<PlayingPiece*>> grid;

    Board(int size) {
        n = size;
        grid.resize(n, vector<PlayingPiece*>(n, nullptr));
    }

    bool addPiece(int r, int c, PlayingPiece* p) {
        if (r < 0 || r >= n || c < 0 || c >= n)
            return false;

        if (grid[r][c] != nullptr)
            return false;

        grid[r][c] = p;
        return true;
    }

    bool isFull() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == nullptr)
                    return false;
            }
        }
        return true;
    }

    void printBoard() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == nullptr)
                    cout << " ";
                else
                    cout << (grid[i][j]->type == PieceType::X ? "X" : "O");

                if (j < n - 1) cout << " | ";
            }

            cout << "\n";

            if (i < n - 1)
                cout << "---------\n";
        }
    }
};

class TicTacToeGame {
public:
    deque<Player> players;
    Board* board;
    Player* winner;

    TicTacToeGame() {
        board = nullptr;
        winner = nullptr;
    }

    ~TicTacToeGame() {
        delete board;
        delete winner;

        for (auto& p : players)
            delete p.piece;
    }

    void initializeGame() {
        board = new Board(3);

        players.push_back(Player("Player1", new PlayingPieceX()));
        players.push_back(Player("Player2", new PlayingPieceO()));
    }

    bool checkForWinner(int r, int c, PieceType type) {
        bool row = true, col = true;
        bool diag = true, antiDiag = true;

        for (int i = 0; i < board->n; i++) {
            if (board->grid[r][i] == nullptr ||
                board->grid[r][i]->type != type)
                row = false;

            if (board->grid[i][c] == nullptr ||
                board->grid[i][c]->type != type)
                col = false;

            if (board->grid[i][i] == nullptr ||
                board->grid[i][i]->type != type)
                diag = false;

            if (board->grid[i][board->n - 1 - i] == nullptr ||
                board->grid[i][board->n - 1 - i]->type != type)
                antiDiag = false;
        }

        return row || col || diag || antiDiag;
    }

    GameStatus startGame() {
        while (true) {
            if (board->isFull())
                return GameStatus::DRAW;

            board->printBoard();

            Player current = players.front();
            players.pop_front();

            cout << current.name << ", enter row and column (0-2): ";

            int r, c;
            if (!(cin >> r >> c)) {
                players.push_front(current);
                return GameStatus::DRAW;
            }

            if (!board->addPiece(r, c, current.piece)) {
                cout << "Invalid position! Try again.\n";
                players.push_front(current);
                continue;
            }

            if (checkForWinner(r, c, current.piece->type)) {
                board->printBoard();
                winner = new Player(current);
                return GameStatus::WIN;
            }

            players.push_back(current);
        }
    }
};

int main() {
    TicTacToeGame game;
    game.initializeGame();

    GameStatus status = game.startGame();

    if (status == GameStatus::WIN)
        cout << game.winner->name << " won the game!\n";
    else
        cout << "It's a draw!\n";

    return 0;
}
