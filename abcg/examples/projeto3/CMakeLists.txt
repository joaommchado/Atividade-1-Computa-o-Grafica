project(projeto3)
add_executable(${PROJECT_NAME} main.cpp model.cpp openglwindow.cpp
                               trackball.cpp bola.cpp ground.cpp camera.cpp stadium.cpp goal.cpp)
enable_abcg(${PROJECT_NAME})
