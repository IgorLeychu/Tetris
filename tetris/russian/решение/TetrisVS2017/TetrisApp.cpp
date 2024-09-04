#include "TetrisApp.h"
#include <ctime>
#include <conio.h>

Figure::Figure()
{
	name = 0; // первая фигура
	turn = 0; // исходное положение
	speed = 1;
	width = MAX_FIGURE_LENGTH;
	height = MAX_FIGURE_LENGTH;
}

Figure::Figure(TetrisApp* tetrApp)
{
	tApp = tetrApp;
}

void Figure::SetParent(TetrisApp* tetrApp)
{
	tApp = tetrApp;
}

void Figure::Create(int figName, int figTurn)
{
	name = figName;
	turn = figTurn;
	
	// очищаем поле фигуры
	for (int i = 0; i < MAX_FIGURE_LENGTH; i++)
		for (int j = 0; j < MAX_FIGURE_LENGTH; j++)
			figField[i][j] = 0;

	//формируем одну из возможных фигур
	if (figName == 0) //квадрат
	{
		figField[0][0] = 1;
		figField[0][1] = 1;
		figField[1][0] = 1;
		figField[1][1] = 1;
	}
	else if (figName == 1) //линия
	{
		figField[0][0] = 1;
		figField[1][0] = 1;
		figField[2][0] = 1;
		figField[3][0] = 1;
	}
	else if (figName == 2) //двойная г маленькая
	{
		figField[0][1] = 1;
		figField[0][2] = 1;
		figField[1][0] = 1;
		figField[1][1] = 1;
	}
	else if (figName == 3) //обратная двойная г маленькая
	{
		figField[0][0] = 1;
		figField[0][1] = 1;
		figField[1][1] = 1;
		figField[1][2] = 1;
	}
	else if (figName == 4) // L большое
	{
		figField[0][0] = 1;
		figField[1][0] = 1;
		figField[2][0] = 1;
		figField[2][1] = 1;
	}
	else if (figName == 5) // L большое обратное
	{
		figField[0][1] = 1;
		figField[1][1] = 1;
		figField[2][1] = 1;
		figField[2][0] = 1;
	}
	else if (figName == 6) // T маленькая
	{
		figField[0][0] = 1; 
		figField[0][1] = 1;
		figField[0][2] = 1;
		figField[1][1] = 1;
	}
}

void Figure::CreateRandom()
{
	int randomName;
	srand(time(0));
	randomName = rand() % FIGURE_COUNT;

	name = randomName;
	turn = 0; // поворот не случайный, исходный

	// положение вверху по центру
	jBeg = FIELD_WIDTH / 2 - 1;
	iBeg = 0;
	Create(name, turn);
}

void Figure::Left()
{
	   jBeg--;
};

void Figure::Right()
{
	   jBeg++;
};

void Figure::Down()
{
	iBeg++;
}

void Figure::Turn()
{     
	turn = (turn + 1) % 4;
	int resField[MAX_FIGURE_LENGTH][MAX_FIGURE_LENGTH];
    //обнуляем результирующую матрицу
	for (int i = 0; i < MAX_FIGURE_LENGTH; i++)
		for (int j = 0; j < MAX_FIGURE_LENGTH; j++)
			resField[i][j] = 0;
	
	//вращаем матрицу(поле фигуры) против часовой стрелки
	for (int i = 0; i < MAX_FIGURE_LENGTH; i++)
		for (int j = 0; j < MAX_FIGURE_LENGTH; j++)
			resField[MAX_FIGURE_LENGTH - j - 1][i] = figField[i][j];

	/*сдвигаем фигуру к левой и верхней стороне, удаляя пустые строки в матрице слева и сверху*/		
	bool isEmpty = true;
	int dI = 0;
	for (int i = 0; (i < MAX_FIGURE_LENGTH) && isEmpty; i++)
	{
		for (int j = 0; (j < MAX_FIGURE_LENGTH) && isEmpty; j++)
			if (resField[i][j] != 0) isEmpty = false;
		if (isEmpty) dI++;
	}
	int dJ = 0;
	isEmpty = true;
	for (int j = 0; (j < MAX_FIGURE_LENGTH) && isEmpty; j++)
	{
		for (int i = 0; (i < MAX_FIGURE_LENGTH) && isEmpty; i++)
			if (resField[i][j] != 0) isEmpty = false;
		if (isEmpty) dJ++;
	}

	//обнуляем исходную матрицу
	for (int i = 0; i < MAX_FIGURE_LENGTH; i++)
		for (int j = 0; j < MAX_FIGURE_LENGTH; j++)
			figField[i][j] = 0;

	//записываем результирующую матрицу в исходную со сдвигом на dI и dJ
	for (int i = 0; i < MAX_FIGURE_LENGTH - dI; i++)
		for (int j = 0; j < MAX_FIGURE_LENGTH - dJ; j++)
			figField[i][j] = resField[i+dI][j+dJ];
}

