#include "map.h"


Map::Map()
{
	m_cellSize = 1;
}


void Map::init(int w, int h)
{
	for (int y = 0; y < h; y++)
	{
		vector<Cell> row(w);
		gridmap.push_back(row);
	}

	
	srand(time(NULL));
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			gridmap[y][x] = Map::Cell::Empty;
		}
	}



	if (saveLatest)
	{
		save();
	}
}





bool Map::IsValidRange(glm::vec2 coord)
{
	if (coord.x < 0)	return false;
	if (coord.y < 0)	return false;
	if (coord.x >= getWidth())	return false;
	if (coord.y >= getHeight())	return false;
	return true;
}



int Map::getWidth()
{
	return gridmap[0].size();
}

int Map::getHeight()
{
	return gridmap.size();
}

Map::Cell Map::getCell(int x, int y)
{
	return gridmap[y][x];
}

Map::Cell Map::getCell(glm::vec2 coord)
{
	return gridmap[coord.y][coord.x];
}


void Map::debug()
{
	for (int y = 0; y < getHeight(); y++)
	{
		string s = "";
		for (int x = 0; x < getWidth(); x++)
		{
			s += utl::intToStr((int)(gridmap[y][x]));
		}
		cout << s << endl;
	}
	cout << endl;
}


void Map::resetFlags()
{

}


void Map::clearMap()
{
	for (int y = 0; y < getHeight(); y++)
	{
		for (int x = 0; x < getWidth(); x++)
		{
			gridmap[y][x] = Map::Cell::Empty;
		}
	}
}


Object Map::serializeCell(Map::Cell gem)
{
	Object pointObj;

	pointObj.push_back(Pair("cell", (int)gem));

	return pointObj;
}


Map::Cell Map::deserializeCell(const mObject& obj)
{
	int intCell = utl::findValue(obj, "cell").get_int();
	return (Map::Cell)intCell;
}



float Map::getCellSize()
{
	return m_cellSize;
}


