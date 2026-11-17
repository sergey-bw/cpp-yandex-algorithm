/**
 * Спринт 3. Финальное задание: Эффективная быстрая сортировка
 *
 * Постановка задачи:
 *     Реализовать алгоритм быстрой сортировки без выделения O(n) дополнительной
 *     памяти для промежуточных данных (in-place).
 * 
 *     Требуется отсортировать участников по правилам:
 *         1. Чем больше решено задач, тем выше участник (по убыванию P).
 *         2. При равенстве задач - меньший штраф идет выше (по возрастанию F).
 *         3. При равенстве задач и штрафов - логины упорядочиваются лексикографически.
 * 
 * Формат ввода:
 *     В первой строке задано число участников n.
 *     В каждой из следующих n строк задана информация про одного из участников.
 *     i-й участник описывается тремя параметрами:
 *         - уникальным логином (строкой из маленьких латинских букв длиной не более 20)
 *         - числом решённых задач Pi
 *         - штрафом Fi
 * 
 * Формат вывода:
 *     Для отсортированного списка участников выведите по порядку их логины по одному в строке.
 * 
 * Ограничения:
 *     Число участников: 1 ≤ n ≤ 100000.
 *     Логин участника: строка из маленьких латинских букв длиной не более 20.
 *     Fi и Pi - целые числа, лежащие в диапазоне от 0 до 10^9.
 * 
 * Принцип работы:
 *     Используется быстрая сортировка in-place.
 *     Участники сравниваются по ключу (-P, F, login), который задаёт нужный порядок:
 *     количество задач по убыванию, штраф и логин по возрастанию.
 *
 *     Опорный элемент выбирается в середине текущего диапазона [begin, end)
 *     и переносится в его начало.
 *     Остальные элементы просматриваются двумя указателями:
 *         - Левый указатель движется вправо, пока ключ элемента меньше опорного.
 *         - Правый указатель движется влево, пока ключ элемента больше опорного.
 *         - Если указатели не пересеклись, элементы меняются местами,
 *           после чего оба указателя сдвигаются навстречу друг другу.
 * 
 *     Цель - сформировать такой массив, чтобы левее опорного элемента были все элементы
 *     со значениями меньшими опорного элемента, а правее элементы большие по значению.
 *
 *     После пересечения указателей опорный элемент меняется местами
 *     с элементом под правым указателем и занимает окончательную позицию.
 *     Затем рекурсивно сортируются части слева и справа от него.
 *          
 * Сложность:
 *     Временная:
 *         O(N * log N) в среднем и лучшем случае и O(N^2) в худшем случае (плохой выбор Pivot),
 *             где N - количество элементов в сортируемом массиве
 * 
 *     Пространственная:
 *         Хранение входного массива - O(N), где
 *             N - количество элементов во входном массиве
 * 
 *         Дополнительная память под стек рекурсивных вызовов:
 *             O(log N) в среднем и лучшем случае
 *             O(N) в худшем случае (неудачно выбирался опорный элемент)
 *  
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/23815/problems/B/
 *     Посылка 2024: https://contest.yandex.ru/contest/23815/run-report/129509304/
 *     Посылка 2026: https://contest.yandex.ru/contest/23815/run-report/172146172/
 */

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <iterator>
#include <utility>
#include <tuple>

struct Intern {
    std::string login;
    int count_task;
    int fine;
};

template <typename RandomIt, typename Key>
RandomIt Partition(RandomIt begin, RandomIt end, Key key) {
    // Опорный элемент уже находится в начале диапазона
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
    std::swap(*begin, *(begin + std::distance(begin, end) / 2)); // Прячем pivot в начало
    RandomIt it_part = Partition(begin, end, key);
    QuickSort(begin, it_part, key);
    QuickSort(std::next(it_part), end, key);
}

template <typename RandomIt>
void QuickSort(RandomIt begin, RandomIt end) {
    QuickSort(begin, end, [](const auto& value) {
        return value;
    });
}

std::vector<Intern> ReadVectorInterns(std::istream& in) {
    int n;
    in >> n;
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
            return std::make_tuple(
                -item.count_task,
                item.fine,
                std::string_view{item.login});
        });
    std::cout << interns;

    return 0;
}