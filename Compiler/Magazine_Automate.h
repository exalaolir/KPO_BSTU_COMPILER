#pragma once

#include "GRB.h"
#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_DIGBER 3

template <typename T>
struct Stack
{
private:
	std::stack<T> steck;
public:
	void push(T st) {
		steck.push(st);
	}
	void pop() {
		steck.pop();
	}
	T top() {
		return steck.top();
	}
	int size() {
		return steck.size();
	}
	T operator[](int s) {
		std::stack<T> st;
		st = steck;
		for (int i = (s + 1); i < steck.size(); i++)
		{
			st.pop();
		}
		return st.top();
	}
};

typedef Stack<short> MFSTSTSTACK;

namespace MFST
{
	struct MfstState				//состояние автомата(для сохранения
	{
		short lenta_position;		//позиция на ленте
		short nrule;				//номер текущего правила
		short nrulechain;			//номер текущей цепочки
		MFSTSTSTACK st;				//стек автомата
		MfstState();
		MfstState(
			short pposition,		//позиция на ленте
			MFSTSTSTACK pst,		//стек автомата
			short pnrulechain		//номер текущей цепочки, текущего правила
		);
		MfstState(
			short pposition,		//позиция на ленте
			MFSTSTSTACK pst,		//стек автомата
			short pnrule,			//номер текущего правила
			short pnrulechain		//номер текущей цепочки, текущего правила
		);
	};

	struct Mfst					//магазинный автомат	
	{
		enum RC_STEP			//код возврата функции step,
		{
			NS_OK,				//найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,			//не найдено правило грамматики (ошибки в грамматике)
			NS_NORULECHAIN,		//не найдена подходящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,			//неизвестный нетерминальный символ грамматики
			TS_OK,				//текущий символ ленты == вершине стека, продвинулась лента, рор стека
			TS_NOK,				//текущий символ ленты != вершине стека, восстановлено состояние
			LENTA_END,			//текущая позиция ленты >= lenta_size
			SURPRISE			//неожиданный код возврата ( ошибка в step)
		};
		struct MfstDiagnosis		//диагностика
		{
			short lenta_position;	//позиция на ленте
			RC_STEP rc_step;		//код завершения шага
			short nrule;			//номер правила
			short nrule_chain;		//номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis(			//диагностика
						  short plenta_position,	//позиция на ленте
						  RC_STEP prc_step,		//код завершения шага
						  short pnrule,			//номер правила
						  short pnrule_chain		//номер цепочки правила
			);
		}
		diagnosis[MFST_DIAGN_DIGBER];	//последние самые глубокие сообщения
		GRBALPHABET* lenta;				//перекодированная(TS/NS) лента (из LEX)
		short lenta_position;			//текущая позиция на ленте
		short nrule;					//номер текущего правила
		short nrulechain;				//номер текущей цепочки, текущего правила
		short lenta_size;				//размер ленты
		GRB::Greibach grebach;			//грамматика Грейбах
		LEXER::IdTable table;				//результат работы лексического анализатора
		MFSTSTSTACK st;					//стек автомата		
		Stack<MfstState> storestate;	//стек для сохранения состояний
		Mfst();
		Mfst(
			std::vector<LEXER::Lexem> lexems,
			LEXER::IdTable table,
			GRB::Greibach pgrebach		//грамматика Грейбах
		);
		char* getCSt(char* buf);			//получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25);//лента: n символов с pos
		std::vector<LEXER::Lexem> lexems;
		const char* getDiagnosis(short n, char* buf);			//получить n-ую строку диагностики или 0х00
		bool savestate(const std::shared_ptr<std::ofstream> log);					//сохранить состояние автомата
		bool reststate(const std::shared_ptr<std::ofstream> log);					//восстановить состояние автомата
		bool push_chain(					//поместить уепочку правила в стек
						GRB::Rule::Chain chain				//цепочка правила
		);
		RC_STEP step(const std::shared_ptr<std::ofstream> log);			//выполнить шаг автомата
		bool start(const std::shared_ptr<std::ofstream> log);			//запустить автомат
		bool savediagnois(
			RC_STEP pprc_step			//код завершения шага
		);
		void printrules(const std::shared_ptr<std::ofstream> log);		//вывести последовательность правил
		struct Deducation		//вывод
		{
			short size;		//количество шагов в выводе
			short* nrules;	//номера правил нрамматики
			short* nrulechains;//номера цепочек правил грамматики (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool savededucation();	//сохранить дерево вывода
	};
};


