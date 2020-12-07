#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

DWORD WINAPI task11(double step, double left, double right);

DWORD WINAPI test(LPVOID lparam) {
    for (int i = 0; i < 100; i++) {
        Sleep(1);
        printf("\n \033[36m %d\033[0m -> %d ", GetCurrentThreadId(), i);
    }
    printf("\n\n (\033[32m%d\033[0m) FINISHED!\n", GetCurrentThreadId());

    return 0;
}
//ass
void createThreads(int ammount);
void closeHandles(int ammount);
void runControls();

//----------------------
//GLOBAL VARIABLES
HANDLE  threads[16];
DWORD   threadID[16];
double* dataArray[16];

string              program = "C:\\Users\\illyich_\\source\\repos\\OS_LAB_5_CONTOLS\\Debug\\OS_LAB_5_CONTOLS.exe";
STARTUPINFO         si;
PROCESS_INFORMATION pi;
//----------------------


int main()
{
    int n;
    cout << "ENTER AMMOUNT OF THREADS : ";
    cin >> n;
    createThreads(n);
    runControls();
    WaitForMultipleObjects(n, threads, TRUE, INFINITE);
    //task11(0.001, -0.5, 0.5);
    closeHandles(n);
}

//Tabulation of (1 + x)^(1/3)
DWORD WINAPI task11(double step, double left, double right) {
    printf("\n FUNCTION PARAMETERS: \n  step  :\033[33m %+.4f \033[0m \n  left  :\033[33m %+.4f \033[0m  \n  right :\033[33m %+.4f \033[0m\n", step, left, right);
    double x;
    for (double i = left; i < right; i += step) {
        x = 1 + i / 3 - i * i / 9 + i * i * i * 5 / 81 - i * i * i * i * 80 / 1944;
        printf("\n \033[36m %d\033[0m -> X: %+.4f | Y: %+.4f ",GetCurrentThreadId(), i, x);
    }
    printf("\n\n \033[32m FINISHED!\033[0m\n");
    return 0;
}

void runControls() {
    string temp_prog = program;
    for(DWORD id : threadID) {
        if (id != 0) {
            temp_prog += " ";
            temp_prog += to_string(id);
        }
    }

    int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, temp_prog.c_str(), temp_prog.size(), NULL, 0);

    //SOME SHITTY CODE FOR STR TO LPWSTR CONVERTION
    LPWSTR widestr = new WCHAR[bufferlen + 1];
    ::MultiByteToWideChar(CP_ACP, 0, temp_prog.c_str(), temp_prog.size(), widestr, bufferlen);
    widestr[bufferlen] = 0;

    if (!CreateProcess(
        NULL,            // Module name 
        widestr,               // Command line
        NULL,               // Process handle inheritance
        NULL,               // Thread handle
        FALSE,              // Handle inheritance
        CREATE_NEW_CONSOLE, // Flags
        NULL,               // Use parent's environment block
        NULL,               // Use parent's starting directory 
        &si,             // Pointer to STARTUPINFO
        &pi)             // Pointer to PROCESS_INFORMATION
        )
    {
        printf("FAILED to createProcess. ERROR_CODE [%d]\n", GetLastError());
        Sleep(5000);
        return;
    }
}

void createThreads(int ammount) {
    for (int i = 0; i < ammount; i++) {
        threads[i] = CreateThread(
            NULL,
            0,
            test,
            dataArray[i],
            CREATE_SUSPENDED,
            &threadID[i]
        );
        if (threads[i] == NULL)
        {
            printf("\nClosing program\n\tCouldn`t create thread [%d]",i);
            ExitProcess(3);
        }
    }
}

void closeHandles(int ammount) {
    for (int i = 0; i < ammount; i++) {
        CloseHandle(threads[i]);
    }
}