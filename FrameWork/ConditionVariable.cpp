#include "stdafx.h"

std::condition_variable			GhasPacketDataCond_;
std::atomic_bool				GisEmptyPacketQueue_ = false;