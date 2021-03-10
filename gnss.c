#gps/gnss/mgp/inc/mgp_api.h
/*
******************************************************************************
* mgp_GnssGetConstellationConfig
*
* Function description:
*
*  Reads NV item which describes the enabled constellations and SBAS control information.
*
* Parameters:
*  p_GnssCfg- pointer to GNSS Configuration constellations information
*                   Bit 0  Controls GPS, and has to be set to 1 always
*			Bit 1  Controls Glonass.
*			Bit 2  Controls Beidou outside of USA.
*			Bit 3  Controls Galileo outside of USA.
*			Bit 4  Controls Beidou worldwide.
*			Bit 5  Controls Galileo worldwide.
*			Bit 6  Reserved for future use, and has to be set to 0 always.
*			Bit 7  Reserved for HBW
*			Bits 8..31  Reserved, and has to be set to 0 always.
*
*  p_SbasCfg- pointer to SBAS control information
*			Value 0  SBAS is disabled
*			Value 1  SBAS is enabled
*			All other values  Reserved
*
* Return value:
*  TRUE in case both values are read otherwise FALSE incase the input pointers are corrupted
*
********************************************************************************
*/
boolean mgp_GnssGetConstellationConfig(uint32 *p_GnssCfg, uint32 *p_SbasCfg);


/*
******************************************************************************
* mgp_GetActiveGnssRcvrConfiguration
*
* Function description:
*
*  This API returns the current GNSS receiver configuration.
*
*  Active Receiver GNSS configuration bit mask.
*  #define C_RCVR_GNSS_CONFIG_GPS_ENABLED                (1<<0)
*  #define C_RCVR_GNSS_CONFIG_GLO_ENABLED                (1<<1)
*  #define C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE      (1<<4)
*  #define C_RCVR_GNSS_CONFIG_BDS_ENABLED (C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE)
*
* Parameters:
*  Pointer to configuration mask.
* 
* Return value:
*  None.
*
******************************************************************************
*/
void mgp_GetActiveGnssRcvrConfiguration(uint8 *pu_CfgMask);



#include<stdio.h>
#define C_RCVR_GNSS_CONFIG_GPS_ENABLED                (1<<0)
#define C_RCVR_GNSS_CONFIG_GLO_ENABLED                (1<<1)
#define C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE      (1<<4)
#define C_RCVR_GNSS_CONFIG_BDS_ENABLED (C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE)

int main() {
    printf("C_RCVR_GNSS_CONFIG_GPS_ENABLED  = %u\n", C_RCVR_GNSS_CONFIG_GPS_ENABLED);
    printf("C_RCVR_GNSS_CONFIG_GLO_ENABLED  = %u\n", C_RCVR_GNSS_CONFIG_GLO_ENABLED);
    printf("C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE  = %u\n", C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE);
    printf("C_RCVR_GNSS_CONFIG_BDS_ENABLED  = %u\n", C_RCVR_GNSS_CONFIG_BDS_ENABLED);
}

C_RCVR_GNSS_CONFIG_GPS_ENABLED  = 1
C_RCVR_GNSS_CONFIG_GLO_ENABLED  = 2
C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE  = 16
C_RCVR_GNSS_CONFIG_BDS_ENABLED  = 16




Which diag task called the mgp_NavConfig API and write the p_NavConfig pointer

