/**
 * Спринт 2. Финальное задание: Дек (Кольцевой буфер)
 *
 * Постановка задачи:
 *   Требуется реализовать структуру данных Дек (двусторонняя очередь) фиксированного
 *   максимального размера N.
 *   Методы push_back(x), push_front(x), pop_back(), pop_front() должны выполняться за O(1).
 *   При реализации требуется использовать кольцевой буфер.
 *   
 *   Команды:
 *   - push_back(x)  : добавить элемент x в конец дека. При переполнении вывести "error".
 *   - push_front(x) : добавить элемент x в начало дека. При переполнении вывести "error".
 *   - pop_front()   : извлечь и вывести первый элемент дека. Если дек пуст — "error".
 *   - pop_back()    : извлечь и вывести последний элемент дека. Если дек пуст — "error".
 *
 * Принцип работы:
 *   Структура реализована на базе кольцевого буфера фиксированной емкости (std::vector).
 * 
 *   Индексы index_front_ и index_back_ указывают на ячейки массива,
 *   в которые должна производиться вставка новых элементов дека
 *   (логически для дека — это пустая ячейка для вставки нового элемента).
 *   После добавления элемента индекс, соответствующий методу вставки,
 *   сдвигается на следующий "пустой" элемент массива для новой вставки.
 * 
 *   При извлечении элемента соответствующий индекс сдвигается
 *   на обратную позицию, которая будет использована для следующей вставки
 *   элемента, и по этой позиции возвращается текущий элемент.
 *
 * Сложность:
 *   - Временная:
 *     - Инициализация дека (внутреннего буфера): O(N), где N — максимальный размер дека.
 *     - Все операции (push_back, push_front, pop_back, pop_front): строго O(1).
 *     - Общее время алгоритма: O(N + M), где M — количество команд.
 *   - Пространственная:
 *     - O(N) для хранения кольцевого буфера емкостью N элементов.
 *
 * Ссылки на контест:
 *   Условие: https://contest.yandex.ru/contest/22781/problems/A/
 *   Посылка 2024: https://contest.yandex.ru/contest/22781/run-report/126854278/
 *   Посылка 2026: https://contest.yandex.ru/contest/22781/run-report/167967522/
 */

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

template <typename T>
class Deque {
public:
    explicit Deque(size_t capacity)
        : buffer_(capacity), capacity_(capacity) {
        if (capacity_ > 0) {
            index_back_ = NextIndex(index_back_);
        }
    }

    [[nodiscard]] bool is_empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] bool is_full() const noexcept {
        return size_ == capacity_;
    }

    [[nodiscard]] size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_t capacity() const noexcept {
        return capacity_;
    }

    void push_back(T value) {
        buffer_[index_back_] = std::move(value);
        index_back_ = NextIndex(index_back_);
        ++size_;
    }

    void push_front(T value) {
        buffer_[index_front_] = std::move(value);
        index_front_ = PrevIndex(index_front_);
        ++size_;
    }

    std::optional<T> pop_back() {
        if (is_empty()) {
            return std::nullopt;
        }
        --size_;
        index_back_ = PrevIndex(index_back_);
        return buffer_[index_back_];
    }

    std::optional<T> pop_front() {
        if (is_empty()) {
            return std::nullopt;
        }
        --size_;
        index_front_ = NextIndex(index_front_);
        return buffer_[index_front_];
    }

private:
    [[nodiscard]] size_t NextIndex(size_t index) const noexcept {
        return (index + 1 == capacity_) ? 0 : index + 1;
    }

    [[nodiscard]] size_t PrevIndex(size_t index) const noexcept {
        return (index == 0) ? (capacity_ - 1) : index - 1;
    }

    std::vector<T> buffer_;
    size_t capacity_ = 0;
    size_t size_ = 0;
    size_t index_front_ = 0;
    size_t index_back_ = 0;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int commands_count = 0;
    if (!(std::cin >> commands_count)) {
        return 0;
    }

    size_t capacity = 0;
    std::cin >> capacity;
    Deque<int> deque(capacity);

    std::string command;
    for (int i = 0; i < commands_count; ++i) {
        std::cin >> command;

        if (command == "push_back") {
            int val = 0;
            std::cin >> val;
            if (deque.is_full()) {
                std::cout << "error\n";
            } else {
                deque.push_back(val);
            }
        } else if (command == "push_front") {
            int val = 0;
            std::cin >> val;
            if (deque.is_full()) {
                std::cout << "error\n";
            } else {
                deque.push_front(val);
            }
        } else if (command == "pop_back") {
            if (const auto result = deque.pop_back(); result.has_value()) {
                std::cout << *result << '\n';
            } else {
                std::cout << "error\n";
            }
        } else if (command == "pop_front") {
            if (const auto result = deque.pop_front(); result.has_value()) {
                std::cout << *result << '\n';
            } else {
                std::cout << "error\n";
            }
        }
    }
    
    return 0;
}