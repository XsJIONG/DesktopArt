#include <windows.h>
#include <commctrl.h>
#include <cmath>

const int R=100;
int main() {
	HWND hwndParent=::FindWindow("Progman", "Program Manager");
	HWND hwndSHELLDLL_DefView=::FindWindowEx(hwndParent, NULL, "SHELLDLL_DefView", NULL);
	HWND hwndSysListView32=::FindWindowEx(hwndSHELLDLL_DefView, NULL, "SysListView32", "FolderView");
	int Nm = ListView_GetItemCount(hwndSysListView32);
	double s=360/Nm;
	int ox=R+10,oy=R+10;
	int xx=10,yy=-10;
	int W=GetSystemMetrics(SM_CXFULLSCREEN);
	int H=GetSystemMetrics(SM_CYFULLSCREEN);
	while (true) {
		for(int i=0;i<Nm;i++) {
			int x = ox + R*cos((i*s+ox)*3.1415926/180);
			int y = oy + R*sin((i*s+oy)*3.1415926/180);
			::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, i, MAKELPARAM(x,y));
		}
		ListView_RedrawItems(hwndSysListView32, 0, ListView_GetItemCount(hwndSysListView32) - 1);
		::UpdateWindow(hwndSysListView32);
		ox+=xx, oy+=yy;
		if (ox+R>W||ox<R) {
			ox-=xx;
			xx=-xx;
		}
		if (oy+R>H||oy<R) {
			oy-=yy;
			yy=-yy;
		}
		Sleep(30);
	}
	return 0;
}
