#include <windows.h>
#include <commctrl.h>
#include <cmath>
#include <iostream>
#include <pthread.h>
#include <conio.h>
using namespace std;

const double g=9.8;
const double JUMP=2.0/3.0;
const int UP=10;
const int size=50;
struct Item {
	double x,y,v;
	bool touch(Item a) {
		return x,
	}
}a[1000];
unsigned long long AL=0;
int GX=1,GY=0;
int Mx[]={0,1,0,-1},My[]={-1,0,1,0};
int Nm;
void *read(void* args) {
	int i;
	while (true) {
		i=getch();
		if (i==224) {
			i=getch();
			switch (i) {
				case 72:i=0;break;
				case 77:i=1;break;
				case 80:i=2;break;
				case 75:i=3;break;
			}
			GX=Mx[i],GY=My[i];
			for (int i=0;i<Nm;i++) a[i].v=0;
		}
	}
}
int main() {
	pthread_t th;
	int ret=pthread_create(&th,NULL,read,NULL);
	pthread_detach(th);
	HWND hwndParent=::FindWindow("Progman", "Program Manager");
	HWND hwndSHELLDLL_DefView=::FindWindowEx(hwndParent, NULL, "SHELLDLL_DefView", NULL);
	HWND hwndSysListView32=::FindWindowEx(hwndSHELLDLL_DefView, NULL, "SysListView32", "FolderView");
	Nm = ListView_GetItemCount(hwndSysListView32);
	int ox=0;
	int W=GetSystemMetrics(SM_CXFULLSCREEN)-50;
	int H=GetSystemMetrics(SM_CYFULLSCREEN)-50;
	double s;
	s=W/Nm;
	for(int i=0;i<Nm;i++) a[i].x=i*s,a[i].y=0,a[i].v=0;
	while (true) {
		s=W/Nm;
		for(int i=0;i<Nm;i++) {
			a[i].x+=AL*a[i].v/2000*GX;
			a[i].y+=AL*a[i].v/2000*GY;
			if (a[i].x+size>W||a[i].y+size>H||a[i].x<size||a[i].y<size) a[i].x-=AL*a[i].v/2000*GX,a[i].y-=AL*a[i].v/2000*GY,a[i].v=-a[i].v*JUMP;
			a[i].v+=UP*g/1000;
			for (int j=0;j<Nm;j++) {
				if (i==j) continue;
				
			}
			::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i, MAKELPARAM(a[i].x,a[i].y));
		}
		ListView_RedrawItems(hwndSysListView32, 0, ListView_GetItemCount(hwndSysListView32) - 1);
		::UpdateWindow(hwndSysListView32);
		Sleep(UP);
		AL+=UP;
	}
	return 0;
}
