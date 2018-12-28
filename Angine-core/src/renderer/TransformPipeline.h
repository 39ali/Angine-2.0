#pragma once
#include "../math/Vector3f.h"
#include "../math/Matrix4f.h"
namespace Angine {
	class TransformPipeline
	{
	public:
		TransformPipeline() :scale(vec3f(1, 1, 1)) {};
		~TransformPipeline() {};
		inline void setScale(const vec3f& scale) {
			this->scale = scale; 
		}
		inline void setScale(const vec3f&& scale) 
		{
			this->scale = std::move(scale);
		}
		inline void setWorldPos(const vec3f& worldPos) { this->worldPos = worldPos; };
		inline void setWorldPos(const vec3f&& worldPos) { this->worldPos =std::move( worldPos); };


		// in degrees
		inline void setRotate(const vec3f& rotvec) { this->rotate = rotvec; };
		inline void setRotate(const vec3f&& rotvec) {
			this->rotate = std::move(rotvec);
		};
		inline void setPerspectiveProj(const float znear ,const float zfar ,const float width ,const  float height,const float fov) {
			PerspectiveProjection.znear = znear;
			PerspectiveProjection.zfar = zfar;
			PerspectiveProjection.width = width;
			PerspectiveProjection.height = height;
			PerspectiveProjection.fov = fov;
		}

		inline void setCamera(const vec3f& pos , const vec3f&target , const vec3f& up) {
			camera.pos = pos;
			camera.target = target;
			camera.up = up;
		}
		const mat4f* getTransform() {
			Matrix4f mscale, mrotate, mtranslate,proj, mcamera;
			mscale.makeScale(scale);
			mrotate.makeRotateTransform(rotate);
			mtranslate.makeTranslate(worldPos);
			mcamera.makeCameraTransform(camera.pos, camera.target, camera.up);
			proj.makePerspectiveProjection(PerspectiveProjection.znear, PerspectiveProjection.zfar, PerspectiveProjection.width,
				PerspectiveProjection.height, PerspectiveProjection.fov);
			
			worldTransform = mtranslate * mrotate*mscale;
			transform =proj*mcamera*worldTransform;
			//transform = stransform;
			return &transform;
		};
		const mat4f* getWorldTransform() { return &worldTransform; }
	private:

		struct PerspectiveProjData{
			float znear, zfar, width, height, fov;
		};

		vec3f scale;
		vec3f rotate;
		vec3f worldPos;
		mat4f transform;
		mat4f worldTransform;
		PerspectiveProjData PerspectiveProjection;
		struct {
			vec3f pos;
			vec3f target;
			vec3f up;
		} camera;

	};


}