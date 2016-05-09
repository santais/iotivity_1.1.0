#include "ocbaseresource.h"
#include "resource_types.h"
#include <stdlib.h>
#include <string>
#include <signal.h>
#include <unistd.h>

#include "rd_client.h"

OCBaseResourceT* g_lightResource;
char g_rdAddress[MAX_ADDR_STR_SIZE];
uint16_t g_rdPort;

int g_quitFlag = false;

void handleSigInt(int signum)
{
    if(signum == SIGINT)
    {
        g_quitFlag = true;
    }
}

void lightIOHandler(OCRepPayloadValue *attribute, OCIOPort *port, OCResourceHandle handle, bool *underObservation)
{
    if(port->type == OUTPUT)
    {
        bool power(false);
        int brightness(0);
       // OIC_LOG(DEBUG, TAG, "LightIOHandler: OUTPUT");
        OCRepPayloadValue *current = attribute;
        while(current != NULL)
        {
            //OIC_LOG_V(DEBUG, TAG, "Attribute name: %s", current->name);
            //OIC_LOG(DEBUG, TAG, "Searching light");
            if(strcmp(current->name, "power") == 0)
            {
                power = current->b;
            }
            else if (strcmp(current->name, "brightness") == 0)
            {
                brightness = current->i;
            }

            current = current->next;
        }

        if(power)
        {
            analogWrite(port->pin, brightness);
        }
        else
        {
            analogWrite(port->pin, 0);
        }

        if(*underObservation)
        {
            OIC_LOG(DEBUG, TAG, "LIGHT: Notifying observers");
            if(OCNotifyAllObservers(handle, OC_LOW_QOS) == OC_STACK_NO_OBSERVERS)
            {
                OIC_LOG(DEBUG, TAG, "No more observers!");
                *underObservation = false;
            }
        }
    }
}

void createLightResource()
{
    OIC_LOG(DEBUG, TAG, "Creating resource");

    OCIOPort portLight;
    portLight.pin = TEST_LED_PIN; // LED_PIN for debug
    portLight.type = OUT;

    // Light resource
    g_lightResource = createResource("/a/light", OIC_DEVICE_LIGHT, OC_RSRVD_INTERFACE_DEFAULT,
                                              (OC_DISCOVERABLE | OC_OBSERVABLE), lightIOHandler, &portLight);

    if(g_lightResource != NULL)
    {
        OIC_LOG(INFO, TAG, "Light resource created successfully");
        Serial.println((int)g_lightResource->handle, HEX);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Unable to create light resource");
    }
    g_lightResource->name = "Mark's Light";

    addType(g_lightResource, OIC_TYPE_BINARY_SWITCH);
    addType(g_lightResource, OIC_TYPE_LIGHT_BRIGHTNESS);

    OCRepPayloadValue powerValue;
    powerValue.b = true;
    powerValue.name = "power";
    powerValue.next = NULL;
    powerValue.type = OCREP_PROP_BOOL;
    addAttribute(&g_lightResource->attribute, &powerValue);


    OCRepPayloadValue brightnessValue;
    brightnessValue.i = 255;
    brightnessValue.name = "brightness";
    brightnessValue.next = NULL;
    brightnessValue.type = OCREP_PROP_INT;
    addAttribute(&g_lightResource->attribute, &brightnessValue);

}

int biasFactorCB(char addr[MAX_ADDR_STR_SIZE], uint16_t port)
{
    OICStrcpy(rdAddress, MAX_ADDR_STR_SIZE, addr);
    rdPort = port;
    std::cout << "RD Address is : " <<  addr << ":" << port << std::endl;
    return 0;
}


int main()
{
    printf("Starting Light Constrained Program");

    PlatformConfig cfg;

    OCPlatform::Configure(cfg);

    if(OCStartPresence(60 * 60) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Unable to start presence server");
    }

    std::cout << "Setup completed" << std::endl;
    signal(SIGINT, handleSigInt);

    while(OCRDDiscover(biasFactorCB) != OC_STACK_OK);

    // RD discovered. Publisher resources
    createLightResource();
    OCRDPublish(g_rdAddress, g_rdPort, 1, g_lightResource->handle);
    while(!g_quitFlag)
    {
        if(OCProcess() != OC_STACK_OK)
        {
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

}
