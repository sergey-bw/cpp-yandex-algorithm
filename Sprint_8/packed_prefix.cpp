/**
 * https://contest.yandex.ru/contest/26133/run-report/135531327/
 * 
 * Реализация:
 *   Наивный алгоритм. Распаковываем строки и ищем максимальный
 *   общий префикс.
 *   Если в алгоритме использовать полиномиальные хеши для сравнения
 *   префиксов, можно подобрать такие две строки, что их хеши совпадут
 *   на разных префиксах.
 * 
 *   В реализации распаковки строк, выбрал программный стек вместо
 *   рекурсии. В качестве оптимизации можно заменить стек например
 *   на вектор или дек. Это позволит иметь доступ к корню стека и
 *   завершать распаковку по достижении необходимой длинны строки.
 *    
 * Оценка временной сложности:
 *   Общая сложность алгоритма - O(N * L), где
 *      N - количество строк
 *      L - среднее количество символов в строке
 * 
 * Оценка пространственной сложности:
 *   Хранение исходных строк - O(N * L) - , где
 *      N - количество строк
 *      L - среднее количество символов в строке
 * 
 *   O(M) - дополнительная память для хранения строки в
 *   распакованном виде. Объем этой памяти может превосходить
 *   общую память, затраченную на хранение всех исходных строк.
 * 
 *   Общая пространственная сложность: O(N * L + M)
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <limits>

int ReadDigit(const std::string& line, size_t* offset) {
    int digit = 0;
    while(line[*offset] != '[') {
        digit = digit * 10 + (line[(*offset)++] - '0');
    }
    return digit;
}

void AppendN(std::string& lhs, const std::string& rhs, int count) {
    lhs.reserve(count * rhs.length()+ lhs.length());
    for (int j = 0; j < count; ++j) {
        lhs += rhs;
    }
}

std::string UnpackString(const std::string& line
    , size_t max_len = std::numeric_limits<size_t>::max()) {
    std::vector<std::pair<int, std::string>> stack;
    stack.emplace_back(1, "");
    for (size_t i = 0; i < line.size(); ++i) {
        if (stack[0].second.length() >= max_len) {
            break;
        }
        if (line[i] == ']') {
            auto [count, str] = std::move(stack.back());
            stack.pop_back();
            AppendN(stack.back().second, str, count);
        } else if (isdigit(line[i])) {
            stack.emplace_back(ReadDigit(line, &i), "");
        } else {
            stack.back().second.push_back(line[i]);
        }
    }
    return stack[0].second.substr(0
        , std::min(stack.back().second.length(), max_len));
}

std::string GetMaxCommonPrefix(const std::vector<std::string>& lines) {
    if (lines.size() < 2) {
        return lines.empty() ? "" : UnpackString(lines[0]);
    }
    std::string lhs = UnpackString(lines[0]);
    size_t max_prefix = lhs.length();
    for (size_t i = 1; i < lines.size() && max_prefix != 0; ++i) {
        std::string rhs = UnpackString(lines[i], max_prefix);
        max_prefix = std::min(max_prefix, rhs.length());
        for (size_t j = 0; j < max_prefix; ++j) {
            if (lhs[j] != rhs[j]) {
                max_prefix = j;
                break;
            }
        }
    }
    return lhs.substr(0, max_prefix);
}

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> lines(n);
    for (auto& str : lines) {
        std::cin >> str;
    }
    std::cout << GetMaxCommonPrefix(lines) << std::endl;
    return 0;
}