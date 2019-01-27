/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * v4l2_device.h - V4L2 Device
 */
#ifndef __LIBCAMERA_V4L2_DEVICE_H__
#define __LIBCAMERA_V4L2_DEVICE_H__

#include <string>

#include <linux/videodev2.h>

namespace libcamera {

struct V4L2Capability final : v4l2_capability {
	const char *driver() const
	{
		return reinterpret_cast<const char *>(v4l2_capability::driver);
	}
	const char *card() const
	{
		return reinterpret_cast<const char *>(v4l2_capability::card);
	}
	const char *bus_info() const
	{
		return reinterpret_cast<const char *>(v4l2_capability::bus_info);
	}
	unsigned int device_caps() const
	{
		return capabilities & V4L2_CAP_DEVICE_CAPS
				    ? v4l2_capability::device_caps
				    : v4l2_capability::capabilities;
	}
	bool isMultiplanar() const
	{
		return device_caps() & (V4L2_CAP_VIDEO_CAPTURE_MPLANE |
					V4L2_CAP_VIDEO_OUTPUT_MPLANE);
	}
	bool isCapture() const
	{
		return device_caps() & (V4L2_CAP_VIDEO_CAPTURE |
					V4L2_CAP_VIDEO_CAPTURE_MPLANE);
	}
	bool isOutput() const
	{
		return device_caps() & (V4L2_CAP_VIDEO_OUTPUT |
					V4L2_CAP_VIDEO_OUTPUT_MPLANE);
	}
	bool hasStreaming() const
	{
		return device_caps() & V4L2_CAP_STREAMING;
	}
};

class V4L2DeviceFormat
{
public:
	uint32_t width;
	uint32_t height;
	uint32_t fourcc;

	struct {
		uint32_t size;
		uint32_t bpl;
	} planesFmt[3];
	unsigned int planes;
};

class MediaEntity;
class V4L2Device
{
public:
	explicit V4L2Device(const std::string &deviceNode);
	explicit V4L2Device(const MediaEntity &entity);
	V4L2Device(const V4L2Device &) = delete;
	~V4L2Device();

	V4L2Device &operator=(const V4L2Device &) = delete;

	int open();
	bool isOpen() const;
	void close();

	const char *driverName() const { return caps_.driver(); }
	const char *deviceName() const { return caps_.card(); }
	const char *busName() const { return caps_.bus_info(); }

private:
	std::string deviceNode_;
	int fd_;
	V4L2Capability caps_;
};

} /* namespace libcamera */

#endif /* __LIBCAMERA_V4L2_DEVICE_H__ */
