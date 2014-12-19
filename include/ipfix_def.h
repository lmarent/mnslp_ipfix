/// ----------------------------------------*- mode: C; -*--
/// @file ipfix_def.h
/// Tools for processing IPFIX messages in NSIS metering.
/// ----------------------------------------------------------
/// $Id: ipfix_def.h 2558 2014-11-26 14:11:00 amarentes $
/// $HeadURL: https://./include/ipfix_def.h $
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


#ifndef IPFIX_DEF_H
#define IPFIX_DEF_H

#include <cstddef>
#include <stdlib.h>
#include <string>


#define IPFIX_CODING_INT    1
#define IPFIX_CODING_UINT   2
#define IPFIX_CODING_BYTES  3
#define IPFIX_CODING_STRING 4
#define IPFIX_CODING_FLOAT  5
#define IPFIX_CODING_NTP    6
#define IPFIX_CODING_IPADDR 7

#define REV_PEN 29305 /* reverse elements private enterprise number, see RFC5103 */


/**
 * This structure represents a field to be exhange in the message
 */ 
typedef struct
{
    int         	eno;                ///< enterprise number or 0
    int         	ftype;              ///< field type - defines the type of field
    ssize_t     	length;             ///< field length 
    int        	 	coding;				///< it says how to encode and decode the field.
    std::string     name;				///< Name of the field
    std::string     documentation;      ///< Ued for fields with documentation.

} ipfix_field_type_t;


