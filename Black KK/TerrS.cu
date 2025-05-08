#include"TerrS.cuh"
#include"ScreenDraw.h"
#include<Windows.h>
#include<iostream>
__constant__ int d_perm[512];
__constant__ float d_quan[256];
__global__ void perlinNoiseKernel(float* noise, int width, float freq) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    if (x >= width) 
        return;
    float xf = x / freq;
    int xi0 = (static_cast<int>(xf) & 255);
    int xi1 = (xi0 + 1) & 255;
    float tx = xf - static_cast<int>(xf);
    float u = tx * tx * tx * (tx * (tx * 6 - 15) + 10);
    int FirstIndex = d_perm[xi0];
    int SecondIndex = d_perm[xi1];
    float noisenum = d_quan[FirstIndex];
    float Subnum = d_quan[SecondIndex] - d_quan[FirstIndex];
    noisenum += Subnum * u;
    noise[x] = noisenum;
}
void TerrS::initThep()
{
    const int PerSize = 256;
    std::vector<int> perm(PerSize);
    float gradients[PerSize];
    std::srand(std::time(0));
    for (int i = 0; i < PerSize;++i) {
        perm[i] = i % PerSize;
    }
    int UpMax = 850;
    int LowDown = 400;
    int countnum1 = 0;
    int countnum2 = 0;
    bool Can1 = true, Can2 = true;
    int TheMedium = Width / Freq;
    TheMedium /= 2;
    for (int medium=TheMedium,medium2=TheMedium+1; Can1||Can2;--medium,++medium2) 
    {
        if (medium < 0)
            Can1 = false;
        if (medium2 >= PerSize)
            Can2 = false;
        if(countnum1>5)
        {
            countnum1 = 0;
            if(LowDown>=100)
                LowDown -= 100;
        }
        if(countnum2>11)
        {
            countnum2 = 0;
            if (UpMax > 100)
                UpMax -= 100;
        }
        int TheSub = UpMax - LowDown;
        if (Can1)
        {
            float RandNum = rand() % TheSub + LowDown;
            RandNum /= 1000.0;
            gradients[medium] = RandNum;
        }
        if (Can2)
        {
            float RandNum2 = rand() % TheSub + LowDown;
            RandNum2 /= 1000.0;
            gradients[medium2] = RandNum2;
        }
        ++countnum1;
        ++countnum2;
    }
    cudaMemcpyToSymbol(d_perm, perm.data(), 2 * PerSize * sizeof(int));
    cudaMemcpyToSymbol(d_quan, gradients, PerSize * sizeof(float));
}
void TerrS::SpawnTerr(std::vector<float> vectorNoise)
{
    int PanDuan = 0;//用于判断从两边还是从中间扩散，0为中间，1为两边
    int CurrentLie1 = Width / 2;
    int CurrentLie2 = CurrentLie1 + 1;
    float HeightTemp = Height / 4;
    int Hang1 = HeightTemp *2;
    int Hang2 = HeightTemp * 3;
    int HangSum = Hang2 - Hang1;
    bool Can1 = true, Can2 = true;
    int countnum1 = 0;
    while(Can1||Can2)
    {
       if(PanDuan==0)
       {
           if (countnum1 > Width/10) 
           {
               countnum1 = 0;
               PanDuan =(PanDuan+1)%3;
               continue;
           }
           if (Can1)
           {
               int RandHang =0;
               int IncreaseWay = 0;
               int useTemp = rand() % 4;
               if(useTemp<=1)
               {
                   RandHang = Hang1 - rand() % HangSum/4*3;
                   IncreaseWay = 1;
               }
               else
               {
                   RandHang=Hang2- rand() % HangSum;
                   IncreaseWay = -1;
               }
               float Temp = HeightTemp * vectorNoise[CurrentLie1];
               int MaxBarr1 = static_cast<int>(Temp);
               for (int i1 = RandHang;i1>=0&&i1<Height&&MaxBarr1>0;i1+=IncreaseWay)
               {
                   (*TheTerrS)[i1][CurrentLie1] = 1;
                   MaxBarr1 -= 1;
               }
           }
           if(Can2)
           {
               int RandHang = 0;
               int IncreaseWay = 0;
               int useTemp = rand() % 4;
               if (useTemp <= 1)
               {
                   RandHang = Hang1 - rand() % HangSum/4*3;
                   IncreaseWay = 1;
               }
               else
               {
                   RandHang = Hang2 - rand() % HangSum;
                   IncreaseWay = -1;
               }
               float Temp = HeightTemp * vectorNoise[CurrentLie2];
               int MaxBarr2 = static_cast<int>(Temp);
               for (int i1 = RandHang; i1>=0&&i1<Height&&MaxBarr2>0;i1+=IncreaseWay)
               {
                   (*TheTerrS)[i1][CurrentLie2] = 1;
                   MaxBarr2 -= 1;
               }
           }
       }
       else if(PanDuan==1)
       {
           if (countnum1 > Width/8)
           {
               countnum1 = 0;
               PanDuan=(PanDuan + 1) % 3;
               continue;
           }
           if(Can1)
           {
               int RandPanduan = rand() % 4;
               int RanHang = 0;
               int IncreaseWay = 0;
               if (RandPanduan <= 1)
               {
                   RanHang = 0;
                   IncreaseWay = 1;
               }
               else
               {
                   RanHang = rand() % HangSum + Hang2;
                   IncreaseWay = 1;
               }
               float Temp = HeightTemp * vectorNoise[CurrentLie1];
               int MaxBarr1 = static_cast<int>(Temp);
               for(int i1=RanHang;i1>=0&&i1<Height&&MaxBarr1>0;i1+=IncreaseWay)
               {
                   (*TheTerrS)[i1][CurrentLie1] = 1;
                   --MaxBarr1;
               }
           }
           if(Can2)
           {
               int RandPanduan = rand() % 4;
               int RanHang = 0;
               int IncreaseWay = 0;
               if (RandPanduan <= 1)
               {
                   RanHang = rand() % HangSum;
                   IncreaseWay = 1;
               }
               else
               {
                   RanHang = rand() % HangSum + Hang2;
                   IncreaseWay = 1;
               }
               float Temp = HeightTemp * vectorNoise[CurrentLie2];
               int MaxBarr2 = static_cast<int>(Temp);
               for (int i1 = RanHang; i1 >= 0 && i1 < Height && MaxBarr2>0; i1 += IncreaseWay)
               {
                   (*TheTerrS)[i1][CurrentLie2] = 1;
                   --MaxBarr2;
               }
           }
       }
       else 
       {
           if (countnum1 > Width / 15)
           {
               countnum1 = 0;
               PanDuan = (PanDuan + 1) % 3;
               continue;
           }
       }
       --CurrentLie1;
       if (CurrentLie1 < 0)
           Can1 = false;
       ++CurrentLie2;
       if (CurrentLie2 >= Width)
           Can2 = false;
       ++countnum1;
    }
}
TerrS::TerrS(int wid,int hei):Width(wid),Height(hei),Freq(5)
{
    TheTerrS = std::make_shared<std::vector<std::vector<int>>>(Height, std::vector<int>(Width, 0));
    NoiseNum = std::vector<float>(Width, 0.0);
    float* d_noise;
    cudaMalloc(&d_noise, Width*sizeof(float));
    initThep();
    int threadsPerBlock = 64;
    int numBlocks = (Width + threadsPerBlock - 1) / threadsPerBlock;
    perlinNoiseKernel << <numBlocks, threadsPerBlock>> > (d_noise, Width,Freq); 
        float* Thenoise = new float[Width];
    cudaMemcpy(Thenoise, d_noise, Width*sizeof(float), cudaMemcpyDeviceToHost);
    {
        std::vector<float> vectorNoise;
        for (int i = 0; i < Width; ++i)
        {
            vectorNoise.push_back(Thenoise[i]);
        }
        SpawnTerr(vectorNoise);
    }
    for(int i=0;i<Height;++i)
    {
        (*TheTerrS)[i][0] = 0;
        (*TheTerrS)[i][Width - 1] = 0;
    }
    delete []Thenoise;
    cudaFree(d_noise);
}

TerrS::~TerrS()
{
}


