extern "C"
{
	void (__stdcall* fChatWindowSay)(char*, char*, int) = reinterpret_cast
		<void (__stdcall*)(char*, char*, int)>
		(0x004C1F20);

	void (__cdecl* fSysWindowSay)(char*, int, int) = reinterpret_cast
		<void (__cdecl*)(char*, int, int)>
		(0x004C1FC0);

	int (__fastcall* fParseCmd)(int, char*) = reinterpret_cast
		<int (__fastcall*)(int, char*)>
		(0x005DD5D0);

	void (__cdecl* fEncryptPacket)(int, int) = reinterpret_cast
		<void (__cdecl*)(int,int)>
		(0x0042C6EC);
	
	int (__cdecl* fSendPacket)() = reinterpret_cast
		<int (__cdecl*)()>
		(0x0042C71B);

	__declspec(dllexport) void SysWindowSay(char* message, int colour)
	{
		__asm {
			PUSH 0
			PUSH colour
			PUSH 0x843A70
			MOV EDI, message
			CALL fSysWindowSay }
	}

	__declspec(dllexport) int SendPacket(BYTE* packet, int psize)
	{
		__asm {
			MOV ESI, DWORD PTR DS:[0x0C5BE14]
			PUSH psize
			MOV EAX, packet
			PUSH EAX
			CALL fEncryptPacket
			MOV ECX, DWORD PTR DS:[0x0C5BE14]
			CALL fSendPacket
			POP ESI
			LEAVE
			RETN
		}
	}
}