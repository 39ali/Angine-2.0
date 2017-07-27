#include "Time.h"

namespace Angine {
	namespace Core {

		double Time::m_deltaTime;
		double Time::m_prevTime;

		double Time::m_firstTime;
		float Time::m_frames;
		bool Time::m_first;
		float Time::m_fps;

		void Time::updateTime(const double& currentTime)
		{

			if (m_first) {
				m_firstTime = currentTime;
				m_first = false;
			}
			m_deltaTime = currentTime - m_prevTime;
			m_prevTime = currentTime;
		}


		void  Time::updateFps(const double& currentTime) {
			m_frames++;
			if ((currentTime - m_firstTime) >= 1.0)
			{
				m_fps = (double)m_frames / (currentTime - m_firstTime);
				m_frames = 0;
				m_firstTime = currentTime;
			}

		}

	}
}