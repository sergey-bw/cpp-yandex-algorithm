/**
 * Спринт 5. Финальное задание: Пирамидальная сортировка
 *
 * Постановка задачи:
 *     Требуется отсортировать участников соревнования по следующим правилам:
 *         - Чем больше решено задач, тем выше участник.
 *         - При равном количестве решённых задач выше участник с меньшим штрафом.
 *         - При равенстве количества задач и штрафа логины сравниваются лексикографически.
 *
 *     Для сортировки необходимо реализовать алгоритм пирамидальной сортировки.
 *     В результате требуется вывести логины участников в полученном порядке.
 * 
 * Ограничения:
 *     В первой строке задано число участников n, 1 ≤ n ≤ 100000.
 *     В каждой из следующих n строк задана информация про одного из участников.
 *     i-й участник описывается тремя параметрами:
 *         - уникальным логином (строкой из маленьких латинских букв длиной не более 20)
 *         - числом решённых задач Pi
 *         - штрафом Fi
 *     Pi и Fi целые числа, лежащие в диапазоне от 0 до 10^9.
 *     Для отсортированного списка участников выведите по порядку их логины по одному в строке.
 *
 * Принцип работы:
 *     Выбрал реализацию пирамидальной сортировки методом inline,
 *     без выделения дополнительной памяти под массив элементов.
 * 
 *     В исходном векторе перестановками элементов стоим валидную кучу.
 *     Для этого для всех вершин начиная с уровня h-1 до 1
 *     делаем просеивание вниз.
 * 
 *     Затем идет этап сортировки. На вершине кучи всегда находится
 *     максимальный по значению элемент. Меняем его местами с
 *     последним элементом кучи и уменьшаем размер кучи на единицу.
 *     Просеиваем первый элемент кучи вниз.
 *     Получаем снова валидную кучу размером (N - 1), а в конце массива,
 *     за пределами размера кучи отсортированный элемент.
 *     Повторяем итерацию для всех элементов кучи.
 *
 * Сложность:
 *     Временная:
 *         Функция SiftDown() - в худшем случае O(H), где H высота кучи.
 *         Так как куча является сбалансированным бинарным деревом,
 *         сложность можно выразить как O(log(N)) в худшем случае.
 * 
 *         Функция MakeHeap() - O(N), где N количество элементов кучи.
 *  
 *         Функция SortHeap - O(N) + O(N/2 * log(N)) = O(N * log(N)), где
 *            N - количество сортируемых элементов.
 * 
 *         Общая сложность O(N * log(N, M)), где
 *            N - количество сортируемых элементов,
 *            M - количество символов в сравниваемых строках
 *
 *     Пространственная:
 *         Алгоритм сортировки O(1)
 * 
 *     Хранения исходного вектора O(N * M), где
 *         N - количество элементов,
 *         M - количество символов в строках
 *
 * Ссылки на контест:
 *     Посылка 2025: https://contest.yandex.ru/contest/24810/run-report/132557302/
 */

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <cstddef>
#include <iterator>
#include <tuple>
#include <utility>

template <typename RandomIt, typename Key>
void SiftDown(RandomIt begin, size_t index, size_t size, Key key) {
    while (true) {
        size_t left_index = 2 * (index + 1) - 1;
        size_t right_index = 2 * (index + 1);

        if (left_index >= size) {
            break;
        }

        size_t index_largest = left_index;
        if (right_index < size
            && key(*(begin + right_index)) > key(*(begin + left_index)))
        {
            index_largest = right_index; 
        }

        if (key(*(begin + index_largest)) > key(*(begin + index))) {
            std::swap(*(begin + index), *(begin + index_largest));
            index = index_largest;
            continue;
        }
       break;
    }
}

template <typename RandomIt, typename Key>
void MakeHeap(RandomIt begin, size_t size, Key key) {
    for (int index = size / 2; index >= 0; --index) {
        SiftDown(begin, index, size, key);
    }
}

template <typename RandomIt, typename Key>
void SortHeap(RandomIt begin, RandomIt end, Key key) {
    size_t size = std::distance(begin, end);
    if (size < 2) {
        return;
    }
    MakeHeap(begin, size, key);
    for (int idx_end = size - 1; idx_end > 0; --idx_end) {
        std::swap(*begin, *(begin + idx_end));
        SiftDown(begin, 0, idx_end, key);
    }
}

struct Intern {
    std::string login;
    int count_task;
    int fine;
};

std::vector<Intern> ReadVectorInterns(std::istream& in) {
    int n;
    in >> n;
    std::cin.ignore(); // '\n'
    std::vector<Intern> result(n);
    std::istringstream is{};
    for (auto& intern : result) {
        std::string line;
        std::getline(in, line);
        is.clear();
        is.str(line);
        is >> intern.login >> intern.count_task >> intern.fine;
    }
    return result;
}

namespace {
std::ostream& operator<<(std::ostream& out, std::vector<Intern> interns) {
    for (const auto& intern : interns) {
        out << intern.login << '\n';
    }
    return out;
}
} // namespace

int main() {
    std::vector<Intern> interns = ReadVectorInterns(std::cin);
    SortHeap(interns.begin(), interns.end()
        , [](const auto& item){
            return std::tuple<int, int, std::string_view>(
                -item.count_task
                , item.fine
                , item.login
            );
       });
    std::cout << interns;
}