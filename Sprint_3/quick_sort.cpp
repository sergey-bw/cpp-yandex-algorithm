/**
 * Спринт 3. Финальное задание: Эффективная быстрая сортировка
 *
 * Постановка задачи:
 *     Реализовать алгоритм быстрой сортировки без выделения O(n) дополнительной
 *     памяти для промежуточных данных (in-place).
 * 
 *     Требуется отсортировать участников по правилам:
 *         1. Чем больше решено задач, тем выше участник (по убыванию P).
 *         2. При равенстве задач — меньший штраф идет выше (по возрастанию F).
 *         3. При равенстве задач и штрафов — логины упорядочиваются лексикографически.
 * 
 * Принцип работы:
 *     Реализуется алгоритм QuickSort in-place. Выбирается опорный элемент Pivot (в данной
 *     реализации опорный элемент выбирается на центральной позиции в диапазоне).
 *     Цель - сформировать такой массив, чтобы левее опорного элемента были все элементы
 *     со значениями меньшими опорного элемента, а правее элементы большие по значению.
 *     Затем алгоритм вызывается рекурсивно для двух полученных частей.
 * 
 *     Для этого:
 *         - Опорный элемент переносим на самую левую позицию и исключаем эту позицию из
 *           перестановок. Работаем с отрезком [begin + 1, end - 1].
 *         - Проходим по массиву с двух сторон указателями. Левый указатель двигаем вправо
 *           до тех пор, пока он указывает на элемент, меньший опорного. Двигаем правый указатель
 *           пока значение элементов больше опорного. Если нашли такие элементы - меняем их местами.
 *         - Как только указатели встретились, заканчиваем перестановки и ставим опорный элемент на
 *           свое место (место правого указателя).
 *          
 * Сложность:
 *     Временная:
 *         O(N * log N) в среднем и лучшем случае и O(N^2) в худшем случае (плохой выбор Pivot),
 *             где N - количество элементов в сортируемом массиве
 *     Пространственная:
 *         O(log N) в среднем и O(N) в худшем случае под стек рекурсивных вызовов,
 *             где N - количество элементов в сортируемом массиве
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/23815/problems/B/
 *     Посылка 2024: https://contest.yandex.ru/contest/23815/run-report/129509304/
 *     Посылка 2026: https://contest.yandex.ru/contest/23815/run-report/168183593/
 */

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <iterator>
#include <tuple>

struct Intern {
    std::string login;
    int count_task;
    int fine;
};

template <typename RandomIt, typename Key>
RandomIt Partition(RandomIt begin, RandomIt end, Key key) {
    // Прячем Pivot в начало диапазона
    RandomIt pivot_it = begin;
    auto left = std::next(begin);
    auto right = std::prev(end);

    while (left <= right) {
        while (left <= right && key(*left) < key(*pivot_it)) {
            ++left;
        }
        while (left <= right && key(*right) > key(*pivot_it)) {
            --right;
        }
        if (left <= right) {
            std::swap(*left, *right);
            ++left;
            --right;
        }
    }

    // Возвращаем pivot на свое место
    std::swap(*pivot_it, *right);
    return right;
}

template <typename RandomIt, typename Key>
void QuickSort(RandomIt begin, RandomIt end, Key key) {
    if (std::distance(begin, end) < 2) {
        return;
    }
    std::swap(*begin, *(begin + std::distance(begin, end) / 2)); // pivot
    RandomIt it_part = Partition(begin, end, key);
    QuickSort(begin, it_part, key);
    QuickSort(std::next(it_part), end, key);
}

template <typename RandomIt>
void QuickSort(RandomIt begin, RandomIt end) {
    QuickSort(begin, end, [](const auto& value){
        return value;
    });
}

std::vector<Intern> ReadVectorInterns(std::istream& in) {
    int n;
    in >> n;
    std::cin.ignore(); // '\n'
    std::vector<Intern> result(n);
    for (auto& intern : result) {
        in >> intern.login >> intern.count_task >> intern.fine;
    }
    return result;
}

namespace {
std::ostream& operator<<(std::ostream& out, const std::vector<Intern>& interns) {
    for (const auto& intern : interns) {
        out << intern.login << '\n';
    }
    return out;
}
} // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Intern> interns = ReadVectorInterns(std::cin);
    QuickSort(interns.begin(), interns.end()
        , [](const auto& item){
            return std::make_tuple(-item.count_task, item.fine, std::string_view{item.login});
        });
    std::cout << interns;

    return 0;
}