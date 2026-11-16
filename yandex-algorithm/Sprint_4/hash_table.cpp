/**
 * Спринт 4. Финальное задание: Хеш-таблица
 *
 * Постановка задачи:
 *     Реализовать структуру данных - хеш-таблица.
 *    
 *     Хеш-таблица должна поддерживать следующие операции:
 *         - put(key, value) - добавление пары ключ-значение. Если заданный ключ уже есть
 *         в таблице, то соответствующее ему значение обновляется.
 *         - get(key) - получение значения по ключу. Если ключа нет в таблице,
 *         то вывести «None». Иначе вывести найденное значение. 
 *         - delete(key) – удаление ключа из таблицы. Если такого ключа нет, то вывести «None»,
 *         иначе вывести хранимое по данному ключу значение и удалить ключ.
 * 
 *     В таблице хранятся уникальные ключи.
 * 
 * Требования к реализации:
 *     Нельзя использовать имеющиеся в языках программирования реализации хеш-таблиц.
 *     Разрешать коллизии следует с помощью метода цепочек или с помощью открытой адресации.
 *     Все операции должны выполняться за O(1) в среднем.
 *     Поддерживать рехеширование и масштабирование хеш-таблицы не требуется.
 *     Ключи и значения, id сотрудников и их зарплата, – целые числа.
 *     Поддерживать произвольные хешируемые типы не требуется.
 * 
 * Формат ввода:
 *     В первой строке задано общее число запросов к таблице n (1 ≤ n ≤ 10^6).
 *     В следующих n строках записаны запросы, которые бывают трех видов – get, put, delete.
 * 
 * Ограничения:
 *     Все ключи и значения – целые числа, не превосходящие 10^9 по модулю. Числа могут быть и
 *     отрицательными в том числе.
 *     При любой последовательности команд, количество ключей в хеш-таблице не может
 *     превышать 10^5.
 *
 * Принцип работы:
 *     Для разрешения коллизий выбран метод открытой адресации с линейным пробированием.
 *
 *     Особенности и выбор метода:
 *         - В отличие от метода цепочек, все корзины и их значения расположены в памяти
 *           последовательно, обеспечивая эффективное использование кеш-памяти процессора при
 *           обходе (пробировании).
 *         - По условию задачи поддерживать рехеширование и масштабирование хеш-таблицы не требуется,
 *           поэтому размер таблицы выбран равным 101501 (простое число с учётом ограничения
 *           в 10^5 ключей), а константа шага пробирования равна 4. Так как НОД(101501, 4) = 1,
 *           пробирование гарантированно обходит абсолютно все корзины таблицы без преждевременных
 *           зацикливаний.
 *         - В качестве хеш-функции используется приведение знакового int к беззнаковому
 *           типу size_t.
 *         - При удалении (delete) корзина помечается статусом DELETED (ленивое удаление),
 *           чтобы не разрывать цепочку последующего поиска для элементов, попавших в коллизию.
 *
 * Сложность:
 *     Временная:
 *         Сложность операций Добавления/Поиска/Удаления:
 *             O(1) - в среднем случае, где вычисление хеш-функции для int занимает O(1)
 *             O(С) - в худшем случае (пробирование заполненной таблицы), где
 *                 C - вместимость таблицы (101501)
 * 
 *         Парсинг запросов - O(N * M), где
 *             N - количество запросов,
 *             M - количество символов в запросе
 * 
 *     Пространственная:
 *         O(C) - память под вектор корзин, где C = 101501
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/24414/problems/B/
 *     Посылка 2025: https://contest.yandex.ru/contest/24414/run-report/131201669/
 *     Посылка 2026: https://contest.yandex.ru/contest/24414/run-report/168188063/
 */

#include <iostream>
#include <vector>
#include <optional>

const size_t HASH_TABLE_SIZE = 101501;
const size_t PROBE_CONST = 4;

class HashTable {
public :
    bool Put(int key, int value) {
        std::optional<size_t> bucket_index = SearchBucketIndex(key);
        if (!bucket_index.has_value()) {
            if (size_ == capacity_) { // Добавлять некуда
                return false;
            }
            bucket_index = SearchBucketIndex(key, [](const Bucket& bucket) {
                return bucket.status != BucketStatus::USED;
            });
            ++size_;
        }
        hash_table_[*bucket_index] = {BucketStatus::USED, key, value};
        return true;
    }

    std::optional<int> Get(int key) const noexcept {
        std::optional<size_t> bucket_index = SearchBucketIndex(key);
        if (bucket_index.has_value()) {
            return hash_table_[*bucket_index].value;
        }
        return std::nullopt;
    }

    std::optional<int> Delete(int key) {
        std::optional<size_t> bucket_index = SearchBucketIndex(key);
        if (bucket_index.has_value()) {
            hash_table_[*bucket_index].status = BucketStatus::DELETED;
            --size_;
            return hash_table_[*bucket_index].value;
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
        while (!p(hash_table_[bucket_index])) {
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
            && hash_table_[*bucket_index].status != BucketStatus::FREE)
        {
            return bucket_index;
        }
        return std::nullopt;
    }

    size_t size_ = 0;
    size_t capacity_ = HASH_TABLE_SIZE;
    std::vector<Bucket> hash_table_{HASH_TABLE_SIZE};
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    HashTable hash_table;
    int n;
    std::cin >> n;
    using namespace std::literals;
    for (int i = 0; i < n; ++i) {
        std::string command;
        std::cin >> command;
        
        if (command == "put"s) {
            int key, val;
            std::cin >> key >> val;
            if (!hash_table.Put(key, val)) {
                std::cout << "Error"s << '\n';
            }
            continue;
        }

        if (command == "get"s) {
            int key;
            std::cin >> key;
            if (auto result = hash_table.Get(key); result.has_value()) {
                std::cout << *result << '\n';
            } else {
                std::cout << "None"s << '\n';
            }
            continue;
        }

        if (command == "delete"s) {
            int key;
            std::cin >> key;
            if (auto result = hash_table.Delete(key); result.has_value()) {
                std::cout << *result << '\n';
            } else {
                std::cout << "None"s << '\n';
            }
            continue;
        }
    }
    return 0;
}