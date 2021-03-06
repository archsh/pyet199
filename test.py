#!/usr/bin/env python
import pyet199
import time
import datetime
import os,sys
for ctx in pyet199.Enumerate():
    ctx.open()
    ctx.ctrl_led(1)
    time.sleep(1)
    ctx.ctrl_led(0)
    time.sleep(1)
    ctx.ctrl_led(1,5)
    print "ET_GET_DEVICE_TYPE: %02x"%ctx.ctrl_get(pyet199.ET_GET_DEVICE_TYPE)
    print "ET_GET_SERIAL_NUMBER: ",''.join(['%02x'%x for x in ctx.ctrl_get(pyet199.ET_GET_SERIAL_NUMBER)])
    print "ET_GET_DEVICE_USABLE_SPACE: %04x"%ctx.ctrl_get(pyet199.ET_GET_DEVICE_USABLE_SPACE)
    print "ET_GET_DEVICE_ATR",','.join(['%02x'%x for x in ctx.ctrl_get(pyet199.ET_GET_DEVICE_ATR)])
    print "ET_GET_CUSTOMER_NAME: %08x"%ctx.ctrl_get(pyet199.ET_GET_CUSTOMER_NAME)
    print "ET_GET_MANUFACTURE_DATE: ",datetime.datetime(*ctx.ctrl_get(pyet199.ET_GET_MANUFACTURE_DATE))
    print "ET_GET_DF_AVAILABLE_SPACE",ctx.ctrl_get(pyet199.ET_GET_DF_AVAILABLE_SPACE)
    
    print "ET_GET_COS_VERSION: %04x"%ctx.ctrl_get(pyet199.ET_GET_COS_VERSION)


    ctx.verify_pin('12345678',pyet199.ET_USER_PIN)
    ctx.change_pin('12345678','87654321',pyet199.ET_USER_PIN,0x10)
    try:
        ctx.verify_pin('12345678',pyet199.ET_USER_PIN)
    except Exception,e:
        print e
    ctx.verify_pin('87654321',pyet199.ET_USER_PIN)
    ctx.change_pin('87654321','12345678',pyet199.ET_USER_PIN,0xff)

    ctx.verify_pin('123456781234567812345678',pyet199.ET_DEV_PIN)

    pubkey,prikey = ctx.gen_rsa_key(1024,65537,0x1001,0x1010)
    print 'Pubkey(%d):\n'%len(pubkey), ['%02x'%ord(x) for x in pubkey],'\n'
    print 'PriKey(%d):\n'%len(prikey), ['%02x'%ord(x) for x in prikey],'\n'
    print "ET_GET_EF_INFO: ",ctx.ctrl_get(pyet199.ET_GET_EF_INFO,0x1001)
    print "ET_GET_EF_INFO: ",ctx.ctrl_get(pyet199.ET_GET_EF_INFO,0x1010)

    ctx.ctrl_reset()
    ctx.verify_pin('123456781234567812345678',pyet199.ET_DEV_PIN)
    ctx.ctrl_set(pyet199.ET_SET_DEVICE_TYPE,pyet199.ET_DEVICE_TYPE_EMPTY)
    ctx.ctrl_set(pyet199.ET_SET_DEVICE_TYPE,pyet199.ET_DEVICE_TYPE_DONGLE)
    ctx.ctrl_set(pyet199.ET_SET_CUSTOMER_NAME,bytearray(b"This is a test for test!"))
    ctx.ctrl_set(pyet199.ET_SET_SHELL_KEY,bytearray(b"ABCDEFGH"))
    ctx.ctrl_set(pyet199.ET_SET_DEVICE_ATR,bytearray(b"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"))

    f = open("RESTKEY.bin","rb")

    ctx.write_file(0x9911,0,f.read(),1024,pyet199.ET_CREATE_NEW|pyet199.ET_UPDATE_FILE,pyet199.FILE_TYPE_EXE)
    ctx.verify_pin('12345678',pyet199.ET_USER_PIN)
    ret = ctx.execute(0x9911,"0123456789ABCDEF")
    
    print "Ret:", ret

    ctx.close()
