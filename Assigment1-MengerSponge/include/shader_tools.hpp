#pragma once

#include "glad/glad.h"

namespace openGL {

bool checkShaderCompileStatus(GLuint shaderID);

bool checkProgramLinkStatus(GLuint programID);

} // namespace openGL
