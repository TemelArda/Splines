#include "Missile.h"
#include <algorithm>




Missile::Missile(const std::string& name) :
	BaseSystem(name)
{
	m_sx = m_sy = m_sz = 1;
	m_pos[0] = m_initialPosition[0] = 0;
	m_pos[1] = m_initialPosition[1] = 1;
	m_pos[2] = m_initialPosition[2] = 0;
	m_rot = new Quaternion();
	m_initialRot = new Quaternion();
	m_front[0] = 1;
	m_front[1] = m_front[2] = 0;
	m_vel = 1;
	m_target[0] = m_target[1] = m_target[2] = 0;
	m_path = new Hermite("MissilePath");
	m_path->AddPoint(m_pos, {0,0,0});
	m_path->AddPoint(m_target, { 0, 0, 0 });
	m_path->CRinitilization();
}

void Missile::getState(double* p)
{
	VecCopy(p, m_pos);
	
}

void Missile::setState(double* p)
{
	VecCopy(m_pos, p);
}

void Missile::reset(double time)
{
	m_rot->copy(m_initialRot);
	this->m_path->SetPoint(0, m_initialPosition);
	this->m_path->CurveAt(0, m_pos);
}

void Missile::SetTarget(const double* targetPos) {
	m_target[0] = targetPos[0];
	m_target[1] = targetPos[1];
	m_target[2] = targetPos[2];
}

void Missile::SetTargetTangent(const double* tangent)
{
	m_targetTangent[0] = tangent[0];
	m_targetTangent[1] = tangent[1];
	m_targetTangent[2] = tangent[2];
}

int Missile::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
		return TCL_ERROR;
	if (strcmp(argv[0], "read") == 0)
	{
		if (argc == 2)
		{
			m_model.ReadOBJ(argv[1]);
			glmFacetNormals(&m_model);
			glmVertexNormals(&m_model, 90);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: read <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "scale") == 0)
	{
		if (argc == 4)
		{
			m_sx = (float)atof(argv[1]);
			m_sy = (float)atof(argv[2]);
			m_sz = (float)atof(argv[3]);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "pos") == 0)
	{
		if (argc == 4)
		{
			m_pos[0] = atof(argv[1]);
			m_pos[1] = atof(argv[2]);
			m_pos[2] = atof(argv[3]);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "state") == 0)
	{
		if (argc == 8)
		{
			animTcl::OutputMessage("setting up missile..");
			m_initialPosition[0] = atof(argv[1]);
			m_initialPosition[1] = atof(argv[2]);
			m_initialPosition[2] = atof(argv[3]);

			m_initialRot->set(atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]));
			this->reset(0.0);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: state <x> <y> <z> <v1> <v2> <v3> <e>");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "speed") == 0)
	{
		if (argc == 2)
		{
			this->SetVelocity(atof(argv[1]));
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: spped <s>");
			return TCL_ERROR;

		}
	}
	return TCL_OK;
}

void Missile::Update(double time) {
	//Update the Position based on Velocity...
	
	
	if (time < END_TIME) {
		//UPDATE POSITION
		double distanceNormalized = Ease1(time / END_TIME);
		double distanceAlongTheCurve = m_path->GetArcLength(1.0) * distanceNormalized;
		double u = m_path->ArchLengthInverse(distanceAlongTheCurve);

		m_path->CurveAt(u, m_pos);
		m_path->CurveAt(u + 0.05, m_tangent);
		if (u + 0.05 > 1.0)
			m_path->CurveAt(1, m_tangent);

		VecSubtract(m_tangent, m_tangent, m_pos);
		VecScale(m_tangent, 7);
		VecScale(m_targetTangent, 7);


		m_path->SetPoint(0, m_pos);
		m_path->SetPoint(1, m_target);
		m_path->SetTangent(0, m_tangent);
		m_path->SetTangent(1, m_targetTangent);
		Rotate(u);
	}
	else if (time < (END_TIME + 10)) {
		double distanceNormalized = Ease2((time - END_TIME) / 10);
		double distanceAlongTheCurve = m_path->GetArcLength(1.0) * distanceNormalized;
		double u = m_path->ArchLengthInverse(distanceAlongTheCurve);
		m_path->CurveAt(u, m_pos);
		Rotate(u);
	}
}

void Missile::Rotate(double t)
{
	double angle = 0;

	Vector newAxis;
	m_path->TangentAt(t, m_tangent);
	
	VecCrossProd(newAxis, m_front, m_tangent);
	
	double adotb = VecDotProd(m_front, m_tangent);
	double lenghtA = sqrt(VecDotProd(m_front, m_front));
	double lengthB = sqrt(VecDotProd(m_tangent, m_tangent));
	
	angle = acos(adotb / (lengthB * lenghtA)) ;
	VecNormalize(newAxis);

	m_rot->setAxisAngle(newAxis, angle);
	m_rot->normalize();
	//m_rot->rotatePoint(m_front);
}


void Missile::SetVelocity(double vel)
{
	m_vel = vel;
	animTcl::OutputMessage("velocity is set to %f", m_vel);
}



void Missile::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(m_pos[0], m_pos[1], m_pos[2]);
	Vector axis;
	double angle = 0;
	m_rot->getAxisAngle(axis, &angle);
	VecNormalize(axis);
	angle = angle * 180.0 / 3.141592;


	glRotated(angle, axis[0], axis[1], axis[2]);
	glScalef(m_sx, m_sy, m_sz);
	
	if (m_model.numvertices > 0)
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	else
		glutSolidSphere(1.0, 20, 20);

	glPopMatrix();
	glPopAttrib();
	
	m_path->display();
}