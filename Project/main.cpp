#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <stdio.h>
using namespace std;

enum LineType
{
    otherStatement = 0,
    ifStatement = 1,
    elseStatement = 2,
    whileStatement = 3,
    repeatUntilStatementBegin = 4,
    repeatUntilStatementEnd = 5,
    forStatement = 6,
    braceBeggining = 7,
    braceEnd = 8
} lineType;


LineType getLineType(char codeLine[])
{
    if(strstr(codeLine,"if")) return ifStatement;
    else if(strstr(codeLine,"else")) return elseStatement;
    else if(strstr(codeLine,"while")) return whileStatement;
    else if(strstr(codeLine,"repeat")) return repeatUntilStatementBegin;
    else if(strstr(codeLine,"until")) return repeatUntilStatementEnd;
    else if(strstr(codeLine,"for")) return forStatement;
    else if(strstr(codeLine,"{")) return braceBeggining;
    else if(strstr(codeLine,"}")) return braceEnd;
    else return otherStatement;
}


void analyzeCode(FILE *fptr, char rawCode[])
{
    while(fgets(rawCode,61,fptr)){
        lineType=getLineType(rawCode);
        cout<<lineType<<endl;
    }

}


int main()
{
    char rawCode[61];
    FILE *pseudocode = fopen("input.txt", "r");
    if (!pseudocode)
    {
        printf("Could not open file");
        return -1;
    }
    else
    {
        analyzeCode(pseudocode,rawCode);
    }

    return 0;
}