TetrisApp::TetrisApp() : Parent(30, 40)
{
	fullGameTime = 0;
	objDeltaTime = 0;

	obj.SetParent(this);
	objNext = obj;
	
	//начинаем игровой процесс
	BeginGame();	
}

void TetrisApp::BeginGame()
{
	//пустое игровое поле
	ClearField();

	score = 0;

	obj.CreateRandom();
	objNext.CreateRandom();
	gameState = GAME_PLAYING;
}

void TetrisApp::ClearField(int aField[FIELD_HEIGHT][FIELD_WIDTH])
{
	if (aField == NULL)
		aField = field;
	//пустое игровое поле
	for (int i = 0; i < FIELD_HEIGHT; i++)
		for (int j = 0; j < FIELD_WIDTH; j++)
			aField[i][j] = 0;
}

bool TetrisApp::CheckIntersection(Figure aFig)
{
	//true фигура пересекается с полем
	//false свободно на него ложится
	for (int i = 0; i < MAX_FIGURE_LENGTH; i++)
		for (int j = 0; j < MAX_FIGURE_LENGTH; j++)
		{
			if (((aFig.iBeg + i < 0) || (aFig.jBeg + j < 0) || (aFig.iBeg + i >= FIELD_HEIGHT) || (aFig.jBeg + j >= FIELD_WIDTH)) )
			{
				if (aFig.figField[i][j] != 0)
					return true;
			}
			else if ( (aFig.figField[i][j] != 0) && (field[aFig.iBeg + i][aFig.jBeg + j]!=0))
				return true;			
		}

	return false; 
}

void TetrisApp::FigureLeft()
{
	Figure tmpFig;
	tmpFig = obj;
	tmpFig.Left();
	if (CheckIntersection(tmpFig) == false) //не пересекается с полем
	{
		obj.Left();
	}
}

void TetrisApp::FigureRight()
{
	Figure tmpFig;
	tmpFig = obj;
	tmpFig.Right();
	if (CheckIntersection(tmpFig) == false) //не пересекается с полем
	{
		obj.Right();
	}
}

void TetrisApp::FigureDown()
{
	Figure tmpFig;
	tmpFig = obj;
	tmpFig.Down();
	if (CheckIntersection(tmpFig) == false) //не пересекается с полем
	{
		obj.Down();
	}
	else
	{
		SetObjOnTheField();
		DeleteFullLines();
		// следующая игровая итерация
		obj = objNext;		
		objNext.CreateRandom();	
		if (CheckIntersection(obj))
			gameState = GAME_OVER;
	}
}

void  TetrisApp::FigureTurn()
{
	Figure tmpFig;
	tmpFig = obj;
	tmpFig.Turn();
	if (CheckIntersection(tmpFig) == false) //не пересекается с полем
	{
		obj.Turn();
	}
}

void TetrisApp::SetObjOnTheField()
{
	int iOb = 0; int jOb = 0;
	for (int i = obj.iBeg, iOb = 0; i < obj.iBeg + obj.height; i++,iOb++)
		for (int j = obj.jBeg, jOb = 0; j < obj.jBeg + obj.width;j++,jOb++)
			if (obj.figField[iOb][jOb] != 0)
				field[i][j] = obj.figField[iOb][jOb];
}

//удаляем собранные линии и увеличиваем очки
void TetrisApp::DeleteFullLines()
{
	bool isFullLine;
	int i, j;

	// создаем временное результируещее поле и очищаем его
	int resField[FIELD_HEIGHT][FIELD_WIDTH];
	ClearField(resField);

	int iRes = FIELD_HEIGHT - 1;
	for (i = FIELD_HEIGHT-1; i >= 0; i--)
	{
		isFullLine = true;
		for (j = 0; (j < FIELD_WIDTH) && isFullLine; j++)
			if (field[i][j] == 0)
				isFullLine = false;
		
		if (isFullLine)
		{
			score++;
		}
		else
		{
			for (j = 0; j < FIELD_WIDTH; j++)
				resField[iRes][j] = field[i][j];
			iRes--;
		}
	}

	//записываем временное результируещее поле в главное
	for (i = 0; i < FIELD_HEIGHT; i++)
		for (j = 0; j < FIELD_WIDTH; j++)
			field[i][j] = resField[i][j];
}

