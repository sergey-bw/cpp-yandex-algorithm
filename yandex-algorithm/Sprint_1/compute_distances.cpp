/**
 * Спринт 1. Финальное задание: Ближайший ноль
 *
 * Постановка задачи:
 *     Улица состоит из N участков (домов и пустых участков). Пустые участки
 *     обозначены нулями, а дома - положительными числами (номера домов).
 *     Требуется для каждого участка с домом определить расстояние до ближайшего нуля.
 *     Если участок пустой, эта величина будет равна нулю - расстояние до самого себя.
 * 
 * Формат ввода:
 *     В первой строке дана длина улицы N.
 *     В следующей строке записаны N целых неотрицательных чисел - номера домов
 *     и обозначения пустых участков на карте (нули).
 * 
 * Формат вывода:
 *     Для каждого из участков выведите расстояние до ближайшего нуля.
 *     Числа выводите в одну строку, разделяя их пробелами.
 * 
 * Ограничения:
 *     - Длина улицы: 1 ≤ N ≤ 10^6.
 *     - Номера домов (положительные числа) уникальны и не превосходят 10^9.
 *     - Гарантируется, что в последовательности есть хотя бы один ноль.
 * 
 * Принцип работы:
 *     Используется алгоритм двух линейных проходов по массиву:
 *     1. Прямой проход (слева направо):
 *        Записываем расстояние до ближайшего нуля слева. Если нуля слева
 *        ещё не было, сохраняем начальное значение - максимальное значение int.
 *     2. Обратный проход (справа налево):
 *        Вычисляем расстояние до ближайшего нуля справа и выбираем минимум
 *        между ним и ранее записанным расстоянием.
 *
 * Сложность:
 *     Временная:
 *         Два линейных прохода и ввод/вывод массива: O(N), где
 *             N - количество элементов в массиве
 * 
 *     Пространственная:
 *         Хранение входного и результирующего массивов: O(N)
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/22450/problems/A/
 *     Посылка 2024: https://contest.yandex.ru/contest/22450/run-report/125497479/
 *     Посылка 2026: https://contest.yandex.ru/contest/22450/run-report/171945477/
 */

#include <algorithm>
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
    zero_distance = -1;
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

namespace {
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
}// namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto street = ReadVector(std::cin);
    auto distances = ComputeDistance(street);
    std::cout << distances << '\n';

    return 0;
}