#include "board.h"
#include "move.h"

class Game {
		protected:

		Board board;
		vector<Move> history;

		int ply = 0;
		int turn = 0;
		vector<Player> players;
		Player* current;

		public:

		Game(Board b, vector<Move> m, vector<Player> p) : board(b), history(m), players(p) {
				current = &players[turn];
		}

		vector<Move*> moves() {
				vector<Move*> m;
				// for (Player p : players) {
						// for (Move* x : p.moves()) m.push_back(x);
				// }
				for (Move* x : current -> moves()) m.push_back(x);
				return m;
		}

		Move random() {
				vector<Move*> m = moves();
				return *m[rand() % m.size()];
		}

		Game* move(Move m) {
				board.move(m);
				history.push_back(m);

				ply ++;
				turn = ply % players.size();
				current = &players[turn];
				return this;
		}

		Game clone() {
				Game result(board.clone(), {}, {});
				return result;
		}

		string tostring() {
				string result = "Game [\n";
				result += fmt::format("{} moves, ply {}\n", moves().size(), ply);
				for (auto p : players) result += p.tostring() + '\n';
				result += board.tostring();
				result += "]";
				return result;
		}

		Game* print() {
				std::cout << tostring();
				return this;
		}
};
