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
null_err BYTE "������, ������� �� ����", 0
Literal_New_0__none REAL8 3.140000
Literal_1__none REAL8 2.000000
Literal_2__none DWORD 1
Literal_3__none DWORD 0
Literal_4__none DWORD 1
Literal_5__none DWORD 1
Literal_6__none DWORD 0
Literal_7__none DWORD 1
Literal_New_1__none DWORD 3
Literal_9__none DWORD 50
Literal_10__none DWORD 30
Literal_11__none SDWORD 34
Literal_12__none SDWORD 56
Literal_13__none SDWORD 2
Literal_14__none BYTE ?
Literal_15__none BYTE "kkk ", 0
Literal_16__none BYTE ?
Literal_17__none BYTE "asdf", 0
Literal_18__none DWORD 456
Literal_19__none DWORD 6
Literal_20__none DWORD 4
Literal_21__none REAL8 2.450000
Literal_22__none BYTE "��������� ����� ����������", 0
Literal_23__none BYTE 10, "�������������� ������ c uint", 0
Literal_24__none DWORD 4
Literal_25__none DWORD 5
Literal_26__none BYTE 10, "�������������� ������ c int", 0
Literal_27__none SDWORD 23
Literal_28__none SDWORD 45
Literal_29__none BYTE 10, "������� �����", 0
Literal_30__none BYTE "20>>2", 0
Literal_31__none SDWORD 20
Literal_32__none SDWORD 2
Literal_33__none BYTE "20<<2", 0
Literal_34__none SDWORD 20
Literal_35__none SDWORD 2
Literal_New_2__none SDWORD 0
Literal_37__none BYTE 10, "�������", 0
Literal_38__none SDWORD 10
Literal_39__none SDWORD 1
Literal_40__none BYTE 10, "��������� ��������", 0
Literal_41__none BYTE "a � b", 0
Literal_42__none DWORD 'a'
Literal_43__none DWORD 'b'
Literal_44__none BYTE "a � a", 0
Literal_45__none DWORD 'a'
Literal_46__none DWORD 'a'
Literal_47__none BYTE 10, "���������� �����", 0
Literal_48__none BYTE "Hello,", 0
Literal_49__none BYTE " World", 0
Literal_50__none BYTE "���������� �������� ���� ������������ � ������",10, 0
Literal_51__none DWORD 'a'
Literal_52__none DWORD 'b'
Literal_New_3__none SDWORD 0
PI__g0 REAL8 3.140000
.CODE
option prologue:PrologueDef
option epilogue:EpilogueDef
countEllipseLength__g0 proc rad__g0_c11:REAL8
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
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

countEllipseLength__g0 endp
shiftR__g0 proc secondOp__g0_s21:SDWORD, firstOp__g0_s21:SDWORD
push firstOp__g0_s21
push secondOp__g0_s21
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
shiftL__g0 proc secondOp__g0_s31:SDWORD, firstOp__g0_s31:SDWORD
push firstOp__g0_s31
push secondOp__g0_s31
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
characterChar__g0 proc secondOp__g0_c41:DWORD, firstOp__g0_c41:DWORD
push firstOp__g0_c41
push secondOp__g0_c41
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
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

characterChar__g0 endp
characterString__g0 proc secondOp__g0_c71_S:ptr byte, firstOp__g0_c71_S:ptr byte
local firstOp__g0_c71[256]:BYTE, secondOp__g0_c71[256]:BYTE
mov esi, firstOp__g0_c71_S
lea edi, firstOp__g0_c71
 copy_loop_0:
lodsb
stosb
test al, al
jnz copy_loop_0
mov esi, secondOp__g0_c71_S
lea edi, secondOp__g0_c71
 copy_loop_1:
lodsb
stosb
test al, al
jnz copy_loop_1
lea eax, firstOp__g0_c71
push eax
lea eax, secondOp__g0_c71
push eax
call comp
cmp eax, 1
jne If_l1
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
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

characterString__g0 endp
sum__g0 proc secondOp__g0_s101:DWORD, firstOp__g0_s101:DWORD
push firstOp__g0_s101
push secondOp__g0_s101
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
arithmetic__g0 proc secondOp__g0_a111:DWORD, firstOp__g0_a111:DWORD
local del__g0_a111:DWORD, result__g0_a111:DWORD
push Literal_New_1__none
pop del__g0_a111
push firstOp__g0_a111
push secondOp__g0_a111
push Literal_9__none
push Literal_10__none
call sum__g0
push eax
pop EBX
pop EAX
mul EBX
push EAX
pop EBX
pop EAX
add EAX, EBX
push EAX
push firstOp__g0_a111
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
null_exception:
lea eax, null_err
push eax
call Println
pop eax
INVOKE ExitProcess, -1

