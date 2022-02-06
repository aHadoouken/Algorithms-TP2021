#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstring>
#include <set>
#include <fstream>

const char FieldSize = 16;
const std::array<char, FieldSize> finishField = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                                 11, 12, 13, 14, 15, 0};

class GameState {
public:
    GameState(const std::array<char, FieldSize> &field)
            : field(field) {
        emptyPos = -1;
        for (int i = 0; i < FieldSize; i++) {
            if (field[i] == 0)
                emptyPos = i;
        }
        assert(emptyPos != -1);
    }

    GameState(const GameState &other)
            : field(other.field), emptyPos(other.emptyPos) {
    }

    bool IsComplete() const {
        return field == finishField;
    }

    bool IsSolvable() const {
        size_t invCount = getInvCount();
        char K = emptyPos / 4 + 1;
        return (invCount + K) % 2 == 0;
    }

    bool CanMoveLeft() const {
        return emptyPos % 4 != 3;
    }

    bool CanMoveRight() const {
        return emptyPos % 4 != 0;
    }

    bool CanMoveUp() const {
        return emptyPos < 12;
    }

    bool CanMoveDown() const {
        return emptyPos > 3;
    }

    GameState MoveLeft() const {
        assert(CanMoveLeft());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 1]);
        newState.emptyPos++;
        return newState;
    }

    GameState MoveRight() const {
        assert(CanMoveRight());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 1]);
        newState.emptyPos--;
        return newState;
    }

    GameState MoveUp() const {
        assert(CanMoveUp());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 4]);
        newState.emptyPos += 4;
        return newState;
    }

    GameState MoveDown() const {
        assert(CanMoveDown());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 4]);
        newState.emptyPos -= 4;
        return newState;
    }

    bool operator==(const GameState &other) const {
        return field == other.field;
    }

    int finishDist() {
        int i_real, j_real;
        int total_dist = 0;
//        if (emptyPos == 2) {
//            std::cout << "printarray===============" << std::endl;
//            for (int i = 0; i < 4; i++) {
//                for (int j = 0; j < 4; j++) {
//                    std::cout << static_cast<int>(field[i * 4 + j]) << " ";
//                }
//                std::cout << std::endl;
//            }
//        std::cout << "printarray===============" << std::endl;
//        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i * 4 + j != emptyPos) {
                    j_real = (field[i * 4 + j] - 1) % 4;
                    i_real = (field[i * 4 + j] - 1) / 4;
                    total_dist += abs(i_real - i) + abs(j_real - j);
//                    std::cout << "i = " << i << " j = " << j << " i_real = " <<
//                              i_real << " j_real = " << j_real << " dist = "
//                              << abs(i_real - i) + abs(j_real - j)
//                              << std::endl;
                } else {
//                    std::cout << "i = " << i << " j = " << j << " i_real = " <<
//                              i_real << " j_real = " << j_real << " dist = "
//                              << abs(3 - i) + abs(3 - j)
//                              << std::endl;
                    //total_dist += abs(3 - i) + abs(3 - j);
                }
            }
        }
        return total_dist;
    }

    size_t getInvCount() const {
        size_t inv_count = 0;
        for (int i = 0; i < FieldSize - 1; i++) {
            for (int j = i + 1; j < FieldSize; j++) {
                if (field[i] && field[j] && field[i] > field[j])
                    inv_count++;
            }
        }
        return inv_count;
    }

private:



    std::array<char, FieldSize> field;
    char emptyPos;

    friend struct GameStateHash;

    friend std::ostream &operator<<(std::ostream &out, const GameState &state);
};

std::ostream &operator<<(std::ostream &out, const GameState &state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out << static_cast<int>(state.field[i * 4 + j]) << " ";
        }
        out << std::endl;
    }
    return out;
}

struct GameStateHash {
public:
    size_t operator()(const GameState &state) const {
        size_t hash = 0;
        std::memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
};

struct cmp {
    bool operator()(GameState a, GameState b) const {
        return a.finishDist() < b.finishDist();
    }
};

std::string GetSolution(const std::array<char, FieldSize> &field) {
    GameState startState(field);

    if (!startState.IsSolvable())
        return "-1";

    std::unordered_map<GameState, char, GameStateHash> visited;
    visited[startState] = 'S';

    std::set<GameState, cmp> set;
    set.insert(startState);

    std::cout << "stat:" << std::endl;
    std::cout << startState;
    std::cout << startState.finishDist() << std::endl;

    while (true) {
        GameState state = *set.begin();

//        for (GameState el:set){
//            std::cout << el.finishDist() << " ";
//        }
//        std::cout<< std::endl << "dist: ";
//        std::cout << state.finishDist() << std::endl;

        set.erase(set.begin());

        if (state.IsComplete())
            break;

        if (state.CanMoveLeft()) {
            GameState newState = state.MoveLeft();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'L';
                set.insert(newState);
            }
        }
        if (state.CanMoveRight()) {
            GameState newState = state.MoveRight();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'R';
                set.insert(newState);
            }
        }
        if (state.CanMoveUp()) {
            GameState newState = state.MoveUp();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'U';
                set.insert(newState);
            }
        }
        if (state.CanMoveDown()) {
            GameState newState = state.MoveDown();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'D';
                set.insert(newState);
            }
        }
    }

    std::string path;
    GameState state(finishField);

    std::cout << state << std::endl;

    while (visited[state] != 'S') {
        char move = visited[state];
        switch (move) {
            case 'L':
                state = state.MoveRight();
                path += 'L';
                break;
            case 'R':
                state = state.MoveLeft();
                path += 'R';
                break;
            case 'U':
                state = state.MoveDown();
                path += 'U';
                break;
            case 'D':
                state = state.MoveUp();
                path += 'D';
                break;
        }
        //std::cout << state << std::endl;
    }

    std::reverse(path.begin(), path.end());
    std::cout << path.size() << std::endl;
    return path;
}

int main(int argc, const char *argv[]) {
//    std::array<char, FieldSize> field = {1, 3, 0, 4,
//                                         5, 2, 7, 8,
//                                         9, 6, 10, 11,
//                                         13, 14, 15, 12};
//    std::array<char, FieldSize> field = {12, 10, 8, 15,
//                                         6, 13, 0, 11,
//                                         3, 1, 14, 2,
//                                         7, 5, 4, 9};
    std::array<char, 16> startField = {0};
    size_t el;
    std::ifstream file("../input4_1.txt");
    for (int i = 0; i < 16; i ++){
        file >> el;
        startField[i] = el;
    }
    std::cout << GetSolution(startField) << std::endl;
    return 0;
}
