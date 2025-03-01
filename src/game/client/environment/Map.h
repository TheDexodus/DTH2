//
// Created by danii on 09.09.2023.
//

#ifndef DDNET_MAP_H
#define DDNET_MAP_H

#include "engine/client.h"
#include "game/client/component.h"

using namespace std;

#define MAP_TILE_UNDER_BORDER -1
#define MAP_TILE_AIR 0
#define MAP_TILE_HOOKABLE_GROUND 1
#define MAP_TILE_NON_HOOKABLE_GROUND 3

#define FREE_MOVE_ACTION_NONE 0
#define FREE_MOVE_ACTION_LEFT 1
#define FREE_MOVE_ACTION_RIGHT 2
#define FREE_MOVE_ACTION_JUMP 3
#define FREE_MOVE_ACTION_FALL 4

struct MapGraphNode {
	ivec2 connectionWith;
	int cost;
	int freeMoveAction;

	MapGraphNode() = default;

	MapGraphNode(ivec2 connectionWith, int cost, int freeMoveAction = FREE_MOVE_ACTION_NONE)
	{
		this->connectionWith = connectionWith;
		this->cost = cost;
		this->freeMoveAction = freeMoveAction;
	}

	MapGraphNode(int x, int y, int cost, int freeMoveAction = FREE_MOVE_ACTION_NONE)
	{
		this->connectionWith = ivec2(x, y);
		this->cost = cost;
		this->freeMoveAction = freeMoveAction;
	}
};
typedef vector<MapGraphNode> MapGraphCell;
typedef vector<MapGraphCell> MapGraphLine;
typedef vector<MapGraphLine> MapGraph;

struct Node {
	int x, y;
	int cost;
	Node* parent;

	bool operator==(const Node& other) const {
		return x == other.x && y == other.y;
	}
};

struct PriorityQueueCompare {
	bool operator()(const Node* a, const Node* b) {
		return a->cost > b->cost;
	}
};

class Map : public CComponent
{
public:
	void scanTiles();

	void buildGraph();
	vector<Node*> aStar(ivec2 startPosition, ivec2 endPosition);
	int findGroundNear(ivec2 position);
	int findGroundBottom(ivec2 position);

	int getTile(int mapX, int mapY);
	int getTile(ivec2 mapPosition);

	bool canWalk(ivec2 startPosition, ivec2 endPosition);
	bool canFallFromGround(ivec2 startPosition, ivec2 endPosition);
	bool canFall(ivec2 startPosition, ivec2 endPosition);
	bool canJumpOnWall(ivec2 startPosition, ivec2 endPosition);

	bool isTileGround(int tileId);

	virtual void OnMapLoad() override;
	virtual int Sizeof() const override { return sizeof(*this); }

	vec2 convertWorldToUI(vec2 worldPos);
	vec2 convertMapToUI(vec2 mapPos);
protected:
	vector<vector<int> > tiles;
	MapGraph graph;
};

#endif // DDNET_MAP_H
