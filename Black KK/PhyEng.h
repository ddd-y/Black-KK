#pragma once
#include"physis.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<vector>
#include<memory>
class Terr;
class PhyEng
{
	std::allocator<physis> BossBullet;
	std::allocator<physis> PlayerBullet;
	std::vector<physis> TheTerr;
};

