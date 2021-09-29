#include "tool_music.h"

Tool_Music::Tool_Music()
{

}

Tool_Music::~Tool_Music()
{

}

void Tool_Music::Music_Inital(std::string path)
{
    std::string head = "open ";
    std::string name = " alias MUSIC";
    std::string cmd = head + path + name;
    std::string str = "open D:\\ALLproject\\QT\\Z_Sounder\\music\\定风波.mp3  alias MUSIC";
//    int ato = mciSendString(TEXT("open D:\\ALLproject\\QT\\Z_Sounder\\music\\关大洲-定风波.mp3  alias MUSIC") , NULL, 0, NULL);
    int ato = Music_CMD("open D:\\ALLproject\\QT\\Z_Sounder\\music\\music.mp3  alias MUSIC" , NULL, 0, NULL);

    int i = ato;

    char *buff;
    WCHAR *Tbuff = TEXT("buff");
}

void Tool_Music::Music_Play()
{
    if(status == 0)
    {
        status = 1;
//        mciSendString(TEXT("PLAY MUSIC FROM 0"), NULL, 0, NULL);
        Music_CMD("PLAY MUSIC FROM 0", NULL, 0, NULL);
    }
    else if(status == 2)
    {
        status = 1;
//        mciSendString(TEXT("resume MUSIC"),0,0,0);
        Music_CMD("resume MUSIC", NULL, 0, NULL);

    }
}

void Tool_Music::Music_Play_From(long position)
{
      char* cmd = NULL;
      sprintf(cmd, "seek MUSIC to %d", position);
      Music_CMD(cmd ,0 ,0 ,0 );
}

void Tool_Music::Music_Pause()
{
    if(status == 1)
    {
        status = 2;
        mciSendString(TEXT("pause MUSIC"),0,0,0);
        Music_CMD("pause MUSIC",0,0,0);
    }

}

int Tool_Music::Music_Get_Length()
{
    char st[100];
    std::string cmd = "status MUSIC length";
    //mciSendStringA(cmd.c_str(), st, 100, NULL);
    Music_CMD(cmd.c_str(),st,100,0);

    int len = strlen(st);
    int length = std::atoi(st);
    return length;

}

int Tool_Music::Music_Get_Current_Position()
{
    char st[100];
    std::string cmd = "status MUSIC position";
    //mciSendStringA(cmd.c_str(), st, 100, 0);
    Music_CMD(cmd.c_str(), st, 100, 0);
    int length = std::atoi(st);
    return length;

}

std::string Tool_Music::Music_Get_Current_Info()
{
    char st[20];
    std::string cmd = "status MUSIC mode";
    //mciSendStringA(cmd.c_str(), st, 20, 0);
    Music_CMD(cmd.c_str(), st, 20, 0);
    std::string data = st;
    if(data.substr(0,7) == "stopped")
    {
        status = 0;
    }
    return st;
}


void Tool_Music::Error_Msg(int errorMsg)
{
        MCIERROR mciError = errorMsg;
        char buf[50];
        if(errorMsg)
        {
          mciGetErrorStringA(mciError,buf,strlen(buf));
        }
}

int Tool_Music::Music_CMD(std::string cmd,char* buff,int buffLen,int value = NULL)
{
   int result = mciSendStringA(cmd.c_str(), buff, buffLen, NULL);
//    int result = mciSendString(TEXT(cmd.c_str()), buff, buffLen, NULL);
    return result;
}

////发送信息给MCI
//bool SendToMCI(std::string command)
//{
//	if(!mciSendString(TEXT(command.c_str()),NULL,0,0))
//		return true;
//	return false;
//}

////从MCI获取信息
//std::string GetFromMCI(std::string command)
//{
//	char message[20];
//	mciSendString(command.c_str(),message,20,0);
//	std::string str(message);
//	return str;
//}
