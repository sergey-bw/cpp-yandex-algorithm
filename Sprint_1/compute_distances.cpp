/**
 * Спринт 1. Финальное задание: Ближайший ноль
 *
 * Постановка задачи:
 *     Улица состоит из n участков (домов и пустых участков). Пустые участки
 *     обозначены нулями, а дома — положительными числами (номера домов).
 *     Гарантируется, что в последовательности есть хотя бы один ноль.
 *     Требуется для каждого участка определить расстояние до ближайшего нуля.
 *
 * Принцип работы:
 *     Используется алгоритм двух линейных проходов по массиву:
 *     1. Прямой проход (слева направо):
 *        От каждого нуля инкрементируем расстояния вправо.
 *     2. Обратный проход (справа налево):
 *        От каждого нуля обновляем значение влево минимальным значением
 *        дистанции первого и второго проходов: 
 *        std::min(distances_first, distances_second).
 *
 * Сложность:
 *     Временная:
 *         O(N), где N - размер входного массива
 *     Пространственная:
 *         O(N), где N - входной массив и результирующий массив
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/22450/problems/A/
 *     Посылка 2024: https://contest.yandex.ru/contest/22450/run-report/125497479/
 */

#include <iostream>
#include <vector>
#include <limits>

std::vector<int> ComputeDistance(const std::vector<int>& street) {
    const int n = static_cast<int>(street.size());
    std::vector<int> distances(n, std::numeric_limits<int>::max());

    // Прямой проход
    int zero_distance = -1;
    for (int i = 0; i < n; ++i) {
        if (street[i] == 0) {
            zero_distance = 0;
        }
        if (zero_distance != -1) {
            distances[i] = zero_distance;
            ++zero_distance;
        }
    }

    // Обратный проход
    int zero_distance = -1;
    for (int i = n - 1; i >= 0; --i) {
        if (street[i] == 0) {
            zero_distance = 0;
        }
        if (zero_distance != -1) {
            distances[i] = std::min(distances[i], zero_distance);
            ++zero_distance;
        }
    }
    return distances;
}

std::vector<int> ReadVector(std::istream& in) {
    size_t n;
    in >> n;
    std::vector<int> vct(n);
    for (auto& item : vct) {
        in >> item;
    }
    return vct;
}

std::ostream& operator<<(std::ostream& out, const std::vector<int>& vct) {
    bool is_first = true;
    for (auto el : vct) {
        if (is_first) {
            is_first = false;
        } else {
            out << ' ';
        }
        out << el;
    }
    return out;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto street = ReadVector(std::cin);
    auto distances = ComputeDistance(street);
    std::cout << distances << '\n';

    return 0;
}