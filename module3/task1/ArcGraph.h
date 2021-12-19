#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include "IGraph.h"

struct ArcGraph : public IGraph {
public:
    explicit ArcGraph(int size)
            : vertCount(size) {
    }

    explicit ArcGraph(const IGraph &graph)
            : vertCount(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            std::vector<int> vert = graph.GetNextVertices(i);
            for (int &v: vert) {
                pairs.emplace_back(i, v);
            }
        }
    }

    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < vertCount);
        assert(0 <= to && to < vertCount);
        pairs.emplace_back(from, to);
    }

    int VerticesCount() const override {
        return (int) vertCount;
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < vertCount);
        std::vector<int> nextVertices;
        for (auto &pair: pairs) {
            if (pair.first == vertex) {
                nextVertices.push_back(pair.second);
            }
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < vertCount);
        std::vector<int> prevVertices;
        for (auto &pair: pairs) {
            if (pair.second == vertex) {
                prevVertices.push_back(pair.first);
            }
        }
        return prevVertices;
    }

private:
    std::vector<std::pair<int, int>> pairs;
    size_t vertCount;
};