arithmetic__g0 endp
sumInt__g0 proc secondOp__g0_s121:SDWORD, firstOp__g0_s121:SDWORD
push firstOp__g0_s121
push secondOp__g0_s121
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
arithmeticsInt__g0 proc secondOp__g0_a131:SDWORD, firstOp__g0_a131:SDWORD
push firstOp__g0_a131
push secondOp__g0_a131
pop EBX
pop EAX
add EAX, EBX
push EAX
push Literal_11__none
push Literal_12__none
call sumInt__g0
push eax
pop EBX
pop EAX
imul EAX, EBX
push EAX
push secondOp__g0_a131
pop EBX
pop EAX
sub EAX, EBX
push EAX
push Literal_13__none
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
local functionStatus__g0_m141:SDWORD, x__g0_m141:SDWORD, result__g0_m141:REAL8, counter__g0_m141:SDWORD, h__g0_m141[256]:BYTE, a__g0_m141[256]:BYTE, str__g0_m141[256]:BYTE, op__g0_m141[256]:BYTE
lea eax, Literal_14__none
push eax
pop esi
lea edi, a__g0_m141
 copy_loop_2:
lodsb
stosb
test al, al
jnz copy_loop_2
lea eax, Literal_15__none
push eax
pop esi
lea edi, a__g0_m141
 copy_loop_3:
lodsb
stosb
test al, al
jnz copy_loop_3
lea eax, a__g0_m141
push eax
call Println
push eax
pop x__g0_m141
lea eax, Literal_16__none
push eax
pop esi
lea edi, h__g0_m141
 copy_loop_4:
lodsb
stosb
test al, al
jnz copy_loop_4
lea eax, h__g0_m141
push eax
lea eax, Literal_17__none
push eax
call Concat
push eax
call Println
push eax
pop x__g0_m141
push Literal_18__none
push Literal_19__none
pop ecx
pop eax
shr eax, cl
push eax
push Literal_20__none
pop ecx
pop eax
shl eax, cl
push eax
call UIToString
push eax
call Println
push eax
pop x__g0_m141
fld Literal_21__none
fstp real_buff
lea eax, real_buff
push sdword ptr [eax+4]
push sdword ptr [eax]
call countEllipseLength__g0
fstp real_buff
fld real_buff
fstp result__g0_m141
lea eax, Literal_22__none
push eax
call Println
push eax
pop functionStatus__g0_m141
fld result__g0_m141
fstp real_buff
lea eax, real_buff
push sdword ptr [eax+4]
push sdword ptr [eax]
call FToString
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_23__none
push eax
call Println
push eax
pop functionStatus__g0_m141
push Literal_24__none
push Literal_25__none
call arithmetic__g0
push eax
call UIToString
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_26__none
push eax
call Println
push eax
pop functionStatus__g0_m141
push Literal_27__none
push Literal_28__none
call arithmeticsInt__g0
push eax
call IToString
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_29__none
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_30__none
push eax
call Println
push eax
pop functionStatus__g0_m141
push Literal_31__none
push Literal_32__none
call shiftR__g0
push eax
call IToString
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_33__none
push eax
call Println
push eax
pop functionStatus__g0_m141
push Literal_34__none
push Literal_35__none
call shiftL__g0
push eax
call IToString
push eax
call Println
push eax
pop functionStatus__g0_m141
push Literal_New_2__none
pop counter__g0_m141
lea eax, Literal_37__none
push eax
call Println
push eax
pop functionStatus__g0_m141
While_start0:
push counter__g0_m141
push Literal_38__none
pop ebx
pop eax
cmp eax, ebx
jle While_l0
jmp Else_While0
While_l0:
push counter__g0_m141
call IToString
push eax
call Println
push eax
pop functionStatus__g0_m141
push counter__g0_m141
push Literal_39__none
pop EBX
pop EAX
add EAX, EBX
push EAX
pop counter__g0_m141
jmp While_start0
Else_While0:
lea eax, Literal_40__none
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_41__none
push eax
call Println
push eax
pop functionStatus__g0_m141
push Literal_42__none
push Literal_43__none
call characterChar__g0
push eax
call BoolToString
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_44__none
push eax
call Println
push eax
pop functionStatus__g0_m141
push Literal_45__none
push Literal_46__none
call characterChar__g0
push eax
call BoolToString
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_47__none
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_48__none
push eax
lea eax, Literal_49__none
push eax
call Concat
push eax
pop esi
lea edi, str__g0_m141
 copy_loop_5:
lodsb
stosb
test al, al
jnz copy_loop_5
lea eax, str__g0_m141
push eax
call Println
push eax
pop functionStatus__g0_m141
lea eax, Literal_50__none
push eax
call Println
push eax
pop functionStatus__g0_m141
push Literal_51__none
call CharToString
push eax
push Literal_52__none
call CharToString
push eax
call Concat
push eax
pop esi
lea edi, op__g0_m141
 copy_loop_6:
lodsb
stosb
test al, al
jnz copy_loop_6
lea eax, op__g0_m141
push eax
call Println
push eax
pop functionStatus__g0_m141
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