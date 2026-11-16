/**
 * Спринт 6. Финальное задание: Острова
 *
 * Постановка задачи:
 *     Дана географическая карта в виде двумерной матрицы символов размера M x N,
 *     где '#' обозначает сушу, а '.' — воду. Островом считается связная группа
 *     клеток суши, соединённых по горизонтали или вертикали (соседство по 4 направлениям).
 *     Требуется определить общее количество островов и площадь (количество клеток)
 *     наибольшего из них. Если суши на карте нет, максимальная площадь равна 0.
 * 
 * Ограничения:
 *     - Размеры карты M (строки) и N (столбцы): 1 <= M, N <= 1000.
 *     - Символы матрицы: '#' (суша) и '.' (вода).
 *
 * Принцип работы (BFS на одномерном буфере с внешним вектором посещений):
 *     Карта упаковывается в плоский одномерный вектор std::vector<char> размера M * N,
 *     что обеспечивает непрерывность данных в памяти и кэш-локальность.
 *     Посещённые вершины отслеживаются в локальном массиве std::vector<bool>.
 *     При нахождении непосещённой клетки суши '#' инкрементируется счётчик островов
 *     и запускается функция обхода острова (BFS).
 *     По завершении обхода каждой компоненты связности обновляется максимальная площадь острова.
 *
 * Сложность:
 *     Временная:
 *         O(M * N) - каждая ячейка матрицы просматривается во внешнем цикле константное
 *         число раз, а BFS посещает каждое ребро и вершину острова ровно один раз
 *
 *     Пространственная:
 *         O(M * N) - хранение исходной карты и вектора посещённых ячеек
 *         Вспомогательная память под очередь BFS в худшем случае: O(min(M, N))
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/25070/problems/B/
 *     Посылка 2025: https://contest.yandex.ru/contest/25070/run-report/134841263/
 *     Посылка 2025: https://contest.yandex.ru/contest/25070/run-report/170886161/
 */

#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <string>

struct IslandStats {
    int count = 0;
    int max_area = 0;
};

class Map {
public:
    Map() = delete;

    static Map FromStrings(const std::vector<std::string>& lines) {
        if (lines.empty() || lines[0].empty()) {
            return Map(0, 0, {});
        }

        const int rows = static_cast<int>(lines.size());
        const int cols = static_cast<int>(lines[0].size());

        std::vector<char> grid;
        grid.reserve(rows * cols);
        for (const std::string& line : lines) {
            grid.insert(grid.end(), line.begin(), line.end());
        }

        return Map(rows, cols, std::move(grid));
    }

    static Map FromStream(std::istream& in) {
        int rows = 0;
        int cols = 0;
        in >> rows >> cols;

        std::vector<std::string> lines(rows);
        for (int r = 0; r < rows; ++r) {
            in >> lines[r];
        }

        return FromStrings(lines);
    }

    IslandStats ComputeIslandStats() const {
        IslandStats stats;
        std::vector<bool> visited(rows_ * cols_, false);

        for (size_t i = 0; i < grid_.size(); ++i) {
            if (grid_[i] == '#' && !visited[i]) {
                ++stats.count;
                stats.max_area = std::max(stats.max_area, BfsIslandArea(i, visited));
            }
        }
        return stats;
    }

private:
    Map(int rows, int cols, std::vector<char> grid)
        : rows_(rows), cols_(cols), grid_(std::move(grid)) {}

    int BfsIslandArea(int vertex, std::vector<bool>& visited) const {
        int island_area = 1;
        std::queue<int> planned_vertex;
        planned_vertex.push(vertex);
        visited[vertex] = true;

        static constexpr std::array<std::pair<int, int>, 4> kOffsets = {{
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        }};

        while (!planned_vertex.empty()) {
            int current_vertex = planned_vertex.front();
            planned_vertex.pop();

            for (const auto& [row_offset, col_offset] : kOffsets) {
                int row = current_vertex / cols_ + row_offset;
                int col = current_vertex % cols_ + col_offset;

                if (row >= 0 && row < rows_ && col >= 0 && col < cols_) {
                    int neighbor_vertex = row * cols_ + col;
                    if (grid_[neighbor_vertex] == '#' && !visited[neighbor_vertex]) {
                        planned_vertex.push(neighbor_vertex);
                        visited[neighbor_vertex] = true;
                        ++island_area;
                    }
                }
            }
        }
        return island_area;
    }

    int rows_ = 0;
    int cols_ = 0;
    std::vector<char> grid_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto map = Map::FromStream(std::cin);
    const auto stats = map.ComputeIslandStats();
    std::cout << stats.count << ' ' << stats.max_area << '\n';
    return 0;
}