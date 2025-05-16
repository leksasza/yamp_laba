#pragma once
#include <iostream>
#include <vector>
#include<string>
#include"hashitem.h"
using namespace std;
class hash_table {
private:
    vector<hash_item>table; // �������
    int total_elements; // ���������� ���������
    int size; // ������
    const double fill_coef = 0.8; // ����������� ���������� �������
    // �������� ��������� �������� -- ������� �����
    const int sizes[22] = { 3, 11, 19, 37, 79, 149, 281, 563, 1129, 2251, 4507, 9001, 18013,
        36007, 72019, 144013, 288007, 576001, 1152023, 2300003, 4600003, 9200017 };
    int size_index = 0; // ������ � ������� ������� ����� �� ������� �� ���������
    int get_hash(string key) { // ������� ��������� ���� �� �����
        int answ = 0;
        for (int i = 0; i < key.length(); i++)
            answ += int(key[i]);
        return answ % size;
    }
    friend class Semantic;
public:
    hash_table() { // �������� �� ���������
        total_elements = 0;
        size = get_next_size(); 
        table.resize(size);
    }
    int get_next_size() { // ���������� ��������� ������ � ������� ������� �����
        int size_ = sizes[size_index];
        size_index++;
        return size_;
    }
    void resize() { // ����� ������. ����� ����. �������� � ������� � ������ ������ ������ �������
        vector<hash_item>ht_copy = table;
        size = get_next_size();
        vector<hash_item>ht(size);
        table = ht;
        for (int i = 0; i < ht_copy.size(); i++) {
            if (!ht_copy[i].is_deleted)
                insert_element(ht_copy[i]);
        }
    }
    // ������� ��� �����
    void insert_element(hash_item new_element) {
        check_if_resize(); // ���������� � void ��� ����������� ����� �� ��������� ������� � ���� �� �� ���������� ����� ������
        int hash_key = get_hash(new_element.key);
        /* ����� ���������� �������� ������� ������������
        * �� ���� ������ ���� ������� � �������, � ���� ������ ������ ����� ����, �� ��������
        * ������ �� ��������� */
        for (int i = 0; i < size; i++) {
            int index = (hash_key + i) % size;
            if (table[index].is_deleted) {
                table[index] = new_element;
                total_elements++;
                break;
            }
        }
    }
    // ������� ������ (�������� ��� ����� ������)
    void insert_element(string key, string value) {
        check_if_resize();
        hash_item new_element(key, value);
        int hash_key = get_hash(key);
        for (int i = 0; i < size; i++) {
            int index = (hash_key + i) % size;
            if (table[index].is_deleted) {
                table[index] = new_element;
                total_elements++;
                break;
            }
        }
    }
    // ����� �������. ���������� ������
    int find_element(string key) {
        int hash_key = get_hash(key);
        for (int i = 0; i < size; i++) {
            int index = (hash_key + i) % size;
            if (!table[index].is_deleted)
                if (table[index].key == key)
                    return index;
        }
        return -1;
    }
    // ������� �������
    void delete_element(string key) {
        int index = find_element(key); // ���� �������
        if (index == -1)
            return;
        table[index].is_deleted = true; // �������� ��� ������� ����� � ��� ����� ������������
        total_elements--;
    }
    // ������ ���� ���������� ��������� ������ ������� �� ����������� ���������� (���������� 80%)
    void check_if_resize() {
        if (total_elements > size * fill_coef)
            resize();
    }
    hash_table& operator = (const hash_table& ht)
    {
        table = ht.table; 
        total_elements= ht.total_elements; 
        size=ht.size; 
        size_index = ht.size_index;
        return *this;   // ���������� ������ �� ������� ������
    }
    // ���������� ������. ������� �������, ���� � ��������
    friend ostream& operator <<(ostream& out, const hash_table& ht) {
        out << "INDEX\t\t KEY\t\t VALUE\n";
        for (int i = 0; i < ht.size; i++) {
            if (!ht.table[i].is_deleted)
                out << i << "\t\t" << ht.table[i].key << "\t\t" << ht.table[i].value << '\n';
        }
        return out;
    };
};