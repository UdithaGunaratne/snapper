# Snapper

<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#project-description">Project Description</a>
    </li>
    <li><a href="#implementation">Implementation</a></li>
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

## Project Description

This project is a specific requirement for Intellisense Labs. They have undertaken a project to Automate the process of defect detection in fabrics. Going into bit more details in the textile industry Apparels must not have defects as an end product. But fabric a mass produced raw material comes in huge rolls of fabric sheets and they have different kinds of defects in random on the sheet. These defects should not at all cost go through the manufacturing process and end up in the end product as they will be rejected from the final inspections and worst case scenario end up as a defect product in the customers hands. There for factory's have taken messures to control these defects. Before the fabric is to be used for the manufacturing process it will go through an inspection for defects. This will be done by an inspector manually. Roll will be rolling on a board such that fabric sheet will be layed on an inclined table give any moment of time. Inspector will inspect the sheet with naked eye for defects. This could have lots of human errors not to mention the person lost in his own thought whilst looking at sheet of fabric without any change. Even when he finds a defect he will stop the rolls and confirms his find note in down. Which is also time consuming. As we can clearly see this process is inefficient even though it is very crucial. The Solution is an automated system which will look at the fabric using multiple cameras mounted on top of the fabric and detect defects in realtime and make relevant notes. For this process FabVis team has proposed an machine learning solution.

The proposed machine learning solution requires a healthy dataset of accurate data of defects. This project is a solution to cater that requirement. Most efficient and reliable way to obtain such a data set is through the defect inspectors them selfs. They know how to detect the defects and what kind of defect they are. So the solution is to give them a system which will ease their day to day work whilst creating a healthy data set for the machine learning process.

The inspector will be given a number pad, each number will be assigned to a defect. When he recognise a defect rather than taking notes of the defects, all the inspector have to do is to press the corrosponding number on the number pad for the corrosponding defect. The system will automatically generate his daily report for him whilst creating the data set.

## Implementation

Add implementation

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