#include<stdio.h>
#include<windows.h>
#include<conio.h>

#define WinVerMajor()        LOBYTE(LOWORD(GetVersion()))
#define WinVerMinor()        HIBYTE(LOWORD(GetVersion()))
#define IsWinVerNTs()        (GetVersion() < 0x80000000)
#define IsWinVerNT351Plus()  (IsWinVerNTs() && WinVerMajor() >= 3)
#define IsWinVerNT4Plus()    (IsWinVerNTs() && WinVerMajor() > 3)
#define IsWinVer98Plus()     (LOWORD(GetVersion()) != 4)
#define IsWinVerMEPlus()     (WinVerMajor() >= 5 || WinVerMinor() > 10)
#define IsWinVer2000Plus()   (WinVerMajor() >= 5)
#define IsWinVerXPPlus()     (WinVerMajor() >= 5 && LOWORD(GetVersion()) != 5)

#define ROW 32
#define COL 26





void main()
{
	DWORD pid;
	HWND hwnd = FindWindow(0,"���� ã��");
	GetWindowThreadProcessId(hwnd, (DWORD*)&pid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,pid);
	
	DWORD len;
	unsigned char board[COL][ROW];
	unsigned char size_x,size_y,num_mine,change_value;
	unsigned int board_adr,size_x_adr,size_y_adr;   
	int i,j;
	
	if(hProcess == 0)
	{
		printf("���� ã�⸦ ���� ������ �ּ���.\n");
		return;
	}
	
	
	
	
	
	if(IsWinVerXPPlus())                 //  ���� �����찡 xp���� �˻�. 
	{ 
		board_adr=0x01005340;
		size_x_adr=0x01005334;
		size_y_adr=0x01005338;
	}
	else
	{
		board_adr=0x01005700;
		size_x_adr=0x010056f8;
		size_y_adr=0x01005a68;
	}
	
	
	printf("���ڸ� ǥ���Ͻ÷��� �ƹ�Ű�� �����ÿ�.\n");
	getch();
	
	
	while(1)
	{
		memset((void*)board,0,sizeof(board));
		
		
		
		// ���� �迭 �� �о����..  
		
		ReadProcessMemory(hProcess,(void*)board_adr,board,sizeof(board), &len);
		ReadProcessMemory(hProcess,(void*)size_x_adr,&size_x,1, &len);
		ReadProcessMemory(hProcess,(void*)size_y_adr,&size_y,1, &len);
		
		
		for(i=1;i<size_y+1;i++)
		{
			for(j=1;j<size_x+1;j++)
			{
				switch(board[i][j])
				{
				case 0x0E:
					change_value=0x0F;  
					WriteProcessMemory(hProcess,
						
						(LPVOID)(board_adr+i*ROW+j),&change_value,1,NULL);  
					break;
					
					
					
				case 0x8F:case 0x8D:                            // ���� (��ã�� ��)
					change_value=0x8A;  
					WriteProcessMemory(hProcess,
						
						(LPVOID)(board_adr+i*ROW+j),&change_value,1,NULL);  
					break;
				}
			}
		}
		change_value=0;
		InvalidateRect(hwnd,NULL,TRUE);
		printf("��� ���ڰ� ǥ�� �Ǿ����ϴ�. �ٽ� �Ͻ÷��� �ƹ�Ű�� ��������\n");
		getch();
	}
}