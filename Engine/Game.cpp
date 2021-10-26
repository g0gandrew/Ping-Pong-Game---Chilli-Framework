/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "time.h"

 
// objects
Game::Bar left, right;
Game::Square square;


// Bar Objects - Variables
//

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}


void Game::Go()
{

	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();

}

void Game::UpdateModel()
{

	// left bar
	if (wnd.kbd.KeyIsPressed(VK_DOWN)) {
		if (!left.pressedDown) {
		}
		else
			left.speed++;
	}
	else
		left.pressedDown = true;
	if (wnd.kbd.KeyIsPressed(VK_UP)) {
		if (!left.pressedUp) {
		}
		else
			left.speed--;
	}
	else
		left.pressedUp = true;
	
	// right bar
	if (wnd.kbd.KeyIsPressed(0x53)) {
		if (!right.pressedDown) {
		}
		else
			right.speed++;
	}
	else
		right.pressedDown = true;
	if (wnd.kbd.KeyIsPressed(0x57)) {
		if (!right.pressedUp) {
		}
		else
			right.speed--;
	}
	else
		right.pressedUp = true;
}

void Game::ComposeFrame()
{
	// change color depending on score (I tried with function, but i got an internal error from framework, idk why).
	srand(time(NULL));
	left.r = 255;
	left.g = 255;
	left.b = 255;
	right.r = 255;
	right.g = 255;
	right.b = 255;
	if (right.points > left.points) {
		right.r = rand() % 255;
		right.g = rand() % 255;
		right.b = rand() % 255;
	}
	else if(left.points > right.points) {
		left.r = rand() % 255;
		left.g = rand() % 255;
		left.b = rand() % 255;
	}
	//

	ConstructBar(left, right);
	MoveSquare(square.L1, square.L2, square.x, square.y, left.start, right.start);
	ConstructSquare(square.x, square.y);

}

void Game::ConstructBar(Bar x, Bar y) {
	// left
	left.start += left.speed;
	right.start += right.speed;
	OutsideScreen(left.start, right.start);
	unsigned int i, z;
		for (z = 0; z <= 10; ++z)
			for (i = left.start; i <= left.start+100; ++i) 
				gfx.PutPixel(z, i, x.r, x.g, x.b);
		for(z = 789; z <= 799; ++z)
			for (i = right.start; i <= right.start + 100; ++i) 
				gfx.PutPixel(z, i, y.r, y.g, y.b);
}
							
void Game::OutsideScreen(int& lStart, int& rStart) {
	// left
	if (lStart + 100 >= gfx.ScreenHeight) {
		lStart = gfx.ScreenHeight - 100;
		left.speed = 0;
	}
	if (lStart <= 0) {
		lStart = 1;
		left.speed = 0;
	}
	// right
	if (rStart + 100 >= gfx.ScreenHeight) {
		rStart = gfx.ScreenHeight - 101; // idk why here works only if we substract "101", not 100.
		right.speed = 0;
	}
	if (rStart <= 0) {
		rStart = 1;
		right.speed = 0;
	}
}

