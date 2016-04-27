#ifndef _RPIRCSRESOURCEOBJECT_H_
#define _RPIRCSRESOURCEOBJECT_H_

#include "RCSResourceObject.h"

#include <vector>

/* Functions:
 *
 * OCStackResult createResource(std::vector<std::string> resourceTypes, std::vector<std::string> resourceInterfaces,
 *                              bool discoverable, bool observable, bool secured, RCSresourceAttributes attributes);
 *
 **/

using namespace OIC;
using namespace OIC::Service;

enum class RpiIOType {
    HDMI,
    AUX,
    GPIO,
    VIRTUAL
};

class RPIRCSResourceObject
{
public:
    /**
     * @brief RPIRCSResourceObject Set the internal parameters for this specific resource
     * @param resourceTypes         The types associated with the resource
     * @param resourceInterfaces    The interfaces associated with the resource
     */
    RPIRCSResourceObject(std::vector<std::string> resourceTypes, std::vector<std::string> resourceInterfaces);

    /**
     * @brief ~RPIRCSResourceObject Clera all resources bindings.
     */
    ~RPIRCSResourceObject();

    /**
     * @brief addRpiOutputType Adds the output type to the resource
     * @param type
     */
    void addRpiIOType(RpiIOType &type);
private:

    RPIRCSResourceObject() {}


private:
    /**
     * @brief m_resource
     * Iotivity resource object container
     */
    RCSResourceObject m_resource;

    /**
     * @brief m_resourceTypes Resource types of the resource
     */
    std::vector<std::string> m_resourceTypes;

    /**
     * @brief m_resourceInterfaces Resource interfaces of the resource
     */
    std::vector<std::string> m_resourceInterfaces;

    /**
     * @brief m_outputType Determines the physical output
     */
    addRpiIOType m_outputType;


protected:

};

#endif /* _RPIRCSRESOURCEOBJECT_H_ */
