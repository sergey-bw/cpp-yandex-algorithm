/**
 * Спринт 1. Финальное задание: Ловкость рук
 *
 * Постановка задачи:
 *     Игра "Тренажер для скоростной печати" представляет собой поле клавиш 4x4.
 *     В каждой ячейке либо отображается цифра от 1 до 9 (момент времени, когда клавишу
 *     нужно нажать), либо точка '.' (пустая ячейка).
 *     Два игрока могут одновременно нажать максимум k клавиш каждый (суммарно 2 * k клавиш).
 *     Игроки получают 1 балл за каждый раунд (цифру t от 1 до 9), если в этот раунд
 *     количество клавиш с этой цифрой не превышает суммарное число доступных пальцев (<= 2 * k).
 *     Требуется найти максимальное количество баллов, которое могут набрать игроки.
 *
 * Принцип работы:
 *     - Вычисляем частоту появления каждой цифры от 1 до 9 на поле тренажера
 *     - Для каждой цифры проверяем условие 1 <= count(digit) <= 2 * k
 *
 * Сложность:
 *     Временная:
 *         O(1) - Подсчет частот и проход по 9 цифрам выполняются за константное время
 *     Пространственная:
 *         O(1) - массив константного размера на стеке (std::array<int, 10>)
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/22450/problems/B/
 *     Посылка 2024: https://contest.yandex.ru/contest/22450/run-report/125513986/
 *     Посылка 2026: https://contest.yandex.ru/contest/22450/run-report/168081370/
 */

#include <array>
#include <cctype>
#include <iostream>
#include <string>

constexpr size_t kMaxDigits = 10;

std::array<int, kMaxDigits> CountKeys(std::istream& in) {
    std::array<int, kMaxDigits> counts_key{};
    std::string line;
    while (std::getline(in, line)) {
        for (const char c : line) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                ++counts_key[static_cast<size_t>(c - '0')];
            }
        }
    }
    return counts_key;
}

int CalculateScore(const std::array<int, kMaxDigits>& counts_key, int k) {
    int result = 0;
    for (size_t i = 1; i < counts_key.size(); ++i) {
        if (counts_key[i] != 0 && counts_key[i] <= k) {
            ++result;
        }
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int k;
    std::cin >> k;
    std::cin.ignore(); // '\n'
    auto counts_key = CountKeys(std::cin);
    std::cout << CalculateScore(counts_key, k * 2) << '\n';

    return 0;
}