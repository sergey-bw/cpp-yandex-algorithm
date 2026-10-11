/**
 * Sprint 2. Финальное задание: Калькулятор (Обратная польская нотация)
 *
 * Постановка задачи:
 *   Задано выражение в обратной польской нотации (RPN), состоящее из целых чисел
 *   и знаков бинарных операций: '+', '-', '*', '/'.
 *   Числа и знаки операций разделены пробелами.
 *   Требуется вычислить результат выражения.
 *   Деление выполняется нацело с округлением вниз (floor division).
 *   Гарантируется, что деления на отрицательное число нет (делитель rhs > 0).
 *
 * Принцип работы:
 *   Используется стек операндов. При чтении очередного токена:
 *   - Если токен является числом, помещаем его в стек.
 *   - Если токен — бинарный оператор, извлекаем из стека два последних операнда
 *     (первым извлекается правый операнд, вторым — левый), применяем операцию
 *     и возвращаем полученный результат обратно в стек.
 *
 * Сложность:
 *   - Временная: O(N), где N — длина входной строки. Каждый токен обрабатывается за O(1).
 *   - Пространственная: O(N) суммарно для хранения входной строки и потока;
 *     дополнительная память O(M) для стека операндов, где M — количество чисел (M <= N).
 *
 * Ссылки на контест:
 *   Условие: https://contest.yandex.ru/contest/22781/problems/B/
 *   Посылка 2024: https://contest.yandex.ru/contest/22781/run-report/126860559/
 *   Посылка 2026: https://contest.yandex.ru/contest/22781/run-report/167962258/
 */

#include <cassert>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

int ExecuteOperation(char op, int lhs, int rhs) {
    switch (op) {
        case '+':
            return lhs + rhs;
        case '-':
            return lhs - rhs;
        case '*':
            return lhs * rhs;
        case '/': {
            // rhs > 0 по условию задачи.
            // При делении отрицательного числа с остатком округляем вниз (floor).
            int res = lhs / rhs;
            int rem = lhs % rhs;
            return (lhs < 0 && rem != 0) ? (res - 1) : res;
        }
        default:
            assert(false && "Unknown operator");
            return 0;
    }
}

int Calculation(const std::string& line) {
    std::istringstream stream(line);
    std::stack<int, std::vector<int>> operands;
    std::string token;

    while (stream >> token) {
        if (std::isdigit(static_cast<unsigned char>(token.back()))) {
            operands.push(std::stoi(token));
        } else {
            int rhs = operands.top();
            operands.pop();
            int lhs = operands.top();
            operands.pop();
            operands.push(ExecuteOperation(token[0], lhs, rhs));
        }
    }
    return operands.top();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string line;
    if (std::getline(std::cin, line)) {
        std::cout << Calculation(line) << "\n";
    }

    return 0;
}