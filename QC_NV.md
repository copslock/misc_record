nv 550 IMEI号

nv 453 FTM模式打开，此处会导致RADIO_UNAVAILABLE

nv 70210 热插拔

nv74308 开关SA NSA模式

高通AT

1、设置卡槽

 at$qcsimapp=卡槽   0 与 1 slot 


2、读取APN

AT+CGDCONT?

删除profile 2的apn

AT+CGDCONT=2,"",""

修改profile2 apn为IPV6

修改apn

at+cgdcont=2,"IPV6","ims"
