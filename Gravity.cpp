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
int abs(int a, int b) {
	return a>b?a-b:b-a;
}
struct Item {
	double x,y,v;
	bool touch(Item a) {
		return abs(this->x,a.x)<=size&&abs(this->y,a.y)<=size;
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
	int W=GetSystemMetrics(SM_CXFULLSCREEN);
	int H=GetSystemMetrics(SM_CYFULLSCREEN);
	double s;
	s=W/Nm;
	for(int i=0;i<Nm;i++) a[i].x=i*s,a[i].y=0,a[i].v=0;
	Item *cur,*che;
	while (true) {
		s=W/Nm;
		for(int i=0;i<Nm;i++) {
			cur=&a[i];
			cur->x+=AL*cur->v/2000*GX;
			cur->y+=AL*cur->v/2000*GY;
			for (int j=0;j<Nm;j++) {
				if (i==j) continue;
				cur=&a[i];
				che=&a[j];
				if (cur->touch(*che)) {
					cur->x-=AL*cur->v/2000*GX,cur->y-=AL*cur->v/2000*GY;
					if (cur->v>che->v) cur->v=-cur->v*JUMP; else cur->v=cur->v*JUMP;
				}
			}
			cur->v+=UP*g/1000;
			if (cur->x+size>W||cur->y+size>H||cur->x<0||cur->y<0) cur->x-=AL*cur->v/2000*GX,cur->y-=AL*cur->v/2000*GY,cur->v=-cur->v*JUMP;
			::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i, MAKELPARAM(cur->x,cur->y));
		}
		ListView_RedrawItems(hwndSysListView32, 0, ListView_GetItemCount(hwndSysListView32) - 1);
		::UpdateWindow(hwndSysListView32);
		Sleep(UP);
		AL+=UP;
	}
	return 0;
}
