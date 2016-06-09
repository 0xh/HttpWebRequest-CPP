#include "KeyValuePair.hpp"

//
// Created by cj on 6/5/16.
//
class HeaderMap {
    typedef KeyValuePair<char *, char *> Header;
    Header *headers;
    int size;
    char *request;
public:
    HeaderMap() {
        size = 0;
    }


    HeaderMap(char *request) {
        size = 0;
        this->request = request;
    }

    char *getRequest() {
        return request;
    }

    Header *operator[](int index) {
        return getHeader(index);
    }

    Header *getHeader(int index) {
        return (headers + index);
    }

    ~HeaderMap() {
        clear();
    }

    void clear() {
        if (size > 0) {
            delete[] headers;
        }
    }

    int getSize() const {
        return size;
    }

    bool removeHeader(int index) {
        if (isEmpty()) return false;
        Header *newHeaders = new Header[size - 1];
        int skipped = 0;
        for (int i = 0; i < size; i++) {
            if (i == index) {
                skipped++;
                continue;
            }
            newHeaders[i - skipped] = headers[i];
        }
        delete[] headers;
        headers = new Header[size - 1];
        for (int i = 0; i < size - 1; i++) {
            headers[i] = newHeaders[i];
        }
        delete[] newHeaders;
        size--;
        return true;
    }

    bool equals(char *t1, const char *t2) {
        return strncmp(t1, t2, strlen(t2)) == 0;
    }

    bool isEmpty() {
        return size == 0;
    }

    bool removeHeader(char *header) {
        if (isEmpty()) return false;
        Header *newHeaders = new Header[size - 1];
        int skipped = 0;
        bool deleted = false;
        for (int i = 0; i < size; i++) {
            if (equals(headers[i].getKey(), header)) {
                skipped++;
                deleted = true;
                continue;
            }
            newHeaders[i - skipped] = headers[i];
        }
        delete[] headers;
        headers = new Header[size - 1];
        for (int i = 0; i < size - 1; i++) {
            headers[i] = newHeaders[i];
        }
        delete[] newHeaders;
        size--;
        return deleted;
    }

    bool hasKey(char *key) {
        for (int i = 0; i < size; i++) {
            if (equals(headers[i].getKey(), key)) {
                return true;
            }
        }
        return false;
    }

    bool addHeader(char *header, char *value) {
        if (hasKey(header)) return false;
        size++;
        KeyValuePair<char*, char*> *newHeaders = new KeyValuePair<char*, char*>[size];
        for (int i = 0; i < size - 1; i++) {
            newHeaders[i] = headers[i];
        }
        KeyValuePair<char*, char*> h(header, value);

        newHeaders[size - 1] = h;
        delete[] headers;
        headers = new Header[size];
        for (int i = 0; i < size; i++) {
            headers[i] = newHeaders[i];
        }

        delete[] newHeaders;
        return true;
    }

};
