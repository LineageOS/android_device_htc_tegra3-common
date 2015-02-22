/*
 * Copyright (C) 2014 The CyanogenMod Project
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
 */

package com.android.internal.telephony;

import android.content.Context;
import android.os.Parcel;
import android.provider.Settings;
import android.telephony.Rlog;

public class HTCTegra3RIL extends RIL implements CommandsInterface {
    private static final String LOG_TAG = "HTCTegra3RIL";
    private static boolean T3_RILJ_LOGD = true;

    public static final int RIL_UNSOL_SMART_DATA_ROAMING_STATUS = 3019;
    public static final int RIL_UNSOL_PHONE_BOOK_READY = 3022;
    public static final int RIL_UNSOL_SIM_HOT_SWAP = 3029;
    public static final int RIL_UNSOL_CUSTOMIZE_SIM_INFO = 3050;
    public static final int RIL_UNSOL_SIM_HOT_SWAP_COUNT = 3052;

    public HTCTegra3RIL(Context context, int networkMode, int cdmaSubscription, Integer instanceId) {
        super(context, networkMode, cdmaSubscription, instanceId);

        mQANElements = 5;
    }

    @Override
    protected void processUnsolicited (Parcel p) {
        Object ret;
        int dataPosition = p.dataPosition(); // save off position within the Parcel
        int response = p.readInt();

        try{switch(response) {
            case RIL_UNSOL_SMART_DATA_ROAMING_STATUS: ret = responseVoid(p); break;
            case RIL_UNSOL_PHONE_BOOK_READY: ret = responseVoid(p); break;
            case RIL_UNSOL_SIM_HOT_SWAP: ret = responseInts(p); break;
            case RIL_UNSOL_CUSTOMIZE_SIM_INFO: ret = responseString(p); break;
            case RIL_UNSOL_SIM_HOT_SWAP_COUNT: ret = responseInts(p); break;
            default:
                // Rewind the Parcel
                p.setDataPosition(dataPosition);

                // Forward responses that we are not overriding to the super class
                super.processUnsolicited(p);
                return;
            }
        } catch (Throwable tr) {
            Rlog.e(RILJ_LOG_TAG, "Exception processing unsol response: " + response +
                    "Exception:" + tr.toString());
            return;
        }

        switch (response) {
            case RIL_UNSOL_SMART_DATA_ROAMING_STATUS:
                if (T3_RILJ_LOGD) unsljLog(response);
                break;

            case RIL_UNSOL_PHONE_BOOK_READY:
                if (T3_RILJ_LOGD) unsljLog(response);
                break;

            case RIL_UNSOL_SIM_HOT_SWAP:
                if (T3_RILJ_LOGD) unsljLogRet(response, ret);
                break;

            case RIL_UNSOL_CUSTOMIZE_SIM_INFO:
                if (T3_RILJ_LOGD) unsljLogRet(response, ret);
                break;

            case RIL_UNSOL_SIM_HOT_SWAP_COUNT:
                if (T3_RILJ_LOGD) unsljLogRet(response, ret);
                break;
        }
    }

    /**
     * Logging functions are just here to pick up our responseToString function.
     */
    @Override
    protected void unsljLog(int response) {
        riljLog("[UNSL]< " + responseToString(response));
    }

    @Override
    protected void unsljLogRet(int response, Object ret) {
        riljLog("[UNSL]< " + responseToString(response) + " " + retToString(response, ret));
    }

    static String
    responseToString(int request) {
        switch (request) {
            case RIL_UNSOL_SMART_DATA_ROAMING_STATUS:
                return "RIL_UNSOL_SMART_DATA_ROAMING_STATUS";
            case RIL_UNSOL_PHONE_BOOK_READY:
                return "UNSOL_PHONE_BOOK_READY";
            case RIL_UNSOL_SIM_HOT_SWAP:
                return "UNSOL_SIM_HOT_SWAP";
            case RIL_UNSOL_CUSTOMIZE_SIM_INFO:
                return "UNSOL_CUSTOMIZE_SIM_INFO";
            case RIL_UNSOL_SIM_HOT_SWAP_COUNT:
                return "UNSOL_SIM_HOT_SWAP_COUNT";
            default:
                return RIL.responseToString(request);
        }
    }
}
