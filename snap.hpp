#include "stdafx.h"

#include "./headers/FlyCapture2.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include "time.hpp"
#include <unistd.h>

using namespace FlyCapture2;
using namespace std;

class imgData{
    private:
        string pathToImg;

    public:
        imgData(string path){
            imgData::pathToImg = path;
        }

        int setImgPath(string path){
            imgData::pathToImg = path;

            return 0;
        }

        string getImgPath(){
            return pathToImg;
        }

};

void PrintFormat7Capabilities(Format7Info fmt7Info)
{
    cout << "Max image pixels: (" << fmt7Info.maxWidth << ", "
         << fmt7Info.maxHeight << ")" << endl;
    cout << "Image Unit size: (" << fmt7Info.imageHStepSize << ", "
         << fmt7Info.imageVStepSize << ")" << endl;
    cout << "Offset Unit size: (" << fmt7Info.offsetHStepSize << ", "
         << fmt7Info.offsetVStepSize << ")" << endl;
    cout << "Pixel format bitfield: 0x" << fmt7Info.pixelFormatBitField << endl;
}

void PrintError(Error error) { error.PrintErrorTrace(); }

int getSnap(string path, unsigned int snapCount, Mode mode, imgData &imgdata, PixelFormat camera_pixel_format,PixelFormat save_pixel_format)
{
    

    const Mode k_fmt7Mode = mode; //Camera MODE
    const PixelFormat k_fmt7PixFmt = camera_pixel_format;
    const int k_numImages = snapCount;

    Error error;

    BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    cout << "Number of cameras detected: " << numCameras << endl;

    if (numCameras < 1)
    {
        cout << "Insufficient number of cameras... Check camera connectivity !!" << endl;
        imgdata.setImgPath("./warning.png");
        return 0;
    }

    PGRGuid guid;
    error = busMgr.GetCameraFromIndex(0, &guid);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }    

    // Connect to a camera
	Camera cam;
    error = cam.Connect(&guid);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    // Get the camera information
    CameraInfo camInfo;
    error = cam.GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    // Query for available Format 7 modes
    Format7Info fmt7Info;
    bool supported;
    fmt7Info.mode = k_fmt7Mode;
    error = cam.GetFormat7Info(&fmt7Info, &supported);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    cout << "############ Format Capabilities ##########" << endl;
    PrintFormat7Capabilities(fmt7Info);

    if ((k_fmt7PixFmt & fmt7Info.pixelFormatBitField) == 0)
    {
        // Pixel format not supported!
        cout << "Pixel format is not supported" << endl;
        return -1;
    }

    //Settings
    Format7ImageSettings fmt7ImageSettings;
    fmt7ImageSettings.mode = k_fmt7Mode;
    fmt7ImageSettings.offsetX = 0;
    fmt7ImageSettings.offsetY = 0;
    fmt7ImageSettings.width = fmt7Info.maxWidth;
    fmt7ImageSettings.height = fmt7Info.maxHeight;
    fmt7ImageSettings.pixelFormat = k_fmt7PixFmt;

    bool valid;
    Format7PacketInfo fmt7PacketInfo;

    // Validate the settings to make sure that they are valid
    error = cam.ValidateFormat7Settings(
        &fmt7ImageSettings, &valid, &fmt7PacketInfo);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    if (!valid)
    {
        // Settings are not valid
        cout << "Format7 settings are not valid" << endl;
        return -1;
    }

    // Set the settings to the camera
    error = cam.SetFormat7Configuration(
        &fmt7ImageSettings, fmt7PacketInfo.recommendedBytesPerPacket);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    // Start capturing images
    error = cam.StartCapture();
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    // Retrieve frame rate property
    Property frmRate;
    frmRate.type = FRAME_RATE;
    error = cam.GetProperty(&frmRate);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    cout << "Frame rate is " << fixed << setprecision(2) << frmRate.absValue
         << " fps" << endl;

    cout << "Grabbing " << k_numImages << " images" << endl;

    Image rawImage;
    for (int imageCount = 0; imageCount < k_numImages; imageCount++)
    {
        // Retrieve an image
        error = cam.RetrieveBuffer(&rawImage);
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            continue;
        }

        cout << ".";

        // Get the raw image dimensions
        PixelFormat pixFormat;
        unsigned int rows, cols, stride;
        rawImage.GetDimensions(&rows, &cols, &stride, &pixFormat);

        // Create a converted image
        Image convertedImage;

        // Convert the raw image
        error = rawImage.Convert(save_pixel_format, &convertedImage);
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            return -1;
        }

        // Create a unique filename

        ostringstream filename;
        filename << path << getCurrentTime() << "#" << camInfo.serialNumber << ".bmp";

        // Save the image. If a file format is not passed in, then the file
        // extension is parsed to attempt to determine the file format.
        error = convertedImage.Save(filename.str().c_str());
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            return -1;
        }

        imgdata.setImgPath(filename.str());
        cout << imgdata.getImgPath() << endl;
        
    }

    cout << endl;
    cout << "Finished grabbing images" << endl;

    // Stop capturing images
    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        cout << "stopcamer" << endl;
        PrintError(error);
        return -1;
    }

    // Disconnect the camera
    error = cam.Disconnect();

    while( error != PGRERROR_OK){ //wait for camera to dissconnect
        error = cam.Disconnect();
        sleep(1);
    }

    if (error != PGRERROR_OK)
    {
        cout << "disconnectcamer" << endl;
        PrintError(error);
        return 0;
    }

    cout << "Done!" << endl;
    //cin.ignore();

    return 0;
}
