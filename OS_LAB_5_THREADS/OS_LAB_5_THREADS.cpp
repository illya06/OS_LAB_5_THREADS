#include <iostream>

void task11(double step, double left, double right);

int main()
{
    std::cout << "Hello World!\n";
}

//Tabulation of (1 + x)^(1/3)
void task11(double step, double left, double right) {
    int x;
    for (int i = left; i < right; i += step) {
        x = 1 + x / 3 - x * x / 9 + x * x * x * 5 / 81 - x * x * x * x * 80 / 1944;
    }
}
