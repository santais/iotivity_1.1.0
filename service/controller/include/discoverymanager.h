#ifndef DISCOVERYMANAGER
#define DISCOVERYMANAGER

#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "resource_types.h"

#include "OCPlatform.h"
#include "OCApi.h"
#include "ExpiryTimer.h"
#include "rd_server.h"

#include "PrimitiveResource.h"
#include "RCSResourceObject.h"
#include "RCSRemoteResourceObject.h"
#include "RCSDiscoveryManager.h"

#include "ResourceObject.hpp"

// Scene-manager
#include "SceneList.h"


namespace OIC { namespace Service
{

    using namespace OC;
    using namespace OIC;
    using namespace OIC::Service;

    const std::string HOSTING_TAG = "/hosting";
    const auto HOSTING_TAG_SIZE = HOSTING_TAG.size();


    enum class SceneState
    {
        START_SCENE,
        STOP_SCENE
    };

    /**
     * @brief The DiscoveryManagerInfo class
     */
    class DiscoveryManagerInfo
    {
    public:
        /**
         * @brief DiscoveryManagerInfo
         */
        DiscoveryManagerInfo();

        /**
         * @brief DiscoveryManagerInfo
         * @param host
         * @param uri
         * @param types
         * @param cb
         */
        DiscoveryManagerInfo(const std::string& host, const std::string& uri,
                             const std::vector<std::string>& types, FindCallback cb);


        /**
         * @brief discover
         */
        void discover() const;

    private:
        std::string m_host;
        std::string m_relativeUri;
        std::vector<std::string> m_resourceTypes;
        FindCallback m_discoveryCb;
    };

    /**
     * @brief The DiscoveryManager class
     *
     * Discovers resource on the network
     */
    class DiscoveryManager
    {
        typedef long long cbTimer;

    public:
        /**
         * @brief DiscoveryManager
         * @param time_ms
         */
        DiscoveryManager(cbTimer timeMs);
        DiscoveryManager()                                          = default;
        DiscoveryManager(const DiscoveryManager& dm)                = default;
        DiscoveryManager(DiscoveryManager&& dm)                     = default;
        DiscoveryManager& operator=(const DiscoveryManager& dm)     = default;
        DiscoveryManager& operator=(DiscoveryManager&& dm)          = default;

        ~DiscoveryManager();

        /**
         * @brief isSearching
         * @return
         */
        bool isSearching() const;

        /**
         * @brief cancel
         */
        void cancel();

        /**
         * @brief setTimer
         * @param time_ms
         */
        void setTimer(cbTimer time_ms);

        /**
         * @brief discoverResource
         * @param types
         * @param cb
         * @param host
         */
        void discoverResource(const std::string& uri, const std::vector<std::string>& types, FindCallback cb,
                              std::string host = "");

        /**
         * @brief discoverResource
         * @param type
         * @param cb
         * @param host
         */
        void discoverResource(const std::string& uri, const std::string& type, FindCallback cb,
                              std::string host = "");
    private:
        /**
         * @brief m_timer
         */
        ExpiryTimer m_timer;

        /**
         * @brief m_timerMs
         */
        cbTimer m_timerMs;

        /**
         * @brief m_isRunning
         */
        bool m_isRunning;

        /**
         * @brief m_discoveryInfo
         */
        DiscoveryManagerInfo m_discoveryInfo;

        /**
         * @brief m_cancelMutex
         */
        std::mutex m_discoveryMutex;

    private:

        /**
         * @brief timeOutCB
         * @param id
         */
        void timeOutCB();
    };


#endif // DISCOVERYMANAGER