void TetrisApp::KeyPressed(int btnCode)
{
	if (gameState == GAME_OVER) return;

	if (btnCode == 224)
	{
		int arrowCode = getch();
		if (arrowCode == 80)          //стрелка вниз
			obj.speed = MAX_OBJ_SPEED;
		else if (arrowCode == 75)     //стрелка влево
			FigureLeft();
		else if (arrowCode == 77)     //стрелка вправо
			FigureRight();
	}
	else if (btnCode == 0x20)
		FigureTurn();                 //пробел
}

void TetrisApp::UpdateF(float deltaTime)
{
	fullGameTime += deltaTime;
	objDeltaTime += deltaTime;

	if (gameState == GAME_PLAYING)
	{
		//свой таймер для (каждого)объекта. В данном случае - только для одной текущей фигуры
		if (objDeltaTime >= 1.0 / obj.speed)
		{
			FigureDown();
			objDeltaTime = 0;
		}

		//вывод всей сцены(как в 3d или 2d графике) При желании, могли бы тоже в свой таймер обернуть
		//чтоб уменьшить количество выводов на экран, если бы, например главный таймер(updateF) был слишком резвым
		DrawAll();
	}
	else if (gameState == GAME_OVER)
	{
		DrawAll(); //оставляем экран с тем, что на нем было в конце игры
	}
	
}

void TetrisApp::DrawFigure()
{
	for (int i = 0; i < MAX_FIGURE_LENGTH; i++)
		for (int j = 0; j < MAX_FIGURE_LENGTH; j++)
			if ((obj.iBeg + i >= 0) && (obj.jBeg + j >= 0))
			   if (obj.figField[i][j] != 0)		
				  SetChar(obj.jBeg + j, obj.iBeg + i, FIG_SYMBOL);
}

void TetrisApp::DrawPrevNextFigure()
{
	Figure tmpObjNext;
	tmpObjNext = objNext;
	tmpObjNext.iBeg = 3;
	tmpObjNext.jBeg = FIELD_WIDTH + 3;
	for (int i = 0; i < MAX_FIGURE_LENGTH; i++)
		for (int j = 0; j < MAX_FIGURE_LENGTH; j++)
			if ((tmpObjNext.iBeg + i >= 0) && (tmpObjNext.jBeg + j >= 0))
				if (tmpObjNext.figField[i][j] != 0)
					SetChar(tmpObjNext.jBeg + j, tmpObjNext.iBeg + i, FIG_SYMBOL);
}


void  TetrisApp::DrawTextString(int x, int y, const wchar_t* aText)
{
	int len = wcslen(aText);
	for (int j = 0; j < len; j++)
		SetChar(x+j, y, aText[j]);
}

void  TetrisApp::DrawNumber(int x, int y, int number)
{
	wchar_t resStr[20];

	int scoreLen = 0;
	int tmpScore = score;
	int lastDigit;
	do
	{
		lastDigit = tmpScore % 10;
		tmpScore = tmpScore / 10;
		scoreLen++;
	} while (tmpScore != 0);

	tmpScore = score;
	int curPos = 0;
	resStr[scoreLen] = L'\0';
	do
	{
		lastDigit = tmpScore % 10;
		tmpScore = tmpScore / 10;
		resStr[scoreLen - 1 - curPos] = lastDigit + L'0';
		curPos++;
	} while (tmpScore != 0);
	
	DrawTextString(x, y, resStr);
}

void TetrisApp::DrawBorder()
{
	int i, j;
	for (j = 0; j < FIELD_WIDTH; j++)
		SetChar(j, FIELD_HEIGHT, FIELD_BORDER_SYMBOL);
	
	for (i = 0; i < FIELD_HEIGHT+1; i++)
		SetChar(FIELD_WIDTH, i, FIELD_BORDER_SYMBOL);
}

void TetrisApp::DrawScore()
{
	int iScore = FIELD_HEIGHT + 1;
	int jScore = 0;
	DrawTextString(jScore, iScore, L"score:");
	DrawNumber(jScore + 7, iScore, score);
}

void TetrisApp::DrawClearAll()
{
	for (int i = 0; i < FIELD_HEIGHT+20; i++)
		for (int j = 0; j < FIELD_WIDTH+20; j++)
			SetChar(j, i, L' ');
}

void TetrisApp::DrawAll()
{
	DrawClearAll();
	DrawPrevNextFigure();
	DrawBorder();
	DrawScore();
	DrawMainField();
	DrawFigure();
}

void TetrisApp::DrawMainField()
{
	for (int i = 0 ; i<FIELD_HEIGHT; i++)
		for(int j = 0; j < FIELD_WIDTH; j++)
			if (field[i][j] == 0)
				SetChar(j, i, L'.');
			else
			{
				SetChar(j, i, FIG_SYMBOL);				
			}
}