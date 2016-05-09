#include "LightResource.h"
#include "resource_types.h"

LightResource::LightResource()
{
    // Not initialized
    /*wiringPiSetup();
    m_outputPortPin = -1;*/
}

/**
 * @brief LightResource::LightResource
 *
 * @param portPin
 * @param uri       Uri of the new light resource
 */
LightResource::LightResource(int portPin, const std::string &uri)
{
    m_uri = uri;

    m_outputPortPin = portPin;

    // Initialize pins
   /* wiringPiSetup();
    pinMode(portPin, OUTPUT);*/
}

LightResource::~LightResource()
{
    m_resource.reset();
}

LightResource::LightResource(const LightResource& light) :
    m_resource(light.m_resource),
    m_outputPortPin(light.m_outputPortPin),
    m_uri(light.m_uri)
{}

LightResource::LightResource(LightResource&& light) :
    m_resource(std::move(light.m_resource)),
    m_outputPortPin(std::move(light.m_outputPortPin)),
    m_uri(std::move(light.m_uri))
{}

LightResource& LightResource::operator=(const LightResource& light)
{
    m_resource = light.m_resource;
    m_outputPortPin = light.m_outputPortPin;
    m_uri = light.m_uri;
}

LightResource& LightResource::operator=(LightResource&& light)
{
    m_resource = std::move(light.m_resource);
    m_outputPortPin = std::move(light.m_outputPortPin);
    m_uri = std::move(light.m_uri);
}

/**
 * @brief setOutputPortPin
 *
 * @param portPin
 */
void LightResource::setOutputPortPin(int portPin)
{
 //   pinMode(portPin, OUTPUT);
    m_outputPortPin = portPin;
}

/**
 * @brief getOutputPortPin
 *
 * @return
 */
int LightResource::getOutputPortPin()
{
    return m_outputPortPin;
}

RPIRCSResourceObject::Ptr LightResource::getResourceObject()
{
    return m_resource;
}


/**
 * @brief createResource
 */
int LightResource::createResource()
{
    if(m_uri.empty())
    {
        std::cout << "Uri is empty!" << std::endl;
        return -1;
    }
    // Using default parameters
    std::vector<std::string> resource_types = {OIC_DEVICE_LIGHT, OIC_TYPE_BINARY_SWITCH};

    m_resource = std::make_shared<RPIRCSResourceObject>(RPIRCSResourceObject(m_uri,
                            std::move(resource_types), std::move(std::vector<std::string> {OC_RSRVD_INTERFACE_DEFAULT})));

    m_resource->createResource(true, true, false);

    m_resource->setReqHandler(std::bind(&LightResource::setRequestHandler, this, std::placeholders::_1,
                                        std::placeholders::_2));

    // Set the attributes
    this->setAttributes();

    return 1;
}

void LightResource::setUri(std::string& uri)
{
    m_uri = uri;
}

std::string LightResource::getUri()
{
    return m_uri;
}

/**
 * @brief setRequestHandler
 *
 * @param request
 * @param attr
 */
void LightResource::setRequestHandler(const RCSRequest &request, RCSResourceAttributes &attrs)
{
    // Lookup the power attribute
    std:cout << "============================================ \n";
    if(m_outputPortPin >=    0)
    {
        if(attrs["power"] == true)
        {
            std::cout << "\t Key: Power is set to TRUE" << std::endl;
            // digitalWrite(m_outputPortPin, HIGH);
        }
        else if (attrs["power"] == false)
        {
            std::cout << "\t Key: State is set to FALSE" << std::endl;
            // digitalWrite(m_outputPortPin, LOW);
        }
        else
        {
            std::cerr << "Unable to find attribute power" << std::endl;
        }
    }
    else
    {
        std::cout << "Output pin has not been initialized. Please call setOutputPortPin(...)" << std::endl;
    }
    std::cout << "============================================ \n";
}


/**
 * @brief setAttributes
 */
void LightResource::setAttributes()
{
    RCSResourceAttributes::Value power((bool) false);
    m_resource->addAttribute("power", power);
}