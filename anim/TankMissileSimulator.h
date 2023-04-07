#pragma once

#include "BaseSimulator.h"
#include "Tank.h"
#include "Missile.h"


class TankMissileSimulator : public BaseSimulator {
	
public:
	TankMissileSimulator(const std::string &name, Tank *tank, Missile *miss );
	~TankMissileSimulator();
	
	int step(double time);
	int init(double time);

	int command(int argc, myCONST_SPEC char** argv) { return TCL_OK; }
private:

	Tank *m_Tank;
	Missile *m_Miss;
	double m_prevTime;

};