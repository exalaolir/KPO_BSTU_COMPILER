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
Literal_New_0__none SDWORD 66
Literal_1__none REAL8 1.000000
Literal_2__none REAL8 0.000000
Literal_3__none REAL8 0.000000
Literal_4__none REAL8 0.000000
Literal_New_1__none REAL8 0.000000
Literal_7__none BYTE "@", 0
Literal_8__none DWORD '@'
Literal_New_2__none DWORD 3147483641
Literal_11__none DWORD 1
Literal_12__none DWORD 1
Literal_13__none BYTE "bool test done", 0
Literal_14__none DWORD 1
Literal_15__none DWORD 0
Literal_16__none BYTE "bool test done", 0
Literal_17__none DWORD 1
Literal_18__none DWORD 0
Literal_19__none BYTE "bool test not done", 0
Literal_20__none BYTE "abcd", 0
Literal_21__none BYTE "abcd", 0
Literal_22__none BYTE "str test not done", 0
Literal_23__none BYTE "abcd", 0
Literal_24__none BYTE "gggggggggggg", 0
Literal_25__none BYTE "str test done", 0
Literal_26__none BYTE "a", 0
Literal_27__none BYTE "g", 0
Literal_28__none BYTE "str test not done", 0
Literal_29__none BYTE "a", 0
Literal_30__none BYTE "aa", 0
Literal_31__none BYTE "str test not done", 0
Literal_32__none DWORD 'h'
Literal_33__none DWORD 'h'
Literal_34__none BYTE "char test done", 0
Literal_35__none DWORD 'h'
Literal_36__none DWORD 0Ah
Literal_37__none BYTE "char test done", 0
Literal_38__none BYTE "ggg//", 0
Literal_New_3__none SDWORD 0
.CODE
option prologue:PrologueDef
option epilogue:EpilogueDef
test__g0 proc ll__g0_t11_S:ptr byte
local ll__g0_t11[256]:BYTE
mov esi, ll__g0_t11_S
lea edi, ll__g0_t11
 copy_loop_0:
lodsb
stosb
test al, al
jnz copy_loop_0
push Literal_New_0__none

pop eax
ret
ret
test__g0 endp
main__g0 proc
local yyy__g0_m21:SDWORD, i__g0_m21_i52:SDWORD, i__g0_m21_i32:SDWORD, i__g0_m21[256]:BYTE, y__g0_m21:REAL8, a__g0_m21:REAL8, r__g0_m21:REAL8, yywy__g0_m21:SDWORD, z__g0_m21:DWORD, uio__g0_m21:SDWORD, i__g0_m21_i112:SDWORD, i__g0_m21_i42:SDWORD, i__g0_m21_i82:SDWORD, i__g0_m21_i62:SDWORD, i__g0_m21_i72:SDWORD, i__g0_m21_i92:SDWORD, i__g0_m21_i102:SDWORD
fld Literal_1__none
fld Literal_2__none
fdiv
fstp a__g0_m21
fld Literal_3__none
fld Literal_4__none
fdiv
fstp y__g0_m21
fld Literal_New_1__none
fstp r__g0_m21
lea eax, Literal_7__none
push eax
call Println
push eax
pop yyy__g0_m21
push Literal_8__none
call CharToString
push eax
call Println
push eax
pop yywy__g0_m21
push Literal_New_2__none
pop z__g0_m21
push z__g0_m21
call UIToString
push eax
call Println
push eax
pop uio__g0_m21
push Literal_11__none
push Literal_12__none
pop eax
pop ebx
cmp eax, ebx
je If_l0
jmp Else0
If_l0:
lea eax, Literal_13__none
push eax
call Println
push eax
pop i__g0_m21_i32
jmp Endif0
Else0:
Endif0:
push Literal_14__none
push Literal_15__none
pop eax
pop ebx
cmp eax, ebx
jne If_l1
jmp Else1
If_l1:
lea eax, Literal_16__none
push eax
call Println
push eax
pop i__g0_m21_i42
jmp Endif1
Else1:
Endif1:
push Literal_17__none
push Literal_18__none
pop eax
pop ebx
cmp eax, ebx
je If_l2
jmp Else2
If_l2:
lea eax, Literal_19__none
push eax
call Println
push eax
pop i__g0_m21_i52
jmp Endif2
Else2:
Endif2:
lea eax, Literal_20__none
push eax
lea eax, Literal_21__none
push eax
pop esi
pop edi
mov ecx, 256
repe cmpsb
jz If_l3
jmp Else3
If_l3:
lea eax, Literal_22__none
push eax
call Println
push eax
pop i__g0_m21_i62
jmp Endif3
Else3:
Endif3:
lea eax, Literal_23__none
push eax
lea eax, Literal_24__none
push eax
pop esi
pop edi
mov ecx, 256
repe cmpsb
jnz If_l4
jmp Else4
If_l4:
lea eax, Literal_25__none
push eax
call Println
push eax
pop i__g0_m21_i72
jmp Endif4
Else4:
Endif4:
lea eax, Literal_26__none
push eax
lea eax, Literal_27__none
push eax
pop esi
pop edi
mov ecx, 256
repe cmpsb
jz If_l5
jmp Else5
If_l5:
lea eax, Literal_28__none
push eax
call Println
push eax
pop i__g0_m21_i82
jmp Endif5
Else5:
Endif5:
lea eax, Literal_29__none
push eax
lea eax, Literal_30__none
push eax
pop esi
pop edi
mov ecx, 256
repe cmpsb
jz If_l6
jmp Else6
If_l6:
lea eax, Literal_31__none
push eax
call Println
push eax
pop i__g0_m21_i92
jmp Endif6
Else6:
Endif6:
push Literal_32__none
push Literal_33__none
pop eax
pop ebx
cmp eax, ebx
je If_l7
jmp Else7
If_l7:
lea eax, Literal_34__none
push eax
call Println
push eax
pop i__g0_m21_i102
jmp Endif7
Else7:
Endif7:
push Literal_35__none
push Literal_36__none
pop eax
pop ebx
cmp eax, ebx
jne If_l8
jmp Else8
If_l8:
lea eax, Literal_37__none
push eax
call Println
push eax
pop i__g0_m21_i112
jmp Endif8
Else8:
Endif8:
lea eax, Literal_38__none
push eax
pop esi
lea edi, i__g0_m21
 copy_loop_1:
lodsb
stosb
test al, al
jnz copy_loop_1
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