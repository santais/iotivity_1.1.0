/**
 * ***************************************************************
 * <p/>
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 * <p/>
 * <p/>
 * <p/>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p/>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p/>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <p/>
 * ****************************************************************
 */

package org.iotivity.service.easysetup.impl;

import java.util.Timer;
import java.util.TimerTask;

import org.iotivity.service.easysetup.core.OnBoardingConnection;
import org.iotivity.service.easysetup.core.EnrolleeDevice;
import org.iotivity.service.easysetup.core.EnrolleeState;
import org.iotivity.service.easysetup.core.IpOnBoardingConnection;
import org.iotivity.service.easysetup.core.OnBoardingConfig;
import org.iotivity.service.easysetup.core.ProvisioningConfig;
import org.iotivity.service.easysetup.mediator.EasySetupManager;
import org.iotivity.service.easysetup.mediator.EnrolleeInfo;
import org.iotivity.service.easysetup.mediator.IOnBoardingStatus;
import org.iotivity.service.easysetup.mediator.IProvisioningListener;
import org.iotivity.service.easysetup.mediator.ProvisionEnrollee;
import org.iotivity.service.easysetup.mediator.ip.WiFiSoftAPManager;

import android.content.Context;
import android.net.wifi.WifiConfiguration;
import android.util.Log;

/**
 * This is a ready to use class for Enrollee device having Soft AP as on-boarding connectivity.
 */
public class EnrolleeDeviceWiFiOnboarding extends EnrolleeDevice {

    public static final String TAG = EnrolleeDeviceWiFiOnboarding.class.getName();

    final Context mContext;
    final WiFiSoftAPManager mWifiSoftAPManager;
    EnrolleeInfo connectedDevice;
    private EasySetupManager easySetupManagerNativeInstance;
    ProvisionEnrollee provisionEnrolleInstance;

    // Native Api to start provisioning process after successful on-boarding on Wifi AP.
    // Library is already loaded while constructing EasySetupService
    private native void ProvisionEnrollee(String ipAddress, String netSSID,
                                          String netPWD, int connectivityType);

    IOnBoardingStatus deviceScanListener = new IOnBoardingStatus() {

        @Override
        public void deviceOnBoardingStatus(EnrolleeInfo enrolleStatus) {
            Log.d("ESSoftAPOnBoarding", "Entered");
            if (enrolleStatus != null && enrolleStatus.getIpAddr() != null) {
                String finalResult = "Easy Connect : ";

                if (enrolleStatus.isReachable()) {
                    finalResult = "Device OnBoarded" + "["
                            + enrolleStatus.getIpAddr() + "]";

                    connectedDevice = enrolleStatus;
                    IpOnBoardingConnection conn = new IpOnBoardingConnection();
                    conn.setConnectivity(true);
                    conn.setIp(connectedDevice.getIpAddr());
                    Log.d("ESSoftAPOnBoarding", "Entered");
                    mOnBoardingCallback.onFinished(conn);
                    return;

                }
            }

            IpOnBoardingConnection conn = new IpOnBoardingConnection();
            conn.setConnectivity(false);
            mOnBoardingCallback.onFinished(conn);
        }
    };


    protected EnrolleeDeviceWiFiOnboarding(Context context, OnBoardingConfig onBoardingConfig, ProvisioningConfig provConfig) {
        super(onBoardingConfig, provConfig);
        mContext = context;
        mWifiSoftAPManager = new WiFiSoftAPManager(mContext);
    }

    @Override
    protected void startOnBoardingProcess() {
        Log.i(TAG, "Starging on boarding process");

        //1. Create Soft AP
        boolean status = mWifiSoftAPManager.setWifiApEnabled((WifiConfiguration) mOnBoardingConfig.getConfig(), true);

        Log.i(TAG, "Soft AP is created with status " + status);

        Timer myTimer;
        myTimer = new Timer();
        myTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                // Below function to be called after 5 seconds
                mWifiSoftAPManager.getClientList(deviceScanListener, 300);
            }

        }, 0, 5000);
    }

    protected void stopOnBoardingProcess() {
        Log.i(TAG, "Stopping on boarding process");
        boolean status = mWifiSoftAPManager.setWifiApEnabled(null, false);
        Log.i(TAG, "Soft AP is disabled with status " + status);
    }

    @Override
    protected void startProvisioningProcess(OnBoardingConnection conn) {

        if (mProvConfig.getConnType() == ProvisioningConfig.ConnType.WiFi) {

            provisionEnrolleInstance = new ProvisionEnrollee(mContext);
            provisionEnrolleInstance.registerProvisioningHandler(new IProvisioningListener() {
                @Override
                public void onFinishProvisioning(int statuscode) {
                    mState = (statuscode == 0) ? EnrolleeState.DEVICE_PROVISIONING_SUCCESS_STATE : EnrolleeState.DEVICE_PROVISIONING_FAILED_STATE;
                    mProvisioningCallback.onFinished(EnrolleeDeviceWiFiOnboarding.this);
                }
            });

            IpOnBoardingConnection connection = (IpOnBoardingConnection) conn;
            WiFiProvConfig wifiProvConfig = (WiFiProvConfig) mProvConfig;

            // Native Api call to start provisioning of the enrolling device
            EasySetupManager.getInstance().provisionEnrollee(connection.getIp(), wifiProvConfig.getSsId(), wifiProvConfig.getPassword(), mOnBoardingConfig.getConnType().getValue());

        }

    }


}
