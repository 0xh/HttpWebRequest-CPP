//
// Created by cj on 6/5/16.
//

template<class K, class V>
class KeyValuePair {
private:
    K key;
    V value;
public:
    ~KeyValuePair() {
        //  delete key;
        // delete value;
    }

    KeyValuePair() { }


    KeyValuePair(K key, V value) {
        this->key = key;
        this->value = value;
    }


    void setKey(K key) {
        this->key = key;
    }

    void setValue(V value) {
        this->value = value;
    }

    K getKey() {
        return key;
    }

    V getValue() {
        return value;
    }
};
