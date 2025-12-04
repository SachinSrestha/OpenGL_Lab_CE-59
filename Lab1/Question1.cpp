#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  if (mode)
  {
    std::cout << "Screen Resolution: " << mode->width << "x" << mode->height << std::endl;
  }
  else
  {
    std::cerr << "Failed to get video mode" << std::endl;
  }

  glfwTerminate();
  return 0;
}
