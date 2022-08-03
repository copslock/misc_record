1、检查原理图，把每个频段的发送，接收通道都整理清楚形成表格。

     a) 一般每个频段有一个发送通路，两个接受通路（4G要求的），主天线通路和分集天线通路。rx0, rx1, tx0，主天线通路默认是rx0。

     b) 有的频段没有分集天线通路，如tdscmda B34

     c)  有的接收回路几个频段共用一条线：G900/B8, G850/B5, G1900/B2, G1800/B3, B1/B4,B38/B41

 

2、在rf_card的配置里找，看有没有和我们硬件配置相似的，比如modem_proc/rfc_dime/rf_card/rfc_wtr1625_apac，如果没有相似的，就重新建一个rf_card配置文件：modem_proc/rfc_dime/rf_card/rfc_wtr1625_bacon。整体复制rfc_wtr1625_apac，然后改名，然后把里面的宏，函数名都改成wtr1625_bacon。

改动编译文件scons

在文件modem_proc/rfc_dime/build/modem_rfc_dime.scons里加入include目录

env.PublishProtectedApi('RFC_RF_CARD_INC', [

    '${RFC_ROOT}/rf_card/rfc_wtr1625_vz_sv2/common/inc',

   '${RFC_ROOT}/rf_card/rfc_wtr1625_bacon/common/inc'

    ])

编译软件，看看编译是否通过？编译通过后，进行下一步。

 

3、加入新的开关器件

  a) 在modem_proc/rfdevice_asm/src目录下，复制一个类似的器件，然后改动。

  b) 

  修改modem_proc/rfdevice_asm/src/rfcdevice_asm_factory_ag.cpp，并增加新的ASM驱动

  else if ( mfg_id ==  0x01A5 && prd_id == 0x5F  && prd_rev == 0)

  {

    asm_data =rfdevice_asm_sky13488_data_ag::get_instance();

  }

编译通过后，进行下一步。

 

4、加入新的PA器件

  a) 在目录modem_proc/rfdevice_pa/src，复制一个类似的器件，然后改动。

  b) 

修改modem_proc/rfdevice_pa/src/rfdevice_pa_factory_ag.cpp，并增加新的PA驱动

 

  else if ( mfg_id ==  0x134 && prd_id == 0x30  && prd_rev == 0)

  {

    pa_data =rfdevice_pa_rfmd_7459_data_ag::get_instance();

  }

 

编译通过后，进行下一步。

 

5、在rf_card里加上器件，ASM，PA等MIPI接口的器件。这些器件在modem软件启动时会扫描的。

如果用了不是MIPI接口的器件，不用在这里加

modem_proc/rfc_dime/rf_card/rfc_wtr1625_bacon/common/src/rfc_wtr1625_bacon_cmn_ag.cpp

rfc_device_cfg_info_typerfc_wtr1625_bacon_cmn_devices_list[] =

