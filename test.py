#!/usr/bin/env python
import pyet199
import time
import datetime
for ctx in pyet199.Enumerate():
    ctx.open()
    #ctx.ctrl_led(1)
    #time.sleep(1)
    #ctx.ctrl_led(0)
    #time.sleep(1)
    #ctx.ctrl_led(1,5)
    print "ET_GET_DEVICE_TYPE: %02x"%ctx.ctrl_get(pyet199.ET_GET_DEVICE_TYPE)
    print "ET_GET_SERIAL_NUMBER: ",''.join(['%02x'%x for x in ctx.ctrl_get(pyet199.ET_GET_SERIAL_NUMBER)])
    print "ET_GET_DEVICE_USABLE_SPACE: %04x"%ctx.ctrl_get(pyet199.ET_GET_DEVICE_USABLE_SPACE)
    print "ET_GET_DEVICE_ATR",','.join(['%02x'%x for x in ctx.ctrl_get(pyet199.ET_GET_DEVICE_ATR)])
    print "ET_GET_CUSTOMER_NAME: %08x"%ctx.ctrl_get(pyet199.ET_GET_CUSTOMER_NAME)
    print "ET_GET_MANUFACTURE_DATE: ",datetime.datetime(*ctx.ctrl_get(pyet199.ET_GET_MANUFACTURE_DATE))
    print "ET_GET_DF_AVAILABLE_SPACE",ctx.ctrl_get(pyet199.ET_GET_DF_AVAILABLE_SPACE)
    print "ET_GET_EF_INFO: ",ctx.ctrl_get(pyet199.ET_GET_EF_INFO,"9911")
    print "ET_GET_COS_VERSION: %04x"%ctx.ctrl_get(pyet199.ET_GET_COS_VERSION)
    ctx.close()
