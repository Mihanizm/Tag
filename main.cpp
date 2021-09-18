#include <iostream>
#include <vector>
#include <ctime>
#include <SFML/Graphics.hpp>

using namespace sf;

#define BLACK Color::Black
#define GREY Color(0,0,0,150)
#define WHITE Color::White
#define GREEN Color::Green
#define RED Color::Red
#define YELLOW Color::Yellow
#define BLUE Color::Blue

struct Cell
{
	int type;
	Color color;
	int useFlag = 0;
	Color bcolor = BLACK;
};

Vector2f sizeCell = { 48,48 };
float sizeBorder = 1.0f;

void DrawCell(int j, int i, RenderWindow& window, std::vector<std::vector<Cell>>& field)
{
	RectangleShape rectangle(sizeCell);
	rectangle.move(j * (rectangle.getSize().x + 2*sizeBorder), i * (rectangle.getSize().y + 2*sizeBorder));
	rectangle.setFillColor(field[i][j].color);
	rectangle.setOutlineThickness(sizeBorder);
	rectangle.setOutlineColor(field[i][j].bcolor);

	window.draw(rectangle);
}

void DrawField(RenderWindow& window, std::vector<std::vector<Cell>>& field)
{
	window.clear(WHITE);

	for (int i = 0; i < field.size(); i++)
		for (int j = 0; j < field[0].size(); j++)
			DrawCell(j, i, window, field);

	window.display();
}

void MixCells(std::vector<std::vector<Cell>>& field)
{
	Cell* a = &field[0][0];
	Cell* b = &field[0][0];
	std::vector<std::vector<int>> posEmptyCells = { {2,2},{2,4},{4,2},{4,4} };

	srand(time(NULL));

	int i,j,k,l,n;
	for (int q = 0; q < 500; q++)
	{
		do
		{
			n = rand() % 4;

			i = posEmptyCells[n][0];
			j = posEmptyCells[n][1];

			k = i + rand() % 3 - 1;
			l = j + rand() % 3 - 1;
		} while (field[i][j].type != 0 || field[k][l].type != 1 || abs(k-i)+abs(l-j) != 1 || field[i][j].useFlag == 1 && field[k][l].useFlag == 1);
		
		std::swap(field[i][j], field[k][l]);
		a->useFlag = 0;
		b->useFlag = 0;
		field[i][j].useFlag = 1;
		field[k][l].useFlag = 1;
		a = &field[i][j];
		b = &field[k][l];

		posEmptyCells[n] = { k,l };
	}
}

int Check(std::vector<std::vector<Cell>>& field)
{
	for (int i = 1; i < field.size() - 1; i++)
		for (int j = 1; j < field[0].size(); j += 2)
			if (field[i][j].color != field[0][j].color)
				return -1;
	
	return 0;
}

static Color Transfusion(float speed)
{
	static int cur = 0;
	static float d = speed;
	static float clr[3] = { 0,0,0 };

	srand(time(NULL));

	if (clr[cur] > 255)
	{
		clr[cur] = 255;
		cur = rand() % 3;
		if (clr[cur] == 255)
			d = -speed;
		else
			d = speed;
	}
	else if	(clr[cur] < 0)
	{
		clr[cur] = 0;
		cur = rand() % 3;
		if (clr[cur] == 255)
			d = -speed;
		else
			d = speed;
	}
	clr[cur] += d;

	return Color(clr[0], clr[1], clr[2]);
}

int main()
{
	std::vector<std::vector<Cell>> field =
	{
		{ Cell{-1,GREY}, Cell{-1,RED}, Cell{-1,GREY}, Cell{-1,YELLOW}, Cell{-1,GREY}, Cell{-1,BLUE}, Cell{-1,GREY}},
		{ Cell{-1,GREY}, Cell{1,RED}, Cell{-1,GREY}, Cell{1,YELLOW}, Cell{-1,GREY}, Cell{1,BLUE}, Cell{-1,GREY}},
		{ Cell{-1,GREY}, Cell{1,RED}, Cell{0,GREEN}, Cell{1,YELLOW}, Cell{0,GREEN}, Cell{1,BLUE}, Cell{-1,GREY}},
		{ Cell{-1,GREY}, Cell{1,RED}, Cell{-1,GREY}, Cell{1,YELLOW}, Cell{-1,GREY}, Cell{1,BLUE}, Cell{-1,GREY}},
		{ Cell{-1,GREY}, Cell{1,RED}, Cell{0,GREEN}, Cell{1,YELLOW}, Cell{0,GREEN}, Cell{1,BLUE}, Cell{-1,GREY}},
		{ Cell{-1,GREY}, Cell{1,RED}, Cell{-1,GREY}, Cell{1,YELLOW}, Cell{-1,GREY}, Cell{1,BLUE}, Cell{-1,GREY}},
		{ Cell{-1,GREY}, Cell{-1,GREY}, Cell{-1,GREY}, Cell{-1,GREY}, Cell{-1,GREY}, Cell{-1,GREY}, Cell{-1,GREY}}

	};
	RenderWindow window(VideoMode((sizeCell.x + 2 * sizeBorder - 1) * field[0].size(), (sizeCell.y + 2 * sizeBorder - 1) * field.size()), "Tag", sf::Style::Close | sf::Style::Titlebar);
	int flagClick;
	Vector2<int> pos1;
	Vector2<int> pos2;

	window.clear(WHITE);
	MixCells(field);
	DrawField(window, field);
	flagClick = 0;
	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			else if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					flagClick++;

					if (flagClick == 1)
					{
						//pos1 = Vector2<int>((event.mouseButton.x-1) / sizeCell.x,(event.mouseButton.y-10) / sizeCell.y);
						Vector2i tmp(event.mouseButton.x, event.mouseButton.y);
						pos1 = Vector2<int>(event.mouseButton.x / (sizeCell.x+2*sizeBorder), event.mouseButton.y / (sizeCell.y+2*sizeBorder));
						if (field[pos1.y][pos1.x].type == 1)
						{
							field[pos1.y][pos1.x].color.a = 150;
							DrawField(window, field);
							field[pos1.y][pos1.x].color.a = 255;
						}
						else
							flagClick--;
					}
					else if (flagClick == 2)
					{
						flagClick = 0;
						pos2 = Vector2<int>(event.mouseButton.x / sizeCell.x, event.mouseButton.y / sizeCell.y);
						if (field[pos2.y][pos2.x].type == 0)
							if (abs(pos2.x - pos1.x) + abs(pos2.y - pos1.y) < 2)
								std::swap(field[pos1.y][pos1.x], field[pos2.y][pos2.x]);
						DrawField(window, field);

						float speed = 0.05f;
						if (Check(field) == 0)
						{
							window.setVisible(false);
							
							RenderWindow winGameOver(VideoMode(500,500), "Game Over!", sf::Style::Close | sf::Style::Titlebar);
							while (winGameOver.isOpen())
							{								
								Event event;
								winGameOver.clear(Transfusion(speed));
								while (winGameOver.pollEvent(event))
								{
									if (event.type == Event::Closed)
									{
										winGameOver.close();
										window.setVisible(true);
										MixCells(field);
										DrawField(window, field);
									}
								}
								winGameOver.display();
							}
						}
					}
				}
			}
		}
	}

	return 0;
}