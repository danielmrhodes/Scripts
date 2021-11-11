#!/usr/bin/env python3

import sys

sega = 1

SeGA_channel_format = """
{name} {{
  Address: {address}
  EnergyCoeff: {offset} {slope}
}}
"""

def sega_address(crate, slot, channel):
    return '0x{system:02x}{crate:02x}{slot:02x}{channel:02x}'.format(
        system=1,crate=crate, slot=slot, channel=channel)

def sega_name(detnum, segnum):
    return 'SEG{detnum:02d}A{NP}{segnum:02d}'.format(
        detnum=detnum, NP=('N' if segnum!=0 else 'P'), segnum=segnum)

f = open(sys.argv[1],'w')
#coefs = open(sys.argv[2],'r')
#lines = coefs.readlines()
#line=0

#central contacts
for chan in range(16):
    address = sega_address(0,2,chan)
    name = sega_name(chan+1,0)
    #offset=coefs.readline(2*chan + 1)
    #slope=coefs.readline(2*chan + 2)
    #offset=coefs.read(7)
    #offset=lines[line]
    #line+=8
    #slope=coefs.read(7)
    #slope=lines[line]
    #line+=8
    slope=1
    offset=0
    f.write(SeGA_channel_format.format(name=name, address=address, offset=offset, slope=slope))

detnums = {(0,3):1,
           (0,4):1,
           (0,5):2,
           (0,6):2,
           (0,7):3,
           (0,8):3,
           (0,9):4,
           (0,10):4,
           (0,11):5,
           (0,12):5,
           (0,13):6,
           (0,14):6,

           (1,2):7,
           (1,3):7,
           (1,4):8,
           (1,5):8,
           (1,6):9,
           (1,7):9,
           (1,8):10,
           (1,9):10,
           (1,10):11,
           (1,11):11,
           (1,12):12,
           (1,13):12,

           (2,2):13,
           (2,3):13,
           (2,4):14,
           (2,5):14,
           (2,6):15,
           (2,7):15,
           (2,8):16,
           (2,9):16,
           }
first_slot = 0
last_slot = 0
for crate in range(0,3):
    if crate==0:
        first_slot = 3
        last_slot = 14
    elif crate==1:
        first_slot = 2
        last_slot = 13
    elif crate==2:
        first_slot = 2
        last_slot = 9
    for slot in range(first_slot,last_slot+1):
        for chan in range(16):
            address = sega_address(crate, slot, chan)
            try:
                detnum = detnums[(crate,slot)]
            except KeyError:
                continue

            if crate==0:
                # In crate 1, A1 (channum==1) is on slots 3,5,...
                channum = (0 if slot%2==1 else 16) + chan + 1
            else:
                # In crate 2 and 3, A1 (channum==1) is in slots 2,4,...
                channum = (0 if slot%2==0 else 16) + chan + 1

            name = sega_name(detnum, channum)
            #offset=coefs.readline(2*channum + 1)
            #slope=coefs.readline(2*channum + 2)
            #offset=coefs.read(7)
            #offset=lines[line]
            #line+=8
            #slope=coefs.read(7)
            #slope=lines[line]
            #line+=8
            slope=1
            offset=0
            f.write(SeGA_channel_format.format(name=name, address=address, offset=offset, slope=slope))

def janus_detnum(crate, slot, chan):
    if slot<= 5:
        return 0
    else:
        return 1

def janus_frontback(crate, slot, chan):
    if slot in [2,3,6,7]:
        return 'B'
    else:
        return 'F'

def janus_segnum(crate, slot, chan):
    # Amp 0 Slot 0
    # Header Sectors 16-1 <=> Physical Sectors 25-10
    if slot == 2:
        return 25 - chan
    
    # Amp 0 Slot 1
    # Header Sectors 32-17 <=> Physical Sectors 9-1 and 32-26
    elif slot == 3:
        if chan < 9:
            return 9 - chan		
        else:
            return 41 - chan

    # Amp 0 Slots 2-3
    # Header Rings 9 - 24
    elif slot == 4:
        if chan < 8:
            return chan + 17
        else:
            return chan + 1

    # Amp 0 Slot 4
    #  Header Rings 1-8
    elif slot == 5:
        if chan < 8:
            return chan + 1

    # Amp 1 Slot 0
    # Downstream Header sectors 16-1
    elif slot == 6:
        return 25 - chan #v1
        #return chan + 10 #v3
    # Amp 1 Slot 1
    # Downstream Header sectors 32-17
    elif slot == 7:
        if chan < 9:
            return 9 - chan
        else:
            return 41 - chan #v1
        #if chan > 6:
            #return chan - 6
        #else:
            #return chan + 26 #v3

    # Amp 1 Slots 2-3
    # Header Rings 9-24
    elif slot == 8:
        if chan < 8:
           return chan + 17 #v1
           #return 24 - chan #v2
        else:
           return chan + 1 #v1
           #return 24 - chan #v2

    # Amp 1 Slot 4
    # Downstream Rings 1 -8
    elif slot == 9:
        return chan + 1 #v1
        #return 8 - chan #v2
            
janus_channel_format = """
JAN0{detnum}{frontback}N{segnum:02d} {{
  Address: 0x05{crate:02x}{slot:02x}{chan:02x}
  EnergyCoeff: -21.366 0.021675
}}
"""

crate = 3
for slot in range(2,2+8):
    for chan in range(16):
        if (slot == 5 and chan > 7) or (slot == 9 and chan > 7):
            continue
        detnum = janus_detnum(crate, slot, chan)
        frontback = janus_frontback(crate, slot, chan)
        segnum = janus_segnum(crate, slot, chan)

        f.write(janus_channel_format.format(detnum=detnum, frontback=frontback, segnum=segnum,
                              crate=crate, slot=slot, chan=chan))

f.close()
