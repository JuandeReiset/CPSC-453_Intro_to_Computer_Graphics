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

#include <utility>

#include "glad/glad.h"

#include "object.hpp"

namespace opengl {

class Texture {
public:
  Texture(GLuint name) : m_object(name) {}

  GLuint id() const;

  void bind() const;
  void unbind() const;

private:
  Object<Texture> m_object;
};

Texture generateTexture();

} // opengl
