/* Copyright (c) 2020, Dyssol Development Team. All rights reserved. This file is part of Dyssol. See LICENSE file for license information. */

#pragma once

#include "DynamicUnit.h"

class COutlet : public CDynamicUnit
{
public:
	COutlet();

	void Simulate(double _dStartTime, double _dEndTime) override;
};
