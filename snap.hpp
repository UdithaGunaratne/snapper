// method should be getSnap(std::stri
#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"
#include <iostream>
#include <sstream>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

// Helper Function That will sleep in both windows and linux

void SleepyWrapper(int milliseconds){
    #if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64
        Sleep(milliseconds); // milliseconds for windows
    #else
        usleep(1000 * milliseconds); //microseconds for linux
    #endif
}

class ImageEventHandlerImpl : public ImageEventHandler
{
  public:
    // The constructor retrieves the serial number and initializes the image
    // counter to 0.
    ImageEventHandlerImpl(CameraPtr pCam, unsigned int snap_count)
    {
        // Retrieve device serial number
        INodeMap& nodeMap = pCam->GetTLDeviceNodeMap();

        m_deviceSerialNumber = "";
        CStringPtr ptrDeviceSerialNumber = nodeMap.GetNode("DeviceSerialNumber");
        if (IsAvailable(ptrDeviceSerialNumber) && IsReadable(ptrDeviceSerialNumber))
        {
            m_deviceSerialNumber = ptrDeviceSerialNumber->GetValue();
        }

        // Initialize image counter to 0
        m_imageCnt = 0;

        mk_numImages = snap_count;

        // Release reference to camera
        pCam = nullptr;
    }
    ~ImageEventHandlerImpl()
    {
    }

    // This method defines an image event. In it, the image that triggered the
    // event is converted and saved before incrementing the count. Please see
    // Acquisition_CSharp example for more in-depth comments on the acquisition
    // of images.
    void OnImageEvent(ImagePtr image)
    {
        // Save a maximum of 10 images
        if (m_imageCnt < mk_numImages)
        {
            cout << "Image event occurred..." << endl;

            // Check image retrieval status
            if (image->IsIncomplete())
            {
                cout << "Image incomplete with image status " << image->GetImageStatus() << "..." << endl << endl;
            }
            else
            {
                // Print image information
                cout << "Grabbed image " << m_imageCnt << ", width = " << image->GetWidth()
                     << ", height = " << image->GetHeight() << endl;

                // Convert image to mono 8
                ImagePtr convertedImage = image->Convert(PixelFormat_Mono8, HQ_LINEAR);

                // Create a unique filename and save image
                ostringstream filename;

                filename << "./images/";
                if (m_deviceSerialNumber != "")
                {
                    filename << m_deviceSerialNumber.c_str() << "-";
                }
                filename << m_imageCnt << ".jpg";

                convertedImage->Save(filename.str().c_str());

                cout << "Image saved at " << filename.str() << endl << endl;

                // Increment image counter
                m_imageCnt++;
            }
        }
    }

    // Getter for image counter
    int getImageCount()
    {
        return m_imageCnt;
    }

    // Getter for maximum images
    int getMaxImages()
    {
        return mk_numImages;
    }

  private:
    unsigned int mk_numImages;
    unsigned int m_imageCnt;
    string m_deviceSerialNumber;
};

int ConfigureImageEvents(CameraPtr pCam, ImageEventHandlerImpl*& imageEventHandler, unsigned int snap_count)
{
    int result = 0;

    try
    {
        imageEventHandler = new ImageEventHandlerImpl(pCam, snap_count);
        pCam->RegisterEventHandler(*imageEventHandler);
    }
    catch (Spinnaker::Exception& e)
    {
        cout << "Error: " << e.what() << endl;
        result = -1;
    }

    return result;
}

int WaitForImages(ImageEventHandlerImpl*& imageEventHandler)
{
    int result = 0;

    try
    {
        const int sleepDuration = 200; // in milliseconds

        cout << endl << "\t### Starting Grabbing Snapps ###" << endl;
        while (imageEventHandler->getImageCount() < imageEventHandler->getMaxImages())
        {

            SleepyWrapper(sleepDuration);
        }
        cout << endl << "\t### Grabbing Images Completed ###" << endl;
    }
    catch (Spinnaker::Exception& e)
    {
        cout << "Error: " << e.what() << endl;
        result = -1;
    }

    return result;
}

int ResetImageEvents(CameraPtr pCam, ImageEventHandlerImpl*& imageEventHandler)
{
    int result = 0;

    try
    {
        
        pCam->UnregisterEventHandler(*imageEventHandler);

        // Delete image event handler (because it is a pointer)
        delete imageEventHandler;

        cout << "Image events unregistered..." << endl << endl;
    }
    catch (Spinnaker::Exception& e)
    {
        cout << "Error: " << e.what() << endl;
        result = -1;
    }

    return result;
}

