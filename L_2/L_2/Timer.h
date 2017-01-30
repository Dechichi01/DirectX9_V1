#pragma once

#include "Main.h"
#include <timeapi.h>
#include <string>

//-----------------------------------------------------------------------------
// Definitions, Macros & Constants
//-----------------------------------------------------------------------------
const ULONG MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

								   //-----------------------------------------------------------------------------
								   // Main Class Declarations
								   //-----------------------------------------------------------------------------
								   //-----------------------------------------------------------------------------
								   // Name : CTimer (Class)
								   // Desc : Game Timer class, queries performance hardware if available, and 
								   //        calculates all the various values required for frame rate based
								   //        vector / value scaling.
								   //-----------------------------------------------------------------------------
class CTimer
{
public:
	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class
	//-------------------------------------------------------------------------
	CTimer();
	virtual ~CTimer();

	//------------------------------------------------------------
	// Public Functions For This Class
	//------------------------------------------------------------
	void	        Tick(float fLockFPS = 0.0f);
	unsigned long   GetFrameRate(LPTSTR lpszString = NULL) const;
	float           GetTimeElapsed() const;

private:
	//------------------------------------------------------------
	// Private Variables For This Class
	//------------------------------------------------------------
	bool            m_PerfHardware;             // Has Performance Counter
	float           m_TimeScale;                // Amount to scale counter
	float           m_TimeElapsed;              // Time elapsed since previous frame
	__int64         m_CurrentTime;              // Current Performance Counter
	__int64         m_LastTime;                 // Performance Counter last frame
	__int64         m_PerfFreq;                 // Performance Frequency

	float           m_FrameTimes[MAX_SAMPLE_COUNT];
	ULONG           m_SampleCount;

	unsigned long   m_FrameRate;                // Stores current framerate
	unsigned long   m_FPSFrameCount;            // Elapsed frames in any given second
	float           m_FPSTimeElapsed;           // How much time has passed during FPS sample

												//------------------------------------------------------------
												// Private Functions For This Class
												//------------------------------------------------------------
};
