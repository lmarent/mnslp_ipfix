/// ----------------------------------------*- mode: C++; -*--
/// @file ipfix_fields.cpp
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: ipfix_fields.cpp 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/ipfix_fields.h $
// ===========================================================
//                      
// Copyright (C) 2012-2014, all rights reserved by
// - System and Computing Engineering, Universidad de los Andes
//
// More information and contact:
// https://www.uniandes.edu.co/
//                      
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// ===========================================================

#include "mnslp_ipfix_fields.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>



namespace mnslp_ipfix
{

#ifndef NTOHLL

uint8_t g_isLittleEndian = 0;
void testEndianness() {
	uint32_t tmp = 0x0a0b0c0d;
	g_isLittleEndian = (tmp != ntohl(tmp));
}
	
#define HTONLL(val)  (g_isLittleEndian ? ((uint64_t)(htonl((uint32_t)((val)>>32))) | \
                          (((uint64_t)htonl((uint32_t)((val)&0xFFFFFFFF)))<<32)) : (val))
#define NTOHLL(val)  (g_isLittleEndian ? ((uint64_t)(ntohl((uint32_t)((val)>>32))) | \
                          (((uint64_t)ntohl((uint32_t)((val)&0xFFFFFFFF)))<<32)) : (val))
#endif

mnslp_ipfix_field::mnslp_ipfix_field(ipfix_field_type_t param): field_type(param)
{
	    if ( param.coding == IPFIX_CODING_INT ) {
            encode = ipfix_encode_int;
            decode = ipfix_decode_int;
            snprint= ipfix_snprint_int;
        }
        else if ( param.coding == IPFIX_CODING_UINT ) {
            encode = ipfix_encode_int;
            decode = ipfix_decode_int;
            snprint= ipfix_snprint_uint;
        }
        else if ( param.coding == IPFIX_CODING_NTP ) {
            encode = ipfix_encode_int;
            decode = ipfix_decode_int;
            snprint= ipfix_snprint_uint;
        }
        else if ( param.coding == IPFIX_CODING_FLOAT ) {
            encode = ipfix_encode_float;
            decode = ipfix_decode_float;
            snprint= ipfix_snprint_float;
        }
        else if ( param.coding == IPFIX_CODING_IPADDR ) {
            encode = ipfix_encode_bytes;
            decode = ipfix_decode_bytes;
            snprint= ipfix_snprint_ipaddr;
        }
        else if ( param.coding == IPFIX_CODING_STRING ) {
            encode = ipfix_encode_bytes;
            decode = ipfix_decode_bytes;
            snprint= ipfix_snprint_string;
        }
        else {
            encode = ipfix_encode_bytes;
            decode = ipfix_decode_bytes;
            snprint= ipfix_snprint_bytes;
        }

}

mnslp_ipfix_field::~mnslp_ipfix_field()
{
	
}

mnslp_ipfix_field& mnslp_ipfix_field::operator= (const mnslp_ipfix_field &param)
{
	field_type = param.field_type;
	encode = param.encode;
	decode = param.decode;
	snprint = param.snprint;
	return *this;
}


msnlp_ipfix_field_container::msnlp_ipfix_field_container()
{

}

msnlp_ipfix_field_container::~msnlp_ipfix_field_container()
{

}
	

void msnlp_ipfix_field_container::AddFieldType(int _eno, int _ftype, ssize_t _length, 
											  int _coding, const std::string _name, 
										  	  const std::string _documentation)
{
ipfix_field_type_t newType;
newType.eno = _eno;
newType.ftype = _ftype;
newType.length = _length;
newType.coding = _coding;
newType.name = _name;
newType.documentation = _documentation;

mnslp_ipfix_field a = mnslp_ipfix_field(newType);
fieldTypeList.push_back(a);

};


