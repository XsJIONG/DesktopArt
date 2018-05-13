#include <windows.h>
#include <commctrl.h>
#include <cmath>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;

const bool AI=true;
const bool MOUSE=false;

const bool Scrict=false;
const double g=9.8;
const int UP=10;
const int BIG=9000;
const double JUMP=1.7;
const int size=50;
const int K=100;
const int SPEED=10;
const int GS=1;

double v=0;
bool an=false;
int center;
bool playing=false;
int tx;
int W,H;
void jump() {
	cout<<"JUMP!!"<<endl;
	v=-JUMP;
}
void* mouse(void* args) {
	bool qwe;
	while (true) {
		qwe=KEY_DOWN(MOUSE_MOVED);
		while (!playing) Sleep(20);
		if (!qwe&&an) an=false; else if (!an&&qwe) {
			an=true;
			jump();
		}
		Sleep(10);
	}
}
int abs(int a, int b) {
	return a>b?a-b:b-a;
}
int abs(int a) {
	return a>0?a:-a;
}
bool ai(int x, int y, int ox, int oy) {
	if (ox<W/2-size/2) {
		if (H/2<y) return true;
		return false;
	}
	if (oy<y) return true;
	return false;
}
int sc=0;
char tmp[500];
int X=0,Y=0;
int main() {
	if (AI) MoveWindow(GetForegroundWindow(),0,0,300,300,true); else ShowWindow(GetForegroundWindow(),false);
	HWND hwndParent=::FindWindow("Progman", "Program Manager");
	HWND hwndSHELLDLL_DefView=::FindWindowEx(hwndParent, NULL, "SHELLDLL_DefView", NULL);
	HWND hwndSysListView32=::FindWindowEx(hwndSHELLDLL_DefView, NULL, "SysListView32", "FolderView");
	int Nm = ListView_GetItemCount(hwndSysListView32);
	if (MOUSE) {
		pthread_t th;
		pthread_create(&th,NULL,mouse,NULL);
		pthread_detach(th);
	}
	W=GetSystemMetrics(SM_CXFULLSCREEN);
	H=GetSystemMetrics(SM_CYFULLSCREEN);
	srand(time(NULL));
	int ox=W/2,oy=H/2;
	center=rand()%(Nm-GS)+1;
	tx=W/4*3;
	double s=(H-K*2)/(Nm-1);
	int lll=Scrict?W/2:W/4;
	playing=true;
		while (true) {
			oy+=BIG*v/2000;
			v+=UP*g/1000;
			if (oy+50>H||oy<0) oy-=BIG*v/2000,v=0;
			for (int i=0;i<center;i++)
				::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i+GS, MAKELPARAM(tx,i*s));
			for (int i=Nm-1;i>center;i--)
				::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i, MAKELPARAM(tx,(i-1)*s+2*K));
			for (int i=0;i<GS;i++)
				::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i, MAKELPARAM(ox,oy));
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
				center=rand()%(Nm-GS)+1;
				playing=true;
				continue;
			}
			tx-=SPEED;
			if (tx<lll) {
				tx=W/4*3;
				center=rand()%(Nm-GS)+1;
				sc++;
				system("cls");
			}
			X=tx-ox;
			Y=s*center+K-oy;
			if (AI) if (ai(ox,oy,tx,s*center+K)) jump();
		}
	return 0;
}
