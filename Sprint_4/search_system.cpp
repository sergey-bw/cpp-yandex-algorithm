/**
 * https://contest.yandex.ru/contest/24414/run-report/131201872/
 * 
 * Реализация:
 *   Хранить слова всех документов очень дорого, поэтому при добавлении
 *   документов индексируем их (храним лишь уникальные слова сквозь
 *   все добавленные документы).
 *   Для этого используется хэш таблица:
 *      std::unordered_map<std::string, Documents>, где
 *      Documents = std::pair<Id, Relevance>
 *   В соответствие каждому уникальному слову хранится хэш таблица соответствий
 *      ID Документа -> Релевантность слова в документе
 * 
 *   При обработке запросов к поисковой системе проходим по всем уникальным
 *   словам запроса и собираем информацию о всех документах,
 *   где эти слова встречаются.
 *   Для выдачи ТОП 5 результатов запроса используется сортировка выбором, 
 *   т.к. нужно отсортировать лишь малую (константную) часть результатов.
 *   
 * Оценка временной сложности:
 *   Добавление документа - O(N), где N количество символов в строке
 * 
 *   Парсинг запроса - O(N), где N количество символов в запросе
 * 
 *   Поисковый запрос (FindAllDocuments) - O(N * M), где
 *      N - количество уникальных слов в запросе,
 *      M - количество документов, где это слово встречается
 * 
 *   Сортировка документов (FindTopDocuments) - O(N), константу 5 опускаем
 *                                    
 *   Общая сложность алгоритма:
 *      O(N * M), где
 *          N - количество уникальных слов в запросе,
 *          M - количество документов, где это слово встречается
 * 
 * Оценка пространственной сложности:
 *   Складываем все сложности и откидываем все лишнее
 *   O(N * M), где
 *      N - количество уникальных слов,
 *      M - количество документов
 * 
 *   В оценке пространственной сложности размер слов не участвует,
 *   т.к. они хранятся в виде хешей (числа) O(1)
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

    std::vector<Document> FindAllDocuments(const Query& query) const noexcept {
        Documents documents;
        for (const auto& word : query) {
            if (data_base_.count(word) == 0) {
                continue;
            }
            for (const auto& [id, relevance] : data_base_.at(word)) {
                documents[id] += relevance;
            }
        }
        return {documents.begin(), documents.end()};
    }

    std::vector<Document> FindTopDocuments(const Query& query) const noexcept{
        std::priority_queue<Document> queue;
        for (const auto& document: FindAllDocuments(query)) {
            queue.emplace(-document.second, document.first);
            if (queue.size() > 5) {
                // Удаляет элемент с большим приоритетом
                // На вершине должен быть элемент с меньшей релевантностью
                queue.pop();
            }
        }
        // Переворачиваем порядок топ элементов O(5)
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
    std::getchar(); // '\n'
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

std::ostream& operator<<(std::ostream& out, std::vector<SearchServer::Document> vec) {
    for (const auto& [id, _] : vec) {
        out << id << ' ';
    }
    return out;
}

int main() {
    SearchServer search_server;
    ParseDocuments(search_server, std::cin);

    int query_count;
    std::cin >> query_count;
    std::getchar(); // '\n'
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