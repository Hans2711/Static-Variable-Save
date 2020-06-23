#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <sstream>

std::string CryptKey;
std::string FilePath;
std::string encrypt(std::string msg)//Simple Encryption
{
    if (!CryptKey.size())
        return msg;
    for (std::string::size_type i = 0; i < msg.size(); ++i)
        msg[i] ^= CryptKey[i % CryptKey.size()];
    return msg;
}
std::string decrypt(std::string const& msg)
{
    return encrypt(msg);
}

std::string getAllVar()//Reads the File into a string
{
    std::ifstream inFile;
    inFile.open(FilePath);
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    std::string str = strStream.str();
    return str;
}
std::string GetVar(std::string VarName);
void AddVar(std::string VarName, std::string VarContent)//Adds ur updates a variable
{
    VarContent = encrypt(VarContent);
    if (GetVar(VarName) != "Error")
    {
        std::string OldVar = GetVar(VarName);
        if (OldVar == VarContent)
        {
            return;
        }
        std::string allVar = getAllVar();
        allVar.replace(allVar.find(OldVar), OldVar.length(), VarContent);
        std::fstream Update(FilePath);
        Update << allVar;
        Update.close();
    }
    else
    {
        std::string Togstr = VarName;
        Togstr += "=" + VarContent + "&";
        std::string allVar = getAllVar();
        allVar += Togstr;
        std::fstream Update(FilePath);
        Update << allVar;
        Update.close();
    }
}
std::string GetVar(std::string VarName)//Gets the value of a variable but encrypted
{
    std::string allVar = getAllVar();
    for (int i = 0; i < allVar.length(); i++)
    {
        if (allVar[i] == '&' || i == 0)
        {
            if(allVar[i] == '&')
                i++;
            int EndName = 0;
            bool brk = true;
            for(int j = i; j < allVar.length(); j++)
            { 
                if (allVar[j] == '=' && brk)
                {
                    EndName = j;
                    brk = false;
                }
            }
            if (EndName == 0)
            {
                return "Error";
            }
            std::string Name = "";
            for (int j = i; j < EndName; j++)
            {
                Name += allVar[j];
            }
            i = EndName + 1;
            brk = true;
            for (int j = i; j < allVar.length(); j++)
            {
                if ((allVar[j] == '&' || j == allVar.length() - 1) && brk)
                {
                    EndName = j;
                    brk = false;
                }
            }
            std::string Content = "";
            for (int j = i; j < EndName; j++)
            {
                Content += allVar[j];
            }
            if (Name == VarName)
            {
                return Content;
            }
        }
    }
    return "Error";
}
void Start(std::string EncryptionKey)//Starts the session by setting stuff like the current directory etc...
{
    CryptKey = EncryptionKey;
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string tmp = std::string(buffer).substr(0, pos);
    tmp += "\\Var";
    FilePath = tmp;
    std::ifstream f(FilePath.c_str());
    if (!f.good())
    {
        std::ofstream tmp(FilePath);
        tmp << "";
        tmp.close();
    }
}
