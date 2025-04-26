#include "PhyEngS.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<chrono>
__global__ void UpdateBulletKernel(int* speedx, int* speedy, int* NowX, int* NowY, bool* IfBossBulletValid, int* D_TheGrid, int TheWidth, int TheHeight, int currentIndex)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx < currentIndex)
	{
		if (IfBossBulletValid[idx]==true)
		{
			NowX[idx] += speedx[idx];
			NowY[idx] += speedy[idx];
			if (NowX[idx] < 0 || NowX[idx] >= TheWidth || NowY[idx] < 0 || NowY[idx] >= TheHeight)
			{
				IfBossBulletValid[idx] = false;
			}
			if (D_TheGrid[NowY[idx] * TheWidth + NowX[idx]] == 1)
			{
				IfBossBulletValid[idx] = false;
			}
		}
	}
}
__global__ void UpdatePlayerBulletKernel(int* speedx, int* speedy, int* NowX, int* NowY, bool* IfPlayerBulletValid, int* D_TheGrid, int TheWidth, int TheHeight, int currentIndex)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx < currentIndex)
	{
		if (IfPlayerBulletValid[idx])
		{
			NowX[idx] += speedx[idx];
			NowY[idx] += speedy[idx];
			if (D_TheGrid[NowY[idx] * TheWidth + NowX[idx]] == 1)
			{
				IfPlayerBulletValid[idx] = false;
			}
		}
	}
}
PhyEngS::PhyEngS(std::shared_ptr<Terr> TheNewTerr) :TheTerr(TheNewTerr), maxBossIndex(64), maxPlayerIndex(64), currentBossIndex(0), currentPlayerIndex(0)
{
	TheScreenDraw = std::make_shared<ScreenDraw>(TheTerr->GetWidth(), TheTerr->GetHeight(), TheTerr->GetGrid());
	BossBullet.resize(maxBossIndex);
	PlayerBullet.resize(maxPlayerIndex);
	speedx.resize(maxBossIndex);
	speedy.resize(maxBossIndex);
	NowX.resize(maxBossIndex);
	NowY.resize(maxBossIndex);
	IfBossBulletValid.resize(maxBossIndex);
	IfPlayerBulletValid.resize(maxPlayerIndex);
	for (int i = 0; i < maxBossIndex; ++i)
	{
		IfBossBulletValid[i] = false;
	}
	for (int i = 0; i < maxPlayerIndex; ++i)
	{
		IfPlayerBulletValid[i] = false;
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
	cudaMalloc((void**)&D_IfBossBulletValid, maxBossIndex * sizeof(bool));
	cudaMalloc((void**)&D_speedxPlayer, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_speedyPlayer, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_NowXPlayer, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_NowYPlayer, maxPlayerIndex * sizeof(int));
	cudaMalloc((void**)&D_IfPlayerBulletValid, maxPlayerIndex * sizeof(bool));
	delete[] TheGrid;
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
}
void PhyEngS::UpDateBullet()
{
	for (int i = 0; i < maxBossIndex; ++i)
	{
		if (IfBossBulletValid[i])
			TheScreenDraw->Draw(NowX[i], NowY[i], ' ');
	}
	bool* H_IfPlayerBulletValid = new bool[maxPlayerIndex];
	for (auto i : IfPlayerBulletValid)
	{
		H_IfPlayerBulletValid[i] = i;
	}
	bool* H_IfBossBulletValid = new bool[maxBossIndex];
	for (auto i : IfBossBulletValid)
	{
		H_IfBossBulletValid[i] = i;
	}
	cudaStream_t stream1, stream2, stream3, stream4, stream5, stream6;
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
	cudaMemcpyAsync(D_IfBossBulletValid, H_IfBossBulletValid, maxBossIndex * sizeof(bool), cudaMemcpyHostToDevice, stream1);
	cudaMemcpyAsync(D_IfPlayerBulletValid, H_IfPlayerBulletValid, maxPlayerIndex * sizeof(bool), cudaMemcpyHostToDevice, stream2);
	cudaStreamSynchronize(stream1);
	cudaStreamSynchronize(stream2);
	cudaStreamDestroy(stream1);
	cudaStreamDestroy(stream2);
	cudaStreamCreate(&stream3);
	cudaStreamCreate(&stream4);
	int blockSize = 256;
	int numBlocks = (currentBossIndex + blockSize - 1) / blockSize;
	UpdateBulletKernel << <numBlocks, blockSize, 0, stream3 >> > (D_speedx, D_speedy, D_NowX, D_NowY, D_IfBossBulletValid, D_TheGrid, TheTerr->GetWidth(), TheTerr->GetHeight(), currentBossIndex);
	UpdatePlayerBulletKernel << <numBlocks, blockSize, 0, stream4 >> > (D_speedxPlayer, D_speedyPlayer, D_NowXPlayer, D_NowYPlayer, D_IfPlayerBulletValid, D_TheGrid, TheTerr->GetWidth(), TheTerr->GetHeight(), currentPlayerIndex);
	cudaStreamSynchronize(stream3);
	cudaStreamSynchronize(stream4);
	cudaStreamDestroy(stream3);
	cudaStreamDestroy(stream4);
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
	cudaMemcpyAsync(H_IfBossBulletValid, D_IfBossBulletValid, maxBossIndex * sizeof(bool), cudaMemcpyDeviceToHost, stream5);
	cudaMemcpyAsync(H_IfPlayerBulletValid, D_IfPlayerBulletValid, maxPlayerIndex * sizeof(bool), cudaMemcpyDeviceToHost, stream6);
	cudaStreamSynchronize(stream5);
	cudaStreamSynchronize(stream6);
	cudaStreamDestroy(stream5);
	cudaStreamDestroy(stream6);
	for (int i = 0; i < maxBossIndex; ++i)
	{
		IfBossBulletValid[i] = H_IfBossBulletValid[i];
	}
	for (int i = 0; i < maxPlayerIndex; ++i)
	{
		IfPlayerBulletValid[i] = H_IfPlayerBulletValid[i];
	}
	delete[] H_IfBossBulletValid;
	delete[] H_IfPlayerBulletValid;
	for (int i = 0; i < maxBossIndex; ++i)
	{
		if (IfBossBulletValid[i])
			TheScreenDraw->Draw(NowX[i], NowY[i], 'B');
	}
	TheScreenDraw->Display();
}
int main()
{
	std::shared_ptr<Terr> TheTerr = std::make_shared<Terr>();
	std::shared_ptr<PhyEngS> ThePhyEng = std::make_shared<PhyEngS>(TheTerr);
	ThePhyEng->spawnBossBullet(0, 16, 1, 1);
	ThePhyEng->spawnBossBullet(18, 0, 1, 1);
	ThePhyEng->spawnBossBullet(0, 0, 1, 1);
	ThePhyEng->spawnBossBullet(24, 0, 1, 1);
	ThePhyEng->spawnBossBullet(17, 0, 1, 1);
	while (true)
	{
		ThePhyEng->UpDateBullet();
		Sleep(100);
	}
}