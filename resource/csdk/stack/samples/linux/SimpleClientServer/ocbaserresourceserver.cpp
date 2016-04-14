#include "ocbaseresource.h"
#include "resource_types.h"


void lightIOHandler(OCRepPayloadValue *attribute, OCIOPort *port, OCResourceHandle handle, bool *underObservation)
{
    OIC_LOG(DEBUG, TAG, "Inside LightIoHandler");
}


int main(int argc, char* argv[])
{
    // Add your initialization code here
    // Note : This will initialize Serial port on Arduino at 115200 bauds
    OIC_LOG_INIT();
    OIC_LOG(DEBUG, TAG, ("OCServer is starting..."));

    // Initialize the OC Stack in Server mode
    if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, ("OCStack init error"));
        return -1;
    }

    OIC_LOG(DEBUG, TAG, "Creating resource");

    OCIOPort portLight;
    portLight.pin = 0; // LED_PIN for debug
    portLight.type = OUT;

    // Light resource
    OCBaseResourceT *resourceLight = createResource("/a/light", OIC_DEVICE_LIGHT, OC_RSRVD_INTERFACE_DEFAULT,
                                              (OC_DISCOVERABLE | OC_OBSERVABLE), lightIOHandler, &portLight);
    
    if(resourceLight != NULL)
    {
        OIC_LOG(INFO, TAG, "Light resource created successfully");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Unable to create light resource");
        return -1;
    }

    resourceLight->name = "Mark's Light";

    addType(resourceLight, OIC_TYPE_BINARY_SWITCH);
    addType(resourceLight, OIC_TYPE_LIGHT_BRIGHTNESS);

    OCRepPayloadValue powerValue;
    powerValue.b = false;
    powerValue.name = "power";
    powerValue.next = NULL;
    addAttribute(&resourceLight->attribute, &powerValue);

    
    OCRepPayloadValue brightnessValue;
    brightnessValue.i = 0;
    brightnessValue.name = "brightness";
    brightnessValue.next = NULL;
    addAttribute(&resourceLight->attribute, &brightnessValue);

    //printResource(resourceLight);

    OIC_LOG(DEBUG, TAG, "Finished setup");

    while(OCProcess() == OC_STACK_OK)
    {
    	//sleep(0.5);
    }

    return -1;
}	