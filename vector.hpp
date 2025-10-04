#pragma once
#include <iostream>

struct Iterator {
    Iterator(int* ptr) : m_ptr{ptr} {}

    int& operator*() const {
        return *m_ptr;
    }
    int* operator->() {
        return m_ptr;
    }

    // Prefix increment
    Iterator& operator++() {
        m_ptr++;

        return *this;
    }
    // Postfix increment
    Iterator operator++(int) {
        Iterator tmp = *this;

        m_ptr++;

        return tmp;
    }

    friend bool operator== (const Iterator& a, const Iterator& b) {
        return a.m_ptr == b.m_ptr;
    };
    friend bool operator!= (const Iterator& a, const Iterator& b) {
        return a.m_ptr != b.m_ptr;
    };

    private:
        int* m_ptr;
};

namespace MyContainer {
    template<typename T>
    class Vector {
        T *items = nullptr;
        size_t size = 0;

        void setCount(size_t count) {
            size = count;
        }
    public:
        Vector() = default;
        Vector& operator=(Vector& other) {
            if (this == &other) {
                return *this;
            }

            T *newRegion = new T[100];

            for (size_t i = 0; i < other.size; i++) {
                newRegion[i] = other[i];
            }

            items = newRegion;
            size = other.size;

            return *this;
        }
        Vector& operator=(Vector&& other) {
            if (this == &other) {
                return *this;
            }

            items = other.items;
            other.items = nullptr;
            size = other.size;
            other.size = 0;

            return *this;
        }
        ~Vector() {
            delete[] items;

            items = nullptr;
        }

        size_t getCount() {
            return size;
        }
        void push_back(T value);
        void insert(size_t index, T value);
        void erase(size_t index, size_t count = 1);
        inline bool isInRange(size_t index) const {
            return index < size;
        }

        template<typename T>
        friend std::ostream& operator<<(std::ostream &os, const Vector<T> &vector);

        T& operator[] (size_t index) {
            // std::cout << "[] index: " << index << "; size" << size << std::endl;

            if (!isInRange(index)) {
                throw std::out_of_range("index вне диапазона items");
            }

            return items[index];
        }


        Iterator begin() {
            return Iterator(items);
        }
        Iterator end() {
            return Iterator(&items[size]);
        }
    };

    template<typename T>
    void Vector<T>::insert(size_t index, T value) {
        if (size <= index) {
            // Можем бросить исключение или значение по умолчанию, но пока игнорируем
            return;
        }

        const size_t newCount = size + 1;
        T *newRegion = new T[newCount];
        size_t j = 0;

        for (size_t i = 0; i < index; i++) {
            newRegion[j++] = items[i];
        }

        newRegion[j++] = value;

        for (size_t i = index; i < newCount; i++) {
            newRegion[j++] = items[i];
        }

        delete[] items;

        items = newRegion;
        size = newCount;
    }

    template<typename T>
    void Vector<T>::push_back(T value) {
        const size_t newCount = size + 1;
        T *newRegion = new T[newCount];
        size_t j = 0;

        for (size_t i = 0; i < size; i++) {
            newRegion[j++] = items[i];
        }

        newRegion[j++] = value;

        delete[] items;

        items = newRegion;
        size = newCount;
    }

    template<typename T>
    void Vector<T>::erase(size_t index, size_t count) {
        /** Здесь можем бросить исключение out of range */
        if (!isInRange(index)) {
            return;
        }

        if (!count) {
            return;
        }

        if (index + count > size) {
            count = size - index;
        }

        const size_t newCount = size - count;

        if (!newCount) {
            delete[] items;

            items = nullptr;
            size = 0;

            return;
        }

        T *newRegion = new T[newCount];
        size_t j = 0;

        for (size_t i = 0; i < index; i++) {
            newRegion[j++] = items[i];
        }

        for (size_t i = index + count; i < size; i++) {
            newRegion[j++] = items[i];
        }

        delete[] items;

        items = newRegion;
        size = newCount;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream &os, const Vector<T> &vector) {
        for (size_t i = 0; i < vector.size; i++) {
            os << vector.items[i] << " ";
        }

        return os;
    }
}
