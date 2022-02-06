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
        std::cout << from << " " << to.first << " " << to.second << std::endl;
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

    int VerticesCount() const
    {
        return (int)adjacencyLists.size();
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

void createGraphEdges(ListGraph &graph, const int &a, const int &b){
    int M = graph.VerticesCount();
    for (long i = 0; i < M; i++){
        graph.AddEdge(i, std::make_pair(a, (i + 1) % M));
        graph.AddEdge(i, std::make_pair(b, (i*i + 1) % M));
    }
}

int main() {
    std::ifstream file("../input2_1.txt");
    int a, b, M, x, y;
    file >> a;
    file >> b;
    file >> M;
    file >> x;
    file >> y;
    ListGraph graph(M);

    createGraphEdges(graph, a, b);

    std::cout << graph.ShortestPathLen(x, y);
    return 0;
}