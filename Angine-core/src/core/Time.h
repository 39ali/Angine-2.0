#pragma once

namespace Angine {
	namespace Core {
		class Time
		{
		public:
			Time() = delete;
			~Time() = delete;

			static	const double&  getDeltaTime() { return m_deltaTime; }
			void static updateTime(const double& currentTime);
			void static updateFps(const double& currentTime);
			static const double& getTime() { return m_prevTime; }
			static const float&  getFps() { return m_fps; }
		private:
			static	double m_deltaTime;
			static	double m_prevTime;

			static double m_firstTime;
			static  float m_fps;
			static float m_frames;
			static bool m_first;
		};
	}
}