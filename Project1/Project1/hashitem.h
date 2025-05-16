#pragma once
#include <string>
using namespace std;
class hash_item { // ����� ������� ���-�������
public:
    // �������� ����, �������� � ������� ���������� ������������ �� ��������� �� ��� ������
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