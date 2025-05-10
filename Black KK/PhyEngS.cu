#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "PhyEngS.cuh"
#include "physis.h"
#include"Boss.h"
#include"Player.h"
#include"AINode.h"
#include<chrono>
#include<fstream>
#include<thread>
__global__ void UpdateBulletKernel(int* speedx, int* speedy, int* NowX, int* NowY, int* IfBossBulletValid, 
	int* D_TheGrid, int TheWidth, int TheHeight, int currentIndex,const int ThePlayerX,const int ThePlayerY,int*BossHitPlayer,int*PlayerNowX,int *PlayerNowY,int *IfPlayerBulletValid
    ,int maxPlayerIndex)
{
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
		if (NowX[idx] == ThePlayerX && NowY[idx] == ThePlayerY) {
			BossHitPlayer[idx] = 1;
			IfBossBulletValid[idx] = 0;
			return;
		}
		if (D_TheGrid[NowY[idx] * TheWidth + NowX[idx]] == 1)
		{
			IfBossBulletValid[idx] = 0;
			return;
		}
		for(int i=0;i<maxPlayerIndex;++i)
		{
			if (IfPlayerBulletValid[i] == 1 && NowX[idx] == PlayerNowX[i] && NowY[idx] == PlayerNowY[i])
			{
				IfBossBulletValid[idx] = 0;
				return;
		    }
		}
	}
}
__global__ void UpdatePlayerBulletKernel(int* aspeedx, int* aspeedy, int* aNowX, int* aNowY, int* IfPlayerBulletValid, 
	int* D_TheGrid, int TheWidth, int TheHeight, int currentIndex,const int TheBossX,const int TheBossY,int *PlayerHitBoss)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= currentIndex)
		return;
	if (IfPlayerBulletValid[idx]==1)
	{
		aNowX[idx] += aspeedx[idx];
		aNowY[idx] += aspeedy[idx];
		if (aNowX[idx] < 0 || aNowX[idx] >= TheWidth || aNowY[idx] < 0 || aNowY[idx] >= TheHeight)
		{
			IfPlayerBulletValid[idx] = 0;
			return;
		}
        if(aNowX[idx]==TheBossX&&aNowY[idx]==TheBossY)
		{
			IfPlayerBulletValid[idx] = 0;
			PlayerHitBoss[idx] = 1;
			return;
		}
		if (D_TheGrid[aNowY[idx] * TheWidth + aNowX[idx]] == 1)
			IfPlayerBulletValid[idx] = 0;
	}
}
void PhyEngS::PrePrepare()
{
	DealBossqueue();
	DealPlayerqueue();
	DealBossBullet();
	DealPlayerBullet();
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
void PhyEngS::DealBossqueue()
{
	std::lock_guard<std::mutex> Themu(BossMessage);
	if (!Bossqueue.empty())
	{
		auto Top = Bossqueue.front();
		Bossqueue.pop();
		TheScreenDraw->Draw(BossX, BossY, ' ');
		BossX = Top[0];
		BossY = Top[1];
	}
}
void PhyEngS::DealPlayerqueue()
{
	std::lock_guard<std::mutex> Themu(PlayerMessage);
	if(!Playerqueue.empty())
	{
		auto Top = Playerqueue.front();
		Playerqueue.pop();
		TheScreenDraw->Draw(PlayerX, PlayerY, ' ');
		PlayerX = Top[0];
		PlayerY = Top[1];
	}
}
void PhyEngS::DealBossBullet()
{
	BossBulletMu.lock();
	while(!BossBulletqueue.empty())
	{
		auto TheTop = BossBulletqueue.front();
		BossBulletqueue.pop();
		if (IfBossBulletValid[currentBossIndex] == 1)
		{
			currentBossIndex = (currentBossIndex + 1) % maxBossIndex;
			BossBulletMu.unlock();
			return;
		}
		NowX[currentBossIndex] = TheTop[0];
		NowY[currentBossIndex] = TheTop[1];
		speedx[currentBossIndex] = TheTop[2];
		speedy[currentBossIndex] = TheTop[3];
		IfBossBulletValid[currentBossIndex] = 1;
		currentBossIndex = (currentBossIndex + 1)%maxBossIndex;
	}
	BossBulletMu.unlock();
}
void PhyEngS::DealPlayerBullet()
{
	PlayerBulletMu.lock();
	if (PlayerAttackCount != 0)
	{
		PlayerAttackCount = (PlayerAttackCount + 1) %17;
		if (!PlayerBulletqueue.empty())
			PlayerBulletqueue.pop();
		PlayerBulletMu.unlock();
		return;
	}
    if(!PlayerBulletqueue.empty())
	{
		auto TheTop = PlayerBulletqueue.front();
		PlayerBulletqueue.pop();
		if (IfPlayerBulletValid[currentPlayerIndex] == 1) 
		{
			currentPlayerIndex = (currentPlayerIndex + 1) % maxPlayerIndex;
			PlayerBulletMu.unlock();
			return;
		}
		NowXPlayer[currentPlayerIndex] = TheTop[0];
		NowYPlayer[currentPlayerIndex] = TheTop[1];
		speedxPlayer[currentPlayerIndex] = TheTop[2];
		speedyPlayer[currentPlayerIndex] = TheTop[3];
		IfPlayerBulletValid[currentPlayerIndex] = 1;
		currentPlayerIndex = (currentPlayerIndex + 1) % maxPlayerIndex;
		++PlayerAttackCount;
	}
	PlayerBulletMu.unlock();
}
PhyEngS::PhyEngS()
	:maxBossIndex(256), maxPlayerIndex(256), 
	currentBossIndex(0), currentPlayerIndex(0)
{
	TheScreenDraw = std::make_shared<MyScreenDraw>();
	TheTerr = std::make_shared<TerrS>(TheScreenDraw->GetWidth(), TheScreenDraw->GetHeight());
	PlayerX = 0;
	PlayerY = 6;
	BossX = TheScreenDraw->GetWidth() - 1;
	BossY = TheScreenDraw->GetHeight() - 7;
	PlayerHitBoss.resize(maxPlayerIndex);
	BossHitPlayer.resize(maxBossIndex);
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
	for (int i = 0; i < Height; ++i) {
		for(int j=0;j<Width;++j)
		{
			if (!TheTerr->IfCanMove(j, i))
				TheScreenDraw->Draw(j, i, L'#', 0x0007);
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
	cudaMalloc((void**)&D_PlayerHitBoss, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_BossHitPlayer, maxBossIndex * sizeof(int));
	cudaStreamCreate(&stream1);
	cudaStreamCreate(&stream2);
	cudaStreamCreate(&stream3);
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
	cudaStreamDestroy(stream1);
	cudaStreamDestroy(stream2);
	cudaStreamDestroy(stream3);
}
void PhyEngS::UpDateBullet()
{
	Tosy->Wait();
	PrePrepare();
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
	int bossBlockSize =256;
	int bossNumBlocks = (maxBossIndex + bossBlockSize - 1) / bossBlockSize;
	int playerBlockSize =256;
	int playerNumBlocks = (maxPlayerIndex + playerBlockSize - 1) / playerBlockSize;
	UpdateBulletKernel << <bossNumBlocks, bossBlockSize, 0, stream3 >> > (D_speedx, D_speedy, D_NowX, D_NowY, D_IfBossBulletValid,
		D_TheGrid, TheTerr->GetWidth(), TheTerr->GetHeight(), maxBossIndex, PlayerX, PlayerY, D_BossHitPlayer,D_NowXPlayer,D_NowYPlayer,D_IfPlayerBulletValid,maxPlayerIndex);
	cudaStreamSynchronize(stream3);
	UpdatePlayerBulletKernel << <playerNumBlocks, playerBlockSize, 0, stream3 >> > (D_speedxPlayer, D_speedyPlayer, D_NowXPlayer, D_NowYPlayer, D_IfPlayerBulletValid,
		D_TheGrid, TheTerr->GetWidth(), TheTerr->GetHeight(), maxPlayerIndex, BossX, BossY, D_PlayerHitBoss);
	cudaStreamSynchronize(stream3);
	cudaMemcpyAsync(speedx.data(), D_speedx, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream1);
	cudaMemcpyAsync(speedxPlayer.data(), D_speedxPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream2);
	cudaMemcpyAsync(speedy.data(), D_speedy, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream1);
	cudaMemcpyAsync(speedyPlayer.data(), D_speedyPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream2);
	cudaMemcpyAsync(NowX.data(), D_NowX, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream1);
	cudaMemcpyAsync(NowXPlayer.data(), D_NowXPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream2);
	cudaMemcpyAsync(NowY.data(), D_NowY, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream1);
	cudaMemcpyAsync(NowYPlayer.data(), D_NowYPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream2);
	cudaMemcpyAsync(IfBossBulletValid.data(), D_IfBossBulletValid, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream1);
	cudaMemcpyAsync(IfPlayerBulletValid.data(), D_IfPlayerBulletValid, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream2);
	cudaMemcpyAsync(PlayerHitBoss.data(), D_PlayerHitBoss, maxBossIndex * sizeof(int), cudaMemcpyDeviceToHost, stream1);
	cudaMemcpyAsync(BossHitPlayer.data(), D_BossHitPlayer, maxPlayerIndex * sizeof(int), cudaMemcpyDeviceToHost, stream2);
	cudaStreamSynchronize(stream1);
	cudaStreamSynchronize(stream2);
	AfterCollision();
	Draw();
	TheScreenDraw->Render();
}

void PhyEngS::Draw()
{
	std::lock_guard<std::mutex> abs1(BossBulletMu);
	std::lock_guard<std::mutex> abs2(PlayerBulletMu);
	for (int i = 0; i < maxBossIndex; ++i)
	{
		if (IfBossBulletValid[i] == 1)
			TheScreenDraw->Draw(NowX[i], NowY[i], L'*',0x0004);
	}
	for (int i = 0; i < maxPlayerIndex; ++i)
	{
		if (IfPlayerBulletValid[i] == 1)
			TheScreenDraw->Draw(NowXPlayer[i], NowYPlayer[i], 'o',0x0002);
	}
	TheScreenDraw->Draw(BossX, BossY, BossChar,BossColor);
	TheScreenDraw->Draw(PlayerX, PlayerY, PlayerChar,PlayerColor);
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
