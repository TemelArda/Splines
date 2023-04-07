#include "Tank.h"

Tank::Tank(const std::string& name) :
	BaseSystem(name)
{
	m_pos[0] = m_pos[1] = m_pos[2] = 0;
	m_tangent[0] = m_tangent[1] = m_tangent[2] = 0;
	m_sx = m_sy = m_sz = 1.0f;
	m_PathLenght = 0;
	m_Path = new Hermite("TankPath");
}

Tank::~Tank()
{
	delete m_Path;
}

void Tank::getState(double* p)
{
	VecCopy(p, m_pos);
	p[3] = m_sx;
	p[4] = m_sy;
	p[5] = m_sz;
}

void Tank::setState(double* p)
{
	VecCopy(m_pos, p);
	m_sx = p[3];
	m_sy = p[4];
	m_sz = p[5];
}	

void Tank::reset(double time)
{
	if(m_Path-> GetNumberOfPoints() > 2)
		m_Path->CurveAt(0, m_pos);
}	


double Tank::getVelocity(double time)
{
	double t = time / END_TIME;
	if (t < .1) {
		return t / .1 ;
	}
	else if(t >= .1 && t <= .9) {
		return 1;
	}
	else if(t > .9 && t < 1) {
		return 1 - ((t - .9) / .1);
	}
	else {
		return 0.0;
	}
}

int Tank::command(int argc, myCONST_SPEC char** argv)
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
	else if (strcmp(argv[0], "load") == 0)
	{
		if (argc == 2)
		{
			this->LoadPath(argv[1]);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: load <spline file> ");
			return TCL_ERROR;

		}
	}

	return TCL_OK;
}


void Tank::Update(double time) {
	//Update the Position....
	if (time <= END_TIME && m_PathLenght > 0) {
		double distanceNormilized = Ease(time/END_TIME);
		distanceNormilized /= Ease(1.0);
		double distanceAlongTheCurve = m_PathLenght * distanceNormilized;
		double u = m_Path->ArchLengthInverse(distanceAlongTheCurve);
		
		m_Path->CurveAt(u, m_pos);
		m_Path->CurveAt(u + 0.025, m_tangent);
		if(u + 0.025 > 1.0)
			m_Path->CurveAt(1, m_tangent);

		VecSubtract(m_tangent, m_tangent, m_pos);
		VecNormalize(m_tangent);

	}
}


//double Tank::distanceNormilized(double t) {
//	if (t >= .1 && t <= .9)
//		return  t;
//	double ease = 0.0;
//	if (t < .1) {
//		ease = (sin(5.0 * PI * t - PI / 2.0)) / 10.0 + .1;
//	}
//	else
//	{
//		ease = (sin(5.0 * PI * t - (9.0 * PI) / 2.0)) / 10.0 + .9;
//	}
//	
//	return  ease > 1 ? 1.0 : ease;
//}
double Tank::Ease(double t) {

	if (t < .1) 
	{
		return t * t / .2;
	}
	else if(t >= .1 && t <= .9)
	{
		return .05 + (t - .1);
	}
	else if(t > .9)
	{
		return .85 + (t - .9) * (1.0 - ((t - .9) / .2));
	}
}


void Tank::LoadPath(const std::string& fileName)
{		
	if (m_Path != NULL) {
		m_Path->Load(fileName);
		m_PathLenght = m_Path->GetArcLength(1);
		this->m_Path->CurveAt(0, m_pos);
		m_Path->TangentAt(0, m_tangent);
		animTcl::OutputMessage("Path loaded, length is: %f", m_PathLenght);
	}
}


void Tank::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(m_pos[0], m_pos[1], m_pos[2]);
	glScalef(m_sx, m_sy, m_sz);

	if (m_model.numvertices > 0)
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
	else
		glutSolidSphere(1.0, 20, 20);

	glPopMatrix();
	glPopAttrib();
	m_Path->display();
}
