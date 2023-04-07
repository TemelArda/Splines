#pragma once
#include "BaseSystem.h"

// a sample system
class Terrain : public BaseSystem
{

public:
	Terrain(const Vector& pos, const std::string& name = "Terrain", float m_sx = 1, float m_sy = 1, float m_sz = 1);
	Terrain(const std::string& name = "Terrain", float m_sx = 1, float m_sy = 1, float m_sz = 1);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	int command(int argc, myCONST_SPEC char** argv);

private:

	float m_sx;
	float m_sy;
	float m_sz;
	Vector m_pos;
};

