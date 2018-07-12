#pragma once

#include "utility_math.h"
#include <vector>
#include <functional>
#include "utility_json.h"
#include "utility_debug.h"
#include <unordered_set>
#include <iostream>
#include <fstream>
using namespace std;


class Map
{


	public: 	
		enum class Cell
		{
			Empty,
			Wall,	
			NUM_TYPES
		};

		Map();

		void init(int w, int h);
		void init(vector<string> worldMap);

		void debug();
		
		int getWidth();
		int getHeight();

		Cell getCell(glm::vec2 coord);
		Cell getCell(int x, int y);
		void setCell(int x, int y, Map::Cell gem);
		void setCell(glm::ivec2 gc, Map::Cell gem);
		void setCell_Float(float x, float y, Map::Cell gem);

		void save();
		void load(char* filename);
		void clearMap();

		void debugVisitedFlags();
//		void drawCircle(glm::vec2 center, int radius);

		void drawCircle_int(glm::vec2 center, int radius, bool fill);
		void drawCircleOutline_int(glm::vec2 center, int radius);
		void fillCircle_int(glm::vec2 center, float radius);
		void fillLine_int(int x0, int x1, int y, Map::Cell gem);

		void drawCircle_float(glm::vec2 center, float radius, bool fill);
		void drawCircleOutline_float(glm::vec2 center, float radius);
		
		void drawCircleOutline_float_oct1(glm::vec2 center, float radius);
		void drawCircleOutline_float_oct2(glm::vec2 center, float radius);
		void drawCircleOutline_float_oct3(glm::vec2 center, float radius);
		void drawCircleOutline_float_oct4(glm::vec2 center, float radius);
		void drawCircleOutline_float_oct5(glm::vec2 center, float radius);
		void drawCircleOutline_float_oct6(glm::vec2 center, float radius);
		void drawCircleOutline_float_oct7(glm::vec2 center, float radius);
		void drawCircleOutline_float_oct8(glm::vec2 center, float radius);
		void drawCircleOutline_float_traverseY2X(int x, int y, glm::vec2 center, float err, int xSign, int ySign);


		void drawCircleOutline_float_traverseX2Y(int x, int y, glm::vec2 center, float err, int xSign, int ySign);

		void fillCircle_float(glm::vec2 center, float radius);
		void fillLine_float(float x0, float x1, float y, Map::Cell gem);
		void resetFlags();


		bool saveLatest;

		glm::ivec2 worldPos2GridCoord(glm::vec2 pos);
		glm::vec2 gridCoord2WorldPos(glm::ivec2 gridCoord);
		glm::vec2 getCellMinCorner(glm::ivec2 gridCoord);
		glm::vec2 getCellMaxCorner(glm::ivec2 gridCoord);
		glm::vec2 getCellCenter(glm::ivec2 gridCoord);

		float getCellSize();
		bool IsValidRange(glm::vec2 coord);

	private:
		float m_cellSize;

		Object serializeCell(Map::Cell gem);
		Map::Cell deserializeCell(const mObject& obj);

		vector<vector<Cell>> gridmap;
};