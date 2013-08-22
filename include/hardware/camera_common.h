/*
 * Copyright (C) 2012 The Android Open Source Project
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

// FIXME: add well-defined names for cameras

#ifndef ANDROID_INCLUDE_CAMERA_COMMON_H
#define ANDROID_INCLUDE_CAMERA_COMMON_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <cutils/native_handle.h>
#include <system/camera.h>
#include <hardware/hardware.h>
#include <hardware/gralloc.h>

__BEGIN_DECLS

/**
 * The id of this module
 */
#define CAMERA_HARDWARE_MODULE_ID "camera"

/**
 * Module versioning information for the Camera hardware module, based on
 * camera_module_t.common.module_api_version. The two most significant hex
 * digits represent the major version, and the two least significant represent
 * the minor version.
 *
 *******************************************************************************
 * Versions: 0.X - 1.X [CAMERA_MODULE_API_VERSION_1_0]
 *
 *   Camera modules that report these version numbers implement the initial
 *   camera module HAL interface. All camera devices openable through this
 *   module support only version 1 of the camera device HAL. The device_version
 *   and static_camera_characteristics fields of camera_info are not valid. Only
 *   the android.hardware.Camera API can be supported by this module and its
 *   devices.
 *
 *******************************************************************************
 * Version: 2.0 [CAMERA_MODULE_API_VERSION_2_0]
 *
 *   Camera modules that report this version number implement the second version
 *   of the camera module HAL interface. Camera devices openable through this
 *   module may support either version 1.0 or version 2.0 of the camera device
 *   HAL interface. The device_version field of camera_info is always valid; the
 *   static_camera_characteristics field of camera_info is valid if the
 *   device_version field is 2.0 or higher.
 *
 *******************************************************************************
 * Version: 2.1 [CAMERA_MODULE_API_VERSION_2_1]
 *
 *   This camera module version adds support for asynchronous callbacks to the
 *   framework from the camera HAL module, which is used to notify the framework
 *   about changes to the camera module state. Modules that provide a valid
 *   set_callbacks() method must report at least this version number.
 */

/**
 * Predefined macros for currently-defined version numbers
 */

/**
 * All module versions <= HARDWARE_MODULE_API_VERSION(1, 0xFF) must be treated
 * as CAMERA_MODULE_API_VERSION_1_0
 */
#define CAMERA_MODULE_API_VERSION_1_0 HARDWARE_MODULE_API_VERSION(1, 0)
#define CAMERA_MODULE_API_VERSION_2_0 HARDWARE_MODULE_API_VERSION(2, 0)
#define CAMERA_MODULE_API_VERSION_2_1 HARDWARE_MODULE_API_VERSION(2, 1)

#define CAMERA_MODULE_API_VERSION_CURRENT CAMERA_MODULE_API_VERSION_2_1

/**
 * All device versions <= HARDWARE_DEVICE_API_VERSION(1, 0xFF) must be treated
 * as CAMERA_DEVICE_API_VERSION_1_0
 */
#define CAMERA_DEVICE_API_VERSION_1_0 HARDWARE_DEVICE_API_VERSION(1, 0)
#define CAMERA_DEVICE_API_VERSION_2_0 HARDWARE_DEVICE_API_VERSION(2, 0)
#define CAMERA_DEVICE_API_VERSION_2_1 HARDWARE_DEVICE_API_VERSION(2, 1)
#define CAMERA_DEVICE_API_VERSION_3_0 HARDWARE_DEVICE_API_VERSION(3, 0)

// Device version 2.x is outdated; device version 3.0 is experimental
#define CAMERA_DEVICE_API_VERSION_CURRENT CAMERA_DEVICE_API_VERSION_1_0

/**
 * Defined in /system/media/camera/include/system/camera_metadata.h
 */
typedef struct camera_metadata camera_metadata_t;

