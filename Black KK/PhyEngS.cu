#include "PhyEngS.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "physis.h"
#include"Boss.h"
#include"Player.h"
#include"AINode.h"
#include<chrono>
#include<fstream>
#include<thread>
__global__ void UpdateBulletKernel(int* speedx, int* speedy, int* NowX, int* NowY, int* IfBossBulletValid, 
	int* D_TheGrid, int TheWidth, int TheHeight, int currentIndex,int PlayerX,int PlayerY,int*BossHitPlayer)
{
	int ThePlayerX = PlayerX;
	int ThePlayerY = PlayerY;
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= currentIndex)
		return;
	if (IfBossBulletValid[idx]==1)
	{
	    NowX[idx] += speedx[idx];
	    NowY[idx] += speedy[idx];
		if (NowX[idx] < 0 || NowX[idx] >= TheWidth || NowY[idx] < 0 || NowY[idx] >= TheHeight)
		{
			IfBossBulletValid[idx] = 0;
			return;
		}
		int isValid = (NowX[idx] != ThePlayerX || NowY[idx] != ThePlayerY);
		IfBossBulletValid[idx] *= isValid;
		if (!isValid)
			BossHitPlayer[idx] = 1;
		if (D_TheGrid[NowY[idx] * TheWidth + NowX[idx]] == 1)
			IfBossBulletValid[idx] = 0;
	}
}
__global__ void UpdatePlayerBulletKernel(int* aspeedx, int* aspeedy, int* aNowX, int* aNowY, int* IfPlayerBulletValid, 
	int* D_TheGrid, int TheWidth, int TheHeight, int currentIndex,int BossX,int BossY,int *PlayerHitBoss,int *BossNowX,int *BossNowY,int *IfBossBulletValid,int maxBossIndex)
{
	int TheBossX = BossX;
	int TheBossY = BossY;
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= currentIndex)
		return;
	if (IfPlayerBulletValid[idx]==1)
	{
		int isValid = 1;
		for (int i = 0; i < maxBossIndex; ++i)
		{
			if (IfBossBulletValid[i] == 1 && aNowX[idx] == BossNowX[i] && aNowY[idx] == BossNowY[i])
			{
				IfPlayerBulletValid[idx] = 0;
				break;
			}
		}
		aNowX[idx] += aspeedx[idx];
		aNowY[idx] += aspeedy[idx];
		if (aNowX[idx] < 0 || aNowX[idx] >= TheWidth || aNowY[idx] < 0 || aNowY[idx] >= TheHeight)
		{
			IfPlayerBulletValid[idx] = 0;
			return;
		}
		isValid = (aNowX[idx] != TheBossX || aNowY[idx] != TheBossY);
		IfPlayerBulletValid[idx] *= isValid;
		if (!isValid)
			PlayerHitBoss[idx] = 1;
		if (D_TheGrid[aNowY[idx] * TheWidth + aNowX[idx]] == 1)
			IfPlayerBulletValid[idx] = 0;
	}
}
void PhyEngS::PrePrepare()
{
	for (auto& i : PlayerHitBoss)
	{
		i = 0;
	}
	for (auto& i : BossHitPlayer)
	{
		i = 0;
	}
	for (int i = 0; i < maxBossIndex; ++i)
	{
		if (IfBossBulletValid[i] == 1)
			TheScreenDraw->Draw(NowX[i], NowY[i], ' ');
	}
	for (int i = 0; i < maxPlayerIndex; ++i)
	{
		if (IfPlayerBulletValid[i] == 1)
			TheScreenDraw->Draw(NowXPlayer[i], NowYPlayer[i], ' ');
	}
}
void PhyEngS::AfterCollision()
{
	abBoss->BossBeHitted();
	abPlayer->PlayerBeHitted();
}
PhyEngS::PhyEngS(std::shared_ptr<Terr> TheNewTerr, int aPlayerX, int aPlayerY, int aBossX, int aBossY)
	:TheTerr(TheNewTerr), maxBossIndex(64), maxPlayerIndex(64), 
	currentBossIndex(0), currentPlayerIndex(0), BossX(aBossX), BossY(aBossY), PlayerX(aPlayerX), PlayerY(aPlayerY)
{
	TheScreenDraw = std::make_shared<ScreenDraw>(TheTerr->GetWidth(), TheTerr->GetHeight(), TheTerr->GetGrid());
	PlayerHitBoss.resize(maxBossIndex);
	BossHitPlayer.resize(maxPlayerIndex);
	speedx.resize(maxBossIndex);
	speedy.resize(maxBossIndex);
	NowX.resize(maxBossIndex);
	NowY.resize(maxBossIndex);
	IfBossBulletValid.resize(maxBossIndex);
	IfPlayerBulletValid.resize(maxPlayerIndex);
	speedxPlayer.resize(maxPlayerIndex);
	speedyPlayer.resize(maxPlayerIndex);
	NowXPlayer.resize(maxPlayerIndex);
	NowYPlayer.resize(maxPlayerIndex);
	for (int i = 0; i < maxBossIndex; ++i)
	{
		IfBossBulletValid[i] = 0;
	}
	for (int i = 0; i < maxPlayerIndex; ++i)
	{
		IfPlayerBulletValid[i] = 0;
	}
	int Width = TheTerr->GetWidth();
	int Height = TheTerr->GetHeight();
	int* TheGrid = new int[Width * Height];
	for (int i = 0; i < Height; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			TheGrid[i * Width + j] = (*TheTerr->GetGrid())[i][j];
		}
	}
	cudaMalloc((void**)&D_TheGrid, Width * Height * sizeof(int));
	cudaMemcpy(D_TheGrid, TheGrid, Width * Height * sizeof(int), cudaMemcpyHostToDevice);
	cudaMalloc((void**)&D_speedx, maxBossIndex * sizeof(int));
	cudaMalloc((void**)&D_speedy, maxBossIndex * sizeof(int));
	cudaMalloc((void**)&D_NowX, maxBossIndex * sizeof(int));
	cudaMalloc((void**)&D_NowY, maxBossIndex * sizeof(int));
	cudaMalloc((void**)&D_IfBossBulletValid, maxBossIndex * sizeof(int));
	cudaMalloc((void**)&D_speedxPlayer, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_speedyPlayer, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_NowXPlayer, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_NowYPlayer, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_IfPlayerBulletValid, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_PlayerHitBoss, maxBossIndex * sizeof(int));
	cudaMalloc((void**)&D_BossHitPlayer, maxPlayerIndex * sizeof(int));
	delete[] TheGrid;
}
void PhyEngS::Innitialization(std::shared_ptr<Boss> aBoss, std::shared_ptr<Player> aPlayer)
{
	abBoss = aBoss;
	abPlayer = aPlayer;
}
PhyEngS::~PhyEngS()
{
	cudaFree(D_TheGrid);
	cudaFree(D_speedx);
	cudaFree(D_speedy);
	cudaFree(D_NowX);
	cudaFree(D_NowY);
	cudaFree(D_IfBossBulletValid);
	cudaFree(D_speedxPlayer);
	cudaFree(D_speedyPlayer);
	cudaFree(D_NowXPlayer);
	cudaFree(D_NowYPlayer);
	cudaFree(D_IfPlayerBulletValid);
	cudaFree(D_PlayerHitBoss);
	cudaFree(D_BossHitPlayer);
}
void PhyEngS::UpDateBullet()
{
	using namespace std::chrono;
	PrePrepare();
	cudaStream_t stream1, stream2, stream3, stream5,stream6;
	cudaStreamCreate(&stream1);
	cudaStreamCreate(&stream2);
	cudaMemcpyAsync(D_speedx, speedx.data(), maxBossIndex * sizeof(int), cudaMemcpyHostToDevice, stream1);
	cudaMemcpyAsync(D_speedxPlayer, speedxPlayer.data(), maxPlayerIndex * sizeof(int), cudaMemcpyHostToDevice, stream2);
	cudaMemcpyAsync(D_speedy, speedy.data(), maxBossIndex * sizeof(int), cudaMemcpyHostToDevice, stream1);
	cudaMemcpyAsync(D_speedyPlayer, speedyPlayer.data(), maxPlayerIndex * sizeof(int), cudaMemcpyHostToDevice, stream2);
	cudaMemcpyAsync(D_NowX, NowX.data(), maxBossIndex * sizeof(int), cudaMemcpyHostToDevice, stream1);
	cudaMemcpyAsync(D_NowXPlayer, NowXPlayer.data(), maxPlayerIndex * sizeof(int), cudaMemcpyHostToDevice, stream2);
	cudaMemcpyAsync(D_NowY, NowY.data(), maxBossIndex * sizeof(int), cudaMemcpyHostToDevice, stream1);
	cudaMemcpyAsync(D_NowYPlayer, NowYPlayer.data(), maxPlayerIndex * sizeof(int), cudaMemcpyHostToDevice, stream2);
	cudaMemcpyAsync(D_IfBossBulletValid, IfBossBulletValid.data(), maxBossIndex * sizeof(int), cudaMemcpyHostToDevice, stream1);
	cudaMemcpyAsync(D_IfPlayerBulletValid, IfPlayerBulletValid.data(), maxPlayerIndex * sizeof(int), cudaMemcpyHostToDevice, stream2);
	cudaMemcpyAsync(D_PlayerHitBoss, PlayerHitBoss.data(), maxBossIndex * sizeof(int), cudaMemcpyHostToDevice, stream1);
	cudaMemcpyAsync(D_BossHitPlayer, BossHitPlayer.data(), maxPlayerIndex * sizeof(int), cudaMemcpyHostToDevice, stream2);
	cudaStreamSynchronize(stream1);
	cudaStreamSynchronize(stream2);
	cudaStreamDestroy(stream1);
	cudaStreamDestroy(stream2);
	cudaStreamCreate(&stream3);
	int bossBlockSize = 64;
	int bossNumBlocks = (currentBossIndex + bossBlockSize - 1) / bossBlockSize;
	int playerBlockSize = 64;
	int playerNumBlocks = (currentPlayerIndex + playerBlockSize - 1) / playerBlockSize;
	UpdateBulletKernel << <bossNumBlocks, bossBlockSize, 0, stream3 >> > (D_speedx, D_speedy, D_NowX, D_NowY, D_IfBossBulletValid,
		D_TheGrid, TheTerr->GetWidth(), TheTerr->GetHeight(), maxBossIndex, PlayerX, PlayerY, D_BossHitPlayer);
	cudaStreamSynchronize(stream3);
	UpdatePlayerBulletKernel << <playerNumBlocks, playerBlockSize, 0, stream3 >> > (D_speedxPlayer, D_speedyPlayer, D_NowXPlayer, D_NowYPlayer, D_IfPlayerBulletValid,
		D_TheGrid, TheTerr->GetWidth(), TheTerr->GetHeight(), maxPlayerIndex, BossX, BossY, D_PlayerHitBoss, D_NowX, D_NowY, D_IfBossBulletValid, maxBossIndex);
	cudaStreamSynchronize(stream3);
	cudaStreamDestroy(stream3);
	cudaStreamCreate(&stream5);
	cudaStreamCreate(&stream6);
	cudaMemcpyAsync(speedx.data(), D_speedx, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream5);
	cudaMemcpyAsync(speedxPlayer.data(), D_speedxPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream6);
	cudaMemcpyAsync(speedy.data(), D_speedy, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream5);
	cudaMemcpyAsync(speedyPlayer.data(), D_speedyPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream6);
	cudaMemcpyAsync(NowX.data(), D_NowX, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream5);
	cudaMemcpyAsync(NowXPlayer.data(), D_NowXPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream6);
	cudaMemcpyAsync(NowY.data(), D_NowY, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream5);
	cudaMemcpyAsync(NowYPlayer.data(), D_NowYPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream6);
	cudaMemcpyAsync(IfBossBulletValid.data(), D_IfBossBulletValid, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream5);
	cudaMemcpyAsync(IfPlayerBulletValid.data(), D_IfPlayerBulletValid, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream6);
	cudaMemcpyAsync(PlayerHitBoss.data(), D_PlayerHitBoss, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream5);
	cudaMemcpyAsync(BossHitPlayer.data(), D_BossHitPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream6);
	cudaStreamSynchronize(stream5);
	cudaStreamSynchronize(stream6);
	cudaStreamDestroy(stream5);
	cudaStreamDestroy(stream6);
	AfterCollision();
}

