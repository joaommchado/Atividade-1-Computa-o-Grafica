#include "stadium.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/hash.hpp>
#include "abcg.hpp"

void Stadium::initializeGL(GLuint program, std::string assetsPath, glm::vec3 scale, glm::vec3 translation) {

  m_program = program;

  modelMatrixLoc = glGetUniformLocation(m_program, "modelMatrix");
  viewMatrixLoc = glGetUniformLocation(m_program, "viewMatrix");
  normalMatrixLoc = glGetUniformLocation(m_program, "normalMatrix");
  shininessLoc = glGetUniformLocation(m_program, "shininess");
  mappingModeLoc = glGetUniformLocation(m_program, "mappingMode");

  m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
  
  KaLoc = glGetUniformLocation(m_program, "Ka");
  KdLoc = glGetUniformLocation(m_program, "Kd");
  KsLoc = glGetUniformLocation(m_program, "Ks");

  m_modelMatrix = glm::scale(m_modelMatrix, scale);
  m_modelMatrix = glm::translate(m_modelMatrix, translation);

  loadModel(assetsPath);
}

void Stadium::paintGL(glm::mat4 viewMatrix, double deltaTime) {

  //m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0f, 0.0f, (0.25f * deltaTime)));
  //m_modelMatrix = glm::rotate(m_modelMatrix, (glm::radians(45.0f) * (float)deltaTime), glm::vec3(0, 1, 0));
  
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

  auto modelViewMatrix{glm::mat3(viewMatrix * m_modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  // Draw white bunny
  //glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  //glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
  //glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
  
  glUniform1i(mappingModeLoc, m_mappingMode);
  glUniform1f(shininessLoc, m_shininess);
  glUniform4fv(KaLoc, 1, &m_Ka.x);
  glUniform4fv(KdLoc, 1, &m_Kd.x);
  glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_model.render(m_trianglesToDraw);
}

void Stadium::loadModel(std::string path) {

  m_model.loadDiffuseTexture(path + "wembley/wembley.jpg");
  m_model.loadFromFile(path + "wembley/wembley.obj");
  m_model.setupVAO(m_program);
  m_trianglesToDraw = m_model.getNumTriangles();
  

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}