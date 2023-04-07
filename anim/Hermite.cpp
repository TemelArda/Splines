#include "Hermite.h"
#include <fstream>
#include <string>
#include <sstream> 
#include <vector>

Hermite::Hermite() :
	BaseSystem("hermite"), m_NumberOfPoints(0)
{
	this->UpdateTables();
}

Hermite::Hermite(const std::string& name) :
	BaseSystem(name), m_NumberOfPoints(0)
{
	this->UpdateTables();
}


void Hermite::getState(double* p)
{
	int stateCount = m_NumberOfPoints * 6;
	double *flattenedState = new double[stateCount]();
	if (!flattenedState) {
		animTcl::OutputMessage("Couldn't allocate memory");
		return;
	}
	size_t j = 0;
	for (size_t i = 0; i < m_NumberOfPoints; j++)
	{
		flattenedState[i++] = m_ControlPoints[j][0];
		flattenedState[i++] = m_ControlPoints[j][1];
		flattenedState[i++] = m_ControlPoints[j][2];

		flattenedState[i++] = m_ControlPoints[j][0];
		flattenedState[i++] = m_ControlPoints[j][1];
		flattenedState[i++] = m_ControlPoints[j][2];
	}
	VecCopy(p, flattenedState);
	delete [] flattenedState;
	flattenedState = 0;
	
}

void Hermite::setState(double* p)
{
	animTcl::OutputMessage("Set state not implemented....");
}

void Hermite::reset(double time)
{
	for (size_t i = 0; i < m_NumberOfPoints; i++) {
		
		m_Tangents[i] = {0, 0, 0};
		m_ControlPoints[i] = {0, 0, 0};
	}
	m_NumberOfPoints = 0;
}

void Hermite::display(GLenum mode)
{
	//Draw Curve
	glColor3f(1.0, 1.0, 1.0);
	VectorObj* p = new VectorObj();
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i <= 50; i++) {
		this->Curve((float)i / 50.0, p);
		glVertex3d(p->x(), p->y(), p->z());
	}
	glEnd();
	delete p;
	p = 0;
	//Draw Control points
	glPointSize(5.0);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POINTS);
	for (size_t i = 0; i < m_NumberOfPoints; i++)
		glVertex3d(m_ControlPoints[i].x(), m_ControlPoints[i].y(), m_ControlPoints[i].z());
	glEnd();
	glFlush();
	
	
}

void Hermite::Curve(double u, VectorObj *point) {
	if (u < 0.00 || u > 1.00)
		return;
	


	double t = u * (m_NumberOfPoints - 1);

	if (t <= 0.00000009) {
		*point = m_ControlPoints[0];
		return;
	}
	if ((m_NumberOfPoints - 1)- t <= 0.00000009) {
		*point = m_ControlPoints[m_NumberOfPoints - 1];
		return;
	}
	double tdiffrence = t - floor(t);
	double usquare = pow(tdiffrence, 2);
	long double ucube = usquare * tdiffrence;


	VectorObj point1 = m_ControlPoints[(int)floor(t)] * ((2 * ucube) - (3 * usquare) + 1);
	VectorObj point2 = m_ControlPoints[(int)ceil(t)] * ((-2 * ucube) + (3 * usquare));
	VectorObj tangent1 = m_Tangents[(int)floor(t)] * ((ucube) - (2 * usquare) + tdiffrence);
	VectorObj tangent2 = m_Tangents[(int)ceil(t)] * (ucube - usquare);

	*point = point1 + point2 + tangent1 + tangent2;
	

}

void Hermite::UpdateTables() {
	if (m_NumberOfPoints < 2)
		return;
	VectorObj *p1 = new VectorObj();
	VectorObj *p2 = new VectorObj();
	if (!p1 || !p2)
		return;
	m_ArchLength[0] = 0;
	for (size_t i = 0; i < NUMBER_INDICES; i++) {
		m_u[i] = (float)i / (NUMBER_INDICES - 1);
		if (i > 0) {
			//Get points at u value
			this->Curve(m_u[i], p1);
			this->Curve(m_u[i - 1], p2);
			//Get distance of the points
			double xsquare = pow(p1->x() - p2->x(), 2);
			double ysquare = pow(p1->y() - p2->y(), 2);
			double zsquare = pow(p1->z() - p2->z(), 2);
			double dist = xsquare + ysquare + zsquare;
			dist = sqrt(dist);
			//Add up
			m_ArchLength[i] = dist + m_ArchLength[i - 1];
		}
	}
	delete p1;
	delete p2;
	p1 = 0;
	p2 = 0;
}

