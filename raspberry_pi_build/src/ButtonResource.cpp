#include "ButtonResource.h"
#include "resource_types.h"

ButtonResource::ButtonResource()
{
    // Not initialized
    /*wiringPiSetup();
    m_inputPortPin = -1;*/
}

/**
 * @brief ButtonResource::ButtonResource
 *
 * @param portPin
 * @param uri       Uri of the new light resource
 */
ButtonResource::ButtonResource(int portPin, const std::string &uri)
{
    m_uri = uri;

    m_inputPortPin = portPin;

    // Initialize pins
   /* wiringPiSetup();
    pinMode(portPin, OUTPUT);*/
}

ButtonResource::~ButtonResource()
{
    m_resource.reset();
}

ButtonResource::ButtonResource(const ButtonResource& light) :
    m_resource(light.m_resource),
    m_inputPortPin(light.m_inputPortPin),
    m_uri(light.m_uri)
{}

ButtonResource::ButtonResource(ButtonResource&& light) :
    m_resource(std::move(light.m_resource)),
    m_inputPortPin(std::move(light.m_inputPortPin)),
    m_uri(std::move(light.m_uri))
{}

ButtonResource& ButtonResource::operator=(const ButtonResource& light)
{
    m_resource = light.m_resource;
    m_inputPortPin = light.m_inputPortPin;
    m_uri = light.m_uri;
}

ButtonResource& ButtonResource::operator=(ButtonResource&& light)
{
    m_resource = std::move(light.m_resource);
    m_inputPortPin = std::move(light.m_inputPortPin);
    m_uri = std::move(light.m_uri);
}

/**
 * @brief setInputPortPin
 *
 * @param portPin
 */
void ButtonResource::setInputPortPin(int portPin)
{
 //   pinMode(portPin, OUTPUT);
    m_inputPortPin = portPin;
}

/**
 * @brief getInputPortPin
 *
 * @return
 */
int ButtonResource::getInputPortPin()
{
    return m_inputPortPin;
}

RPIRCSResourceObject::Ptr ButtonResource::getResourceObject()
{
    return m_resource;
}


/**
 * @brief createResource
 */
int ButtonResource::createResource()
{
    if(m_uri.empty())
    {
        std::cout << "Uri is empty!" << std::endl;
        return -1;
    }
    // Using default parameters
    std::vector<std::string> resource_types = {OIC_DEVICE_BUTTON};

    m_resource = std::make_shared<RPIRCSResourceObject>(RPIRCSResourceObject(m_uri,
                            std::move(resource_types), std::move(std::vector<std::string> {OC_RSRVD_INTERFACE_DEFAULT,
                                                                 OC_RSRVD_INTERFACE_READ})));

    m_resource->createResource(true, true, false);

    m_resource->setReqHandler(std::bind(&ButtonResource::setRequestHandler, this, std::placeholders::_1,
                                        std::placeholders::_2));

    // Set the attributes
    this->setAttributes();

    return 1;
}

void ButtonResource::setUri(std::string& uri)
{
    m_uri = uri;
}

std::string ButtonResource::getUri()
{
    return m_uri;
}

/**
 * @brief setRequestHandler
 *
 * @param request
 * @param attr
 */
void ButtonResource::setRequestHandler(const RCSRequest &request, RCSResourceAttributes &attrs)
{
    // Lookup the state attribute
    std:cout << "============================================ \n";
    if(m_inputPortPin >= 0)
    {
        if(attrs["state"] == true)
        {
            std::cout << "\t Key: State is set to TRUE" << std::endl;
            // digitalWrite(m_inputPortPin, HIGH);
        }
        else if (attrs["state"] == false)
        {
            std::cout << "\t Key: State is set to FALSE" << std::endl;
            // digitalWrite(m_inputPortPin, LOW);
        }
        else
        {
            std::cerr << "Unable to find attribute state" << std::endl;
        }
    }
    else
    {
        std::cout << "Input pin has not been initialized. Please call setInputPortPin(...)" << std::endl;
    }
    std::cout << "============================================ \n";
}


/**
 * @brief setAttributes
 */
void ButtonResource::setAttributes()
{
    RCSResourceAttributes::Value power((bool) false);
    m_resource->addAttribute("state", power);
}
