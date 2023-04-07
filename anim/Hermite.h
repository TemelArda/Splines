#pragma once
#include "BaseSystem.h"
#define MAX_CONTROLPOINTS 40
#define NUMBER_INDICES 30

class Hermite: public BaseSystem {

public:
	Hermite();

	Hermite(const std::string& name);
	
	virtual void getState(double* p);
	
	virtual void setState(double* p);

	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	int command(int argc, myCONST_SPEC char** argv);

	void CRinitilization();

	void SetTangent(int index, const Vector& tangent);
	
	void SetPoint(int index, const Vector& point);
	
	void AddPoint(const Vector& point, const Vector& tangent);
	
	double GetArcLength(double t);

	void Load(const std::string& fileName);

	void Export(const std::string& fileName);

	int GetNumberOfPoints() { return m_NumberOfPoints; }

	double ArchLengthInverse(double distance);

	void CurveAt(double u, Vector& point);

	void TangentAt(double u, Vector& tangent);

private:
	int m_NumberOfPoints;
	VectorObj m_ControlPoints[MAX_CONTROLPOINTS];
	VectorObj m_Tangents[MAX_CONTROLPOINTS];
	double m_ArchLength [NUMBER_INDICES];
	double m_u [NUMBER_INDICES];

	void UpdateTables();
	void Curve(double u, VectorObj* point);
};