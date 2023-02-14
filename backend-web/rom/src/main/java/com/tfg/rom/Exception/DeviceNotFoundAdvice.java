package com.tfg.rom.Exception;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.ResponseStatus;

@ControllerAdvice
public class DeviceNotFoundAdvice {

	  @ResponseBody
	  @ExceptionHandler(DeviceNotFoundException.class)
	  @ResponseStatus(HttpStatus.NOT_FOUND)
	  String employeeNotFoundHandler(DeviceNotFoundException ex) {
	    return ex.getMessage();
	  }

}
