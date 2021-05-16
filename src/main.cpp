#include <stdio.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ShellController.h"

int main()
{
    auto controller = new ShellController();
    while(true)
    {
        controller->readCommand();
    }
}
