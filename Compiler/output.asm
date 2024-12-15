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
Literal_7__none BYTE "@|", 0
Literal_8__none DWORD '@'
Literal_New_2__none SDWORD 0
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
local yyy__g0_m21:SDWORD, y__g0_m21:REAL8, a__g0_m21:REAL8, r__g0_m21:REAL8, yywy__g0_m21:SDWORD
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