#define IPFIX_FT_OCTETDELTACOUNT              1
#define IPFIX_FT_PACKETDELTACOUNT             2
#define IPFIX_FT_FLOWS                        3
#define IPFIX_FT_PROTOCOLIDENTIFIER           4
#define IPFIX_FT_IPCLASSOFSERVICE             5
#define IPFIX_FT_TCPCONTROLBITS               6
#define IPFIX_FT_SOURCETRANSPORTPORT          7
#define IPFIX_FT_SOURCEIPV4ADDRESS            8
#define IPFIX_FT_SOURCEIPV4PREFIXLENGTH       9
#define IPFIX_FT_INGRESSINTERFACE             10
#define IPFIX_FT_DESTINATIONTRANSPORTPORT     11
#define IPFIX_FT_DESTINATIONIPV4ADDRESS       12
#define IPFIX_FT_DESTINATIONIPV4PREFIXLENGTH  13
#define IPFIX_FT_EGRESSINTERFACE              14
#define IPFIX_FT_IPNEXTHOPIPV4ADDRESS         15
#define IPFIX_FT_BGPSOURCEASNUMBER            16
#define IPFIX_FT_BGPDESTINATIONASNUMBER       17
#define IPFIX_FT_BGPNEXTHOPIPV4ADDRESS        18
#define IPFIX_FT_POSTMCASTPACKETDELTACOUNT    19
#define IPFIX_FT_POSTMCASTOCTETDELTACOUNT     20
#define IPFIX_FT_FLOWENDSYSUPTIME             21
#define IPFIX_FT_FLOWSTARTSYSUPTIME           22
#define IPFIX_FT_POSTOCTETDELTACOUNT          23
#define IPFIX_FT_POSTPACKETDELTACOUNT         24
#define IPFIX_FT_MINIMUMIPTOTALLENGTH         25
#define IPFIX_FT_MAXIMUMIPTOTALLENGTH         26
#define IPFIX_FT_SOURCEIPV6ADDRESS            27
#define IPFIX_FT_DESTINATIONIPV6ADDRESS       28
#define IPFIX_FT_SOURCEIPV6PREFIXLENGTH       29
#define IPFIX_FT_DESTINATIONIPV6PREFIXLENGTH  30
#define IPFIX_FT_FLOWLABELIPV6                31
#define IPFIX_FT_ICMPTYPECODEIPV4             32
#define IPFIX_FT_IGMPTYPE                     33
#define IPFIX_FT_SAMPLING_INTERVAL            34
#define IPFIX_FT_SAMPLING_ALGORITHM           35
#define IPFIX_FT_FLOWACTIVETIMEOUT            36
#define IPFIX_FT_FLOWIDLETIMEOUT              37
#define IPFIX_FT_ENGINE_TYPE                  38
#define IPFIX_FT_ENGINE_ID                    39
#define IPFIX_FT_EXPORTEDOCTETTOTALCOUNT      40
#define IPFIX_FT_EXPORTEDMESSAGETOTALCOUNT    41
#define IPFIX_FT_EXPORTEDFLOWRECORDTOTALCOUNT 42
#define IPFIX_FT_SOURCEIPV4PREFIX             44
#define IPFIX_FT_DESTINATIONIPV4PREFIX        45
#define IPFIX_FT_MPLSTOPLABELTYPE             46
#define IPFIX_FT_MPLSTOPLABELIPV4ADDRESS      47
#define IPFIX_FT_FLOW_SAMPLER_ID              48
#define IPFIX_FT_FLOW_SAMPLER_MODE            49
#define IPFIX_FT_FLOW_SAMPLER_RANDOM_INTERVAL 50
#define IPFIX_FT_MINIMUMTTL                   52
#define IPFIX_FT_MAXIMUMTTL                   53
#define IPFIX_FT_FRAGMENTIDENTIFICATION       54
#define IPFIX_FT_POSTIPCLASSOFSERVICE         55
#define IPFIX_FT_SOURCEMACADDRESS             56
#define IPFIX_FT_POSTDESTINATIONMACADDRESS    57
#define IPFIX_FT_VLANID                       58
#define IPFIX_FT_POSTVLANID                   59
#define IPFIX_FT_IPVERSION                    60
#define IPFIX_FT_FLOWDIRECTION                61
#define IPFIX_FT_IPNEXTHOPIPV6ADDRESS         62
#define IPFIX_FT_BGPNEXTHOPIPV6ADDRESS        63
#define IPFIX_FT_IPV6EXTENSIONHEADERS         64
#define IPFIX_FT_MPLSTOPLABELSTACKSECTION     70
#define IPFIX_FT_MPLSLABELSTACKSECTION2       71
#define IPFIX_FT_MPLSLABELSTACKSECTION3       72
#define IPFIX_FT_MPLSLABELSTACKSECTION4       73
#define IPFIX_FT_MPLSLABELSTACKSECTION5       74
#define IPFIX_FT_MPLSLABELSTACKSECTION6       75
#define IPFIX_FT_MPLSLABELSTACKSECTION7       76
#define IPFIX_FT_MPLSLABELSTACKSECTION8       77
#define IPFIX_FT_MPLSLABELSTACKSECTION9       78
#define IPFIX_FT_MPLSLABELSTACKSECTION10      79
#define IPFIX_FT_DESTINATIONMACADDRESS        80
#define IPFIX_FT_POSTSOURCEMACADDRESS         81
#define IPFIX_FT_OCTETTOTALCOUNT              85
#define IPFIX_FT_PACKETTOTALCOUNT             86
#define IPFIX_FT_FRAGMENTOFFSET               88
#define IPFIX_FT_MPLSVPNROUTEDISTINGUISHER    90
#define IPFIX_FT_BGPNEXTADJACENTASNUMBER      128
#define IPFIX_FT_BGPPREVADJACENTASNUMBER      129
#define IPFIX_FT_EXPORTERIPV4ADDRESS          130
#define IPFIX_FT_EXPORTERIPV6ADDRESS          131
#define IPFIX_FT_DROPPEDOCTETDELTACOUNT       132
#define IPFIX_FT_DROPPEDPACKETDELTACOUNT      133
#define IPFIX_FT_DROPPEDOCTETTOTALCOUNT       134
#define IPFIX_FT_DROPPEDPACKETTOTALCOUNT      135
#define IPFIX_FT_FLOWENDREASON                136
#define IPFIX_FT_COMMONPROPERTIESID           137
#define IPFIX_FT_OBSERVATIONPOINTID           138
#define IPFIX_FT_ICMPTYPECODEIPV6             139
#define IPFIX_FT_MPLSTOPLABELIPV6ADDRESS      140
#define IPFIX_FT_LINECARDID                   141
#define IPFIX_FT_PORTID                       142
#define IPFIX_FT_METERINGPROCESSID            143
#define IPFIX_FT_EXPORTINGPROCESSID           144
#define IPFIX_FT_TEMPLATEID                   145
#define IPFIX_FT_WLANCHANNELID                146
#define IPFIX_FT_WLANSSID                     147
#define IPFIX_FT_FLOWID                       148
#define IPFIX_FT_OBSERVATIONDOMAINID          149
#define IPFIX_FT_FLOWSTARTSECONDS             150
#define IPFIX_FT_FLOWENDSECONDS               151
#define IPFIX_FT_FLOWSTARTMILLISECONDS        152
#define IPFIX_FT_FLOWENDMILLISECONDS          153
#define IPFIX_FT_FLOWSTARTMICROSECONDS        154
#define IPFIX_FT_FLOWENDMICROSECONDS          155
#define IPFIX_FT_FLOWSTARTNANOSECONDS         156
#define IPFIX_FT_FLOWENDNANOSECONDS           157
#define IPFIX_FT_FLOWSTARTDELTAMICROSECONDS   158
#define IPFIX_FT_FLOWENDDELTAMICROSECONDS     159
#define IPFIX_FT_SYSTEMINITTIMEMILLISECONDS   160
#define IPFIX_FT_FLOWDURATIONMILLISECONDS     161
#define IPFIX_FT_FLOWDURATIONMICROSECONDS     162
#define IPFIX_FT_OBSERVEDFLOWTOTALCOUNT       163
#define IPFIX_FT_IGNOREDPACKETTOTALCOUNT      164
#define IPFIX_FT_IGNOREDOCTETTOTALCOUNT       165
#define IPFIX_FT_NOTSENTFLOWTOTALCOUNT        166
#define IPFIX_FT_NOTSENTPACKETTOTALCOUNT      167
#define IPFIX_FT_NOTSENTOCTETTOTALCOUNT       168
#define IPFIX_FT_DESTINATIONIPV6PREFIX        169
#define IPFIX_FT_SOURCEIPV6PREFIX             170
#define IPFIX_FT_POSTOCTETTOTALCOUNT          171
#define IPFIX_FT_POSTPACKETTOTALCOUNT         172
#define IPFIX_FT_FLOWKEYINDICATOR             173
#define IPFIX_FT_POSTMCASTPACKETTOTALCOUNT    174
#define IPFIX_FT_POSTMCASTOCTETTOTALCOUNT     175
#define IPFIX_FT_ICMPTYPEIPV4                 176
#define IPFIX_FT_ICMPCODEIPV4                 177
#define IPFIX_FT_ICMPTYPEIPV6                 178
#define IPFIX_FT_ICMPCODEIPV6                 179
#define IPFIX_FT_UDPSOURCEPORT                180
#define IPFIX_FT_UDPDESTINATIONPORT           181
#define IPFIX_FT_TCPSOURCEPORT                182
#define IPFIX_FT_TCPDESTINATIONPORT           183
#define IPFIX_FT_TCPSEQUENCENUMBER            184
#define IPFIX_FT_TCPACKNOWLEDGEMENTNUMBER     185
#define IPFIX_FT_TCPWINDOWSIZE                186
#define IPFIX_FT_TCPURGENTPOINTER             187
#define IPFIX_FT_TCPHEADERLENGTH              188
#define IPFIX_FT_IPHEADERLENGTH               189
#define IPFIX_FT_TOTALLENGTHIPV4              190
#define IPFIX_FT_PAYLOADLENGTHIPV6            191
#define IPFIX_FT_IPTTL                        192
#define IPFIX_FT_NEXTHEADERIPV6               193
#define IPFIX_FT_MPLSPAYLOADLENGTH            194
#define IPFIX_FT_IPDIFFSERVCODEPOINT          195
#define IPFIX_FT_IPPRECEDENCE                 196
#define IPFIX_FT_FRAGMENTFLAGS                197
#define IPFIX_FT_OCTETDELTASUMOFSQUARES       198
#define IPFIX_FT_OCTETTOTALSUMOFSQUARES       199
#define IPFIX_FT_MPLSTOPLABELTTL              200
#define IPFIX_FT_MPLSLABELSTACKLENGTH         201
#define IPFIX_FT_MPLSLABELSTACKDEPTH          202
#define IPFIX_FT_MPLSTOPLABELEXP              203
#define IPFIX_FT_IPPAYLOADLENGTH              204
#define IPFIX_FT_UDPMESSAGELENGTH             205
#define IPFIX_FT_ISMULTICAST                  206
#define IPFIX_FT_IPV4IHL                      207
#define IPFIX_FT_IPV4OPTIONS                  208
#define IPFIX_FT_TCPOPTIONS                   209
#define IPFIX_FT_PADDINGOCTETS                210
#define IPFIX_FT_COLLECTORIPV4ADDRESS         211
#define IPFIX_FT_COLLECTORIPV6ADDRESS         212
#define IPFIX_FT_COLLECTORINTERFACE           213
#define IPFIX_FT_COLLECTORPROTOCOLVERSION     214
#define IPFIX_FT_COLLECTORTRANSPORTPROTOCOL   215
#define IPFIX_FT_COLLECTORTRANSPORTPORT       216
#define IPFIX_FT_EXPORTERTRANSPORTPORT        217
#define IPFIX_FT_TCPSYNTOTALCOUNT             218
#define IPFIX_FT_TCPFINTOTALCOUNT             219
#define IPFIX_FT_TCPRSTTOTALCOUNT             220
#define IPFIX_FT_TCPPSHTOTALCOUNT             221
#define IPFIX_FT_TCPACKTOTALCOUNT             222
#define IPFIX_FT_TCPURGTOTALCOUNT             223
#define IPFIX_FT_IPTOTALLENGTH                224
#define IPFIX_FT_POSTMPLSTOPLABELEXP          237
#define IPFIX_FT_TCPWINDOWSCALE               238
#define IPFIX_FT_OBSERVATIONPOINTID_PSAMP     300
#define IPFIX_FT_SELECTIONSEQUENCEID          301
#define IPFIX_FT_SELECTORID                   302
#define IPFIX_FT_INFORMATIONELEMENTID         303
#define IPFIX_FT_SELECTORALGORITHM            304
#define IPFIX_FT_SAMPLINGPACKETINTERVAL       305
#define IPFIX_FT_SAMPLINGPACKETSPACE          306
#define IPFIX_FT_SAMPLINGTIMEINTERVAL         307
#define IPFIX_FT_SAMPLINGTIMESPACE            308
#define IPFIX_FT_SAMPLINGSIZE                 309
#define IPFIX_FT_SAMPLINGPOPULATION           310
#define IPFIX_FT_SAMPLINGPROBABILITY          311
#define IPFIX_FT_DATALINKFRAMESIZE            312
#define IPFIX_FT_IPHEADERPACKETSECTION        313
#define IPFIX_FT_IPPAYLOADPACKETSECTION       314
#define IPFIX_FT_DATALINKFRAMESECTION         315
#define IPFIX_FT_MPLSLABELSTACKSECTION        316
#define IPFIX_FT_MPLSPAYLOADPACKETSECTION     317
#define IPFIX_FT_PACKETSOBSERVED              318
#define IPFIX_FT_PACKETSSELECTED              319
#define IPFIX_FT_FIXEDERROR                   320
#define IPFIX_FT_RELATIVEERROR                321
#define IPFIX_FT_OBSERVATIONTIMESECONDS       322
#define IPFIX_FT_OBSERVATIONTIMEMILLISECONDS  323
#define IPFIX_FT_OBSERVATIONTIMEMICROSECONDS  324
#define IPFIX_FT_OBSERVATIONTIMENANOSECONDS   325
#define IPFIX_FT_DIGESTHASHVALUE              326
#define IPFIX_FT_HASHIPPAYLOADOFFSET          327
#define IPFIX_FT_HASHIPPAYLOADSIZE            328
#define IPFIX_FT_HASHOUTPUTRANGEMIN           329
#define IPFIX_FT_HASHOUTPUTRANGEMAX           330
#define IPFIX_FT_HASHSELECTEDRANGEMIN         331
#define IPFIX_FT_HASHSELECTEDRANGEMAX         332
#define IPFIX_FT_HASHDIGESTOUTPUT             333
#define IPFIX_FT_HASHINITIALISERVALUE         334