// http://www.cplusplus.com/forum/beginner/60827/
// c++ truncates, aka rounds down
glm::vec2 Map::worldPos2GridCoord(glm::vec2 pos)
{
	return glm::vec2(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

glm::vec2 Map::gridCoord2WorldPos(glm::vec2 gridCoord)
{
	return gridCoord;
}

glm::vec2 Map::getCellMinCorner(glm::vec2 gridCoord)
{
	return gridCoord;
}

glm::vec2 Map::getCellMaxCorner(glm::vec2 gridCoord)
{
	glm::vec2 pos = gridCoord;
	pos.x += m_cellSize;
	pos.y += m_cellSize;
	return pos;
}

glm::vec2 Map::getCellCenter(glm::vec2 gridCoord)
{
	glm::vec2 pos = gridCoord;
	pos.x += m_cellSize / 2;
	pos.y += m_cellSize / 2;
	return pos;
}

// https://www.youtube.com/watch?v=QeOvZTYlmAI&t=104s

/*
void Map::drawCircle(glm::vec2 center, int radius)
{
	int x0 = center.x;
	int y0 = center.y;

	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		setCell(x0 + x, y0 + y, Map::Cell::Wall);
		setCell(x0 + y, y0 + x, Map::Cell::Wall);
		setCell(x0 - y, y0 + x, Map::Cell::Wall);
		setCell(x0 - x, y0 + y, Map::Cell::Wall);
		setCell(x0 - x, y0 - y, Map::Cell::Wall);
		setCell(x0 - y, y0 - x, Map::Cell::Wall);
		setCell(x0 + y, y0 - x, Map::Cell::Wall);
		setCell(x0 + x, y0 - y, Map::Cell::Wall);

		if (err <= 0)
		{
			y++;
			err += 2*y + 1;
		}

		if (err > 0)
		{
			x--;
			err -= 2*x  + 1;
		}

	}

}
*/

// https://www.youtube.com/watch?v=QeOvZTYlmAI&t=607s
void Map::drawCircle_int(glm::vec2 center, int radius, bool fill)
{
	if (fill)
	{
		fillCircle_int(center, radius);
	}
	else
	{
		drawCircleOutline_int(center, radius);
	}
}



// https://www.youtube.com/watch?v=QeOvZTYlmAI&t=607s
void Map::drawCircleOutline_int(glm::vec2 center, int radius)
{
	int x0 = center.x;
	int y0 = center.y;

	int x = 0;
	int y = radius;
	int err = 1 - radius;

	while (y >= x)
	{
		setCell(x0 + x, y0 + y, Map::Cell::Wall);
		setCell(x0 + y, y0 + x, Map::Cell::Wall);
		setCell(x0 - y, y0 + x, Map::Cell::Wall);
		setCell(x0 - x, y0 + y, Map::Cell::Wall);
		setCell(x0 - x, y0 - y, Map::Cell::Wall);
		setCell(x0 - y, y0 - x, Map::Cell::Wall);
		setCell(x0 + y, y0 - x, Map::Cell::Wall);
		setCell(x0 + x, y0 - y, Map::Cell::Wall);

		if (err <= 0)
		{
			err = err + 2 * x + 3;
		}
		else
		{
			err = err + 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}


void Map::fillCircle_int(glm::vec2 center, float radius)
{
	int x0 = center.x;
	int y0 = center.y;

	int x = 0;
	int y = radius;
	int err = 1 - radius;

	while (y >= x)
	{
		fillLine_int(x0 - x, x0 + x, y0 + y, Map::Cell::Wall);
		fillLine_int(x0 - y, x0 + y, y0 + x, Map::Cell::Wall);
		fillLine_int(x0 - x, x0 + x, y0 - y, Map::Cell::Wall);
		fillLine_int(x0 - y, x0 + y, y0 - x, Map::Cell::Wall);

		if (err <= 0)
		{
			err = err + 2 * x + 3;
		}
		else
		{
			err = err + 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}



void Map::fillLine_int(int x0, int x1, int y, Map::Cell gem)
{
	for (int x = x0; x <= x1; x++)
	{
		if (IsValidRange(glm::vec2(x, y)))
		{
			gridmap[y][x] = gem;
		}
	}
}




void Map::drawCircle_float(glm::vec2 center, float radius, bool fill)
{
	if (fill)
	{
		fillCircle_float(center, radius);
	}
	else
	{
		drawCircleOutline_float(center, radius);
	}
}




void Map::drawCircleOutline_float(glm::vec2 center, float radius)
{
	float x0 = center.x;
	float y0 = center.y;

	float x = 0;
	float y = radius;
	float err = 1.25 - radius;

	glm::vec2 q1_g0;
	glm::vec2 q1_g1;

	glm::vec2 q2_g0;
	glm::vec2 q2_g1;

	glm::vec2 q3_g0;
	glm::vec2 q3_g1;

	glm::vec2 q4_g0;
	glm::vec2 q4_g1;


	// need to the examine the 
	while ( (int)y >= (int)x) 
	{					
	/*
							

			q2_g0	|	q1_g0
					|	
		q2_g1		|		q1_g1
		____________|___________
					|
		q3_g1		|		q4_g1
					|
			q3_g0	|	q4_g0

	*/

		q1_g0 = glm::vec2((int)(x0 + x), (int)(y0 + y));
		q1_g1 = glm::vec2((int)(x0 + y), (int)(y0 + x));
		
		q2_g0 = glm::vec2((int)(x0 - x), (int)(y0 + y));
		q2_g1 = glm::vec2((int)(x0 - y), (int)(y0 + x));

		q3_g0 = glm::vec2((int)(x0 - x), (int)(y0 - y));
		q3_g1 = glm::vec2((int)(x0 - y), (int)(y0 - x));

		q4_g0 = glm::vec2((int)(x0 + x), (int)(y0 - y));
		q4_g1 = glm::vec2((int)(x0 + y), (int)(y0 - x));


		// first quadrant;
		setCell(x0 + x, y0 + y, Map::Cell::Wall);
		setCell(x0 + y, y0 + x, Map::Cell::Wall);
		
		// second quadant
		setCell(x0 - x, y0 + y, Map::Cell::Wall);
		setCell(x0 - y, y0 + x, Map::Cell::Wall);

		// third quadant
		setCell(x0 - x, y0 - y, Map::Cell::Wall);
		setCell(x0 - y, y0 - x, Map::Cell::Wall);

		// fourth quadrant
		setCell(x0 + y, y0 - x, Map::Cell::Wall);
		setCell(x0 + x, y0 - y, Map::Cell::Wall);
		
		// if the error function is less than zero, we only increment x
		if (err <= 0)
		{
			err = err + 2 * x + 3;
		}
		else
		{
			err = err + 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
	
	// first quardant
	int gx = q1_g0.x;
	int gy = q1_g0.y;
	if (q1_g1.x - q1_g0.x > 1)
	{
		gx = q1_g0.x + 1;
 	}

	if (q1_g0.y - q1_g1.y > 1)
	{
		gy = q1_g0.y - 1;
	}
	setCell_Float(gx, gy, Map::Cell::Wall);
	

	// 2nd quadrant
	gx = q2_g0.x;
	gy = q2_g0.y;
	if ( (q2_g0.x - q2_g1.x) > 1)
	{
		gx = q2_g0.x - 1;
	}
	if ( (q2_g0.y - q2_g1.y) > 1)
	{
		gy = q2_g0.y - 1;
	}
	
	setCell_Float(gx, gy, Map::Cell::Wall);

	
	// 3rd quadrant
	gx = q3_g0.x;
	gy = q3_g0.y;
	if ((q3_g1.y - q3_g0.y) > 1)
	{
		gy = q3_g0.y + 1;
	}

	if ( (q3_g0.x - q3_g1.x) > 1)
	{
		gx = q3_g0.x - 1;
	}
	setCell_Float(gx, gy, Map::Cell::Wall);

	
	// 4th quadrant
	gx = q4_g0.x;
	gy = q4_g0.y;
	if ( (q4_g1.y - q4_g0.y) > 1)
	{
		gy = q4_g0.y + 1;
	}

	if ( (q4_g1.x - q4_g0.x) > 1)
	{
		gx = q4_g0.x + 1;
	}
	setCell_Float(gx, gy, Map::Cell::Wall);
}



void Map::fillCircle_float(glm::vec2 center, float radius)
{
	float x0 = center.x;
	float y0 = center.y;

	float x = 0;
	float y = radius;
	float err = 1.25 - radius;

	GridCoord q1_g0, q1_g1, q2_g0, q2_g1, q3_g0, q3_g1, q4_g0, q4_g1;

	// need to the examine the 
	while ((int)y >= (int)x)
	{
		q1_g0 = GridCoord((int)(x0 + x), (int)(y0 + y));
		q1_g1 = GridCoord((int)(x0 + y), (int)(y0 + x));

		q2_g0 = GridCoord((int)(x0 - x), (int)(y0 + y));
		q2_g1 = GridCoord((int)(x0 - y), (int)(y0 + x));

		q3_g0 = GridCoord((int)(x0 - x), (int)(y0 - y));
		q3_g1 = GridCoord((int)(x0 - y), (int)(y0 - x));

		q4_g0 = GridCoord((int)(x0 + x), (int)(y0 - y));
		q4_g1 = GridCoord((int)(x0 + y), (int)(y0 - x));

		fillLine_int((int)(x0 - x), (int)(x0 + x), (int)(y0 + y), Map::Cell::Wall);
		fillLine_int((int)(x0 - x), (int)(x0 + x), (int)(y0 - y), Map::Cell::Wall);
		fillLine_int((int)(x0 - y), (int)(x0 + y), (int)(y0 - x), Map::Cell::Wall);
		fillLine_int((int)(x0 - y), (int)(x0 + y), (int)(y0 + x), Map::Cell::Wall);

		// if the error function is less than zero, we only increment x
		if (err <= 0)
		{
			err = err + 2 * x + 3;
		}
		else
		{
			err = err + 2 * (x - y) + 5;
			y--;
		}
		x++;
	}

	// first quardant
	int gx1 = q1_g0.x;
	int gy1 = q1_g0.y;
	if (q1_g1.x - q1_g0.x > 1)
	{
		gx1 = q1_g0.x + 1;
	}

	if (q1_g0.y - q1_g1.y > 1)
	{
		gy1 = q1_g0.y - 1;
	}


	// 2nd quadrant
	int gx2 = q2_g0.x;
	int gy2 = q2_g0.y;
	if ((q2_g0.x - q2_g1.x) > 1)
	{
		gx2 = q2_g0.x - 1;
	}
	if ((q2_g0.y - q2_g1.y) > 1)
	{
		gy2 = q2_g0.y - 1;
	}
	fillLine_int(gx2, gx1, gy1, Map::Cell::Wall);


	// 3rd quadrant
	int gx3 = q3_g0.x;
	int gy3 = q3_g0.y;
	if ((q3_g1.y - q3_g0.y) > 1)
	{
		gy3 = q3_g0.y + 1;
	}

	if ((q3_g0.x - q3_g1.x) > 1)
	{
		gx3 = q3_g0.x - 1;
	}



	// 4th quadrant
	int gx4 = q4_g0.x;
	int gy4 = q4_g0.y;
	if ((q4_g1.y - q4_g0.y) > 1)
	{
		gy4 = q4_g0.y + 1;
	}

	if ((q4_g1.x - q4_g0.x) > 1)
	{
		gx4 = q4_g0.x + 1;
	}
	fillLine_int(gx3, gx4, gy4, Map::Cell::Wall);

}

/*
void Map::fillCircle_float(glm::vec2 center, float radius)
{
	float x0 = center.x;
	float y0 = center.y;

	float x = 0;
	float y = radius;
	float err = 1.25 - radius;

	// to compensate for non-symmetry
	// sometimes, there is no symmetry, so we do an extra iteration to make up for it
	//	while (y >= x)
	while (y >= (x - 1))
	{
		fillLine_float(x0 - x, x0 + x, y0 + y, Map::Cell::Wall);
		fillLine_float(x0 - y, x0 + y, y0 + x, Map::Cell::Wall);
		fillLine_float(x0 - x, x0 + x, y0 - y, Map::Cell::Wall);
		fillLine_float(x0 - y, x0 + y, y0 - x, Map::Cell::Wall);

		// if the error function is less than zero, we only increment x
		if (err <= 0)
		{
			err = err + 2 * x + 3;
		}
		else
		{
			err = err + 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}
*/

void Map::fillLine_float(float x0, float x1, float y, Map::Cell gem)
{
	int gy = (int)y;

	int gx0 = (int)x0;
	int gx1 = (int)x1;

	for (float gx = gx0; gx <= gx1; gx++)
	{
		if (IsValidRange(glm::vec2(gx, gy)))
		{
			gridmap[gy][gx] = gem;
		}
	}
}


void Map::save()
{
	ofstream myfile;
	myfile.open("data.txt");
	cout << "saving data " << endl;


	Object boardObj;
	boardObj.push_back(Pair("h", (int)gridmap.size()));
	boardObj.push_back(Pair("w", (int)gridmap[0].size()));

	Array array;
	for (int y = 0; y < gridmap.size(); y++)
	{
		Array row;
		for (int x = 0; x < gridmap[0].size(); x++)
		{
			Object vObj = serializeCell(gridmap[y][x]);
			row.push_back(vObj);
		}

		array.push_back(row);
	}

	boardObj.push_back(Pair("gridmap", array));
	write(boardObj, myfile, pretty_print);
	myfile.close();
}

void Map::setCell(int x, int y, Map::Cell gem)
{
	if (IsValidRange(glm::vec2(x, y)))
	{
		gridmap[y][x] = gem;
	}
}

void Map::setCell(GridCoord gc, Map::Cell gem)
{
	if (IsValidRange(glm::vec2(gc.x, gc.y)))
	{
		gridmap[gc.y][gc.x] = gem;
	}
}

void Map::setCell_Float(float x, float y, Map::Cell gem)
{
	int gx = (int)x;
	int gy = (int)y;

	if (IsValidRange(glm::vec2(gx, gy)))
	{
		gridmap[gy][gx] = gem;
	}
}

void Map::load(char* filename)
{
	mValue content = utl::readJsonFileToMap(filename);

	const mObject& obj = content.get_obj();

	int w = utl::findValue(obj, "w").get_int();
	int h = utl::findValue(obj, "h").get_int();

	cout << w << " " << h << endl;

	vector<vector<Cell>> newMap;

	for (int y = 0; y < h; y++)
	{
		vector<Cell> row(w);
		newMap.push_back(row);
	}

	const mArray& boardArray = utl::findValue(obj, "gridmap").get_array();
	for (int y = 0; y < h; y++)
	{
		const mArray& row = boardArray[y].get_array();
		vector<Cell> gemRow(w);
		for (int x = 0; x < w; x++)
		{
			const mObject obj = row[x].get_obj();
			Cell gem = deserializeCell(obj);
			newMap[y][x] = gem;
		}
	}

	gridmap = newMap;
}




void Map::init(vector<string> worldMap)
{
	int w = worldMap[0].size();
	int h = worldMap.size();

	vector<vector<Cell>> newMap;
	for (int y = 0; y < h; y++)
	{
		vector<Cell> row(w);
		newMap.push_back(row);
	}

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			newMap[y][x] = worldMap[h - y - 1][x] == '1' ? Map::Cell::Wall : Map::Cell::Empty;
		}
	}

	gridmap = newMap;
}


void Map::debugVisitedFlags()
{

}


