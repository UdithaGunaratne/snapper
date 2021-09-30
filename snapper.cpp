#include <iostream>
#include <stdio.h>
#include <experimental/filesystem>
#include <algorithm>
#include "snap.hpp"
#include "show.hpp"

int main(){

    char c;
    int index, snap_status, monitor_width, monitor_height, amount_of_snaps_per_input, amount_of_miliseconds_to_display_the_image;
    std::string defect, path;
    Mode mode;
    PixelFormat camera_pixel_format, save_pixel_format;

    imgData data = imgData("./warning.png");

    //<- ---------------------------------------------- ->
    //Settings

    //monitor settings
    monitor_width = 1366;
    monitor_height = 768;

    //image settings
    amount_of_snaps_per_input = 1;
    mode = MODE_0;
    camera_pixel_format = PIXEL_FORMAT_RGB;
    save_pixel_format = PIXEL_FORMAT_RGB;

    //defects settings
    char inputs[14] = { '0','1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '/', '*', '-'};
    std::string defects[14] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"}; //defects should be the set for needed defects, NOTE: Relevent Directories must be created maually.

    //display settings
    amount_of_miliseconds_to_display_the_image = 2000;
    //<- ---------------------------------------------- ->

    system("stty raw");
    while(1){
        c = getchar();

        if ( c == 'e'){
            system("stty cooked");
            exit(0);
        }

        if ( c == '+'){
            system("stty cooked");
            path = "./defects/calibration/";
            snap_status = getSnap(path, amount_of_snaps_per_input, mode, data, camera_pixel_format, save_pixel_format); //RGB8 & RGB8
            if (snap_status == -1) exit(0);
            showImg(data.getImgPath(), amount_of_miliseconds_to_display_the_image, monitor_width, monitor_height);
            system("stty raw");
        }

        char* found = std::find(std::begin(inputs), std::end(inputs), c);

        if(found != std::end(inputs)){
            system("stty cooked");
            index = std::distance(inputs, found);
            defect = defects[index];
            path = "./defects/" + defect + "/";
            snap_status = getSnap(path, amount_of_snaps_per_input, mode, data, camera_pixel_format, save_pixel_format); //RGB8 & RGB8
            if (snap_status == -1) exit(0);
            showImg(data.getImgPath(), amount_of_miliseconds_to_display_the_image, monitor_width, monitor_height);
            system("stty raw");
        }

    }

    //added getCurrentTime(). 

    return 0;
}
