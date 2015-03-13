#include "stdafx.h"
#include "GoL_Renderer.h"

GoL_Renderer::~GoL_Renderer(){
	SDL_DestroyRenderer(c_Renderer);
	SDL_DestroyWindow(gol_Win);
	gol_Win = NULL;
	c_Renderer = NULL;

}

GoL_Renderer::GoL_Renderer(){

	SCREEN_HEIGHT = 800;
	SCREEN_WIDTH = 800;

}

int GoL_Renderer::Init(HWND hWnd){
	
	gol_Win = SDL_CreateWindowFrom(hWnd);

	if (gol_Win == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return 0;
	}
	else{
		c_Renderer = SDL_CreateRenderer(gol_Win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (c_Renderer == NULL){
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return 0;
		}

	}


	cellRect.h = 1;
	cellRect.w = 1;

	return 1;

}

void GoL_Renderer::Render_Cell(int x, int y){
	
	cellRect.x = (x * cellRect.w)+cameraX;
	cellRect.y = (y * cellRect.h)+cameraY;

	cellRect.h = 1+cameraZ;
	cellRect.w = 1+cameraZ;
	
	SDL_RenderFillRect(c_Renderer, &cellRect);

}

void GoL_Renderer::Render_Life(gol_Map& map){
	
	//Clear screen
	SDL_SetRenderDrawColor(c_Renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(c_Renderer);

	
	BOOST_FOREACH(gol_Map::value_type cell, map){
		if (cell.second){
			SDL_SetRenderDrawColor(c_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			Render_Cell(cell.first.x_Pos, cell.first.y_Pos);
		}
		else{
			SDL_SetRenderDrawColor(c_Renderer, 0xFF, 0x00, 0xFF, 0xFF);
			Render_Cell(cell.first.x_Pos, cell.first.y_Pos);
		}
	}

	SDL_RenderPresent(c_Renderer);

}