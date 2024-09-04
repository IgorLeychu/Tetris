#pragma once

#include "ConsolEngine\BaseApp.h"

//высота и ширина игрового поля
const int FIELD_HEIGHT = 20;
const int FIELD_WIDTH = 15;

const int MAX_OBJ_SPEED = 14; //максимальная скорость падающей фигуры
const int FIGURE_COUNT = 7;   //количество разных фигур
const int MAX_FIGURE_LENGTH = 4; //максимальная длинна фигуры

const int FIG_SYMBOL = L'#'; //0x25d9; символ, из которого состоит фигура
const int FIELD_BORDER_SYMBOL = L'*'; //символ для границ поля 

enum GAME_STATES { GAME_OVER, GAME_PLAYING };

class TetrisApp;

struct Figure 
{   	
private:
	TetrisApp* tApp; //связываем объект с главным классом, чтоб иметь возможность обращаться к его данным
	                 //и остальным теоретически возможным в общем случае графическим объектам. В данном тетрисе не используется
public:
	int iBeg, jBeg;  // координаты фигуры(левый верхний угол) относительно главного игрового поля
	int speed;       // скорость фигуры

	int figField[MAX_FIGURE_LENGTH][MAX_FIGURE_LENGTH]; //поле одной фигуры(на котором строится одна фигура)
	int width;       // ширина и высота конкретной фигуры(зависящая от номера фигуры)
	int height;

	int name;        // номер фигуры: от 0 до всех возможных фигур
	int turn;        // поворот фигуры: от нуля до 3-х

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

		int score; // количество очков(удалённых полных линий)
		GAME_STATES gameState;
		
		Figure obj; //текущая падающая фигура
		Figure objNext; //следующая фигура (в превью экрана)

		bool CheckIntersection(Figure aFig); //проверить, пересекается ли фигура с полем
		//сдвиг фигуры: влево, вправо, вниз
		void FigureLeft();
		void FigureRight();
		void FigureDown();
		//поворот фигуры
		void FigureTurn();

		void ClearField(int aField[FIELD_HEIGHT][FIELD_WIDTH] = NULL); //очистить вспомогательное 
		                                               // (или главное, по умолчанию) игровое поле
		void SetObjOnTheField(); // установить фигуру на поле(слить с полем, когда ей уже некуда падать)
		void DeleteFullLines();  // удалить полные собранные на главном игровом поле линии, и увеличить очки
		int field[FIELD_HEIGHT][FIELD_WIDTH]; //главное игровое поле

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