{

  { /*Device: WTR1625 */

    RFDEVICE_TRANSCEIVER, /* DEVICE_TYPE */

    WTR1625, /* DEVICE_NAME */

    0, /* DEVICE_TYPE_INSTANCE */

    RFDEVICE_COMM_PROTO_SSBI, /*DEVICE_COMM_PROTOCOL */

    {    RFC_DEVICE_COMM_BUS_SSBI_1,RFC_DEVICE_COMM_BUS_SSBI_2,},/* DEVICE_COMM_BUS */

    0 /*Warning: Not specified*/,/* MANUFACTURER_ID */

    0 /*Warning: Not specified*/, /* PRODUCT_ID */

    0 /*Warning: Not specified*/,/* PRODUCT_REV */

    0 /*Warning: Not specified*/,/* DEFAULT USID RANGE START */

    0 /*Warning: Not specified*/,/* DEFAULT USID RANGE END */

    0 /*Warning: Not specified*/,/* ASSIGNED_USID */

    0 /*Warning: Not specified*/,/* RFFE_GROUP_ID */

    FALSE, /* INIT */

    RFC_TX_MODEM_CHAIN_0, /*ASSOCIATED_DAC */

    RFDEVICE_TYPE_INVALID, /*ASSOCIATED_DEVICE_TYPE */

    0 /*Warning: Not specified*/,/* ASSOCIATED_DEVICE_TYPE_INSTANCE */

  }, /* END - Device: WTR1625 */

 

  { /*Device: QFE1101 */ PAPower Manager，用了管理PA的功率，可以减少电流消耗。发射通道需要

    RFDEVICE_PAPM, /* DEVICE_TYPE */

    QFE1101, /* DEVICE_NAME */

    0, /* DEVICE_TYPE_INSTANCE */

    RFDEVICE_COMM_PROTO_RFFE, /*DEVICE_COMM_PROTOCOL */

    {    0,0 /* 0 not specified */,}, /*DEVICE_COMM_BUS */

    0x0217, /* MANUFACTURER_ID */

    0x30, /* PRODUCT_ID */

    0, /* PRODUCT_REV */

    0x4, /* DEFAULT USID RANGESTART */

    0x4, /* DEFAULT USID RANGE END */

    0x4, /* ASSIGNED_USID */

    0x4, /* RFFE_GROUP_ID */

    TRUE, /* INIT */

    RFC_INVALID_PARAM, /*ASSOCIATED_DAC */

    RFDEVICE_TYPE_INVALID, /*ASSOCIATED_DEVICE_TYPE */

    0 /*Warning: Not specified*/,/* ASSOCIATED_DEVICE_TYPE_INSTANCE */

  }, /* END - Device: QFE1100 */

 

  {   /*Device: QFE1550 */  Antanna Tuner，天线调谐器，接收通道用的

    RFDEVICE_TUNER, /* DEVICE_TYPE */

    QFE1550, /* DEVICE_NAME */

    0, /* DEVICE_TYPE_INSTANCE */

    RFDEVICE_COMM_PROTO_RFFE, /*DEVICE_COMM_PROTOCOL */

    {    RFC_DEVICE_COMM_BUS_SSBI_1,0 /*0 not specified */,}, /* DEVICE_COMM_BUS */

    0x0217, /* MANUFACTURER_ID */

    0x02, /* PRODUCT_ID */

    0, /* PRODUCT_REV */

    0x7, /* DEFAULT USID RANGESTART */

    0x7, /* DEFAULT USID RANGE END */

    0x7, /* ASSIGNED_USID */

    0x7, /* RFFE_GROUP_ID */

    FALSE, /* INIT */

    RFC_INVALID_PARAM, /*ASSOCIATED_DAC */

    RFDEVICE_TYPE_INVALID, /*ASSOCIATED_DEVICE_TYPE */

    0 /*Warning: Not specified*/,/* ASSOCIATED_DEVICE_TYPE_INSTANCE */

  }, /* END - Device: QFE1550 */

 

  {   /*Device: RFMD1500 */   开关器件

    RFDEVICE_ASM, /* DEVICE_TYPE */

    GEN_ASM /* ASM_16T */, /* DEVICE_NAME */

    0, /* DEVICE_TYPE_INSTANCE */

    RFDEVICE_COMM_PROTO_RFFE, /*DEVICE_COMM_PROTOCOL */

    {    RFC_DEVICE_COMM_BUS_SSBI_1,0 /*0 not specified */,}, /* DEVICE_COMM_BUS */

    0x0134, /* MANUFACTURER_ID */

    0x5D, /* PRODUCT_ID */

    0, /* PRODUCT_REV */

    0xB, /* DEFAULT USID RANGESTART */

    0xB, /* DEFAULT USID RANGE END */

    0xB, /* ASSIGNED_USID */

    0xB, /* RFFE_GROUP_ID */

    FALSE, /* INIT */

    RFC_INVALID_PARAM, /*ASSOCIATED_DAC */

    RFDEVICE_TYPE_INVALID, /*ASSOCIATED_DEVICE_TYPE */

    0 /*Warning: Not specified*/,/* ASSOCIATED_DEVICE_TYPE_INSTANCE */

}, /* END - Device: ASM_16T */

 

{   /*Device: RFMD1660 */  开关器件

    RFDEVICE_ASM, /* DEVICE_TYPE */

    GEN_ASM /* ASM_10T */, /* DEVICE_NAME */

    1, /* DEVICE_TYPE_INSTANCE */

    RFDEVICE_COMM_PROTO_RFFE, /*DEVICE_COMM_PROTOCOL */

    {   RFC_DEVICE_COMM_BUS_SSBI_1,0/* 0 not specified */,}, /* DEVICE_COMM_BUS */

    0x0134, /* MANUFACTURER_ID */

    0x78, /* PRODUCT_ID */

    0, /* PRODUCT_REV */

    0xA, /* DEFAULT USID RANGESTART */

    0xA, /* DEFAULT USID RANGE END*/

    0xA, /* ASSIGNED_USID */

    0xA, /* RFFE_GROUP_ID */

    FALSE, /* INIT */

    RFC_INVALID_PARAM, /*ASSOCIATED_DAC */

    RFDEVICE_TYPE_INVALID, /*ASSOCIATED_DEVICE_TYPE */

    0 /*Warning: Not specified*/,/* ASSOCIATED_DEVICE_TYPE_INSTANCE */

}, /* END - Device: ASM_10T */

 

  {   /*Device: HCPA,RF7459, Same asRF7389 */

    RFDEVICE_PA, /* DEVICE_TYPE */ PA器件

    GEN_PA /* HCPA */, /*DEVICE_NAME */

    0, /* DEVICE_TYPE_INSTANCE */

    RFDEVICE_COMM_PROTO_RFFE, /*DEVICE_COMM_PROTOCOL */

    {   0,0 /* 0 not specified */,},/* DEVICE_COMM_BUS */

    0x0134, /* MANUFACTURER_ID */

    0x30,

    0, /* PRODUCT_REV */

    0xF, /* DEFAULT USID RANGESTART */

    0xF, /* DEFAULT USID RANGE END*/

    0xF, /* ASSIGNED_USID */

    0xF, /* RFFE_GROUP_ID */

    FALSE, /* INIT */

    RFC_INVALID_PARAM, /*ASSOCIATED_DAC */

    RFDEVICE_TYPE_INVALID, /*ASSOCIATED_DEVICE_TYPE */

    0 /*Warning: Not specified*/,/* ASSOCIATED_DEVICE_TYPE_INSTANCE */

  }, /* END - Device: HCPA */

 

  { /*Device: TRX_HDET */ 这个设备没有具体的硬件器件，它是通过某种电路来实现的，需要在软件里加上它。

    RFDEVICE_HDET, /* DEVICE_TYPE */

    TRX_HDET, /* DEVICE_NAME */

    0, /* DEVICE_TYPE_INSTANCE */

    RFDEVICE_COMM_PROTO_SSBI, /*DEVICE_COMM_PROTOCOL */

    {    RFC_DEVICE_COMM_BUS_SSBI_1,0 /*0 not specified */,}, /* DEVICE_COMM_BUS */

    0 /*Warning: Not specified*/,/* MANUFACTURER_ID */

    0 /*Warning: Not specified*/, /* PRODUCT_ID */

    0 /*Warning: Not specified*/,/* PRODUCT_REV */

    0 /*Warning: Not specified*/,/* DEFAULT USID RANGE START */

    0 /*Warning: Not specified*/,/* DEFAULT USID RANGE END */

    0 /*Warning: Not specified*/,/* ASSIGNED_USID */

    0 /*Warning: Not specified*/,/* RFFE_GROUP_ID */

    FALSE, /* INIT */

    RFC_INVALID_PARAM, /*ASSOCIATED_DAC */

    RFDEVICE_TRANSCEIVER, /*ASSOCIATED_DEVICE_TYPE */

    0, /* ASSOCIATED_DEVICE_TYPE_INSTANCE */

  }, /* END - Device: TRX_HDET*/

 

  { /*Device: RFDEVICE_INVALID*/

    RFDEVICE_TYPE_INVALID, /* DEVICE_TYPE */

    RFDEVICE_INVALID, /* DEVICE_NAME */

    0 /*Warning: Not specified*/,/* DEVICE_TYPE_INSTANCE */

    RFDEVICE_COMM_PROTO_INVALID,/* DEVICE_COMM_PROTOCOL */

    {    0 /* 0 not specified */,0 /* 0not specified */,}, /* DEVICE_COMM_BUS */

    0 /*Warning: Not specified*/,/* MANUFACTURER_ID */

    0 /*Warning: Not specified*/, /* PRODUCT_ID */

    0 /*Warning: Not specified*/,/* PRODUCT_REV */

    0 /*Warning: Not specified*/,/* DEFAULT USID RANGE START */

    0 /*Warning: Not specified*/,/* DEFAULT USID RANGE END */

    0 /*Warning: Not specified*/,/* ASSIGNED_USID */

    0 /*Warning: Not specified*/,/* RFFE_GROUP_ID */

    FALSE, /* INIT */

    RFC_INVALID_PARAM, /*ASSOCIATED_DAC */

    RFDEVICE_TYPE_INVALID, /*ASSOCIATED_DEVICE_TYPE */

 

    0 /*Warning: Not specified*/,/* ASSOCIATED_DEVICE_TYPE_INSTANCE */

  }, /* END - Device:RFDEVICE_INVALID */

 

};

 