typedef struct camera_info {
    /**
     * The direction that the camera faces to. It should be CAMERA_FACING_BACK
     * or CAMERA_FACING_FRONT.
     *
     * Version information:
     *   Valid in all camera_module versions
     */
    int facing;

    /**
     * The orientation of the camera image. The value is the angle that the
     * camera image needs to be rotated clockwise so it shows correctly on the
     * display in its natural orientation. It should be 0, 90, 180, or 270.
     *
     * For example, suppose a device has a naturally tall screen. The
     * back-facing camera sensor is mounted in landscape. You are looking at the
     * screen. If the top side of the camera sensor is aligned with the right
     * edge of the screen in natural orientation, the value should be 90. If the
     * top side of a front-facing camera sensor is aligned with the right of the
     * screen, the value should be 270.
     *
     * Version information:
     *   Valid in all camera_module versions
     */
    int orientation;

    /**
     * The value of camera_device_t.common.version.
     *
     * Version information (based on camera_module_t.common.module_api_version):
     *
     *  CAMERA_MODULE_API_VERSION_1_0:
     *
     *    Not valid. Can be assumed to be CAMERA_DEVICE_API_VERSION_1_0. Do
     *    not read this field.
     *
     *  CAMERA_MODULE_API_VERSION_2_0 or higher:
     *
     *    Always valid
     *
     */
    uint32_t device_version;

    /**
     * The camera's fixed characteristics, which include all camera metadata in
     * the android.*.info.* sections. This should be a sorted metadata buffer,
     * and may not be modified or freed by the caller. The pointer should remain
     * valid for the lifetime of the camera module, and values in it may not
     * change after it is returned by get_camera_info().
     *
     * Version information (based on camera_module_t.common.module_api_version):
     *
     *  CAMERA_MODULE_API_VERSION_1_0:
     *
     *    Not valid. Extra characteristics are not available. Do not read this
     *    field.
     *
     *  CAMERA_MODULE_API_VERSION_2_0 or higher:
     *
     *    Valid if device_version >= CAMERA_DEVICE_API_VERSION_2_0. Do not read
     *    otherwise.
     *
     */
    const camera_metadata_t *static_camera_characteristics;
} camera_info_t;

#ifdef NVCAMERA_HARDWARE
typedef struct camera_info_extended {
    /**
     * The direction that the camera faces to. It should be CAMERA_FACING_BACK
     * or CAMERA_FACING_FRONT.
     *
     * Version information:
     *   Valid in all camera_module versions
     */
    int facing;

    /**
     * The orientation of the camera image. The value is the angle that the
     * camera image needs to be rotated clockwise so it shows correctly on the
     * display in its natural orientation. It should be 0, 90, 180, or 270.
     *
     * For example, suppose a device has a naturally tall screen. The
     * back-facing camera sensor is mounted in landscape. You are looking at the
     * screen. If the top side of the camera sensor is aligned with the right
     * edge of the screen in natural orientation, the value should be 90. If the
     * top side of a front-facing camera sensor is aligned with the right of the
     * screen, the value should be 270.
     *
     * Version information:
     *   Valid in all camera_module versions
     */
    int orientation;

    /**
     * Indicates whether the camera can be used in stereo mode. It should be
     * CAMERA_STEREO_CAPS_UNDEFINED, CAMERA_STEREO_CAPS_MONO or CAMERA_STEREO_CAPS_STEREO.
     */
    int stereoCaps;

    /**
     * It indicates whether camera is connected via USB, or is built-in.
     * CAMERA_CONNECTION_INTERNAL indicates a built-in camera,
     * while CAMERA_CONNECTION_USB indicates a camera connected via USB. It should be
     * CAMERA_CONNECTION_UNDEFINED, CAMERA_CONNECTION_INTERNAL or CAMERA_CONNECTION_USB.
     */
    int connection;
} camera_info_extended_t;
#endif

