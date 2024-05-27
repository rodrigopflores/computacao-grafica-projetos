#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector> 

using namespace std;

class Curve {
public:
	Curve() {}
	inline void setControlPoints(vector <glm::vec3> controlPoints) { this->controlPoints = controlPoints; }
	void generateCurve(int pointsPerSegment);
	int getNbCurvePoints() { return curvePoints.size(); }
	glm::vec3 getPointOnCurve(int i) { return curvePoints[i]; }
protected:
	vector <glm::vec3> controlPoints;
	vector <glm::vec3> curvePoints;
	glm::mat4 M; //Matriz de base
};

