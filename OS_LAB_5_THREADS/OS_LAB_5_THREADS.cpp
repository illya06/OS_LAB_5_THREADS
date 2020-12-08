#include <windows.h>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

//ass
void createThreads(int ammount);
void closeHandles(int ammount);
void runControls();

//----------------------
//GLOBAL VARIABLES
HANDLE  threads[16];
DWORD   threadID[16];
double  dataArray[16];

string              program = "C:\\Users\\illyich_\\source\\repos\\OS_LAB_5_CONTOLS\\Debug\\OS_LAB_5_CONTOLS.exe";
STARTUPINFO         si;
PROCESS_INFORMATION pi;

double st;
//----------------------


int main()
{
    int n;
    cout << "ENTER AMMOUNT OF THREADS : ";
    cin >> n;
    
    createThreads(n);
    runControls();
    Sleep(1000);
    auto start = std::chrono::steady_clock::now();
    WaitForMultipleObjects(n, threads, TRUE, INFINITE);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cout << endl << endl << "TIME PASSED : " << elapsed_seconds.count() << endl;
    closeHandles(n);
}

//Tabulation of (1 + x)^(1/3)
DWORD WINAPI task11(LPVOID data) {
    double* arr = (double*)data;
    double step = arr[0], left = arr[1], right = arr[2];
    printf("\n (\033[32m%d\033[0m) FUNCTION PARAMETERS: \n  step  :\033[33m %+.4f \033[0m \n  left  :\033[33m %+.4f \033[0m  \n  right :\033[33m %+.4f \033[0m\n", GetCurrentThreadId(), step, left, right);
    double x;
    Sleep(10);
    for (double i = left; i < right; i += step) {
        x = 1 + i / 3 - i * i / 9 + i * i * i * 5 / 81 - i * i * i * i * 80 / 1944;
        printf("\n \033[36m %d\033[0m -> X: %+.4f | Y: %+.4f ",GetCurrentThreadId(), i, x);
    }
    printf("\n\n (\033[32m%d\033[0m) FINISHED!\n", GetCurrentThreadId());
    return 0;
}

DWORD WINAPI name(LPVOID data) {
    int n = (int)data;
    Sleep(10);
    for (double i = 0; i < n; i++) {
        printf("\n \033[36m %d\033[0m -> Shevcuk Illia #001244012 ", GetCurrentThreadId());
    }
    printf("\n\n (\033[32m%d\033[0m) FINISHED!\n", GetCurrentThreadId());
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
        NULL,               // Module name 
        widestr,            // Command line
        NULL,               // Process handle inheritance
        NULL,               // Thread handle
        TRUE,               // Handle inheritance
        CREATE_NEW_CONSOLE, // Flags
        NULL,               // Use parent's environment block
        NULL,               // Use parent's starting directory 
        &si,                // Pointer to STARTUPINFO
        &pi)                // Pointer to PROCESS_INFORMATION
        )
    {
        printf("FAILED to createProcess. ERROR_CODE [%d]\n", GetLastError());
        Sleep(5000);
        return;
    }
}

void createThreads(int ammount) {
    int ch;
    cout << "CHOOSE : (1) - NAME | (2) - TABULATION ";
    cout << "CHOISE > ";
    cin >> ch;

    if (ch == 1) {
        int cy;
        cout << "ENTER AMMOUNTT OF CYCLES : ";
        cin >> cy;
        cy = cy / ammount;
        for (int i = 0; i < ammount; i++) {
            threads[i] = CreateThread(
                NULL,
                0,
                name,
                &cy,
                CREATE_SUSPENDED,
                &threadID[i]
            );
            if (threads[i] == NULL)
            {
                printf("\nClosing program\n\tCouldn`t create thread [%d]", i);
                ExitProcess(3);
            }
        }
    }

    if (ch == 2) {
        cout << "ENTER STEP : ";
        cin >> st;
        double left = -0.9, right = -0.9 + (1.8 / ammount), step = st;

        for (int i = 0; i < ammount; i++) {
            dataArray[0] = step;
            dataArray[1] = left;
            dataArray[2] = right;

            threads[i] = CreateThread(
                NULL,
                0,
                task11,
                dataArray,
                CREATE_SUSPENDED,
                &threadID[i]
            );
            if (threads[i] == NULL)
            {
                printf("\nClosing program\n\tCouldn`t create thread [%d]", i);
                ExitProcess(3);
            }

            left = left + (1.8 / ammount);
            right = right + (1.8 / ammount);
        }
    }
    

    
}

void closeHandles(int ammount) {
    for (int i = 0; i < ammount; i++) {
        CloseHandle(threads[i]);
    }
}