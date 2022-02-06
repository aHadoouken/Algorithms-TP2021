//Если в числе содержится только один бит со значением 1, записать в выходной поток OK.
//Иначе записать FAIL.
//Необходимо использование битовых операций.
//Использование арифметических операций запрещено.
#include <iostream>

using namespace std;

void check_single_byte(unsigned int num) {
    int byte_count = 0;
    while (num) {
        if (num & 1) {
            byte_count++;
        }
        num = num >> 1;
    }
    if (byte_count == 1) {
        cout << "OK";
    } else {
        cout << "FAIL";
    }
}

int main() {
    unsigned int num;
    cin >> num;
    check_single_byte(num);

    return 0;
}