#!/bin/python

import sys
import argparse
import sqlite3
import time

# read config
# set up loop
# publish data

def find_usb():
	usb = '/dev/ttyACM0'
	usb_conn = open( usb, 'rwt' )

	return usb_conn

def open_db():
	conn = sqlite3.connect( 'hydropony.db' )
	c = conn.cursor()

	# check if need table setup
	res = c.execute( "select count(*) from sqlite_master where type='table' and name=?", 'measurements' )
	if res.fetchone()[ 0 ] == 0:
		c.execute( 'create table measurements ( date text, measure text, value real )' )

	return conn

def main():
	
	# find usb
	usb_adc = find_usb()
	
	# open database
	conn = open_db()
	c = conn.cursor()

	# main loop
	while True:
		# read data from adc
		temp, humidity, ph = read_measurements( usb_adc )
		
		t = time.time()

		# save to database
		c.execute( "insert into measurments value ( ?, ?, ?, ?)", (t, 't', temp ) )
                c.execute( "insert into measurments value ( ?, ?, ?, ?)", (t, 'h', temp ) )
                c.execute( "insert into measurments value ( ?, ?, ?, ?)", (t, 'ph', temp ) )

		# sleep
		time.sleep( 600 )





if __name__ == "__main__":
	main()

