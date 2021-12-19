#include <iostream>
#include <functional>
#include <queue>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

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

int main() {
    /*
     * ListGraph test
     */
    std::cout << "Testing ListGraph object..." << std::endl;
    ListGraph listGraph(7);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);

    mainBFS(listGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    ListGraph listGraph2(listGraph);
    mainBFS(listGraph2, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    /*
    * MatrixGraph test
    */
    std::cout << "Testing MatrixGraph object..." << std::endl;
    MatrixGraph matrixGraph(7);
    matrixGraph.AddEdge(0, 1);
    matrixGraph.AddEdge(0, 5);
    matrixGraph.AddEdge(1, 2);
    matrixGraph.AddEdge(1, 3);
    matrixGraph.AddEdge(1, 5);
    matrixGraph.AddEdge(1, 6);
    matrixGraph.AddEdge(3, 2);
    matrixGraph.AddEdge(3, 4);
    matrixGraph.AddEdge(3, 6);
    matrixGraph.AddEdge(5, 4);
    matrixGraph.AddEdge(5, 6);
    matrixGraph.AddEdge(6, 4);

    mainBFS(matrixGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    MatrixGraph matrixGraph2(matrixGraph);
    mainBFS(matrixGraph2, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    /*
     * SetGraph test
     */
    std::cout << "Testing SetGraph object..." << std::endl;
    SetGraph setGraph(7);
    setGraph.AddEdge(0, 1);
    setGraph.AddEdge(0, 5);
    setGraph.AddEdge(1, 2);
    setGraph.AddEdge(1, 3);
    setGraph.AddEdge(1, 5);
    setGraph.AddEdge(1, 6);
    setGraph.AddEdge(3, 2);
    setGraph.AddEdge(3, 4);
    setGraph.AddEdge(3, 6);
    setGraph.AddEdge(5, 4);
    setGraph.AddEdge(5, 6);
    setGraph.AddEdge(6, 4);

    mainBFS(setGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    SetGraph setGraph2(setGraph);
    mainBFS(setGraph2, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    /*
     * ArcGraph test
     */
    std::cout << "Testing ArcGraph object..." << std::endl;
    ArcGraph arcGraph(7);
    arcGraph.AddEdge(0, 1);
    arcGraph.AddEdge(0, 5);
    arcGraph.AddEdge(1, 2);
    arcGraph.AddEdge(1, 3);
    arcGraph.AddEdge(1, 5);
    arcGraph.AddEdge(1, 6);
    arcGraph.AddEdge(3, 2);
    arcGraph.AddEdge(3, 4);
    arcGraph.AddEdge(3, 6);
    arcGraph.AddEdge(5, 4);
    arcGraph.AddEdge(5, 6);
    arcGraph.AddEdge(6, 4);

    mainBFS(arcGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    ArcGraph ArcGraph2(arcGraph);
    mainBFS(ArcGraph2, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    return 0;
}
