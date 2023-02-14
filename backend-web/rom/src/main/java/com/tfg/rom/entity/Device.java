package com.tfg.rom.entity;

import java.util.Objects;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.OneToOne;
import javax.persistence.SequenceGenerator;
import javax.persistence.UniqueConstraint;


@Entity
public class Device {

	@Id
	@SequenceGenerator(
			name= "device_sequence",
			sequenceName = "device_sequence",
			allocationSize = 1)
	@GeneratedValue(strategy = GenerationType.SEQUENCE,generator = "device_sequence")
	private Integer id;
	
	@Column(unique = true)
	private String route;
	@Column(columnDefinition = "varchar(255) default 'off'")
	private String status;
	@Column
	private String type;
	@Column
	private String name;
	@Column
	private String shutterMoveRoute;
	
	@OneToOne(cascade = CascadeType.REMOVE)
	@JoinColumn(name = "controlledDevice", referencedColumnName = "id")
	private Device controlledDevice;
	
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public String getroute() {
		return route;
	}
	public void setroute(String route) {
		this.route = route;
	}
	public String getStatus() {
		return status;
	}
	public void setStatus(String status) {
		this.status = status;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getShutterMoveRoute() {
		return shutterMoveRoute;
	}
	public void setShutterMoveRoute(String shutterMoveRoute) {
		this.shutterMoveRoute = shutterMoveRoute;
	}
	public Device getControlledDevice() {
		return controlledDevice;
	}
	public void setControlledDevice(Device controlledDevice) {
		this.controlledDevice = controlledDevice;
	}
	@Override
	public int hashCode() {
		return Objects.hash(controlledDevice, id, name, route, shutterMoveRoute, status, type);
	}
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Device other = (Device) obj;
		return Objects.equals(controlledDevice, other.controlledDevice) && Objects.equals(id, other.id)
				&& Objects.equals(name, other.name) && Objects.equals(route, other.route)
				&& Objects.equals(shutterMoveRoute, other.shutterMoveRoute) && Objects.equals(status, other.status)
				&& Objects.equals(type, other.type);
	}
	@Override
	public String toString() {
		return "Device [id=" + id + ", route=" + route + ", status=" + status + ", type=" + type + ", name=" + name
				+ ", shutterMoveRoute=" + shutterMoveRoute + ", controlledDevice=" + controlledDevice + "]";
	}
	
	
	
	
	

}
