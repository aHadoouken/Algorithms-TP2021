//Реализовать дек с динамическим зацикленным буфером.
//Требования: Дек должен быть реализован в виде класса.
#include <iostream>
#include <fstream>

using namespace std;

#define INIT_BUFF_SIZE 5

class Deck {
public:
    Deck()
            : buff_size(0), deck_size(0), deck(nullptr), head(0), tail(0) {
    }

    ~Deck() {
        delete[] deck;
    };

    void pushBack(int val) {
        if (isFull()) {
            reallocateDeck();
        }
        deck_size++;
        deck[tail] = val;
        tail = (tail + 1) % buff_size;
    }

    int popBack() {
        if (isEmpty()) {
            return -1;
        }
        if (tail == 0) {
            tail = buff_size - 1;
        } else {
            tail -= 1;
        }
        deck_size--;
        return deck[tail];
    }

    void pushFront(int val) {
        if (isFull()) {
            reallocateDeck();
        }
        if (head == 0) {
            head = buff_size - 1;
        } else {
            head -= 1;
        }
        deck_size++;
        deck[head] = val;
    }

    int popFront() {
        if (isEmpty()) {
            return -1;
        }
        int return_value = deck[head];
        head = (head + 1) % buff_size;
        deck_size--;
        return return_value;
    }

    bool isEmpty() const {
        return deck_size == 0;
    }

    bool isFull() const {
        return deck_size == buff_size;
    }

private:
    size_t buff_size;
    size_t deck_size;
    int *deck;
    int head;
    int tail;

    void reallocateDeck() {
        if (!buff_size) {
            buff_size = INIT_BUFF_SIZE;
            deck = new int[buff_size];
            return;
        }
        size_t new_buff_size = buff_size * 2;
        auto *new_deck = new int[new_buff_size];
        int val;
        int i = 0;
        while (val = popFront(), val != -1) {
            new_deck[i] = val;
            i++;
        }
        head = 0;
        tail = i;
        deck_size = i;
        buff_size = new_buff_size;
        delete[]deck;
        deck = new_deck;
    }
};

int main() {
    Deck deck;
    ifstream infile("../input3_4.txt");
    int n = 0;
    infile >> n;
    for (int i = 0; i < n; i++) {
        int op = 0, val = 0, popped_val;
        infile >> op >> val;
        switch (op) {
            case 1: {
                deck.pushFront(val);
                break;
            }
            case 2: {
                popped_val = deck.popFront();
                if (popped_val != val) {
                    cout << "NO" << endl;
                    return 0;
                }
                break;
            }
            case 3: {
                deck.pushBack(val);
                break;
            }
            case 4: {
                popped_val = deck.popBack();
                if (popped_val != val) {
                    cout << "NO" << endl;
                    return 0;
                }
                break;
            }
        }
    }
    cout << "YES" << endl;
    return 0;
}
