#pragma once
#include <string>
using namespace std;
class hash_item { // класс объекта хэш-таблицы
public:
    // содержит ключ, значение и булевую переменную обозначающую не заполнена ли эта €чейка
    string key;
    string value;
    bool is_deleted;
    hash_item() {
        key = "";
        value = "";
        is_deleted = true;
    };
    hash_item(string k, string v)
    {
        key = k;
        value = v;
        is_deleted = false;
    }
};