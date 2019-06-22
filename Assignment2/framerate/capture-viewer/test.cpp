#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include<iostream>
#include<string>

#define Test "200"
using namespace std;
int main()
{

string arr= string("Hello")+Test;
cout<<arr.length()<<endl;
cout<<arr<<endl;
char array[(arr.length())+1] = "";
array = arr;
cout<<array<<endl;
return 1;
}

