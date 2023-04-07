#pragma once
#include "BaseSystem.h"	
#include "util/Quaternion.h"
#include "Hermite.h"
#define END_TIME 40


class Missile : public BaseSystem
{

public:
	Missile(const std::string& name);
	~Missile()
	{
		delete m_path;
		m_path = 0;
		delete m_rot;
		m_rot = 0;
		delete m_initialRot;
		m_initialRot = 0;
	
	}
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);
	void Update(double time);



	void SetVelocity(double vel);
	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);
	
	void SetTarget(const double* targetPos);
	void SetTargetTangent(const double* Tangent);

protected:
	void Rotate(double t);
	double Ease1(double t) {
		return t * m_vel * 0.0025;
	}
	double Ease2(double t) {
		return t * m_vel;
	}
	
	float m_sx;
	float m_sy;
	float m_sz;

	double m_vel;
	Vector m_target;
	Vector m_pos;
	Vector m_initialPosition;
	Vector m_targetTangent;
	Vector m_tangent;
	double m_front[4];
	Quaternion *m_rot;
	Quaternion* m_initialRot;




	Hermite *m_path;
	GLMmodel m_model;

};