/**
 * camera_device_status_t:
 *
 * The current status of the camera device, as provided by the HAL through the
 * camera_module_callbacks.camera_device_status_change() call.
 *
 * At module load time, the framework will assume all camera devices are in the
 * CAMERA_DEVICE_STATUS_PRESENT state. The HAL should invoke
 * camera_module_callbacks::camera_device_status_change to inform the framework
 * of any initially NOT_PRESENT devices.
 *
 * Allowed transitions:
 *      PRESENT            -> NOT_PRESENT
 *      NOT_PRESENT        -> ENUMERATING
 *      NOT_PRESENT        -> PRESENT
 *      ENUMERATING        -> PRESENT
 *      ENUMERATING        -> NOT_PRESENT
 */
typedef enum camera_device_status {
    /**
     * The camera device is not currently connected, and opening it will return
     * failure. Calls to get_camera_info must still succeed, and provide the
     * same information it would if the camera were connected
     */
    CAMERA_DEVICE_STATUS_NOT_PRESENT = 0,

    /**
     * The camera device is connected, and opening it will succeed. The
     * information returned by get_camera_info cannot change due to this status
     * change. By default, the framework will assume all devices are in this
     * state.
     */
    CAMERA_DEVICE_STATUS_PRESENT = 1,

    /**
     * The camera device is connected, but it is undergoing an enumeration and
     * so opening the device will return -EBUSY. Calls to get_camera_info
     * must still succeed, as if the camera was in the PRESENT status.
     */
    CAMERA_DEVICE_STATUS_ENUMERATING = 2,

} camera_device_status_t;

/**
 * Callback functions for the camera HAL module to use to inform the framework
 * of changes to the camera subsystem. These are called only by HAL modules
 * implementing version CAMERA_MODULE_API_VERSION_2_1 or higher of the HAL
 * module API interface.
 */
typedef struct camera_module_callbacks {

    /**
     * camera_device_status_change:
     *
     * Callback to the framework to indicate that the state of a specific camera
     * device has changed. At module load time, the framework will assume all
     * camera devices are in the CAMERA_DEVICE_STATUS_PRESENT state. The HAL
     * must call this method to inform the framework of any initially
     * NOT_PRESENT devices.
     *
     * camera_module_callbacks: The instance of camera_module_callbacks_t passed
     *   to the module with set_callbacks.
     *
     * camera_id: The ID of the camera device that has a new status.
     *
     * new_status: The new status code, one of the camera_device_status_t enums,
     *   or a platform-specific status.
     *
     */
    void (*camera_device_status_change)(const struct camera_module_callbacks*,
            int camera_id,
            int new_status);

} camera_module_callbacks_t;

typedef struct camera_module {
    hw_module_t common;

    /**
     * get_number_of_cameras:
     *
     * Returns the number of camera devices accessible through the camera
     * module.  The camera devices are numbered 0 through N-1, where N is the
     * value returned by this call. The name of the camera device for open() is
     * simply the number converted to a string. That is, "0" for camera ID 0,
     * "1" for camera ID 1.
     *
     * The value here must be static, and cannot change after the first call to
     * this method
     */
    int (*get_number_of_cameras)(void);

    /**
     * get_camera_info:
     *
     * Return the static camera information for a given camera device. This
     * information may not change for a camera device.
     *
     */
    int (*get_camera_info)(int camera_id, struct camera_info *info);

    int (*get_camera_info_extended)(int camera_id, struct camera_info_extended *info);

    /**
     * set_callbacks:
     *
     * Provide callback function pointers to the HAL module to inform framework
     * of asynchronous camera module events. The framework will call this
     * function once after initial camera HAL module load, after the
     * get_number_of_cameras() method is called for the first time, and before
     * any other calls to the module.
     *
     * Version information (based on camera_module_t.common.module_api_version):
     *
     *  CAMERA_MODULE_API_VERSION_1_0, CAMERA_MODULE_API_VERSION_2_0:
     *
     *    Not provided by HAL module. Framework may not call this function.
     *
     *  CAMERA_MODULE_API_VERSION_2_1:
     *
     *    Valid to be called by the framework.
     *
     */
    int (*set_callbacks)(const camera_module_callbacks_t *callbacks);

} camera_module_t;

__END_DECLS

#endif /* ANDROID_INCLUDE_CAMERA_COMMON_H */
