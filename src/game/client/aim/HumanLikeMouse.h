//
// Created by danii on 03.09.2023.
//

#ifndef DDNET_HUMANLIKEMOUSE_H
#define DDNET_HUMANLIKEMOUSE_H

#include <cmath>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <functional>
#include "engine/client.h"
#include "engine/input.h"
#include "game/client/component.h"

using namespace std;

struct Point {
	int x;
	int y;
};

class HumanLikeMouse : public CComponent
{
public:
	std::vector<Point> getPoints(int start_x, int start_y, int dest_x, int dest_y, double G_0 = 9, double W_0 = 3, double M_0 = 15, double D_0 = 12);
	void removeMoving();
	void moveToPoint(Point* targetPoint, float moveTime = 0.02, function<void()> onArrival = [](){});
	void moveToPlayer(int id, float moveTime = 0.02, function<void()> onArrival = [](){});
	virtual int Sizeof() const override { return sizeof(*this); }
	void OnUpdate();
	void cancel();
	bool isMoveEnded();
protected:
	Point getCurrentMousePosition();
	std::queue<Point> targetWay;
	int countPointsInWay;
	function<void()> onArrival;
	float endMoveTime;
	float countIterationsForOnePoint = 10;
	int movingToUser = -1;

	void processMouseMoving();
};

#endif // DDNET_HUMANLIKEMOUSE_H
