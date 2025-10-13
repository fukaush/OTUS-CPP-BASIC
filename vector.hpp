#pragma once
#include <iostream>

template<typename T>
struct Iterator {
    Iterator(T* _ptr) : ptr{_ptr} {}

    T& operator*() const {
        return *ptr;
    }
    T* operator->() {
        return ptr;
    }

    // Prefix increment
    Iterator& operator++() {
        ptr++;

        return *this;
    }
    // Postfix increment
    Iterator operator++(T) {
        Iterator tmp = *this;

        ptr++;

        return tmp;
    }

    friend bool operator== (const Iterator& a, const Iterator& b) {
        return a.ptr == b.ptr;
    };
    friend bool operator!= (const Iterator& a, const Iterator& b) {
        return a.ptr != b.ptr;
    };

    private:
        T* ptr;
};

namespace MyContainer {
    template<typename T>
    class Vector {
        T *items = nullptr;
        size_t size = 0;

        void setCount(size_t count) {
            size = count;
        }
        inline bool isInRange(size_t index) const {
            return index < size;
        }
    public:
        Vector() = default;
        Vector(const Vector &other) {
            T *newRegion = new T[other.size];

            for (size_t i = 0; i < other.size; i++) {
                newRegion[i] = other[i];
            }

            items = newRegion;
            size = other.size;
        }
        Vector& operator=(const Vector& other) {
            if (this == &other) {
                return *this;
            }

            delete[] items;

            T *newRegion = new T[other.size];

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

            delete[] items;

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

        template<typename T>
        friend std::ostream& operator<<(std::ostream &os, const Vector<T> &vector);

        T& operator[](size_t index) const {
            if (!isInRange(index)) {
                throw std::out_of_range("index вне диапазона items");
            }

            return items[index];
        }


        Iterator<T> begin() {
            return Iterator(items);
        }
        Iterator<T> end() {
            return Iterator(&items[size]);
        }
    };

    template<typename T>
    void Vector<T>::insert(size_t index, T value) {
        if (size <= index) {
            throw std::out_of_range("index вне диапазона items");
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
        if (!isInRange(index)) {
            throw std::out_of_range("index вне диапазона items");
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