bool Game::OutsideScreen_square(int& x, int& y) {	
	if (x - 11 <= 0) { // outside left
		right.points++;
		left.start = 250;
		right.start = 250;
		square.x = 400;
		square.y = 250;
		square.xDirection = 0;
		square.yDirection = 0;
		scenario = 7;
	}
	else if (x + 11 >= gfx.ScreenWidth) { // outside right
		left.points++;
		left.start = 250;
		right.start = 250;
		square.x = 400;
		square.y = 250;
		square.xDirection = 0;
		square.yDirection = 0;
		scenario = 0;
	}
	
	else if (y - 11 <= 0) { // if the square goes under pixels value on horizontal
		y = 11;
		if (scenario == 1)
			scenario = 3;
		else if (scenario == 4)
			scenario = 6;
	}
	else if (y + 11 >= gfx.ScreenHeight) { // if  the square goes over pixels value on horizontal 
		y = gfx.ScreenHeight - 11;
		if (scenario == 3)
			scenario = 1;
		else if (scenario == 6)
			scenario = 4;
	}
	else
		return 1;	

}
void Game::MoveSquare(int a, int b, int& k, int& l, int m, int n) {
	if(!OutsideScreen_square(k, l)) {
	}
	else {
		a = k - 6; // L1
		b = k + 6; // L2	
		///
		if ((a <= 10 && l >= m && l <= m + 40) && l <= 300)  // point 3  
			scenario = 3;
		else if((a <= 10 && l >= m && l <= + 40) && l >= 300)
			scenario = 1;
		if ((a <= 10 && l >= m + 60 && l <= m + 100) && l >= 300)// point 1
			scenario = 1;
		else if((a <= 10 && l >= m + 60 && l <= m + 100 ) && l <= 300)
			scenario = 3;
		if (a <= 10 && l >= m + 41 && l <= m + 59) // point 2
			scenario = rand() % 3 + 1;
		///

		if ((b >= 789 && l >= n && l <= n + 40) && l <= 300) // point 3
			scenario = 6;
		else if((b >= 789 && l >= n && l <= n + 40) && l >= 300)
			scenario = 4;
		if (b >= 789 && l >= n + 41 && l <= n + 59) // point 2
			scenario = 0;
		if ((b >= 789 && l >= n + 60 && l <= n + 100) && l >= 300) // point 1
			scenario = 4;
		else if((b >= 789 && l >= n + 60 && l <= n + 100) && l <= 300)
			scenario = 6;
		///
	}

	switch (scenario) {
		case 0: {
			k-=4;
			break;
		}
		case 1: {
			k+=4, l-=4;
			break;
		}
		case 2: {
			k += 4;
			break;
		}
		case 3: {
			k+=4, l+=4;
			break;
		}
		case 4: {
			k-=4, l-=4;
			break;
		}
		case 5: {
			k-=4;
			break;
		}
		case 6: {
			k-=4, l+=4;
			break;
		}
		case 7: {
			k+=4;
			break;
		}
	}

}

	
void Game::ConstructSquare(int x, int y) {
	/////////////////////////////////////////////////////////////////// <---- SQUARE SPRITE
	gfx.PutPixel(x, y, 0, 255, 0); // central
	gfx.PutPixel(x + 1, y, 0, 255, 0); // dreapta central 1 
	gfx.PutPixel(x + 2, y, 0, 255, 0); // dreapta central 2 
	gfx.PutPixel(x + 3, y, 0, 255, 0); // dreapta central 3 
	gfx.PutPixel(x + 4, y, 0, 255, 0); // dreapta central 4 
	gfx.PutPixel(x + 5, y, 0, 255, 0); // dreapta central 5
	gfx.PutPixel(x + 6, y, 0, 255, 0); // dreapta central 6
	gfx.PutPixel(x - 1, y, 0, 255, 0); // stanga central 1
	gfx.PutPixel(x - 2, y, 0, 255, 0); // stanga central 2
	gfx.PutPixel(x - 3, y, 0, 255, 0); // stanga central 3
	gfx.PutPixel(x - 4, y, 0, 255, 0); // stanga central 4
	gfx.PutPixel(x - 5, y, 0, 255, 0); // stanga central 5
	gfx.PutPixel(x - 6, y, 0, 255, 0); // stanga central 5
	gfx.PutPixel(x, y + 1, 0, 255, 0); // jos 1
	gfx.PutPixel(x, y + 2, 0, 255, 0); // jos 2
	gfx.PutPixel(x, y + 3, 0, 255, 0); // jos 3
	gfx.PutPixel(x, y + 4, 0, 255, 0); // jos 4
	gfx.PutPixel(x, y + 5, 0, 255, 0); // jos 5
	gfx.PutPixel(x, y + 6, 0, 255, 0); // jos 6
	gfx.PutPixel(x, y - 1, 0, 255, 0); // sus 1
	gfx.PutPixel(x, y - 2, 0, 255, 0); // sus 2
	gfx.PutPixel(x, y - 3, 0, 255, 0); // sus 3
	gfx.PutPixel(x, y - 4, 0, 255, 0); // sus 4
	gfx.PutPixel(x, y - 5, 0, 255, 0); // sus 5
	gfx.PutPixel(x, y - 6, 0, 255, 0); // sus 6
	// 2 / 4
	gfx.PutPixel(x + 1, y - 1, 0, 255, 0); // dreapta sus 1 - 1
	gfx.PutPixel(x + 2, y - 1, 0, 255, 0); // dreapta sus 1 - 2
	gfx.PutPixel(x + 3, y - 1, 0, 255, 0); // dreapta sus 1 - 3
	gfx.PutPixel(x + 4, y - 1, 0, 255, 0); // dreapta sus 1 - 4
	gfx.PutPixel(x + 5, y - 1, 0, 255, 0); // dreapta sus 1 - 5
	gfx.PutPixel(x + 6, y - 1, 0, 255, 0); // dreapta sus 1 - 6
	gfx.PutPixel(x + 1, y - 2, 0, 255, 0); // dreapta sus 2 - 1 
	gfx.PutPixel(x + 2, y - 2, 0, 255, 0); // dreapta sus 2 - 2 
	gfx.PutPixel(x + 3, y - 2, 0, 255, 0); // dreapta sus 2 - 3 
	gfx.PutPixel(x + 4, y - 2, 0, 255, 0); // dreapta sus 2 - 4 
	gfx.PutPixel(x + 5, y - 2, 0, 255, 0); // dreapta sus 2 - 5 
	gfx.PutPixel(x + 6, y - 2, 0, 255, 0); // dreapta sus 2 - 6 
	gfx.PutPixel(x + 1, y - 3, 0, 255, 0); // dreapta sus 3 - 1
	gfx.PutPixel(x + 2, y - 3, 0, 255, 0); // dreapta sus 3 - 2
	gfx.PutPixel(x + 3, y - 3, 0, 255, 0); // dreapta sus 3 - 3
	gfx.PutPixel(x + 4, y - 3, 0, 255, 0); // dreapta sus 3 - 4
	gfx.PutPixel(x + 5, y - 3, 0, 255, 0); // dreapta sus 3 - 5
	gfx.PutPixel(x + 6, y - 3, 0, 255, 0); // dreapta sus 3 - 6
	gfx.PutPixel(x + 1, y - 4, 0, 255, 0); // dreapta sus 4 - 1
	gfx.PutPixel(x + 2, y - 4, 0, 255, 0); // dreapta sus 4 - 2 
	gfx.PutPixel(x + 3, y - 4, 0, 255, 0); // dreapta sus 4 - 3
	gfx.PutPixel(x + 4, y - 4, 0, 255, 0); // dreapta sus 4 - 4
	gfx.PutPixel(x + 5, y - 4, 0, 255, 0); // dreapta sus 4 - 5
	gfx.PutPixel(x + 6, y - 4, 0, 255, 0); // dreapta sus 4 - 6
	gfx.PutPixel(x + 1, y - 5, 0, 255, 0); // dreapta sus 5 - 1
	gfx.PutPixel(x + 2, y - 5, 0, 255, 0); // dreapta sus 5 - 2
	gfx.PutPixel(x + 3, y - 5, 0, 255, 0); // dreapta sus 5 - 3
	gfx.PutPixel(x + 4, y - 5, 0, 255, 0); // dreapta sus 5 - 4
	gfx.PutPixel(x + 5, y - 5, 0, 255, 0); // dreapta sus 5 - 5
	gfx.PutPixel(x + 6, y - 5, 0, 255, 0); // dreapta sus 5 - 6
	gfx.PutPixel(x + 1, y - 6, 0, 255, 0); // dreapta sus 6 - 1
	gfx.PutPixel(x + 2, y - 6, 0, 255, 0); // dreapta sus 6 - 2
	gfx.PutPixel(x + 3, y - 6, 0, 255, 0); // dreapta sus 6 - 3
	gfx.PutPixel(x + 4, y - 6, 0, 255, 0); // dreapta sus 6 - 4
	gfx.PutPixel(x + 5, y - 6, 0, 255, 0); // dreapta sus 6 - 5
	gfx.PutPixel(x + 6, y - 6, 0, 255, 0); // dreapta sus 6 - 6
	// 1/4
	gfx.PutPixel(x - 1, y - 1, 0, 255, 0); // stanga sus 1 - 1
	gfx.PutPixel(x - 2, y - 1, 0, 255, 0); // stanga sus 1 - 2
	gfx.PutPixel(x - 3, y - 1, 0, 255, 0); // stanga sus 1 - 3
	gfx.PutPixel(x - 4, y - 1, 0, 255, 0); // stanga sus 1 - 4
	gfx.PutPixel(x - 5, y - 1, 0, 255, 0); // stanga sus 1 - 5
	gfx.PutPixel(x - 6, y - 1, 0, 255, 0); // stanga sus 1 - 6
	gfx.PutPixel(x - 1, y - 2, 0, 255, 0); // stanga sus 2 - 1 
	gfx.PutPixel(x - 2, y - 2, 0, 255, 0); // stanga sus 2 - 2 
	gfx.PutPixel(x - 3, y - 2, 0, 255, 0); // stanga sus 2 - 3 
	gfx.PutPixel(x - 4, y - 2, 0, 255, 0); // stanga sus 2 - 4 
	gfx.PutPixel(x - 5, y - 2, 0, 255, 0); // stanga sus 2 - 5 
	gfx.PutPixel(x - 6, y - 2, 0, 255, 0); // stanga sus 2 - 6 
	gfx.PutPixel(x - 1, y - 3, 0, 255, 0); // stanga sus 3 - 1
	gfx.PutPixel(x - 2, y - 3, 0, 255, 0); // stanga sus 3 - 2
	gfx.PutPixel(x - 3, y - 3, 0, 255, 0); // stanga sus 3 - 3
	gfx.PutPixel(x - 4, y - 3, 0, 255, 0); // stanga sus 3 - 4
	gfx.PutPixel(x - 5, y - 3, 0, 255, 0); // stanga sus 3 - 5
	gfx.PutPixel(x - 6, y - 3, 0, 255, 0); // stanga sus 3 - 6
	gfx.PutPixel(x - 1, y - 4, 0, 255, 0); // stanga sus 4 - 1
	gfx.PutPixel(x - 2, y - 4, 0, 255, 0); // stanga sus 4 - 2 
	gfx.PutPixel(x - 3, y - 4, 0, 255, 0); // stanga sus 4 - 3
	gfx.PutPixel(x - 4, y - 4, 0, 255, 0); // stanga sus 4 - 4
	gfx.PutPixel(x - 5, y - 4, 0, 255, 0); // stanga sus 4 - 5
	gfx.PutPixel(x - 6, y - 4, 0, 255, 0); // stanga sus 4 - 6
	gfx.PutPixel(x - 1, y - 5, 0, 255, 0); // stanga sus 5 - 1
	gfx.PutPixel(x - 2, y - 5, 0, 255, 0); // stanga sus 5 - 2
	gfx.PutPixel(x - 3, y - 5, 0, 255, 0); // stanga sus 5 - 3
	gfx.PutPixel(x - 4, y - 5, 0, 255, 0); // stanga sus 5 - 4
	gfx.PutPixel(x - 5, y - 5, 0, 255, 0); // stanga sus 5 - 5
	gfx.PutPixel(x - 6, y - 5, 0, 255, 0); // stanga sus 5 - 6
	gfx.PutPixel(x - 1, y - 6, 0, 255, 0); // stanga sus 6 - 1
	gfx.PutPixel(x - 2, y - 6, 0, 255, 0); // stanga sus 6 - 2
	gfx.PutPixel(x - 3, y - 6, 0, 255, 0); // stanga sus 6 - 3
	gfx.PutPixel(x - 4, y - 6, 0, 255, 0); // stanga sus 6 - 4
	gfx.PutPixel(x - 5, y - 6, 0, 255, 0); // stanga sus 6 - 5
	gfx.PutPixel(x - 6, y - 6, 0, 255, 0); // stanga sus 6 - 6
	// 3 / 4
	gfx.PutPixel(x + 1, y + 1, 0, 255, 0); // dreapta jos 1 - 1
	gfx.PutPixel(x + 2, y + 1, 0, 255, 0); // dreapta jos 1 - 2
	gfx.PutPixel(x + 3, y + 1, 0, 255, 0); // dreapta jos 1 - 3
	gfx.PutPixel(x + 4, y + 1, 0, 255, 0); // dreapta jos 1 - 4
	gfx.PutPixel(x + 5, y + 1, 0, 255, 0); // dreapta jos 1 - 5
	gfx.PutPixel(x + 6, y + 1, 0, 255, 0); // dreapta jos 1 - 6
	gfx.PutPixel(x + 1, y + 2, 0, 255, 0); // dreapta jos 2 - 1 
	gfx.PutPixel(x + 2, y + 2, 0, 255, 0); // dreapta jos 2 - 2 
	gfx.PutPixel(x + 3, y + 2, 0, 255, 0); // dreapta jos 2 - 3 
	gfx.PutPixel(x + 4, y + 2, 0, 255, 0); // dreapta jos 2 - 4 
	gfx.PutPixel(x + 5, y + 2, 0, 255, 0); // dreapta jos 2 - 5 
	gfx.PutPixel(x + 6, y + 2, 0, 255, 0); // dreapta jos 2 - 6 
	gfx.PutPixel(x + 1, y + 3, 0, 255, 0); // dreapta jos 3 - 1
	gfx.PutPixel(x + 2, y + 3, 0, 255, 0); // dreapta jos 3 - 2
	gfx.PutPixel(x + 3, y + 3, 0, 255, 0); // dreapta jos 3 - 3
	gfx.PutPixel(x + 4, y + 3, 0, 255, 0); // dreapta jos 3 - 4
	gfx.PutPixel(x + 5, y + 3, 0, 255, 0); // dreapta jos 3 - 5
	gfx.PutPixel(x + 6, y + 3, 0, 255, 0); // dreapta jos 3 - 6
	gfx.PutPixel(x + 1, y + 4, 0, 255, 0); // dreapta jos 4 - 1
	gfx.PutPixel(x + 2, y + 4, 0, 255, 0); // dreapta jos 4 - 2 
	gfx.PutPixel(x + 3, y + 4, 0, 255, 0); // dreapta jos 4 - 3
	gfx.PutPixel(x + 4, y + 4, 0, 255, 0); // dreapta jos 4 - 4
	gfx.PutPixel(x + 5, y + 4, 0, 255, 0); // dreapta jos 4 - 5
	gfx.PutPixel(x + 6, y + 4, 0, 255, 0); // dreapta jos 4 - 6
	gfx.PutPixel(x + 1, y + 5, 0, 255, 0); // dreapta jos 5 - 1
	gfx.PutPixel(x + 2, y + 5, 0, 255, 0); // dreapta jos 5 - 2
	gfx.PutPixel(x + 3, y + 5, 0, 255, 0); // dreapta jos 5 - 3
	gfx.PutPixel(x + 4, y + 5, 0, 255, 0); // dreapta jos 5 - 4
	gfx.PutPixel(x + 5, y + 5, 0, 255, 0); // dreapta jos 5 - 5
	gfx.PutPixel(x + 6, y + 5, 0, 255, 0); // dreapta jos 5 - 6
	gfx.PutPixel(x + 1, y + 6, 0, 255, 0); // dreapta jos 6 - 1
	gfx.PutPixel(x + 2, y + 6, 0, 255, 0); // dreapta jos 6 - 2
	gfx.PutPixel(x + 3, y + 6, 0, 255, 0); // dreapta jos 6 - 3
	gfx.PutPixel(x + 4, y + 6, 0, 255, 0); // dreapta jos 6 - 4
	gfx.PutPixel(x + 5, y + 6, 0, 255, 0); // dreapta jos 6 - 5
	gfx.PutPixel(x + 6, y + 6, 0, 255, 0); // dreapta jos 6 - 6

	// 4 / 4
	gfx.PutPixel(x - 1, y + 1, 0, 255, 0); // stanga jos 1 - 1
	gfx.PutPixel(x - 2, y + 1, 0, 255, 0); // stanga jos 1 - 2
	gfx.PutPixel(x - 3, y + 1, 0, 255, 0); // stanga jos 1 - 3
	gfx.PutPixel(x - 4, y + 1, 0, 255, 0); // stanga jos 1 - 4
	gfx.PutPixel(x - 5, y + 1, 0, 255, 0); // stanga jos 1 - 5
	gfx.PutPixel(x - 6, y + 1, 0, 255, 0); // stanga jos 1 - 6
	gfx.PutPixel(x - 1, y + 2, 0, 255, 0); // stanga jos 2 - 1 
	gfx.PutPixel(x - 2, y + 2, 0, 255, 0); // stanga jos 2 - 2 
	gfx.PutPixel(x - 3, y + 2, 0, 255, 0); // stanga jos 2 - 3 
	gfx.PutPixel(x - 4, y + 2, 0, 255, 0); // stanga jos 2 - 4 
	gfx.PutPixel(x - 5, y + 2, 0, 255, 0); // stanga jos 2 - 5 
	gfx.PutPixel(x - 6, y + 2, 0, 255, 0); // stanga jos 2 - 6 
	gfx.PutPixel(x - 1, y + 3, 0, 255, 0); // stanga jos 3 - 1
	gfx.PutPixel(x - 2, y + 3, 0, 255, 0); // stanga jos 3 - 2
	gfx.PutPixel(x - 3, y + 3, 0, 255, 0); // stanga jos 3 - 3
	gfx.PutPixel(x - 4, y + 3, 0, 255, 0); // stanga jos 3 - 4
	gfx.PutPixel(x - 5, y + 3, 0, 255, 0); // stanga jos 3 - 5
	gfx.PutPixel(x - 6, y + 3, 0, 255, 0); // stanga jos 3 - 6
	gfx.PutPixel(x - 1, y + 4, 0, 255, 0); // stanga jos 4 - 1
	gfx.PutPixel(x - 2, y + 4, 0, 255, 0); // stanga jos 4 - 2 
	gfx.PutPixel(x - 3, y + 4, 0, 255, 0); // stanga jos 4 - 3
	gfx.PutPixel(x - 4, y + 4, 0, 255, 0); // stanga jos 4 - 4
	gfx.PutPixel(x - 5, y + 4, 0, 255, 0); // stanga jos 4 - 5
	gfx.PutPixel(x - 6, y + 4, 0, 255, 0); // stanga jos 4 - 6
	gfx.PutPixel(x - 1, y + 5, 0, 255, 0); // stanga jos 5 - 1
	gfx.PutPixel(x - 2, y + 5, 0, 255, 0); // stanga jos 5 - 2
	gfx.PutPixel(x - 3, y + 5, 0, 255, 0); // stanga jos 5 - 3
	gfx.PutPixel(x - 4, y + 5, 0, 255, 0); // stanga jos 5 - 4
	gfx.PutPixel(x - 5, y + 5, 0, 255, 0); // stanga jos 5 - 5
	gfx.PutPixel(x - 6, y + 5, 0, 255, 0); // stanga jos 5 - 6
	gfx.PutPixel(x - 1, y + 6, 0, 255, 0); // stanga jos 6 - 1
	gfx.PutPixel(x - 2, y + 6, 0, 255, 0); // stanga jos 6 - 2
	gfx.PutPixel(x - 3, y + 6, 0, 255, 0); // stanga jos 6 - 3
	gfx.PutPixel(x - 4, y + 6, 0, 255, 0); // stanga jos 6 - 4
	gfx.PutPixel(x - 5, y + 6, 0, 255, 0); // stanga jos 6 - 5
	gfx.PutPixel(x - 6, y + 6, 0, 255, 0); // stanga jos 6 - 6
	///////////////////////////////////////////////////////////////////////// <----- SQUARE SPRITE

}