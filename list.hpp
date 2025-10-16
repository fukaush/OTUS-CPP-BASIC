#pragma once
#include <iostream>

namespace MyContainer {
    template<typename T>
    class Node {
        public:
            Node<T>* next = nullptr;
            Node<T>* prev = nullptr;
            T data;

            Node(T _data): data{_data} {};
            ~Node() {
                Node::link(prev, next);
            }

            static void link(Node<T>* prevNode, Node<T>* nextNode)  {
                if (prevNode) {
                    prevNode->next = nextNode;
                }

                if (nextNode) {
                    nextNode->prev = prevNode;
                }
            }
    };

    template<typename T>
    class List {
        Node<T>* first = nullptr;
        Node<T>* last = nullptr;
        size_t count = 0;

        public:
            List() = default;
            ~List() {
                while(first) {
                    Node<T> *node = first->next;

                    delete first;

                    first = node;
                }
            }
            Node<T>* front() {
                return first;
            }
            Node<T>* back() {
                return last;
            }
            Node<T>* prepend(T value, Node<T>* node);
            Node<T>* append(T value, Node<T>* node);
            Node<T>* push_back(T value);
            void erase(Node<T>* node);

            size_t size() {
                return count;
            }
            bool empty() {
                return count == 0;
            }

            template <typename T>
            friend std::ostream& operator<<(std::ostream &os, const List<T> &list);
    };

    template<typename T>
    void List<T>::erase(Node<T>* node) {
        delete node;

        count -= 1;
    }

    template<typename T>
    Node<T>* List<T>::prepend(T value, Node<T>* node) {
        if (!node) {
            return nullptr;
        }

        Node<T>* newNode = new Node(value);

        Node<T>::link(node->prev, newNode);
        Node<T>::link(newNode, node);

        count += 1;

        return newNode;
    }

    template<typename T>
    Node<T>* List<T>::append(T value, Node<T>* node) {
        if (!node) {
            return nullptr;
        }

        Node<T>* newNode = new Node(value);

        Node<T>::link(newNode, node->next);
        Node<T>::link( node, newNode);

        count += 1;

        return newNode;
    }

    template<typename T>
    Node<T>* List<T>::push_back(T value) {
        Node<T>* newNode = new Node(value);

        if (!first) {
            first = newNode;
        }

        Node<T>::link(last, newNode);

        last = newNode;
        count += 1;

        return last;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream &os, const List<T> &list) {
        Node<T>* item = list.first;

        while(item) {
            os << item->data << " ";

            item = item->next;
        }

        return os;
    }
}
