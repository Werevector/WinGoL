#pragma once
#include "stdafx.h"
#include "SDL.h"
#include <iostream>
#include "Cell_Map.h"
#include "Settings.h"



class GoL_Renderer{

public:

	~GoL_Renderer();
	GoL_Renderer();

	void Render_Life(gol_Map&);

	int Init(HWND hWnd);




private:

	SDL_Renderer* c_Renderer;
	SDL_Window* gol_Win;

	SDL_Rect cellRect;

	void Render_Cell(int,int);

	int SCREEN_WIDTH, SCREEN_HEIGHT;


};