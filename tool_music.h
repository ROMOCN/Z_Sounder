#ifndef TOOL_MUSIC_H
#define TOOL_MUSIC_H
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <winnt.h>
class Tool_Music
{
public:
    Tool_Music();
    ~Tool_Music();
    void Music_Inital(std::string path);
    void Music_Play();
    void Music_Play_From(long position);
    void Music_Pause();
    void Music_Step_Front();
    void Music_Step_Reverse();
    int Music_Get_Length();
    int Music_Get_Current_Position();
    std::string Music_Get_Current_Info();
    void Music_Set_Voice();//音量
    void Error_Msg(int errorMsg);
    int status = 0; // -1:关闭, 0未开始， 1播放， 2暂停

private:
    int Music_CMD(std::string cmd,char* buff,int buffLen,int value);

};

#endif // TOOL_MUSIC_H
