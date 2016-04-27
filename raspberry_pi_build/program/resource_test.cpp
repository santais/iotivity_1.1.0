#include "RPIRCSController.h"
#include "RPIRCSResourceObject.h"

#include "OCPlatform.h"
#include "OCApi.h"

#include <signal.h>
#include <unistd.h>

const static std::string LIGHT_DEVICE = "oic.d.light";
const static std::string BINARY_SWITCH_TYPE = "oic.r.binary.switch";
const static std::string LIGHT_DIMMING_TYPE = "oic.r.light.dimming";

std::vector<std::string> g_types = {LIGHT_DEVICE, BINARY_SWITCH_TYPE,
                                   LIGHT_DIMMING_TYPE};
std::vector<std::string> g_interfaces = {OC_RSRVD_INTERFACE_DEFAULT};

int g_quitFlag = false;

void handleSigInt(int signum)
{
    if (signum == SIGINT)
    {
        g_quitFlag = 1;
    }
}

int main()
{
    std::cout << "Starting test program" << std::endl;

    RPIRCSResourceObject::Ptr resource { new RPIRCSResourceObject { "/a/light", g_types, g_interfaces } };
    resource->createResource(true, true, false);

    // Add attributes
    RCSResourceAttributes::Value value((int) 0);
    resource->addAttribute("brightness", value);

    std::cout << "Resource has been created" << std::endl;

    std::cout << "Types are: " << std::endl;
    for(std::string& type : resource->getTypes())
    {
        std::cout << "\t " << type << std::endl;
    }

    std::cout << "Interfaces are: " << std::endl;
    for(std::string& interface : resource->getInterfaces())
    {
        std::cout << "\t " << interface << std::endl;
    }

    signal(SIGINT, handleSigInt);
    while(!g_quitFlag)
    {
        if(OCProcess() != OC_STACK_OK)
        {
            return 0;
        }
        sleep(1);
    }

    return 0;
}
