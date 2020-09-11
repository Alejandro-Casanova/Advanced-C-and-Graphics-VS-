#include "pch.h"

#include "Camera2D.h"

namespace Bengine{

Camera2D::Camera2D() :
    _screenWidth(500),
    _screenHeight(500),
    _needsMatrixUpdate(true),
    _scale(1.0f),
    _position(0.0f, 0.0f),
    _cameraMatrix(1.0f),
    _orthoMatrix(1.0f)
{
}

Camera2D::~Camera2D()
{
    //dtor
}

void Camera2D::init(int screenWidth, int screenHeight){

    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);

}

void Camera2D::update(){

    if(_needsMatrixUpdate){

        //Camera translation
        glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
        _cameraMatrix = glm::translate(_orthoMatrix, translate);

        //Camera scale
        glm::vec3 scale(_scale, _scale, 0.0f);
        _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
        _needsMatrixUpdate = false;
    }

}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords){
    //Invert Y direction
    screenCoords.y = _screenHeight - screenCoords.y;
    //Center coords
    screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
    //Scale coords
    screenCoords /= _scale;
    //Translate with camera position
    screenCoords += _position;

    return screenCoords;
}

bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions){

    glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);

    const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
    const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

    glm::vec2 centerPlayerPos = position + dimensions / 2.0f;
    glm::vec2 distVec = centerPlayerPos - _position; //Camera position is centered

    float xDepth = MIN_DISTANCE_X - abs(distVec.x);
    float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

    //Check for collition
    if(xDepth > 0.0f && yDepth > 0.0f){
        return true;
    }

    return false;

}

}