#NAV_CONFIG DIAG COMMAND HANDLER(DIAG → AMSS) 
/*===========================================================================
FUNCTION tm_diag_nav_config_cmd_handler
DESCRIPTION
  This function is used to configure the Nav Core, which will then
  configure Nav RF as needed and notify upper layers like MC.
DEPENDENCIES
  None.
RETURN VALUE
  Pointer to response packet.
SIDE EFFECTS
  None.
===========================================================================*/
PACKED void* tm_diag_nav_config_cmd_handler(PACKED void* p_PktReq, 
                                            uint16 w_PktLength)
{
  gpsdiag_NavConfigReqType* p_Req = (gpsdiag_NavConfigReqType*) p_PktReq;
  gpsdiag_NavConfigRspType* p_Rsp = NULL;
  mgp_NavConfigType z_NavConfig;

  /* Function parameters Sanity Check */
  if(p_Req == NULL)
  {
    /* Return a diag error packet indicating bad parameters were passed */
    return(diagpkt_err_rsp(DIAG_BAD_PARM_F, p_PktReq, w_PktLength));
  }
  
  /* Allocate space through diag for response packet */
  p_Rsp = (gpsdiag_NavConfigRspType*)
          diagpkt_subsys_alloc_v2( (diagpkt_subsys_id_type)DIAG_SUBSYS_GPS,
                                   TM_DIAG_NAV_CONFIG_CMD,
                                   sizeof(gpsdiag_NavConfigRspType) );

  /* Fill in response packet */
  if(p_Rsp == NULL)
  {
    MSG_ERROR("tm_diag_nav_config_cmd_handler: Rsp packet is NULL!!", 0, 0, 0);
  }
  else
  {
    /* Set the status in the response, 0 => Success */
    diagpkt_subsys_set_status(&(p_Rsp->z_DiagCmdHdrV2), 0);
 
    /* Set the first (0) response count */
    diagpkt_subsys_set_rsp_cnt(&(p_Rsp->z_DiagCmdHdrV2), 0);

    /* Nothing more to follow */
    diagpkt_subsys_reset_delayed_rsp_id(&(p_Rsp->z_DiagCmdHdrV2));

    /* Send the config to MGP */
    z_NavConfig.u_GnssDesiredConfig = p_Req->u_GnssDesiredConfig;
    mgp_NavConfig(&z_NavConfig);

    /* Fill out the response */
    p_Rsp->u_GnssSupportedConfig = z_NavConfig.u_GnssSupportedConfig &
                                   ( C_RCVR_GNSS_CONFIG_GPS_ENABLED |
                                     C_RCVR_GNSS_CONFIG_GLO_ENABLED |
                                     C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE |
                                     C_RCVR_GNSS_CONFIG_GAL_ENABLED_WORLDWIDE );

    p_Rsp->u_GnssActualConfig = z_NavConfig.u_GnssActualConfig &
                                   ( C_RCVR_GNSS_CONFIG_GPS_ENABLED |
                                     C_RCVR_GNSS_CONFIG_GLO_ENABLED |
                                     C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE |
                                     C_RCVR_GNSS_CONFIG_GAL_ENABLED_WORLDWIDE );
  }

  return(p_Rsp);
} 


/* -------------------------------------------------------------------------
 * mgp_NavConfig
 *
 * Description: 
 *
 *  This function is used to configure the Nav Core, which will then
 *  configure Nav RF as needed and notify upper layers like MC.
 *
 * Parameters:
 *  p_NavConfig : Pointer to a struct to hold the config results.
 *
 * Return value: 
 *  None
 * -----------------------------------------------------------------------*/
void mgp_NavConfig(mgp_NavConfigType *p_NavConfig);

typedef struct
{ 
  uint8 u_GnssDesiredConfig;    /* flags containing constellations 
                                   desired, uses the 
                                   C_RCVR_GNSS_CONFIG_XXX defines */
  uint8 u_GnssSupportedConfig; /* flags containing constellations 
                                  supported by HW, uses the 
                                  C_RCVR_GNSS_CONFIG_XXX defines */
  uint8 u_GnssActualConfig;    /* flags containing constellations 
                                  actually configured, uses the 
                                  C_RCVR_GNSS_CONFIG_XXX defines */
} mgp_NavConfigType;



#gps/gnss/sm/tm/src/tm_diag.h
/* Definition of Nav config command */
typedef PACKED struct PACKED_POST
{
  gpsdiag_CmdHdrType_v2 z_DiagCmdHdrV2;         /* diagpkt_subsys_header_type */ 
  uint8                 u_GnssDesiredConfig;    /* flags containing constellations 
                                                   desired, uses the 
                                                   C_RCVR_GNSS_CONFIG_XXX defines */
} gpsdiag_NavConfigReqType;

