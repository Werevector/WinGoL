#pragma once
/////////////////////////
//DEBUG GLOBALS
struct Debg{

	//Position of mouse in SDL
	POINT mousePos;
	//Position of last entered CELL
	POINT lastCell;

	//Time of Generation generation
	double Gen_time;

};

//Camera Options
////Camera Center
extern int cameraX;
extern int cameraY;
////Camera Zoom
extern int cameraZ;
////Camera Speed
extern int cameraV;

//Field Options
extern bool gridON;
extern bool dead_Colour_ON;

//SDL Window Size
extern int sdlw_Width;
extern int sdlw_Height;

//Debug on/off
extern bool dbg_Toggle;

