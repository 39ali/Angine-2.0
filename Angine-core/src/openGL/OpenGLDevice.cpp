#include "OpenGLDevice.h"

namespace Angine {

bool OpenGLDevice::isInitialized = false;
bool OpenGLDevice::initialInit() {
  if (isInitialized) return true;
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  isInitialized = true;
  return true;
}
OpenGLDevice::OpenGLDevice(Renderer::Window& window)
    : currentFBO(0),
      curentviewportFBO(0),
      currentVAO(0),
      currentShader(0),
      currentFaceCulling(FACE_CULL_NONE),
      currentDepthFunc(DRAW_FUNC_ALWAYS),
      currentSourceBlend(BLEND_FUNC_NONE),
      currentDestBlend(BLEND_FUNC_NONE),
      currentStencilFunc(DRAW_FUNC_ALWAYS),
      currentStencilTestMask((uint32)0xFFFFFFFF),
      currentStencilWriteMask((uint32)0xFFFFFFFF),
      currentStencilComparisonVal(0),
      currentStencilFail(STENCIL_KEEP),
      currentStencilPassButDepthFail(STENCIL_KEEP),
      currentStencilPass(STENCIL_KEEP),
      blendingEnabled(false),
      shouldWriteDepth(false),
      stencilTestEnabled(false),
      scissorTestEnabled(false) {
  glfwMakeContextCurrent(window.getWindowHandle());
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glewExperimental = GL_TRUE;
  GLenum stat = glewInit();
  if (stat != GLEW_OK) {
    A_LOG("Render", "%s", glewGetErrorString(stat));
    throw std::runtime_error("could not initialize Render Device");
  }

  // glDepthFunc(DRAW_FUNC_ALWAYS);
  // glDepthMask(GL_FALSE);

  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  Fbo fbo;
  fbo.width = window.getWidth();
  fbo.height = window.getHeight();
  fboMap[0] = fbo;
};
OpenGLDevice::~OpenGLDevice() {}

void OpenGLDevice::clear(uint32 fbo, bool shouldclearcolor,
                         bool shouldcleardepth, bool shouldclearstencil,
                         const Color& color, uint32 stencilWriteMask) {
  if (fbo != currentFBO) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    currentFBO = fbo;
  }

  uint32 currentFlags = 0;
  if (shouldclearcolor) {
    currentFlags |= GL_COLOR_BUFFER_BIT;
    glClearColor(color[0], color[1], color[2], color[3]);
  }
  if (shouldcleardepth) {
    currentFlags |= GL_DEPTH_BUFFER_BIT;
  }
  if (shouldclearstencil) {
    //  setStencilWriteMask(stencilWriteMask);
    currentFlags |= GL_STENCIL_BUFFER_BIT;
  }
  glClear(currentFlags);
}

