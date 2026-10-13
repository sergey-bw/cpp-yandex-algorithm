/**
 * Спринт 3. Финальное задание: Поиск в сломанном массиве
 *
 * Постановка задачи:
 *     Дан отсортированный по возрастанию массив уникальных целых чисел,
 *     который был циклически сдвинут на неизвестное число позиций (сломанный массив).
 *     Требуется за время O(log N) найти индекс заданного числа k в таком массиве,
 *     либо вернуть -1, если число k отсутствует.
 *
 * Принцип работы:
 *     У сломанного массива всего одна точка перегиба. Если такой массив разбить на две части,
 *     то одна из этих частей будет гарантированно отсортированной. В отсортированной части
 *     можно сразу проверить, входит ли искомый элемент в её диапазон. Исходя из этого
 *     принимается решение, в какой части искать элемент.
 *     Алгоритм представляет собой модифицированную версию бинарного поиска.
 *
 * Сложность:
 *     Временная:
 *         O(log N), где N - размер вектора
 *     Пространственная:
 *         O(1) - дополнительная память для индексов
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/23815/problems/A/
 *     Посылка 2024: https://contest.yandex.ru/contest/23815/run-report/129610896/
 *     Посылка 2026: https://contest.yandex.ru/contest/23815/run-report/168169889/
 */

//#include "solution.h"
#include <vector>
#include <cassert>

int broken_search(const std::vector<int>& broken_arr, int k) {
    if (broken_arr.empty()) {
        return -1;
    }

    int left = 0;
    int right = static_cast<int>(broken_arr.size()) - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (broken_arr[mid] == k) {
            return mid;
        }

        if (broken_arr[left] <= broken_arr[mid]) {
            // Левая половина отсортирована
            if (k < broken_arr[left] || k > broken_arr[mid]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        } else {
            // Правая половина отсортирована
            if (k < broken_arr[mid] || k > broken_arr[right]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
    }

    return -1;
}

void test() {
    std::vector<int> arr = {3, 5, 6, 7, 9, 1, 2};
    assert(5 == broken_search(arr, 1));
    assert(4 == broken_search(arr, 9));
    assert(-1 == broken_search(arr, 8));
    assert(0 == broken_search(arr, 3));
    assert(6 == broken_search(arr, 2));

    std::vector<int> empty_arr = {};
    assert(-1 == broken_search(empty_arr, 1));

    std::vector<int> sorted_arr = {1, 2, 3, 4, 5, 6, 7, 8};
    assert(4 == broken_search(sorted_arr, 5));

    std::vector<int> single_arr = {1};
    assert(0 == broken_search(single_arr, 1));
}

int main() {
    test();
    return 0;
}