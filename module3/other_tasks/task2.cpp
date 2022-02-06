/*
 * Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших
 * путей между какими-то вершинами. Найдите количество различных кратчайших путей
 * между заданными вершинами. Требуемая сложность O(V+E).
 */
#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <queue>
#include <functional>
#include <fstream>
#include <climits>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

struct ListGraph : public IGraph {
public:
    explicit ListGraph(int size)
            : adjacencyLists(size) {
    }

    explicit ListGraph(const IGraph &graph)
            : adjacencyLists(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph() override = default;

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

    int VerticesCount() const override {
        return (int) adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++) {
            for (int to: adjacencyLists[from]) {
                if (to == vertex)
                    prevVertices.push_back(from);
            }
        }

        return prevVertices;
    }

    int ShortestPathsNum(int &from, int& to){
        std::vector<int> r(adjacencyLists.size(), INT_MAX);
        r[from] = 0;
        std::vector<int> t(adjacencyLists.size(), 0);
        t[from] = 1;
        std::queue<int> q;
        q.push(from);
        std::vector<int> nextVertices;
        int verticeCur;
        while(!q.empty()){
            verticeCur = q.front();
            q.pop();
            nextVertices = GetNextVertices(verticeCur);
            for (auto &v: nextVertices){
                if (r[v] > r[verticeCur] + 1){
                    r[v] = r[verticeCur] + 1;
                    t[v] = t[verticeCur];
                    q.push(v);
                } else if (r[v] == r[verticeCur] + 1){
                    t[v] += t[verticeCur];
                }
            }
        }
        for (auto el: r){
            std::cout << el << " ";
        }
        std::cout << std::endl;
        for (auto el: t){
            std::cout << el << " ";
        }
        std::cout << std::endl;
        return t[to];
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};

int main(int argc, const char *argv[]) {
    std::ifstream file("../input2_1.txt");
    int v, n, e1, e2, from, to;
    file >> v;
    file >> n;
    ListGraph graph(v);
    for (int i = 0; i < n; i++){
        file >> e1 >> e2;
        graph.AddEdge(e1, e2);
        graph.AddEdge(e2, e1);
    }
    file >> from >> to;

    std:: cout << graph.ShortestPathsNum(from, to);
    return 0;
}
