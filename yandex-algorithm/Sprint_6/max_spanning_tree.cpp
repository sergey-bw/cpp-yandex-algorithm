/**
 * Спринт 6. Финальное задание: Дорогая сеть
 *
 * Постановка задачи:
 *     Дан взвешенный неориентированный граф. Требуется найти максимальное остовное
 *     дерево (Maximum Spanning Tree) и вычислить суммарный вес его рёбер.
 * 
 * Формат ввода:
 *     В первой строке дано количество вершин n и ребер m графа.
 *     В каждой из следующих m строк заданы рёбра в виде троек чисел u, v, w, где
 *     u и v — вершины, которые соединяет это ребро, w - его вес.
 * 
 * Формат вывода:
 *     Если максимальное остовное дерево существует, то выведите его вес.
 *     Иначе (если в графе несколько компонент связности) выведите фразу:
 *     «Oops! I did it again».
 * 
 * Ограничения:
 *     Общее количество вершин и ребер графа: 1 ≤ n, m ≤ 1000,
 *     Вес ребер: 0 ≤ w ≤ 10000
 *     В графе могут быть петли и кратные ребра.
 *     Граф может оказаться несвязным.
 *
 * Принцип работы (алгоритм Прима с приоритетной очередью):
 *     1. Помещаем в очередь стартовую вершину 0 с весом входящего ребра 0.
 *     2. Извлекаем из очереди ребро максимального веса.
 *        Если его целевая вершина уже посещена, пропускаем ребро.
 *     3. Иначе включаем вершину в остов и прибавляем вес ребра к ответу.
 *        Добавляем в очередь её рёбра к непосещённым вершинам.
 *     4. Повторяем действия, пока очередь не опустеет
 *        или не будут посещены все вершины.
 *     5. Если посещены не все вершины, граф несвязный
 *        и остовного дерева не существует.
 *
 * Сложность:
 *     Временная:
 *         Инициализация массива посещённых вершин: O(|V|), где
 *             |V| - число вершин
 *         Просмотр кандидатов (смежных вершин и ребер к ним): O(|V| + |E|), где
 *             |E| - число рёбер
 *         Добавление и извлечение ребер из кучи: O(|E| * log(|E|))
 * 
 *         Общая временная сложность:
 *             O(|V| + |E| * log(|E|)), где
 *                 |V| - число вершин,
 *                 |E| - число рёбер
 *
 *     Пространственная:
 *         O(|V| + |E|) - хранение списков смежности графа, массива посещённых
 *         вершин и приоритетной очереди рёбер
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/25070/problems/A/
 *     Посылка 2025: https://contest.yandex.ru/contest/25070/run-report/134841090/
 *     Посылка 2026: https://contest.yandex.ru/contest/25070/run-report/172160533/
 */

#include <iostream>
#include <vector>
#include <optional>
#include <queue>

using Vertex = int;
using Weight = int;

struct Edge {
    Weight weight;
    Vertex to;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

using AdjacencyList = std::vector<Edge>;


class Graph {
public:
    explicit Graph(size_t size) : graph_(size) {}

    void AddEdge(Vertex from, Vertex to, Weight weight) {
        graph_[from - 1].push_back({weight, to - 1});
        graph_[to - 1].push_back({weight, from - 1});
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
        Weight weight;
        in >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
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