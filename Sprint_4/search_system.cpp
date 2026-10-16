/**
 * Спринт 4. Финальное задание: Поисковая система
 *
 * Постановка задачи:
 *     Имеется n документов, каждый из которых представляет собой текст из слов. По этим документам
 *     требуется построить поисковый индекс. На вход системе будут подаваться запросы.
 *     Запрос - некоторый набор слов. По запросу надо вывести 5 самых релевантных документов.
 * 
 *     Релевантность документа оценивается следующим образом: для каждого уникального слова из запроса
 *     берётся число его вхождений в документ, полученные числа для всех слов из запроса суммируются.
 *     Итоговая сумма и является релевантностью документа. Чем больше сумма, тем больше документ
 *     подходит под запрос.
 * 
 *     Сортировка документов на выдаче производится по убыванию релевантности.
 *     Если релевантности документов совпадают - то по возрастанию их порядковых номеров в базе
 *     (то есть во входных данных).
 *
 * Ограничения:
 *     - Число документов в базе не превосходит 10^4.
 *     - Максимальное число слов в документе — не более 1000.
 *     - Число запросов — не более 10^4, слов в запросе — не более 10.
 *     - Для каждого запроса необходимо вывести не более 5 наиболее релевантных документов.
 *     - При равной релевантности приоритет отдается документу с меньшим ID.
 *
 * Принцип работы:
 *       Для исключения дублирования сырого текста документов строится обратный индекс:
 *       слово -> {ID документа, количество вхождений}.
 *       Хранилище реализовано на базе хеш-таблицы:
 *       std::unordered_map<std::string, std::unordered_map<Id, Relevance>>.
 * 
 *       Запрос разбивается на уникальные слова. Для каждого слова извлекается список
 *       документов с накоплением суммарной релевантности.
 * 
 *       Для получения первых K=5 элементов с наивысшей релевантностью  используется очередь
 *       с приоритетом фиксированного размера 5 (частичная сортировка) что исключает необходимость
 *       полной сортировки всего массива найденных документов.
 *
 * Сложность:
 *     Временная:
 *         Чтение и добавление документов в поисковую систему: O(N * W * L), где
 *             N — количество документов
 *             W — среднее число слов в документе
 *             L — средняя длина слова в символах
 * 
 *         Обработка одного запроса: O(W_q * L + D_rel), где
 *             W_q — количество уникальных слов в запросе
 *             D_rel — количество найденных релевантных документов под запрос
 * 
 *         - Общая временная сложность: O(N * W * L + Q * (W_q * L + D)), где
 *              Q — общее число запросов.
 *
 *     Пространственная:
 *         Хранение документов (обратный индекс): O(W * S + D), где
 *             W — общее количество уникальных слов во всех документах,
 *             S — средняя длина слов,
 *             D - общее количество документов содержащие уникальные слова
 * 
 *         Вспомогательная память на запрос: O(W_q), где
 *             W_q - количество уникальных слов в запросе
 * 
 *         FindAllDocuments(): O(W_q + D_rel)
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/24414/problems/A/
 *     Посылка 2025: https://contest.yandex.ru/contest/24414/run-report/131201872/
 *     Посылка 2026: https://contest.yandex.ru/contest/24414/run-report/168498383/
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <iterator>
#include <queue>

const size_t MAX_TOP_DOCUMENTS = 5;

class SearchServer {
public:
    using Id = int;
    using Relevance = int;
    using Document = std::pair<Id, Relevance>;
    using Documents = std::unordered_map<Id, Relevance>;
    using DataBase = std::unordered_map<std::string, Documents>;
    using Query = std::unordered_set<std::string>;

    SearchServer() = default;

    template <typename ForwardIt>
    void AddDocument(int id, ForwardIt begin, ForwardIt end) {
        for (auto it = begin; it != end; ++it) {
            ++data_base_[std::move(*it)][id];
        }
    }

    std::vector<Document> FindAllDocuments(const Query& query) const {
        Documents documents;
        for (const auto& word : query) {
            auto it = data_base_.find(word);
            if (it != data_base_.end()) {
                for (const auto& [id, relevance] : it->second) {
                    documents[id] += relevance;
                }
            }
        }
        return {documents.begin(), documents.end()};
    }

    std::vector<Document> FindTopDocuments(const Query& query) const {
        std::priority_queue<Document> queue;
        for (const auto& document: FindAllDocuments(query)) {
            queue.emplace(-document.second, document.first);
            if (queue.size() > MAX_TOP_DOCUMENTS) {
                // Удаляет элемент с большим приоритетом
                // На вершине должен быть элемент с меньшей релевантностью
                queue.pop();
            }
        }
        // Переворачиваем порядок топ элементов
        std::vector<Document> documents;
        documents.reserve(queue.size());
        for (; !queue.empty(); queue.pop()) {
            documents.emplace_back(queue.top().second, -queue.top().first);
        }
        return {documents.rbegin(), documents.rend()};
    }

private:
    DataBase data_base_;
};

void ParseDocuments(SearchServer& search_server, std::istream& in) {
    int documents_count;
    in >> documents_count;
    in.ignore(); // '\n'
    std::istringstream is{};
    for (int id = 1; id <= documents_count; ++id) {
        std::string line;
        std::getline(in, line);
        is.clear();
        is.str(line);
        search_server.AddDocument(id
            , std::istream_iterator<std::string>{is}
            , std::istream_iterator<std::string>{});
    }
}

SearchServer::Query ParseQuery(std::istringstream is) {
    return {
        std::istream_iterator<std::string>{is}
        , std::istream_iterator<std::string>{}
    };
}

namespace {
std::ostream& operator<<(std::ostream& out, const std::vector<SearchServer::Document>& vec) {
    for (const auto& [id, _] : vec) {
        out << id << ' ';
    }
    return out;
}
} // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    SearchServer search_server;
    ParseDocuments(search_server, std::cin);

    int query_count;
    std::cin >> query_count;
    std::cin.ignore(); // '\n'
    std::istringstream is{};
    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(std::cin, line);
        is.clear();
        is.str(line);
        std::cout << search_server.FindTopDocuments(ParseQuery(std::move(is))) << '\n';
    }
    return 0;
}