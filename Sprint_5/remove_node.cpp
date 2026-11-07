/**
 * Спринт 5. Финальное задание: Удали узел
 *
 * Постановка задачи:
 *     Дано бинарное дерево поиска с уникальными целочисленными ключами.
 *     Необходимо удалить вершину с заданным ключом так, чтобы дерево осталось
 *     корректным бинарным деревом поиска. Если ключа в дереве нет, структура
 *     не изменяется. Функция принимает корень дерева и ключ, возвращая корень
 *     модифицированного дерева. Создавать новые вершины запрещено.
 * 
 * Ограничения:
 *     - Ключи дерева - натуральные числа, не превышающие 10^9.
 *     - Запрещено выделение динамической памяти под новые вершины.
 *     - Временная сложность операции удаления: O(H), где H - высота дерева.
 *
 * Принцип работы (Алгоритм Хиббарда с физической перелинковкой узлов):
 *     1. Выполняется стандартный спуск по BST до узла с искомым ключом.
 *     2. Если у удаляемого узла 0 или 1 потомок, он замещается своим единственным
 *        поддеревом (или nullptr).
 *     3. Если у узла два потомка, в левом поддереве находится максимальный узел (predecessor),
 *        его значение должно предшествовать значению текущего корня. Он вырезается из
 *        своей исходной позиции и встает на место удаляемого узла.
 *        У преемника гарантированно не будет правого ребенка. Левый ребенок должен
 *        занять место преемника в дереве.
 *     4. Извлечённый преемник наследует обоих детей удаляемого узла и встаёт
 *        на его место в дереве.
 *
 * Сложность:
 *     Временная:
 *         O(H), где H - высота дерева.
 *         Для сбалансированного дерева - O(log N), для вырожденного - O(N),
 *             где N - общее число вершин
 *
 *     Пространственная:
 *         O(H) под стек рекурсивных вызовов функции remove
 *         Для сбалансированного дерева - O(log N), для вырожденного - O(N),
 *             где N - общее число вершин
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/24810/problems/B/
 *     Посылка 2025: https://contest.yandex.ru/contest/24810/run-report/132558667/
 *     Посылка 2026: https://contest.yandex.ru/contest/24810/run-report/170815514/
 */

#ifndef REMOTE_JUDGE
struct Node {  
  int value;  
  Node* left = nullptr;  
  Node* right = nullptr;
  Node(Node* left, Node* right, int value) : value(value), left(left), right(right) {}
};  
#endif
#ifdef REMOTE_JUDGE
#include "solution.h"
#endif
#include <cassert>

// Извлекает максимальный узел из поддерева и корректно перелинковывает его детей
Node* ExtractMaxNode(Node* root, Node*& max_node) {
    if (root->right == nullptr) {
        max_node = root;
        return root->left; // На место максимума встаёт его левый ребёнок
    }
    root->right = ExtractMaxNode(root->right, max_node);
    return root;
}

Node* remove(Node* root) {
    assert(root != nullptr);

    // Случай 1: 0 или 1 ребёнок.
    if (root->left == nullptr) {
        return root->right;
    }
    if (root->right == nullptr) {
        return root->left;
    }

    // Случай 2: 2 ребёнка
    // Находим преемника (максимум слева), вырезаем его из поддерева
    // и делаем новым корнем вместо текущего root
    Node* predecessor = nullptr;
    root->left = ExtractMaxNode(root->left, predecessor);
    predecessor->left = root->left;
    predecessor->right = root->right;
    return predecessor;
}

Node* remove(Node* root, int key) {
    if (root == nullptr) {
        return nullptr;
    }
    if (key < root->value) {
        root->left = remove(root->left, key);
    } else if (key > root->value) {
        root->right = remove(root->right, key);
    } else {
        root = remove(root);
    }
    return root;
}

#ifndef REMOTE_JUDGE
void test() {
    Node node1({nullptr, nullptr, 2});
    Node node2({&node1, nullptr, 3});
    Node node3({nullptr, &node2, 1});
    Node node4({nullptr, nullptr, 6});
    Node node5({&node4, nullptr, 8});
    Node node6({&node5, nullptr, 10});
    Node node7({&node3, &node6, 5});
    Node* newHead = remove(&node7, 10);
    assert(newHead->value == 5);
    assert(newHead->right == &node5);
    assert(newHead->right->value == 8);
}

int main() {
    test();
}
#endif