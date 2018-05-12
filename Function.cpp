#include <windows.h>
#include <commctrl.h>
#include <cmath>

int L=200;
int main() {
	HWND hwndParent=::FindWindow("Progman", "Program Manager");
	HWND hwndSHELLDLL_DefView=::FindWindowEx(hwndParent, NULL, "SHELLDLL_DefView", NULL);
	HWND hwndSysListView32=::FindWindowEx(hwndSHELLDLL_DefView, NULL, "SysListView32", "FolderView");
	int Nm = ListView_GetItemCount(hwndSysListView32);
	int ox=0;
	int W=GetSystemMetrics(SM_CXFULLSCREEN);
	int H=GetSystemMetrics(SM_CYFULLSCREEN)/2;
	int of=0;
	int x,y,rx;
	double s;
	while (true) {
		s=W/Nm;
		for(int i=0;i<Nm;i++) {
			x = s*i;
			rx=ox+x;
			y = H-sin(rx)*50;
			::SendMessage(hwndSysListView32, LVM_SETITEMPOSITION, (i+of)%Nm, MAKELPARAM(x,y));
		}
		ListView_RedrawItems(hwndSysListView32, 0, ListView_GetItemCount(hwndSysListView32) - 1);
		::UpdateWindow(hwndSysListView32);
		ox+=1;
		of++;
		of%=Nm;
		Sleep(200);
	}
	return 0;
}
