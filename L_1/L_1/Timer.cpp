#include "Timer.h"

CTimer::CTimer()
{
	//Query performance hardware and setup time scaling values
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&m_PerfFreq))
	{
		m_PerfHardware = TRUE;
		QueryPerformanceCounter((LARGE_INTEGER *)&m_LastTime);
		m_TimeScale = 1.0f / m_PerfFreq;
	}
	else//No performance conter, read in using timeGetTime
	{
		m_PerfHardware = FALSE;
		m_LastTime = timeGetTime();
		m_TimeScale = .001f;
	}

	//Clear variables
	m_SampleCount = 0;
	m_FrameRate = 0;
	m_FPSFrameCount = 0;
	m_FPSTimeElapsed = .0f;
}


CTimer::~CTimer()
{
}

void CTimer::Tick(float fLockFPS)
{
	float fTimeElapsed;

	//Query the time passed since system boot
	if (m_PerfHardware)
		QueryPerformanceCounter((LARGE_INTEGER *)&m_CurrentTime);
	else
		m_CurrentTime = timeGetTime();

	//Calculate elapsed time in seconds
	fTimeElapsed = (m_CurrentTime - m_LastTime)*m_TimeScale;
	//Smoothly ramp up frame rate to prevent jittering
	if (fLockFPS == .0f) fLockFPS = (1.f / GetTimeElapsed()) + 20.f;

	//Should we lock the frame rate?
	if (fLockFPS > 0.0f)
	{
		while (fTimeElapsed < (1.0f / fLockFPS))//Soak up time until fTimeElpsed is 1/fLockFPS
		{
			if (m_PerfHardware)
				QueryPerformanceCounter((LARGE_INTEGER *)&m_CurrentTime);
			else
				m_CurrentTime = timeGetTime();

			//Calculate elapsed time in seconds
			fTimeElapsed = (m_CurrentTime - m_LastTime)*m_TimeScale;
		}
	}

	//Save current frame time
	m_LastTime = m_CurrentTime;

	//Only adds the value if it's not widly different from average
	if (fabsf(fTimeElapsed - m_TimeElapsed) < 1.0f)
	{
		//Wrap FIFO frame time buffer
		memmove(&m_FrameTimes[1], m_FrameTimes, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		m_FrameTimes[0] = fTimeElapsed;
		if (m_SampleCount < MAX_SAMPLE_COUNT) m_SampleCount++;
	}

	//Calculate frme rate
	m_FPSFrameCount++;
	m_FPSTimeElapsed += m_TimeElapsed;
	if (m_FPSTimeElapsed > 1.0f) //One second elapsed?
	{
		m_FrameRate = m_FPSFrameCount;
		m_FPSFrameCount = 0;
		m_FPSTimeElapsed = .0f;
	}

	//Count up the new average elapsed time
	m_TimeElapsed = .0f;
	for (ULONG i = 0; i < m_SampleCount; i++) m_TimeElapsed += m_FrameTimes[i];
	if (m_SampleCount > 0) m_TimeElapsed /= m_SampleCount;
}

unsigned long CTimer::GetFrameRate(LPTSTR lpszString) const
{
	//Filled string buffer?
	if (lpszString)
	{
		std::string s = std::to_string(m_FrameRate).c_str();
		s += " FPS";
		lpszString = (LPTSTR)s.c_str();
	}

	return m_FrameRate;
}

float CTimer::GetTimeElapsed() const
{
	return m_TimeElapsed;
}

