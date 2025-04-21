#pragma once
#include"physis.h"
#include<vector>
#include<memory>
class Terr;
class PhyEng
{
	std::allocator<physis> BossBullet;
	std::allocator<physis> PlayerBullet;
	std::vector<physis> TheTerr;
};

