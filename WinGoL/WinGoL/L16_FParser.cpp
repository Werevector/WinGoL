#include "stdafx.h"
#include "L16_FParser.h"


using namespace std;
int L16_Parser::Load_Pattern(Cell_Map& c_Field, string file_Name){

	ifstream field_File(file_Name);
	if (field_File.is_open()){
		int x, y;
		string line;
		while (getline(field_File, line)){
			if (line[0] != '#'){
				istringstream(line) >> x >> y;

				c_Field.Add_Cell(x,y);
			}
		}
	}
	else{
		cout << "Could not open file " + file_Name;
		return 1;
	}

	return 0;

}

int L16_Parser::Write_Pattern(Cell_Map& c_Field, string file_Name){

	//Open fileStream
	ofstream field_File(file_Name);

	gol_Map gol_Field = c_Field.Get_Cellmap();
	
	//Life 10.6 header
	field_File << "#Life 1.06\n";

	//Write cellmap to file
	if (field_File.is_open()){
		int x, y;
		
		BOOST_FOREACH(gol_Map::value_type cell, gol_Field){
			if (cell.second){
				x = cell.first.x_Pos;
				y = cell.first.y_Pos;

				field_File << x << " " << y << "\n";
			}
		}

	}
	else{
		cout << "Could not create file " + file_Name;
		return 1;
	}

	return 0;

}