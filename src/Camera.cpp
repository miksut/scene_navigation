#include "Camera.h"
#include <iostream>

// to get M_PI from math libary enable defines
#define _USE_MATH_DEFINES
#include <math.h>

namespace cgCourse
{
	Camera::Camera()
	{
		
	}
    void Camera::create(glm::uvec2 _viewPortSize,glm::uvec2 _windowSize, glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up, float _fieldOfView)
    {
        fieldOfView = _fieldOfView;
        create(_viewPortSize,_windowSize,_pos,_target,_up);
    }
	void Camera::create(glm::uvec2 _viewPortSize,glm::uvec2 _windowSize, glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up)
	{
        windowSize = _windowSize;
        this->setViewport(_viewPortSize);
        this->lookAt(_pos, _target, _up);
	}
    
    void Camera::lookAt(glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up){
        this->position = _pos;
        this->target = _target;
        glm::vec3 v = target - position;
        glm::vec3 right = glm::cross(v,_up);
        this->up = glm::normalize(glm::cross(right, v));
        
        computeViewMatrix();
        computeProjectionMatrix();
        computeViewProjectionMatrix();
    }
    
	void Camera::setViewport(glm::uvec2 _viewPortSize)
	{
        viewPortSize = _viewPortSize;
		glViewport(0, 0, int(viewPortSize.x), int(viewPortSize.y));
	}

