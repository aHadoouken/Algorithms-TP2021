#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include "IGraph.h"

struct MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(int size)
            : matrix(size, std::vector<bool>(size, false)) {
    }

    explicit MatrixGraph(const IGraph &graph)
            : matrix(graph.VerticesCount(),
                     std::vector<bool>(graph.VerticesCount(), false)) {
        std::vector<int> vertices;
        for (int i = 0; i < graph.VerticesCount(); i++) {
            vertices = graph.GetNextVertices(i);
            for (int &v: vertices) {
                matrix[i][v] = true;
            }
        }
    }

    ~MatrixGraph() override = default;

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < matrix.size());
        assert(0 <= to && to < matrix.size());
        if (!matrix[from][to]) {
            matrix[from][to] = true;
        }
    }

    int VerticesCount() const override {
        return (int) matrix.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < matrix.size());
        std::vector<int> nextVertices;
        for (int i = 0; i < matrix.size(); i++) {
            if (matrix[vertex][i])
                nextVertices.push_back(i);
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < matrix.size());
        std::vector<int> prevVertices;
        for (int j = 0; j < matrix.size(); j++) {
            if (matrix[j][vertex])
                prevVertices.push_back(j);
        }
        return prevVertices;
    }

private:
    std::vector<std::vector<bool>> matrix;
};