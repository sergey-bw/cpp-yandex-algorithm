/**
 * Спринт 5. Финальное задание: Удали узел
 *
 * Постановка задачи:
 *     Дано бинарное дерево поиска, в котором хранятся ключи. Ключи — уникальные целые числа.
 *     Найдите вершину с заданным ключом и удалите её из дерева так, чтобы дерево осталось
 *     корректным бинарным деревом поиска. Если ключа в дереве нет, то изменять дерево не надо.
 *     На вход функции подаётся корень дерева и ключ, который надо удалить. Функция должна
 *     вернуть корень изменённого дерева. Сложность удаления узла должна составлять O(h),
 *     где h - высота дерева.
 * 
 * Ограничения:
 *     - Ключи дерева – натуральные числа, не превышающие 10^9.
 *     - Создавать новые вершины нельзя.
 *
 * Принцип работы:
 *     - Если у узла нет детей, вместо него возвращается nullptr.
 *     - Если у узла один ребёнок, он занимает место исключаемого узла.
 *     - Если у узла два ребёнка, находится максимальный узел в левом поддереве.
 *       У этого узла отсутствует правый ребёнок, поэтому к нему справа присоединяется
 *       правое поддерево исключаемого узла. Корень левого поддерева занимает место
 *       исключаемого узла.
 *
 * Сложность:
 *     Временная:
 *         O(H), где H - высота исходного дерева
 *         Для сбалансированного дерева — O(log N), для вырожденного - O(N),
 *         где N - общее количество узлов дерева
 *
 *     Пространственная:
 *         O(H) в худшем случае под стек рекурсивных вызовов
 *         Для сбалансированного дерева - O(log N), для вырожденного - O(N)
 *
 * Ссылки на контест:
 *     Условие: https://contest.yandex.ru/contest/24810/problems/B/
 *     Посылка 2025: https://contest.yandex.ru/contest/24810/run-report/132558667/
 *     Посылка 2026: https://contest.yandex.ru/contest/24810/run-report/169905199/
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
#include <utility>

Node* FindMaxNode(Node* root) {
    Node* max_node = root;
    while (max_node->right != nullptr) {
        max_node = max_node->right;
    }
    return max_node;
}

Node* remove(Node* root) {
    if (root->left != nullptr && root->right != nullptr) {
        Node* max_left = FindMaxNode(root->left);
        std::swap(max_left->right, root->right);
        return root->left;
    }
    // Оба остальных случая
    return root->left == nullptr ? root->right : root->left;
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