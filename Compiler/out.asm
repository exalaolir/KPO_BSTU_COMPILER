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
comp PROTO arg1:ptr byte, arg2:ptr byte
Absb PROTO arg:sword
CharToString PROTO arg:sdword
ExitProcess PROTO:DWORD
.STACK 4096
.DATA
real_buff REAL8 0.0
null_err BYTE "Ошибка, деление на ноль", 0
byte_err BYTE "Ошибка, повреждение типа byte", 0
Literal_New_0__none REAL8 3.140000
Literal_1__none DWORD 1
Literal_2__none DWORD 0
Literal_3__none DWORD 1
Literal_4__none DWORD 1
Literal_5__none DWORD 0
Literal_6__none DWORD 1
Literal_New_1__none DWORD 3
Literal_8__none DWORD 50
Literal_9__none DWORD 30
Literal_10__none SDWORD 34
Literal_11__none SDWORD 56
Literal_12__none SDWORD 2
Literal_13__none DWORD 4
Literal_14__none DWORD 5
Literal_15__none SDWORD 23
Literal_16__none SDWORD 45
Literal_17__none SDWORD 20
Literal_18__none SDWORD 2
Literal_19__none SDWORD 20
Literal_20__none SDWORD 2
Literal_New_2__none SDWORD 62
Literal_New_3__none SDWORD 0
PI__g0 REAL8 3.140000
.CODE
option prologue:PrologueDef
option epilogue:EpilogueDef
shiftR__g0 proc b__g0_s11:SDWORD, a__g0_s11:SDWORD
push a__g0_s11
push b__g0_s11
pop ecx
pop eax
shr eax, cl
push eax

pop eax
ret
ret
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

shiftR__g0 endp
shiftL__g0 proc b__g0_s21:SDWORD, a__g0_s21:SDWORD
push a__g0_s21
push b__g0_s21
pop ecx
pop eax
shl eax, cl
push eax

pop eax
ret
ret
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

shiftL__g0 endp
characterChar__g0 proc b__g0_c31:DWORD, a__g0_c31:DWORD
push a__g0_c31
push b__g0_c31
pop eax
pop ebx
cmp eax, ebx
je If_l0
jmp Else0
If_l0:
push Literal_1__none

pop eax
ret
jmp Endif0
Else0:
push Literal_2__none

pop eax
ret
Endif0:
push Literal_3__none

pop eax
ret
ret
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

characterChar__g0 endp
characterString__g0 proc b__g0_c61_S:ptr byte, a__g0_c61_S:ptr byte
local b__g0_c61[256]:BYTE, a__g0_c61[256]:BYTE
mov esi, b__g0_c61_S
lea edi, b__g0_c61
 copy_loop_0:
lodsb
stosb
test al, al
jnz copy_loop_0
mov esi, a__g0_c61_S
lea edi, a__g0_c61
 copy_loop_1:
lodsb
stosb
test al, al
jnz copy_loop_1
lea eax, a__g0_c61
push eax
lea eax, b__g0_c61
push eax
call comp
cmp eax, 1
jne If_l1
jmp Else1
If_l1:
push Literal_4__none

pop eax
ret
jmp Endif1
Else1:
push Literal_5__none

pop eax
ret
Endif1:
push Literal_6__none

pop eax
ret
ret
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

characterString__g0 endp
sum__g0 proc b__g0_s91:DWORD, a__g0_s91:DWORD
push a__g0_s91
push b__g0_s91
pop EBX
pop EAX
add EAX, EBX
push EAX

pop eax
ret
ret
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

sum__g0 endp
arithmetic__g0 proc b__g0_a101:DWORD, a__g0_a101:DWORD
local result__g0_a101:DWORD, del__g0_a101:DWORD
push Literal_New_1__none
pop del__g0_a101
push a__g0_a101
push b__g0_a101
push Literal_8__none
push Literal_9__none
call sum__g0
push eax
pop EBX
pop EAX
imul EAX, EBX
push EAX
pop EBX
pop EAX
add EAX, EBX
push EAX
push a__g0_a101
pop EBX
pop EAX
sub EAX, EBX
push EAX
push del__g0_a101
xor EDX,EDX
pop EBX
cmp EBX, 0
je null_exception
pop EAX
idiv EBX
push EDX
pop result__g0_a101
push result__g0_a101

pop eax
ret
ret
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

arithmetic__g0 endp
sumInt__g0 proc b__g0_s111:SDWORD, a__g0_s111:SDWORD
push a__g0_s111
push b__g0_s111
pop EBX
pop EAX
add EAX, EBX
push EAX

pop eax
ret
ret
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

sumInt__g0 endp
arithmeticsInt__g0 proc b__g0_a121:SDWORD, i__g0_a121:SDWORD
push i__g0_a121
push b__g0_a121
pop EBX
pop EAX
add EAX, EBX
push EAX
push Literal_10__none
push Literal_11__none
call sumInt__g0
push eax
pop EBX
pop EAX
imul EAX, EBX
push EAX
push b__g0_a121
pop EBX
pop EAX
sub EAX, EBX
push EAX
push Literal_12__none
xor EDX,EDX
pop EBX
cmp EBX, 0
je null_exception
pop EAX
idiv EBX
push EDX

pop eax
ret
ret
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

arithmeticsInt__g0 endp
main__g0 proc
local zz__g0_m131:SDWORD, i__g0_m131:SDWORD
push Literal_13__none
push Literal_14__none
call arithmetic__g0
push eax
call UIToString
push eax
call Println
push eax
pop i__g0_m131
push Literal_15__none
push Literal_16__none
call arithmeticsInt__g0
push eax
call IToString
push eax
call Println
push eax
pop i__g0_m131
push Literal_17__none
push Literal_18__none
call shiftR__g0
push eax
call IToString
push eax
call Println
push eax
pop i__g0_m131
push Literal_19__none
push Literal_20__none
call shiftL__g0
push eax
call IToString
push eax
call Println
push eax
pop i__g0_m131
push Literal_New_2__none
pop zz__g0_m131
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