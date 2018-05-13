#include <windows.h>
#include <commctrl.h>
#include <cmath>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;

const double g=9.8;
const int UP=10;
const int BIG=9000;
const double JUMP=1.7;
const int size=50;
const int K=100;
const int SPEED=6;

double v=0;
bool an=false;
int center;
bool playing=false;
int tx;
void* mouse(void* args) {
	bool qwe;
	while (true) {
		qwe=KEY_DOWN(MOUSE_MOVED);
		while (!playing) Sleep(20);
		if (!qwe&&an) an=false; else if (!an&&qwe) {
			an=true;
			v=-JUMP;
		}
		Sleep(10);
	}
}
int abs(int a, int b) {
	return a>b?a-b:b-a;
}
int sc=0;
char tmp[500];
int main() {
	//ShowWindow(GetForegroundWindow(),false);
	MoveWindow(GetForegroundWindow(),0,0,300,300,true);
	HWND hwndParent=::FindWindow("Progman", "Program Manager");
	HWND hwndSHELLDLL_DefView=::FindWindowEx(hwndParent, NULL, "SHELLDLL_DefView", NULL);
	HWND hwndSysListView32=::FindWindowEx(hwndSHELLDLL_DefView, NULL, "SysListView32", "FolderView");
	int Nm = ListView_GetItemCount(hwndSysListView32);
	pthread_t th;
	pthread_create(&th,NULL,mouse,NULL);
	pthread_detach(th);
	int W=GetSystemMetrics(SM_CXFULLSCREEN);
	int H=GetSystemMetrics(SM_CYFULLSCREEN);
	srand(time(NULL));
	while (true) {
	int ox=W/2,oy=H/2;
	center=rand()%(Nm-1)+1;
	tx=W/4*3;
	double s=(H-K*2)/(Nm-1);
	playing=true;
	while (true) {
			oy+=BIG*v/2000;
			v+=UP*g/1000;
			if (oy+50>H||oy<0) oy-=BIG*v/2000,v=0;
			for (int i=1;i<=center;i++)
				::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i, MAKELPARAM(tx,(i-1)*s));
			for (int i=Nm-1;i>center;i--)
				::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i, MAKELPARAM(tx,(i-1)*s+2*K));
			::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, 0, MAKELPARAM(ox,oy));
			ListView_RedrawItems(hwndSysListView32, 0, ListView_GetItemCount(hwndSysListView32) - 1);
			::UpdateWindow(hwndSysListView32);
			Sleep(UP);
			if (abs(ox,tx)<size&&(oy<=s*center||oy>=s*center+2*K-size)) {
				playing=false;
				sprintf(tmp,"您太弱了，只得了%d分",sc);
				int ret=MessageBox(GetForegroundWindow(),tmp,"大佬",2);
				if (ret==3) return 0;
				tx=W/4*3;
				ox=W/2,oy=H/2;
				sc=0;
				center=rand()%(Nm-1)+1;
				playing=true;
				continue;
			}
			tx-=SPEED;
			if (tx<W/4) {
				tx=W/4*3;
				center=rand()%(Nm-1)+1;
				sc++;
			}
			cout<<"X:"<<tx-ox<<" Y:"<<s*center+K-oy<<endl;
		}
	}
	return 0;
}
