#pragma once

#include "CObject.h"

#include <chrono>

class CEvader {
private:
	int m_nLastMoveCmdTick;
	std::chrono::milliseconds m_lastMoveClickTime;
	bool useAutokey;

public:
	CEvader::CEvader() : m_nLastMoveCmdTick(0), m_lastMoveClickTime(0),
		useAutokey(false)
	{
	}

public:
	bool CEvader::drawEvent();
};
