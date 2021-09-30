# Snapper

<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#build-project">Build Project</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#custom-settings">Custom Settings</a></li>
  </ol>
</details>

## About The Project

This Project is to gather data from defect tracking process. Program will take inputs and will take a snap of the focused area of the fabric using the **camera 0** and will save it to the relevent folder for the defect according to the input.

## Getting Started

This program is programmed using C++ and the prerequisites with their versions and how to build the project can be found below.

### Prerequisites

These prerequisites and the version are for a **Raspberry Pi 3 Model B Vi.2 - 2015**.

1. [Ubuntu Mate 18.04 armhf](https://releases.ubuntu-mate.org/archived/bionic/armhf/ubuntu-mate-18.04.2-beta1-desktop-armhf+raspi-ext4.img.xz)
2. GCC/G++

```sh
sudo apt install build-essential
```

3. CMake

```sh
sudo apt install cmake
```

4. [FlyCapture SDK](https://flir.app.boxcn.net/v/Flycapture2SDK/file/434084777399)
5. OpenCv v3.2.0

```sh
sudo apt install python3-opencv
```



### Build Project

- clone the repository

```sh
git clone https://github.com/IntellisenseLab/FabVis-Snapper.git
```

- make a directory inside the repository to build the program

```sh
cd FabVis-Snapper/ && mkdir build && cd build
```

- Inside the build directory, build the project using cmake and make

```sh
cmake ..
make
```

- `snapper` should be build inside the build directory



## Usage

1. copy the build `snapper` and the included `snapper.sh` & `warning.png` files to a separate directory *( Ex: snapper).*
2.  Inside the directory *(snapper)* there must be a separate directory called defects, and inside defects include separate directories for the relevant defects you set in the program settings before building the project. *( Changing settings is described in a following section )*.
3. In `snapper.sh` edit 

```sh
cd /home/pi/Desktop
```

​	to the current directory*(snapper)*. { must be a direct path }

4. If you want to setup the program so that it runs on startup,
   - In system settings search for " **Startup Applications** ".
   - In **Startup Applications** -> ***+add*** another application.
     - Name : Snapper
     - Command : mate-terminal --command "bash **/home/pi/Desktop**/snapper.sh"
       - /home/pi/Desktop/ -> replace with the direct path used in <u>step_3</u>
     - Comment : run snapper

## Custom Settings

This section is to demonstrate how to change settings for the program.

```c++
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
char inputs[10] = { '0','1', '2', '3', '4', '5', '6', '7', '8', '9'};
std::string defects[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}; //defects should be the set for needed defects, NOTE: Relevent Directories must be created maually.

//<- ---------------------------------------------- ->
```
Edit this section in the snapper.cpp before building to change settings.