void PhyEngS::Draw()
{
	for (int i = 0; i < maxBossIndex; ++i)
	{
		if (IfBossBulletValid[i] == 1)
			TheScreenDraw->Draw(NowX[i], NowY[i], '*');
	}
	for (int i = 0; i < maxPlayerIndex; ++i)
	{
		if (IfPlayerBulletValid[i] == 1)
			TheScreenDraw->Draw(NowXPlayer[i], NowYPlayer[i], 'O');
	}
	TheScreenDraw->Draw(BossX, BossY, BossChar);
	TheScreenDraw->Draw(PlayerX, PlayerY, PlayerChar);
}
int PhyEngS::GetPlayerBeHittedTime()
{
	int j = 0;
	for(int i=0;i<maxBossIndex;++i)
	{
		if (BossHitPlayer[i] == 1)
			++j;
	}
	return j;
}
int PhyEngS::GetBossBeHittedTime()
{
	int j = 0;
	for (int i = 0; i < maxPlayerIndex; ++i)
	{
		if (PlayerHitBoss[i] == 1)
			++j;
	}
	return j;
}
int main()
{
	auto Grid = std::make_shared<Terr>();
	if (Grid->IfCanMove(1, 20) && Grid->IfCanMove(1, 1))
	{
		auto PhyEng = std::make_shared<PhyEngS>(Grid, 1, 20, 1, 1);
		auto Bossphysis = std::make_shared<physis>(PhyEng);
		auto aPlayerphysis = std::make_shared<physis>(PhyEng);
		auto aplayer = std::make_shared<Player>(aPlayerphysis);
		auto aBoss = std::make_shared<Boss>(Bossphysis);
		auto aAINode = std::make_shared<AINode>(aBoss);
		PhyEng->Innitialization(aBoss, aplayer);
		int numnow = 0;
		while (true)
		{
			if (numnow % 3 == 0)
			{
				aAINode->Execute();
			}
			if (numnow % 28 == 1)
				aplayer->PlayerAttack();
			PhyEng->UpDateBullet();
			if (numnow % 180 == 1)
				PhyEng->ReSetBullet();
			PhyEng->Draw();
			PhyEng->TheScreenDraw->Display();
			numnow++;
			if (numnow >= 2048)
				numnow = 0;
			if (aplayer->GetHealth() <= 0)
				break;
			Sleep(100);
		}
	}
}
