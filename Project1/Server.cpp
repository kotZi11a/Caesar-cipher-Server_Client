#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

void deshifr(string& text)
{
	for (int i = 0; i < text.length(); i++)
	{
		if ((text[i] >= 'A' + 5 && text[i] <= 'Z') || (text[i] >= 'a' + 5 && text[i] <= 'z'))
			text[i] = text[i] - 5;
		else if (text[i] < 'A' + 5 && text[i] >= 'A')
			text[i] = 'Z' - 4 + (text[i] - 'A');
		else if (text[i] < 'a' + 5 && text[i] >= 'a')
			text[i] = 'z' - 4 + (text[i] - 'a');
	}
}


int main()
{
	HANDLE pipe;
	pipe = CreateNamedPipeA(
		"\\\\.\\pipe\\namePipe",
		PIPE_ACCESS_DUPLEX | PIPE_WAIT,
		PIPE_TYPE_MESSAGE,
		10,
		1024,
		1024,
		50,
		NULL);

	if (pipe == INVALID_HANDLE_VALUE)
	{
		cout << "Error create pipe\n" << GetLastError();
		_getch();
		return 0;
	}

	while (true)
	{
		if (ConnectNamedPipe(pipe, NULL))
		{
			
			wchar_t* name = new wchar_t(1024);
			GetNamedPipeClientComputerNameW(pipe, name, 1024);
			cout << "Connect: " << name << endl;

			string text;
			DWORD byte;
			while(true)
			{
				string text;
				int ret_code = ReadFile(pipe, (char*)text.c_str(), 1024, &byte, NULL);

				if (ret_code == 0 && GetLastError() == ERROR_BROKEN_PIPE)
				{
					break;
				}
				text = text.c_str();
				cout << "Readed\n" << text << endl;
				deshifr(text);
				cout << text << endl;

				char* text_ch = new char(text.length());
				text_ch = (char*)text.c_str();
				WriteFile(pipe, text_ch, text.length(), &byte, NULL);
			}
			if (DisconnectNamedPipe(pipe))
			{
				cout << "\nDisconnect" << name << "\n";
			}
		}
	}
	CloseHandle(pipe);

}