6、根据硬件原理图，来配置每一条软件通路

在目录modem_proc/rfc_dime/rf_card/rfc_wtr1625_bacon下，/gsm, /lte, /tdscdma, wcdma每一条通道都要配置正确。其中QFE1101的硬件手册里没有发现是如何配置Port的，见附件QFE1101.png，里面有Port表。

 

7、配置RX通道，rx0,rx1 

modem_proc/rfc_dime/rf_card/rfc_wtr1625_bacon/common/src/rfc_wtr1625_bacon_cmn_ag.cpp

rfm_devices_configuration_typerfc_wtr1625_bacon_logical_device_properties =

{

    {   /* Mode Concurrencies: Bit0represents Rx concurrency and Bit1 represents Tx concurrency */

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

        { 0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,    },

    },

    {   /* device_support */

        {   /* Logical Device 0 */   //rx0 通道配置，支持哪些频段

            /* bands_supported */

            {

                {

                    /* Bit mask element 0 */

                    (( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 <<(SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 -0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1<< (SYS_BAND_BC0 - 0) ) | ( (uint64)1 << (SYS_BAND_BC1 - 0) )

                    ),  /* End Bit mask element 0 */

                    /* Bit mask element 1 */

                    (

                        //( (uint64)1 <<(SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 <<(SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 <<(SYS_BAND_WCDMA_IV_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 -64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 <<(SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 <<(SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) | ( (uint64)1 <<(SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8- 64) )

             ( (uint64)1<< (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_III_1700 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_IV_1700 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_V_850 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_VIII_900 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND8 - 64) )

                    ),  /* End Bit mask element 1 */

                    /* Bit mask element 2 */

                    (

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) |

             ( (uint64)1<< (SYS_BAND_TDS_BANDA - 128) ) |

             ( (uint64)1<< (SYS_BAND_TDS_BANDF - 128) )

                    ),  /* End Bit mask element 2 */

 

                },

            },

            /* ca1_bands_supported */

            {

                {

                    /* Bit mask element 0 */

                    (

                        0

                    ),  /* End Bit mask element 0 */

                    /* Bit mask element 1 */

                    (

                        0

                    ),  /* End Bit mask element 1 */

                    /* Bit mask element 2 */

                    (

                        0

                    ),  /* End Bit mask element 2 */

                },

            },

            ( RFM_DEVICE_RX_SUPPORTED |RFM_DEVICE_TX_SUPPORTED),

            RFM_DEVICE_1, /* AssociatedSlave Device for RFM_DEVICE_0 */

        }, /* End Logical Device 0 */

        {   /* Logical Device 1 */  //rx1 通道配置，支持哪些频段

            /* bands_supported */

            {

                {

                    /* Bit mask element 0 */

                    (

                        0

                    ),  /* End Bit mask element 0 */

                    /* Bit mask element 1 */

                    (

                        //( (uint64)1 <<(SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 <<(SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 <<(SYS_BAND_WCDMA_IV_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 -64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 <<(SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 <<(SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) | ( (uint64)1 <<(SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 <<(SYS_BAND_LTE_EUTRAN_BAND8 - 64) )

             ( (uint64)1<< (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_III_1700 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_IV_1700 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_V_850 - 64) ) |

             ( (uint64)1<< (SYS_BAND_WCDMA_VIII_900 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND8 - 64) )

                    ),  /* End Bit mask element 1 */

                    /* Bit mask element 2 */

                    (

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) |

             ( (uint64)1<< (SYS_BAND_LTE_EUTRAN_BAND41 - 128) )

                    ),  /* End Bit mask element 2 */

                },

            },

            /* ca1_bands_supported */

            {

                {

                    /* Bit mask element 0 */

                    (

                        0

                    ),  /* End Bit mask element 0 */

                    /* Bit mask element 1 */

                    (

                        0

                    ),  /* End Bit mask element 1 */

                    /* Bit mask element 2 */

                    (

                        0

                    ),  /* End Bit mask element 2 */

                },

            },

            ( RFM_DEVICE_RX_SUPPORTED),

            RFM_INVALID_DEVICE, /*Associated Slave Device for RFM_DEVICE_1 */

        }, /* End Logical Device 1 */

 

 

8、生成QCN

  a)  在目录modem_proc/rftarget_dime/common/qcn，找一个相近的，例如wtr1625_apac，来生成QCN，删除里面不存在的频段，加入有的频段。使之和硬件对应。

  b)  根据硬件修改MSM8974_WTR1625_APAC_MASTERFILE.xml里的这些NV项

 <NvItem id="572"name="NV_RF_NV_LOADED_DATE_I" mapping="direct"encoding="dec">20140414</NvItem>

 <NvItem id="1878"name="NV_RF_HW_CONFIG_I" mapping="direct"encoding="dec">46</NvItem>

 <NvItem id="1877"name="NV_RF_BC_CONFIG_I" mapping="direct"encoding="dec">1154610354700436871</NvItem>

 <NvItem id="4548"name="NV_RF_BC_CONFIG_DIV_I" mapping="direct"encoding="dec">1154610354700436871</NvItem>

 <NvItem id="6828"name="NV_LTE_BC_CONFIG_I" mapping="direct"encoding="dec">2070360228343,0</NvItem>

 <NvItem id="22131"name="RFNV_RF_BC_CONFIG_C2_I" mapping="direct"encoding="dec">0</NvItem>

 <NvItem id="23387"name="RFNV_RF_BC_CONFIG_C3_I" mapping="direct"encoding="dec">0</NvItem>

 <NvItem id="22605"name="RFNV_TDSCDMA_BC_CONFIG_I" mapping="direct"encoding="dec">49</NvItem>

 <NvItem id="24203"name="RFNV_TDSCDMA_BC_CONFIG_DIV_I" mapping="direct"encoding="dec">49</NvItem>

  c) 用QDART来生成QCN

 

