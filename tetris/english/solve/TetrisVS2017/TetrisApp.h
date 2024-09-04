#pragma once

#include "ConsolEngine\BaseApp.h"

//the height and width of the playing field
const int FIELD_HEIGHT = 20;
const int FIELD_WIDTH = 15;

const int MAX_OBJ_SPEED = 14; //maximal speed of the falling figure
const int FIGURE_COUNT = 7;   //number of different figures
const int MAX_FIGURE_LENGTH = 4; //maximal length of a figure

const int FIG_SYMBOL = L'#'; //0x25d9; the symbol from which the figure is consist
const int FIELD_BORDER_SYMBOL = L'*'; //field boundary symbol 

enum GAME_STATES { GAME_OVER, GAME_PLAYING };

class TetrisApp;

struct Figure 
{   	
private:
	TetrisApp* tApp; //bind the object to the main class in order to be able to refer to its data
	                 //and other theoretically possible in general graphical objects. This tetris does not use
public:
	int iBeg, jBeg;  // coordinates of the figure (upper left corner) relative to the main playing field
	int speed;       // figure speed

	int figField[MAX_FIGURE_LENGTH][MAX_FIGURE_LENGTH]; //the field of one figure (on which one figure is built)
	int width;       // the width and height of a concrete figure (depending on the figure number)
	int height;

	int name;        // figure number: from 0 to all possible figures
	int turn;        // figure rotation: from zero to 3

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

		int score; // number of points(full lines deleted)
		GAME_STATES gameState;
		
		Figure obj; //current falling figure
		Figure objNext; //next figure (in screen preview)

		bool CheckIntersection(Figure aFig); //check if the figure intersects with the field
        // shape move: left, right, down
		void FigureLeft();
		void FigureRight();
		void FigureDown();
		//rotate a figure
		void FigureTurn();

		void ClearField(int aField[FIELD_HEIGHT][FIELD_WIDTH] = NULL); //clear the additional 
		                                               // (or main, by default) playing field
		void SetObjOnTheField(); // set a figure on the field(merge with the field when it has nowhere else to fall)
		void DeleteFullLines();  // delete complete lines collected on the main playing field, and increase points
		int field[FIELD_HEIGHT][FIELD_WIDTH]; //main game field

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
