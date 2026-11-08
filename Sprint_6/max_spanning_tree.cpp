/**
 * Спринт 6. Финальное задание: Дорогая сеть
 *
 * Постановка задачи:
 *     Дан взвешенный неориентированный граф. Требуется найти максимальное остовное
 *     дерево (Maximum Spanning Tree) и вычислить суммарный вес его рёбер.
 *     Если граф несвязен и построить остовное дерево невозможно, необходимо
 *     вывести сообщение об ошибке - "Oops! I did it again".
 * 
 * Ограничения:
 *     В первой строке дано количество вершин n и ребер m графа:
 *         1 ≤ n ≤ 1000,
 *         0 ≤ m ≤ 100000.
 *     В каждой из следующих m строк заданы рёбра в виде троек чисел u, v, w:
 *         u и v — вершины, которые соединяет это ребро, w - его вес.
 *         0 ≤ w ≤ 10000
 *     В графе могут быть петли и кратные ребра. Граф может оказаться несвязным.
 *
 * Принцип работы (Алгоритм Прима с приоритетной очередью):
 *     1. Начинаем обход с произвольной стартовой вершины (вершина 0),
 *        помещая в приоритетную очередь рёбра, инцидентные остову.
 *     2. На каждом шаге извлекаем ребро максимального веса (Max-Heap).
 *     3. Если целевая вершина ещё не входит в остов, включаем её, суммируем вес
 *        ребра и добавляем всех её нерассмотренных соседей в очередь.
 *     4. Для предотвращения дублирования и повторного обхода используется
 *        вектор флагов visited (std::vector<bool>).
 *     5. По завершении алгоритма проверяется равенство количества посещённых
 *        вершин общему числу вершин |V|. Несовпадение свидетельствует о несвязности.
 *
 * Сложность:
 *     Временная:
 *         O(|E| * log |V|) - каждое ребро добавляется в очередь не более двух раз,
 *         а операция извлечения из max-heap занимает логарифмическое время.
 *
 *     Пространственная:
 *         O(|V| + |E|) - хранение списков смежности графа, массива посещённых
 *         вершин и приоритетной очереди рёбер.
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/25070/problems/A/
 *     Посылка 2025: https://contest.yandex.ru/contest/25070/run-report/134841090/
 *     Посылка 2026: https://contest.yandex.ru/contest/25070/run-report/170863772/
 */

#include <iostream>
#include <vector>
#include <optional>
#include <queue>

using Vertex = int;
using Distance = int;

struct Edge {
    Distance weight;
    Vertex to;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

using AdjacencyList = std::vector<Edge>;


class Graph {
public:
    explicit Graph(size_t size) : graph_(size){}

    void AddEdge(Vertex from, Vertex to, Distance distance) {
        graph_[from - 1].push_back({distance, to - 1});
        graph_[to - 1].push_back({distance, from - 1});
    }

    size_t Size() const {
        return graph_.size();
    }

    const AdjacencyList& GetEdges(Vertex from) const {
        return graph_[from];
    }

private:
    std::vector<AdjacencyList> graph_;
};

std::optional<int> MaxSpanningTree(const Graph& graph) {
    if (graph.Size() == 0) {
        return 0;
    }

    std::vector<bool> visited(graph.Size(), false);
    size_t visited_vertices_count = 0;

    std::priority_queue<Edge> max_heap;
    max_heap.push({0, 0});

    int max_spanning_tree = 0;
    while (!max_heap.empty() && visited_vertices_count < graph.Size()) {
        Edge current_edge = max_heap.top();
        max_heap.pop();
        if (visited[current_edge.to]) {
            continue;
        }
        visited[current_edge.to] = true;
        ++visited_vertices_count;
        max_spanning_tree += current_edge.weight;
        for (const auto& neighbor_edge : graph.GetEdges(current_edge.to)) {
            if (!visited[neighbor_edge.to]) {
                max_heap.push(neighbor_edge);
            }
        }
    }

    if (visited_vertices_count < graph.Size()) {
        return std::nullopt;
    }
    return max_spanning_tree;
}

Graph ReadGraph(std::istream& in) {
    int size, edges_count;
    in >> size >> edges_count;
    Graph graph(size);
    for (int i = 0; i < edges_count; ++i) {
        Vertex from, to;
        Distance distance;
        in >> from >> to >> distance;
        graph.AddEdge(from, to, distance);
    }
    return graph;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const Graph graph = ReadGraph(std::cin);
    if (const auto result = MaxSpanningTree(graph); result.has_value()) {
        std::cout << *result;
    } else {
        std::cout << "Oops! I did it again";
    }
    std::cout << '\n';
    return 0;
}