/* column name definitions
 */
#define IPFIX_CN_OCTETDELTACOUNT              "ie0_1"
#define IPFIX_CN_PACKETDELTACOUNT             "ie0_2"
#define IPFIX_CN_FLOWS                        "ie0_3"
#define IPFIX_CN_PROTOCOLIDENTIFIER           "ie0_4"
#define IPFIX_CN_IPCLASSOFSERVICE             "ie0_5"
#define IPFIX_CN_TCPCONTROLBITS               "ie0_6"
#define IPFIX_CN_SOURCETRANSPORTPORT          "ie0_7"
#define IPFIX_CN_SOURCEIPV4ADDRESS            "ie0_8"
#define IPFIX_CN_SOURCEIPV4PREFIXLENGTH       "ie0_9"
#define IPFIX_CN_INGRESSINTERFACE             "ie0_a"
#define IPFIX_CN_DESTINATIONTRANSPORTPORT     "ie0_b"
#define IPFIX_CN_DESTINATIONIPV4ADDRESS       "ie0_c"
#define IPFIX_CN_DESTINATIONIPV4PREFIXLENGTH  "ie0_d"
#define IPFIX_CN_EGRESSINTERFACE              "ie0_e"
#define IPFIX_CN_IPNEXTHOPIPV4ADDRESS         "ie0_f"
#define IPFIX_CN_BGPSOURCEASNUMBER            "ie0_10"
#define IPFIX_CN_BGPDESTINATIONASNUMBER       "ie0_11"
#define IPFIX_CN_BGPNEXTHOPIPV4ADDRESS        "ie0_12"
#define IPFIX_CN_POSTMCASTPACKETDELTACOUNT    "ie0_13"
#define IPFIX_CN_POSTMCASTOCTETDELTACOUNT     "ie0_14"
#define IPFIX_CN_FLOWENDSYSUPTIME             "ie0_15"
#define IPFIX_CN_FLOWSTARTSYSUPTIME           "ie0_16"
#define IPFIX_CN_POSTOCTETDELTACOUNT          "ie0_17"
#define IPFIX_CN_POSTPACKETDELTACOUNT         "ie0_18"
#define IPFIX_CN_MINIMUMIPTOTALLENGTH         "ie0_19"
#define IPFIX_CN_MAXIMUMIPTOTALLENGTH         "ie0_1a"
#define IPFIX_CN_SOURCEIPV6ADDRESS            "ie0_1b"
#define IPFIX_CN_DESTINATIONIPV6ADDRESS       "ie0_1c"
#define IPFIX_CN_SOURCEIPV6PREFIXLENGTH       "ie0_1d"
#define IPFIX_CN_DESTINATIONIPV6PREFIXLENGTH  "ie0_1e"
#define IPFIX_CN_FLOWLABELIPV6                "ie0_1f"
#define IPFIX_CN_ICMPTYPECODEIPV4             "ie0_20"
#define IPFIX_CN_IGMPTYPE                     "ie0_21"
#define IPFIX_CN_SAMPLING_INTERVAL            "ie0_22"
#define IPFIX_CN_SAMPLING_ALGORITHM           "ie0_23"
#define IPFIX_CN_FLOWACTIVETIMEOUT            "ie0_24"
#define IPFIX_CN_FLOWIDLETIMEOUT              "ie0_25"
#define IPFIX_CN_ENGINE_TYPE                  "ie0_26"
#define IPFIX_CN_ENGINE_ID                    "ie0_27"
#define IPFIX_CN_EXPORTEDOCTETTOTALCOUNT      "ie0_28"
#define IPFIX_CN_EXPORTEDMESSAGETOTALCOUNT    "ie0_29"
#define IPFIX_CN_EXPORTEDFLOWRECORDTOTALCOUNT "ie0_2a"
#define IPFIX_CN_SOURCEIPV4PREFIX             "ie0_2c"
#define IPFIX_CN_DESTINATIONIPV4PREFIX        "ie0_2d"
#define IPFIX_CN_MPLSTOPLABELTYPE             "ie0_2e"
#define IPFIX_CN_MPLSTOPLABELIPV4ADDRESS      "ie0_2f"
#define IPFIX_CN_FLOW_SAMPLER_ID              "ie0_30"
#define IPFIX_CN_FLOW_SAMPLER_MODE            "ie0_31"
#define IPFIX_CN_FLOW_SAMPLER_RANDOM_INTERVAL "ie0_32"
#define IPFIX_CN_MINIMUMTTL                   "ie0_34"
#define IPFIX_CN_MAXIMUMTTL                   "ie0_35"
#define IPFIX_CN_FRAGMENTIDENTIFICATION       "ie0_36"
#define IPFIX_CN_POSTIPCLASSOFSERVICE         "ie0_37"
#define IPFIX_CN_SOURCEMACADDRESS             "ie0_38"
#define IPFIX_CN_POSTDESTINATIONMACADDRESS    "ie0_39"
#define IPFIX_CN_VLANID                       "ie0_3a"
#define IPFIX_CN_POSTVLANID                   "ie0_3b"
#define IPFIX_CN_IPVERSION                    "ie0_3c"
#define IPFIX_CN_FLOWDIRECTION                "ie0_3d"
#define IPFIX_CN_IPNEXTHOPIPV6ADDRESS         "ie0_3e"
#define IPFIX_CN_BGPNEXTHOPIPV6ADDRESS        "ie0_3f"
#define IPFIX_CN_IPV6EXTENSIONHEADERS         "ie0_40"
#define IPFIX_CN_MPLSTOPLABELSTACKSECTION     "ie0_46"
#define IPFIX_CN_MPLSLABELSTACKSECTION2       "ie0_47"
#define IPFIX_CN_MPLSLABELSTACKSECTION3       "ie0_48"
#define IPFIX_CN_MPLSLABELSTACKSECTION4       "ie0_49"
#define IPFIX_CN_MPLSLABELSTACKSECTION5       "ie0_4a"
#define IPFIX_CN_MPLSLABELSTACKSECTION6       "ie0_4b"
#define IPFIX_CN_MPLSLABELSTACKSECTION7       "ie0_4c"
#define IPFIX_CN_MPLSLABELSTACKSECTION8       "ie0_4d"
#define IPFIX_CN_MPLSLABELSTACKSECTION9       "ie0_4e"
#define IPFIX_CN_MPLSLABELSTACKSECTION10      "ie0_4f"
#define IPFIX_CN_DESTINATIONMACADDRESS        "ie0_50"
#define IPFIX_CN_POSTSOURCEMACADDRESS         "ie0_51"
#define IPFIX_CN_OCTETTOTALCOUNT              "ie0_55"
#define IPFIX_CN_PACKETTOTALCOUNT             "ie0_56"
#define IPFIX_CN_FRAGMENTOFFSET               "ie0_58"
#define IPFIX_CN_MPLSVPNROUTEDISTINGUISHER    "ie0_5a"
#define IPFIX_CN_BGPNEXTADJACENTASNUMBER      "ie0_80"
#define IPFIX_CN_BGPPREVADJACENTASNUMBER      "ie0_81"
#define IPFIX_CN_EXPORTERIPV4ADDRESS          "ie0_82"
#define IPFIX_CN_EXPORTERIPV6ADDRESS          "ie0_83"
#define IPFIX_CN_DROPPEDOCTETDELTACOUNT       "ie0_84"
#define IPFIX_CN_DROPPEDPACKETDELTACOUNT      "ie0_85"
#define IPFIX_CN_DROPPEDOCTETTOTALCOUNT       "ie0_86"
#define IPFIX_CN_DROPPEDPACKETTOTALCOUNT      "ie0_87"
#define IPFIX_CN_FLOWENDREASON                "ie0_88"
#define IPFIX_CN_COMMONPROPERTIESID           "ie0_89"
#define IPFIX_CN_OBSERVATIONPOINTID           "ie0_8a"
#define IPFIX_CN_ICMPTYPECODEIPV6             "ie0_8b"
#define IPFIX_CN_MPLSTOPLABELIPV6ADDRESS      "ie0_8c"
#define IPFIX_CN_LINECARDID                   "ie0_8d"
#define IPFIX_CN_PORTID                       "ie0_8e"
#define IPFIX_CN_METERINGPROCESSID            "ie0_8f"
#define IPFIX_CN_EXPORTINGPROCESSID           "ie0_90"
#define IPFIX_CN_TEMPLATEID                   "ie0_91"
#define IPFIX_CN_WLANCHANNELID                "ie0_92"
#define IPFIX_CN_WLANSSID                     "ie0_93"
#define IPFIX_CN_FLOWID                       "ie0_94"
#define IPFIX_CN_OBSERVATIONDOMAINID          "ie0_95"
#define IPFIX_CN_FLOWSTARTSECONDS             "ie0_96"
#define IPFIX_CN_FLOWENDSECONDS               "ie0_97"
#define IPFIX_CN_FLOWSTARTMILLISECONDS        "ie0_98"
#define IPFIX_CN_FLOWENDMILLISECONDS          "ie0_99"
#define IPFIX_CN_FLOWSTARTMICROSECONDS        "ie0_9a"
#define IPFIX_CN_FLOWENDMICROSECONDS          "ie0_9b"
#define IPFIX_CN_FLOWSTARTNANOSECONDS         "ie0_9c"
#define IPFIX_CN_FLOWENDNANOSECONDS           "ie0_9d"
#define IPFIX_CN_FLOWSTARTDELTAMICROSECONDS   "ie0_9e"
#define IPFIX_CN_FLOWENDDELTAMICROSECONDS     "ie0_9f"
#define IPFIX_CN_SYSTEMINITTIMEMILLISECONDS   "ie0_a0"
#define IPFIX_CN_FLOWDURATIONMILLISECONDS     "ie0_a1"
#define IPFIX_CN_FLOWDURATIONMICROSECONDS     "ie0_a2"
#define IPFIX_CN_OBSERVEDFLOWTOTALCOUNT       "ie0_a3"
#define IPFIX_CN_IGNOREDPACKETTOTALCOUNT      "ie0_a4"
#define IPFIX_CN_IGNOREDOCTETTOTALCOUNT       "ie0_a5"
#define IPFIX_CN_NOTSENTFLOWTOTALCOUNT        "ie0_a6"
#define IPFIX_CN_NOTSENTPACKETTOTALCOUNT      "ie0_a7"
#define IPFIX_CN_NOTSENTOCTETTOTALCOUNT       "ie0_a8"
#define IPFIX_CN_DESTINATIONIPV6PREFIX        "ie0_a9"
#define IPFIX_CN_SOURCEIPV6PREFIX             "ie0_aa"
#define IPFIX_CN_POSTOCTETTOTALCOUNT          "ie0_ab"
#define IPFIX_CN_POSTPACKETTOTALCOUNT         "ie0_ac"
#define IPFIX_CN_FLOWKEYINDICATOR             "ie0_ad"
#define IPFIX_CN_POSTMCASTPACKETTOTALCOUNT    "ie0_ae"
#define IPFIX_CN_POSTMCASTOCTETTOTALCOUNT     "ie0_af"
#define IPFIX_CN_ICMPTYPEIPV4                 "ie0_b0"
#define IPFIX_CN_ICMPCODEIPV4                 "ie0_b1"
#define IPFIX_CN_ICMPTYPEIPV6                 "ie0_b2"
#define IPFIX_CN_ICMPCODEIPV6                 "ie0_b3"
#define IPFIX_CN_UDPSOURCEPORT                "ie0_b4"
#define IPFIX_CN_UDPDESTINATIONPORT           "ie0_b5"
#define IPFIX_CN_TCPSOURCEPORT                "ie0_b6"
#define IPFIX_CN_TCPDESTINATIONPORT           "ie0_b7"
#define IPFIX_CN_TCPSEQUENCENUMBER            "ie0_b8"
#define IPFIX_CN_TCPACKNOWLEDGEMENTNUMBER     "ie0_b9"
#define IPFIX_CN_TCPWINDOWSIZE                "ie0_ba"
#define IPFIX_CN_TCPURGENTPOINTER             "ie0_bb"
#define IPFIX_CN_TCPHEADERLENGTH              "ie0_bc"
#define IPFIX_CN_IPHEADERLENGTH               "ie0_bd"
#define IPFIX_CN_TOTALLENGTHIPV4              "ie0_be"
#define IPFIX_CN_PAYLOADLENGTHIPV6            "ie0_bf"
#define IPFIX_CN_IPTTL                        "ie0_c0"
#define IPFIX_CN_NEXTHEADERIPV6               "ie0_c1"
#define IPFIX_CN_MPLSPAYLOADLENGTH            "ie0_c2"
#define IPFIX_CN_IPDIFFSERVCODEPOINT          "ie0_c3"
#define IPFIX_CN_IPPRECEDENCE                 "ie0_c4"
#define IPFIX_CN_FRAGMENTFLAGS                "ie0_c5"
#define IPFIX_CN_OCTETDELTASUMOFSQUARES       "ie0_c6"
#define IPFIX_CN_OCTETTOTALSUMOFSQUARES       "ie0_c7"
#define IPFIX_CN_MPLSTOPLABELTTL              "ie0_c8"
#define IPFIX_CN_MPLSLABELSTACKLENGTH         "ie0_c9"
#define IPFIX_CN_MPLSLABELSTACKDEPTH          "ie0_ca"
#define IPFIX_CN_MPLSTOPLABELEXP              "ie0_cb"
#define IPFIX_CN_IPPAYLOADLENGTH              "ie0_cc"
#define IPFIX_CN_UDPMESSAGELENGTH             "ie0_cd"
#define IPFIX_CN_ISMULTICAST                  "ie0_ce"
#define IPFIX_CN_IPV4IHL                      "ie0_cf"
#define IPFIX_CN_IPV4OPTIONS                  "ie0_d0"
#define IPFIX_CN_TCPOPTIONS                   "ie0_d1"
#define IPFIX_CN_PADDINGOCTETS                "ie0_d2"
#define IPFIX_CN_COLLECTORIPV4ADDRESS         "ie0_d3"
#define IPFIX_CN_COLLECTORIPV6ADDRESS         "ie0_d4"
#define IPFIX_CN_COLLECTORINTERFACE           "ie0_d5"
#define IPFIX_CN_COLLECTORPROTOCOLVERSION     "ie0_d6"
#define IPFIX_CN_COLLECTORTRANSPORTPROTOCOL   "ie0_d7"
#define IPFIX_CN_COLLECTORTRANSPORTPORT       "ie0_d8"
#define IPFIX_CN_EXPORTERTRANSPORTPORT        "ie0_d9"
#define IPFIX_CN_TCPSYNTOTALCOUNT             "ie0_da"
#define IPFIX_CN_TCPFINTOTALCOUNT             "ie0_db"
#define IPFIX_CN_TCPRSTTOTALCOUNT             "ie0_dc"
#define IPFIX_CN_TCPPSHTOTALCOUNT             "ie0_dd"
#define IPFIX_CN_TCPACKTOTALCOUNT             "ie0_de"
#define IPFIX_CN_TCPURGTOTALCOUNT             "ie0_df"
#define IPFIX_CN_IPTOTALLENGTH                "ie0_e0"
#define IPFIX_CN_POSTMPLSTOPLABELEXP          "ie0_ed"
#define IPFIX_CN_TCPWINDOWSCALE               "ie0_ee"
#define IPFIX_CN_OBSERVATIONPOINTID_PSAMP     "ie0_12c"
#define IPFIX_CN_SELECTIONSEQUENCEID          "ie0_12d"
#define IPFIX_CN_SELECTORID                   "ie0_12e"
#define IPFIX_CN_INFORMATIONELEMENTID         "ie0_12f"
#define IPFIX_CN_SELECTORALGORITHM            "ie0_130"
#define IPFIX_CN_SAMPLINGPACKETINTERVAL       "ie0_131"
#define IPFIX_CN_SAMPLINGPACKETSPACE          "ie0_132"
#define IPFIX_CN_SAMPLINGTIMEINTERVAL         "ie0_133"
#define IPFIX_CN_SAMPLINGTIMESPACE            "ie0_134"
#define IPFIX_CN_SAMPLINGSIZE                 "ie0_135"
#define IPFIX_CN_SAMPLINGPOPULATION           "ie0_136"
#define IPFIX_CN_SAMPLINGPROBABILITY          "ie0_137"
#define IPFIX_CN_DATALINKFRAMESIZE            "ie0_138"
#define IPFIX_CN_IPHEADERPACKETSECTION        "ie0_139"
#define IPFIX_CN_IPPAYLOADPACKETSECTION       "ie0_13a"
#define IPFIX_CN_DATALINKFRAMESECTION         "ie0_13b"
#define IPFIX_CN_MPLSLABELSTACKSECTION        "ie0_13c"
#define IPFIX_CN_MPLSPAYLOADPACKETSECTION     "ie0_13d"
#define IPFIX_CN_PACKETSOBSERVED              "ie0_13e"
#define IPFIX_CN_PACKETSSELECTED              "ie0_13f"
#define IPFIX_CN_FIXEDERROR                   "ie0_140"
#define IPFIX_CN_RELATIVEERROR                "ie0_141"
#define IPFIX_CN_OBSERVATIONTIMESECONDS       "ie0_142"
#define IPFIX_CN_OBSERVATIONTIMEMILLISECONDS  "ie0_143"
#define IPFIX_CN_OBSERVATIONTIMEMICROSECONDS  "ie0_144"
#define IPFIX_CN_OBSERVATIONTIMENANOSECONDS   "ie0_145"
#define IPFIX_CN_DIGESTHASHVALUE              "ie0_146"
#define IPFIX_CN_HASHIPPAYLOADOFFSET          "ie0_147"
#define IPFIX_CN_HASHIPPAYLOADSIZE            "ie0_148"
#define IPFIX_CN_HASHOUTPUTRANGEMIN           "ie0_149"
#define IPFIX_CN_HASHOUTPUTRANGEMAX           "ie0_14a"
#define IPFIX_CN_HASHSELECTEDRANGEMIN         "ie0_14b"
#define IPFIX_CN_HASHSELECTEDRANGEMAX         "ie0_14c"
#define IPFIX_CN_HASHDIGESTOUTPUT             "ie0_14d"
#define IPFIX_CN_HASHINITIALISERVALUE         "ie0_14e"

#endif

