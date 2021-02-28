Qcom WCDMA B19 NV 

Description:

请提供一下WCDA B19的nv参数，xml文件，我找到的WTR4905_JP_WCDMA_B19_TX0.xml 这个文件里面只有一项：<NvItem id="27172" name="RFNV_WCDMA_B19_FBRX_GAIN_SWITCH_POINT_I" mapping="direct" encoding="dec">3,-260,-30,300,0,0,0,0,0,0,0</NvItem>
</NvSource>

Comment:

Created By: Qualcomm (8/13/2015 7:02 PM)

1)B19 shares the same NV with B5. 

2)Do we need to calibrate WCDMA B6 & B19 if our UE support WCDMA B5? 

WCDMA B19 need not be calibrated independently. If W B5 is supported, SW will interpolate and reuse B5 cal/static data. 

Since B6/B19 is the subset of B5 in frequency domain, B5/B6/B19 share same NV items. You DON'T NEED to do B6/B19 RF calibration. Same static NV items are shared among B5/B6/B19. But you need to modify NV_RF_BC_CONFIG_I(NV#1877) and NV_RF_BC_CONFIG_DIV_I(NV#4548, if Rx diversity is enabled) for B6/B19 accordingly. 

The bit positions for B6 and B19 in NV 1877/4848 are 27 & 60. Refer sys.h in code path ..modem_procmmcpapipublicsys.h. 

ypedef enum { 
<snip> 

SYS_SBAND_WCDMA_VI_800 = 27, 
/**< WCDMA Japan 800 band. */ 

<snip> 

SYS_SBAND_WCDMA_XIX_850 = 60, 
/**< WCDMA Japan 850 band. */ 
<snip> 

} sys_band_class_e_type;
