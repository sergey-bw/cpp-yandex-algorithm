/**
 * Спринт 8. Финальное задание: B. Шпаргалка
 *
 * Постановка задачи:
 *     Дана строка текста S (шпаргалка без пробелов) и словарь из k допустимых слов.
 *     Требуется определить, можно ли разбить исходный текст на слова из словаря.
 *     Каждое слово из словаря может использоваться произвольное количество раз.
 * 
 * Формат ввода:
 *     - В первой строке записан исходный текст S, состоящий из маленьких латинских букв.
 *     - Во второй строке записано целое число k — количество слов в словаре.
 *     - В следующих k строках записаны слова из словаря по одному в строке.
 *
 * Формат вывода:
 *     - Выведите "YES", если текст можно разбить на слова из словаря, или "NO" в противном случае.
 *
 * Ограничения:
 *     - Длина текста S не превосходит 100100 символов.
 *     - Количество слов в словаре k: 1 ≤ k ≤ 100.
 *     - Длина каждого слова не превосходит 100 символов.
 *     - Все строки состоят из строчных букв латинского алфавита.
 *
 * Принцип работы (Префиксное дерево + Динамическое программирование):
 *     1. Префиксное дерево:
 *         Словарь упаковывается в префиксное дерево (Trie).
 *         В префиксном дереве выставляются терминальные узлы - помечаются последние буквы каждого слова.
 *         В качестве структуры для перехода по рёбрам выбран массив фиксированного размера
 *         std::array<Node*, 26>, что даёт строгий O(1) доступ по индексу к потомкам узла.
 *         Указатели на узлы дерева хранятся в структуре Дек для защиты от инвалидации при реаллокациях
 *         памяти при заполнении контейнеров. Стоит отметить: структура Node получилась довольно тяжелой,
 *         на x64 системе - 208 байт (в каждый чанк будет помещаться по 2 объекта).
 * 
 *     2. Динамическое программирование:
 *         DP[i] хранит информацию - можно ли составить префикс текста длины i словами из словаря.
 * 
 *         Внешний цикл перебирает стартовые позиции i (длины уже собранных префиксов).
 *         Если dp[i - 1] == false, позиция i пропускается: до неё невозможно дойти.
 *         Базовый случай: dp[0] = true (пустой префикс всегда валиден).
 * 
 *         Если dp[i - 1] == true, значит с позиции i может начаться новое слово.
 *         Запускаем внутренний цикл и спускаемся по префиксному дереву начиная с символа S[i - 1],
 *         пока есть соответствующие рёбра.
 *         Если в процессе спуска встречаем терминальный узел (слово закончилось на позиции j),
 *         помечаем dp[j] = true. Помечаем все возможные слова начинающиеся на символ S[i - 1].
 * 
 *         Ответ на задачу находится в ячейке dp[N].
 *
 * Сложность:
 *     Временная:
 *         Построение префиксного дерева: O(L)
 *             где L - суммарная длина всех слов словаря
 *         Проверка текста: O(N * M), где
 *             N - длина строки с текстом S
 *             M - максимальная длина слова в словаре (M ≤ 100)
 *         Высота префиксного дерева - соответствует максимальной длине слова в словаре (M ≤ 100)
 *         Общая временная сложность: O(L + N * M)
 *
 *     Пространственная:
 *         Префиксное дерево: O(V), где
 *             V - число вершин в префиксном дереве
 *             Константа 26 (размер алфавита) сводится к O(1)
 *         Массив динамики dp: O(N)
 *         Общая пространственная сложность: O(V + N)
 * 
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/26133/problems/B/
 *     Посылка 2025: https://contest.yandex.ru/contest/26133/run-report/135531823/
 *     Посылка 2026: https://contest.yandex.ru/contest/26133/run-report/171177914/
 */

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <deque>

class Trie {
public:
    Trie() : nodes(1, Node{}) {}

    Trie(const std::vector<std::string>& words)
        : Trie()
    {
        for (const auto& word : words) {
            InsertWord(word);
        }
    }

    void InsertWord(const std::string& word) {
        Node* curr_node = GetRoot();
        for (char c : word) {
            Node*& child_node = curr_node->children[c - 'a'];
            if (child_node == nullptr) {
                nodes.emplace_back();
                child_node = &nodes.back();
            }
            curr_node = child_node;
        }
        curr_node->is_terminal = true;
    }

    bool CanSplitTextIntoWords(const std::string& text) const noexcept {
        std::vector<bool> dp(text.length() + 1, false);
        dp[0] = true;
        for (size_t i = 1; i < dp.size() && !dp.back(); ++i) {
            if (!dp[i - 1]) {
                continue;
            }
            const Node* curr_node = GetRoot();
            for (size_t j = i; j < dp.size(); ++j) {
                curr_node = curr_node->children[text[j - 1] - 'a'];
                if (curr_node == nullptr) {
                    break;
                }
                dp[j] = dp[j] || curr_node->is_terminal;
            }
        }
        return dp.back();
    }

private:
    struct Node {
        std::array<Node*, 26> children;
        bool is_terminal = false;

        Node() {
            children.fill(nullptr);
        }
    };

    Node* GetRoot() noexcept {
        return &nodes.front(); 
    }

    const Node* GetRoot() const noexcept {
        return &nodes.front(); 
    }

    std::deque<Node> nodes;
};

std::vector<std::string> ReadWords(std::istream& in) {
    int n;
    in >> n;
    std::vector<std::string> words(n);
    for (auto& word : words) {
        in >> word;
    }
    return words;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string line;
    std::cin >> line;

    if (Trie{ReadWords(std::cin)}.CanSplitTextIntoWords(line)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    std::cout << '\n';
    return 0;
}