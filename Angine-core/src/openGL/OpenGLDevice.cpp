#include "OpenGLDevice.h"

namespace Angine {

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

  glewExperimental = GL_TRUE;
  GLenum stat = glewInit();
  if (stat != GLEW_OK) {
    A_LOG("Render", "%s", glewGetErrorString(stat));
    throw std::runtime_error("could not initialize Render Device");
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(DRAW_FUNC_ALWAYS);
  glDepthMask(GL_FALSE);
  glFrontFace(GL_CW);

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
    setStencilWriteMask(stencilWriteMask);
    currentFlags |= GL_STENCIL_BUFFER_BIT;
  }
  glClear(currentFlags);
}

void OpenGLDevice::draw(uint32 fbo, uint32 shader, uint32 vao,
                        const DrawInfo& drawInfo, uint32 numInstances,
                        uint32 numElements) {}

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

uint32 createFBO(uint32 texture, int32 width, int32 height,
                 enum FrameBufferAttachment attachment, uint32 attachmentNumber,
                 uint32 mipmaplevel);

void unbindFBO(uint32 fbo);

uint32 createVertexArray(const float** Data, const uint32* vertexElementSizes,
                         uint32 numVertexComponents,
                         uint32 numInstanceComponents, uint32 numVertices,
                         const uint32* indices, uint32 numIndices,
                         enum BufferUsage usage);

void updateVertexArrayBuffer(uint32 vao, uint32 bufferIndex, const void* data,
                             uintptr dataSize);

void deleteVertexArray(uint32 vao);

uint32 createSampler(enum SamplerFilter minFilter,
                     enum SamplerWrapMode magFilter, enum SamplerWrapMode wrapU,
                     enum samplerWrapMode wrapV, float anisotropy);

uint32 deleteSampler(uint32 sampler);

uint32 createTexture2D(int32 width, int32 height, const void* data,
                       enum PixelFormat dataFormat,
                       enum PixelFormat internalFormat, bool generateMipmaps,
                       bool compress);
uint32 deleteTexture2D(uint32 texture2D);

uint32 createUniformBuffer(const void* data, uintptr dataSize,
                           enum BufferUsage usage);
void updateUniformBuffer(uint32 buffer, const void* data, uintptr dataSize);
uint32 deleteUniformBuffer(uint32 buffer);

void setShaderUniformBuffer(uint32 shader, const std::string& uniformBufferName,
                            uint32 buffer);
void setShaderSampler(uint32 shader, const std::string& samplerName,
                      uint32 texture, uint32 sampler, uint32 unit);
uint32 deleteShaderProgram(uint32 shader);

void draw(uint32 fbo, uint32 shader, uint32 vao, const DrawOInfo& drawParams,
          uint32 numInstances, uint32 numElements);

}  // namespace Angine