typedef PACKED struct PACKED_POST
{
  gpsdiag_CmdHdrType_v2 z_DiagCmdHdrV2;        /* diagpkt_subsys_header_type */
  uint8                 u_GnssSupportedConfig; /* flags containing constellations 
                                                   supported by HW, uses the 
                                                   C_RCVR_GNSS_CONFIG_XXX defines */
  uint8                 u_GnssActualConfig;    /* flags containing constellations 
                                                   actually configured, uses the 
                                                   C_RCVR_GNSS_CONFIG_XXX defines */
} gpsdiag_NavConfigRspType;


#config inside the session manager
// Runtime constellation configuration is set on boot based on the value set in NV.   
// SM gets the runtime setting by calling mgp_GnssGetConstellationConfig(). 
#define SM_CCFG_RT_GPS  (0x0001<<0)
#define SM_CCFG_RT_GLO  (0x0001<<1)
#define SM_CCFG_RT_BDS  (0x0001<<2)
#define SM_CCFG_RT_GAL  (0x0001<<3)
#define SM_CCFG_RT_QZSS (0x0001<<4)
#define SM_CCFG_RT_ALL  (SM_CCFG_RT_GPS|SM_CCFG_RT_GLO|SM_CCFG_RT_BDS|SM_CCFG_RT_GAL|SM_CCFG_RT_QZSS)
/* Compile time Constellation Configuration */
// Compile time constallation configuration indicates which constellations can be enabled on runtime. 
// If certain constellation is disabled by compiletime featurization, there's no way to enabled it by
// changing NV settings. That's because compile time setting may remove portions of the code for memory
// saving purposes from the build.
#define SM_CCFG_CT_GPS  (0x0001<<0)
#define SM_CCFG_CT_GLO  (0x0001<<1)
#define SM_CCFG_CT_BDS  (0x0001<<2)
#define SM_CCFG_CT_GAL  (0x0001<<3)
#define SM_CCFG_CT_QZSS (0x0001<<4)
#define SM_CCFG_CT_ALL  (SM_CCFG_CT_GPS|SM_CCFG_CT_GLO|SM_CCFG_CT_BDS|SM_CCFG_CT_GAL|SM_CCFG_CT_QZSS)

#define SM_CCFG_NONE    (0x0)

//initianize the ccfg struct
typedef uint16 sm_ConstellationCfgType;
static sm_ConstellationCfgType sm_runtime_ccfg = 0; 
static sm_ConstellationCfgType sm_compiletime_ccfg = 0;

#gps/gnss/common/inc/gnss_common.h
GNSS NV config mask bitmap
/* Bit Mask for Receiver GNSS Configuration */
#define C_RCVR_GNSS_CONFIG_GPS_ENABLED                (1<<0)
#define C_RCVR_GNSS_CONFIG_GLO_ENABLED                (1<<1)
#define C_RCVR_GNSS_CONFIG_BDS_ENABLED_OUTSIDE_OF_US  (1<<2) /* Used only for GNSS Config NV set and get */
#define C_RCVR_GNSS_CONFIG_GAL_ENABLED_OUTSIDE_OF_US  (1<<3)
#define C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE      (1<<4) /* Used only for GNSS Config NV set and get */
#define C_RCVR_GNSS_CONFIG_GAL_ENABLED_WORLDWIDE      (1<<5)
#define C_RCVR_GNSS_CONFIG_QZSS_ENABLED_WORLDWIDE     (1<<6) /* Used only for GNSS Config NV set and get */
#define C_RCVR_GNSS_CONFIG_HBW_ENABLED                (1<<7)
/* QZSS TBD -PS there is no final decision on QZSS enabling bits. 
   Note, MC uses internally 8 bit word presenting GNSS configuration but NV item has 32 bits.*/
