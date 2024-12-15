.686p
.xmm
.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib msvcrt.lib
includelib ucrt.lib
includelib vcruntime.lib
includelib msvcprt.lib
includelib standartLib.lib
Print PROTO arg:ptr byte
Println PROTO arg:ptr byte
IToString PROTO arg:sdword
UIToString PROTO arg:dword
FToString PROTO arg:real8
BoolToString PROTO arg:sdword
Concat PROTO arg1:ptr byte, arg2:ptr byte
Absb PROTO arg:sword
CharToString PROTO arg:sdword
ExitProcess PROTO:DWORD
.STACK 4096
.DATA
real_buff REAL8 0.0
null_err BYTE "Ошибка, деление на ноль", 0
Literal_New_0__none REAL8 3.140000
Literal_1__none REAL8 2.000000
Literal_2__none DWORD 1
Literal_3__none DWORD 0
Literal_4__none DWORD 1
Literal_5__none DWORD 1
Literal_6__none DWORD 0
Literal_7__none DWORD 1
Literal_New_1__none DWORD 2
Literal_9__none DWORD 50
Literal_10__none DWORD 30
Literal_11__none REAL8 2.450000
Literal_12__none BYTE "Результат длины окружности", 0
Literal_13__none BYTE "Арифметический расчёт", 0
Literal_14__none DWORD 4
Literal_15__none DWORD 5
Literal_16__none BYTE 10, "Битовый сдвиг", 0
Literal_17__none BYTE "20>>2", 0
Literal_18__none SDWORD 20
Literal_19__none SDWORD 2
Literal_20__none BYTE "20<<2", 0
Literal_21__none SDWORD 20
Literal_22__none SDWORD 2
Literal_New_2__none SDWORD 0
Literal_24__none BYTE 10, "Счётчик", 0
Literal_25__none SDWORD 10
Literal_26__none SDWORD 1
Literal_27__none BYTE 10, "Сравнение символов", 0
Literal_28__none BYTE "a и b", 0
Literal_29__none DWORD 'a'
Literal_30__none DWORD 'b'
Literal_31__none BYTE "a и a", 0
Literal_32__none DWORD 'a'
Literal_33__none DWORD 'a'
Literal_34__none BYTE 10, "Соединение строк", 0
Literal_35__none BYTE "Hello,", 0
Literal_36__none BYTE " World", 0
Literal_37__none BYTE "Соединение символов путём конвертарции в строку",10, 0
Literal_38__none DWORD 'a'
Literal_39__none DWORD 'b'
Literal_New_3__none SDWORD 0
PI__g0 REAL8 3.140000
.CODE
option prologue:PrologueDef
option epilogue:EpilogueDef
circumference__g0 proc rad__g0_c11:REAL8
fld Literal_1__none
fld PI__g0
fmul
fld rad__g0_c11
fmul
fld rad__g0_c11
fmul
fstp real_buff
fld qword ptr [real_buff]
ret
ret
circumference__g0 endp
shiftR__g0 proc b__g0_s21:SDWORD, a__g0_s21:SDWORD
push a__g0_s21
push b__g0_s21
pop eax
pop ecx
shr eax, cl
push eax
pop eax
ret
ret
shiftR__g0 endp
shiftL__g0 proc b__g0_s31:SDWORD, a__g0_s31:SDWORD
push a__g0_s31
push b__g0_s31
pop eax
pop ecx
shl eax, cl
push eax
pop eax
ret
ret
shiftL__g0 endp
characterChar__g0 proc b__g0_c41:DWORD, a__g0_c41:DWORD
push a__g0_c41
push b__g0_c41
pop eax
pop ebx
cmp eax, ebx
je If_l0
jmp Else0
If_l0:
push Literal_2__none

pop eax
ret
jmp Endif0
Else0:
push Literal_3__none

pop eax
ret
Endif0:
push Literal_4__none

pop eax
ret
ret
characterChar__g0 endp
characterString__g0 proc b__g0_c71_S:ptr byte, a__g0_c71_S:ptr byte
local b__g0_c71[256]:BYTE, a__g0_c71[256]:BYTE
mov esi, b__g0_c71_S
lea edi, b__g0_c71
 copy_loop_0:
lodsb
stosb
test al, al
jnz copy_loop_0
mov esi, a__g0_c71_S
lea edi, a__g0_c71
 copy_loop_1:
