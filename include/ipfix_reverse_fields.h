/// ----------------------------------------*- mode: C; -*--
/// @file ipfix_reverse_fields.h
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: ipfix_reverse_fields.h 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/ipfix_reverse_fields.h $
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

/* 
 * IPFIX structs, types and definitions
 *
 * This is a generated file. Do not edit! 
 *
 */

/* ipfix information element list
 */
ipfix_field_type_t ipfix_reverse_field_types[] = {
   { REV_PEN, IPFIX_FT_OCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse octetDeltaCount", "" }, 
   { REV_PEN, IPFIX_FT_PACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse packetDeltaCount", "" }, 
   { REV_PEN, IPFIX_FT_FLOWS, 8, IPFIX_CODING_UINT, 
     "reverse flows", "reverse Netflow Number of Flows that were aggregated" }, 
   { REV_PEN, IPFIX_FT_PROTOCOLIDENTIFIER, 1, IPFIX_CODING_UINT, 
     "reverse protocolIdentifier", "" }, 
   { REV_PEN, IPFIX_FT_IPCLASSOFSERVICE, 1, IPFIX_CODING_UINT, 
     "reverse ipClassOfService", "" }, 
   { REV_PEN, IPFIX_FT_TCPCONTROLBITS, 1, IPFIX_CODING_UINT, 
     "reverse tcpControlBits", "" }, 
   { REV_PEN, IPFIX_FT_SOURCETRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "reverse sourceTransportPort", "" }, 
   { REV_PEN, IPFIX_FT_SOURCEIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse sourceIPv4Address", "" }, 
   { REV_PEN, IPFIX_FT_SOURCEIPV4PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse sourceIPv4PrefixLength", "" }, 
   { REV_PEN, IPFIX_FT_INGRESSINTERFACE, 4, IPFIX_CODING_UINT, 
     "reverse ingressInterface", "" }, 
   { REV_PEN, IPFIX_FT_DESTINATIONTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "reverse destinationTransportPort", "" }, 
   { REV_PEN, IPFIX_FT_DESTINATIONIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse destinationIPv4Address", "" }, 
   { REV_PEN, IPFIX_FT_DESTINATIONIPV4PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse destinationIPv4PrefixLength", "" }, 
   { REV_PEN, IPFIX_FT_EGRESSINTERFACE, 4, IPFIX_CODING_UINT, 
     "reverse egressInterface", "" }, 
   { REV_PEN, IPFIX_FT_IPNEXTHOPIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse ipNextHopIPv4Address", "" }, 
   { REV_PEN, IPFIX_FT_BGPSOURCEASNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse bgpSourceAsNumber", "" }, 
   { REV_PEN, IPFIX_FT_BGPDESTINATIONASNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse bgpDestinationAsNumber", "" }, 
   { REV_PEN, IPFIX_FT_BGPNEXTHOPIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse bgpNextHopIPv4Address", "" }, 
   { REV_PEN, IPFIX_FT_POSTMCASTPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postMCastPacketDeltaCount", "" }, 
   { REV_PEN, IPFIX_FT_POSTMCASTOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postMCastOctetDeltaCount", "" }, 
   { REV_PEN, IPFIX_FT_FLOWENDSYSUPTIME, 4, IPFIX_CODING_UINT, 
     "reverse flowEndSysUpTime", "" }, 
   { REV_PEN, IPFIX_FT_FLOWSTARTSYSUPTIME, 4, IPFIX_CODING_UINT, 
     "reverse flowStartSysUpTime", "" }, 
   { REV_PEN, IPFIX_FT_POSTOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postOctetDeltaCount", "" }, 
   { REV_PEN, IPFIX_FT_POSTPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postPacketDeltaCount", "" }, 
   { REV_PEN, IPFIX_FT_MINIMUMIPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "reverse minimumIpTotalLength", "" }, 
   { REV_PEN, IPFIX_FT_MAXIMUMIPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "reverse maximumIpTotalLength", "" }, 
   { REV_PEN, IPFIX_FT_SOURCEIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse sourceIPv6Address", "" }, 
   { REV_PEN, IPFIX_FT_DESTINATIONIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse destinationIPv6Address", "" }, 
   { REV_PEN, IPFIX_FT_SOURCEIPV6PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse sourceIPv6PrefixLength", "" }, 
   { REV_PEN, IPFIX_FT_DESTINATIONIPV6PREFIXLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse destinationIPv6PrefixLength", "" }, 
   { REV_PEN, IPFIX_FT_FLOWLABELIPV6, 4, IPFIX_CODING_UINT, 
     "reverse flowLabelIPv6", "" }, 
   { REV_PEN, IPFIX_FT_ICMPTYPECODEIPV4, 2, IPFIX_CODING_UINT, 
     "reverse icmpTypeCodeIPv4", "" }, 
   { REV_PEN, IPFIX_FT_IGMPTYPE, 1, IPFIX_CODING_UINT, 
     "reverse igmpType", "" }, 
   { REV_PEN, IPFIX_FT_SAMPLING_INTERVAL, 4, IPFIX_CODING_UINT, 
     "reverse sampling_interval", "reverse Netflow Sampling Interval" }, 
   { REV_PEN, IPFIX_FT_SAMPLING_ALGORITHM, 1, IPFIX_CODING_UINT, 
     "reverse sampling_algorithm", "reverse Netflow Sampling Algorithm" }, 
   { REV_PEN, IPFIX_FT_FLOWACTIVETIMEOUT, 2, IPFIX_CODING_UINT, 
     "reverse flowActiveTimeout", "" }, 
   { REV_PEN, IPFIX_FT_FLOWIDLETIMEOUT, 2, IPFIX_CODING_UINT, 
     "reverse flowIdleTimeout", "" }, 
   { REV_PEN, IPFIX_FT_ENGINE_TYPE, 1, IPFIX_CODING_UINT, 
     "reverse engine_type", "reverse Netflow Engine Type" }, 
   { REV_PEN, IPFIX_FT_ENGINE_ID, 1, IPFIX_CODING_UINT, 
     "reverse engine_id", "reverse Netflow Engine ID" }, 
   { REV_PEN, IPFIX_FT_EXPORTEDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse exportedOctetTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_EXPORTEDMESSAGETOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse exportedMessageTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_EXPORTEDFLOWRECORDTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse exportedFlowRecordTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_SOURCEIPV4PREFIX, 4, IPFIX_CODING_IPADDR, 
     "reverse sourceIPv4Prefix", "" }, 
   { REV_PEN, IPFIX_FT_DESTINATIONIPV4PREFIX, 4, IPFIX_CODING_IPADDR, 
     "reverse destinationIPv4Prefix", "" }, 
   { REV_PEN, IPFIX_FT_MPLSTOPLABELTYPE, 1, IPFIX_CODING_UINT, 
     "reverse mplsTopLabelType", "" }, 
   { REV_PEN, IPFIX_FT_MPLSTOPLABELIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse mplsTopLabelIPv4Address", "" }, 
   { REV_PEN, IPFIX_FT_FLOW_SAMPLER_ID, 1, IPFIX_CODING_UINT, 
     "reverse flow_sampler_id", "reverse Netflow Flow Sampler ID" }, 
   { REV_PEN, IPFIX_FT_FLOW_SAMPLER_MODE, 1, IPFIX_CODING_UINT, 
     "reverse flow_sampler_mode", "reverse Netflow Flow Sampler Mode" }, 
   { REV_PEN, IPFIX_FT_FLOW_SAMPLER_RANDOM_INTERVAL, 4, IPFIX_CODING_UINT, 
     "reverse flow_sampler_random_interval", "reverse Netflow Packet Sample Interval" }, 
   { REV_PEN, IPFIX_FT_MINIMUMTTL, 1, IPFIX_CODING_UINT, 
     "reverse minimumTTL", "" }, 
   { REV_PEN, IPFIX_FT_MAXIMUMTTL, 1, IPFIX_CODING_UINT, 
     "reverse maximumTTL", "" }, 
   { REV_PEN, IPFIX_FT_FRAGMENTIDENTIFICATION, 4, IPFIX_CODING_UINT, 
     "reverse fragmentIdentification", "" }, 
   { REV_PEN, IPFIX_FT_POSTIPCLASSOFSERVICE, 1, IPFIX_CODING_UINT, 
     "reverse postIpClassOfService", "" }, 
   { REV_PEN, IPFIX_FT_SOURCEMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "reverse sourceMacAddress", "" }, 
   { REV_PEN, IPFIX_FT_POSTDESTINATIONMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "reverse postDestinationMacAddress", "" }, 
   { REV_PEN, IPFIX_FT_VLANID, 2, IPFIX_CODING_UINT, 
     "reverse vlanId", "" }, 
   { REV_PEN, IPFIX_FT_POSTVLANID, 2, IPFIX_CODING_UINT, 
     "reverse postVlanId", "" }, 
   { REV_PEN, IPFIX_FT_IPVERSION, 1, IPFIX_CODING_UINT, 
     "reverse ipVersion", "" }, 
   { REV_PEN, IPFIX_FT_FLOWDIRECTION, 1, IPFIX_CODING_UINT, 
     "reverse flowDirection", "" }, 
   { REV_PEN, IPFIX_FT_IPNEXTHOPIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse ipNextHopIPv6Address", "" }, 
   { REV_PEN, IPFIX_FT_BGPNEXTHOPIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse bgpNextHopIPv6Address", "" }, 
   { REV_PEN, IPFIX_FT_IPV6EXTENSIONHEADERS, 4, IPFIX_CODING_UINT, 
     "reverse ipv6ExtensionHeaders", "" }, 
   { REV_PEN, IPFIX_FT_MPLSTOPLABELSTACKSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsTopLabelStackSection", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION2, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection2", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION3, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection3", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION4, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection4", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION5, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection5", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION6, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection6", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION7, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection7", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION8, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection8", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION9, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection9", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION10, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection10", "" }, 
   { REV_PEN, IPFIX_FT_DESTINATIONMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "reverse destinationMacAddress", "" }, 
   { REV_PEN, IPFIX_FT_POSTSOURCEMACADDRESS, 6, IPFIX_CODING_BYTES, 
     "reverse postSourceMacAddress", "" }, 
   { REV_PEN, IPFIX_FT_OCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse octetTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_PACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse packetTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_FRAGMENTOFFSET, 2, IPFIX_CODING_UINT, 
     "reverse fragmentOffset", "" }, 
   { REV_PEN, IPFIX_FT_MPLSVPNROUTEDISTINGUISHER, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsVpnRouteDistinguisher", "" }, 
   { REV_PEN, IPFIX_FT_BGPNEXTADJACENTASNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse bgpNextAdjacentAsNumber", "" }, 
   { REV_PEN, IPFIX_FT_BGPPREVADJACENTASNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse bgpPrevAdjacentAsNumber", "" }, 
   { REV_PEN, IPFIX_FT_EXPORTERIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse exporterIPv4Address", "" }, 
   { REV_PEN, IPFIX_FT_EXPORTERIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse exporterIPv6Address", "" }, 
   { REV_PEN, IPFIX_FT_DROPPEDOCTETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse droppedOctetDeltaCount", "" }, 
   { REV_PEN, IPFIX_FT_DROPPEDPACKETDELTACOUNT, 8, IPFIX_CODING_UINT, 
     "reverse droppedPacketDeltaCount", "" }, 
   { REV_PEN, IPFIX_FT_DROPPEDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse droppedOctetTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_DROPPEDPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse droppedPacketTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_FLOWENDREASON, 1, IPFIX_CODING_UINT, 
     "reverse flowEndReason", "" }, 
   { REV_PEN, IPFIX_FT_COMMONPROPERTIESID, 8, IPFIX_CODING_UINT, 
     "reverse commonPropertiesId", "" }, 
   { REV_PEN, IPFIX_FT_OBSERVATIONPOINTID, 4, IPFIX_CODING_UINT, 
     "reverse observationPointId", "" }, 
   { REV_PEN, IPFIX_FT_ICMPTYPECODEIPV6, 2, IPFIX_CODING_UINT, 
     "reverse icmpTypeCodeIPv6", "" }, 
   { REV_PEN, IPFIX_FT_MPLSTOPLABELIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse mplsTopLabelIPv6Address", "" }, 
   { REV_PEN, IPFIX_FT_LINECARDID, 4, IPFIX_CODING_UINT, 
     "reverse lineCardId", "" }, 
   { REV_PEN, IPFIX_FT_PORTID, 4, IPFIX_CODING_UINT, 
     "reverse portId", "" }, 
   { REV_PEN, IPFIX_FT_METERINGPROCESSID, 4, IPFIX_CODING_UINT, 
     "reverse meteringProcessId", "" }, 
   { REV_PEN, IPFIX_FT_EXPORTINGPROCESSID, 4, IPFIX_CODING_UINT, 
     "reverse exportingProcessId", "" }, 
   { REV_PEN, IPFIX_FT_TEMPLATEID, 2, IPFIX_CODING_UINT, 
     "reverse templateId", "" }, 
   { REV_PEN, IPFIX_FT_WLANCHANNELID, 1, IPFIX_CODING_UINT, 
     "reverse wlanChannelId", "" }, 
   { REV_PEN, IPFIX_FT_WLANSSID, 65535, IPFIX_CODING_STRING, 
     "reverse wlanSSID", "" }, 
   { REV_PEN, IPFIX_FT_FLOWID, 8, IPFIX_CODING_UINT, 
     "reverse flowId", "" }, 
   { REV_PEN, IPFIX_FT_OBSERVATIONDOMAINID, 4, IPFIX_CODING_UINT, 
     "reverse observationDomainId", "" }, 
   { REV_PEN, IPFIX_FT_FLOWSTARTSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowStartSeconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWENDSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowEndSeconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWSTARTMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "reverse flowStartMilliseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWENDMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "reverse flowEndMilliseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWSTARTMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse flowStartMicroseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWENDMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse flowEndMicroseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWSTARTNANOSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse flowStartNanoseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWENDNANOSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse flowEndNanoseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWSTARTDELTAMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowStartDeltaMicroseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWENDDELTAMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowEndDeltaMicroseconds", "" }, 
   { REV_PEN, IPFIX_FT_SYSTEMINITTIMEMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "reverse systemInitTimeMilliseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWDURATIONMILLISECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowDurationMilliseconds", "" }, 
   { REV_PEN, IPFIX_FT_FLOWDURATIONMICROSECONDS, 4, IPFIX_CODING_UINT, 
     "reverse flowDurationMicroseconds", "" }, 
   { REV_PEN, IPFIX_FT_OBSERVEDFLOWTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse observedFlowTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_IGNOREDPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse ignoredPacketTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_IGNOREDOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse ignoredOctetTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_NOTSENTFLOWTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse notSentFlowTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_NOTSENTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse notSentPacketTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_NOTSENTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse notSentOctetTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_DESTINATIONIPV6PREFIX, 16, IPFIX_CODING_IPADDR, 
     "reverse destinationIPv6Prefix", "" }, 
   { REV_PEN, IPFIX_FT_SOURCEIPV6PREFIX, 16, IPFIX_CODING_IPADDR, 
     "reverse sourceIPv6Prefix", "" }, 
   { REV_PEN, IPFIX_FT_POSTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postOctetTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_POSTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postPacketTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_FLOWKEYINDICATOR, 8, IPFIX_CODING_UINT, 
     "reverse flowKeyIndicator", "" }, 
   { REV_PEN, IPFIX_FT_POSTMCASTPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postMCastPacketTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_POSTMCASTOCTETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse postMCastOctetTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_ICMPTYPEIPV4, 1, IPFIX_CODING_UINT, 
     "reverse icmpTypeIPv4", "" }, 
   { REV_PEN, IPFIX_FT_ICMPCODEIPV4, 1, IPFIX_CODING_UINT, 
     "reverse icmpCodeIPv4", "" }, 
   { REV_PEN, IPFIX_FT_ICMPTYPEIPV6, 1, IPFIX_CODING_UINT, 
     "reverse icmpTypeIPv6", "" }, 
   { REV_PEN, IPFIX_FT_ICMPCODEIPV6, 1, IPFIX_CODING_UINT, 
     "reverse icmpCodeIPv6", "" }, 
   { REV_PEN, IPFIX_FT_UDPSOURCEPORT, 2, IPFIX_CODING_UINT, 
     "reverse udpSourcePort", "" }, 
   { REV_PEN, IPFIX_FT_UDPDESTINATIONPORT, 2, IPFIX_CODING_UINT, 
     "reverse udpDestinationPort", "" }, 
   { REV_PEN, IPFIX_FT_TCPSOURCEPORT, 2, IPFIX_CODING_UINT, 
     "reverse tcpSourcePort", "" }, 
   { REV_PEN, IPFIX_FT_TCPDESTINATIONPORT, 2, IPFIX_CODING_UINT, 
     "reverse tcpDestinationPort", "" }, 
   { REV_PEN, IPFIX_FT_TCPSEQUENCENUMBER, 4, IPFIX_CODING_UINT, 
     "reverse tcpSequenceNumber", "" }, 
   { REV_PEN, IPFIX_FT_TCPACKNOWLEDGEMENTNUMBER, 4, IPFIX_CODING_UINT, 
     "reverse tcpAcknowledgementNumber", "" }, 
   { REV_PEN, IPFIX_FT_TCPWINDOWSIZE, 2, IPFIX_CODING_UINT, 
     "reverse tcpWindowSize", "" }, 
   { REV_PEN, IPFIX_FT_TCPURGENTPOINTER, 2, IPFIX_CODING_UINT, 
     "reverse tcpUrgentPointer", "" }, 
   { REV_PEN, IPFIX_FT_TCPHEADERLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse tcpHeaderLength", "" }, 
   { REV_PEN, IPFIX_FT_IPHEADERLENGTH, 1, IPFIX_CODING_UINT, 
     "reverse ipHeaderLength", "" }, 
   { REV_PEN, IPFIX_FT_TOTALLENGTHIPV4, 2, IPFIX_CODING_UINT, 
     "reverse totalLengthIPv4", "" }, 
   { REV_PEN, IPFIX_FT_PAYLOADLENGTHIPV6, 2, IPFIX_CODING_UINT, 
     "reverse payloadLengthIPv6", "" }, 
   { REV_PEN, IPFIX_FT_IPTTL, 1, IPFIX_CODING_UINT, 
     "reverse ipTTL", "" }, 
   { REV_PEN, IPFIX_FT_NEXTHEADERIPV6, 1, IPFIX_CODING_UINT, 
     "reverse nextHeaderIPv6", "" }, 
   { REV_PEN, IPFIX_FT_MPLSPAYLOADLENGTH, 4, IPFIX_CODING_UINT, 
     "reverse mplsPayloadLength", "" }, 
   { REV_PEN, IPFIX_FT_IPDIFFSERVCODEPOINT, 1, IPFIX_CODING_UINT, 
     "reverse ipDiffServCodePoint", "" }, 
   { REV_PEN, IPFIX_FT_IPPRECEDENCE, 1, IPFIX_CODING_UINT, 
     "reverse ipPrecedence", "" }, 
   { REV_PEN, IPFIX_FT_FRAGMENTFLAGS, 1, IPFIX_CODING_UINT, 
     "reverse fragmentFlags", "" }, 
   { REV_PEN, IPFIX_FT_OCTETDELTASUMOFSQUARES, 8, IPFIX_CODING_UINT, 
     "reverse octetDeltaSumOfSquares", "" }, 
   { REV_PEN, IPFIX_FT_OCTETTOTALSUMOFSQUARES, 8, IPFIX_CODING_UINT, 
     "reverse octetTotalSumOfSquares", "" }, 
   { REV_PEN, IPFIX_FT_MPLSTOPLABELTTL, 1, IPFIX_CODING_UINT, 
     "reverse mplsTopLabelTTL", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKLENGTH, 4, IPFIX_CODING_UINT, 
     "reverse mplsLabelStackLength", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKDEPTH, 4, IPFIX_CODING_UINT, 
     "reverse mplsLabelStackDepth", "" }, 
   { REV_PEN, IPFIX_FT_MPLSTOPLABELEXP, 1, IPFIX_CODING_UINT, 
     "reverse mplsTopLabelExp", "" }, 
   { REV_PEN, IPFIX_FT_IPPAYLOADLENGTH, 4, IPFIX_CODING_UINT, 
     "reverse ipPayloadLength", "" }, 
   { REV_PEN, IPFIX_FT_UDPMESSAGELENGTH, 2, IPFIX_CODING_UINT, 
     "reverse udpMessageLength", "" }, 
   { REV_PEN, IPFIX_FT_ISMULTICAST, 1, IPFIX_CODING_UINT, 
     "reverse isMulticast", "" }, 
   { REV_PEN, IPFIX_FT_IPV4IHL, 1, IPFIX_CODING_UINT, 
     "reverse ipv4IHL", "" }, 
   { REV_PEN, IPFIX_FT_IPV4OPTIONS, 4, IPFIX_CODING_UINT, 
     "reverse ipv4Options", "" }, 
   { REV_PEN, IPFIX_FT_TCPOPTIONS, 8, IPFIX_CODING_UINT, 
     "reverse tcpOptions", "" }, 
   { REV_PEN, IPFIX_FT_PADDINGOCTETS, 65535, IPFIX_CODING_BYTES, 
     "reverse paddingOctets", "" }, 
   { REV_PEN, IPFIX_FT_COLLECTORIPV4ADDRESS, 4, IPFIX_CODING_IPADDR, 
     "reverse collectorIPv4Address", "" }, 
   { REV_PEN, IPFIX_FT_COLLECTORIPV6ADDRESS, 16, IPFIX_CODING_IPADDR, 
     "reverse collectorIPv6Address", "" }, 
   { REV_PEN, IPFIX_FT_COLLECTORINTERFACE, 4, IPFIX_CODING_UINT, 
     "reverse collectorInterface", "" }, 
   { REV_PEN, IPFIX_FT_COLLECTORPROTOCOLVERSION, 1, IPFIX_CODING_UINT, 
     "reverse collectorProtocolVersion", "" }, 
   { REV_PEN, IPFIX_FT_COLLECTORTRANSPORTPROTOCOL, 1, IPFIX_CODING_UINT, 
     "reverse collectorTransportProtocol", "" }, 
   { REV_PEN, IPFIX_FT_COLLECTORTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "reverse collectorTransportPort", "" }, 
   { REV_PEN, IPFIX_FT_EXPORTERTRANSPORTPORT, 2, IPFIX_CODING_UINT, 
     "reverse exporterTransportPort", "" }, 
   { REV_PEN, IPFIX_FT_TCPSYNTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpSynTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_TCPFINTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpFinTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_TCPRSTTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpRstTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_TCPPSHTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpPshTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_TCPACKTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpAckTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_TCPURGTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "reverse tcpUrgTotalCount", "" }, 
   { REV_PEN, IPFIX_FT_IPTOTALLENGTH, 8, IPFIX_CODING_UINT, 
     "reverse ipTotalLength", "" }, 
   { REV_PEN, IPFIX_FT_POSTMPLSTOPLABELEXP, 1, IPFIX_CODING_UINT, 
     "reverse postMplsTopLabelExp", "" }, 
   { REV_PEN, IPFIX_FT_TCPWINDOWSCALE, 2, IPFIX_CODING_UINT, 
     "reverse tcpWindowScale", "" }, 
   { REV_PEN, IPFIX_FT_OBSERVATIONPOINTID_PSAMP, 8, IPFIX_CODING_UINT, 
     "reverse observationPointId_PSAMP", "" }, 
   { REV_PEN, IPFIX_FT_SELECTIONSEQUENCEID, 8, IPFIX_CODING_UINT, 
     "reverse selectionSequenceId", "" }, 
   { REV_PEN, IPFIX_FT_SELECTORID, 2, IPFIX_CODING_UINT, 
     "reverse selectorId", "" }, 
   { REV_PEN, IPFIX_FT_INFORMATIONELEMENTID, 2, IPFIX_CODING_UINT, 
     "reverse informationElementId", "" }, 
   { REV_PEN, IPFIX_FT_SELECTORALGORITHM, 2, IPFIX_CODING_UINT, 
     "reverse selectorAlgorithm", "" }, 
   { REV_PEN, IPFIX_FT_SAMPLINGPACKETINTERVAL, 4, IPFIX_CODING_UINT, 
     "reverse samplingPacketInterval", "" }, 
   { REV_PEN, IPFIX_FT_SAMPLINGPACKETSPACE, 4, IPFIX_CODING_UINT, 
     "reverse samplingPacketSpace", "" }, 
   { REV_PEN, IPFIX_FT_SAMPLINGTIMEINTERVAL, 8, IPFIX_CODING_NTP, 
     "reverse samplingTimeInterval", "" }, 
   { REV_PEN, IPFIX_FT_SAMPLINGTIMESPACE, 8, IPFIX_CODING_NTP, 
     "reverse samplingTimeSpace", "" }, 
   { REV_PEN, IPFIX_FT_SAMPLINGSIZE, 4, IPFIX_CODING_UINT, 
     "reverse samplingSize", "" }, 
   { REV_PEN, IPFIX_FT_SAMPLINGPOPULATION, 4, IPFIX_CODING_UINT, 
     "reverse samplingPopulation", "" }, 
   { REV_PEN, IPFIX_FT_SAMPLINGPROBABILITY, 8, IPFIX_CODING_FLOAT, 
     "reverse samplingProbability", "" }, 
   { REV_PEN, IPFIX_FT_DATALINKFRAMESIZE, 4, IPFIX_CODING_UINT, 
     "reverse dataLinkFrameSize", "" }, 
   { REV_PEN, IPFIX_FT_IPHEADERPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse ipHeaderPacketSection", "" }, 
   { REV_PEN, IPFIX_FT_IPPAYLOADPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse ipPayloadPacketSection", "" }, 
   { REV_PEN, IPFIX_FT_DATALINKFRAMESECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse dataLinkFrameSection", "" }, 
   { REV_PEN, IPFIX_FT_MPLSLABELSTACKSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsLabelStackSection", "" }, 
   { REV_PEN, IPFIX_FT_MPLSPAYLOADPACKETSECTION, 65535, IPFIX_CODING_BYTES, 
     "reverse mplsPayloadPacketSection", "" }, 
   { REV_PEN, IPFIX_FT_PACKETSOBSERVED, 8, IPFIX_CODING_UINT, 
     "reverse packetsObserved", "" }, 
   { REV_PEN, IPFIX_FT_PACKETSSELECTED, 8, IPFIX_CODING_UINT, 
     "reverse packetsSelected", "" }, 
   { REV_PEN, IPFIX_FT_FIXEDERROR, 8, IPFIX_CODING_FLOAT, 
     "reverse fixedError", "" }, 
   { REV_PEN, IPFIX_FT_RELATIVEERROR, 8, IPFIX_CODING_FLOAT, 
     "reverse relativeError", "" }, 
   { REV_PEN, IPFIX_FT_OBSERVATIONTIMESECONDS, 4, IPFIX_CODING_UINT, 
     "reverse observationTimeSeconds", "" }, 
   { REV_PEN, IPFIX_FT_OBSERVATIONTIMEMILLISECONDS, 8, IPFIX_CODING_UINT, 
     "reverse observationTimeMilliseconds", "" }, 
   { REV_PEN, IPFIX_FT_OBSERVATIONTIMEMICROSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse observationTimeMicroseconds", "" }, 
   { REV_PEN, IPFIX_FT_OBSERVATIONTIMENANOSECONDS, 8, IPFIX_CODING_NTP, 
     "reverse observationTimeNanoseconds", "" }, 
   { REV_PEN, IPFIX_FT_DIGESTHASHVALUE, 8, IPFIX_CODING_UINT, 
     "reverse digestHashValue", "" }, 
   { REV_PEN, IPFIX_FT_HASHIPPAYLOADOFFSET, 8, IPFIX_CODING_UINT, 
     "reverse hashIPPayloadOffset", "" }, 
   { REV_PEN, IPFIX_FT_HASHIPPAYLOADSIZE, 8, IPFIX_CODING_UINT, 
     "reverse hashIPPayloadSize", "" }, 
   { REV_PEN, IPFIX_FT_HASHOUTPUTRANGEMIN, 8, IPFIX_CODING_UINT, 
     "reverse hashOutputRangeMin", "" }, 
   { REV_PEN, IPFIX_FT_HASHOUTPUTRANGEMAX, 8, IPFIX_CODING_UINT, 
     "reverse hashOutputRangeMax", "" }, 
   { REV_PEN, IPFIX_FT_HASHSELECTEDRANGEMIN, 8, IPFIX_CODING_UINT, 
     "reverse hashSelectedRangeMin", "" }, 
   { REV_PEN, IPFIX_FT_HASHSELECTEDRANGEMAX, 8, IPFIX_CODING_UINT, 
     "reverse hashSelectedRangeMax", "" }, 
   { REV_PEN, IPFIX_FT_HASHDIGESTOUTPUT, 1, IPFIX_CODING_BYTES, 
     "reverse hashDigestOutput", "" }, 
   { REV_PEN, IPFIX_FT_HASHINITIALISERVALUE, 8, IPFIX_CODING_UINT, 
     "reverse hashInitialiserValue", "" }, 
   { REV_PEN, 0, -1, 0, NULL, NULL, }
};

