#pragma once
#include <boost/unordered_map.hpp>
struct Cell{

	int x_Pos;
	int y_Pos;

};

bool operator==(Cell const& a, Cell const& b);

bool operator<(const Cell& a, const Cell& b);