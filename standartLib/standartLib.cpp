#include "pch.h"
#include "framework.h"

extern "C" {
    char* _stdcall IToString(int num) {
        char* str = new char[256];
        return _itoa(num, str, 10);
    }

    char* _stdcall UIToString(unsigned int num) {
        char* str = new char[256];
        return _ultoa(num, str, 10);
    }

    char* _stdcall FToString(double num) {
        char* str = new char[256];
        sprintf(str, "%f", num);
        return str;
    }

    char* _stdcall BoolToString(bool num) {
        char* str = new char[256];
        if (num)
        {
           strcpy(str, "true");
        }else strcpy(str, "false");
        return str;
    }

    char* _stdcall CharToString(char num) {
        char* str = new char[256];
        str[0] = num;
        str[1] = '\0';
        return str;
    }

    char* _stdcall Concat(char* sec, char* first) {
        if (strlen(sec) + strlen(first) > 256)
        {
            setlocale(LC_ALL, "ru");
            std::cout << "Превышена длина строки" << std::endl;
            exit(-1);
        }
        return strcat(first, sec);
    }

    int _stdcall comp(char* str2, char* str1) {
        int res = false;
        strcmp(str1, str2) == 0 ? res = 0 : res = 1;
        return res;
    }

    int _stdcall Print(const char* str) {
        setlocale(LC_ALL, "ru");
        std::cout << str;
        return 0;
    }

    int _stdcall Println(const char* str) {
        setlocale(LC_ALL, "ru");
        std::cout << str << std::endl;
        return 0;
    }
}