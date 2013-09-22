package ch.sharpsoft.arducopter.client.uart;

import org.joda.time.DateTime;

public class GPSData {
	private boolean fix;
	private long time; // /< GPS time (milliseconds from epoch)
	private long date; // /< GPS date (FORMAT TBD)
	private int latitude; // /< latitude in degrees * 10,000,000
	private int longitude; // /< longitude in degrees * 10,000,000
	private int altitude; // /< altitude in cm
	private long ground_speed; // /< ground speed in cm/sec
	private int ground_course; // /< ground course in 100ths of a degree
	private short hdop; // /< horizontal dilution of precision in cm
	private byte num_sats; // /< Number of visible satelites

	private long millis;
	private double lat;
	private double lon;
	private double alt;
	private double gs;
	private double gc;

	public void calc() {
		int year = (int) (2000 + date % 100);
		int month = (int) (date / 100) % 100;
		int day = (int) (date / 10000) % 100;
		int hour = (int) time / 10000000;
		int minutes = (int) (time / 100000) % 100;
		int seconds = (int) (time / 1000) % 100;
		int mil = (int) time % 1000;
		DateTime dt = new DateTime(year, month, day, hour, minutes, seconds);
		millis = dt.getMillis() + mil;
		lat = latitude / 100;
		lon = longitude / 100;
		alt = altitude / 100;
		gs = ground_speed / 100;
		gc = ground_course / 100;
	}

	public final long getMillis() {
		return millis;
	}

	public final double getLat() {
		return lat;
	}

	public final double getLon() {
		return lon;
	}

	public final double getAlt() {
		return alt;
	}

	public final double getGs() {
		return gs;
	}

	public final double getGc() {
		return gc;
	}

	public final boolean isFix() {
		return fix;
	}

	public final void setFix(final boolean fix) {
		this.fix = fix;
	}

	public final long getTime() {
		return time;
	}

	public final void setTime(final long time) {
		this.time = time;
	}

	public final long getDate() {
		return date;
	}

	public final void setDate(final long date) {
		this.date = date;
	}

	public final int getLatitude() {
		return latitude;
	}

	public final void setLatitude(final int latitude) {
		this.latitude = latitude;
	}

	public final int getLongitude() {
		return longitude;
	}

	public final void setLongitude(final int longitude) {
		this.longitude = longitude;
	}

	public final int getAltitude() {
		return altitude;
	}

	public final void setAltitude(final int altitude) {
		this.altitude = altitude;
	}

	public final long getGround_speed() {
		return ground_speed;
	}

	public final void setGround_speed(final long ground_speed) {
		this.ground_speed = ground_speed;
	}

	public final int getGround_course() {
		return ground_course;
	}

	public final void setGround_course(final int ground_course) {
		this.ground_course = ground_course;
	}

	public final short getHdop() {
		return hdop;
	}

	public final void setHdop(final short hdop) {
		this.hdop = hdop;
	}

	public final byte getNum_sats() {
		return num_sats;
	}

	public final void setNum_sats(final byte num_sats) {
		this.num_sats = num_sats;
	}

	@Override
	public String toString() {
		return "GPSData [fix=" + fix + ", time=" + time + ", date=" + date + ", latitude=" + latitude + ", longitude=" + longitude + ", altitude=" + altitude + ", ground_speed=" + ground_speed + ", ground_course=" + ground_course + ", hdop=" + hdop
				+ ", num_sats=" + num_sats + ", millis=" + millis + " (" + (new DateTime(millis)).toString() + "), lat=" + lat + ", lon=" + lon + ", alt=" + alt + ", gs=" + gs + ", gc=" + gc + "]";
	}
}
