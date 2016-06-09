#include <iostream>

template<class type>
class Vector {
private:

    type *items;
    int size;

public:

    Vector<type>() {
        size = 0;
    }

    Vector<type>(int arrsize) {
        size = arrsize;
        items = new type[arrsize];
    }

    bool set(int index, type &item) {
        if (index >= 0 && size > index) {
            *(items + index) = item;
            return true;
        } else return false;
    }

    ~Vector<type>() {
        clear();
    }

    type *operator[](int index) {
        return at(index);
    }

    Vector<type> operator+(const Vector<type> vec) {
        Vector<type> newvec;
        for (int i = 0; i < getSize(); i++) {
            newvec.push_back(*at(i));
        }
        for (int i = 0; i < vec.getSize(); i++) {
            newvec.push_back(*vec.at(i));
        }
        return newvec;
    }

    void operator+=(const Vector<type> vec) {
        for (int i = 0; i < vec.getSize(); i++) {
            push_back(*vec.at(i));
        }
    }

    type *at(int index) const {
        return items + index;
    }

    int getSize() const {
        return size;
    }

    type *front() {
        return items;
    }

    type *back() {
        return items + (getSize() - 1);
    }

    bool isEmpty() {
        return size == 0;
    }

    bool pop_back() {
        return remove(size - 1);
    }


    void log() {
        for (int i = 0; i < getSize(); i++) {
            std::cout << "Index: " << i << ", atAddr: " << at(i) << std::endl;
        }
    }

    void clear() {
        if (size > 0) {
            delete[] items;
            size = 0;
        }
    }

    bool insert(int index, type &item) {
        if (index > size) size = index + 1;
        else if (index == size) {
            push_back(item);
            return true;
        } else size++;
        int insertPos = 0;

        type *newarray = new type[size];
        for (int i = 0; i < size; i++) {
            if (i == index) {
                newarray[i] = item;
                insertPos++;
                continue;
            }
            newarray[i] = items[i - insertPos];
        }
        if (size - 1 > 0)
            delete[] items;
        items = new type[size];
        for (int i = 0; i < size; i++)
            items[i] = newarray[i];
        delete[] newarray;
        return true;
    }


    bool remove(int index) {
        if (0 <= index < size) {
            int skipped = 0;
            type *newArray = new type[size - 1];
            for (int i = 0; i < size; i++) {
                if (i == index) {
                    skipped++;
                    continue;
                }
                newArray[i - skipped] = items[i];
            }
            if (size - 1 > 0)
                delete[] items;
            items = new type[size - 1];
            for (int i = 0; i < size - 1; i++)
                items[i] = newArray[i];
            delete[] newArray;
            size--;
            return true;
        } else {
            return false;
        }
    }

    bool push_first(type &item) {
        return insert(0, item);
    }

    bool pop_first() {
        return remove(0);
    }

    void push_back_item(type item){
        push_back(item);
    }

    void push_back(type &item) {
        size++;
        type *newbackpack = new type[size];
        for (int i = 0; i < size - 1; i++)
            newbackpack[i] = items[i];
        newbackpack[size - 1] = item;
        if (size - 1 > 0)
            delete[] items;
        items = new type[size];
        for (int i = 0; i < size; i++)
            items[i] = newbackpack[i];
        delete[] newbackpack;
    }
};