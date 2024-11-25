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
	struct MfstState				//��������� ��������(��� ����������
	{
		short lenta_position;		//������� �� �����
		short nrule;				//����� �������� �������
		short nrulechain;			//����� ������� �������
		MFSTSTSTACK st;				//���� ��������
		MfstState();
		MfstState(
			short pposition,		//������� �� �����
			MFSTSTSTACK pst,		//���� ��������
			short pnrulechain		//����� ������� �������, �������� �������
		);
		MfstState(
			short pposition,		//������� �� �����
			MFSTSTSTACK pst,		//���� ��������
			short pnrule,			//����� �������� �������
			short pnrulechain		//����� ������� �������, �������� �������
		);
	};

	struct Mfst					//���������� �������	
	{
		enum RC_STEP			//��� �������� ������� step,
		{
			NS_OK,				//������� ������� � �������, ������� �������� � ����
			NS_NORULE,			//�� ������� ������� ���������� (������ � ����������)
			NS_NORULECHAIN,		//�� ������� ���������� ������� ������� (������ � �������� ����)
			NS_ERROR,			//����������� �������������� ������ ����������
			TS_OK,				//������� ������ ����� == ������� �����, ������������ �����, ��� �����
			TS_NOK,				//������� ������ ����� != ������� �����, ������������� ���������
			LENTA_END,			//������� ������� ����� >= lenta_size
			SURPRISE			//����������� ��� �������� ( ������ � step)
		};
		struct MfstDiagnosis		//�����������
		{
			short lenta_position;	//������� �� �����
			RC_STEP rc_step;		//��� ���������� ����
			short nrule;			//����� �������
			short nrule_chain;		//����� ������� �������
			MfstDiagnosis();
			MfstDiagnosis(			//�����������
						  short plenta_position,	//������� �� �����
						  RC_STEP prc_step,		//��� ���������� ����
						  short pnrule,			//����� �������
						  short pnrule_chain		//����� ������� �������
			);
		}
		diagnosis[MFST_DIAGN_DIGBER];	//��������� ����� �������� ���������
		GRBALPHABET* lenta;				//����������������(TS/NS) ����� (�� LEX)
		short lenta_position;			//������� ������� �� �����
		short nrule;					//����� �������� �������
		short nrulechain;				//����� ������� �������, �������� �������
		short lenta_size;				//������ �����
		GRB::Greibach grebach;			//���������� �������
		LEXER::IdTable table;				//��������� ������ ������������ �����������
		MFSTSTSTACK st;					//���� ��������		
		Stack<MfstState> storestate;	//���� ��� ���������� ���������
		Mfst();
		Mfst(
			std::vector<LEXER::Lexem> lexems,
			LEXER::IdTable table,
			GRB::Greibach pgrebach		//���������� �������
		);
		char* getCSt(char* buf);			//�������� ���������� �����
		char* getCLenta(char* buf, short pos, short n = 25);//�����: n �������� � pos
		std::vector<LEXER::Lexem> lexems;
		const char* getDiagnosis(short n, char* buf);			//�������� n-�� ������ ����������� ��� 0�00
		bool savestate(const std::shared_ptr<std::ofstream> log);					//��������� ��������� ��������
		bool reststate(const std::shared_ptr<std::ofstream> log);					//������������ ��������� ��������
		bool push_chain(					//��������� ������� ������� � ����
						GRB::Rule::Chain chain				//������� �������
		);
		RC_STEP step(const std::shared_ptr<std::ofstream> log);			//��������� ��� ��������
		bool start(const std::shared_ptr<std::ofstream> log);			//��������� �������
		bool savediagnois(
			RC_STEP pprc_step			//��� ���������� ����
		);
		void printrules(const std::shared_ptr<std::ofstream> log);		//������� ������������������ ������
		struct Deducation		//�����
		{
			short size;		//���������� ����� � ������
			short* nrules;	//������ ������ ����������
			short* nrulechains;//������ ������� ������ ���������� (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool savededucation();	//��������� ������ ������
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
//						  "�������� ��������� ���������"
//					  },
//					  {
//						   {'F', '}','E', 'r', 'B', '{', ')', 'P', '(', 'i', 't', 'f'},
//						   "�������� ��������� ���������"
//					  },
//					  {
//							{'S',';', 'E', '=', 'i', 't'},
//							"�������� ��������� ���������"
//					  }
//				  }
//			},
//			{'F',
//				  {
//					  {
//						  {'}', 'E', 'r', 'B' , '{', ')','(', 'm', 't', 'f'},
//						  "�������� ��������� ���������"
//					  },
//					  {
//						   {'F', '}', 'E', 'r', 'B' , '{', ')', 'P', '(', 'i', 't', 'f'},
//						   "�������� ��������� ���������"
//					  },
//				  }
//			},
//			{'P',
//				  {
//					{
//						 {'i', 't'},
//						"�������� ��������� ���������"
//					},
//					{
//						{'P', ',', 'i', 't'},
//						"�������� ��������� ���������"
//					},
//				  }
//			},
//			{'E',
//				  {
//					{
//						 {'i'},
//						"�������� ��������� ���������"
//					},
//					{
//						{'l'},
//						"�������� ��������� ���������"
//					},
//					{
//						 {')','E', '('},
//						"�������� ��������� ���������"
//					},
//					{
//						{')', 'W', '(', 'i'},
//						"�������� ��������� ���������"
//					},
//					{
//						 {'X','i'},
//						"�������� ��������� ���������"
//					},
//					{
//						{'X','l'},
//						"�������� ��������� ���������"
//					},
//					{
//						 {'X',')','E', '('},
//						"�������� ��������� ���������"
//					},
//					{
//						{'X',')', 'W', '(', 'i'},
//						"�������� ��������� ���������"
//					},
//				  }
//			},
//			{'N',
//				  {
//					{
//						 {';','i', 't'},
//						"�������� ��������� ���������"
//					},
//					{
//						{';', 'E', '=', 'i'},
//						"�������� ��������� ���������"
//					},
//					{
//						 {'N', ';', 'i', 't'},
//						"�������� ��������� ���������"
//					},
//					{
//						{'N', ';', 'E', '=', 'i'},
//						"�������� ��������� ���������"
//					}
//				  }
//			},
//			{'X',
//				  {
//					{
//						 {'E', 'o'},
//						"�������� ��������� ���������"
//					},
//					{
//						{'x','E', 'o'},
//						"�������� ��������� ���������"
//					},
//				  }
//			},
//			{'W',
//				  {
//					{
//						 {'i'},
//						"�������� ��������� ���������"
//					},
//					{
//						{'l'},
//						"�������� ��������� ���������"
//					},
//					{
//						{'W',',' ,'l'},
//						"�������� ��������� ���������"
//					},
//					{
//						{'W', ',', 'i'},
//						"�������� ��������� ���������"
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