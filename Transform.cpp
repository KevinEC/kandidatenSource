#include "Transform.h"


Transform::Transform()
{
}

Transform::~Transform()
{
}

float Transform::scale(glm::vec2 v1, glm::vec2 v2)
{
	float res = sqrt(v1.x*v1.x + v1.y*v1.y) - sqrt(v2.x*v2.x + v2.y*v2.y);
	if (res != 0) return res;
	else return 1;
	//glm::length(v1) - glm::length(v2);
}

float Transform::rotateCard(glm::vec2 v1, glm::vec2 v2)
{
    v1 = glm::normalize(v1);
    v2 = glm::normalize(v2);
    /*
    glm::vec2 xAxis(1.0,0.0);
    float angle1 = glm::orientedAngle(v1,xAxis); // radians
    float angle2 = glm::orientedAngle(v2, xAxis); // radians
    float angle = angle1 / angle2;*/

    float angle = glm::orientedAngle(v1, v2);
    //float angle = glm::angle(v1, v2);
    
    
    //if (angle > M_PI && angle < -M_PI) return 0;
    return angle;
}

glm::mat3 Transform::rotate(glm::vec2 v1, glm::vec2 v2)
{
    GLfloat Mtemp[9];
    glm::mat3 res;
    v1 = glm::normalize(v1);
    v2 = glm::normalize(v2);

    //glm::vec2 xAxis(1.0,0.0);
    //float angle1 = glm::orientedAngle(v1,xAxis); // radians
    //float angle2 = glm::orientedAngle(v2, xAxis); // radians
    //float angle = angle1 / angle2;

    float angle = glm::orientedAngle(v1, v2);

    CI_LOG_I("angle: " << glm::degrees(angle));
   
    glm::mat3 trans = glm::mat3(1.0f);
    //trans = glm::rotate(trans, angle);

    /* rot z
    Mtemp[0] = cos(angle);  Mtemp[1] = -sin(angle); Mtemp[2] = 0.0f; 
    Mtemp[3] = sin(angle);  Mtemp[4] = cos(angle);  Mtemp[5] = 0.0f; 
    Mtemp[6] = 0.0f;        Mtemp[7] = 0.0f;        Mtemp[8] = 1.0f; */

    /* rot y 
    Mtemp[0] = cos(angle);  Mtemp[1] = 0.0f;    Mtemp[2] = sin(angle);
    Mtemp[3] = 0.0f;        Mtemp[4] = 1.0f;      Mtemp[5] = 0.0f;
    Mtemp[6] = -sin(angle);   Mtemp[7] = 0.0f;   Mtemp[8] = cos(angle);
    */
    //res = glm::make_mat3(Mtemp);
    //res = glm::transpose(res);

     return glm::rotate(trans, angle);
}

glm::mat3 Transform::translateCard(float oldX, float oldY, float x, float y)
{
    glm::mat3 trans = glm::mat3(1.0f);

	float deltaX = x - oldX;
	float deltaY = y - oldY;
    
    return glm::translate(trans, {deltaX,deltaY});
}

glm::mat3 Transform::scaling(float size)
{
    //glm::mat3 trans = glm::mat3(1.0f);
    
    //return glm::scale(trans, {size, size});
	return glm::mat3(size);
}


//Calculate the new coordinates for card taking into consideration the relative position to the top left corner
float * Transform::translate(const float x, const float y, const float mx, const float my, const bool isDragged)
{
	if(!isDragged) {
		this->deltaX = mx - x;
		this->deltaY = my - y;
		//CI_LOG_I("updated delta var");
	}

	float newx = mx - this->deltaX;
	float newy = my - this->deltaY;

	return new float[2]{ newx, newy };

}