#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include<iostream>
#include"Cursor_Surveillance.h"
#include"Input_Surveillance.h"
#include"Broadcast_UDP.h"
#include<string>
#include<thread>
#include<ctime>
#include"AES.h"
const unsigned int BLOCK_BYTES_LENGTH = 16 * sizeof(unsigned char);
int main() {
	Cursor_Surveillance* CS_Obj = new Cursor_Surveillance;
	Broadcast_UDP BU_Obj;
	AES aes(256);
	unsigned char key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
	  0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
	unsigned int len = 0;
	int counter = 0;
	int TempX = 0;
	int ServerDesktop = 1919;
	bool control[3] = { false };
	std::thread t([] {
		Input_Surveillance IS_Obj;
		MSG msg;
		while (!GetMessage(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook                                                        
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		});
	t.detach();
	while (true) {
		if (CS_Obj->GetPresentPos().x < ServerDesktop && control[0] != true) {
			std::string message = "3 0 0 0 0 1 0";
			//std::string test = "";
			//unsigned char plain[50];
			//std::copy(message.data(), message.data() + message.length() + 1, plain);
			//unsigned char* encrypted = aes.EncryptECB(plain, BLOCK_BYTES_LENGTH, key, len);
			//test = aes.EncryptECB(plain, BLOCK_BYTES_LENGTH, key, len);
			//std::cout << "ENCRYPTED " << encrypted;
			BU_Obj.Broadcast(message);
			control[0] = !control[0];
		}
		if (CS_Obj->GetPresentPos().x > ServerDesktop && control[0] != false) {
			std::string message = "3 0 0 0 0 2 0";
			BU_Obj.Broadcast(message);
			control[0] = !control[0];
		}
		if (CS_Obj->IsCursorMoved() && CS_Obj->GetPresentPos().x > 1919) {
			if (CS_Obj->GetPresentPos().x == 3839 && 1919 + TempX != 2560) {
				SetCursorPos(3839 - 100, CS_Obj->GetPresentPos().y);
				//std::cout << "COUNTER:" << counter << " TempX:" << TempX << "\n";
				TempX += 100;
				counter++;
			}
			if (CS_Obj->GetPresentPos().x < 2000 && counter != 0) {
				SetCursorPos(1919 + TempX, CS_Obj->GetPresentPos().y);
				//std::cout << "COUNTER:" << counter << " TempX:" << TempX << "\n";
				TempX -= TempX;
				counter = 0;
			}
			std::string message = "2 " + std::to_string(CS_Obj->GetPresentPos().x - ServerDesktop + TempX) + " " + std::to_string(CS_Obj->GetPresentPos().y)+" 0"+" 0 0 0";
			BU_Obj.Broadcast(message);
			//std::cout << std::dec << CS_Obj->GetPresentPos().x << " " << CS_Obj->GetPresentPos().y << "\n";
		}
	}
	delete CS_Obj;
	return 0;
}