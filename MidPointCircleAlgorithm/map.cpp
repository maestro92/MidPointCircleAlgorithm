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
void Map::drawCircle_int(glm::vec2 center, int radius)
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



void Map::drawCircle_Float(glm::vec2 center, float radius)
{
	float x0 = center.x;
	float y0 = center.y;

	float x = 0;
	float y = radius;
	float err = 1.25 - radius;

	while (y >= x)
	{
		setCell_Float(x0 + x, y0 + y, Map::Cell::Wall);
		setCell_Float(x0 + y, y0 + x, Map::Cell::Wall);
		setCell_Float(x0 - y, y0 + x, Map::Cell::Wall);
		setCell_Float(x0 - x, y0 + y, Map::Cell::Wall);
		setCell_Float(x0 - x, y0 - y, Map::Cell::Wall);
		setCell_Float(x0 - y, y0 - x, Map::Cell::Wall);
		setCell_Float(x0 + y, y0 - x, Map::Cell::Wall);
		setCell_Float(x0 + x, y0 - y, Map::Cell::Wall);

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


