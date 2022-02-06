/*
 * Реализуйте структуру данных типа “множество строк” на основе динамической
 * хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных
 * латинских букв. Хеш-функция строки должна быть реализована с помощью вычисления
 * значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми.
 * Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент
 * заполнения таблицы достигает 3/4. Структура данных должна поддерживать операции
 * добавления строки в множество, удаления строки из множества и проверки принадлежности
 * данной строки множеству. 1_1. Для разрешения коллизий используйте квадратичное
 * пробирование. i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. 1_2.
 * Для разрешения коллизий используйте двойное хеширование.
 */
#include <iostream>
#include <vector>
#include <fstream>

const size_t DEFAULT_CAPACITY = 8;
const float MAX_ALPHA = 3. / 4;

class StringHasher {
public:
    StringHasher(unsigned int prime = 71)
            : prime(prime) {
    }

    size_t operator()(const std::string &str) {
        size_t hash = 0;
        for (int i = 0; i < str.size(); i++) {
            hash = hash * prime + str[i];
        }
        return hash;
    }

private:
    unsigned int prime;
};

template<typename Hasher>
class HashTable {
public:
    HashTable(size_t initial_capacity = DEFAULT_CAPACITY)
            : table(initial_capacity, "NIL"), size(0) {
    }

    bool Add(const std::string &key) {
        if (size >= table.size() * MAX_ALPHA)
            grow();
        size_t hash = hasher(key) % table.size();
        size_t count = 0;
        size_t del_i;
        bool del_flag = false;
        while (table[hash] != "NIL" && count <= table.size()) {
            if (table[hash] == key) {
                return false;
            }
            if (table[hash] == "DEL" && !del_flag) {
                del_flag = true;
                del_i = hash;
            }
            hash = (hash + count + 1) % table.size();
            count++;
        }
        if (del_flag) {
            table[del_i] = key;
            size++;
            return true;
        } else if (count <= table.size()){
            table[hash] = key;
            size++;
            return true;
        }
        return false;
    }

    bool Delete(const std::string &key) {
        size_t hash = hasher(key) % table.size();
        size_t count = 0;
        while (table[hash] != key) {
            if (count > table.size()) {
                return false;
            }
            if (table[hash] == "NIL") {
                return false;
            }
            hash = (hash + count + 1) % table.size();
            count++;
        }
        table[hash] = "DEL";
        size--;
        return true;
    }

    bool Has(const std::string &key) {
        size_t hash = hasher(key) % table.size();
        size_t count = 0;
        while (table[hash] != key) {
            if (count > table.size()) {
                return false;
            }
            if (table[hash] == "NIL") {
                return false;
            }
            hash = (hash + count + 1) % table.size();
            count++;
        }
        return true;
    }
    void print(){
        for (auto &elem: table){
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

private:
    void grow() {
        std::vector<std::string> newTable(table.size() * 2, "NIL");

        for (auto &elem: table) {
            if (elem != "NIL" && elem != "DEL") {
                size_t hash = hasher(elem) % newTable.size();
                size_t count = 0;
                while (newTable[hash] != "NIL") {
                    hash = (hash + count + 1) % newTable.size();
                    count++;
                }
                newTable[hash] = elem;
            }
        }
        table = std::move(newTable);
    }

    std::vector<std::string> table;
    size_t size;
    Hasher hasher;
};

int main(int argc, const char *argv[]) {
    std::ifstream file("../input1_1.txt");
    HashTable<StringHasher> hashTable;
    char op;
    std::string key;
    while (file >> op >> key) {
        switch (op) {
            case '+': {
                std::cout << (hashTable.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-': {
                std::cout << (hashTable.Delete(key) ? "OK" : "FAIL")
                          << std::endl;
                break;
            }
            case '?': {
                std::cout << (hashTable.Has(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
        }
    }
    hashTable.print();
    return 0;
}
