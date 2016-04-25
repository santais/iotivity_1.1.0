//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "ocbaseresource.h"
#include "resource_types.h"

static const int DELAY_TIME_INPUT_THREAD = 10;      // ms

// Blinking LED
static const char LED_PIN = 13;
static const char TEST_LED_PIN = 5; // PWM Pin
static const char TEST_BUT_PIN = 2;	


static int g_prevButtonReading = false;
#define TAG "ArduinoServer"

#ifdef ARDUINOWIFI
// Arduino WiFi Shield
// Note : Arduino WiFi Shield currently does NOT support multicast and therefore
// this server will NOT be listening on 224.0.1.187 multicast address.

static const char ARDUINO_WIFI_SHIELD_UDP_FW_VER[] = "1.1.0";
    
/// WiFi Shield firmware with Intel patches
static const char INTEL_WIFI_SHIELD_FW_VER[] = "1.2.0";

/// WiFi network info and credentials
char ssid[] = "EasySetup123";
char pass[] = "EasySetup123";

int ConnectToNetwork()
{
    char *fwVersion;
    int status = WL_IDLE_STATUS;
    pinMode(9, OUTPUT);      // set the LED pin mode
    delay(1000);
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD)
    {
        OIC_LOG(ERROR, TAG, ("WiFi shield not present"));
        return -1;
    }

    // Verify that WiFi Shield is running the firmware with all UDP fixes
    fwVersion = WiFi.firmwareVersion();
    OIC_LOG_V(INFO, TAG, "WiFi Shield Firmware version %s", fwVersion);
    if ( strncmp(fwVersion, ARDUINO_WIFI_SHIELD_UDP_FW_VER, sizeof(ARDUINO_WIFI_SHIELD_UDP_FW_VER)) !=0 )
    {
        OIC_LOG(DEBUG, TAG, ("!!!!! Upgrade WiFi Shield Firmware version !!!!!!"));
        return -1;
    }

    // attempt to connect to Wifi network:
    OIC_LOG(DEBUG, TAG, "Connecting...");
    while (status != WL_CONNECTED)
    {
        OIC_LOG_V(INFO, TAG, "Attempting to connect to SSID: %s", ssid);
        status = WiFi.begin(ssid,pass);

        // wait 10 seconds for connection:
        delay(10000);
        OIC_LOG(DEBUG, TAG, "Retrying...");
    }
    OIC_LOG(DEBUG, TAG, ("Connected to wifi"));

    IPAddress ip = WiFi.localIP();
    OIC_LOG_V(INFO, TAG, "IP Address:  %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    return 0;
}
#else
// Arduino Ethernet Shield
int ConnectToNetwork()
{
    // Note: ****Update the MAC address here with your shield's MAC address****
    uint8_t ETHERNET_MAC[] = {0x90, 0xA2, 0xDA, 0x10, 0x29, 0xE2}; 
    uint8_t error = Ethernet.begin(ETHERNET_MAC);
    if (error  == 0)
    {
        OIC_LOG_V(ERROR, TAG, "error is: %d", error);
        return -1;
    }

    IPAddress ip = Ethernet.localIP();
    OIC_LOG_V(INFO, TAG, "IP Address:  %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    return 0;
}
#endif //ARDUINOWIFI

// On Arduino Atmel boards with Harvard memory architecture, the stack grows
// downwards from the top and the heap grows upwards. This method will print
// the distance(in terms of bytes) between those two.
// See here for more details :
// http://www.atmel.com/webdoc/AVRLibcReferenceManual/malloc_1malloc_intro.html
void PrintArduinoMemoryStats()
{
    //#ifdef ARDUINO_AVR_MEGA2560
    //This var is declared in avr-libc/stdlib/malloc.c
    //It keeps the largest address not allocated for heap
    extern char *__brkval;
    //address of tmp gives us the current stack boundry
    int tmp;
    OIC_LOG_V(INFO, TAG, "Stack: %u         Heap: %u", (unsigned int)&tmp, (unsigned int)__brkval);
    OIC_LOG_V(INFO, TAG, "Unallocated Memory between heap and stack: %u",
            ((unsigned int)&tmp - (unsigned int)__brkval));
   // #endif
}

void printAttribute(OCRepPayloadValue *attributes)
{
    OIC_LOG(DEBUG, TAG, "Attributes :");
    OCRepPayloadValue *current = attributes;
    while(current != NULL)
    {
        OIC_LOG_V(DEBUG, TAG, "Name: %s", current->name);
        switch(current->type)
        {
        case OCREP_PROP_INT:
            OIC_LOG_V(DEBUG, TAG, "Value: %i", current->i);
            //OIC_LOG_V(DEBUG, TAG, "Value: %i", *((int*)current->value.data));
            break;
        case OCREP_PROP_DOUBLE:
            OIC_LOG_V(DEBUG, TAG, "Value: %f", current->d);
            //OIC_LOG_V(DEBUG, TAG, "Value: %f", *((double*)current->value.data));
            break;
            break;
        case OCREP_PROP_BOOL:
            OIC_LOG_V(DEBUG, TAG, "Value: %s", current->b ? "true" : "false");
           /* bool boolean = *((bool*) current->value.data);
            OIC_LOG_V(DEBUG, TAG, "Value: %s", boolean ? "true" : "false");*/
            break;
        case OCREP_PROP_STRING:
            OIC_LOG_V(DEBUG, TAG, "Value: %s", current->str);
            //OIC_LOG_V(DEBUG, TAG, "Value: %s", *((char**)current->value.data));
            break;
        }
        current = current->next;
    }
    OIC_LOG(DEBUG, TAG, "Done printing attributes!");
}

void printResource(OCBaseResourceT *resource)
{
    OIC_LOG(DEBUG, TAG, "=============================");
    OIC_LOG_V(DEBUG, TAG, "Resource URI: %s", resource->uri);
    OIC_LOG_V(DEBUG, TAG, "Handle of the resource: %p", (void*) resource->handle);

    OIC_LOG(DEBUG, TAG, "Resource Types: ");
    OCResourceType *currentType = resource->type;
    while(currentType != NULL)
    {
        OIC_LOG_V(DEBUG, TAG, "\t%s", currentType->resourcetypename);
        currentType = currentType->next;
    }

    OIC_LOG(DEBUG, TAG, "Resource Interfaces: ");
    OCResourceInterface *currentInterface = resource->interface;
    while(currentInterface != NULL)
    {
        OIC_LOG_V(DEBUG, TAG, "\t%s", currentInterface->name);
        currentInterface = currentInterface->next;
    }

    printAttribute(resource->attribute);
    OIC_LOG(DEBUG, TAG, "=============================");
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

void checkInputThread()
{
    // Search through the list of resource
    OCBaseResourceT *current = getResourceList();
    while(current != NULL) 
    {
        if(current->port->type == INPUT)
        {
            int reading = digitalRead(current->port->pin);

            // Debounce
            if(reading > 0) {
                delay(50);
                reading = digitalRead(current->port->pin);
            }
            if(current->underObservation && (g_prevButtonReading != reading))
            {
                OIC_LOG(DEBUG, TAG, "Notifying observers");
                current->attribute->b = reading;
                Serial.println((int)current->handle, HEX);
                if(OCNotifyAllObservers(current->handle, OC_MEDIUM_QOS) == OC_STACK_NO_OBSERVERS)
                {
                    OIC_LOG(DEBUG, TAG, "No more observers!");
                    current->underObservation = false;
                }
                g_prevButtonReading = reading;
            }
        }
        current = current->next;
    }
}

//The setup function is called once at startup of the sketch
void setup()
{
    // Add your initialization code here
    // Note : This will initialize Serial port on Arduino at 115200 bauds
    OIC_LOG_INIT();
    OIC_LOG(DEBUG, TAG, ("OCServer is starting..."));


    // Connect to Ethernet or WiFi network
    if (ConnectToNetwork() != 0)
    {
        Serial.print("Unable to connect to Network");
        OIC_LOG(ERROR, TAG, ("Unable to connect to network"));
        return;
    }

    // Initialize the OC Stack in Server mode
    if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
    {
        Serial.println("OCStack Init Error");
        OIC_LOG(ERROR, TAG, ("OCStack init error"));
        return;
    }

    // DEBUG PIN
    pinMode(LED_PIN, OUTPUT);

    OIC_LOG(DEBUG, TAG, "Creating resource");

    OCIOPort portLight;
    portLight.pin = TEST_LED_PIN; // LED_PIN for debug
    portLight.type = OUT;

    // Light resource
    OCBaseResourceT *resourceLight = createResource("/a/light", OIC_DEVICE_LIGHT, OC_RSRVD_INTERFACE_DEFAULT,
                                              (OC_DISCOVERABLE | OC_OBSERVABLE), lightIOHandler, &portLight);
    
    if(resourceLight != NULL)
    {
        OIC_LOG(INFO, TAG, "Light resource created successfully");
        Serial.println((int)resourceLight->handle, HEX);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Unable to create light resource");
    }
    resourceLight->name = "Mark's Light";

    addType(resourceLight, OIC_TYPE_BINARY_SWITCH);
    addType(resourceLight, OIC_TYPE_LIGHT_BRIGHTNESS);

    OCRepPayloadValue powerValue;
    powerValue.b = true;
    powerValue.name = "power";
    powerValue.next = NULL;
    powerValue.type = OCREP_PROP_BOOL;
    addAttribute(&resourceLight->attribute, &powerValue);

    
    OCRepPayloadValue brightnessValue;
    brightnessValue.i = 255;
    brightnessValue.name = "brightness";
    brightnessValue.next = NULL;
    brightnessValue.type = OCREP_PROP_INT;
    addAttribute(&resourceLight->attribute, &brightnessValue);

    /** Create Button Resource **/
    OCIOPort buttonPort;
    buttonPort.pin = TEST_BUT_PIN;
    buttonPort.type = IN;

    OCBaseResourceT *buttonResource = createResource("/a/button", OIC_DEVICE_BUTTON, OC_RSRVD_INTERFACE_DEFAULT,
                                            (OC_DISCOVERABLE | OC_OBSERVABLE), NULL, &buttonPort);

    if(buttonResource != NULL)
    {
        OIC_LOG(INFO, TAG, "Button resource created successfully");
        int pointer;
        Serial.println((int)buttonResource->handle, HEX);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Unable to create the button resource");
    }

    OCStackResult result = addType(buttonResource, OIC_TYPE_BINARY_SWITCH);
    result = addInterface(buttonResource, OC_RSRVD_INTERFACE_READ);

    if(result != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Can't add interface READ");
    }

    OCRepPayloadValue buttonValue;
    buttonValue.b = false;
    buttonValue.name = "state";
    buttonValue.next = NULL;
    buttonValue.type = OCREP_PROP_BOOL;
    addAttribute(&buttonResource->attribute, &buttonValue);

    if(OCStartPresence(60 * 60) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Unable to start presence server");
    }


    OIC_LOG(DEBUG, TAG, "Finished setup");
}

// The loop function is caplled in an endless loop
void loop()
{
    // This artificial delay is kept here to avoid endless spinning
    // of Arduino microcontroller. Modify it as per specific application needs.
    //Serial.println("Alive");
    delay(DELAY_TIME_INPUT_THREAD);
    checkInputThread();

    // This call displays the amount of free SRAM available on Arduino
    //PrintArduinoMemoryStats();

    // Give CPU cycles to OCStack to perform send/recv and other OCStack stuff
    if (OCProcess() != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, ("OCStack process error"));
        return;
    }

    //yield();
}
