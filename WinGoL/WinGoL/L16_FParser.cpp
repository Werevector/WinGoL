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