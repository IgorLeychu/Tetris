#pragma once

#include "ConsolEngine\BaseApp.h"

//������ � ������ �������� ����
const int FIELD_HEIGHT = 20;
const int FIELD_WIDTH = 15;

const int MAX_OBJ_SPEED = 14; //������������ �������� �������� ������
const int FIGURE_COUNT = 7;   //���������� ������ �����
const int MAX_FIGURE_LENGTH = 4; //������������ ������ ������

const int FIG_SYMBOL = L'#'; //0x25d9; ������, �� �������� ������� ������
const int FIELD_BORDER_SYMBOL = L'*'; //������ ��� ������ ���� 

enum GAME_STATES { GAME_OVER, GAME_PLAYING };

class TetrisApp;

struct Figure 
{   	
private:
	TetrisApp* tApp; //��������� ������ � ������� �������, ���� ����� ����������� ���������� � ��� ������
	                 //� ��������� ������������ ��������� � ����� ������ ����������� ��������. � ������ ������� �� ������������
public:
	int iBeg, jBeg;  // ���������� ������(����� ������� ����) ������������ �������� �������� ����
	int speed;       // �������� ������

	int figField[MAX_FIGURE_LENGTH][MAX_FIGURE_LENGTH]; //���� ����� ������(�� ������� �������� ���� ������)
	int width;       // ������ � ������ ���������� ������(��������� �� ������ ������)
	int height;

	int name;        // ����� ������: �� 0 �� ���� ��������� �����
	int turn;        // ������� ������: �� ���� �� 3-�

public:
	Figure();
	Figure(TetrisApp* tetrApp);
	void SetParent(TetrisApp* tetrApp);

	void Create(int figName, int figTurn);
	void CreateRandom();
	void Left();
	void Right();
	void Down();
	void Turn();

//	friend TetrisApp;
};

class TetrisApp : public BaseApp
{
	typedef BaseApp Parent;

	private:
		float fullGameTime;
		float objDeltaTime;

		int score; // ���������� �����(�������� ������ �����)
		GAME_STATES gameState;
		
		Figure obj; //������� �������� ������
		Figure objNext; //��������� ������ (� ������ ������)

		bool CheckIntersection(Figure aFig); //���������, ������������ �� ������ � �����
		//����� ������: �����, ������, ����
		void FigureLeft();
		void FigureRight();
		void FigureDown();
		//������� ������
		void FigureTurn();

		void ClearField(int aField[FIELD_HEIGHT][FIELD_WIDTH] = NULL); //�������� ��������������� 
		                                               // (��� �������, �� ���������) ������� ����
		void SetObjOnTheField(); // ���������� ������ �� ����(����� � �����, ����� �� ��� ������ ������)
		void DeleteFullLines();  // ������� ������ ��������� �� ������� ������� ���� �����, � ��������� ����
		int field[FIELD_HEIGHT][FIELD_WIDTH]; //������� ������� ����

		void BeginGame();

		void DrawTextString(int x, int y, const wchar_t* aText);
		void DrawNumber(int x, int y, int number);

		void DrawClearAll();
		void DrawAll();
		void DrawFigure();
		void DrawPrevNextFigure();
		void DrawMainField();
		void DrawBorder();
		void DrawScore();

	public:
		TetrisApp();
		virtual void KeyPressed(int btnCode);
		virtual void UpdateF(float deltaTime);
		
	//	friend Figure;
};
