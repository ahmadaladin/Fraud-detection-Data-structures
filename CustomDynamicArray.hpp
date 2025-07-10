#ifndef CUSTOMDYNAMICARRAY_HPP
#define CUSTOMDYNAMICARRAY_HPP

#include <stdexcept>

template <typename T>
class CustomDynamicArray {
private:
    T* data;
    int capacity;
    int length;

    void resize() {
        capacity *= 2;
        T* temp = new T[capacity];
        for (int i = 0; i < length; i++)
            temp[i] = data[i];
        delete[] data;
        data = temp;
    }

public:
    CustomDynamicArray(int initialCapacity = 10)
        : capacity(initialCapacity), length(0) {
        data = new T[capacity];
    }

    // Copy constructor
    CustomDynamicArray(const CustomDynamicArray& other) {
        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];
        for (int i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    CustomDynamicArray& operator=(const CustomDynamicArray& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            length = other.length;
            data = new T[capacity];
            for (int i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~CustomDynamicArray() {
        delete[] data;
    }

    void push_back(const T& item) {
        if (length == capacity)
            resize();
        data[length++] = item;
    }

    T& get(size_t index) {
        if (index >= length)
            throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    const T& get(size_t index) const {
        if (index >= length)
            throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    int size() const {
        return length;
    }
};

#endif