/******************************************************************
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

#include <jni.h>
/* Header for class org_iotivity_ca_caLeClientInterface */

#ifndef CA_Included_org_iotivity_ca_caLeClientInterface_H_
#define CA_Included_org_iotivity_ca_caLeClientInterface_H_
#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeRegisterLeScanCallback
 * Signature: (Landroid/bluetooth/BluetoothAdapter/LeScanCallback;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeRegisterLeScanCallback
(JNIEnv *, jobject, jobject);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeRegisterGattCallback
 * Signature: (Landroid/bluetooth/BluetoothGattCallback;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeRegisterGattCallback
(JNIEnv *, jobject, jobject);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeScanCallback
 * Signature: (Landroid/bluetooth/BluetoothDevice;I[B)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeScanCallback
(JNIEnv *, jobject, jobject);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeGattConnectionStateChangeCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;II)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeGattConnectionStateChangeCallback
(JNIEnv *, jobject, jobject, jint, jint);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeGattNWConnectionStateChangeCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;II)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeGattNWConnectionStateChangeCallback
(JNIEnv *, jobject, jobject, jint, jint);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeGattServicesDiscoveredCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeGattServicesDiscoveredCallback
(JNIEnv *, jobject, jobject, jint);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeGattCharacteristicWritjclasseCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;Landroid/bluetooth/BluetoothGattCharacteristic;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeGattCharacteristicWriteCallback
(JNIEnv *, jobject, jobject, jbyteArray, jint);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeGattCharacteristicChangedCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;Landroid/bluetooth/BluetoothGattCharacteristic;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeGattCharacteristicChangedCallback
(JNIEnv *, jobject, jobject, jbyteArray);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeGattDescriptorWriteCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;Landroid/bluetooth/BluetoothGattDescriptor;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeGattDescriptorWriteCallback
(JNIEnv *, jobject, jobject, jint);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeStateChangedCallback
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeStateChangedCallback
(JNIEnv *, jobject, jint);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caLeBondStateChangedCallback
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_caLeClientInterface_caLeBondStateChangedCallback
(JNIEnv *, jobject, jstring);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caManagerLeGattConnectionStateChangeCB
 * Signature: (Landroid/bluetooth/BluetoothGatt;II)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerLeGattConnectionStateChangeCB
(JNIEnv *, jobject, jobject, jint, jint);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caManagerAdapterStateChangedCallback
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerAdapterStateChangedCallback
(JNIEnv *, jobject, jint);

/*
 * Class:     org_iotivity_ca_caLeClientInterface
 * Method:    caManagerBondStateChangedCallback
 * Signature: (Landroid/bluetooth/BluetoothDevice;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerBondStateChangedCallback
(JNIEnv *, jobject, jobject);

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    caManagerLeServicesDiscoveredCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerLeServicesDiscoveredCallback
(JNIEnv *, jobject, jobject, jint);

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    caManagerLeRemoteRssiCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerLeRemoteRssiCallback
(JNIEnv *, jobject, jobject, jint, jint);

#ifdef __cplusplus
}
#endif
#endif

