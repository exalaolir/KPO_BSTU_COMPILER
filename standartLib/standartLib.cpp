// standartLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"

extern "C"  int _stdcall printString__g0(const char* str) {
    setlocale(LC_ALL, "ru");
    std::cout << str;
    return 0;
}

