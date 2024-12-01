#include "pch.h"
#include "GRB.h"
#include <string.h>

#define GRB_ERROR_SERIES 600

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'), 10,

		Rule(NS('S'), GRB_ERROR_SERIES, 7,//СТРУКТУРА КОДА

			Rule::Chain(11, TS('f'), TS('t'), TS('m'), TS('('), TS(')'), TS('{'), NS('B'), TS('r'), TS('l'), TS(';'), TS('}')),
			Rule::Chain(10, TS('f'), TS('t'), TS('m'), TS('('), TS(')'), TS('{'), TS('r'), TS('l'), TS(';'), TS('}')),

			Rule::Chain(13, TS('f'), TS('t'), TS('i'), TS('('), NS('P'), TS(')'), TS('{'), NS('B'), TS('r'), NS('R'), TS(';'), TS('}'), NS('S')),

			Rule::Chain(13, TS('f'), TS('t'), TS('i'), TS('('), NS('P'), TS(')'), TS('{'), NS('B'), TS('r'), NS('V'), TS(';'), TS('}'), NS('S')),

			Rule::Chain(12, TS('f'), TS('t'), TS('i'), TS('('), NS('P'), TS(')'), TS('{'), TS('r'), NS('R'), TS(';'), TS('}'), NS('S')),

			Rule::Chain(12, TS('f'), TS('t'), TS('i'), TS('('), NS('P'), TS(')'), TS('{'), TS('r'), NS('V'), TS(';'), TS('}'), NS('S')),

			Rule::Chain(5, TS('t'), TS('i'), NS('L'), TS(';'), NS('S'))

		),

		Rule(NS('B'), GRB_ERROR_SERIES + 1, 10,//ТЕЛО

			Rule::Chain(5, TS('t'), TS('i'), NS('L'), TS(';'), NS('B')),

			Rule::Chain(4, TS('t'), TS('i'), NS('L'), TS(';')),//3, TS('t'), TS('i'), NS('L')

			Rule::Chain(9, TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('A'), TS(')'), TS(';'), NS('B')),

			Rule::Chain(8, TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('A'), TS(')'), TS(';')),

			Rule::Chain(3, TS('i'), NS('L'), TS(';')),//2, TS('i'), NS('L')

			Rule::Chain(4, TS('i'), NS('L'), TS(';'), NS('B')),
			/// <summary>
			/// условные операторы
			/// </summary>

			Rule::Chain(7, TS('q'), TS('('), NS('U'), TS(')'), TS('{'), NS('B'), TS('}')),

			Rule::Chain(8, TS('q'), TS('('), NS('U'), TS(')'), TS('{'), NS('B'), TS('}'), NS('B')),

			Rule::Chain(11, TS('q'), TS('('), NS('U'), TS(')'), TS('{'), NS('B'), TS('}'),TS('s'), TS('{'), NS('B'), TS('}')),

			Rule::Chain(12, TS('q'), TS('('), NS('U'), TS(')'), TS('{'), NS('B'), TS('}'), TS('s'), TS('{'), NS('B'), TS('}'), NS('B'))


		),

		Rule(NS('L'), GRB_ERROR_SERIES + 2, 3,//ЛИТЕРАЛ

			Rule::Chain(2, TS('='), TS('l')),//3, TS('='), TS('l'), TS(';')

			Rule::Chain(2, TS('='), TS('i')),//3, TS('='), TS('i'), TS(';')

			Rule::Chain(2, TS('='), NS('V'))//3, TS('='), NS('V'), TS(';')
		),

		Rule(NS('P'), GRB_ERROR_SERIES + 3, 2,//ПАРАМЕТР

			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('P')),

			Rule::Chain(2, TS('t'), TS('i'))

		),
		Rule(NS('A'), GRB_ERROR_SERIES + 4, 16,//Аргументы

			Rule::Chain(1, TS('i')),

			Rule::Chain(1, TS('l')),

			Rule::Chain(2, TS('i'), NS('T')),

			Rule::Chain(2, TS('l'), NS('T')),


			Rule::Chain(4, TS('i'), TS('('), NS('A'), TS(')')),
			Rule::Chain(5, TS('i'), TS('('), NS('A'), TS(')'), NS('T')),


			Rule::Chain(4, TS('('), NS('V'), TS(')'), NS('T')),
			Rule::Chain(3, TS('('), NS('V'), TS(')')),


			Rule::Chain(3, TS('i'), TS(','), NS('A')),
			Rule::Chain(3, TS('l'), TS(','), NS('A')),

			Rule::Chain(4, TS('i'), NS('T'), TS(','), NS('A')),
			Rule::Chain(4, TS('l'), NS('T'), TS(','), NS('A')),

			Rule::Chain(6, TS('i'), TS('('), NS('A'), TS(')'), TS(','), NS('A')),
			Rule::Chain(7, TS('i'), TS('('), NS('A'), TS(')'), NS('T'), TS(','), NS('A')),

			Rule::Chain(6, TS('('), NS('V'), TS(')'), NS('T'), TS(','), NS('A')),
			Rule::Chain(5, TS('('), NS('V'), TS(')'), TS(','), NS('A'))
			
		),

		Rule(NS('V'), GRB_ERROR_SERIES + 6, 5,//начало ВЫРАЖЕНИЕ

			Rule::Chain(2, TS('i'), NS('T')),
			Rule::Chain(2, TS('l'), NS('T')),
			Rule::Chain(5, TS('i'), TS('('), NS('A'), TS(')'), NS('T')),

			Rule::Chain(3, TS('('), NS('V'), TS(')')),

			Rule::Chain(4, TS('('), NS('V'), TS(')'),NS('T'))
			
		),
		Rule(NS('T'), GRB_ERROR_SERIES + 6, 4,//ПРОДОЛЖЕНИЕ ВЫРАЖЕНИЕ

		Rule::Chain(3, TS('o'), NS('R'), NS('T')),

		Rule::Chain(5, TS('o'), TS('('), NS('V'), TS(')'), NS('T')),


		Rule::Chain(2, TS('o'), NS('R')),

		Rule::Chain(4, TS('o'), TS('('), NS('V'), TS(')'))

		),

		Rule(NS('R'), GRB_ERROR_SERIES + 5, 3,//RETURN

		Rule::Chain(1, TS('i')),

		Rule::Chain(1, TS('l')),

		Rule::Chain(4, TS('i'), TS('('), NS('A'), TS(')'))
		),

		Rule(NS('U'), GRB_ERROR_SERIES + 6, 12,//начало УСЛОВИЯ

		Rule::Chain(2, TS('i'), NS('I')),
		Rule::Chain(2, TS('l'), NS('I')),
		Rule::Chain(4, TS('('), NS('V'), TS(')'), NS('I')),
		Rule::Chain(5, TS('i'), TS('('), NS('A'), TS(')'), NS('I')),

		Rule::Chain(3, TS('('), NS('U'), TS(')')),
		Rule::Chain(4, TS('('), NS('U'), TS(')'), NS('I')),

		Rule::Chain(3, TS('!'), TS('i'), NS('I')),
		Rule::Chain(3, TS('!'), TS('l'), NS('I')),
		Rule::Chain(5, TS('!'), TS('('), NS('V'), TS(')'), NS('I')),
		Rule::Chain(6, TS('!'), TS('i'), TS('('), NS('A'), TS(')'), NS('I')),

		Rule::Chain(4, TS('!'), TS('('), NS('U'), TS(')')),
		Rule::Chain(5, TS('!'), TS('('), NS('U'), TS(')'), NS('I'))

		),

		Rule(NS('I'), GRB_ERROR_SERIES + 6, 36,//ПРОДОЛЖЕНИЕ  УСЛОВИЯ

		    Rule::Chain(3, TS('u'), NS('R'), NS('I')),
			Rule::Chain(4, TS('u'), TS('!'), NS('R'), NS('I')),

		    Rule::Chain(5, TS('u'), TS('('), NS('V'), TS(')'), NS('I')),
			Rule::Chain(6, TS('u'), TS('!'), TS('('), NS('V'), TS(')'), NS('I')),

		    Rule::Chain(5, TS('u'), TS('('), NS('U'), TS(')'), NS('I')),
			Rule::Chain(6, TS('u'), TS('!'), TS('('), NS('U'), TS(')'), NS('I')),


	    	Rule::Chain(2, TS('u'), NS('R')),
			Rule::Chain(3, TS('u'), TS('!'), NS('R')),

		    Rule::Chain(4, TS('u'), TS('('), NS('V'), TS(')')),
			Rule::Chain(5, TS('u'), TS('!'), TS('('), NS('V'), TS(')')),

		    Rule::Chain(4, TS('u'), TS('('), NS('U'), TS(')')),
			Rule::Chain(5, TS('u'), TS('!'), TS('('), NS('U'), TS(')')),


			Rule::Chain(3, TS('<'), NS('R'), NS('I')),
			Rule::Chain(4, TS('<'), TS('!'), NS('R'), NS('I')),

			Rule::Chain(5, TS('<'), TS('('), NS('V'), TS(')'), NS('I')),
			Rule::Chain(6, TS('<'), TS('!'), TS('('), NS('V'), TS(')'), NS('I')),

			Rule::Chain(5, TS('<'), TS('('), NS('U'), TS(')'), NS('I')),
			Rule::Chain(5, TS('<'), TS('!'), TS('('), NS('U'), TS(')'), NS('I')),


			Rule::Chain(2, TS('<'), NS('R')),
			Rule::Chain(3, TS('<'), TS('!'), NS('R')),

			Rule::Chain(4, TS('<'), TS('('), NS('V'), TS(')')),
			Rule::Chain(5, TS('<'), TS('!'), TS('('), NS('V'), TS(')')),

			Rule::Chain(4, TS('<'), TS('('), NS('U'), TS(')')),
			Rule::Chain(5, TS('<'), TS('!'), TS('('), NS('U'), TS(')')),


			Rule::Chain(3, TS('>'), NS('R'), NS('I')),
			Rule::Chain(4, TS('>'), TS('!'), NS('R'), NS('I')),

			Rule::Chain(5, TS('>'), TS('('), NS('V'), TS(')'), NS('I')),
			Rule::Chain(6, TS('>'), TS('!'), TS('('), NS('V'), TS(')'), NS('I')),

			Rule::Chain(5, TS('>'), TS('('), NS('U'), TS(')'), NS('I')),
			Rule::Chain(6, TS('>'), TS('!'), TS('('), NS('U'), TS(')'), NS('I')),


			Rule::Chain(2, TS('>'), NS('R')),
			Rule::Chain(3, TS('>'), TS('!'), NS('R')),

			Rule::Chain(4, TS('>'), TS('('), NS('V'), TS(')')),
			Rule::Chain(5, TS('>'), TS('!'), TS('('), NS('V'), TS(')')),

			Rule::Chain(4, TS('>'), TS('('), NS('U'), TS(')')),
			Rule::Chain(5, TS('>'), TS('!'), TS('('), NS('U'), TS(')'))


		),

		Rule(NS('R'), GRB_ERROR_SERIES + 5, 3,//RETURN

		Rule::Chain(1, TS('i')),

		Rule::Chain(1, TS('l')),

		Rule::Chain(4, TS('i'), TS('('), NS('A'), TS(')'))
		)
					 
	//),
	//				  Rule(NS('B'), GRB_ERROR_SERIES + 2, 16,  // возможные конструкции в ф-иях

	//				  Rule::Chain(5, TS('a'), TS('n'), TS('i'), TS(';'), NS('N')),
	//				  Rule::Chain(5, TS('a'), TS('c'), TS('i'), TS(';'), NS('N')),
	//				  Rule::Chain(5, TS('a'), TS('u'), TS('i'), TS(';'), NS('N')),
	//				  Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
	//				  Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
	//				  Rule::Chain(10, TS('f'), TS('('), NS('E'), TS('&'), NS('E'), TS(')'), TS('['), NS('N'), TS(']'), NS('N')),
	//				  Rule::Chain(10, TS('f'), TS('('), NS('E'), TS('<'), NS('E'), TS(')'), TS('['), NS('N'), TS(']'), NS('N')),
	//				  Rule::Chain(10, TS('f'), TS('('), NS('E'), TS('>'), NS('E'), TS(')'), TS('['), NS('N'), TS(']'), NS('N')),
	//				  Rule::Chain(10, TS('f'), TS('('), NS('E'), TS('!'), NS('E'), TS(')'), TS('['), NS('N'), TS(']'), NS('N')),
	//				  Rule::Chain(5, TS('e'), TS('['), NS('N'), TS(']'), NS('N')),
	//				  Rule::Chain(4, TS('e'), TS('['), NS('N'), TS(']')),
	//				  Rule::Chain(3, TS('r'), NS('E'), TS(';')),
	//				  Rule::Chain(6, TS('y'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')),
	//				  Rule::Chain(5, TS('y'), TS('('), NS('E'), TS(')'), TS(';')),
	//				  Rule::Chain(6, TS('x'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')),
	//				  Rule::Chain(5, TS('x'), TS('('), NS('E'), TS(')'), TS(';'))
	//),
	//				  Rule(NS('E'), GRB_ERROR_SERIES + 3, 20, // выражения

	//				  Rule::Chain(1, TS('i')),
	//				  Rule::Chain(1, TS('q')),
	//				  Rule::Chain(1, TS('l')),
	//				  Rule::Chain(1, TS('k')),
	//				  Rule::Chain(2, TS('('), TS('i')),
	//				  Rule::Chain(2, TS('i'), TS(')')),
	//				  Rule::Chain(3, TS('i'), TS(')'), TS(')')),
	//				  Rule::Chain(2, TS('i'), NS('M')),
	//				  Rule::Chain(2, TS('l'), NS('M')),
	//				  Rule::Chain(2, TS('k'), NS('M')),
	//				  Rule::Chain(4, TS('j'), TS('('), NS('W'), TS(')')),
	//				  Rule::Chain(5, TS('j'), TS('('), NS('W'), TS(')'), NS('M')),
	//				  Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),
	//				  Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
	//				  Rule::Chain(4, TS('w'), TS('('), NS('W'), TS(')')),
	//				  Rule::Chain(5, TS('w'), TS('('), NS('W'), TS(')'), NS('M')),
	//				  Rule::Chain(4, TS('m'), TS('('), NS('W'), TS(')')),
	//				  Rule::Chain(5, TS('m'), TS('('), NS('W'), TS(')'), NS('M')),
	//				  Rule::Chain(4, TS('s'), TS('('), NS('W'), TS(')')),
	//				  Rule::Chain(5, TS('s'), TS('('), NS('W'), TS(')'), NS('M'))

	//),

	//				  Rule(NS('W'), GRB_ERROR_SERIES + 4, 6, // принимаемые параметры ф-ии

	//				  Rule::Chain(3, TS('i'), TS(','), NS('W')),
	//				  Rule::Chain(3, TS('l'), TS(','), NS('W')),
	//				  Rule::Chain(3, TS('k'), TS(','), NS('W')),
	//				  Rule::Chain(1, TS('l')),
	//				  Rule::Chain(1, TS('i')),
	//				  Rule::Chain(1, TS('k'))
	//),

	//				  Rule(NS('M'), GRB_ERROR_SERIES + 5, 16, // знаки

	//				  Rule::Chain(2, TS('+'), NS('E')),
	//				  Rule::Chain(4, TS('+'), TS('('), NS('E'), TS(')')),
	//				  Rule::Chain(5, TS('+'), TS('('), NS('E'), TS(')'), NS('M')),
	//				  Rule::Chain(3, TS('+'), NS('E'), NS('M')),
	//				  Rule::Chain(2, TS('-'), NS('E')),
	//				  Rule::Chain(4, TS('-'), TS('('), NS('E'), TS(')')),
	//				  Rule::Chain(5, TS('-'), TS('('), NS('E'), TS(')'), NS('M')),
	//				  Rule::Chain(3, TS('-'), NS('E'), NS('M')),
	//				  Rule::Chain(2, TS('*'), NS('E')),
	//				  Rule::Chain(4, TS('*'), TS('('), NS('E'), TS(')')),
	//				  Rule::Chain(5, TS('*'), TS('('), NS('E'), TS(')'), NS('M')),
	//				  Rule::Chain(3, TS('*'), NS('E'), NS('M')),
	//				  Rule::Chain(2, TS('/'), NS('E')),
	//				  Rule::Chain(4, TS('/'), TS('('), NS('E'), TS(')')),
	//				  Rule::Chain(5, TS('/'), TS('('), NS('E'), TS(')'), NS('M')),
	//				  Rule::Chain(3, TS('/'), NS('E'), NS('M'))
	//)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)  //конструктор цепочки - праыой части правила(кол-во символов в цепочке, терминал или нетерминал...)
	{
		nt = new GRBALPHABET[size = psize];    //цепочка терминалов
		int* p = (int*)&s;                      //присваиваем символ указателю п
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRBALPHABET)p[i]; //заполняем цепочку терминалов
	};
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) //конструктор правила
	{															//(нетерминал, идентификатор диагностического сообщения, количество цепочек(правых частей правила), множество цепочек (правых частей правила)
		nn = pnn;    //нетерминал
		iderror = piderror; //идентификатор
		chains = new Chain[size = psize]; //место для цепочки
		Chain* p = &c;
		for (int i = 0; i < size; i++)
			chains[i] = p[i]; //заполняем множество цепочек
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)//конструктор гграматики Грейбаха(стартовый символ, дно стека, количество правил, правила...)
	{
		startN = pstartN; //стартовый символ
		stbottomT = pstbottom;//дно стека
		rules = new Rule[size = psize];//выделяем память
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];//заполняем правила
	};
	Greibach getGreibach() { return greibach; }; //получить грамматику
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) //получить правило (левый символ правила, возвращаемое правило грамматики)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
			k++;   //пока К меньше количества правил и пока левый символ правила не равен парметру ф-ции
		if (k < size)
			prule = rules[rc = k];    //возвращаемое правило граматики равно правилу с индексом К
		return rc; //возвращается номер правила или -1
	};
	Rule Greibach::getRule(short n) //получить правило по номеру
	{
		Rule rc;      //создаём правило рц
		if (n < size)rc = rules[n];  //присваиваем правилу рц правило н
		return rc; //возвращаем созданное правило
	};
	char* Rule::getCRule(char* b, short nchain) //получить правило в виде N->цепочка (буфер, номер цепочки(правой части) в правиле)
	{
		char bchain[200]; //строка
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00; //терминал -> 
		chains[nchain].getCChain(bchain); //получает правую сторонц правила
		strcat_s(b, sizeof(bchain) + 5, bchain);//добавляем строку (куда, с какого элемента, строку)
		return b;
	};
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) //получить следующую за j подходящую цепочку, вернуть её номер или -1 
	{                                                                //(первый символ цепочки, возвращаемая цепочка, номер цепочки)
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)++j;
		rc = (j < size ? j : -1);
		if (rc >= 0)pchain = chains[rc];
		return rc;
	};
	char* Rule::Chain::getCChain(char* b) //получить правую сторону правила
	{
		for (int i = 0; i < size; i++)b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
};