void msnlp_ipfix_field_container::initialize_forward(void)
{

	AddFieldType(0, IPFIX_FT_OCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
				 "octetDeltaCount", "");

    AddFieldType(0, IPFIX_FT_PACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "packetDeltaCount", "" );
    AddFieldType(0, IPFIX_FT_FLOWS, 8, IPFIX_CODING_UINT, 
     "flows", "Netflow Number of Flows that were aggregated" );
    AddFieldType(0, IPFIX_FT_PROTOCOLIDENTIFIER, 1, IPFIX_CODING_UINT, 
     "protocolIdentifier", "" );
    AddFieldType(0, IPFIX_FT_IPCLASSOFSERVICE, 1, IPFIX_CODING_UINT, 
     "ipClassOfService", "" );
    AddFieldType(0, IPFIX_FT_TCPCONTROLBITS, 1, IPFIX_CODING_UINT, 
     "tcpControlBits", "" );
    AddFieldType(0, IPFIX_FT_SOURCETRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "sourceTransportPort", "");
    AddFieldType(0, IPFIX_FT_SOURCEIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "sourceIPv4Address", "" );
    AddFieldType(0, IPFIX_FT_SOURCEIPV4PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "sourceIPv4PrefixLength", "");
    AddFieldType(0, IPFIX_FT_INGRESSINTERFACE, 4, IPFIX_CODING_UINT, 
     "ingressInterface", "" );
    AddFieldType(0, IPFIX_FT_DESTINATIONTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "destinationTransportPort", ""); 
    AddFieldType(0, IPFIX_FT_DESTINATIONIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "destinationIPv4Address", "");
    AddFieldType(0, IPFIX_FT_DESTINATIONIPV4PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "destinationIPv4PrefixLength", ""); 
    AddFieldType(0, IPFIX_FT_EGRESSINTERFACE, 4, IPFIX_CODING_UINT, 
     "egressInterface", "");
    AddFieldType(0, IPFIX_FT_IPNEXTHOPIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "ipNextHopIPv4Address", "" );
    AddFieldType(0, IPFIX_FT_BGPSOURCEASNUMBER, 4, IPFIX_CODING_UINT, 
     "bgpSourceAsNumber", "" );
    AddFieldType(0, IPFIX_FT_BGPDESTINATIONASNUMBER, 4, IPFIX_CODING_UINT, 
     "bgpDestinationAsNumber", ""); 
    AddFieldType(0, IPFIX_FT_BGPNEXTHOPIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "bgpNextHopIPv4Address", "");
    AddFieldType(0, IPFIX_FT_POSTMCASTPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "postMCastPacketDeltaCount", "" );
    AddFieldType(0, IPFIX_FT_POSTMCASTOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "postMCastOctetDeltaCount", "");
    AddFieldType(0, IPFIX_FT_FLOWENDSYSUPTIME, 4, IPFIX_CODING_UINT, 
     "flowEndSysUpTime", "");
    AddFieldType(0, IPFIX_FT_FLOWSTARTSYSUPTIME, 4, IPFIX_CODING_UINT, 
     "flowStartSysUpTime", "");
    AddFieldType(0, IPFIX_FT_POSTOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "postOctetDeltaCount", "");
    AddFieldType(0, IPFIX_FT_POSTPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "postPacketDeltaCount", ""); 
    AddFieldType(0, IPFIX_FT_MINIMUMIPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "minimumIpTotalLength", "");
    AddFieldType(0, IPFIX_FT_MAXIMUMIPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "maximumIpTotalLength", "");
    AddFieldType(0, IPFIX_FT_SOURCEIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "sourceIPv6Address", "");
    AddFieldType(0, IPFIX_FT_DESTINATIONIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "destinationIPv6Address", ""); 
    AddFieldType(0, IPFIX_FT_SOURCEIPV6PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "sourceIPv6PrefixLength", "");
    AddFieldType(0, IPFIX_FT_DESTINATIONIPV6PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "destinationIPv6PrefixLength", "");
    AddFieldType(0, IPFIX_FT_FLOWLABELIPV6, 4, IPFIX_CODING_UINT, 
     "flowLabelIPv6", "");
    AddFieldType(0, IPFIX_FT_ICMPTYPECODEIPV4, 2, IPFIX_CODING_UINT, 
     "icmpTypeCodeIPv4", ""); 
    AddFieldType(0, IPFIX_FT_IGMPTYPE, 1, IPFIX_CODING_UINT, 
     "igmpType", "");
    AddFieldType(0, IPFIX_FT_SAMPLING_INTERVAL, 4, IPFIX_CODING_UINT, 
     "sampling_interval", "Netflow Sampling Interval"); 
    AddFieldType(0, IPFIX_FT_SAMPLING_ALGORITHM, 1, IPFIX_CODING_UINT, 
     "sampling_algorithm", "Netflow Sampling Algorithm");
    AddFieldType(0, IPFIX_FT_FLOWACTIVETIMEOUT, 2, IPFIX_CODING_UINT, 
     "flowActiveTimeout", "");
    AddFieldType(0, IPFIX_FT_FLOWIDLETIMEOUT, 2, IPFIX_CODING_UINT, 
     "flowIdleTimeout", "" );
    AddFieldType(0, IPFIX_FT_ENGINE_TYPE, 1, IPFIX_CODING_UINT, 
     "engine_type", "Netflow Engine Type");
    AddFieldType(0, IPFIX_FT_ENGINE_ID, 1, IPFIX_CODING_UINT, 
     "engine_id", "Netflow Engine ID" );
    AddFieldType(0, IPFIX_FT_EXPORTEDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "exportedOctetTotalCount", "" );
    AddFieldType(0, IPFIX_FT_EXPORTEDMESSAGETOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "exportedMessageTotalCount", "");
    AddFieldType(0, IPFIX_FT_EXPORTEDFLOWRECORDTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "exportedFlowRecordTotalCount", "" );
    AddFieldType(0, IPFIX_FT_SOURCEIPV4PREFIX, 4, IPFIX_CODING_IPADDR, 
     "sourceIPv4Prefix", "" );
    AddFieldType(0, IPFIX_FT_DESTINATIONIPV4PREFIX, 4, IPFIX_CODING_IPADDR, 
     "destinationIPv4Prefix", "" );
    AddFieldType(0, IPFIX_FT_MPLSTOPLABELTYPE, 1, IPFIX_CODING_UINT, 
     "mplsTopLabelType", "" ); 
    AddFieldType(0, IPFIX_FT_MPLSTOPLABELIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "mplsTopLabelIPv4Address", "");
    AddFieldType(0, IPFIX_FT_FLOW_SAMPLER_ID, 1, IPFIX_CODING_UINT, 
     "flow_sampler_id", "Netflow Flow Sampler ID" );
    AddFieldType(0, IPFIX_FT_FLOW_SAMPLER_MODE, 1, IPFIX_CODING_UINT, 
     "flow_sampler_mode", "Netflow Flow Sampler Mode");
    AddFieldType(0, IPFIX_FT_FLOW_SAMPLER_RANDOM_INTERVAL, 4, IPFIX_CODING_UINT, 
     "flow_sampler_random_interval", "Netflow Packet Sample Interval" );
    AddFieldType(0, IPFIX_FT_MINIMUMTTL, 1, IPFIX_CODING_UINT, 
     "minimumTTL", "" );
    AddFieldType(0, IPFIX_FT_MAXIMUMTTL, 1, IPFIX_CODING_UINT, 
     "maximumTTL", "" );
    AddFieldType(0, IPFIX_FT_FRAGMENTIDENTIFICATION, 4, IPFIX_CODING_UINT, 
     "fragmentIdentification", "" );
    AddFieldType(0, IPFIX_FT_POSTIPCLASSOFSERVICE, 1, IPFIX_CODING_UINT, 
     "postIpClassOfService", "" );
    AddFieldType(0, IPFIX_FT_SOURCEMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "sourceMacAddress", "" );
    AddFieldType(0, IPFIX_FT_POSTDESTINATIONMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "postDestinationMacAddress", "" );
    AddFieldType(0, IPFIX_FT_VLANID, 2, IPFIX_CODING_UINT, 
     "vlanId", "" );
    AddFieldType(0, IPFIX_FT_POSTVLANID, 2, IPFIX_CODING_UINT, 
     "postVlanId", "" );
    AddFieldType(0, IPFIX_FT_IPVERSION, 1, IPFIX_CODING_UINT, 
     "ipVersion", "" );
    AddFieldType(0, IPFIX_FT_FLOWDIRECTION, 1, IPFIX_CODING_UINT, 
     "flowDirection", "" );
    AddFieldType(0, IPFIX_FT_IPNEXTHOPIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "ipNextHopIPv6Address", "" );
    AddFieldType(0, IPFIX_FT_BGPNEXTHOPIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "bgpNextHopIPv6Address", "" );
    AddFieldType(0, IPFIX_FT_IPV6EXTENSIONHEADERS, 4, IPFIX_CODING_UINT, 
     "ipv6ExtensionHeaders", "" );
    AddFieldType(0, IPFIX_FT_MPLSTOPLABELSTACKSECTION, 65535, IPFIX_CODING_BYTES, 
     "mplsTopLabelStackSection", "" );
    AddFieldType(0, IPFIX_FT_MPLSLABELSTACKSECTION2, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection2", "");
    AddFieldType(0, IPFIX_FT_MPLSLABELSTACKSECTION3, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection3", "" );
    AddFieldType(0, IPFIX_FT_MPLSLABELSTACKSECTION4, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection4", "" );
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKSECTION5, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection5", "" );
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKSECTION6, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection6", "" );
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKSECTION7, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection7", "" );
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKSECTION8, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection8", "" );
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKSECTION9, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection9", "" ); 
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKSECTION10, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection10", "" );
   AddFieldType( 0, IPFIX_FT_DESTINATIONMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "destinationMacAddress", "" );
   AddFieldType( 0, IPFIX_FT_POSTSOURCEMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "postSourceMacAddress", "" );
   AddFieldType( 0, IPFIX_FT_OCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "octetTotalCount", "" ); 
   AddFieldType( 0, IPFIX_FT_PACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "packetTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_FRAGMENTOFFSET, 2, IPFIX_CODING_UINT, 
     "fragmentOffset", "" );
   AddFieldType( 0, IPFIX_FT_MPLSVPNROUTEDISTINGUISHER, 65535, IPFIX_CODING_BYTES, 
     "mplsVpnRouteDistinguisher", "" );
   AddFieldType( 0, IPFIX_FT_BGPNEXTADJACENTASNUMBER, 4, IPFIX_CODING_UINT, 
     "bgpNextAdjacentAsNumber", "" );
   AddFieldType( 0, IPFIX_FT_BGPPREVADJACENTASNUMBER, 4, IPFIX_CODING_UINT, 
     "bgpPrevAdjacentAsNumber", "" );
   AddFieldType( 0, IPFIX_FT_EXPORTERIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "exporterIPv4Address", "" );
   AddFieldType( 0, IPFIX_FT_EXPORTERIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "exporterIPv6Address", "" );
   AddFieldType( 0, IPFIX_FT_DROPPEDOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "droppedOctetDeltaCount", "" ); 
   AddFieldType( 0, IPFIX_FT_DROPPEDPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "droppedPacketDeltaCount", "" );
   AddFieldType( 0, IPFIX_FT_DROPPEDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "droppedOctetTotalCount", "" ); 
   AddFieldType( 0, IPFIX_FT_DROPPEDPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "droppedPacketTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_FLOWENDREASON, 1, IPFIX_CODING_UINT, 
     "flowEndReason", "" );
   AddFieldType( 0, IPFIX_FT_COMMONPROPERTIESID, 8, IPFIX_CODING_UINT, 
     "commonPropertiesId", "" );
   AddFieldType( 0, IPFIX_FT_OBSERVATIONPOINTID, 4, IPFIX_CODING_UINT, 
     "observationPointId", "" );
   AddFieldType( 0, IPFIX_FT_ICMPTYPECODEIPV6, 2, IPFIX_CODING_UINT, 
     "icmpTypeCodeIPv6", "" );
   AddFieldType( 0, IPFIX_FT_MPLSTOPLABELIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "mplsTopLabelIPv6Address", "" );
   AddFieldType( 0, IPFIX_FT_LINECARDID, 4, IPFIX_CODING_UINT, 
     "lineCardId", "" );
   AddFieldType( 0, IPFIX_FT_PORTID, 4, IPFIX_CODING_UINT, 
     "portId", "" );
   AddFieldType( 0, IPFIX_FT_METERINGPROCESSID, 4, IPFIX_CODING_UINT, 
     "meteringProcessId", "" ); 
   AddFieldType( 0, IPFIX_FT_EXPORTINGPROCESSID, 4, IPFIX_CODING_UINT, 
     "exportingProcessId", "" );
   AddFieldType( 0, IPFIX_FT_TEMPLATEID, 2, IPFIX_CODING_UINT, 
     "templateId", "" );
   AddFieldType( 0, IPFIX_FT_WLANCHANNELID, 1, IPFIX_CODING_UINT, 
     "wlanChannelId", "" );
   AddFieldType( 0, IPFIX_FT_WLANSSID, 65535, IPFIX_CODING_STRING, 
     "wlanSSID", "" );
   AddFieldType( 0, IPFIX_FT_FLOWID, 8, IPFIX_CODING_UINT, 
     "flowId", "" );
   AddFieldType( 0, IPFIX_FT_OBSERVATIONDOMAINID, 4, IPFIX_CODING_UINT, 
     "observationDomainId", "" );
   AddFieldType( 0, IPFIX_FT_FLOWSTARTSECONDS, 4, IPFIX_CODING_UINT, 
     "flowStartSeconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWENDSECONDS, 4, IPFIX_CODING_UINT, 
     "flowEndSeconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWSTARTMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "flowStartMilliseconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWENDMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "flowEndMilliseconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWSTARTMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "flowStartMicroseconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWENDMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "flowEndMicroseconds", "" ); 
   AddFieldType( 0, IPFIX_FT_FLOWSTARTNANOSECONDS, 8, IPFIX_CODING_NTP, 
     "flowStartNanoseconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWENDNANOSECONDS, 8, IPFIX_CODING_NTP, 
     "flowEndNanoseconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWSTARTDELTAMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "flowStartDeltaMicroseconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWENDDELTAMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "flowEndDeltaMicroseconds", "" );
   AddFieldType( 0, IPFIX_FT_SYSTEMINITTIMEMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "systemInitTimeMilliseconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWDURATIONMILLISECONDS, 4, IPFIX_CODING_UINT, 
     "flowDurationMilliseconds", "" );
   AddFieldType( 0, IPFIX_FT_FLOWDURATIONMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "flowDurationMicroseconds", "" );
   AddFieldType( 0, IPFIX_FT_OBSERVEDFLOWTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "observedFlowTotalCount", "" ); 
   AddFieldType( 0, IPFIX_FT_IGNOREDPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "ignoredPacketTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_IGNOREDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "ignoredOctetTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_NOTSENTFLOWTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "notSentFlowTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_NOTSENTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "notSentPacketTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_NOTSENTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "notSentOctetTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_DESTINATIONIPV6PREFIX, 16, IPFIX_CODING_IPADDR, 
     "destinationIPv6Prefix", "" );
   AddFieldType( 0, IPFIX_FT_SOURCEIPV6PREFIX, 16, IPFIX_CODING_IPADDR, 
     "sourceIPv6Prefix", "" );
   AddFieldType( 0, IPFIX_FT_POSTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "postOctetTotalCount", "" ); 
   AddFieldType( 0, IPFIX_FT_POSTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "postPacketTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_FLOWKEYINDICATOR, 8, IPFIX_CODING_UINT, 
     "flowKeyIndicator", "" );
   AddFieldType( 0, IPFIX_FT_POSTMCASTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "postMCastPacketTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_POSTMCASTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "postMCastOctetTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_ICMPTYPEIPV4, 1, IPFIX_CODING_UINT, 
     "icmpTypeIPv4", "" );
   AddFieldType( 0, IPFIX_FT_ICMPCODEIPV4, 1, IPFIX_CODING_UINT, 
     "icmpCodeIPv4", "" );
   AddFieldType( 0, IPFIX_FT_ICMPTYPEIPV6, 1, IPFIX_CODING_UINT, 
     "icmpTypeIPv6", "" );
   AddFieldType( 0, IPFIX_FT_ICMPCODEIPV6, 1, IPFIX_CODING_UINT, 
     "icmpCodeIPv6", "" ); 
   AddFieldType( 0, IPFIX_FT_UDPSOURCEPORT, 2, IPFIX_CODING_UINT, 
     "udpSourcePort", "" );
   AddFieldType( 0, IPFIX_FT_UDPDESTINATIONPORT, 2, IPFIX_CODING_UINT, 
     "udpDestinationPort", "" );
   AddFieldType( 0, IPFIX_FT_TCPSOURCEPORT, 2, IPFIX_CODING_UINT, 
     "tcpSourcePort", "" );
   AddFieldType( 0, IPFIX_FT_TCPDESTINATIONPORT, 2, IPFIX_CODING_UINT, 
     "tcpDestinationPort", "" );
   AddFieldType( 0, IPFIX_FT_TCPSEQUENCENUMBER, 4, IPFIX_CODING_UINT, 
     "tcpSequenceNumber", "" );
   AddFieldType( 0, IPFIX_FT_TCPACKNOWLEDGEMENTNUMBER, 4, IPFIX_CODING_UINT, 
     "tcpAcknowledgementNumber", "" );
   AddFieldType( 0, IPFIX_FT_TCPWINDOWSIZE, 2, IPFIX_CODING_UINT, 
     "tcpWindowSize", "" );
   AddFieldType( 0, IPFIX_FT_TCPURGENTPOINTER, 2, IPFIX_CODING_UINT, 
     "tcpUrgentPointer", "" );
   AddFieldType( 0, IPFIX_FT_TCPHEADERLENGTH, 1, IPFIX_CODING_UINT, 
     "tcpHeaderLength", "" );
   AddFieldType( 0, IPFIX_FT_IPHEADERLENGTH, 1, IPFIX_CODING_UINT, 
     "ipHeaderLength", "" ); 
   AddFieldType( 0, IPFIX_FT_TOTALLENGTHIPV4, 2, IPFIX_CODING_UINT, 
     "totalLengthIPv4", "" );
   AddFieldType( 0, IPFIX_FT_PAYLOADLENGTHIPV6, 2, IPFIX_CODING_UINT, 
     "payloadLengthIPv6", "" );
   AddFieldType( 0, IPFIX_FT_IPTTL, 1, IPFIX_CODING_UINT, 
     "ipTTL", "" );
   AddFieldType( 0, IPFIX_FT_NEXTHEADERIPV6, 1, IPFIX_CODING_UINT, 
     "nextHeaderIPv6", "" );
   AddFieldType( 0, IPFIX_FT_MPLSPAYLOADLENGTH, 4, IPFIX_CODING_UINT, 
     "mplsPayloadLength", "" );
   AddFieldType( 0, IPFIX_FT_IPDIFFSERVCODEPOINT, 1, IPFIX_CODING_UINT, 
     "ipDiffServCodePoint", "" );
   AddFieldType( 0, IPFIX_FT_IPPRECEDENCE, 1, IPFIX_CODING_UINT, 
     "ipPrecedence", "" ); 
   AddFieldType( 0, IPFIX_FT_FRAGMENTFLAGS, 1, IPFIX_CODING_UINT, 
     "fragmentFlags", "" );
   AddFieldType( 0, IPFIX_FT_OCTETDELTASUMOFSQUARES, 8, IPFIX_CODING_UINT, 
     "octetDeltaSumOfSquares", "" );
   AddFieldType( 0, IPFIX_FT_OCTETTOTALSUMOFSQUARES, 8, IPFIX_CODING_UINT, 
     "octetTotalSumOfSquares", "" );
   AddFieldType( 0, IPFIX_FT_MPLSTOPLABELTTL, 1, IPFIX_CODING_UINT, 
     "mplsTopLabelTTL", "" );
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKLENGTH, 4, IPFIX_CODING_UINT, 
     "mplsLabelStackLength", "" );
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKDEPTH, 4, IPFIX_CODING_UINT, 
     "mplsLabelStackDepth", "" );
   AddFieldType( 0, IPFIX_FT_MPLSTOPLABELEXP, 1, IPFIX_CODING_UINT, 
     "mplsTopLabelExp", "" );
   AddFieldType( 0, IPFIX_FT_IPPAYLOADLENGTH, 4, IPFIX_CODING_UINT, 
     "ipPayloadLength", "" ); 
   AddFieldType( 0, IPFIX_FT_UDPMESSAGELENGTH, 2, IPFIX_CODING_UINT, 
     "udpMessageLength", "" );
   AddFieldType( 0, IPFIX_FT_ISMULTICAST, 1, IPFIX_CODING_UINT, 
     "isMulticast", "" );
   AddFieldType( 0, IPFIX_FT_IPV4IHL, 1, IPFIX_CODING_UINT, 
     "ipv4IHL", "" );
   AddFieldType( 0, IPFIX_FT_IPV4OPTIONS, 4, IPFIX_CODING_UINT, 
     "ipv4Options", "" );
   AddFieldType( 0, IPFIX_FT_TCPOPTIONS, 8, IPFIX_CODING_UINT, 
     "tcpOptions", "" );
   AddFieldType( 0, IPFIX_FT_PADDINGOCTETS, 65535, IPFIX_CODING_BYTES, 
     "paddingOctets", "" );
   AddFieldType( 0, IPFIX_FT_COLLECTORIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "collectorIPv4Address", "" ); 
   AddFieldType( 0, IPFIX_FT_COLLECTORIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "collectorIPv6Address", "" );
   AddFieldType( 0, IPFIX_FT_COLLECTORINTERFACE, 4, IPFIX_CODING_UINT, 
     "collectorInterface", "" );
   AddFieldType( 0, IPFIX_FT_COLLECTORPROTOCOLVERSION, 1, IPFIX_CODING_UINT, 
     "collectorProtocolVersion", "" );
   AddFieldType( 0, IPFIX_FT_COLLECTORTRANSPORTPROTOCOL, 1, IPFIX_CODING_UINT, 
     "collectorTransportProtocol", "" );
   AddFieldType( 0, IPFIX_FT_COLLECTORTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "collectorTransportPort", "" );
   AddFieldType( 0, IPFIX_FT_EXPORTERTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "exporterTransportPort", "" );
   AddFieldType( 0, IPFIX_FT_TCPSYNTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "tcpSynTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_TCPFINTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "tcpFinTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_TCPRSTTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "tcpRstTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_TCPPSHTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "tcpPshTotalCount", "" ); 
   AddFieldType( 0, IPFIX_FT_TCPACKTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "tcpAckTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_TCPURGTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "tcpUrgTotalCount", "" );
   AddFieldType( 0, IPFIX_FT_IPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "ipTotalLength", "" );
   AddFieldType( 0, IPFIX_FT_POSTMPLSTOPLABELEXP, 1, IPFIX_CODING_UINT, 
     "postMplsTopLabelExp", "" );
   AddFieldType( 0, IPFIX_FT_TCPWINDOWSCALE, 2, IPFIX_CODING_UINT, 
     "tcpWindowScale", "" );
   AddFieldType( 0, IPFIX_FT_OBSERVATIONPOINTID_PSAMP, 8, IPFIX_CODING_UINT, 
     "observationPointId_PSAMP", "" );
   AddFieldType( 0, IPFIX_FT_SELECTIONSEQUENCEID, 8, IPFIX_CODING_UINT, 
     "selectionSequenceId", "" );
   AddFieldType( 0, IPFIX_FT_SELECTORID, 2, IPFIX_CODING_UINT, 
     "selectorId", "" );
   AddFieldType( 0, IPFIX_FT_INFORMATIONELEMENTID, 2, IPFIX_CODING_UINT, 
     "informationElementId", "" );
   AddFieldType( 0, IPFIX_FT_SELECTORALGORITHM, 2, IPFIX_CODING_UINT, 
     "selectorAlgorithm", "" );
   AddFieldType( 0, IPFIX_FT_SAMPLINGPACKETINTERVAL, 4, IPFIX_CODING_UINT, 
     "samplingPacketInterval", "" );
   AddFieldType( 0, IPFIX_FT_SAMPLINGPACKETSPACE, 4, IPFIX_CODING_UINT, 
     "samplingPacketSpace", "" );
   AddFieldType( 0, IPFIX_FT_SAMPLINGTIMEINTERVAL, 8, IPFIX_CODING_NTP, 
     "samplingTimeInterval", "" );
   AddFieldType( 0, IPFIX_FT_SAMPLINGTIMESPACE, 8, IPFIX_CODING_NTP, 
     "samplingTimeSpace", "" );
   AddFieldType( 0, IPFIX_FT_SAMPLINGSIZE, 4, IPFIX_CODING_UINT, 
     "samplingSize", "" );
   AddFieldType( 0, IPFIX_FT_SAMPLINGPOPULATION, 4, IPFIX_CODING_UINT, 
     "samplingPopulation", "" );
   AddFieldType( 0, IPFIX_FT_SAMPLINGPROBABILITY, 8, IPFIX_CODING_FLOAT, 
     "samplingProbability", "" );
   AddFieldType( 0, IPFIX_FT_DATALINKFRAMESIZE, 4, IPFIX_CODING_UINT, 
     "dataLinkFrameSize", "" );
   AddFieldType( 0, IPFIX_FT_IPHEADERPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "ipHeaderPacketSection", "" );
   AddFieldType( 0, IPFIX_FT_IPPAYLOADPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "ipPayloadPacketSection", "" );
   AddFieldType( 0, IPFIX_FT_DATALINKFRAMESECTION, 65535, IPFIX_CODING_BYTES, 
     "dataLinkFrameSection", "" );
   AddFieldType( 0, IPFIX_FT_MPLSLABELSTACKSECTION, 65535, IPFIX_CODING_BYTES, 
     "mplsLabelStackSection", "" );
   AddFieldType( 0, IPFIX_FT_MPLSPAYLOADPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "mplsPayloadPacketSection", "" );
   AddFieldType( 0, IPFIX_FT_PACKETSOBSERVED, 8, IPFIX_CODING_UINT, 
     "packetsObserved", "" );
   AddFieldType( 0, IPFIX_FT_PACKETSSELECTED, 8, IPFIX_CODING_UINT, 
     "packetsSelected", "" );
   AddFieldType( 0, IPFIX_FT_FIXEDERROR, 8, IPFIX_CODING_FLOAT, 
     "fixedError", "" );
   AddFieldType( 0, IPFIX_FT_RELATIVEERROR, 8, IPFIX_CODING_FLOAT, 
     "relativeError", "" );
   AddFieldType( 0, IPFIX_FT_OBSERVATIONTIMESECONDS, 4, IPFIX_CODING_UINT, 
     "observationTimeSeconds", "" );
   AddFieldType( 0, IPFIX_FT_OBSERVATIONTIMEMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "observationTimeMilliseconds", "" );
   AddFieldType( 0, IPFIX_FT_OBSERVATIONTIMEMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "observationTimeMicroseconds", "" );
   AddFieldType( 0, IPFIX_FT_OBSERVATIONTIMENANOSECONDS, 8, IPFIX_CODING_NTP, 
     "observationTimeNanoseconds", "" ); 
   AddFieldType( 0, IPFIX_FT_DIGESTHASHVALUE, 8, IPFIX_CODING_UINT, 
     "digestHashValue", "" );
   AddFieldType( 0, IPFIX_FT_HASHIPPAYLOADOFFSET, 8, IPFIX_CODING_UINT, 
     "hashIPPayloadOffset", "" );
   AddFieldType( 0, IPFIX_FT_HASHIPPAYLOADSIZE, 8, IPFIX_CODING_UINT, 
     "hashIPPayloadSize", "" );
   AddFieldType( 0, IPFIX_FT_HASHOUTPUTRANGEMIN, 8, IPFIX_CODING_UINT, 
     "hashOutputRangeMin", "" );
   AddFieldType( 0, IPFIX_FT_HASHOUTPUTRANGEMAX, 8, IPFIX_CODING_UINT, 
     "hashOutputRangeMax", "" );
   AddFieldType( 0, IPFIX_FT_HASHSELECTEDRANGEMIN, 8, IPFIX_CODING_UINT, 
     "hashSelectedRangeMin", "" );
   AddFieldType( 0, IPFIX_FT_HASHSELECTEDRANGEMAX, 8, IPFIX_CODING_UINT, 
     "hashSelectedRangeMax", "" );
   AddFieldType( 0, IPFIX_FT_HASHDIGESTOUTPUT, 1, IPFIX_CODING_BYTES, 
     "hashDigestOutput", "" );
   AddFieldType( 0, IPFIX_FT_HASHINITIALISERVALUE, 8, IPFIX_CODING_UINT, 
     "hashInitialiserValue", "" );
     
   AddFieldType( 0, 0, -1, 0, NULL, NULL );

}

void msnlp_ipfix_field_container::initialize_reverse(void)
{

   AddFieldType( REV_PEN, IPFIX_FT_OCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse octetDeltaCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_PACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse packetDeltaCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWS, 8, IPFIX_CODING_UINT, 
     "reverse flows", "reverse Netflow Number of Flows that were aggregated" ); 
   AddFieldType( REV_PEN, IPFIX_FT_PROTOCOLIDENTIFIER, 1, IPFIX_CODING_UINT, 
     "reverse protocolIdentifier", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPCLASSOFSERVICE, 1, IPFIX_CODING_UINT, 
     "reverse ipClassOfService", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPCONTROLBITS, 1, IPFIX_CODING_UINT, 
     "reverse tcpControlBits", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SOURCETRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "reverse sourceTransportPort", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SOURCEIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse sourceIPv4Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SOURCEIPV4PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse sourceIPv4PrefixLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_INGRESSINTERFACE, 4, IPFIX_CODING_UINT, 
     "reverse ingressInterface", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DESTINATIONTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "reverse destinationTransportPort", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DESTINATIONIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse destinationIPv4Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DESTINATIONIPV4PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse destinationIPv4PrefixLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_EGRESSINTERFACE, 4, IPFIX_CODING_UINT, 
     "reverse egressInterface", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPNEXTHOPIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse ipNextHopIPv4Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_BGPSOURCEASNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse bgpSourceAsNumber", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_BGPDESTINATIONASNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse bgpDestinationAsNumber", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_BGPNEXTHOPIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse bgpNextHopIPv4Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTMCASTPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postMCastPacketDeltaCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTMCASTOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postMCastOctetDeltaCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWENDSYSUPTIME, 4, IPFIX_CODING_UINT, 
     "reverse flowEndSysUpTime", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWSTARTSYSUPTIME, 4, IPFIX_CODING_UINT, 
     "reverse flowStartSysUpTime", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postOctetDeltaCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postPacketDeltaCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MINIMUMIPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "reverse minimumIpTotalLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MAXIMUMIPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "reverse maximumIpTotalLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SOURCEIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse sourceIPv6Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DESTINATIONIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse destinationIPv6Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SOURCEIPV6PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse sourceIPv6PrefixLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DESTINATIONIPV6PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse destinationIPv6PrefixLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWLABELIPV6, 4, IPFIX_CODING_UINT, 
     "reverse flowLabelIPv6", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ICMPTYPECODEIPV4, 2, IPFIX_CODING_UINT, 
     "reverse icmpTypeCodeIPv4", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IGMPTYPE, 1, IPFIX_CODING_UINT, 
     "reverse igmpType", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLING_INTERVAL, 4, IPFIX_CODING_UINT, 
     "reverse sampling_interval", "reverse Netflow Sampling Interval" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLING_ALGORITHM, 1, IPFIX_CODING_UINT, 
     "reverse sampling_algorithm", "reverse Netflow Sampling Algorithm" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWACTIVETIMEOUT, 2, IPFIX_CODING_UINT, 
     "reverse flowActiveTimeout", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWIDLETIMEOUT, 2, IPFIX_CODING_UINT, 
     "reverse flowIdleTimeout", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ENGINE_TYPE, 1, IPFIX_CODING_UINT, 
     "reverse engine_type", "reverse Netflow Engine Type" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ENGINE_ID, 1, IPFIX_CODING_UINT, 
     "reverse engine_id", "reverse Netflow Engine ID" ); 
   AddFieldType( REV_PEN, IPFIX_FT_EXPORTEDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse exportedOctetTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_EXPORTEDMESSAGETOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse exportedMessageTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_EXPORTEDFLOWRECORDTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse exportedFlowRecordTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SOURCEIPV4PREFIX, 4, IPFIX_CODING_IPADDR, 
     "reverse sourceIPv4Prefix", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DESTINATIONIPV4PREFIX, 4, IPFIX_CODING_IPADDR, 
     "reverse destinationIPv4Prefix", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSTOPLABELTYPE, 1, IPFIX_CODING_UINT, 
     "reverse mplsTopLabelType", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSTOPLABELIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse mplsTopLabelIPv4Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOW_SAMPLER_ID, 1, IPFIX_CODING_UINT, 
     "reverse flow_sampler_id", "reverse Netflow Flow Sampler ID" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOW_SAMPLER_MODE, 1, IPFIX_CODING_UINT, 
     "reverse flow_sampler_mode", "reverse Netflow Flow Sampler Mode" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOW_SAMPLER_RANDOM_INTERVAL, 4, IPFIX_CODING_UINT, 
     "reverse flow_sampler_random_interval", "reverse Netflow Packet Sample Interval" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MINIMUMTTL, 1, IPFIX_CODING_UINT, 
     "reverse minimumTTL", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MAXIMUMTTL, 1, IPFIX_CODING_UINT, 
     "reverse maximumTTL", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FRAGMENTIDENTIFICATION, 4, IPFIX_CODING_UINT, 
     "reverse fragmentIdentification", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTIPCLASSOFSERVICE, 1, IPFIX_CODING_UINT, 
     "reverse postIpClassOfService", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SOURCEMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "reverse sourceMacAddress", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTDESTINATIONMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "reverse postDestinationMacAddress", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_VLANID, 2, IPFIX_CODING_UINT, 
     "reverse vlanId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTVLANID, 2, IPFIX_CODING_UINT, 
     "reverse postVlanId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPVERSION, 1, IPFIX_CODING_UINT, 
     "reverse ipVersion", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWDIRECTION, 1, IPFIX_CODING_UINT, 
     "reverse flowDirection", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPNEXTHOPIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse ipNextHopIPv6Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_BGPNEXTHOPIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse bgpNextHopIPv6Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPV6EXTENSIONHEADERS, 4, IPFIX_CODING_UINT, 
     "reverse ipv6ExtensionHeaders", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSTOPLABELSTACKSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsTopLabelStackSection", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION2, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection2", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION3, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection3", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION4, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection4", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION5, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection5", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION6, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection6", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION7, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection7", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION8, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection8", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION9, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection9", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION10, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection10", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DESTINATIONMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "reverse destinationMacAddress", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTSOURCEMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "reverse postSourceMacAddress", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse octetTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_PACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse packetTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FRAGMENTOFFSET, 2, IPFIX_CODING_UINT, 
     "reverse fragmentOffset", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSVPNROUTEDISTINGUISHER, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsVpnRouteDistinguisher", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_BGPNEXTADJACENTASNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse bgpNextAdjacentAsNumber", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_BGPPREVADJACENTASNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse bgpPrevAdjacentAsNumber", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_EXPORTERIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse exporterIPv4Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_EXPORTERIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse exporterIPv6Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DROPPEDOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse droppedOctetDeltaCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DROPPEDPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse droppedPacketDeltaCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DROPPEDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse droppedOctetTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DROPPEDPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse droppedPacketTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWENDREASON, 1, IPFIX_CODING_UINT, 
     "reverse flowEndReason", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_COMMONPROPERTIESID, 8, IPFIX_CODING_UINT, 
     "reverse commonPropertiesId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OBSERVATIONPOINTID, 4, IPFIX_CODING_UINT, 
     "reverse observationPointId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ICMPTYPECODEIPV6, 2, IPFIX_CODING_UINT, 
     "reverse icmpTypeCodeIPv6", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSTOPLABELIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse mplsTopLabelIPv6Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_LINECARDID, 4, IPFIX_CODING_UINT, 
     "reverse lineCardId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_PORTID, 4, IPFIX_CODING_UINT, 
     "reverse portId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_METERINGPROCESSID, 4, IPFIX_CODING_UINT, 
     "reverse meteringProcessId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_EXPORTINGPROCESSID, 4, IPFIX_CODING_UINT, 
     "reverse exportingProcessId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TEMPLATEID, 2, IPFIX_CODING_UINT, 
     "reverse templateId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_WLANCHANNELID, 1, IPFIX_CODING_UINT, 
     "reverse wlanChannelId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_WLANSSID, 65535, IPFIX_CODING_STRING, 
     "reverse wlanSSID", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWID, 8, IPFIX_CODING_UINT, 
     "reverse flowId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OBSERVATIONDOMAINID, 4, IPFIX_CODING_UINT, 
     "reverse observationDomainId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWSTARTSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowStartSeconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWENDSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowEndSeconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWSTARTMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "reverse flowStartMilliseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWENDMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "reverse flowEndMilliseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWSTARTMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse flowStartMicroseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWENDMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse flowEndMicroseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWSTARTNANOSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse flowStartNanoseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWENDNANOSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse flowEndNanoseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWSTARTDELTAMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowStartDeltaMicroseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWENDDELTAMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowEndDeltaMicroseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SYSTEMINITTIMEMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "reverse systemInitTimeMilliseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWDURATIONMILLISECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowDurationMilliseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWDURATIONMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowDurationMicroseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OBSERVEDFLOWTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse observedFlowTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IGNOREDPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse ignoredPacketTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IGNOREDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse ignoredOctetTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_NOTSENTFLOWTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse notSentFlowTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_NOTSENTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse notSentPacketTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_NOTSENTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse notSentOctetTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DESTINATIONIPV6PREFIX, 16, IPFIX_CODING_IPADDR, 
     "reverse destinationIPv6Prefix", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SOURCEIPV6PREFIX, 16, IPFIX_CODING_IPADDR, 
     "reverse sourceIPv6Prefix", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postOctetTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postPacketTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FLOWKEYINDICATOR, 8, IPFIX_CODING_UINT, 
     "reverse flowKeyIndicator", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTMCASTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postMCastPacketTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTMCASTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postMCastOctetTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ICMPTYPEIPV4, 1, IPFIX_CODING_UINT, 
     "reverse icmpTypeIPv4", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ICMPCODEIPV4, 1, IPFIX_CODING_UINT, 
     "reverse icmpCodeIPv4", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ICMPTYPEIPV6, 1, IPFIX_CODING_UINT, 
     "reverse icmpTypeIPv6", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ICMPCODEIPV6, 1, IPFIX_CODING_UINT, 
     "reverse icmpCodeIPv6", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_UDPSOURCEPORT, 2, IPFIX_CODING_UINT, 
     "reverse udpSourcePort", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_UDPDESTINATIONPORT, 2, IPFIX_CODING_UINT, 
     "reverse udpDestinationPort", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPSOURCEPORT, 2, IPFIX_CODING_UINT, 
     "reverse tcpSourcePort", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPDESTINATIONPORT, 2, IPFIX_CODING_UINT, 
     "reverse tcpDestinationPort", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPSEQUENCENUMBER, 4, IPFIX_CODING_UINT, 
     "reverse tcpSequenceNumber", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPACKNOWLEDGEMENTNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse tcpAcknowledgementNumber", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPWINDOWSIZE, 2, IPFIX_CODING_UINT, 
     "reverse tcpWindowSize", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPURGENTPOINTER, 2, IPFIX_CODING_UINT, 
     "reverse tcpUrgentPointer", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPHEADERLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse tcpHeaderLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPHEADERLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse ipHeaderLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TOTALLENGTHIPV4, 2, IPFIX_CODING_UINT, 
     "reverse totalLengthIPv4", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_PAYLOADLENGTHIPV6, 2, IPFIX_CODING_UINT, 
     "reverse payloadLengthIPv6", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPTTL, 1, IPFIX_CODING_UINT, 
     "reverse ipTTL", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_NEXTHEADERIPV6, 1, IPFIX_CODING_UINT, 
     "reverse nextHeaderIPv6", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSPAYLOADLENGTH, 4, IPFIX_CODING_UINT, 
     "reverse mplsPayloadLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPDIFFSERVCODEPOINT, 1, IPFIX_CODING_UINT, 
     "reverse ipDiffServCodePoint", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPPRECEDENCE, 1, IPFIX_CODING_UINT, 
     "reverse ipPrecedence", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FRAGMENTFLAGS, 1, IPFIX_CODING_UINT, 
     "reverse fragmentFlags", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OCTETDELTASUMOFSQUARES, 8, IPFIX_CODING_UINT, 
     "reverse octetDeltaSumOfSquares", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OCTETTOTALSUMOFSQUARES, 8, IPFIX_CODING_UINT, 
     "reverse octetTotalSumOfSquares", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSTOPLABELTTL, 1, IPFIX_CODING_UINT, 
     "reverse mplsTopLabelTTL", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKLENGTH, 4, IPFIX_CODING_UINT, 
     "reverse mplsLabelStackLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKDEPTH, 4, IPFIX_CODING_UINT, 
     "reverse mplsLabelStackDepth", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSTOPLABELEXP, 1, IPFIX_CODING_UINT, 
     "reverse mplsTopLabelExp", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPPAYLOADLENGTH, 4, IPFIX_CODING_UINT, 
     "reverse ipPayloadLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_UDPMESSAGELENGTH, 2, IPFIX_CODING_UINT, 
     "reverse udpMessageLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_ISMULTICAST, 1, IPFIX_CODING_UINT, 
     "reverse isMulticast", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPV4IHL, 1, IPFIX_CODING_UINT, 
     "reverse ipv4IHL", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPV4OPTIONS, 4, IPFIX_CODING_UINT, 
     "reverse ipv4Options", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPOPTIONS, 8, IPFIX_CODING_UINT, 
     "reverse tcpOptions", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_PADDINGOCTETS, 65535, IPFIX_CODING_BYTES, 
     "reverse paddingOctets", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_COLLECTORIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse collectorIPv4Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_COLLECTORIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse collectorIPv6Address", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_COLLECTORINTERFACE, 4, IPFIX_CODING_UINT, 
     "reverse collectorInterface", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_COLLECTORPROTOCOLVERSION, 1, IPFIX_CODING_UINT, 
     "reverse collectorProtocolVersion", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_COLLECTORTRANSPORTPROTOCOL, 1, IPFIX_CODING_UINT, 
     "reverse collectorTransportProtocol", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_COLLECTORTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "reverse collectorTransportPort", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_EXPORTERTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "reverse exporterTransportPort", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPSYNTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpSynTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPFINTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpFinTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPRSTTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpRstTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPPSHTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpPshTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPACKTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpAckTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPURGTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpUrgTotalCount", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "reverse ipTotalLength", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_POSTMPLSTOPLABELEXP, 1, IPFIX_CODING_UINT, 
     "reverse postMplsTopLabelExp", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_TCPWINDOWSCALE, 2, IPFIX_CODING_UINT, 
     "reverse tcpWindowScale", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OBSERVATIONPOINTID_PSAMP, 8, IPFIX_CODING_UINT, 
     "reverse observationPointId_PSAMP", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SELECTIONSEQUENCEID, 8, IPFIX_CODING_UINT, 
     "reverse selectionSequenceId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SELECTORID, 2, IPFIX_CODING_UINT, 
     "reverse selectorId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_INFORMATIONELEMENTID, 2, IPFIX_CODING_UINT, 
     "reverse informationElementId", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SELECTORALGORITHM, 2, IPFIX_CODING_UINT, 
     "reverse selectorAlgorithm", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLINGPACKETINTERVAL, 4, IPFIX_CODING_UINT, 
     "reverse samplingPacketInterval", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLINGPACKETSPACE, 4, IPFIX_CODING_UINT, 
     "reverse samplingPacketSpace", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLINGTIMEINTERVAL, 8, IPFIX_CODING_NTP, 
     "reverse samplingTimeInterval", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLINGTIMESPACE, 8, IPFIX_CODING_NTP, 
     "reverse samplingTimeSpace", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLINGSIZE, 4, IPFIX_CODING_UINT, 
     "reverse samplingSize", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLINGPOPULATION, 4, IPFIX_CODING_UINT, 
     "reverse samplingPopulation", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_SAMPLINGPROBABILITY, 8, IPFIX_CODING_FLOAT, 
     "reverse samplingProbability", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DATALINKFRAMESIZE, 4, IPFIX_CODING_UINT, 
     "reverse dataLinkFrameSize", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPHEADERPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse ipHeaderPacketSection", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_IPPAYLOADPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse ipPayloadPacketSection", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DATALINKFRAMESECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse dataLinkFrameSection", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_MPLSPAYLOADPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsPayloadPacketSection", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_PACKETSOBSERVED, 8, IPFIX_CODING_UINT, 
     "reverse packetsObserved", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_PACKETSSELECTED, 8, IPFIX_CODING_UINT, 
     "reverse packetsSelected", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_FIXEDERROR, 8, IPFIX_CODING_FLOAT, 
     "reverse fixedError", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_RELATIVEERROR, 8, IPFIX_CODING_FLOAT, 
     "reverse relativeError", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OBSERVATIONTIMESECONDS, 4, IPFIX_CODING_UINT, 
     "reverse observationTimeSeconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OBSERVATIONTIMEMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "reverse observationTimeMilliseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OBSERVATIONTIMEMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse observationTimeMicroseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_OBSERVATIONTIMENANOSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse observationTimeNanoseconds", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_DIGESTHASHVALUE, 8, IPFIX_CODING_UINT, 
     "reverse digestHashValue", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_HASHIPPAYLOADOFFSET, 8, IPFIX_CODING_UINT, 
     "reverse hashIPPayloadOffset", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_HASHIPPAYLOADSIZE, 8, IPFIX_CODING_UINT, 
     "reverse hashIPPayloadSize", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_HASHOUTPUTRANGEMIN, 8, IPFIX_CODING_UINT, 
     "reverse hashOutputRangeMin", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_HASHOUTPUTRANGEMAX, 8, IPFIX_CODING_UINT, 
     "reverse hashOutputRangeMax", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_HASHSELECTEDRANGEMIN, 8, IPFIX_CODING_UINT, 
     "reverse hashSelectedRangeMin", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_HASHSELECTEDRANGEMAX, 8, IPFIX_CODING_UINT, 
     "reverse hashSelectedRangeMax", "" ); 
   AddFieldType( REV_PEN, IPFIX_FT_HASHDIGESTOUTPUT, 1, IPFIX_CODING_BYTES, 
     "reverse hashDigestOutput", "" );
   AddFieldType( REV_PEN, IPFIX_FT_HASHINITIALISERVALUE, 8, IPFIX_CODING_UINT, 
     "reverse hashInitialiserValue", "" );
   AddFieldType( REV_PEN, 0, -1, 0, NULL, NULL );	

}

void msnlp_ipfix_field_container::AddFieldType(ipfix_field_type_t &param)
{
	mnslp_ipfix_field a = mnslp_ipfix_field(param);
	fieldTypeList.push_back(a);
}


/* name:       get_field()
 * parameters: eno, ftype
 * return:     field from the container list or NULL
 */
mnslp_ipfix_field * msnlp_ipfix_field_container::get_field( int eno, int type )
{
 
	for (std::vector<mnslp_ipfix_field>::iterator it = fieldTypeList.begin() ; 
			it != fieldTypeList.end(); ++it)
	{
        if( ((it->get_field_type()).ftype == type) && ((it->get_field_type()).eno==eno) )
			return &(*it);
	}
    return NULL;
}

/*
 * descr: encode,decode,print funcs
 */
int ipfix_encode_int( void *in, void *out, size_t len )
{
    unsigned char *i = (unsigned char*) in;
    unsigned char *o = (unsigned char*) out;
    uint16_t      tmp16;
    uint32_t      tmp32;
    uint64_t      tmp64;

    switch ( len )
    {
      case 1:
          o[0] = i[0];
          break;
      case 2:
          memcpy( &tmp16, i, len );
          tmp16 = htons( tmp16 );
          memcpy( out, &tmp16, len );
          break;
      case 4:
          memcpy( &tmp32, i, len );
          tmp32 = htonl( tmp32 );
          memcpy( out, &tmp32, len );
          break;
      case 8:
          memcpy( &tmp64, i, len );
          tmp64 = HTONLL( tmp64 );
          memcpy( out, &tmp64, len );
          break;
      default:
          memset( out, 0xff, len );
          return -1;
    }
    return 0;
}

int ipfix_decode_int( void *in, void *out, size_t len )
{
    unsigned char *i = (unsigned char*) in;
    unsigned char *o = (unsigned char*) out;
    uint16_t      tmp16;
    uint32_t      tmp32;
    uint64_t      tmp64;

    switch ( len )
    {
      case 1:
          o[0] = i[0];
          break;
      case 2:
          memcpy( &tmp16, i, len );
          tmp16 = ntohs( tmp16 );
          memcpy( out, &tmp16, len );
          break;
      case 4:
          memcpy( &tmp32, i, len );
          tmp32 = ntohl( tmp32 );
          memcpy( out, &tmp32, len );
          break;
      case 8:
          memcpy( &tmp64, i, len );
          tmp64 = NTOHLL( tmp64 );
          memcpy( out, &tmp64, len );
          break;
      default:
          memset( out, 0xff, len );
          return -1;
    }
    return 0;
}

int ipfix_snprint_int( char *str, size_t size, void *data, size_t len )
{
    int8_t       tmp8;
    int16_t      tmp16;
    int32_t      tmp32;
    int64_t      tmp64;

    switch ( len ) {
      case 1:
          memcpy( &tmp8, data, len );
          return snprintf( str, size, "%d", tmp8 );
      case 2:
          memcpy( &tmp16, data, len );
          //tmp16 = ntohs( tmp16 );
          return snprintf( str, size, "%d", tmp16 );
      case 4:
          memcpy( &tmp32, data, len );
          //tmp32 = ntohl( tmp32 );
          return snprintf( str, size, "%d", (int)tmp32 );
      case 8:
          memcpy( &tmp64, data, len );
          //tmp64 = NTOHLL( tmp64 );
          return snprintf( str, size, "%lld", (long long int)tmp64 );
      default:
          break;
    }
    return snprintf( str, size, "err" );
}

int ipfix_snprint_uint( char *str, size_t size, void *data, size_t len )
{
    uint8_t       tmp8;
    uint16_t      tmp16;
    uint32_t      tmp32;
    uint64_t      tmp64;

    switch ( len ) {
      case 1:
          memcpy( &tmp8, data, len );
          return snprintf( str, size, "%u", tmp8 );
      case 2:
          memcpy( &tmp16, data, len );
          //tmp16 = htons( tmp16 );
          return snprintf( str, size, "%u", tmp16 );
      case 4:
          memcpy( &tmp32, data, len );
          //tmp32 = htonl( tmp32 );
          return snprintf( str, size, "%u", (unsigned int)tmp32 );
      case 8:
          memcpy( &tmp64, data, len );
          //tmp64 = HTONLL( tmp64 );
          return snprintf( str, size, "%llu", (long long unsigned int)tmp64 );
      default:
          break;
    }
    return snprintf( str, size, "err" );
}

int ipfix_encode_bytes( void *in, void *out, size_t len )
{
    if ( in != out )
        memcpy( out, in, len );
    return 0;
}

int ipfix_decode_bytes( void *in, void *out, size_t len )
{
    if ( in != out )
        memcpy( out, in, len );
    return 0;
}

int ipfix_snprint_bytes( char *str, size_t size, void *data, size_t len )
{
    size_t  i, n = 0;
    uint8_t *in = (uint8_t*) data;

    if ( size < 4 )
        return snprintf( str, size, "err" );

    while ( ((len*2) + 2) > size )
        len--;

    sprintf( str, "0x" );
    n = 2;
    for( i=0; i<len; i++ ) {
        sprintf( str+n, "%02x", *in );
        n += 2;
        in++;
    }
    return n;
}

int ipfix_snprint_string( char *str, size_t size, void *data, size_t len )
{
    ssize_t  i;
    uint8_t *in = (uint8_t*) data;

    for( i=len-1; i>=0; i-- ) {
        if ( in[i] == '\0' ) {
            return snprintf( str, size, "%s", in );
        }
    }

    if ( len < size ) {
        memcpy( str, in, len );
        str[len] = '\0';
        return len;
    }

    return snprintf( str, size, "err" );
}

int ipfix_snprint_ipaddr( char *str, size_t size, void *data, size_t len )
{
    uint8_t *in = (uint8_t*)data;
    char    tmpbuf[100];

    switch ( len ) {
      case 4:
          return snprintf( str, size, "%u.%u.%u.%u",
                           in[0], in[1], in[2], in[3] );
      case 16:
      {
          /** change this!
           */
          uint16_t  i, tmp16;

          for( i=0, *tmpbuf=0; i<16; i+=2 ) {
              memcpy( &tmp16, (char*)data+i, 2 );
              tmp16 = htons( tmp16 );
              sprintf( tmpbuf+strlen(tmpbuf), "%s%x", i?":":"", tmp16 );
          }
          return snprintf( str, size, "%s", tmpbuf );
      }

      default:
          return ipfix_snprint_bytes( str, size, data, len );
    }
}

int ipfix_encode_float( void *in, void *out, size_t len )
{
    uint32_t      tmp32;
    uint64_t      tmp64;

    switch ( len ) {
      case 4:
          memcpy( &tmp32, in, len );
          tmp32 = htonl( tmp32 );
          memcpy( out, &tmp32, len );
          break;
      case 8:
          memcpy( &tmp64, in, len );
          tmp64 = HTONLL( tmp64 );
          memcpy( out, &tmp64, len );
          break;
      default:
          memset( out, 0xff, len );
          return -1;
    }

    return 0;
}

int ipfix_decode_float( void *in, void *out, size_t len )
{
    return ipfix_encode_float( in, out, len );
}

int ipfix_snprint_float( char *str, size_t size, void *data, size_t len )
{
    //float tmp32;
    //double tmp64;

    switch ( len ) {
      case 4:
          //ipfix_decode_float( data, &tmp32, 4);
          return snprintf( str, size, "%f", *(float*)data );
      case 8:
          //ipfix_decode_float( data, &tmp64, 8);
          return snprintf( str, size, "%lf", *(double*)data);
      default:
          break;
    }

    return snprintf( str, size, "err" );
}

	
}