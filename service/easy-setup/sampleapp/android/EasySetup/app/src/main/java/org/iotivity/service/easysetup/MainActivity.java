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

package org.iotivity.service.easysetup;

import java.io.IOException;

import org.iotivity.service.easysetup.core.EasySetupService;
import org.iotivity.service.easysetup.core.EasySetupStatus;
import org.iotivity.service.easysetup.core.EnrolleeDevice;
import org.iotivity.service.easysetup.core.EnrolleeState;
import org.iotivity.service.easysetup.impl.EnrolleeDeviceFactory;
import org.iotivity.service.easysetup.impl.WiFiOnBoardingConfig;
import org.iotivity.service.easysetup.impl.WiFiProvConfig;
//import org.iotivity.service.easysetup.mediator.EasySetupManager;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.wifi.WifiConfiguration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {


    /*Status to update the UI */
    public static final int SUCCESS = 0;
    public static final int FAILED = 1;
    public static final int STATE_CHANGED = 2;

    private boolean mRunningStatus = false;

    static final int REQUEST_IMAGE_CAPTURE = 1;
    ImageView imageView;

    EditText mSsidText;
    EditText mPassText;
    TextView mResultTextView;
    ProgressBar mProgressbar;
    Button mStartButton;
    Button mStopButton;
    Handler mHandler = new ThreadHandler();

    /**
     * Objects to be instantiated by the programmer
     */
    WiFiProvConfig mWiFiProvConfig;
    WiFiOnBoardingConfig mWiFiOnBoardingConfig;
    EasySetupService mEasySetupService;
    EnrolleeDeviceFactory mDeviceFactory;
    EnrolleeDevice mDevice;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        /* Initialize widgets to get user input for target network's SSID & password*/
        mSsidText = (EditText) findViewById(R.id.ssid);
        mPassText = (EditText) findViewById(R.id.password);
        mResultTextView = (TextView) findViewById(R.id.status);
        mProgressbar = (ProgressBar) findViewById(R.id.progressBar);


       /* Create Easy Setup Service instance*/
        mEasySetupService = EasySetupService.getInstance(getApplicationContext(),
                new EasySetupStatus() {

                    @Override
                    public void onFinished(final EnrolleeDevice enrolledevice) {
                        mRunningStatus = false;
                        if (enrolledevice.isSetupSuccessful()) {
                            mHandler.sendEmptyMessage(SUCCESS);
                        } else {
                            mHandler.sendEmptyMessage(FAILED);
                        }
                    }

                    @Override
                    public void onProgress(EnrolleeState state) {
                        mHandler.sendEmptyMessage(STATE_CHANGED);
                    }

                });

        /* Create EnrolleeDevice Factory instance*/
        mDeviceFactory = EnrolleeDeviceFactory.newInstance(getApplicationContext());

        /* Create a device using Factory instance*/
        mDevice = mDeviceFactory.newEnrolleeDevice(getOnBoardingWifiConfig(),
                getEnrollerWifiConfig());

        addListenerForStartAP();
        addListenerForStopAP();
    }

    public WiFiProvConfig getEnrollerWifiConfig() {
        /* Provide the credentials for the Mediator Soft AP to be connected by Enrollee*/
        mWiFiProvConfig = new WiFiProvConfig("EasySetup123", "EasySetup123");
        return mWiFiProvConfig;
    }

    public WiFiOnBoardingConfig getOnBoardingWifiConfig() {
        mWiFiOnBoardingConfig = new WiFiOnBoardingConfig();

        /* Provide the target credentials to be provisioned to the Enrollee by Mediator*/
        mWiFiOnBoardingConfig.setSSId("EasySetup123");
        mWiFiOnBoardingConfig.setSharedKey("EasySetup123");
        mWiFiOnBoardingConfig.setAuthAlgo(WifiConfiguration.AuthAlgorithm.OPEN);
        mWiFiOnBoardingConfig.setKms(WifiConfiguration.KeyMgmt.WPA_PSK);

        // Updating the UI with default credentials
        mSsidText.setText("EasySetup123");
        mPassText.setText("EasySetup123");

        return mWiFiOnBoardingConfig;
    }


    public void onDestroy() {
        super.onDestroy();
        /*Reset the Easy setup process*/
        mEasySetupService.finish();
    }

    public void addListenerForStartAP() {
        mStartButton = (Button) findViewById(R.id.startSetup);

        mStartButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View arg0) {
                try {

                    mRunningStatus = true;
                    mProgressbar.setVisibility(View.VISIBLE);
                    mProgressbar.setIndeterminate(true);
                    mStartButton.setEnabled(false);
                    mResultTextView.setText(R.string.running);

                    String ssid = mSsidText.getText().toString();
                    String password = mPassText.getText().toString();

                    mWiFiOnBoardingConfig.setSSId(ssid);
                    mWiFiOnBoardingConfig.setSharedKey(password);

                    mEasySetupService.startSetup(mDevice);

                    mStopButton.setEnabled(true);


                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
    }

    public void addListenerForStopAP() {
        mStopButton = (Button) findViewById(R.id.stopSetup);

        mStopButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View arg0) {
                mRunningStatus = false;
                mStartButton.setEnabled(true);
                mStopButton.setEnabled(false);
                mResultTextView.setText(R.string.stopped);
                mProgressbar.setIndeterminate(false);
                mProgressbar.setVisibility(View.INVISIBLE);
                mEasySetupService.stopSetup(mDevice);
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == RESULT_OK) {
            Bundle extras = data.getExtras();
            Bitmap imageBitmap = (Bitmap) extras.get("data");
            imageView.setImageBitmap(imageBitmap);
        }
    }

    class ThreadHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {

            // Returns if Test is stopped, this has to be handled in EasySetupService
            if (!mRunningStatus) return;

            switch (msg.what) {
                case SUCCESS: {

                    mProgressbar.setIndeterminate(false);
                    mStopButton.setEnabled(false);
                    mStartButton.setEnabled(true);
                    mProgressbar.setVisibility(View.INVISIBLE);
                    String resultMsg = "Device configured successfully";
                    mResultTextView.setText(R.string.success);
                    Toast.makeText(getApplicationContext(), resultMsg, Toast.LENGTH_SHORT).show();
                    break;
                }
                case FAILED: {

                    mProgressbar.setIndeterminate(false);
                    mStopButton.setEnabled(false);
                    mStartButton.setEnabled(true);
                    mProgressbar.setVisibility(View.INVISIBLE);
                    String resultMsg = "Device configuration failed";
                    mResultTextView.setText(R.string.failed);
                    Toast.makeText(getApplicationContext(), resultMsg, Toast.LENGTH_SHORT).show();
                    break;
                }

                case STATE_CHANGED: {
                    String resultMsg = "Device state changed";
                    Toast.makeText(getApplicationContext(), resultMsg, Toast.LENGTH_SHORT).show();
                    break;
                }

            }


        }
    }

}
