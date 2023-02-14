package com.tfg.rom.utils;

import com.tfg.rom.entity.Device;

public class utils {
	
	public static Device getNewDevice(String payload) {
		String dividedString [] =payload.split("-");
		
		Device auxDevice = new Device();
		String deviceName =dividedString[0];
		auxDevice.setName(deviceName);
		auxDevice.setroute(deviceName+"/status");
		auxDevice.setShutterMoveRoute(deviceName+"/move");
		auxDevice.setType(deviceType( Integer.parseInt(dividedString[1])));
		return auxDevice;
	}
	
	public static String deviceType(int deviceType) {
		
		switch (deviceType) {
			case Constants.ONOFF_DEVICES:
				return "lightBulb";
			case Constants.SHUTTER_DEVICE:
				return "shutter";
			case Constants.BUTTON_ONOFF_DEVICE:
				return "button";
			case Constants.BUTTONS_UPDOWN_DEVICE:
				return "Shutter Button";
			case Constants.PHOTORESISTANCE_SENSOR:
				return "Light sensor";
		
		}
		return "-1";

	}
}
