import time
import sys
import array

# dump raw 8-bit-WAV to arduino-header.  20120901 hpyle

def main():

    from optparse import OptionParser
    parser = OptionParser("%prog inputfile outputfile name")
    options, args = parser.parse_args()
    if len(args) != 3:
        parser.error("must specify input output name");

    print args
    (inputfile, outputfile, name) = args

    fi = open(inputfile, "rb")
    fo = open(outputfile, "w")

    s = fi.read()
    m = len(s)
    n = 0

    print >>fo, "const int %s_len = " % name,
    print >>fo, "%s;" % m
    print >>fo
    print >>fo, "static PROGMEM prog_uchar %s[] = {" % name
    print >>fo, "  ",
    
    for b in s:
        n += 1
        if n < m:
            print >>fo, "0x%02x, " % ((ord(b)+127) & 0xff),
        else:
            print >>fo, "0x%02x };" % ((ord(b)+127) & 0xff)
        if n % 16 == 0:
            print >>fo
            print >>fo, "  ",
            
    print >>fo
    
    fo.close()
    fi.close()

if __name__ == "__main__":
    main()
