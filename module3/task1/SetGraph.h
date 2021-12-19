#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include "IGraph.h"

struct SetGraph : public IGraph {
public:
    explicit SetGraph(int size)
            : adjacencyLists(size) {
    }

    explicit SetGraph(const IGraph &graph)
            : adjacencyLists(graph.VerticesCount()) {
        std::vector<int> vert;
        for (int i = 0; i < graph.VerticesCount(); i++) {
            vert = graph.GetNextVertices(i);
            for (int &v: vert)
                adjacencyLists[i].insert(v);
        }
    }

    ~SetGraph() override = default;

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].insert(to);
    }

    int VerticesCount() const override {
        return (int) adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> nextVertices(adjacencyLists.size());
        for (int elem: adjacencyLists[vertex])
            nextVertices.push_back(elem);
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++) {
            if (adjacencyLists[from].count(vertex))
                prevVertices.push_back(from);
        }

        return prevVertices;
    }

private:
    std::vector<std::set<int>> adjacencyLists;
};