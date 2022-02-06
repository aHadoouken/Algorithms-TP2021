/*
 * Требуется отыскать самый короткий маршрут между городами. Из города может выходить
 * дорога, которая возвращается в этот же город.
 * Требуемое время работы O((N + M)log N), где N – количество городов,
 * M – известных дорог между ними. N ≤ 10000, M ≤ 250000. Длина каждой дороги ≤ 10000.
 */
#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <functional>
#include <fstream>
#include <climits>

struct ListGraph {
public:
    explicit ListGraph(int size)
            : adjacencyLists(size) {
    }

    ~ListGraph() = default;

    void AddEdge(int from, std::pair<int, int> to) {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to.second && to.second < adjacencyLists.size());
        for (auto &el: adjacencyLists[from]) {
            if (el.second == to.second && el.first > to.first) {
                el.first = to.first;
                return;
            }
        }
        adjacencyLists[from].push_back(to);
    }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    int ShortestPathLen(int from, int to) {
        std::vector<int> r(adjacencyLists.size(), INT_MAX);
        r[from] = 0;

        std::set<std::pair<int, int>> set;
        set.insert(std::make_pair(0, from));

        std::vector<std::pair<int, int>> nextVertices;
        std::pair<int, int> curVert;
        while (!set.empty()) {
            curVert = *set.begin();
            set.erase(set.begin());
            nextVertices = GetNextVertices(curVert.second);
            for (auto &v: nextVertices) {
                if (r[v.second] > r[curVert.second] + v.first) {
                    set.erase(std::make_pair(r[v.second], v.second));
                    r[v.second] = r[curVert.second] + v.first;
                    set.insert(std::make_pair(r[v.second], v.second));
                }
            }
        }
        return r[to];
    }

private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyLists;
};

int main() {
    std::ifstream file("../input3_3.txt");
    int N, M, e1, e2, w, from, to;
    file >> N;
    file >> M;
    ListGraph graph(N);
    for (int i = 0; i < M; i++) {
        file >> e1 >> e2 >> w;
        graph.AddEdge(e1, std::make_pair(w, e2));
        graph.AddEdge(e2, std::make_pair(w, e1));
    }
    file >> from >> to;

    std::cout << graph.ShortestPathLen(from, to);
    return 0;
}