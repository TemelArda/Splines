#include "TankMissileSimulator.h"

TankMissileSimulator::TankMissileSimulator(const std::string& name, Tank *tank, Missile *miss) :
	BaseSimulator(name), m_Tank(tank), m_Miss(miss)
{}

TankMissileSimulator::~TankMissileSimulator()
{
	delete m_Tank;
	delete m_Miss;
	m_Tank = 0;
	m_Miss = 0;
}

int TankMissileSimulator::step(double time)
{

	m_Tank->Update(time);
	m_Miss->SetTarget(*m_Tank->GetPos());
	m_Miss->SetTargetTangent(*m_Tank->GetTangent());
	m_Miss->Update(time);
	if (abs(time - m_prevTime) > 2) {
		double v = m_Tank->getVelocity(time);
		animTcl::OutputMessage("Tank Velocity: %f", v);
		m_prevTime = time;
	}

	return 0;
}

int TankMissileSimulator::init(double time)
{
	return 0;
}
