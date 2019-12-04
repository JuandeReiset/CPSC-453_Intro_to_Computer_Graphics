/**
 * @author	Andrew Robert Owens, Philmo Gu
 * @date 	2019-10-15.
 * @details Organization: Biological Modeling and Visualization research group
 * University of Calgary, Calgary, AB, Canada
 *
 * Contact:	arowens [at] ucalgary.ca
 * @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
 *
 * @brief
 */

#pragma once

#include <string>

#include <glad/glad.h>

#include "object.hpp"

namespace opengl {

class Shader final {
public:
  enum Type : GLenum {
    INVALID = GL_INVALID_ENUM,
    VERTEX = GL_VERTEX_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER
  };

  enum : GLuint { INVALID_ID = 0 };

  bool isValid() const;

  explicit operator bool() const;

  Shader::Type type() const;

  GLuint id() const;

private:
  /* Only called through makeShader() factory function */
  Shader(GLuint id, Shader::Type shaderType);

  void release();

  friend Shader makeShader(std::string const &shaderCode,
                           Shader::Type shaderType);
  friend void swap(Shader &lhs, Shader &rhs);

private:
  Object<Shader> m_object;
  Shader::Type m_shaderType = INVALID;
};

Shader makeShader(std::string const &shaderCode, GLenum shaderType);

void swap(Shader &lhs, Shader &rhs);

Shader::Type enumToShaderType(GLenum shaderTypeEnum);

bool checkCompileStatus(GLuint shaderID);

bool isValidShaderID(GLuint id);

bool isValidShaderType(Shader::Type type);

} // namespace openGL