void OpenGLDevice::setFBO(uint32 fbo) {
  if (fbo == currentFBO) {
    return;
  }

  currentFBO = fbo;
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void OpenGLDevice::setViewport(uint32 fbo) {
  if (fbo == curentviewportFBO) {
    return;
  }
  glViewport(0, 0, fboMap[fbo].width, fboMap[fbo].height);
  curentviewportFBO = fbo;
}

void OpenGLDevice::setShader(uint32 shader) {
  if (shader == currentShader) {
    return;
  }
  glUseProgram(shader);
  currentShader = shader;
}

void OpenGLDevice::setVAO(uint32 vao) {
  if (vao == currentVAO) {
    return;
  }
  currentVAO = vao;
  glBindVertexArray(vao);
}

void OpenGLDevice::setScissorTest(bool enable, uint32 startX, uint32 startY,
                                  uint32 width, uint32 height) {
  if (!enable) {
    if (!scissorTestEnabled) {
      return;
    } else {
      glDisable(GL_SCISSOR_TEST);
      scissorTestEnabled = false;
      return;
    }
  }
  if (!scissorTestEnabled) {
    glEnable(GL_SCISSOR_TEST);
  }
  glScissor(startX, startY, width, height);
  scissorTestEnabled = true;
}

void OpenGLDevice::setBlending(enum BlendFunc sourceBlend,
                               enum BlendFunc destBlend) {
  if (sourceBlend == currentSourceBlend && destBlend == currentDestBlend) {
    return;
  } else if (sourceBlend == BLEND_FUNC_NONE || destBlend == BLEND_FUNC_NONE) {
    glDisable(GL_BLEND);
  } else if (currentSourceBlend == BLEND_FUNC_NONE ||
             currentDestBlend == BLEND_FUNC_NONE) {
    glEnable(GL_BLEND);
    glBlendFunc(sourceBlend, destBlend);
  } else {
    glBlendFunc(sourceBlend, destBlend);
  }

  currentSourceBlend = sourceBlend;
  currentDestBlend = destBlend;
}

void OpenGLDevice::setFaceCulling(enum FaceCulling faceCulling) {
  if (currentFaceCulling == faceCulling) {
    return;
  }

  if (currentFaceCulling == FACE_CULL_NONE) {
    glDisable(GL_CULL_FACE);
  } else if (currentFaceCulling == FACE_CULL_NONE) {
    glCullFace(faceCulling);
    glEnable(GL_CULL_FACE);
  } else {
    glCullFace(currentFaceCulling);
  }
  currentFaceCulling = faceCulling;
}

void OpenGLDevice::setDepthTest(enum DrawFunc depthfunc, bool shouldWrite) {
  if (shouldWrite != shouldWriteDepth) {
    shouldWriteDepth = shouldWrite;
    glDepthMask(shouldWrite ? GL_TRUE : GL_FALSE);
  }

  if (depthfunc = currentDepthFunc) {
    return;
  }
  currentDepthFunc = depthfunc;
  glDepthFunc(depthfunc);
}

uint32 OpenGLDevice::createFBO(uint32 texture, int32 width, int32 height,
                               enum FrameBufferAttachment attachment,
                               uint32 attachmentNumber, uint32 mipmaplevel) {
  uint32 fbo;
  glGenFramebuffers(1, &fbo);
  setFBO(fbo);

  GLenum att = attachment + attachmentNumber;
  glFramebufferTexture2D(GL_FRAMEBUFFER, att, GL_TEXTURE_2D, texture,
                         mipmaplevel);

  Fbo data;
  data.width = width;
  data.height = height;
  fboMap[fbo] = data;

  return fbo;
}

void OpenGLDevice::unbindFBO(uint32 fbo) {
  auto it = fboMap.find(fbo);
  if (it == fboMap.end()) return;
  glDeleteFramebuffers(1, &fbo);

  fboMap.erase(it);
}

uint32 OpenGLDevice::createVertexArray(const float** Data,
                                       const uint32* vertexElementSizes,
                                       uint32 numVertexComponents,
                                       uint32 numInstanceComponents,
                                       uint32 numVertices,
                                       const uint32* indices, uint32 numIndices,
                                       enum BufferUsage usage) {
  return 0;
}

void OpenGLDevice::updateVertexArrayBuffer(uint32 vao, uint32 bufferIndex,
                                           const void* data, uintptr dataSize) {
  if (!vao) return;
  auto it = vaoMap.find(vao);
  if (it == vaoMap.end()) return;

  const VertexArray* vdata = &it->second;
  //  TODO complete this;
}

void deleteVertexArray(uint32 vao);

uint32 OpenGLDevice::createSampler(
    enum SamplerFilter minFilter, enum SamplerWrapMode magFilter,
    enum SamplerWrapMode wrapU, enum samplerWrapMode wrapV,
    float anisotropy)  // anisotropy  val :1.0f-16.0f
{
  uint32 samp = 0;
  glGenSamplers(1, &samp);
  glSamplerParameteri(samp, GL_TEXTURE_WRAP_S, wrapU);
  glSamplerParameteri(samp, GL_TEXTURE_WRAP_T, wrapV);
  glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, minFilter);
  glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, magFilter);
  if (anisotropy != 0.0f && minFilter != SamplerFilter::FILTER_NEAREST &&
      minFilter != SamplerFilter::FILTER_LINEAR)
    glSamplerParameterf(samp, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

  return samp;
}

void OpenGLDevice::deleteSampler(uint32 sampler) {
  if (sampler == 0) return;
  glDeleteSamplers(1, &sampler);
};

static GLint getOpenGLFormat(OpenGLDevice::TexturePixelFormat format) {
  switch (format) {
    case OpenGLDevice::FORMAT_R:
      return GL_RED;
    case OpenGLDevice::FORMAT_RG:
      return GL_RG;
    case OpenGLDevice::FORMAT_RGB:
      return GL_RGB;
    case OpenGLDevice::FORMAT_RGBA:
      return GL_RGBA;
    case OpenGLDevice::FORMAT_BGR:
      return GL_BGR;
    case OpenGLDevice::FORMAT_BGRA:
      return GL_BGRA;
    case OpenGLDevice::FORMAT_DEPTH:
      return GL_DEPTH_COMPONENT;
    case OpenGLDevice::FORMAT_DEPTH_AND_STENCIL:
      return GL_DEPTH_STENCIL;
  };

  A_LOG("Render", "PixelFormat %d is not a valid PixelFormat.", format);
  return 0;
}

