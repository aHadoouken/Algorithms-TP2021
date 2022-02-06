#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <queue>
#include <fstream>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

struct ListGraph: public IGraph
{
public:
    ListGraph(int size)
            : adjacencyLists(size)
    {
    }

    ListGraph(const IGraph &graph)
            : adjacencyLists(graph.VerticesCount())
    {
        for (int i = 0; i < graph.VerticesCount(); i++)
        {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph()
    {
    }

    void AddEdge(int from, int to)
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

    int VerticesCount() const
    {
        return (int)adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++)
        {
            for (int to: adjacencyLists[from])
            {
                if (to == vertex)
                    prevVertices.push_back(from);
            }
        }

        return prevVertices;
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);

        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            BFS(graph, i, visited, func);
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            DFS(graph, nextVertex, visited, func);
    }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            DFS(graph, i, visited, func);
    }
}

int ConnNum(const IGraph &graph){
    std::vector<bool> visited(graph.VerticesCount(), false);

    int connNum = 0;
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i]) {
            connNum++;
            DFS(graph, i, visited, [](int vertex){});
        }
    }
    return connNum;
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;

    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            topologicalSortInternal(graph, nextVertex, visited, sorted);
    }

    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            topologicalSortInternal(graph, i, visited, sorted);
    }

    return sorted;
}

int main(int argc, const char * argv[]) {
    std::ifstream file("../input2_1.txt");
    int N, M, K, e1, e2, from, to;
    file >> N;
    file >> M;
    ListGraph graph(N);
    for (int i = 0; i < M; i++){
        file >> e1 >> e2;
        graph.AddEdge(e1, e2);
        graph.AddEdge(e2, e1);
    }
    std:: cout << ConnNum(graph);
    return 0;
}
