#include <clocale>
#include "vector.hpp"
#include "list.hpp"

using namespace MyContainer;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Vector<int> vector;

    for (int i = 0; i < 10; i++) {
        vector.push_back(i);
    }

    for (const int item : vector) {
        std::cout << item << std::endl;
    }

    std::cout << vector << std::endl;
    std::cout << "Количество элементов: " << vector.getCount() << std::endl;

    int *ptr = &vector[0];

    std::cout << *ptr << std::endl; // на экране 0

    ptr += 1;

    std::cout << *ptr << std::endl; // на экране 1

    // vector.erase(2, 1); Удаление третьего (по счёту),
    // vector.erase(4, 1); пятого
    // vector.erase(6, 1); и седьмого элементов

    // Ожидаемый результат
    vector.erase(2, 1); // 0 1 3 4 5 6 7 8 9
    vector.erase(3, 1); // 0 1 3 5 6 7 8 9
    vector.erase(4, 1); // 0 1 3 5 7 8 9

    std::cout << vector << std::endl;
    std::cout << "Количество элементов: " << vector.getCount() << std::endl;

    vector.insert(0, 10);

    std::cout << vector << std::endl;
    std::cout << "Количество элементов: " << vector.getCount() << std::endl;

    vector.insert(4, 20);

    std::cout << vector << std::endl;
    std::cout << "Количество элементов: " << vector.getCount() << std::endl;

    vector.push_back(30);

    std::cout << vector << std::endl;
    std::cout << "Количество элементов: " << vector.getCount() << std::endl;

    List<int> list;

    Node<int>* a = list.push_back(10);
    Node<int>* b = list.push_back(20);
    Node<int>* c = list.prepend(30, b);
    Node<int>* d = list.push_back(40);

    std::cout << b->prev->data << std::endl; // 30

    Vector<int> vector2;

    vector2 = vector;

    vector[0] = 11;

    std::cout << "vector: " << vector << std::endl; // 1
    std::cout << "vector2: " << vector2 << std::endl; // 0

    Vector<int> vector3;

    for (int i = 10; i < 20; i++) {
        vector3.push_back(i);
    }

    vector2 = std::move(vector3);

    std::cout << "vector2: " << vector2 << std::endl; // 0

    return 0;
}