9、调试

  a)  MIPI器件可以用QDART来做通路测试

  b)  连接仪器来做测试，如果不通，通过硬件来量信号，看是什么地方不通。

 

根据电路配置来说明软件实现

rfc_device_info_typerf_card_wtr1625_bacon_rx0_lte_b1_device_info =

{

    0x022902A2,  /* Revision: v2.41.674 */

   RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */

    0 /*Warning: Not Specified */,   /* ET Modem Chain */

    0,   /*NV Container */

    0 /*Warning: Not Specified */,   /* Antenna */

    2,  /*NUM_DEVICES_TO_CONFIGURE */

    {

       {

           RFDEVICE_TRANSCEIVER,

           WTR1625,  /* NAME */

           0,  /* DEVICE_TYPE_INSTANCE */

           0,  /* PHY_PATH_NUM */

           {

               0 /*Warning: Not specified*/,  /*INTF_REV */

               (int)WTR1625_LTEFDD_PRX_BAND1_PMB4,  /*PORT */  W1625上的控制端口，看硬件原理图

               ( RFDEVICE_PA_LUT_MAPPING_INVALID ), /* RF_ASIC_BAND_PA_LUT_MAP */

               FALSE,  /* TXAGC_LUT */

               0,  /* Array Filler */

               0,  /* Array Filler */

           },

       },

       {

           RFDEVICE_ASM,

           GEN_ASM /* ASM_16T */,  /* NAME */

           0,  /* DEVICE_TYPE_INSTANCE */

           0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */

           {

               0  /* Orig setting:  */,  /*INTF_REV */

               (0x0134 << 22)/*mfg_id*/ | (0x5D<< 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */  器件选通通道

               0,

               0, /* Array Filler */

               0, /* Array Filler */

               0, /* Array Filler */

           },

       },

    },

};

 

 

rfc_sig_info_typerf_card_wtr1625_bacon_rx0_lte_b1_sig_cfg =

{

    0x022902A2,  /* Revision: v2.41.674 */

    {

       { (int)RFC_SIG_LIST_END,  RFC_LOW  }

    },

};

 

 

rfc_device_info_typerf_card_wtr1625_bacon_rx1_lte_b1_device_info =

{

    0x022902A2,  /* Revision: v2.41.674 */

   RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */

    0 /* Warning:Not Specified */,   /* ET Modem Chain */

    1,   /*NV Container */

    1 /*Warning: Not Specified */,   /* Antenna */

    3,  /*NUM_DEVICES_TO_CONFIGURE */

    {

       {

           RFDEVICE_TRANSCEIVER,

           WTR1625,  /* NAME */

           0,  /* DEVICE_TYPE_INSTANCE */

           1,  /* PHY_PATH_NUM */

           {

               0 /*Warning: Not specified*/,  /*INTF_REV */

               (int)WTR1625_LTEFDD_DRX_BAND1_DMB1,  /*PORT */

               ( RFDEVICE_PA_LUT_MAPPING_INVALID ), /* RF_ASIC_BAND_PA_LUT_MAP */

               FALSE,  /* TXAGC_LUT */

               0,  /* Array Filler */

               0,  /* Array Filler */

           },

       },

       {

           RFDEVICE_ASM,

           GEN_ASM /* ASM_10T */,  /* NAME */

           1,  /* DEVICE_TYPE_INSTANCE */

           0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */

           {

               0  /* Orig setting:  */,  /*INTF_REV */

               (0x0134 << 22)/*mfg_id*/ | (0x78<< 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

           },

       },

       {

           RFDEVICE_TUNER,    从原理图上看，所有的分集天线都通过了Tuner，所以要把这个器件加上

           QFE1550, /* NAME */

           0,  /* DEVICE_TYPE_INSTANCE */

           0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */

           {

               0  /* Orig setting:  */,  /*INTF_REV */

               0, /* Array Filler */

               0, /* Array Filler */

               0, /* Array Filler */

               0, /* Array Filler */

               0, /* Array Filler */

           },

       },

    },

};

 

 

rfc_sig_info_typerf_card_wtr1625_bacon_rx1_lte_b1_sig_cfg =

{

    0x022902A2,  /* Revision: v2.41.674 */

    {

b1 rx1通过了器件XM0830SJ-DL0612来选通b1或b4，这个器件是通过GPIO120来控制的

       { (int)RFC_WTR1625_BACON_RF_PATH_SEL_05, RFC_HIGH  }, /* GPIO_120*/

 

       { (int)RFC_SIG_LIST_END,  RFC_LOW  }

    },

};

 

 

rfc_device_info_typerf_card_wtr1625_bacon_tx0_lte_b1_device_info =

{

    0x022902A2,  /* Revision: v2.41.674 */

   RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */

   RFC_TX_MODEM_CHAIN_1,   /* ET Modem Chain */

    0,   /*NV Container */

    0 /*Warning: Not Specified */,   /* Antenna */

    5,  /*NUM_DEVICES_TO_CONFIGURE */

    {

       {

           RFDEVICE_TRANSCEIVER,

           WTR1625,  /* NAME */

           0,  /* DEVICE_TYPE_INSTANCE */

           0,  /* PHY_PATH_NUM */

           {

               0 /*Warning: Not specified*/,  /*INTF_REV */

               (int)WTR1625_LTEFDD_TX_BAND1_TMB3,  /*PORT */

               (RFDEVICE_PA_LUT_MAPPING_VALID | WTR1625_HP_LUT_TYPE <<RFDEVICE_PA_STATE_0_BSHFT | WTR1625_HP_LUT_TYPE <<RFDEVICE_PA_STATE_1_BSHFT | WTR1625_HP_LUT_TYPE <<RFDEVICE_PA_STATE_2_BSHFT | WTR1625_LP_LUT_TYPE <<RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_PA_LUT_MAP */  Loop 表之类的，抄的，不需要改的

               FALSE,  /* TXAGC_LUT */

               0,  /* Array Filler */

               0,  /* Array Filler */

           },

       },

       {

           RFDEVICE_PA,

           GEN_PA,  /* NAME */

           0,  /* DEVICE_TYPE_INSTANCE */

           0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */

           {

               0  /* Orig setting:  */,  /*INTF_REV */

               (0x0134 << 22)/*mfg_id*/ | (0x30<< 14)/*prd_id*/ | (22)/*port_num*/, /* PORT_NUM */

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

           },

       },

       { 发射通道需要加上PA Power Manager器件来控制发射功率

           RFDEVICE_PAPM,

           QFE1101,  /* NAME */

           0,  /* DEVICE_TYPE_INSTANCE */

           0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */

           {

               0  /* Orig setting:  */,  /*INTF_REV */

               (0x217 <<22)/*mfg_id*/ | (0x30 << 14)/*prd_id*/ | (9)/*port_num*/,  /*PORT_NUM */ 这个没有具体的资料，也是从原来的Code里抄的

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

           },

       },

       {

           RFDEVICE_ASM,

           GEN_ASM /* ASM_16T */, /* NAME */

           0, /* DEVICE_TYPE_INSTANCE */

           0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */

           {

               0  /* Orig setting:  */,  /*INTF_REV */

               (0x0134 << 22)/*mfg_id*/ | (0x5D<< 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */

               0,

               0, /* Array Filler */

               0, /* Array Filler */

               0, /* Array Filler */

           },

       },

       {   没有实际存在的器件对应，可能通过电路来实现，需要加上

           RFDEVICE_HDET,

           TRX_HDET,  /* NAME */

           0,  /* DEVICE_TYPE_INSTANCE */

           0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */

           {

               0  /* Orig setting:  */,  /*INTF_REV */

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

               0,  /* Array Filler */

           },

       },

    },

};

 

 

rfc_sig_info_typerf_card_wtr1625_bacon_tx0_lte_b1_sig_cfg =

{

    0x022902A2,  /* Revision: v2.41.674 */

    {

       { (int)RFC_WTR1625_BACON_TX_GTR_TH, RFC_CONFIG_ONLY },  这两项不加也可以，加在这里是从原有文件里带的

       { (int)RFC_WTR1625_BACON_PA_IND, RFC_CONFIG_ONLY  },

       { (int)RFC_SIG_LIST_END,  RFC_LOW  }

    },

};

 

    {

     RFDEVICE_PA,

     GEN_PA /* HCPA */,  /* NAME */

      0, /* DEVICE_TYPE_INSTANCE */

      0/*Warning: Not specified*/,  /* PHY_PATH_NUM */

      {

       0  /* Orig setting:  */,  /* INTF_REV */

       (0x01A5 << 22)/*mfg_id*/ | (0x09 << 14)/*prd_id*/ | (13 << 7)/*sec_port_num*/ | (9)/*port_num*/, /* PORT_NUM */  这个sec_port_num也是从原来code里抄的，应该不需要

       0,  /* Array Filler */

       0,  /* Array Filler */

       0,  /* Array Filler */

       0,  /* Array Filler */

      },

    },
