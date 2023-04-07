#include "Terrain.h"




Terrain::Terrain(const Vector& pos, const std::string& name, float sx, float sy, float sz):
	BaseSystem(name), m_sx(m_sx), m_sy(sy), m_sz(sz)
{
	VecCopy(m_pos, pos);
}

Terrain::Terrain(const std::string& name, float sx, float sy, float sz) :
	BaseSystem(name), m_sx(sx), m_sy(sy), m_sz(sz)
{
	m_pos[0] = m_pos[1] = m_pos[2] = 0;
}

void Terrain::getState(double* p)
{
	VecCopy(p, m_pos);

	p[3] = (double)m_sx;
	p[4] = (double)m_sy;
	p[5] = (double)m_sz;
}

void Terrain::setState(double* p)
{
	VecCopy(m_pos, p);
	m_sx = p[3];
	m_sy = p[4];
	m_sz = p[5];
}

void Terrain::reset(double time)
{
	return;
}

void Terrain::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslated(m_pos[0], m_pos[1], m_pos[2]);
	glScalef(m_sx, m_sy, m_sz);
	
	//glutSolidCube(1);

	glPopMatrix();
	glPopAttrib();
}

int Terrain::command(int argc, myCONST_SPEC char** argv)
{
	if (strcmp(argv[0], "pos") == 0)
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
	else if (strcmp(argv[0], "scale") == 0)
	{
		if (argc == 4)
		{

			m_sx = atof(argv[1]);
			m_sy = atof(argv[2]);
			m_sz = atof(argv[3]);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <x> <y> <z> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "r") == 0)
	{
		if (argc == 1)
		{
			m_pos[0] = m_pos[1] = m_pos[2] = 0;

			m_sx = 1.0;
			m_sz = 1.0;
			m_sy = 1.0;
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: r ");
			return TCL_ERROR;

		}
	}
	return TCL_OK;;
}
