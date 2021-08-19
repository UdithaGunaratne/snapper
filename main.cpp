#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <algorithm>
#include "snap.hpp"

int main(){

    char c;
    int index, snap_status;
    std::string defect, path;

    char inputs[10] = { '0','1', '2', '3', '4', '5', '6', '7', '8', '9'};
    //defects should be the set for needed defects, NOTE: Releven Directories must be created maually.
    std::string defects[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    system("stty raw");
    while(1){
        c = getchar();

        if ( c == 'e'){
            system("stty cooked");
            exit(0);
        }

        char* found = std::find(std::begin(inputs), std::end(inputs), c);

        if(found != std::end(inputs)){
            system("stty cooked");
            index = std::distance(inputs, found);
            defect = defects[index];
            path = "./defects/" + defect + "/hello.jpg";
            snap_status = getSnap(5);
            if (snap_status == 0) exit(0);
            system("stty raw");
        }

    }

    return 0;
}