#include "PlayerController.h"

PlayerController::PlayerController():FrameCount(0)
{
    // 初始化控制台输入句柄
    hInput_ = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hInput_, &prevMode_);
    // 禁用行缓冲和回显
    SetConsoleMode(hInput_, ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS);

    // 绑定按键映射（可自定义扩展）
    KeyBindings = {
        { 'A',  PlayerAction::MoveLeft},
        { 'D', PlayerAction::MoveRight},
        { 'S', PlayerAction::MoveDown},
        {'W',PlayerAction::MoveUp},
        { VK_ESCAPE, PlayerAction::Quit }
    };
}

void PlayerController::GameIng()
{
     auto NowAction = Update();
     if (MessageQue.size() < 5)
     {
         if(NowAction!=PlayerAction::None)
         MessageQue.push(NowAction);
     }
     if (FrameCount % 3 == 1)
     {
         if (!MessageQue.empty())
         {
             auto TheTop = MessageQue.front();
             MessageQue.pop();
             switch (TheTop)
             {
             case PlayerAction::MoveLeft:
                 ControllGoal->MoveLeft();
                 break;
             case PlayerAction::MoveRight:
                 ControllGoal->MoveRight();
                 break;
             case PlayerAction::MoveUp:
                 ControllGoal->MoveUp();
                 break;
             case PlayerAction::MoveDown:
                 ControllGoal->MoveDown();
                 break;
             case PlayerAction::Attack:
                 ControllGoal->PlayerAttack();
                 break;
             case PlayerAction::None:
                 break;
             default:
                 break;
             }
         }
     }
     FrameCount = (FrameCount + 1) % 3;
    Tosy->Wait();
}
