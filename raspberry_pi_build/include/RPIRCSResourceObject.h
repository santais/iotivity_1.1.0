#ifndef _RPIRCSRESOURCEOBJECT_H_
#define _RPIRCSRESOURCEOBJECT_H_

#include "RCSResourceObject.h"
#include "RCSResourceAttributes.h"
#include "RCSException.h"

#include <vector>
#include <iostream>

constexpr int NO_PORT = -1;

using namespace OIC;
using namespace OIC::Service;


class RpiIOHandler {
public:
    enum class RPIOutputType {
        HDMI,
        AUX,
        GPIO_DIGITAL,
        GPIO_ANALOG,
        VIRTUAL
    };

    enum class RPIInputType {
        GPIO_DIGITAL
    };

private:
    std::vector<int> m_ports;

    RPIInputType m_inputType;
    RPIOutputType m_outputType;

public:
    RpiIOHandler(int port, RPIInputType inputType, RPIOutputType outputType);

    ~RpiIOHandler() {}

    RpiIOHandler& addInputType(RPIInputType type);

    RpiIOHandler& addOutputType(RPIOutputType type);

    RpiIOHandler& addPortPin(int port);

    RpiIOHandler& addPortPins(std::vector<int>&& ports);

    RpiIOHandler& addPortPins(const std::vector<int>& ports);

};



class RPIRCSResourceObject
{
public:
    typedef std::shared_ptr<RPIRCSResourceObject> Ptr;
    typedef std::shared_ptr<const RPIRCSResourceObject> ConstPtr;

public:

    /**
     * @brief RPIRCSResourceObject Set the internal parameters for this specific resource
     * @param resourceTypes         The types associated with the resource
     * @param resourceInterfaces    The interfaces associated with the resource
     */
    RPIRCSResourceObject(const std::string& uri, std::vector<std::string>&& resourceTypes, std::vector<std::string>&& resourceInterfaces);

    /**
     * @brief RPIRCSResourceObject Set the internal parameters for this specific resource
     * @param resourceTypes         The types associated with the resource
     * @param resourceInterfaces    The interfaces associated with the resource
     */
    RPIRCSResourceObject(const std::string &uri, const std::vector<std::string>& resourceTypes, const std::vector<std::string>& resourceInterfaces);

    /**
     * @brief ~RPIRCSResourceObject Clear all resources bindings.
     */
    ~RPIRCSResourceObject();

    /**
     * @brief createResource Creates the resource
     */
    void createResource(bool discovery, bool observable, bool secured);

    /**
     * @brief setRpiOutputType  Set the output type
     * @param type  Type to be set
     * @param port  Port number if type is GPIO
     */
    void setRpiOutputType(RpiIOHandler::RPIOutputType &type, int port = -1);

    /**
     * @brief setRpiInputType   Set the input type
     * @param type  Type of input. Only GPIO supported
     * @param port  Port number if type is GPIO
     */
    void setRpiInputType(RpiIOHandler::RPIInputType &type, int port);

    /**
     * @brief addType
     * @param type
     * @return
     */
    std::vector<std::string> getTypes();

    /**
     * @brief addInterface
     * @param interface
     * @return
     */
    std::vector<std::string> getInterfaces();

    /**
     * @brief addAttributes
     * @param name
     */
    void addAttribute(const std::string& name, RCSResourceAttributes::Value& value);

    /**
     * @brief getAttributes
     * @return
     */
    RCSResourceAttributes getAttributes();

    /**
     * @brief The RpiIOHandler class
     */

private:

    RPIRCSResourceObject() {}


private:
    /**
     * @brief m_resource
     * Iotivity resource object container
     */
    RCSResourceObject::Ptr m_resource;

    /**
     * @brief m_resourceTypes Resource types of the resource
     */
    std::vector<std::string> m_resourceTypes;

    /**
     * @brief m_resourceInterfaces Resource interfaces of the resource
     */
    std::vector<std::string> m_resourceInterfaces;

    /**
     * @brief m_rpiInputType Set the input gpio port type
     */
    RpiIOHandler::RPIInputType m_rpiInputType;

    /**
     * @brief m_outputType Determines the physical output
     */
    RpiIOHandler::RPIOutputType m_rpiOutputType;

    /**
     * @brief m_uri Uri of the registered resource
     */
    std::string m_uri;


protected:

};

#endif /* _RPIRCSRESOURCEOBJECT_H_ */
