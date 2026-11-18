#pragma once

template <typename T>
struct Node {
    T value;
    Node<T>* next = nullptr;
};

template <typename T>
struct Array {
    T* data = nullptr;
    int size = 0;
    int capacity = 0;

    void Init() {
        capacity = 4;
        size = 0;
        data = new T[capacity];
    }

    void MPush(const T& v) {
        if (size == capacity) {
            capacity *= 2;
            T* nd = new T[capacity];
            for (int i = 0; i < size; ++i) nd[i] = data[i];
            delete[] data;
            data = nd;
        }
        data[size++] = v;
    }

    T& MGetRef(int i) { return data[i]; }
    const T& MGetRef(int i) const { return data[i]; }
    T MGet(int i) const { return data[i]; }
    int MLen() const { return size; }

    void clear() {
        delete[] data;
        data = nullptr;
        size = capacity = 0;
    }
};

template <typename T>
struct Stack {
    Node<T>* head = nullptr;

    bool SEmpty() const { return head == nullptr; }

    void SPush(const T& v) {
        head = new Node<T>{ v, head };
    }

    void SPop() {
        if (SEmpty()) return;
        Node<T>* d = head;
        head = d->next;
        delete d;
    }

    T STop() const {
        if (SEmpty()) return T{};
        return head->value;
    }

    void clear() {
        while (head) {
            Node<T>* n = head->next;
            delete head;
            head = n;
        }
    }
};