lodsb
stosb
test al, al
jnz copy_loop_1
lea eax, a__g0_c71
push eax
lea eax, b__g0_c71
push eax
pop esi
pop edi
mov ecx, 256
repe cmpsb
jz If_l1
jmp Else1
If_l1:
push Literal_5__none

pop eax
ret
jmp Endif1
Else1:
push Literal_6__none

pop eax
ret
Endif1:
push Literal_7__none

pop eax
ret
ret
characterString__g0 endp
sum__g0 proc b__g0_s101:DWORD, a__g0_s101:DWORD
push a__g0_s101
push b__g0_s101
pop EBX
pop EAX
add EAX, EBX
push EAX

pop eax
ret
ret
sum__g0 endp
arithmetic__g0 proc b__g0_a111:DWORD, a__g0_a111:DWORD
local del__g0_a111:DWORD, result__g0_a111:DWORD
push Literal_New_1__none
pop del__g0_a111
push a__g0_a111
push b__g0_a111
push Literal_9__none
push Literal_10__none
call sum__g0
push eax
pop EBX
pop EAX
mul EAX, EBX
push EAX
pop EBX
pop EAX
add EAX, EBX
push EAX
push a__g0_a111
pop EBX
pop EAX
sub EAX, EBX
push EAX
push del__g0_a111
xor EDX,EDX
pop EBX
cmp EBX, 0
je null_exception
pop EAX
div EBX
push EDX
pop result__g0_a111
push result__g0_a111

pop eax
ret
ret
arithmetic__g0 endp
main__g0 proc
local i__g0_m121:SDWORD, counter__g0_m121:SDWORD, op__g0_m121[256]:BYTE, result__g0_m121:REAL8, str__g0_m121[256]:BYTE
fld Literal_11__none
fstp real_buff
lea eax, real_buff
push sdword ptr [eax+4]
push sdword ptr [eax]
call circumference__g0
fstp real_buff
fld real_buff
fstp result__g0_m121
lea eax, Literal_12__none
push eax
call Println
push eax
pop i__g0_m121
fld result__g0_m121
fstp real_buff
lea eax, real_buff
push sdword ptr [eax+4]
push sdword ptr [eax]
call FToString
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_13__none
push eax
call Println
push eax
pop i__g0_m121
push Literal_14__none
push Literal_15__none
call arithmetic__g0
push eax
call UIToString
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_16__none
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_17__none
push eax
call Println
push eax
pop i__g0_m121
push Literal_18__none
push Literal_19__none
call shiftR__g0
push eax
call IToString
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_20__none
push eax
call Println
push eax
pop i__g0_m121
push Literal_21__none
push Literal_22__none
call shiftL__g0
push eax
call IToString
push eax
call Println
push eax
pop i__g0_m121
push Literal_New_2__none
pop counter__g0_m121
lea eax, Literal_24__none
push eax
call Println
push eax
pop i__g0_m121
While_start0:
push counter__g0_m121
push Literal_25__none
pop ebx
pop eax
cmp eax, ebx
jle While_l0
jmp Else_While0
While_l0:
push counter__g0_m121
call IToString
push eax
call Println
push eax
pop i__g0_m121
push counter__g0_m121
push Literal_26__none
pop EBX
pop EAX
add EAX, EBX
push EAX
pop counter__g0_m121
jmp While_start0
Else_While0:
lea eax, Literal_27__none
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_28__none
push eax
call Println
push eax
pop i__g0_m121
push Literal_29__none
push Literal_30__none
call characterChar__g0
push eax
call BoolToString
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_31__none
push eax
call Println
push eax
pop i__g0_m121
push Literal_32__none
push Literal_33__none
call characterChar__g0
push eax
call BoolToString
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_34__none
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_35__none
push eax
lea eax, Literal_36__none
push eax
call Concat
push eax
pop esi
lea edi, str__g0_m121
 copy_loop_2:
lodsb
stosb
test al, al
jnz copy_loop_2
lea eax, str__g0_m121
push eax
call Println
push eax
pop i__g0_m121
lea eax, Literal_37__none
push eax
call Println
push eax
pop i__g0_m121
push Literal_38__none
call CharToString
push eax
push Literal_39__none
call CharToString
push eax
call Concat
push eax
pop esi
lea edi, op__g0_m121
 copy_loop_3:
lodsb
stosb
test al, al
jnz copy_loop_3
lea eax, op__g0_m121
push eax
call Println
push eax
pop i__g0_m121
push Literal_New_3__none

pop eax
ret

INVOKE ExitProcess, eax
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

main__g0 endp
end main__g0