#ifndef RPIRCSCONTROLLER_H
#define RPIRCSCONTROLLER_H

#include "RPIRCSResourceObject.h"

class RPIRCSController
{
public:

    /**
     * @brief getInstance Singleton instance of the class
     * @return
     */
    static RPIRCSController* getInstance();

    RPIRCSResourceObject registerResource(std::string& uri, std::vector<std::string>&& types,
                                          std::vector<std::string>&& interfaces, bool discoverable,
                                          bool observable, bool secured);

    ~RPIRCSController();

private:

    /**
     * @brief m_resourceList Contains all registered resources
     */
    std::unordered_map<std::string, RPIRCSResourceObject::Ptr> m_resourceList;

private:
    RPIRCSController() {}
};

#endif // RPIRCSCONTROLLER_H
