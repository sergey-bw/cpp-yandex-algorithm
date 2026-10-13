/**
 * https://contest.yandex.ru/contest/24414/run-report/131201669/
 * 
 * Реализация:
 *   Для реализации выбрал версию с разрешением коллизий методом открытой адресации.
 *   Что у метода цепочек, что у открытой адресации есть свои плюсы и минусы.
 * 
 *   Достоинства метода с открытой адресацией:
 *      Не нужно выделять память при каждом добавлении нового элемента.
 *      Память под все элементы выделяется за один раз (не считая рехеширование).
 *      Все корзины (данные) хеш-таблицы лежат в памяти последовательно,
 *      что позволяет эффективно использовать кеш процессора. Хотя это спорно.
 *      Если использовать функцию пробирования возвращающую индексы лежащие далеко
 *      друг от друга, кеш процессора не будет столь эффективным.
 * 
 *   Минусы метода с открытой адресацией:
 *      При определенном наборе входных данных, чтобы найти нужную корзину
 *      может потребоваться O(N) операций для перебора всех корзин.
 *      Это можно отчасти сгладить держа запас по вместимости хеш таблицы,
 *      что приводит к лишнему потреблению памяти.
 * 
 *   Метод с открытой адресацией скорее выбран из интереса его реализовать,
 *   чем явно обходящим по достоинствам/недостаткам метод цепочек.
 *   Выбор конкретного метода скорей должен исходить из условий дальнейшего
 *   использования хеш таблицы.
 * 
 *   В качестве кешируемой функции ключей используется приведение типа ключа (int)
 *   к беззнаковому числу. За счет переполнения типа, отрицательные ключи
 *   оказываются в конце диапазона.
 *   Плюс такого подхода, весь диапазон int будет иметь уникальный кеш.
 *   Минусом является то, что малое изменение ключа не дает большое изменение
 *   кешированного значения.
 * 
 *   Метод пробирования выбран линейный.
 *   
 * Оценка временной сложности:
 * 
 *   Сложность операций Добавления/Поиска/Удаления оценивается как:
 *      O(h) + O(1 + a), где a - коэффициент заполнения таблицы
 *      O(h) - Сложность хеш функции = O(1) относительно хеширования int
 * 
 *   При запасе свободных корзин, алгоритм как правило работает за среднее время.
 * 
 *   Парсинг запросов - O(N*M), где
 *      N - количество запросов,
 *      M - количество символов в запросе
 * 
 * Оценка пространственной сложности:
 *   O(101501) = O(1) ?
 */

#include <iostream>
#include <vector>
#include <optional>

const size_t HASH_TABBLE_SIZE = 101501;
const size_t PROBE_CONST = 4;

class HashTabble {
public :
    bool Put(int key, int value) {
        std::optional<size_t> bucket_index = SearchBucketIndex(key);
        if (!bucket_index.has_value()) {
            if (size_ == capacity_) { // Добавлять некуда
                return false;
            }
            bucket_index = SearchBucketIndex(key, [key](const Bucket& bucket) {
                return bucket.status != BucketStatus::USED;
            });
            ++size_;
        }
        hash_tabble_[*bucket_index] = {BucketStatus::USED, key, value};
        return true;
    }

    std::optional<int> Get(int key) const noexcept {
        std::optional<size_t> bucket_index = SearchBucketIndex(key);
        if (bucket_index.has_value()) {
            return hash_tabble_[*bucket_index].value;
        }
        return std::nullopt;
    }

    std::optional<int> Delete(int key) {
        std::optional<size_t> bucket_index = SearchBucketIndex(key);
        if (bucket_index.has_value()) {
            hash_tabble_[*bucket_index].status = BucketStatus::DELETED;
            --size_;
            return hash_tabble_[*bucket_index].value;
        }
        return std::nullopt;
    }

private:
    size_t GetHash(int key) const {
        return static_cast<size_t>(key);
    }

    size_t GetBucketIndex(int key) const{
        return GetHash(key) % capacity_;
    }

    size_t GetNextProbeIndex(size_t index) const noexcept{
        return (index + PROBE_CONST) % capacity_;
    }

    enum class BucketStatus {
        FREE,
        USED,
        DELETED,
    };

    struct Bucket {
        BucketStatus status = BucketStatus::FREE;
        int key;
        int value;
    };

    template <typename Predicate>
    std::optional<size_t> SearchBucketIndex(int key, Predicate p) const noexcept{
        size_t start_index = GetBucketIndex(key);
        size_t bucket_index = start_index;
        while (!p(hash_tabble_[bucket_index])) {
            bucket_index = GetNextProbeIndex(bucket_index);
            if (bucket_index == start_index) {
                return std::nullopt;
            }
        }
        return bucket_index;
    }

    std::optional<size_t> SearchBucketIndex(int key) const noexcept{
        auto bucket_index = SearchBucketIndex(key, [key](const Bucket& bucket) {
            return bucket.status == BucketStatus::FREE
                || (bucket.status == BucketStatus::USED && bucket.key == key);
        });
        if (bucket_index.has_value()
            && hash_tabble_[*bucket_index].status != BucketStatus::FREE)
        {
            return bucket_index;
        }
        return std::nullopt;
    }

    size_t size_ = 0;
    size_t capacity_ = HASH_TABBLE_SIZE;
    std::vector<Bucket> hash_tabble_{HASH_TABBLE_SIZE};
};

int main() {
    HashTabble hash_tabble;
    int n;
    std::cin >> n;
    using namespace std::literals;
    for (int i = 0; i < n; ++i) {
        std::string command;
        std::cin >> command;
        
        if (command == "put"s) {
            int key, val;
            std::cin >> key >> val;
            if (!hash_tabble.Put(key, val)) {
                std::cout << "Error"s << '\n';
            }
            continue;
        }

        if (command == "get"s) {
            int key;
            std::cin >> key;
            if (auto result = hash_tabble.Get(key); result.has_value()) {
                std::cout << *result << '\n';
            } else {
                std::cout << "None"s << '\n';
            }
            continue;
        }

        if (command == "delete"s) {
            int key;
            std::cin >> key;
            if (auto result = hash_tabble.Delete(key); result.has_value()) {
                std::cout << *result << '\n';
            } else {
                std::cout << "None"s << '\n';
            }
            continue;
        }
    }
    return 0;
}