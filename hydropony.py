#!/bin/python

import sys
import argparse
import sqlite3
import time
import logging

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger( __name__ )


def find_usb():
    usb = '/dev/ttyACM0'
    logger.info( 'using usb: ' + usb )

    usb_conn = open( usb, 'rwt' )

    return usb_conn

def open_db():
    conn = sqlite3.connect( 'hydropony.db' )
    c = conn.cursor()

    # check if need table setup
    res = c.execute( "select count(*) from sqlite_master where type='table' and name=?", 'measurements' )
    if res.fetchone()[ 0 ] == 0:
        logger.info( 'creating tables' )
        c.execute( 'create table measurements ( date text, measure text, value real )' )

    return conn

def read_measurements( usb_adc ):

    # start measurement
    usb_adc.write( 'm' )

    line = usb_adc.readline()
    logger.debug( 'adc: ' + line )

    d = line.split( ',' )
    return float( d[0] ), float( d[1] ), float( d[2] ), float( d[3] ), float( d[4] ) 

def main():

    # find usb
    usb_adc = find_usb()
	
    # open database
    conn = open_db()
    c = conn.cursor()

    # main loop
    while True:
        # read data from adc
        temp, humidity, ph, volt, cputemp  = read_measurements( usb_adc )
        t = time.time()

    	# save to database
    	c.execute( "insert into measurments value ( ?, ?, ?, ?)", (t, 't', temp ) )
        c.execute( "insert into measurments value ( ?, ?, ?, ?)", (t, 'h', humidity ) )
        c.execute( "insert into measurments value ( ?, ?, ?, ?)", (t, 'ph', ph ) )
        c.execute( "insert into measurments value ( ?, ?, ?, ?)", (t, 'ph', volt ) )
        c.execute( "insert into measurments value ( ?, ?, ?, ?)", (t, 'ph', cputemp ) )
    
    # sleep
    time.sleep( 600 )


if __name__ == "__main__":
    main()

