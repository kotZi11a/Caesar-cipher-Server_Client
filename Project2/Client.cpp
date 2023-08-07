#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <string>

using namespace std;

void shifr(string& text)
{
	for (int i = 0; i < text.length(); i++)
	{
		if ((text[i] >= 'A' && text[i] <= 'Z' - 5) || (text[i] >= 'a' && text[i] <= 'z' - 5))
			text[i] = text[i] + 5;
		else if (text[i] > 'Z' - 5 && text[i] <= 'Z')
			text[i] = 'A' + 4 - ('Z' - text[i]);
		else if (text[i] > 'z' - 5 && text[i] <= 'z')
			text[i] = 'a' + 4 - ('z' - text[i]);
	}
}

int main()
{
	HANDLE pipe;
	string text;

connect:
	WaitNamedPipeA("\\\\.\\pipe\\namePipe", NMPWAIT_WAIT_FOREVER);
	pipe = CreateFileA("\\\\.\\pipe\\namePipe", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (pipe == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == 231)
		{
			cout << "Pipe is open in other process. Wait...\n";
			CloseHandle(pipe);
			goto connect;
		}
		cout << "Error opening pipe\n" << GetLastError();
		_getch();
		return 0;
	}

	cout << "Connect\n";

	while (true)
	{
		getline(cin, text);
		if (text == "--close")
		{
			break;
		}

		shifr(text);
		cout << text << endl;
		char* text_ch = new char(text.length());
		text_ch = (char*)text.c_str();
		int lenght = text.length();
		DWORD byte;
		WriteFile(pipe, text_ch, lenght, &byte, NULL);
		cout << "Byte:" << byte << endl;

		ReadFile(pipe, (char*)text.c_str(), 1024, &byte, NULL);
		text = text.c_str();
		cout << text << endl;
	}

	CloseHandle(pipe);
	cout << "Disconnect";
	_getch();
	return 1;
}