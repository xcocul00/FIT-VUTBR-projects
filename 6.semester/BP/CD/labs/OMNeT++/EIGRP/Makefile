#
# OMNeT++/OMNEST Makefile for ansainet
#
# This file was generated with the command:
#  opp_makemake -f --deep --nolink -O out -X. -I../src -L../../../../out/$$\(CONFIGNAME\)/src -DWITH_TCP_COMMON -DWITH_TCP_INET -DWITH_IPv4 -DWITH_IPv6 -DWITH_xMIPv6 -DWITH_GENERIC -DWITH_FLOOD -DWITH_UDP -DWITH_RTP -DWITH_SCTP -DWITH_DHCP -DWITH_ETHERNET -DWITH_PPP -DWITH_MPLS -DWITH_OSPFv2 -DWITH_BGPv4 -DWITH_PIM -DWITH_RIP -DWITH_POWER -DWITH_RADIO -DWITH_AODV -DWITH_IEEE80211 -DWITH_APSKRADIO -DWITH_IDEALWIRELESS -DWITH_TUN -DWITH_PACKETDRILL -DWITH_BMAC -DWITH_LMAC -DWITH_IEEE802154 -DWITH_CSMA -DANSAINET -DWITH_VISUALIZERS -DWITH_IEEE8021D -DWITH_CSMACA
#

# Output directory
PROJECT_OUTPUT_DIR = ../../../../out
PROJECTRELATIVE_PATH = examples/ansa/eigrp/moje_testy
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target

all:   Makefile
	@# Do nothing

.PHONY: all clean cleanall depend msgheaders smheaders

msgheaders:

smheaders:

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f ansainet ansainet.exe libansainet.so libansainet.a libansainet.dll libansainet.dylib

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...

