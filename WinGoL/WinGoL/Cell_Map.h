#pragma once
#include "Cell.h"
#include <vector>
#include <map>
#include <boost/foreach.hpp>

typedef std::map <Cell, bool> gol_Map;


class Cell_Map{

public:
	
	Cell_Map();

	void Add_Cell(int,int);
	void Add_Cell(gol_Map&, int, int);
	void Next_Gen();
	gol_Map& Get_Cellmap();
	

private:

	gol_Map cell_Collection;
	std::vector <std::vector<int>> relative_a;

	int Count_Neighbors(int,int);


};