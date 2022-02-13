#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <string_view>

#include "abcg.hpp"
#include "model.hpp"
#include "trackball.hpp"
#include "bola.hpp"
#include "camera.hpp"
#include "ground.hpp"
#include "stadium.hpp"
#include "goal.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;
  void start();

 private:
  int m_viewportWidth{};
  int m_viewportHeight{};

  glm::vec2 m_lastPosition{0.0f};
  glm::vec2 m_axis{};

  Model m_model;
  Bola bola;
  Bola bola2;
  Bola bola3;

  Stadium stadium;
  Ground ground;
  Camera m_camera;
  Goal goal;
  Goal goal2;

  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_ballSpeedX{0.0f};
  float m_ballSpeedZ{0.0f};
  float m_panSpeed{0.0f};

  float directionX = 0.0f;
  float directionY = 0.0f;
  float speed = 5.0f;
  float speedY = 5.0f;

  int m_trianglesToDraw{};

  TrackBall m_trackBallModel;
  TrackBall m_trackBallLight;
  float m_zoom{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  // Shaders
  std::vector<const char*> m_shaderNames{
      "normalmapping", "texture", "blinnphong", "phong",
      "gouraud",       "normal",  "depth"};
  GLuint m_program;

  // Skybox
  const std::string m_skyShaderName{"skybox"};
  GLuint m_skyVAO{};
  GLuint m_skyVBO{};
  GLuint m_skyProgram{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};

  float m_shininess{};
  bool m_mouseMove;
  bool m_mouseClick;

  //void loadBallModel(std::string_view path);
  void update();
  void mouseMove(glm::ivec2 position);
};

#endif