	void Camera::computeViewProjectionMatrix()
	{
		this->viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void Camera::computeViewMatrix()
	{

		this->viewMatrix = glm::lookAt(position,target,up);
	}

	void Camera::computeProjectionMatrix()
	{

		float aspect = float(viewPortSize.x) / float(viewPortSize.y);

		this->projectionMatrix = glm::perspective(fieldOfView, aspect,
			nearPlane, farPlane);
	}
    void Camera::setPosition(glm::vec3 _pos){
        position = _pos;
        computeViewMatrix();
        computeViewProjectionMatrix();
    }

	glm::vec3 Camera::getPosition() const
	{
		return this->position;
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return this->viewMatrix;
	}

	glm::mat4 Camera::getProjectionMatrix() const
	{
		return this->projectionMatrix;
	}

	glm::mat4 Camera::getViewProjectionMatrix() const
	{
		return this->viewProjectionMatrix;
	}

    void Camera::mouseDrag(glm::dvec2 _p1, glm::dvec2 _p2, glm::dvec2 _base, int _activeButton){
        std::cout<<"active button "<<_activeButton<<std::endl;
        std::cout<<"type "<<type<<std::endl;
        //TODO: transform the camera parameters (position, target, up) based on the active button, camera type and input parameters
        // active button : 0-left button, 1-right button
        // type : 0-arcball, 1-flyover
        // TIP: use functions for code readability

		// Factor for adjusting the speed the camera moves (-> smoother user experience)
		float delayFactor = 0.05;

		// Defining different cases for the GUI settings
		if(type == 0 ){
			// Case: Arcball camera and left button pressed (rotation)
			if (_activeButton == 0) {
				glm::vec2 midpoint = { 800.0, 500.0 }; // midpoint of relevant viewport
				float radius = 500;
				glm::vec3 base, p2, rotationAxis, targetNew, upNew;
				float rotationAngle; // rotation angle about the arcball's rotation axis
				glm::mat4 rotMatrixX, rotMatrixY, rotMatrixZ, rotMatrixXInv, rotMatrixYInv, rotMatrixM;
				float R1, s1, R2, s2, projectionYZ;

				// Computing 3D sphere-points that correspond to mouse cursor positions
				base.x = (_p1.x - midpoint.x) / radius;
				base.y = (_p1.y - midpoint.y) / radius;

				// make sure that we handle cases when screen points P(x,y) are outside the virtual arcball
				if ((glm::pow(base.x, 2) + glm::pow(base.y, 2)) > 1.0f) {
					R1 = glm::sqrt((glm::pow(_p1.x - midpoint.x, 2) / glm::pow(radius, 2)) + (glm::pow(_p1.y - midpoint.y, 2) / glm::pow(radius, 2)));
					s1 = (1 / R1);
					base.x = s1 * ((_p1.x - midpoint.x) / radius);
					base.y = s1 * ((_p1.y - midpoint.y) / radius);
					base.z = 0.0;
				}
				else {
					base.z = (glm::sqrt((1 - glm::pow(base.x, 2) - glm::pow(base.y, 2))));
				}

				p2.x = (_p2.x - midpoint.x) / radius;
				p2.y = (_p2.y - midpoint.y) / radius;

				// make sure that we handle cases when screen points P(x,y) are outside the virtual arcball
				if ((glm::pow(p2.x, 2) + glm::pow(p2.y, 2)) > 1.0f) {
					R2 = glm::sqrt((glm::pow(_p2.x - midpoint.x, 2) / glm::pow(radius, 2)) + (glm::pow(_p2.y - midpoint.y, 2) / glm::pow(radius, 2)));
					s2 = (1 / R2);
					p2.x = s2 * ((_p2.x - midpoint.x) / radius);
					p2.y = s2 * ((_p2.y - midpoint.y) / radius);
					p2.z = 0.0;
				}
				else {
					p2.z = (glm::sqrt((1 - glm::pow(p2.x, 2) - glm::pow(p2.y, 2))));
				};

				// Computing the rotation axis and rotation angle according to the arcball-model
				rotationAxis = glm::normalize(glm::cross(base, p2));
				rotationAngle = glm::acos(glm::dot(base, p2));

				// Computing the rotation matrix M from axis and angle. For this purpose, the following
				// steps are performed: 1) Make sure that the rotation axis passes through the origin
				// of the camera space's origin, 2) rotate space about x-axis s.t. rotation axis lies in
				// xz-plane, 3) rotate space about y-axis s.t. rotation axis lies along z-axis, 4) perform
				// rotation by theta about z-axis, 5) apply inverse steps of points 3) and 2).
				
				// Step 2) and 5)
				projectionYZ = glm::sqrt(glm::pow(rotationAxis.y, 2) + glm::pow(rotationAxis.z, 2));
				if (projectionYZ == 0) {
					rotMatrixX = glm::mat4(1.0); // case if rotation-axis lies already on x-axis
				}
				else {
					rotMatrixX[1][1] = (rotationAxis.z / projectionYZ);
					rotMatrixX[2][1] = -(rotationAxis.y / projectionYZ);
					rotMatrixX[1][2] = (rotationAxis.y / projectionYZ);
					rotMatrixX[2][2] = (rotationAxis.z / projectionYZ);

					rotMatrixXInv[1][1] = (rotationAxis.z / projectionYZ);
					rotMatrixXInv[2][1] = (rotationAxis.y / projectionYZ);
					rotMatrixXInv[1][2] = -(rotationAxis.y / projectionYZ);
					rotMatrixXInv[2][2] = (rotationAxis.z / projectionYZ);
				}

				// Step 3) and 5)
				rotMatrixY[0][0] = (projectionYZ);
				rotMatrixY[2][0] = -(rotationAxis.x);
				rotMatrixY[0][2] = (rotationAxis.x);
				rotMatrixY[2][2] = (projectionYZ);

				rotMatrixYInv[0][0] = (projectionYZ);
				rotMatrixYInv[2][0] = (rotationAxis.x);
				rotMatrixYInv[0][2] = -(rotationAxis.x);
				rotMatrixYInv[2][2] = (projectionYZ);

				// Step 4)
				rotMatrixZ[0][0] = glm::cos(rotationAngle);
				rotMatrixZ[1][0] = -(glm::sin(rotationAngle));
				rotMatrixZ[0][1] = glm::sin(rotationAngle);
				rotMatrixZ[1][1] = glm::cos(rotationAngle);

				// Composite rotation matrix M
				rotMatrixM = rotMatrixXInv * rotMatrixYInv * rotMatrixZ * rotMatrixY * rotMatrixX;

				// Multiplying modelview matrix by M
				viewMatrix = rotMatrixM * viewMatrix;
				computeProjectionMatrix();
				computeViewProjectionMatrix();

				// Actualization of target and up vectors
				targetNew = glm::inverse(viewMatrix) * glm::vec4(0.0, 0.0, -1.0, 1.0);
				upNew = glm::inverse(viewMatrix) * glm::vec4(1.0, 0.0, 0.0, 1.0);
				target = targetNew;
				up = -(glm::cross((target-position), (upNew-position)));
			}

			// Case: Arcball camera and right button pressed (Note: Functionality follows orbit-control camera from potree.org)
			else {
				// Movement along the x-axis (calculation in camera space followed by a back-transformation to world space)
				glm::vec4 deltaX;
				deltaX.x = (_p2.x - _p1.x) * delayFactor;
				glm::vec3 newPositionX = glm::inverse(getViewMatrix())*((getViewMatrix()*glm::vec4(position, 1.0)) + deltaX);
				glm::vec3 newTargetX = glm::inverse(getViewMatrix())*((getViewMatrix()*glm::vec4(target, 1.0)) + deltaX);
				lookAt(newPositionX, newTargetX, up);

				// Movement along the y-axis
				glm::vec3 deltaY;
				deltaY.y = (_p2.y - _p1.y) * delayFactor;
				glm::vec3 newPositionY = position + deltaY;
				glm::vec3 newTargetY = target + deltaY;
				lookAt(newPositionY, newTargetY, up);
			}
		}
		else {
			// Case: Fly-over camera and left button pressed
			if(_activeButton == 0) {
				// Movement along the x-axis (calculation in camera space followed by a back-transformation to world space)
				glm::vec4 deltaX;
				deltaX.x = (_p2.x - _p1.x) * delayFactor;
				glm::vec3 newPositionX = glm::inverse(getViewMatrix())*((getViewMatrix()*glm::vec4(position, 1.0)) + deltaX);
				glm::vec3 newTargetX = glm::inverse(getViewMatrix())*((getViewMatrix()*glm::vec4(target, 1.0)) + deltaX);
				lookAt(newPositionX, newTargetX, up);

				// Movement along the y-axis
				glm::vec3 deltaY;
				deltaY.y = (_p2.y - _p1.y) * delayFactor;
				glm::vec3 newPositionY = position + deltaY;
				glm::vec3 newTargetY = target + deltaY;
				lookAt(newPositionY, newTargetY, up);
			}

			// Case: Fly-over camera and right button pressed
			else {
				// Implementation of the rotations around the x-axis (= pitch) and y-axis (= yaw)
				float deltaYaw = (_p2.x - _p1.x) * delayFactor;
				float deltaPitch = (_p2.y - _p1.y) * delayFactor;
				this->yaw += deltaYaw; // Note: the yaw- and pitch- variables have been additionally defined in the header-file
				this->pitch += deltaPitch;

				// Restriction for rotation around x-axis
				if (pitch > 89.0f) { pitch = 89.0f; }
				if (pitch < -89.0f) { pitch = -89.0f; }

				glm::vec3 rotation;
				rotation.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
				rotation.y = sin(glm::radians(pitch));
				rotation.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
				rotation = glm::normalize(rotation);

				target = position + rotation;
				lookAt(position, target, glm::vec3(0.0, 1.0, 0.0));
			}
		}
    }
    
    void Camera::mouseScroll(glm::dvec2 _offset){
        std::cout<<"type "<<type<<std::endl;
        std::cout<<"offset "<<_offset.x<<" "<<_offset.y<<std::endl;
        //TODO: transform the camera parameters (position, target, up) based on the camera type and input parameters
        // type : 0-arcball, 1-flyover
        // TIP: use functions for code readability

		// Defining different cases for the GUI settings

		// Case: Arcball camera and Scrolling
		if (type == 0) {
			// Movement along the z-axis (calculation in camera space followed by a back-transformation to world space)
			glm::vec4 deltaZ;
			deltaZ.z += _offset.y;
			glm::vec3 newPositionZ = glm::inverse(getViewMatrix())*((getViewMatrix()*glm::vec4(position, 1.0)) - deltaZ);
			glm::vec3 newTargetZ = glm::inverse(getViewMatrix())*((getViewMatrix()*glm::vec4(target, 1.0)) - deltaZ);
			lookAt(newPositionZ, newTargetZ, up);
		}

		
		// Case: Fly-over camera and Scrolling
		else {
			// Movement along the z-axis (calculation in camera space followed by a back-transformation to world space)
			glm::vec4 deltaZ;
			deltaZ.z += _offset.y;
			glm::vec3 newPositionZ = glm::inverse(getViewMatrix())*((getViewMatrix()*glm::vec4(position, 1.0)) - deltaZ);
			glm::vec3 newTargetZ = glm::inverse(getViewMatrix())*((getViewMatrix()*glm::vec4(target, 1.0)) - deltaZ);
			lookAt(newPositionZ, newTargetZ, up);
		}
    }
    
    int &Camera::getType(){
        return type;
    }
    


}
