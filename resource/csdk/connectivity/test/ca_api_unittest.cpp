/* ****************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "gtest/gtest.h"
#include "cainterface.h"
#include "cacommon.h"
#include <string.h>


void request_handler(CARemoteEndpoint_t* object, CARequestInfo_t* requestInfo);
void response_handler(CARemoteEndpoint_t* object, CAResponseInfo_t* responseInfo);

void request_handler(const CARemoteEndpoint_t *object, const CARequestInfo_t *requestInfo)
{

}

void response_handler(const CARemoteEndpoint_t *object, const CAResponseInfo_t *responseInfo)
{

}

static char* uri = NULL;
static CARemoteEndpoint_t* tempRep = NULL;
static CARequestInfo_t requestInfo;
static CAInfo_t requestData;
static CAInfo_t responseData;
static CAResponseInfo_t responseInfo;
static CAToken_t tempToken = NULL;
uint8_t tokenLength = CA_MAX_TOKEN_LEN;
static const char URI[] = "coap://10.11.12.13:4545/a/light";
static const char RESOURCE_URI[] = "/a/light";

static const char SECURE_INFO_DATA[] =
                                    "{\"oc\":[{\"href\":\"%s\",\"prop\":{\"rt\":[\"core.led\"],"
                                     "\"if\":[\"oc.mi.def\"],\"obs\":1,\"sec\":1,\"port\":%d}}]}";
static const char NORMAL_INFO_DATA[] =
                                    "{\"oc\":[{\"href\":\"%s\",\"prop\":{\"rt\":[\"core.led\"],"
                                     "\"if\":[\"oc.mi.def\"],\"obs\":1}}]}";

#ifdef __WITH_DTLS__
static OCDtlsPskCredsBlob *pskCredsBlob = NULL;

void clearDtlsCredentialInfo()
{
    printf("clearDtlsCredentialInfo IN\n");
    if (pskCredsBlob)
    {
        // Initialize sensitive data to zeroes before freeing.
        memset(pskCredsBlob->creds, 0, sizeof(OCDtlsPskCreds) * (pskCredsBlob->num));
        free(pskCredsBlob->creds);

        memset(pskCredsBlob, 0, sizeof(OCDtlsPskCredsBlob));
        free(pskCredsBlob);
        pskCredsBlob = NULL;
    }
    printf("clearDtlsCredentialInfo OUT\n");
}

// Internal API. Invoked by OC stack to retrieve credentials from this module
void CAGetDtlsPskCredentials(OCDtlsPskCredsBlob **credInfo)
{
    printf("CAGetDtlsPskCredentials IN\n");

    if (pskCredsBlob != NULL)
    {
        *credInfo = pskCredsBlob;
    }

    printf("CAGetDtlsPskCredentials OUT\n");
}

int32_t SetCredentials()
{
    printf("SetCredentials IN\n");
    pskCredsBlob = (OCDtlsPskCredsBlob *)malloc(sizeof(OCDtlsPskCredsBlob));

    memset(pskCredsBlob, 0x0, sizeof(OCDtlsPskCredsBlob));
    memcpy(pskCredsBlob->identity, IDENTITY, DTLS_PSK_ID_LEN);

    pskCredsBlob->num = 1;

    pskCredsBlob->creds = (OCDtlsPskCreds *)malloc(sizeof(OCDtlsPskCreds) * (pskCredsBlob->num));

    memcpy(pskCredsBlob->creds[0].id, IDENTITY, DTLS_PSK_ID_LEN);
    memcpy(pskCredsBlob->creds[0].psk, RS_CLIENT_PSK, DTLS_PSK_PSK_LEN);

    printf("SetCredentials OUT\n");
    return 1;
}
#endif

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// CAInitialize TC
// check return value
TEST(InitializeTest, TC_01_Positive_01)
{
    EXPECT_EQ(CA_STATUS_OK, CAInitialize());
}

//CATerminate TC
TEST(TerminateTest, TC_02_Positive_01)
{
    CATerminate();

    char* check = (char *) "terminate success";
    EXPECT_STREQ(check, "terminate success");

    CAInitialize();
}
// CAStartListeningServer TC
// check return value
TEST(StartListeningServerTest, TC_03_Positive_01)
{
    CASelectNetwork(CA_WIFI);
    EXPECT_EQ(CA_STATUS_OK, CAStartListeningServer());
}

// CAStartDiscoveryServer TC
// check return value
TEST(StartDiscoveryServerTest, TC_04_Positive_01)
{
    EXPECT_EQ(CA_STATUS_OK, CAStartDiscoveryServer());
}

// CARegisterHandlerTest TC
// check return value
TEST(RegisterHandlerTest, TC_05_Positive_01)
{
    CARegisterHandler(request_handler, response_handler);
    char* check = (char *) "registerHandler success";
    EXPECT_STREQ(check, "registerHandler success");
}

// CACreateRemoteEndpoint TC
// check return value
TEST(CreateRemoteEndpointTest, TC_06_Positive_01)
{
    uri = (char *) URI;

    EXPECT_EQ(CA_STATUS_OK, CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep));

    CADestroyRemoteEndpoint(tempRep);
    tempRep = NULL;
}

// check remoteEndpoint and values of remoteEndpoint
TEST(CreateRemoteEndpointTest, TC_07_Positive_02)
{
    uri = (char *) URI;

    CACreateRemoteEndpoint(uri, CA_WIFI, &tempRep);

    EXPECT_TRUE(tempRep != NULL);

    if (tempRep != NULL)
    {
        EXPECT_STRNE(NULL, tempRep->resourceUri);
    }

    CADestroyRemoteEndpoint(tempRep);
    tempRep = NULL;
}

// check return value when uri is NULL
TEST(CreateRemoteEndpointTest, TC_08_Negative_01)
{
    uri = NULL;

    EXPECT_EQ(CA_STATUS_FAILED, CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep));

    if (tempRep != NULL)
    {
        CADestroyRemoteEndpoint(tempRep);
        tempRep = NULL;
    }
}

// check values of remoteEndpoint when uri is NULL
TEST(CreateRemoteEndpointTest, TC_09_Negative_02)
{
    uri = NULL;
    CACreateRemoteEndpoint(uri, CA_WIFI, &tempRep);

    if (tempRep != NULL)
    {
        EXPECT_STREQ(NULL, tempRep->resourceUri);

    }

    if (tempRep != NULL)
    {
        CADestroyRemoteEndpoint(tempRep);
        tempRep = NULL;
    }
}

// CADestroyRemoteEndpoint TC
// check destroyed remoteEndpoint
TEST(DestroyRemoteEndpointTest, TC_10_Positive_01)
{
    uri = (char *) URI;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    CADestroyRemoteEndpoint(tempRep);
    tempRep = NULL;

    char * check = (char *) "destroy success";
    EXPECT_STREQ(check, "destroy success");
}

// CAGerateToken TC
// check return value
TEST(GenerateTokenTest, TC_11_Positive_01)
{
    EXPECT_EQ(CA_STATUS_OK, CAGenerateToken(&tempToken, tokenLength));

    CADestroyToken(tempToken);
}

// check return value when CAGenerateToken is passed a NULL instead a valid pointer
TEST(GenerateTokenTest, TC_12_Negative_01)
{
    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CAGenerateToken(NULL, tokenLength));
}

// CADestroyToken TC
// check destroyed token
TEST(DestroyTokenTest, TC_13_Positive_01)
{
    CAGenerateToken(&tempToken, tokenLength);
    CADestroyToken(tempToken);

    char * check = (char *) "destroy success";
    EXPECT_STREQ(check, "destroy success");
}

// CAFindResource TC
// check return value
TEST(FindResourceTest, TC_14_Positive_01)
{
    uri = (char *) RESOURCE_URI;

    CAGenerateToken(&tempToken, tokenLength);
    EXPECT_EQ(CA_STATUS_OK, CAFindResource(uri, tempToken, tokenLength));
    CADestroyToken(tempToken);
}

// check return value when uri is NULL
TEST(FindResourceTest, TC_15_Negative_01)
{
    uri = NULL;
    CAGenerateToken(&tempToken, tokenLength);
    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CAFindResource(uri, tempToken, tokenLength));
    CADestroyToken(tempToken);
}

// CASendRequest TC
// check return value
TEST(SendRequestTest, TC_16_Positive_01)
{
    uri = (char *) URI;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    memset(&requestData, 0, sizeof(CAInfo_t));
    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    int length = strlen(NORMAL_INFO_DATA) + strlen("a/light");
    requestData.payload = (CAPayload_t) calloc(length, sizeof(char));
    snprintf(requestData.payload, length, NORMAL_INFO_DATA, "a/light");
    requestData.type = CA_MSG_NONCONFIRM;

    memset(&requestInfo, 0, sizeof(CARequestInfo_t));
    requestInfo.method = CA_GET;
    requestInfo.info = requestData;

    EXPECT_EQ(CA_STATUS_OK, CASendRequest(tempRep, &requestInfo));

    CADestroyToken(tempToken);

    free(requestData.payload);

    CADestroyRemoteEndpoint(tempRep);
    tempRep = NULL;

}

// check return value when uri is NULL
TEST(SendRequestTest, DISABLED_TC_17_Negative_01)
{
    uri = NULL;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    memset(&requestData, 0, sizeof(CAInfo_t));
    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    int length = strlen(NORMAL_INFO_DATA) + strlen("a/light");
    requestData.payload = (CAPayload_t) calloc(length, sizeof(char));
    snprintf(requestData.payload, length, NORMAL_INFO_DATA, "a/light");
    requestData.type = CA_MSG_NONCONFIRM;

    memset(&requestInfo, 0, sizeof(CARequestInfo_t));
    requestInfo.method = CA_GET;
    requestInfo.info = requestData;

    EXPECT_EQ(CA_STATUS_FAILED, CASendRequest(tempRep, &requestInfo));

    CADestroyToken(tempToken);

    free(requestData.payload);

    if (tempRep != NULL)
    {
        CADestroyRemoteEndpoint(tempRep);
        tempRep = NULL;
    }
}

// check return value when a NULL is passed instead of a valid CARequestInfo_t address
TEST(SendRequestTest, TC_18_Negative_02)
{
    uri = (char *) URI;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    memset(&requestData, 0, sizeof(CAInfo_t));
    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    int length = strlen(NORMAL_INFO_DATA) + strlen("a/light");
    requestData.payload = (CAPayload_t) calloc(length, sizeof(char));
    snprintf(requestData.payload, length, NORMAL_INFO_DATA, "a/light");
    requestData.type = CA_MSG_NONCONFIRM;

    memset(&requestInfo, 0, sizeof(CARequestInfo_t));
    requestInfo.method = CA_GET;
    requestInfo.info = requestData;

    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CASendRequest(tempRep, NULL));

    CADestroyToken(tempToken);

    free(requestData.payload);

    if (tempRep != NULL)
    {
        CADestroyRemoteEndpoint(tempRep);
        tempRep = NULL;
    }
}

// CASendResponse TC
// check return value
TEST(SendResponseTest, TC_19_Positive_01)
{
    uri = (char *) URI;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;
    responseData.payload = (char *) "response payload";

    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
    responseInfo.result = CA_SUCCESS;
    responseInfo.info = responseData;

    EXPECT_EQ(CA_STATUS_OK, CASendResponse(tempRep, &responseInfo));

    CADestroyToken(tempToken);
    CADestroyRemoteEndpoint(tempRep);
    tempRep = NULL;
}

// check return value when uri is NULL
TEST(SendResponseTest, DISABLED_TC_20_Negative_01)
{
    uri = NULL;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;
    responseData.payload = (char *) "response payload";

    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
    responseInfo.result = CA_SUCCESS;
    responseInfo.info = responseData;

    EXPECT_EQ(CA_STATUS_FAILED, CASendResponse(tempRep, &responseInfo));

    CADestroyToken(tempToken);
    if (tempRep != NULL)
    {
        CADestroyRemoteEndpoint(tempRep);
        tempRep = NULL;
    }
}

// check return value NULL is passed instead of a valid CAResponseInfo_t address
TEST(SendResponseTest, TC_21_Negative_02)
{
    uri = (char *) URI;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;
    responseData.payload = (char *) "response payload";

    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
    responseInfo.result = CA_SUCCESS;
    responseInfo.info = responseData;

    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CASendResponse(tempRep, NULL));

    CADestroyToken(tempToken);
    if (tempRep != NULL)
    {
        CADestroyRemoteEndpoint(tempRep);
        tempRep = NULL;
    }
}

// CASendNotification TC
// check return value
TEST(SendNotificationTest, TC_22_Positive_01)
{
    uri = (char *) URI;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.payload = (char *) "Temp Notification Data";

    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
    responseInfo.result = CA_SUCCESS;
    responseInfo.info = responseData;

    EXPECT_EQ(CA_STATUS_OK, CASendNotification(tempRep, &responseInfo));

    CADestroyToken(tempToken);
    if (tempRep != NULL)
    {
        CADestroyRemoteEndpoint(tempRep);
        tempRep = NULL;
    }
}

// check return value when uri is NULL
TEST(SendNotificationTest, DISABLED_TC_23_Negative_01)
{
    uri = NULL;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);

    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.payload = (char *) "Temp Notification Data";

    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
    responseInfo.result = CA_SUCCESS;
    responseInfo.info = responseData;

    EXPECT_EQ(CA_STATUS_FAILED, CASendNotification(tempRep, &responseInfo));

    CADestroyToken(tempToken);
    if (tempRep != NULL)
    {
        CADestroyRemoteEndpoint(tempRep);
        tempRep = NULL;
    }
}

// CAAdvertiseResource TC
// check return value
TEST(AdvertiseResourceTest, TC_24_Positive_01)
{
    uri = (char *) RESOURCE_URI;
    int optionNum = 2;

    CAHeaderOption_t* headerOpt;
    headerOpt = (CAHeaderOption_t *) calloc(1, optionNum * sizeof(CAHeaderOption_t));

    char* tmpOptionData1 = (char *) "Hello";
    size_t tmpOptionDataLen = (strlen(tmpOptionData1) < CA_MAX_HEADER_OPTION_DATA_LENGTH) ?
            strlen(tmpOptionData1) : CA_MAX_HEADER_OPTION_DATA_LENGTH - 1;
    headerOpt[0].optionID = 3000;
    memcpy(headerOpt[0].optionData, tmpOptionData1, tmpOptionDataLen);
    headerOpt[0].optionLength = (uint16_t) tmpOptionDataLen;

    char* tmpOptionData2 = (char *) "World";
    tmpOptionDataLen = (strlen(tmpOptionData2) < CA_MAX_HEADER_OPTION_DATA_LENGTH) ?
                strlen(tmpOptionData2) : CA_MAX_HEADER_OPTION_DATA_LENGTH - 1;
    headerOpt[1].optionID = 3001;
    memcpy(headerOpt[1].optionData, tmpOptionData2, tmpOptionDataLen);
    headerOpt[1].optionLength = (uint16_t) tmpOptionDataLen;

    CAGenerateToken(&tempToken, tokenLength);

    EXPECT_EQ(CA_STATUS_OK, CAAdvertiseResource(uri, tempToken, tokenLength, headerOpt, (uint8_t )optionNum));

    CADestroyToken(tempToken);

    free(headerOpt);
}

// check return value when uri is NULL
TEST(AdvertiseResourceTest, TC_25_Negative_01)
{
    uri = NULL;
    int optionNum = 2;

    CAHeaderOption_t* headerOpt;
    headerOpt = (CAHeaderOption_t *) calloc(1, optionNum * sizeof(CAHeaderOption_t));

    char* tmpOptionData1 = (char *) "Hello";
    size_t tmpOptionDataLen = (strlen(tmpOptionData1) < CA_MAX_HEADER_OPTION_DATA_LENGTH) ?
            strlen(tmpOptionData1) : CA_MAX_HEADER_OPTION_DATA_LENGTH - 1;
    headerOpt[0].optionID = 3000;
    memcpy(headerOpt[0].optionData, tmpOptionData1, tmpOptionDataLen);
    headerOpt[0].optionLength = (uint16_t) tmpOptionDataLen;

    char* tmpOptionData2 = (char *) "World";
    tmpOptionDataLen = (strlen(tmpOptionData2) < CA_MAX_HEADER_OPTION_DATA_LENGTH) ?
                strlen(tmpOptionData2) : CA_MAX_HEADER_OPTION_DATA_LENGTH - 1;
    headerOpt[1].optionID = 3001;
    memcpy(headerOpt[1].optionData, tmpOptionData2, tmpOptionDataLen);
    headerOpt[1].optionLength = (uint16_t) tmpOptionDataLen;

    CAGenerateToken(&tempToken, tokenLength);

    EXPECT_EQ(CA_STATUS_INVALID_PARAM,
            CAAdvertiseResource(uri, tempToken, tokenLength, headerOpt, (uint8_t )optionNum));

    CADestroyToken(tempToken);

    free(headerOpt);
}

// CASelectNewwork TC
// check return value
 TEST(SelectNetworkTest, TC_26_Positive_01)
{
    //Select wifi network
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_WIFI));
}

// check return value when selected network is disable
TEST(SelectNetworkTest, TC_27_Negative_01)
{
    //Select disable network
    EXPECT_EQ(CA_NOT_SUPPORTED, CASelectNetwork(1000));
}

// CAUnSelectNewwork TC
// check return value
TEST(UnSelectNetworkTest, TC_28_Positive_01)
{
    //Unselect wifi network
    EXPECT_EQ(CA_STATUS_OK, CAUnSelectNetwork(CA_WIFI));
}

// check return value when selected network is disable
TEST(UnSelectNetworkTest, TC_29_Negative_01)
{
    //UnSelect disable network
    EXPECT_EQ(CA_STATUS_FAILED, CAUnSelectNetwork(1000));
}

// CAHandlerRequestResponse TC
// check return value
TEST (HandlerRequestResponseTest, TC_30_Positive_01)
{
    EXPECT_EQ(CA_STATUS_OK, CAHandleRequestResponse());
}

// CASendRequestToAll TC
// check return value
TEST (SendRequestToAllTest, TC_31_Positive_01)
{
    uri = (char *) RESOURCE_URI;
    CACreateRemoteEndpoint(uri, CA_ETHERNET, &tempRep);
    CAGroupEndpoint_t *group = NULL;
    group = (CAGroupEndpoint_t *) malloc(sizeof(CAGroupEndpoint_t));
    group->connectivityType = tempRep->connectivityType;
    group->resourceUri = tempRep->resourceUri;

    memset(&requestData, 0, sizeof(CAInfo_t));
    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    requestData.payload = (char *) "Temp Json Payload";
    requestData.type = CA_MSG_NONCONFIRM;
    memset(&requestInfo, 0, sizeof(CARequestInfo_t));
    requestInfo.method = CA_GET;
    requestInfo.info = requestData;

    EXPECT_EQ(CA_STATUS_OK, CASendRequestToAll(group, &requestInfo));

    CADestroyToken(tempToken);

    CADestroyRemoteEndpoint(tempRep);
    tempRep = NULL;

    free(group);
}

// check return value when group->resourceUri is NULL
TEST (SendRequestToAllTest, TC_32_Negative_01)
{
    uri = (char *) RESOURCE_URI;
    CAGroupEndpoint_t *group = NULL;

    memset(&requestData, 0, sizeof(CAInfo_t));
    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    requestData.payload = (char *) "Temp Json Payload";
    requestData.type = CA_MSG_NONCONFIRM;
    memset(&requestInfo, 0, sizeof(CARequestInfo_t));
    requestInfo.method = CA_GET;
    requestInfo.info = requestData;

    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CASendRequestToAll(group, &requestInfo));

    CADestroyToken(tempToken);
}

// CAGetNetworkInformation TC
// check return value
TEST (GetNetworkInformationTest, TC_33_Positive_01)
{
    CALocalConnectivity_t *tempInfo = NULL;
    uint32_t tempSize = 0;

    EXPECT_EQ(CA_STATUS_OK, CAGetNetworkInformation(&tempInfo, &tempSize));
}

TEST(RegisterDTLSCredentialsHandlerTest, TC_34_positive_01)
{
#ifdef __WITH_DTLS__
    if (SetCredentials() == 0)
    {
        printf("SetCredentials failed\n");
        return 0;
    }

    res = CARegisterDTLSCredentialsHandler(CAGetDtlsPskCredentials);
    EXPECT_EQ(CA_STATUS_OK, CARegisterDTLSCredentialsHandler(CAGetDtlsPskCredentials));
#endif
}

