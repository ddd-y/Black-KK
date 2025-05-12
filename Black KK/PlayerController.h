#pragma once
#include<memory>
#include<unordered_map>
#include<windows.h>
#include<queue>
#include"MyBarrier.h"
#include"Player.h"
enum class PlayerAction {
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Attack,
    Quit,
    None
};
class PlayerController
{
public:
    PlayerController();
    void SetControllGoal(std::shared_ptr<Player> abb) { ControllGoal = abb; }
    ~PlayerController() {
        SetConsoleMode(hInput_, prevMode_); // �ָ�����̨ģʽ
    }
    // ��������ȡ���벢���ص�ǰ֡�Ķ���
    void GameIng();
    void SetBarrier(std::shared_ptr<MyBarrier> abb) { Tosy = abb; }
private:
    std::queue<PlayerAction> MessageQue;
    std::shared_ptr<Player> ControllGoal;
    std::shared_ptr<MyBarrier> Tosy;
    int FrameCount;
    void ProcessInputEvents(INPUT_RECORD* events, DWORD count) {
        currentAction_ = PlayerAction::None; // ���ö���
        for (DWORD i = 0; i < count; ++i) {
            if (events[i].EventType == KEY_EVENT) {
                const auto& ker = events[i].Event.KeyEvent;
                if (ker.wVirtualKeyCode == VK_SPACE && !ker.bKeyDown)
                {
                    currentAction_ = PlayerAction::Attack;
                    return;
                }
                if (ker.bKeyDown) {
                    // ���Ұ�����
                    auto it = KeyBindings.find(ker.wVirtualKeyCode);
                    if (it != KeyBindings.end()) {
                        currentAction_ = it->second;
                    }
                }
            }
        }
    }
    PlayerAction Update() {
        INPUT_RECORD inputBuffer[32];
        DWORD eventsRead = 0;
        PeekConsoleInput(hInput_, inputBuffer, 32, &eventsRead);
        if (eventsRead > 0) {
            ReadConsoleInput(hInput_, inputBuffer, 32, &eventsRead);
            ProcessInputEvents(inputBuffer, eventsRead);
        }
        PlayerAction action = currentAction_;
        currentAction_ = PlayerAction::None;
        return action;
    }
    HANDLE hInput_;
    DWORD prevMode_;
    PlayerAction currentAction_ = PlayerAction::None;
    std::unordered_map<WORD, PlayerAction> KeyBindings; // ������뵽������ӳ��
};

