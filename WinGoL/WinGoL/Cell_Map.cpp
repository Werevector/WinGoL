#include "stdafx.h"
#include "Cell_Map.h"

Cell_Map::Cell_Map(){
	
	relative_a = {
			{ -1, -1 },
			{ -1, 0 },
			{ -1, 1 },
			{ 0, -1 },
			{ 0, 1 },
			{ 1, -1 },
			{ 1, 0 },
			{ 1, 1 }
	};

}

void Cell_Map::Add_Cell(int x, int y){

	Cell newCell;
	newCell.x_Pos = x;
	newCell.y_Pos = y;

	cell_Collection[newCell] = true;

	for (int i = 0; i < relative_a.size() ; i++){
		newCell.x_Pos = x + relative_a[i][0];
		newCell.y_Pos = y + relative_a[i][1];

		cell_Collection.emplace(newCell, false);
	}

}

void Cell_Map::Add_Cell(gol_Map& map, int x, int y){
	Cell newCell;
	newCell.x_Pos = x;
	newCell.y_Pos = y;

	map[newCell] = true;

	for (int i = 0; i < relative_a.size(); i++){
		newCell.x_Pos = x + relative_a[i][0];
		newCell.y_Pos = y + relative_a[i][1];

		map.emplace(newCell, false);
	}
}

int Cell_Map::Count_Neighbors(int x, int y){
	int result = 0;
	Cell testCell;
	for (int i = 0; i < relative_a.size(); i++){
		
		testCell.x_Pos = x + relative_a[i][0];
		testCell.y_Pos = y + relative_a[i][1];
		
		gol_Map::iterator iterator = cell_Collection.find(testCell);
		if (iterator != cell_Collection.end() && iterator->second == true){
			result++;
		}
	}

	return result;
}

void Cell_Map::Next_Gen(){
	int count = 0;

	gol_Map new_Map;

	BOOST_FOREACH(gol_Map::value_type cell, cell_Collection){

		count = Count_Neighbors(cell.first.x_Pos, cell.first.y_Pos);
		if (count == 3 || (cell.second && count == 2)){
			Add_Cell(new_Map, cell.first.x_Pos, cell.first.y_Pos);
		}
	}

	cell_Collection = new_Map;

}

void Cell_Map::Clear_Map(){
	cell_Collection.clear();
}

gol_Map& Cell_Map::Get_Cellmap(){
	return cell_Collection;
}