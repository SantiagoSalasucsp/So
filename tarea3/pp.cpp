#include <iostream>
#include <string>
using namespace std;

int main() {
    string elementos[5];

    
    for (int i = 0; i < 5; i++) {
        cout << "Introduce el elemento " << i + 1 << ": ";
        cin >> elementos[i];
    }

    
    cout << "Los elementos introducidos son:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << elementos[i] << endl;
    }

    return 0;
}