uint32 OpenGLDevice::createTexture2D(int32 width, int32 height,
                                     const void* data,
                                     TexturePixelFormat internalFormat1,
                                     TexturePixelFormat dataFormat,

                                     bool generateMipmaps) {
  GLint format = getOpenGLFormat(dataFormat);
  GLint internalFormat = getOpenGLFormat(internalFormat1);
  GLenum textureTarget = GL_TEXTURE_2D;
  GLuint texture;

  glGenTextures(1, &texture);
  glBindTexture(textureTarget, texture);

  glTexImage2D(textureTarget, 0, internalFormat, width, height, GL_FALSE,
               format, GL_UNSIGNED_BYTE, (const void*)data);

  glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

  if (generateMipmaps)
    glGenerateMipmap(textureTarget);
  else {
    glTexParameteri(textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
  }
  return texture;
}

void OpenGLDevice::deleteTexture2D(uint32 texture2D) {
  if (texture2D == 0) return;
  glDeleteTextures(1, &texture2D);
}

uint32 OpenGLDevice::createUniformBuffer(const void* data, uintptr dataSize,
                                         enum BufferUsage usage) {
  uint32 ubo;
  glGenBuffers(1, &ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(GL_UNIFORM_BUFFER, dataSize, data, usage);
  return ubo;
}
void OpenGLDevice::updateUniformBuffer(uint32 buffer, const void* data,
                                       uintptr dataSize) {
  glBindBuffer(GL_UNIFORM_BUFFER, buffer);
  void* dest = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
  memcpy(dest, data, dataSize);
  glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void OpenGLDevice::deleteUniformBuffer(uint32 buffer) {
  if (buffer == 0) return;
  deleteUniformBuffer(buffer);
}

void OpenGLDevice::setShaderUniformBuffer(uint32 shader,
                                          const std::string& uniformBufferName,
                                          uint32 buffer) {
  setShader(shader);
  glBindBufferBase(GL_UNIFORM_BUFFER,
                   shaderProgramMap[shader].uniforms[uniformBufferName],
                   buffer);
}
void OpenGLDevice::setShaderSampler(uint32 shader,
                                    const std::string& samplerName,
                                    uint32 texture, uint32 sampler,
                                    uint32 unit) {
  setShader(shader);
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindSampler(unit, sampler);
  glUniform1i(shaderProgramMap[shader].samplers[samplerName], unit);
}

void OpenGLDevice::deleteShaderProgram(uint32 shader) {
  if (shader == 0) return;

  auto it = shaderProgramMap.find(shader);
  if (it == shaderProgramMap.end()) return;

  const ShaderProgram* shaderProgram = &it->second;

  for (auto it = shaderProgram->shaders.begin();
       it != shaderProgram->shaders.end(); ++it) {
    glDetachShader(shader, *it);
    glDeleteShader(*it);
  }

  glDeleteProgram(shader);
  shaderProgramMap.erase(it);
};

uint32 OpenGLDevice::createShaderProgram(const std::string& filepath) {
  GLuint shaderProgram = glCreateProgram();

  if (shaderProgram == 0) {
    A_LOG("RENDER", "error", 0);
    return -1;
  }
  ShaderString ss = parseShader(filepath);
  GLuint m_vs_id =
      compile(ss.vertex, GL_VERTEX_SHADER, filepath, shaderProgram);
  GLuint m_fg_id =
      compile(ss.fragment, GL_FRAGMENT_SHADER, filepath, shaderProgram);
  link(shaderProgram, m_vs_id, m_fg_id);

  return shaderProgram;
}

OpenGLDevice::ShaderString OpenGLDevice::parseShader(
    const std::string& filepath) {
  std::ifstream file(filepath);
  std::string line;
  std::stringstream stringStreams[2];

  if (file.fail()) {
    std::cout << "ERROR opening : " << filepath << std::endl;
    return {};
  }

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMNET = 1 };

  ShaderType shaderType = ShaderType::NONE;

  while (getline(file, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        shaderType = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        shaderType = ShaderType::FRAGMNET;
      }

    } else
      stringStreams[(int)shaderType] << line << '\n';
  }

  return {stringStreams[0].str(), stringStreams[1].str()};
}

GLuint OpenGLDevice::compile(const std::string& shaderSource, GLenum type,
                             const std::string& filename, GLuint m_program_id) {
  GLuint shader = glCreateShader(type);

  const GLchar* source = (const GLchar*)shaderSource.c_str();
  glShaderSource(shader, 1, &source, 0);

  glCompileShader(shader);

  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

    std::cout << "shader compile  error : " << filename << "->>"
              << std::string((type == GL_VERTEX_SHADER) ? "VERTEX_SHADER"
                                                        : "FRAGMENT_SHADER")
              << " : " << std::string(infoLog.begin(), infoLog.end())
              << std::endl;

    glDeleteShader(shader);

    return 0;
  }
  glAttachShader(m_program_id, shader);
  return shader;
};

void OpenGLDevice::link(GLuint m_program_id, GLuint m_vs_id, GLuint m_fg_id) {
  glLinkProgram(m_program_id);

  GLint isLinked = 0;
  glGetProgramiv(m_program_id, GL_LINK_STATUS, (int*)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &maxLength);
    //	if (maxLength == 0)return;

    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(m_program_id, maxLength, &maxLength, &infoLog[0]);

    std::cout << "shader linking error : "
              << std::string(infoLog.begin(), infoLog.end()) << std::endl;

    glDeleteProgram(m_program_id);

    glDeleteShader(m_vs_id);
    glDeleteShader(m_vs_id);

    return;
  }

  glDetachShader(m_program_id, m_vs_id);
  glDetachShader(m_program_id, m_fg_id);
}

void OpenGLDevice::draw(uint32 fbo, uint32 shader, uint32 vao,
                        const DrawInfo& drawParams, uint32 numInstances,
                        uint32 numElements) {}

void OpenGLDevice::draw(uint32 shader) { setShader(shader); }

void OpenGLDevice::setUniform() const {}

}  // namespace Angine
