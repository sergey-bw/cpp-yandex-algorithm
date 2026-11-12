/**
 * Спринт 8. Финальное задание: Packed Prefix
 *
 * Постановка задачи:
 *     На вход подаётся набор строк, представленных в виде запакованной строки (ЗС).
 *     Сжатие устроено по правилу повторения блоков в скобках:
 *     запись вида k[A] означает, что подстрока A повторяется ровно k раз подряд.
 *     Коэффициент k - всегда однозначное натуральное число от 1 до 9.
 *     Конструкции могут быть вложенными друг в друга на произвольную глубину.
 *
 *     Примеры распаковки:
 *         - "3[a]"        -> "aaa"
 *         - "2[ab]c"      -> "ababc"
 *         - "2[a3[b]]"    -> 2["abbb"] -> "abbbabbb"
 *
 *     Требуется:
 *         Найти наибольший общий префикс всех n распакованных строк
 *         и вывести его в распакованном виде.
 *         Если общего префикса нет, вывести пустую строку.
 * 
 * Формат ввода:
 *     В первой строке записано число n (1 ≤ n ≤ 1000) - число строк.
 *     В следующих n строках записаны корректные запакованные строки.
 *     Гарантируется, что эти строки корректны, то есть удовлетворяют
 *     указанному рекурсивному определению.
 *
 * Формат вывода:
 *     Вывести наибольший общий префикс распакованных строк.
 *
 * Ограничения:
 *     - Число строк n: 1 ≤ n ≤ 1000.
 *     - Длина каждой строки после распаковки не превосходит 10^5 символов.
 *     - Число повторений k перед скобками: однозначное число (1 ≤ k ≤ 9).
 *     - Строки состоят из строчных букв латинского алфавита, цифр от '1' до '9'
 *       и скобок '[' и ']'.
 *
 * Принцип работы:
 *     1. Распаковка ЗС:
 *         Для распаковки ЗС используется программный стек (контейнер) для организации рекурсии.
 *         В контейнере хранятся пары <k, строка для повторения>: сколько раз повторяется строка.
 *         Добавлен ранний выход при достижении количества символов текущего общего префикса.
 * 
 *     2. Поиск наибольшего общего префикса:
 *        С целью оптимизации, строки распаковываются и сравниваются по очереди.
 *        Длина первой распакованной строки задает максимальную длину возможного общего префикса.
 *        Каждая следующая строка его уточняет. Если длина общего префикса становится
 *        равной нулю - производится ранний выход.
 *        Префиксы сравниваются посимвольно.
 *
 * Сложность:
 *     Временная:
 *         В худшем случае все n строк идентичны, длина запакованной строки равна длине распакованной,
 *         и отсечение префикса не срабатывает. В этом случае длина ЗС не учитывается, потому что
 *         добавляет лишь константный множитель к общей оценке.
 * 
 *         Общая временная сложность:
 *             O(n * L_unpacked_avg) = O(sum(L_unpacked)), где
 *                 n - количество строк,
 *                 sum(L_unpacked) - суммарная длина всех распакованных строк
 *
 *     Пространственная:
 *         Хранение входных запакованных строк:
 *             O(sum(L_packed)), где
 *                 sum(L_packed) - суммарная длина всех n строк во входном массиве
 * 
 *         Дополнительная память алгоритма:
 *             O(L_unpacked_max), где
 *                 L_unpacked_max - максимальная длина распакованной строки
 *
 *         Общая пространственная сложность:
 *             O(sum(L_packed) + L_unpacked_max)
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/26133/problems/A/
 *     Посылка 2025: https://contest.yandex.ru/contest/26133/run-report/135531327/
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <limits>

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

        if (line[i] == '[') {
            continue;
        } else if (line[i] == ']') {
            auto [count, str] = std::move(stack.back());
            stack.pop_back();
            AppendN(stack.back().second, str, count);
        } else if (isdigit(line[i])) {
            stack.emplace_back(line[i] - '0', "");
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