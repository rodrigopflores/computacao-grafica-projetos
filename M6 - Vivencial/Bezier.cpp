#include "Bezier.h"

Bezier::Bezier()
{
	M = glm::mat4(-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0
	);
}

void Bezier::generateCurve(int pointsPerSegment)
{
	float step = 1.0 / (float)pointsPerSegment;

	float t = 0;

	int nControlPoints = controlPoints.size();

	for (int i = 0; i < nControlPoints - 3; i += 3)
	{

		for (float t = 0.0; t <= 1.0; t += step)
		{
			glm::vec3 p;

			glm::vec4 T(t * t * t, t * t, t, 1);

			glm::vec3 P0 = controlPoints[i];
			glm::vec3 P1 = controlPoints[i + 1];
			glm::vec3 P2 = controlPoints[i + 2];
			glm::vec3 P3 = controlPoints[i + 3];

			glm::mat4x3 G(P0, P1, P2, P3);

			p = G * M * T;  //---------

			curvePoints.push_back(p);
		}
	}
}
