#include "shader_tools.hpp"

#include <iostream>
#include <vector>

namespace openGL {

bool checkShaderCompileStatus(GLuint shaderID) {
  GLint result;
  int infoLogLength;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMsg(infoLogLength);
    glGetShaderInfoLog(shaderID, infoLogLength, nullptr, errorMsg.data());
    std::cerr << errorMsg.data() << std::endl;
    return false;
  }

  return true;
}

bool checkProgramLinkStatus(GLuint programID) {
  GLint result;
  int infoLogLength;
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMsg(infoLogLength);
    glGetProgramInfoLog(programID, infoLogLength, nullptr, errorMsg.data());
    std::cerr << errorMsg.data() << std::endl;
    return false;
  }

  return true;
}

} // namespace openGL
