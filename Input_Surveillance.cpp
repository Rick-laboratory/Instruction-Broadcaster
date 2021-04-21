#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Input_Surveillance.h"
#include"Broadcast_UDP.h"

//#define _WIN32_WINNT 0x050


Input_Surveillance::Input_Surveillance()
{
    hhkLowLevelKybd  = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    hhLowLevelMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);
}

Input_Surveillance::~Input_Surveillance()
{
    UnhookWindowsHookEx(hhkLowLevelKybd);
}

bool Input_Surveillance::IsInputDone()
{
    return false;
}

LRESULT CALLBACK Input_Surveillance::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)  
{
    BOOL fEatKeystroke = FALSE;
    POINT CursorPos;
    Broadcast_UDP BU_Obj;
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            GetCursorPos(&CursorPos);
            if (fEatKeystroke = (p->vkCode == p->vkCode) && CursorPos.x > 1919)  //redirect a to b
            {
                //std::cout << p->vkCode << std::endl;
                //printf("Hello a\n");

                if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN)) // Keydown
                {
                    std::string message = "1 0 0 " + std::to_string(p->vkCode) + " 0 0 0";
                    BU_Obj.Broadcast(message);

                }
                else if ((wParam == WM_KEYUP) || (wParam == WM_SYSKEYUP)) // Keyup
                {
                    //std::cout << "0x" << p->vkCode << " released" << std::endl;
                    std::string message = "1 0 0 " + std::to_string(p->vkCode) + " 1 0 0";
                    BU_Obj.Broadcast(message);
                }

                break;
            }
            break;
        }
    }
    
    return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

LRESULT Input_Surveillance::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    BOOL fEatKeystroke = FALSE;
    POINT CursorPos;
    Broadcast_UDP BU_Obj;
    INPUT Input = { 0 };
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:

          //MSLLHOOKSTRUCT &p = *(reinterpret_cast<MSLLHOOKSTRUCT*>(lParam));
          MSLLHOOKSTRUCT* mouseInfo = (MSLLHOOKSTRUCT*)lParam;
          GetCursorPos(&CursorPos);
          if (fEatKeystroke = (&mouseInfo == &mouseInfo) && CursorPos.x > 1919)  //redirect a to b
          {
              //std::cout << p->vkCode << std::endl;
              //printf("Hello a\n");

              if (wParam == WM_LBUTTONDOWN) // Keydown
              {
                  std::cout << "WM_LBUTTONDOWN down\n";
                  std::string message = "0 0 0 " + std::to_string(MOUSEEVENTF_LEFTDOWN) + " 0 0 0";
                  BU_Obj.Broadcast(message);

              }
              else if (wParam == WM_LBUTTONUP) // Keyup
              {
                  std::cout << "WM_LBUTTONUP up\n";
                  std::string message = "0 0 0 " + std::to_string(MOUSEEVENTF_LEFTUP) + " 1 0 0";
                  BU_Obj.Broadcast(message);
              }

              if (wParam == WM_RBUTTONDOWN) // Keydown
              {
                  std::cout << "WM_RBUTTONDOWN down\n";
                  std::string message = "0 0 0 " + std::to_string(MOUSEEVENTF_RIGHTDOWN) + " 0 0 0";
                  BU_Obj.Broadcast(message);

              }
              else if (wParam == WM_RBUTTONUP) // Keyup
              {
                  std::cout << "WM_RBUTTONUP up\n";
                  std::string message = "0 0 0 " + std::to_string(MOUSEEVENTF_RIGHTUP) + " 1 0 0";
                  BU_Obj.Broadcast(message);
              }
              if (wParam == WM_MBUTTONDOWN) // Keydown
              {
                  std::cout << "WM_MBUTTONDOWN down\n";
                  std::string message = "0 0 0 " + std::to_string(MOUSEEVENTF_MIDDLEDOWN) + " 0 0";
                  BU_Obj.Broadcast(message);

              }
              else if (wParam == WM_MBUTTONUP) // Keyup
              {
                  std::cout << "WM_MBUTTONUP up\n";
                  std::string message = "0 0 0 " + std::to_string(MOUSEEVENTF_MIDDLEUP) + " 1 0 0";
                  BU_Obj.Broadcast(message);
              }
              if ((short)HIWORD(mouseInfo->mouseData)>0)
              {
                  std::cout << "WHEEL " << (short)HIWORD(mouseInfo->mouseData) << " \n";
                  std::string message = "0 0 0 0 0 0 120";
                  BU_Obj.Broadcast(message);
              }
              else if((short)HIWORD(mouseInfo->mouseData) < 0)
              {
                  std::cout << "WHEEL " << (short)HIWORD(mouseInfo->mouseData) << " \n";
                  std::string message = "0 0 0 0 0 0 -120";
                  BU_Obj.Broadcast(message);
              }
              break;
          }
          break;
        }
    }

    return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}
