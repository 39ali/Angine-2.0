#pragma once
#define GLEW_STATIC
#include <glew\glew.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "../common.h"
#include "../math/Color.h"
#include "../renderer/Window.h"
#include <string>

/// TODO: make sure you need all these includes ..?
namespace Angine {
	class OpenGLDevice {
	public:
		enum BufferUse {
			STATIC_DRAW = GL_STATIC_DRAW,
			STREAM_DRAW = GL_STREAM_DRAW,
			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,

			STATIC_COPY = GL_STATIC_COPY,
			STREAM_COPY = GL_STATIC_COPY,
			DYNAMIC_COPY = GL_DYNAMIC_COPY,

			STATIC_READ = GL_STATIC_READ,
			STREAM_READ = GL_STREAM_READ,
			DYNAMIC_READ = GL_DYNAMIC_READ
		};

		enum PrimitiveType {
			PRIMITIVE_TRIANGLES = GL_TRIANGLES,
			PRIMITIVE_POINTS = GL_POINTS,
			PRIMITIVE_LINE_STRIP = GL_LINE_STRIP,
			PRIMITIVE_LINE_LOOP = GL_LINE_LOOP,
			PRIMITIVE_LINES = GL_LINES,
			PRIMITIVE_LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
			PRIMITIVE_LINES_ADJACENCY = GL_LINES_ADJACENCY,
			PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
			PRIMITIVE_TRIANGLE_FAN = GL_TRIANGLE_FAN,
			PRIMITIVE_TRAINGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
			PRIMITIVE_TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
			PRIMITIVE_PATCHES = GL_PATCHES,
		};

		enum BlendFunc {
			BLEND_FUNC_NONE,
			BLEND_FUNC_ONE = GL_ONE,
			BLEND_FUNC_SRC_ALPHA = GL_SRC_ALPHA,
			BLEND_FUNC_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
			BLEND_FUNC_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
			BLEND_FUNC_DST_ALPHA = GL_DST_ALPHA,
		};

		enum FaceCulling {
			FACE_CULL_NONE,
			FACE_CULL_BACK = GL_BACK,
			FACE_CULL_FRONT = GL_FRONT,
			FACE_CULL_FRONT_AND_BACK = GL_FRONT_AND_BACK,
		};

		enum DrawFunc {
			DRAW_FUNC_NEVER = GL_NEVER,
			DRAW_FUNC_ALWAYS = GL_ALWAYS,
			DRAW_FUNC_LESS = GL_LESS,
			DRAW_FUNC_GREATER = GL_GREATER,
			DRAW_FUNC_LEQUAL = GL_LEQUAL,
			DRAW_FUNC_GEQUAL = GL_GEQUAL,
			DRAW_FUNC_EQUAL = GL_EQUAL,
			DRAW_FUNC_NOT_EQUAL = GL_NOTEQUAL,
		};

		enum FramebufferAttachment {
			ATTACHMENT_COLOR = GL_COLOR_ATTACHMENT0,
			ATTACHMENT_DEPTH = GL_DEPTH_ATTACHMENT,
			ATTACHMENT_STENCIL = GL_STENCIL_ATTACHMENT,
		};

		enum StencilOp {
			STENCIL_KEEP = GL_KEEP,
			STENCIL_ZERO = GL_ZERO,
			STENCIL_REPLACE = GL_REPLACE,
			STENICL_INCR = GL_INCR,
			STENCIL_INCR_WRAP = GL_INCR_WRAP,
			STENCIL_DECR_WRAP = GL_DECR_WRAP,
			STENCIL_DECR = GL_DECR,
			STENCIL_INVERT = GL_INVERT,
		};

		enum SamplerFilter  // or Texture_filter
		{
			FILTER_NEAREST = GL_NEAREST,
			FILTER_LINEAR = GL_LINEAR,
			FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			FILTER_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			FILTER_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			FILTER_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
		};

		enum SamplerWrapMode {
			WRAP_CLAMP = GL_CLAMP_TO_EDGE,
			WRAP_REPEAT = GL_REPEAT,
			WRAP_CLAMP_MIRROR = GL_MIRROR_CLAMP_TO_EDGE,
			WRAP_REPEAT_MIRROR = GL_MIRRORED_REPEAT,
		};

		enum TexturePixelFormat {
			FORMAT_R,
			FORMAT_RG,
			FORMAT_RGB,
			FORMAT_RGBA,
			FORMAT_BGR,
			FORMAT_BGRA,
			FORMAT_DEPTH,
			FORMAT_DEPTH_AND_STENCIL,
		};

		struct DrawInfo {
			enum PrimitiveType primitiveType = PrimitiveType::PRIMITIVE_TRIANGLES;
			enum FaceCulling faceCulling = FaceCulling::FACE_CULL_NONE;
			enum DrawFunc drawFunc = DrawFunc::DRAW_FUNC_ALWAYS;
			bool depthIsOn = true;

