#include <windows.h>
#include <iostream>

using namespace std;

void task11(double step, double left, double right);

int main()
{
    task11(0.001, -0.5, 0.5);
}

//Tabulation of (1 + x)^(1/3)
void task11(double step, double left, double right) {
    printf("\n FUNCTION PARAMETERS: \n  step  :\033[33m %+.4f \033[0m \n  left  :\033[33m %+.4f \033[0m  \n  right :\033[33m %+.4f \033[0m\n", step, left, right);
    double x;
    for (double i = left; i < right; i += step) {
        x = 1 + i / 3 - i * i / 9 + i * i * i * 5 / 81 - i * i * i * i * 80 / 1944;
        printf("\n \033[36m %d\033[0m -> X: %+.4f | Y: %+.4f ",GetCurrentThreadId(), i, x);
    }
    printf("\n\n \033[32m FINISHED!\033[0m\n");
}
