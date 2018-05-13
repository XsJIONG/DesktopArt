#include <windows.h>
#include <commctrl.h>
#include <cmath>
#include <pthread.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

const double g=9.8;
const int UP=10;
const int BIG=5000;
const double JUMP=2.0/3;
unsigned long long AL;
double v=0;
bool an=false;
void* mouse(void* args) {
	bool qwe;
	while (true) {
		qwe=KEY_DOWN(MOUSE_MOVED);
		if (!qwe&&an) an=false; else if (!an&&qwe) {
			an=true;
			v=-3;
		}
		Sleep(10);
	}
}
int main() {
	HWND hwndParent=::FindWindow("Progman", "Program Manager");
	HWND hwndSHELLDLL_DefView=::FindWindowEx(hwndParent, NULL, "SHELLDLL_DefView", NULL);
	HWND hwndSysListView32=::FindWindowEx(hwndSHELLDLL_DefView, NULL, "SysListView32", "FolderView");
	int Nm = ListView_GetItemCount(hwndSysListView32);
	pthread_t th;
	pthread_create(&th,NULL,mouse,NULL);
	pthread_detach(th);
	double s=360/Nm;
	int W=GetSystemMetrics(SM_CXFULLSCREEN);
	int H=GetSystemMetrics(SM_CYFULLSCREEN);
	int ox=W/2,oy=H/2;
	while (true) {
		oy+=BIG*v/2000;
		v+=UP*g/1000;
		if (oy+59>H||oy<0) oy-=BIG*v/2000,v=0;
		::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, 1, MAKELPARAM(ox,oy));
		ListView_RedrawItems(hwndSysListView32, 0, ListView_GetItemCount(hwndSysListView32) - 1);
		::UpdateWindow(hwndSysListView32);
		Sleep(UP);
		AL+=UP;
	}
	return 0;
}
