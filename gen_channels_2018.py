#!/usr/bin/env python3

import sys

sega = 1

SeGA_channel_format = """
{name} {{
  Address: {address}
  EnergyCoeff: 0 1
}}
"""

def sega_address(crate, slot, channel):
    return '0x{system:02x}{crate:02x}{slot:02x}{channel:02x}'.format(
        system=1,crate=crate, slot=slot, channel=channel)

def sega_name(detnum, segnum):
    return 'SEG{detnum:02d}A{NP}{segnum:02d}'.format(
        detnum=detnum, NP=('N' if segnum!=0 else 'P'), segnum=segnum)

f = open(sys.argv[1],'w')

#central contacts
for channum in range(16):
    address = sega_address(1, 2, channum)
    name = sega_name(channum+1, 0)
    f.write(SeGA_channel_format.format(name=name, address=address))

detnums = {(1,3):1,
           (1,4):1,
           (1,5):2,
           (1,6):2,
           (1,7):3,
           (1,8):3,
           (1,9):4,
           (1,10):4,
           (1,11):5,
           (1,12):5,

           (2,2):6,
           (2,3):6,
           (2,4):7,
           (2,5):7,
           (2,6):8,
           (2,7):8,
           (2,8):9,
           (2,9):9,
           (2,10):10,
           (2,11):10,
           (2,12):11,
           (2,13):11,

           (3,2):12,
           (3,3):12,
           (3,4):13,
           (3,5):13,
           (3,6):14,
           (3,7):14,
           (3,8):15,
           (3,9):15,
           (3,10):16,
           (3,11):16,
           }

for cratenum in range(1,4):
    for slotnum in range(2,14):
        for channum in range(16):
            address = sega_address(cratenum, slotnum, channum)
            try:
                detnum = detnums[(cratenum,slotnum)]
            except KeyError:
                continue

            if cratenum==1:
                # In crate 1, A1 (channum==1) is on slots 3,5,...
                channum = (0 if slotnum%2==1 else 16) + channum + 1
            else:
                # In crate 2 and 3, A1 (channum==1) is in slots 2,4,...
                channum = (0 if slotnum%2==0 else 16) + channum + 1

            name = sega_name(detnum, channum)
            f.write(SeGA_channel_format.format(name=name, address=address))

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
        return chan + 10
    
    # Amp 0 Slot 1
    # Header Sectors 32-17 <=> Physical Sectors 9-1 and 32-26
    elif slot == 3:
        if chan < 7:
            return chan + 26 		
        else:
            return chan - 6

    # Amp 0 Slots 2-3
    # Header Rings 9 - 24
    elif slot == 4:
        if chan < 8:
            return 24 - chan
        else:
            return chan + 1

    # Amp 0 Slot 4
    #  Header Rings 1-8
    elif slot == 5:
        if chan < 8:
            return 8 - chan

    # Amp 1 Slot 0
    # Downstream Header sectors 16-1
    elif slot == 6:
        return 25 - chan

    # Amp 1 Slot 1
    # Downstream Header sectors 32-17
    elif slot == 7:
        if chan < 7:
            return chan + 26
        else:
            return chan - 6

    # Amp 1 Slots 2-3
    # Downstream Rings 9-24
    elif slot == 8:
        if chan < 8:
           return chan+9
	   #v1
           #return  16-chan
        else:
           return chan+9 

    # Amp 1 Slot 4
    # Downstream Rings 1 -8
    elif slot == 9:
        return 8-chan
            
janus_channel_format = """
JAN0{detnum}{frontback}N{segnum:02d} {{
  Address: 0x05{crate:02x}{slot:02x}{chan:02x}
  EnergyCoeff: 0.0 1.0
}}
"""

crate = 4
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
