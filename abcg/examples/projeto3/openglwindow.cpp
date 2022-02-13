#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "imfilebrowser.h"
#include <math.h>

void OpenGLWindow::handleEvent(SDL_Event& event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
  
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP){      
      m_ballSpeedX = 10.0f;
    }
    if (event.key.keysym.sym == SDLK_DOWN){
       m_ballSpeedX = -10.0f;
    }
    if(event.key.keysym.sym == SDLK_LEFT){
      m_ballSpeedZ = 10.0f;
    }
    if(event.key.keysym.sym == SDLK_RIGHT){
      m_ballSpeedZ = -10.0f;
    }
    if (event.key.keysym.sym == SDLK_w){      
      m_dollySpeed = 20.0f;
    }
    if (event.key.keysym.sym == SDLK_s){
      m_dollySpeed = -20.0f;
    }
    if (event.key.keysym.sym == SDLK_a){
      m_truckSpeed = -5.0f;
    }
    if (event.key.keysym.sym == SDLK_d){
      m_truckSpeed = 5.0f;
    }
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP){      
      m_ballSpeedX = 0.0f;
    }
    if (event.key.keysym.sym == SDLK_DOWN){
      m_ballSpeedX = 0.0f;
    }
    if(event.key.keysym.sym == SDLK_LEFT){
      m_ballSpeedZ = 0.0f;
    }
    if(event.key.keysym.sym == SDLK_RIGHT){
      m_ballSpeedZ = 0.0f;
    }
    if (event.key.keysym.sym == SDLK_w){      
      m_dollySpeed = 0.0f;
    }
    if (event.key.keysym.sym == SDLK_s){
      m_dollySpeed = 0.0f;
    }
    if (event.key.keysym.sym == SDLK_a){
      m_truckSpeed = 0.0f;
    }
    if (event.key.keysym.sym == SDLK_d){
      m_truckSpeed = 0.0f;
    }
  }

  if (event.type == SDL_MOUSEMOTION) {
    m_trackBallModel.mouseMove(mousePosition);
    m_trackBallLight.mouseMove(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_mouseClick = true;
      m_trackBallModel.mousePress(mousePosition);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mousePress(mousePosition);
    }
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    m_mouseClick = false;
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_trackBallModel.mouseRelease(mousePosition);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mouseRelease(mousePosition);
    }
  }
}

void OpenGLWindow::initializeGL() {
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

  auto path{getAssetsPath() + "shaders/texture"};
  m_program = createProgramFromFile(path + ".vert", path + ".frag");

  start();

  // Initial trackball spin
  m_trackBallModel.setAxis(glm::normalize(glm::vec3(1, 1, 1)));
  m_trackBallModel.setVelocity(0.0001f);
  
}

void OpenGLWindow::start(){

  bola.initializeGL(m_program, getAssetsPath(), glm::vec3(2.0f), glm::vec3(0.0f, 0.5f, 0.0f), true);
  bola2.initializeGL(m_program, getAssetsPath(), glm::vec3(5.0f), glm::vec3(8.0f, 2.0f, 0.0f), false);
  bola3.initializeGL(m_program, getAssetsPath(), glm::vec3(5.0f), glm::vec3(-8.0f, 2.0f, 0.0f), false);
  ground.initializeGL(m_program, getAssetsPath(), glm::vec3(100.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  goal.initializeGL(m_program, getAssetsPath(), glm::vec3(10.0f), glm::vec3(0.0f, 4.5f, 0.8f), glm::radians(-90.0f), glm::vec3(0, 0, 1));
  goal2.initializeGL(m_program, getAssetsPath(), glm::vec3(10.0f), glm::vec3(0.0f, 4.5f, 0.8f), glm::radians(90.0f), glm::vec3(0, 0, 1)); 
}

void OpenGLWindow::paintGL() {

  update();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(m_program);

  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  GLint lightDirLoc{glGetUniformLocation(m_program, "lightDirWorldSpace")};
  GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{glGetUniformLocation(m_program, "Is")};
 
  GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};
  
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  
  glUniform1i(diffuseTexLoc, 0);
  
  auto lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};
  glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);

  float deltaTime{static_cast<float>(getDeltaTime())};
  
  bola.paintGL(m_viewMatrix, deltaTime);
  bola2.paintGL(m_viewMatrix, deltaTime);
  bola3.paintGL(m_viewMatrix, deltaTime);
  ground.paintGL(m_viewMatrix, deltaTime);
  goal.paintGL(m_viewMatrix, deltaTime);
  goal2.paintGL(m_viewMatrix, deltaTime);

  glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
  m_trackBallModel.resizeViewport(width, height);
  m_trackBallLight.resizeViewport(width, height);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_program);
  
}

void OpenGLWindow::update() {

  float deltaTime{static_cast<float>(getDeltaTime())};

  m_modelMatrix = m_trackBallModel.getRotation();
  
  //mouse movement
  glm::ivec2 currentPosition;
  SDL_GetMouseState(&currentPosition.x, &currentPosition.y);

  if(currentPosition.x != m_lastPosition.x){

    if(currentPosition.x > m_lastPosition.x){
      m_panSpeed = 15.0f;
      m_lastPosition.x = m_lastPosition.x + 1;
    }
    else if(currentPosition.x < m_lastPosition.x){
      m_panSpeed = -15.0f;
      m_lastPosition.x = m_lastPosition.x - 1;
    }
  }
  else{

    m_panSpeed = 0.0f;
  }

  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  
  bola.moveX(m_ballSpeedX * deltaTime);
  bola.moveZ(m_ballSpeedZ * deltaTime);
  
}