int PrintDeviceInfo(INodeMap& nodeMap)
{
    int result = 0;

    cout << endl << "*** DEVICE INFORMATION ***" << endl << endl;

    try
    {
        FeatureList_t features;
        CCategoryPtr category = nodeMap.GetNode("DeviceInformation");
        if (IsAvailable(category) && IsReadable(category))
        {
            category->GetFeatures(features);

            FeatureList_t::const_iterator it;
            for (it = features.begin(); it != features.end(); ++it)
            {
                CNodePtr pfeatureNode = *it;
                cout << pfeatureNode->GetName() << " : ";
                CValuePtr pValue = (CValuePtr)pfeatureNode;
                cout << (IsReadable(pValue) ? pValue->ToString() : "Node not readable");
                cout << endl;
            }
        }
        else
        {
            cout << "Device control information not available." << endl;
        }
    }
    catch (Spinnaker::Exception& e)
    {
        cout << "Error: " << e.what() << endl;
        result = -1;
    }

    return result;
}

int AcquireImages(
    CameraPtr pCam,
    INodeMap& nodeMap,
    INodeMap& nodeMapTLDevice,
    ImageEventHandlerImpl*& imageEventHandler)
{
    int result = 0;

    cout << endl << endl << "*** IMAGE ACQUISITION ***" << endl << endl;

    try
    {
        // Set acquisition mode to continuous
        CEnumerationPtr ptrAcquisitionMode = nodeMap.GetNode("AcquisitionMode");
        if (!IsAvailable(ptrAcquisitionMode) || !IsWritable(ptrAcquisitionMode))
        {
            cout << "Unable to set acquisition mode to continuous (node retrieval). Aborting..." << endl << endl;
            return -1;
        }

        CEnumEntryPtr ptrAcquisitionModeContinuous = ptrAcquisitionMode->GetEntryByName("Continuous");
        if (!IsAvailable(ptrAcquisitionModeContinuous) || !IsReadable(ptrAcquisitionModeContinuous))
        {
            cout << "Unable to set acquisition mode to continuous (enum entry retrieval). Aborting..." << endl << endl;
            return -1;
        }

        int64_t acquisitionModeContinuous = ptrAcquisitionModeContinuous->GetValue();

        ptrAcquisitionMode->SetIntValue(acquisitionModeContinuous);

        cout << "Acquisition mode set to continuous..." << endl;

        // Begin acquiring images
        pCam->BeginAcquisition();

        cout << "Acquiring images..." << endl;

        // Retrieve images using image event handler
        WaitForImages(imageEventHandler);

        // End acquisition
        pCam->EndAcquisition();
    }
    catch (Spinnaker::Exception& e)
    {
        cout << "Error: " << e.what() << endl;
        result = -1;
    }

    return result;
}

int RunSingleCamera(CameraPtr pCam, unsigned int snap_count)
{
    int result = 0;
    int err = 0;

    try
    {
        // Retrieve TL device nodemap and print device information
        INodeMap& nodeMapTLDevice = pCam->GetTLDeviceNodeMap();

        result = PrintDeviceInfo(nodeMapTLDevice);

        // Initialize camera
        pCam->Init();

        // Retrieve GenICam nodemap
        INodeMap& nodeMap = pCam->GetNodeMap();

        // Configure image events
        ImageEventHandlerImpl* imageEventHandler;

        err = ConfigureImageEvents(pCam, imageEventHandler, snap_count);
        if (err < 0)
        {
            return err;
        }

        // Acquire images using the image event handler
        result = result | AcquireImages(pCam, nodeMap, nodeMapTLDevice, imageEventHandler);

        // Reset image events
        result = result | ResetImageEvents(pCam, imageEventHandler);

        // Deinitialize camera
        pCam->DeInit();
    }
    catch (Spinnaker::Exception& e)
    {
        cout << "Error: " << e.what() << endl;
        result = -1;
    }

    return result;
}


int getSnap(unsigned int snap_count){

    int result = 0;

    SystemPtr system = System::GetInstance();

    //Get the list of cameras from the system
    CameraList camList = system->GetCameras();

    unsigned int numCameras = camList.GetSize();

    //If no cameras available end
    if( numCameras == 0 ){

        camList.Clear();
        system->ReleaseInstance();

        cout << "No Cameras Found !!" << endl;

        return -1;
    }

    //Snapping from each camera
    for (unsigned int i = 0; i < numCameras; i++){
        result = result | RunSingleCamera(camList.GetByIndex(i), snap_count);
    }

    camList.Clear();

    system->ReleaseInstance();

    return result;
    
}