int Hermite::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1) 
		return TCL_ERROR;
	if (strcmp(argv[0], "cr") == 0) {
		if (argc == 1) {
			this->CRinitilization();
			return TCL_OK;
		}
		else {
			animTcl::OutputMessage("Usage:  cr");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "set") == 0 && strcmp(argv[1], "tangent") == 0) {
		if (argc == 6)
		{
			Vector tangent;
			int index = atoi(argv[2]);
			tangent[0] = atof(argv[3]);
			tangent[1] = atof(argv[4]);
			tangent[2] = atof(argv[5]);

			this->SetTangent(index, tangent);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage:  set tangent <index> <x y z>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "set") == 0 && strcmp(argv[1], "point") == 0) {
		if (argc == 6)
		{
			Vector point;
			int index = atoi(argv[2]);
			point[0] = atof(argv[3]);
			point[1] = atof(argv[4]);
			point[2] = atof(argv[5]);

			this->SetPoint(index, point);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage:  set tangent <index> <x y z>");
			return TCL_ERROR;
		}

	}
	else if (strcmp(argv[0], "add") == 0 && strcmp(argv[1], "point") == 0) {
		if (argc == 8)
		{
			animTcl::OutputMessage("Input: %f, %f, %f, %f, %f, %f", atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]));

			Vector point;
			point[0] = atof(argv[2]);
			point[1] = atof(argv[3]);
			point[2] = atof(argv[4]);

			Vector tangent;
			tangent[0] = atof(argv[5]);
			tangent[1] = atof(argv[6]);
			tangent[2] = atof(argv[7]);
			this->AddPoint(point, tangent);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage:   <name> add point <x y z sx sy sz>");
			return TCL_ERROR;
		}

	}
	else if (strcmp(argv[0], "getArcLength") == 0) {
		if (argc == 2) {
			double l = this->GetArcLength(atof(argv[1]));
			animTcl::OutputMessage("Length: %f", l);
			return TCL_OK;
		}
		else {
			animTcl::OutputMessage("Usage:  getArcLength <t>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "load") == 0) {
		if (argc == 2)
		{
			this->Load(argv[1]);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: load <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "export") == 0) {
		if (argc == 2)
		{
			this->Export(argv[1]);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: export <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "numberPoints") == 0) {
		if (argc == 1) {
			animTcl::OutputMessage("Number of Points: %d", m_NumberOfPoints);
			return TCL_OK;
		}
		else {
			animTcl::OutputMessage("Usage:  numberPoints");
			return TCL_ERROR;
		}
	}
	return TCL_OK;
}

void Hermite::CRinitilization()
{
	if (m_NumberOfPoints < 3) return;
	
	for (size_t i = 0; i < m_NumberOfPoints; i++) {
		if (i == 0) {
			VectorObj dis1 = m_ControlPoints[1] - m_ControlPoints[0];
			dis1 = dis1 * 2;

			VectorObj dis2 = m_ControlPoints[2] - m_ControlPoints[0];
			dis2 = dis2 * .5;

			m_Tangents[0] = dis1 - dis2;
		} 
		else if (i == m_NumberOfPoints - 1) {
			VectorObj dis1 = m_ControlPoints[m_NumberOfPoints - 1] - m_ControlPoints[m_NumberOfPoints - 2];
			dis1 = dis1 * 2;

			VectorObj dis2 = m_ControlPoints[m_NumberOfPoints - 1] - m_ControlPoints[m_NumberOfPoints - 3];
			dis2 = dis2 * .5;

			m_Tangents[m_NumberOfPoints - 1] = dis1 - dis2;
		}
		else {
			VectorObj newTangent = m_ControlPoints[i + 1] - m_ControlPoints[i - 1];
			m_Tangents[i] = newTangent * .5;
		}
	}
}

void Hermite::SetTangent(int index, const Vector& tangent)
{
	if (m_NumberOfPoints - 1 < index) {
		animTcl::OutputMessage("Index doesn't exsits...");
		return;
	}

	m_Tangents[index] = tangent;
	this->UpdateTables();
}

void Hermite::SetPoint(int index, const Vector& point)
{
	if (m_NumberOfPoints - 1 < index) {
		animTcl::OutputMessage("Index doesn't exsits...");
		return;
	}

	m_ControlPoints[index] = point;
	this->UpdateTables();
}

void Hermite::AddPoint(const Vector& point, const Vector& tangent)
{
	if (m_NumberOfPoints >= MAX_CONTROLPOINTS) {
		animTcl::OutputMessage("Number of Control points can't exceed 40...");
		return;
	}
	
	
	m_ControlPoints[m_NumberOfPoints] = point;
	m_Tangents[m_NumberOfPoints] = tangent;
	m_NumberOfPoints++;
	this->UpdateTables();
}

double Hermite::GetArcLength(double t)
{
	if (t > 1.0 || t < 0.0)
		return 0.0;
	if (t == 1)
		return m_ArchLength[(NUMBER_INDICES - 1)];
	if (t == 0)
		return 0;
	double distance = m_u[1] - m_u[0];
	double index = t / distance;
	double scalar = (t - m_u[(int)index]) / distance;
	double result = m_ArchLength[(int)index] + scalar * (m_ArchLength[(int)index + 1] - m_ArchLength[(int)index]);

	return result;
}

void Hermite::Load(const std::string& fileName)
{
	std::ifstream inputFile;
	inputFile.open(fileName);
	
	if (inputFile.is_open()) {
		animTcl::OutputMessage("File opened reading...");
		std::string line;
		m_NumberOfPoints = 0;
		while (getline(inputFile, line)) {
			std::stringstream sstream(line);
			std::string word;
			std::vector<std::string> words;
			
			while (sstream >> word) {
				words.push_back(word);
				
			}
			if (words.size() == 2) {
				
				m_name = words[0];
			}
			else if (words.size() == 6) {
				Vector point;
				Vector tangent;

				point[0] = stod(words[0]);
				point[1] = stod(words[1]);
				point[2] = stod(words[2]);


				tangent[0] = stod(words[3]);
				tangent[1] = stod(words[4]);
				tangent[2] = stod(words[5]);
				
				this->AddPoint(point, tangent);
			}

			word.clear();
		}
	}
	else {
		animTcl::OutputMessage("Error openning up the file...");
	}
		inputFile.close();
}

void Hermite::Export(const std::string& fileName)
{
	std::ofstream outputFile;
	outputFile.open(fileName);
	if (outputFile.is_open()) {
		outputFile << m_name << " " << m_NumberOfPoints << std::endl;
	
		for (size_t i = 0; i < m_NumberOfPoints; i++) {
			outputFile << m_ControlPoints[i][0] << " " << m_ControlPoints[i][1] << " " << m_ControlPoints[i][2] << " ";
			outputFile << m_Tangents[i][0] << " " << m_Tangents[i][1] << " " << m_Tangents[i][2]<< " " << std::endl;
		}
	}
	outputFile.close();

}

double Hermite::ArchLengthInverse(double distance)
{
	if (distance > m_ArchLength[NUMBER_INDICES - 1]) {
		animTcl::OutputMessage("Error point at length %f exceeds the length of the curve, %f", distance, m_ArchLength[NUMBER_INDICES - 1]);
		return -1;
	}
	double l = 0;
	double r = 1;
	double m = (l + r) / 2.0;
	while (true) {
		//Get the arch length at mid point
		double distFromM = distance - GetArcLength(m);
		double distFromL = distance - GetArcLength(l);
		//Chceck where we lie..
		if (abs(distFromM) < 0.000009) {
			
			return m;
		}
		else if (distFromM * distFromL <= 0) {
			r = m;
			m = (l + r) / 2;
		}
		else {
			l = m;
			m = (l + r) / 2;
		}
	}
}

void Hermite::CurveAt(double u, Vector& point)
{
	VectorObj *v = new VectorObj(point[0], point[1], point[2]);
	Curve(u, v);
	point[0] = v->x();
	point[1] = v->y();
	point[2] = v->z();
	delete v;
	v = 0;

}

void Hermite::TangentAt(double u, Vector& tangent)
{
	if (u < 0.00 || u > 1.00)
		return;



	double t = u * (m_NumberOfPoints - 1);

	if (t <= 0.0000009) {
		tangent[0] = m_Tangents[0].x();
		tangent[1] = m_Tangents[0].y();
		tangent[2] = m_Tangents[0].z();
		return;
	}
	if ((m_NumberOfPoints - 1) - t <= 0.0000009) {
		tangent[0] = m_Tangents[m_NumberOfPoints - 1].x();
		tangent[1] = m_Tangents[m_NumberOfPoints - 1].y();
		tangent[2] = m_Tangents[m_NumberOfPoints - 1].z();
		return;
	}
	double tdiffrence = t - floor(t);
	auto usquare = pow(tdiffrence, 2);


	VectorObj point1 = m_ControlPoints[(int)floor(t)] * ((6 * usquare) - (6 * u));
	VectorObj point2 = m_ControlPoints[(int)ceil(t)] * ((-2 * usquare) + (6 * u));
	VectorObj tangent1 = m_Tangents[(int)floor(t)] * ((3 * usquare)-(4 * u) + 1);
	VectorObj tangent2 = m_Tangents[(int)ceil(t)] * (3 * usquare - 2 * u);

	VectorObj v = point1 + point2 + tangent1 + tangent2;
	v.normalize();
	tangent[0] = v.x();
	tangent[1] = v.y();
	tangent[2] = v.z();
}

