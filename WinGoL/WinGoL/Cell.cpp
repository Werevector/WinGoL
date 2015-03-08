#include "stdafx.h"
#include "Cell.h"


bool operator ==(const Cell& a, const Cell& b) { return (a.x_Pos == b.x_Pos) && (a.y_Pos == b.y_Pos); }

bool operator <(const Cell& a, const Cell& b) { return (a.x_Pos < b.x_Pos) ? true : ((a.x_Pos == b.x_Pos) ? (a.y_Pos < b.y_Pos) : false); }