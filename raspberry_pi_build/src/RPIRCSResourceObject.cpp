#include "RPIRCSResourceObject.h"

/**
 * @brief RPIRCSResourceObject Set the internal parameters for this specific resource
 * @param resourceTypes         The types associated with the resource
 * @param resourceInterfaces    The interfaces associated with the resource
 */
RPIRCSResourceObject::RPIRCSResourceObject(const std::string &uri, std::vector<std::string>&& resourceTypes, std::vector<std::string>&& resourceInterfaces)
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    m_uri = uri;
    m_resourceTypes = std::move(resourceTypes);
    m_resourceInterfaces = std::move(resourceInterfaces);
}

/**
 * @brief RPIRCSResourceObject Set the internal parameters for this specific resource
 * @param resourceTypes         The types associated with the resource
 * @param resourceInterfaces    The interfaces associated with the resource
 */
RPIRCSResourceObject::RPIRCSResourceObject(const std::string& uri, const std::vector<std::string>& resourceTypes, const std::vector<std::string>& resourceInterfaces)
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    m_uri = uri;
    m_resourceTypes = resourceTypes;
    m_resourceInterfaces = resourceInterfaces;
}
/**
 * @brief ~RPIRCSResourceObject Clear all resources bindings.
 */
RPIRCSResourceObject::~RPIRCSResourceObject()
{

}

/**
 * @brief createResource Creates the resource
 */
void RPIRCSResourceObject::createResource(bool discoverable, bool observable, bool secured)
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    RCSResourceObject::Builder builder = RCSResourceObject::Builder(m_uri, m_resourceTypes[0], m_resourceInterfaces[0])
            .setDiscoverable(discoverable)
            .setObservable(observable)
            .setSecureFlag(secured);

    std::for_each(m_resourceTypes.begin() + 1, m_resourceTypes.end(),
                  [&builder](const std::string& typeName) {
        builder.addType(typeName);
    });

    std::for_each(m_resourceInterfaces.begin() + 1, m_resourceInterfaces.end(),
                  [&builder](const std::string& interfaceName) {
        builder.addInterface(interfaceName);
    });

    m_resource = builder.build();
}

/**
 * @brief setRpiOutputType  Set the output type
 * @param type  Type to be set
 * @param port  Port number if type is GPIO
 */
void RPIRCSResourceObject::setRpiOutputType(RpiIOHandler::RPIOutputType &type, int port)
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    m_rpiOutputType = type;
    if(port == NO_PORT && type == RpiIOHandler::RPIOutputType::GPIO_DIGITAL)
    {
        throw RCSException("No port has been assigned to the GPIO");
    }
    else
    {
        switch(type)
        {
        case RpiIOHandler::RPIOutputType::AUX:

            break;
        case RpiIOHandler::RPIOutputType::HDMI:

            break;
        case RpiIOHandler::RPIOutputType::GPIO_DIGITAL:

            break;
        case RpiIOHandler::RPIOutputType::VIRTUAL:

            break;
        default:
            std::cout << "Unknonw type" << std::endl;
            break;
        }
    }
}

/**
 * @brief setRpiInputType   Set the input type
 * @param type  Type of input. Only GPIO supported
 * @param port  Port number if type is GPIO
 */
void RPIRCSResourceObject::setRpiInputType(RpiIOHandler::RPIInputType &type, int port)
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    m_rpiInputType = type;
    if(port == NO_PORT && type == RpiIOHandler::RPIInputType::GPIO_DIGITAL)
    {
        throw RCSException("No port has been assigned to the GPIO");
    }
    else
    {

    }
}

/**
 * @brief addType
 * @param type
 * @return
 */
std::vector<std::string> RPIRCSResourceObject::getTypes()
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    if(m_resource != nullptr)
    {
        return m_resource->getTypes();
    }
    else
    {
        throw RCSException("ResourceObject has not been created");
    }
}

/**
 * @brief addInterface
 * @param interface
 * @return
 */
std::vector<std::string> RPIRCSResourceObject::getInterfaces()
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    if(m_resource != nullptr)
    {
        return m_resource->getInterfaces();
    }
    else
    {
        throw RCSException("ResourceObject has not been created");
    }
}

/**
 * @brief addAttributes
 * @param name
 */
void RPIRCSResourceObject::addAttribute(const std::string& name, RCSResourceAttributes::Value& value)
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    if(m_resource != nullptr)
    {
        m_resource->setAttribute(name, value);
    }
    else
    {
        throw RCSException("ResourceObject has not beenc created");
    }
}

/**
 * @brief getAttributes
 * @return
 */
RCSResourceAttributes RPIRCSResourceObject::getAttributes()
{
#ifdef DEBUG
    std::cout << __func__ << std::endl;
#endif
    if(m_resource != nullptr)
    {
        return m_resource->getAttributes();
    }
    else
    {
        throw RCSException("ResourceObject has not been created");
    }
}