			// Stencil
			/*  bool StencilTestIsOn = false;
					  enum DrawFunc stencilFunc = DrawFunc::DRAW_FUNC_ALWAYS;
					  uint32 stencilTestMask = 0;
					  uint32 stencilWriteMask = 0;
					  uint32 stencilComparisionVal = 0;
					  enum StencilOp stencilFail = STENCIL_KEEP;
					  enum StencilOp stencilPassButDepthFail = STENCIL_KEEP;
					  enum StencilOp stencilPass = STENCIL_KEEP;*/
					  //
			bool useScissorTest = false;
			uint32 scissorStartX = 0;
			uint32 scissorStartY = 0;
			uint32 scissorWidth = 0;
			uint32 scissorHeight = 0;

			enum BlendFunc sourceBlend = BlendFunc::BLEND_FUNC_NONE;
			enum BlendFunc destBlend = BlendFunc::BLEND_FUNC_NONE;
		};

		static bool initialInit();
		OpenGLDevice(Renderer::Window& window);
		~OpenGLDevice();

		uint32 createFBO(uint32 texture, int32 width, int32 height,
			enum FrameBufferAttachment attachment,
			uint32 attachmentNumber, uint32 mipmaplevel);

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
			enum SamplerWrapMode magFilter,
			enum SamplerWrapMode wrapU, enum samplerWrapMode wrapV,
			float anisotropy);

		void deleteSampler(uint32 sampler);

		uint32 createTexture2D(int32 width, int32 height, const void* data,
			TexturePixelFormat internalFormat1,
			TexturePixelFormat dataFormat,
			bool generateMipmaps);
		void deleteTexture2D(uint32 texture2D);

		uint32 createUniformBuffer(const void* data, uintptr dataSize,
			enum BufferUsage usage);
		void updateUniformBuffer(uint32 buffer, const void* data, uintptr dataSize);
		void deleteUniformBuffer(uint32 buffer);

		uint32 createShaderProgram(const std::string& shaderText);

		void setShaderUniformBuffer(uint32 shader,
			const std::string& uniformBufferName,
			uint32 buffer);
		void setShaderSampler(uint32 shader, const std::string& samplerName,
			uint32 texture, uint32 sampler, uint32 unit);
		void deleteShaderProgram(uint32 shader);

		void draw(uint32 fbo, uint32 shader, uint32 vao, const DrawInfo& drawParams,
			uint32 numInstances, uint32 numElements);
		void draw(uint32 shader);

		void clear(uint32 fbo, bool shouldclearcolor, bool shouldcleardepth,
			bool shouldclearstencil, const Color& color, uint32 stencil);

		void setUniform() const;

	private:
		struct VertexArray {
			enum BufferUse usage;
			uint32* buffers;
			uintptr* bufferSizes;
			uint32 numBuffers;
			uint32 numElements;
			uint32 instanceComponentsStartIndex;
		};

		struct ShaderString {
			std::string vertex;
			std::string fragment;
		};

		struct ShaderProgram {
			std::unordered_map<std::string, int32> uniforms;
			std::vector<uint32> shaders;
			std::unordered_map<std::string, int32> samplers;
		};

		struct Fbo {
			uint32 width;
			uint32 height;
		};

		std::unordered_map<uint32, VertexArray> vaoMap;
		std::unordered_map<uint32, Fbo> fboMap;
		std::unordered_map<uint32, ShaderProgram> shaderProgramMap;

		uint32 currentFBO;
		uint32 curentviewportFBO;
		uint32 currentVAO;
		uint32 currentShader;
		enum FaceCulling currentFaceCulling;
		enum DrawFunc currentDepthFunc;
		enum BlendFunc currentSourceBlend;
		enum BlendFunc currentDestBlend;
		enum DrawFunc currentStencilFunc;
		uint32 currentStencilTestMask;
		uint32 currentStencilWriteMask;
		int32 currentStencilComparisonVal;
		enum StencilOp currentStencilFail;
		enum StencilOp currentStencilPassButDepthFail;
		enum StencilOp currentStencilPass;
		bool blendingEnabled;
		bool shouldWriteDepth;
		bool stencilTestEnabled;
		bool scissorTestEnabled;

		static bool isInitialized;

		void setFBO(uint32 fbo);
		void setViewport(uint32 view);
		void setVAO(uint32 vao);
		void setShader(uint32 shader);
		void setFaceCulling(enum FaceCulling faceCulling);
		void setDepthTest(enum DrawFunc depthFunc, bool shouldWrite);
		void setBlending(enum BlendFunc sourceBlend, enum BlendFunc destBlend);
		void setStencilTest(bool enable, enum DrawFunc stencilFunc,
			uint32 stencilTestMask, uint32 stencilWriteMask,
			int32 stencilComparisonVal, enum StencilOp stencilFail,
			enum StencilOp stencilPassButDepthFail,
			enum StencilOp stencilPass);
		// void setStencilWriteMask(uint32 mask);
		void setScissorTest(bool enable, uint32 startX = 0, uint32 startY = 0,
			uint32 width = 0, uint32 height = 0);
		void getCurrentVersion() const;

		static ShaderString parseShader(const std::string& filepath);
		static GLuint compile(const std::string& shaderSource, GLenum type,
			const std::string& filename, GLuint m_program_id);
		static void link(GLuint m_program_id, GLuint m_vs_id, GLuint m_fg_id);
	};
} // namespace Angine