//#include "Lexer.h"
//
//namespace SYNTAX
//{
//	struct Automate
//	{
//	public:
//		Automate(std::vector<LEXER::Lexem>& lexTable)
//		{
//			this->lexTable = lexTable;
//		}
//
//		void checkAndBuildTree();
//	private:
//		std::vector<LEXER::Lexem> lexTable;
//
//		static inline std::unordered_map<char, std::list<std::pair<std::list<char>, std::string>>> rules
//		{
//			{'S',
//				  {
//					  {
//						  {'}', 'E', 'r', 'B' ,'{', ')','(', 'm', 't', 'f'},
//						  "Неверная структура программы"
//					  },
//					  {
//						   {'F', '}','E', 'r', 'B', '{', ')', 'P', '(', 'i', 't', 'f'},
//						   "Неверная структура программы"
//					  },
//					  {
//							{'S',';', 'E', '=', 'i', 't'},
//							"Неверная структура программы"
//					  }
//				  }
//			},
//			{'F',
//				  {
//					  {
//						  {'}', 'E', 'r', 'B' , '{', ')','(', 'm', 't', 'f'},
//						  "Неверная структура программы"
//					  },
//					  {
//						   {'F', '}', 'E', 'r', 'B' , '{', ')', 'P', '(', 'i', 't', 'f'},
//						   "Неверная структура программы"
//					  },
//				  }
//			},
//			{'P',
//				  {
//					{
//						 {'i', 't'},
//						"Неверная структура программы"
//					},
//					{
//						{'P', ',', 'i', 't'},
//						"Неверная структура программы"
//					},
//				  }
//			},
//			{'E',
//				  {
//					{
//						 {'i'},
//						"Неверная структура программы"
//					},
//					{
//						{'l'},
//						"Неверная структура программы"
//					},
//					{
//						 {')','E', '('},
//						"Неверная структура программы"
//					},
//					{
//						{')', 'W', '(', 'i'},
//						"Неверная структура программы"
//					},
//					{
//						 {'X','i'},
//						"Неверная структура программы"
//					},
//					{
//						{'X','l'},
//						"Неверная структура программы"
//					},
//					{
//						 {'X',')','E', '('},
//						"Неверная структура программы"
//					},
//					{
//						{'X',')', 'W', '(', 'i'},
//						"Неверная структура программы"
//					},
//				  }
//			},
//			{'N',
//				  {
//					{
//						 {';','i', 't'},
//						"Неверная структура программы"
//					},
//					{
//						{';', 'E', '=', 'i'},
//						"Неверная структура программы"
//					},
//					{
//						 {'N', ';', 'i', 't'},
//						"Неверная структура программы"
//					},
//					{
//						{'N', ';', 'E', '=', 'i'},
//						"Неверная структура программы"
//					}
//				  }
//			},
//			{'X',
//				  {
//					{
//						 {'E', 'o'},
//						"Неверная структура программы"
//					},
//					{
//						{'x','E', 'o'},
//						"Неверная структура программы"
//					},
//				  }
//			},
//			{'W',
//				  {
//					{
//						 {'i'},
//						"Неверная структура программы"
//					},
//					{
//						{'l'},
//						"Неверная структура программы"
//					},
//					{
//						{'W',',' ,'l'},
//						"Неверная структура программы"
//					},
//					{
//						{'W', ',', 'i'},
//						"Неверная структура программы"
//					},
//				  }
//			},
//		};
//	};
//
//	struct SavedState
//	{
//		SavedState(size_t pos, std::list<char> savedMagazine, size_t countNewAddedElements)
//		{
//			this->pos = pos;
//			this->savedMagazine = savedMagazine;
//			this->countNewAddedElements = countNewAddedElements;
//		}
//		size_t pos;
//		std::list<char> savedMagazine;
//		size_t countNewAddedElements;
//	};
//}