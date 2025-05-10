#include "PlayerController.h"

PlayerController::PlayerController():FrameCount(0)
{
    // ��ʼ������̨������
    hInput_ = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hInput_, &prevMode_);
    // �����л���ͻ���
    SetConsoleMode(hInput_, ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS);

    // �󶨰���ӳ�䣨���Զ�����չ��
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
        switch (NowAction)
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
    Tosy->Wait();
}
