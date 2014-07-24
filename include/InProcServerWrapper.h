//******************************************************************
//
// Copyright 2014 Intel Corporation All Rights Reserved.
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

#ifndef _IN_PROC_SERVER_WRAPPER_H_
#define _IN_PROC_SERVER_WRAPPER_H_

#include <thread>
#include <mutex>
#include <ocstack.h>
#include <OCApi.h>
#include <IServerWrapper.h>
#include <OCReflect.h>

using namespace OC::OCReflect;

namespace OC
{
    class InProcServerWrapper : public IServerWrapper
    {
    public:
        InProcServerWrapper(PlatformConfig cfg);
        virtual ~InProcServerWrapper();

        void registerResource(  const std::string& resourceURI, 
                                const std::string& resourceTypeName,
                                property_binding_vector properties); 
								
	private:
		void processFunc();
		std::thread m_processThread;
        bool m_threadRun;
		std::mutex m_csdkLock;
    };
}

#endif
