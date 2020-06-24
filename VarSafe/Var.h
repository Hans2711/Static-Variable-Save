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
void AddVar(std::string VarName, std::string VarContent)//Adds or updates a variable
{
    VarContent = encrypt(VarContent);// Encrypts the content
    if (GetVar(VarName) != "Error")//If the Variable name already exists
    {
        std::string OldVar = GetVar(VarName);
        if (OldVar == VarContent)//if the content that already exists are the same it will exit
        {
            return;
        }
        //Replaces the old Variable's content with the new one and writes it to the File
        std::string allVar = getAllVar();
        allVar.replace(allVar.find(OldVar), OldVar.length(), VarContent);
        std::fstream Update(FilePath);
        Update << allVar;
        Update.close();
    }
    else
    {
        //Adds the Variable's name and content in the proper format to the file
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
    for (int i = 0; i < allVar.length(); i++)//Loops though every character of the files Content
    {
        if (allVar[i] == '&' || i == 0)//if the character equals to & or if its the first character
        {
            if(allVar[i] == '&')//if if the character equals to & it adds 1 to i
                i++;
            int EndName = 0;
            bool brk = true;
            for(int j = i; j < allVar.length(); j++)//loops again though the content until it finds an =
            { 
                if (allVar[j] == '=' && brk)
                {
                    EndName = j;//saves then spot in the string where the = is
                    brk = false;
                }
            }
            if (EndName == 0)//If it fails the search for the next = symbol it returns an error
            {
                return "Error";
            }
            std::string Name = "";
            for (int j = i; j < EndName; j++)// Loops though the part of the file content where the Variable name is and stores the characters in a new string
            {
                Name += allVar[j];
            }
            i = EndName + 1;
            brk = true;
            for (int j = i; j < allVar.length(); j++)//Loops though antil it finds the next & symbol and safes the position of it
            {
                if ((allVar[j] == '&' || j == allVar.length() - 1) && brk)
                {
                    EndName = j;
                    brk = false;
                }
            }
            std::string Content = "";
            for (int j = i; j < EndName; j++)//Loops though the part where the Variable's Content is and stores it in a new string
            {
                Content += allVar[j];
            }
            if (Name == VarName)//If the Variable's name equals the one requested in the function's parameters
            {
                return Content;// returns the content of the variable
            }
        }
    }
    return "Error";//Returns an error because no variable with that name got found
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
    //Checks if the File where the Variables are stored exists.
    std::ifstream f(FilePath.c_str());
    if (!f.good())
    {
        //Then creates the file if it doesnt exists
        std::ofstream tmp(FilePath);
        tmp << "";
        tmp.close();
    }
}
