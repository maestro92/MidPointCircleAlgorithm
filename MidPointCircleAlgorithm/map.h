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

		void setCell_Float(float x, float y, Map::Cell gem);

		void save();
		void load(char* filename);
		void clearMap();

		void debugVisitedFlags();
//		void drawCircle(glm::vec2 center, int radius);

		void drawCircle_int(glm::vec2 center, int radius);
		void drawCircle_Float(glm::vec2 center, float radius);

		void resetFlags();

		bool saveLatest;

		glm::vec2 worldPos2GridCoord(glm::vec2 pos);
		glm::vec2 gridCoord2WorldPos(glm::vec2 gridCoord);
		glm::vec2 getCellMinCorner(glm::vec2 gridCoord);
		glm::vec2 getCellMaxCorner(glm::vec2 gridCoord);
		glm::vec2 getCellCenter(glm::vec2 gridCoord);

		float getCellSize();
		bool IsValidRange(glm::vec2 coord);

	private:
		float m_cellSize;

		Object serializeCell(Map::Cell gem);
		Map::Cell deserializeCell(const mObject& obj);

		vector<vector<Cell>> gridmap;
};