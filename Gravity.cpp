#include <windows.h>
#include <commctrl.h>
#include <cmath>
#include <iostream>
using namespace std;

const double g=9.8;
const double JUMP=2.0/3;
const int UP=10;
struct Item {
	double x,y,v;
}a[1000];
unsigned long long AL=0;
int GX=-1,GY=0;
int Mx[]={-1,0,1,0},My[]={0,1,0,-1};
int main() {
	HWND hwndParent=::FindWindow("Progman", "Program Manager");
	HWND hwndSHELLDLL_DefView=::FindWindowEx(hwndParent, NULL, "SHELLDLL_DefView", NULL);
	HWND hwndSysListView32=::FindWindowEx(hwndSHELLDLL_DefView, NULL, "SysListView32", "FolderView");
	int Nm = ListView_GetItemCount(hwndSysListView32);
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
			a[i].v+=UP*g/1000;
			if (a[i].x>W||a[i].y>H||a[i].x<0||a[i].y<0) a[i].v=-a[i].v*JUMP;
			::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i, MAKELPARAM(a[i].x,a[i].y));
		}
		ListView_RedrawItems(hwndSysListView32, 0, ListView_GetItemCount(hwndSysListView32) - 1);
		::UpdateWindow(hwndSysListView32);
		Sleep(UP);
		AL+=UP;
	}
	return 0;
}