#define C_RCVR_GNSS_CONFIG_QZSS_ENABLED_OUTSIDE_OF_US (1<<8) /* Used only for GNSS Config NV set and get */
#define C_RCVR_GNSS_CONFIG_BDS_ENABLED (C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE | C_RCVR_GNSS_CONFIG_BDS_ENABLED_OUTSIDE_OF_US) /* Use this definition for BDS capablity of receiver */
#define C_RCVR_GNSS_CONFIG_GAL_ENABLED (C_RCVR_GNSS_CONFIG_GAL_ENABLED_WORLDWIDE | C_RCVR_GNSS_CONFIG_GAL_ENABLED_OUTSIDE_OF_US) /* Use this definition for GAL capablity of receiver */
#define C_RCVR_GNSS_CONFIG_QZSS_ENABLED (C_RCVR_GNSS_CONFIG_QZSS_ENABLED_WORLDWIDE) /* Use this definition for QZSS capablity of receiver (8 bit word) */


/*
******************************************************************************
* mgp_GnssGetConstellationConfig
*
* Function description:
*
*  Reads NV item which describes the enabled constellations and SBAS control information.
*
* Parameters:
*  p_GnssCfg- pointer to GNSS Configuration constellations information
*                   Bit 0  Controls GPS, and has to be set to 1 always
*			Bit 1  Controls Glonass.
*			Bit 2  Controls Beidou outside of USA.
*			Bit 3  Controls Galileo outside of USA.
*			Bit 4  Controls Beidou worldwide.
*			Bit 5  Controls Galileo worldwide.
*			Bit 6  Reserved for future use, and has to be set to 0 always.
*			Bit 7  Reserved for HBW
*			Bits 8..31  Reserved, and has to be set to 0 always.
*
*  p_SbasCfg- pointer to SBAS control information
*			Value 0  SBAS is disabled
*			Value 1  SBAS is enabled
*			All other values  Reserved
*
* Return value:
*  TRUE in case both values are read otherwise FALSE incase the input pointers are corrupted
*
********************************************************************************
*/
boolean mgp_GnssGetConstellationConfig(uint32 *p_GnssCfg, uint32 *p_SbasCfg);


/**
* @brief  Query supported runtime constellations from MGP and initialize supported SM constellation configuration.
* 
* @return  void
* @sideeffects None
* @see
* 
*/

void sm_init_constellation_config()
{
  uint32 q_GnssCfg = 0;
  uint32 q_SbasCfg = 0;
  sm_ConstellationCfgType cfg = 0;

  if (mgp_GnssGetConstellationConfig( &q_GnssCfg, &q_SbasCfg) == TRUE) // Reads NV item which describes the enabled constellations and SBAS control information.
  {
    if ( 0 != (q_GnssCfg & C_RCVR_GNSS_CONFIG_GPS_ENABLED) )
    {
      cfg |= SM_CCFG_RT_GPS;
    }

    if ( 0 != (q_GnssCfg & C_RCVR_GNSS_CONFIG_GLO_ENABLED) )
    {
      cfg |= SM_CCFG_RT_GLO;
    }

    if ( 0 != (q_GnssCfg & (C_RCVR_GNSS_CONFIG_BDS_ENABLED_OUTSIDE_OF_US | C_RCVR_GNSS_CONFIG_BDS_ENABLED_WORLDWIDE) ) )
    {
      cfg |= SM_CCFG_RT_BDS;
    }

    if( 0 != (q_GnssCfg & (C_RCVR_GNSS_CONFIG_GAL_ENABLED_OUTSIDE_OF_US | C_RCVR_GNSS_CONFIG_GAL_ENABLED_WORLDWIDE) ) )
    {
      cfg |= SM_CCFG_RT_GAL;
    }

    if( 0 != (q_GnssCfg & (C_RCVR_GNSS_CONFIG_QZSS_ENABLED_OUTSIDE_OF_US | C_RCVR_GNSS_CONFIG_QZSS_ENABLED_WORLDWIDE) ) )
    {
      cfg |= SM_CCFG_RT_QZSS;
    }
  }
  else
  {
      /*something went wrong, fallback to GPS only */
      cfg=SM_CCFG_RT_GPS;
  }

  sm_set_constellation_config(cfg);
  return;
}
