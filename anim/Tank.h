#pragma once
#include "BaseSystem.h"	
#include "Hermite.h"
#define END_TIME 40

class Tank : public BaseSystem
{

public:
	Tank(const std::string& name);
	~Tank();
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);
	void Update(double time);
	
	void LoadPath(const std::string& fileName);
	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	double getVelocity(double t);
	int command(int argc, myCONST_SPEC char** argv);
	Vector* GetTangent() { return &m_tangent; }


	Vector* GetPos() { return &m_pos; }


protected:

	double Ease(double t);

	float m_sx;
	float m_sy;
	float m_sz;

	Vector m_pos;
	Vector m_tangent;


	double m_PathLenght;
	Hermite* m_Path;
	GLMmodel m_model;

};