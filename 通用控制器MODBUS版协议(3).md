# 通用控制器MODBUS版协议

## 一、通用控制器公共指令

### 1、通用控制器modbus控制协议格式（frameId:A000）

**协议帧格式：**

|   字节索引   |       N/A       |   N/A    |     N/A      |     0     |  1\|2   |    3    |   4-5    |      6      |   7    |    8    |   9-10   |              |      |                   |
| :----------: | :-------------: | :------: | :----------: | :-------: | :-----: | :-----: | :------: | :---------: | :----: | :-----: | :------: | :----------: | :--: | :---------------: |
|    数据域    | deviceFrameHead |   addr   | deviceOthers | frameHead | frameId | dataLen | randomId | isBroadcast | zoneId | groupId | interval | modbusPacket | CRC8 |     frameEnd      |
| 长度（byte） |        1        |    4     |      3       |     1     |    2    |    1    |    2     |      1      |   1    |    1    |    2     |      n       |  1   |         2         |
|   示例数据   |       61        | 14052A0C |   00 XXXX    |    FE     |  A000   |         |   1234   |     00      |   01   |   01    |   0000   |   XXXXXXXX   |  00  | 0D 0A 0D 0A 0D 0A |

**协议帧各字段域说明：**

| 字段域          | 说明         | 长度（byte） | 备注                                                         |
| --------------- | :----------- | :----------: | :----------------------------------------------------------- |
| deviceFrameHead | 硬件帧头     |      1       | 仁钰LoRa M-KL9按地址发送时需要                               |
| addr            | 设备地址     |      4       |                                                              |
| deviceOthers    | 控制字等     |      3       | 仁钰LoRa M-KL9按地址发送时需要                               |
| frameHead       | 帧头         |      2       | FE                                                           |
| frameId         | 帧ID         |      2       | 查看帧id对照表A020                                           |
| dataLen         | 数据长度     |      1       | 从此位后开始计算（不包含自身），一直到`CRC8`位结束（不包括CRC8位） |
| randomId        | 包随机Id     |      2       | 回执时原样返回                                               |
| isBroadcast     | 是否广播     |      1       | 55广播，00单播                                               |
| zoneId          | 区域Id       |      1       |                                                              |
| groupId         | 组Id         |      1       |                                                              |
| modbusPacket    | modbus数据包 |      n       | 协议格式为标准modbus-rtu格式                                 |
| CRC8            | CRC8校验码   |      1       | 用于进行CRC8计算的数据DataLen指代的长度                      |
| frameEnd        | 帧尾         |      6       | 0d 0a 0d 0a 0d 0a                                            |

**应用示例：**

打开区域1组1所有设备线圈0为1

modbus格式：

01 05 00 00 FF 00 8C 3A//线圈寄存器0置1

数据格式：

```json
00000071A0C000000 FE A000 0D 1234 55 01 01 01050000FF008C3A 00 0D0A0D0A0D0A
```



### 2、返回协议帧格式**协议帧格式：**

|   字节索引   |     0     |  1\|2   |    3    |  4-5   |      6      |   7    |    8    |    9     |   9~(9+n)    | 10+n |     11+n~16+n     |
| :----------: | :-------: | :-----: | :-----: | :----: | :---------: | :----: | :-----: | :------: | :----------: | :--: | :---------------: |
|    数据域    | frameHead | frameId | dataLen | random | isBroadcast | zoneId | groupId | interval | modbusPacket | CRC8 |     frameEnd      |
| 长度（byte） |     1     |    2    |    1    |   2    |      1      |   1    |    1    |    2     |      n       |  1   |         2         |
|   示例数据   |    FE     |  E000   |         |  1234  |     00      |   01   |   01    |   0000   |   XXXXXXXX   |  00  | 0D 0A 0D 0A 0D 0A |

**协议帧各字段域说明：**

| 字段域       | 说明             | 长度（byte） | 备注                                                         |
| ------------ | :--------------- | :----------: | :----------------------------------------------------------- |
| frameHead    | 帧头             |      2       | FE                                                           |
| frameId      | 帧ID             |      2       | 查看帧id对照表E000                                           |
| dataLen      | 数据长度         |      1       | 从此位后开始计算（不包含自身），一直到`CRC8`位结束（不包括CRC8位） |
| randomId     | 包随机数         |      2       | 原样返回                                                     |
| isBroadcast  | 是否广播         |      1       | 55广播，00单播,原样返回                                      |
| zoneId       | 区域Id           |      1       | 原样返回                                                     |
| groupId      | 组Id             |      1       | 原样返回                                                     |
| interval     | 自动上报时间间隔 |      2       | 0000不上报，单位s                                            |
| modbusPacket | modbus数据包     |      n       | 协议格式为标准modbus-rtu格式                                 |
| CRC8         | CRC8校验码       |      1       | 用于进行CRC8计算的数据DataLen指代的长度                      |
| frameEnd     | 帧尾             |      6       | 0d 0a 0d 0a 0d 0a                                            |

**应用示例：**

```json
FE E000 0D 1234 55 01 01 01050000FF008C3A 00 0D0A0D0A0D0A
```

### **3、回执：**

**协议帧格式：**

|   字节索引   |       N/A       |   N/A    |     N/A      |     0     |  1\|2   |    3    |  4-5   |          |  6   |       7~12        |
| :----------: | :-------------: | :------: | :----------: | :-------: | :-----: | :-----: | :----: | :------: | :--: | :---------------: |
|    数据域    | deviceFrameHead |   addr   | deviceOthers | frameHead | frameId | dataLen | random | interval | CRC8 |     frameEnd      |
| 长度（byte） |        1        |    4     |      3       |     1     |    2    |    1    |   2    |    2     |  1   |         2         |
|   示例数据   |       61        | 14052A0C |   00 XXXX    |    FE     |  A001   |   02    |  1234  |   0000   |  00  | 0D 0A 0D 0A 0D 0A |

**协议帧各字段域说明：**

| 字段域          | 说明                         | 长度（byte） | 备注                                                         |
| --------------- | :--------------------------- | :----------: | :----------------------------------------------------------- |
| deviceFrameHead | 硬件帧头                     |      1       | 仁钰LoRa M-KL9按地址发送时需要                               |
| addr            | 设备地址                     |      4       |                                                              |
| deviceOthers    | 控制字等                     |      3       | 仁钰LoRa M-KL9按地址发送时需要                               |
| frameHead       | 帧头                         |      2       | FE                                                           |
| frameId         | 帧ID                         |      2       | 查看帧id对照表A020                                           |
| dataLen         | 数据长度                     |      1       | 从此位后开始计算（不包含自身），一直到`CRC8`位结束（不包括CRC8位） |
| randomId        | 包随机数                     |      2       | 回执时原样返回                                               |
| interval        | 自动上报输入输出状态时间间隔 |      2       | 大于30min就关闭自动上报                                      |
| CRC8            | CRC8校验码                   |      1       | 用于进行CRC8计算的数据DataLen指代的长度                      |
| frameEnd        | 帧尾                         |      6       | 0d 0a 0d 0a 0d 0a                                            |

**应用示例：**

### **4、设置输出默认状态及超时时间：**

**协议帧格式：**

|   字节索引   |       N/A       |   N/A    |     N/A      |     0     |  1\|2   |    3    |   4-5    |                      |        |         |       |  6   |       7~12        |
| :----------: | :-------------: | :------: | :----------: | :-------: | :-----: | :-----: | :------: | :------------------: | :----: | :-----: | ----- | :--: | :---------------: |
|    数据域    | deviceFrameHead |   addr   | deviceOthers | frameHead | frameId | dataLen | randomId |        DOInit        | AOInit | timeout | RS485 | CRC8 |     frameEnd      |
| 长度（byte） |        1        |    4     |      3       |     1     |    2    |    1    |    2     |          8           |   16   |    2    | n     |  1   |         2         |
|   示例数据   |       61        | 14052A0C |   00 XXXX    |    FE     |  A002   |   02    |   1234   | 00 00 00 00 00 00 00 |        |         |       |  00  | 0D 0A 0D 0A 0D 0A |

**协议帧各字段域说明：**

| 字段域          | 说明                 | 长度（byte） | 备注                                                         |
| --------------- | :------------------- | :----------: | :----------------------------------------------------------- |
| deviceFrameHead | 硬件帧头             |      1       | 仁钰LoRa M-KL9按地址发送时需要                               |
| addr            | 设备地址             |      4       |                                                              |
| deviceOthers    | 控制字等             |      3       | 仁钰LoRa M-KL9按地址发送时需要                               |
| frameHead       | 帧头                 |      2       | FE                                                           |
| frameId         | 帧ID                 |      2       | 查看帧id对照表A020                                           |
| dataLen         | 数据长度             |      1       | 从此位后开始计算（不包含自身），一直到`CRC8`位结束（不包括CRC8位） |
| randomId        | 包随机数             |      2       | 回执时原样返回                                               |
| isBroadcast     | 是否广播             |      1       |                                                              |
| QDInit          | 数字输出默认状态     |      8       | 00 00 00 00 00 00 00,                                        |
| QAInit          | 模拟量输出默认状态   |      16      |                                                              |
| timeout         | 复位默认状态超时时间 |      2       | 单位s                                                        |
| RS485           | RS485初始指令        |      n       |                                                              |
| CRC8            | CRC8校验码           |      1       | 用于进行CRC8计算的数据DataLen指代的长度                      |
| frameEnd        | 帧尾                 |   chaxun6    | 0d 0a 0d 0a 0d 0a                                            |

**应用示例：**

### **5、设置输出默认状态及超时时间回执：**

|   字节索引   |     0     |  1\|2   |    3    |   4-5    |   9    | 10+n |     11+n~16+n     |
| :----------: | :-------: | :-----: | :-----: | :------: | :----: | :--: | :---------------: |
|    数据域    | frameHead | frameId | dataLen | randomId | status | CRC8 |     frameEnd      |
| 长度（byte） |     1     |    2    |    1    |    2     |   1    |  1   |         2         |
|   示例数据   |    FE     |  E002   |         |   1234   |        |  00  | 0D 0A 0D 0A 0D 0A |

**协议帧各字段域说明：**

| 字段域    | 说明       | 长度（byte） | 备注                                                         |
| --------- | :--------- | :----------: | :----------------------------------------------------------- |
| frameHead | 帧头       |      2       | FE                                                           |
| frameId   | 帧ID       |      2       | 查看帧id对照表E000                                           |
| dataLen   | 数据长度   |      1       | 从此位后开始计算（不包含自身），一直到`CRC8`位结束（不包括CRC8位） |
| randomId  | 包随机数   |      2       | 原样返回                                                     |
| status    | 设置状态   |      1       |                                                              |
| CRC8      | CRC8校验码 |      1       | 用于进行CRC8计算的数据DataLen指代的长度                      |
| frameEnd  | 帧尾       |      6       | 0d 0a 0d 0a 0d 0a                                            |

**应用示例：**

### 6、公共指令查询实时工作状态（A014）

#### 协议帧格式

|   字节索引   |       N/A       |   N/A    |     N/A      |     0     |  1\|2   |    3    |     4\|5     |      6      |   7    |    8    |    9    |  10  |       11-16       |
| :----------: | :-------------: | :------: | :----------: | :-------: | :-----: | :-----: | :----------: | :---------: | :----: | :-----: | :-----: | :--: | :---------------: |
|    数据域    | DeviceFrameHead |   Addr   | deviceOthers | FrameHead | FrameId | DataLen | DeviceTypeId | IsBroadcast | ZoneId | GroupId | channel | CRC8 |     FrameEnd      |
| 长度（byte） |        1        |    4     |      3       |     1     |    2    |    1    |      2       |      1      |   1    |    1    |    1    |  1   |         2         |
|   示例数据   |       61        | 00000071 |   00 XXXX    |    FE     |  A014   |   06    |     C003     |     55      |   01   |   01    |   00    |  00  | 0D 0A 0D 0A 0D 0A |

协议帧各字段域说明：

| 字段域          | 说明       | 长度（byte） | 备注                                                         |
| --------------- | :--------- | :----------: | :----------------------------------------------------------- |
| DeviceFrameHead | 硬件帧头   |      1       | 仁钰LoRa M-KL9按地址发送时需要                               |
| Addr            | 设备地址   |      4       | 00000071(广播，所有设备上报自己的ID和SN)                     |
| deviceOthers    | 控制字等   |      3       | 仁钰LoRa M-KL9按地址发送时需要                               |
| FrameHead       | 帧头       |      2       | FE                                                           |
| FrameId         | 帧ID       |      2       | A011                                                         |
| DataLen         | 数据长度   |      1       | 从此位后开始计算（不包含自身），一直到`CRCR8`位结束（不包括CRC8位） |
| DeviceTypeId    | 设备类型ID |      2       | C003                                                         |
| ZoneId          | 所属区域Id |      1       | 0x01                                                         |
| GroupId         | 组id       |      1       | 0x01                                                         |
| channel         | 查询角色   |      1       | 0x00：表示主控，0x55表示所有，其他表示相应路数的子设备       |
| CRC8            | CRC8校验码 |      1       | 用于进行CRC8计算的数据DataLen指代的长度                      |
| FrameEnd        | 帧尾       |      6       | 0d 0a 0d 0a 0d 0a                                            |

**应用示例：**

```javascript
61 14432A0C 000100 FE A014 04 C001 01 01 00 E1 0D0A0D0A0D0A
```

### 7、实时状态上报格式

|   字节索引   |     0     |  1\|2   |    3    |   4-5    |   9    |       |       |      |      |      |      |      |      | 10+n |     11+n~16+n     |
| :----------: | :-------: | :-----: | :-----: | :------: | :----: | ----- | ----- | ---- | ---- | ---- | ---- | ---- | ---- | :--: | :---------------: |
|    数据域    | frameHead | frameId | dataLen | randomId | status | swVer | hwVer | rssi | csq  | DI   | DO   | AI   | AO   | CRC8 |     frameEnd      |
| 长度（byte） |     1     |    2    |    1    |    2     |   1    | 2     | 2     | 1    | 1    | 8    | 8    | 16   | 16   |  1   |         6         |
|   示例数据   |    FE     |  E014   |         |   1234   |        |       |       |      |      |      |      |      |      |  00  | 0D 0A 0D 0A 0D 0A |

**协议帧各字段域说明：**

| 字段域    | 说明               | 长度（byte） | 备注                                                         |
| --------- | :----------------- | :----------: | :----------------------------------------------------------- |
| frameHead | 帧头               |      2       | FE                                                           |
| frameId   | 帧ID               |      2       | 查看帧id对照表E000                                           |
| dataLen   | 数据长度           |      1       | 从此位后开始计算（不包含自身），一直到`CRC8`位结束（不包括CRC8位） |
| randomId  | 包随机数           |      2       | 原样返回                                                     |
| status    | 设置状态           |      1       |                                                              |
| swVer     | 软件版本           |      2       |                                                              |
| hwVer     | 硬件版本           |      2       |                                                              |
| rssi      |                    |      1       |                                                              |
| csq       |                    |      1       |                                                              |
| DI        | 64位数字输入状态   |      8       |                                                              |
| DO        | 64位数字输出状态   |      8       |                                                              |
| AI        | 64个模拟量输入状态 |      16      |                                                              |
| AO        | 64个模拟量输出状态 |      16      |                                                              |
| CRC8      | CRC8校验码         |      1       | 用于进行CRC8计算的数据DataLen指代的长度                      |
| frameEnd  | 帧尾               |      6       | 0d 0a 0d 0a 0d 0a                                            |



## 二、与云端交互接口

### 通用部分：

### 1、设置设备类型及各接口路数

```json
{
    "confTyp":"COM_CTRL",
    "method":"SET_INTERFACE",
    "qryId":"135776125701578973925",
    "para":{
        "type":"Com_Ctrl",
        "sn":"C00320191022100719",
        "DO":16,
        "DI":16,
        "AO":16,
        "AI":16,
        "RS485":1
    }
}
```

字段解释：

* type：通用控制器子设备类型
* sn：主控sn
* DO：数字量输出总数量
* DI：数字量输入总数量
* AO：模拟量输出总数量
* AI：模拟量输入总数量
* RS485：485硬件路数总量

> 注：这些参数设置在通用控制器主控的`f_actuator_settings`里
>
> 格式如下：
>
> ```json
> "f_actuator_setting":{
>     "type":"Com_Roller",
>     "sn":"C00320191022100719",
>     "DO":16,
>     "DI":16,
>     "AO":16,
>     "AI":16,
>     "RS485":1
> }
> ```
>
> 前端解析通用控制器时需要显示该部分配置信息
>
> 前端查询卷膜机时，后端需要返回1拖1,1拖2,1拖N三种类型的所有卷膜机信息

返回：

```json
{
  "confTyp" : "COM_CTRL",
  "method" : "SET_TYPE",
  "qryId" : "135776125701578973925",
  "status" : "success",
  "msg" : "设备SN为C00320191022100719的通用控制器类型为Com_Roller及其他输入输出接口数量成功!!!",
  "para" : [ ]
}
```

### 2、删除通用控制器及其子设备参数

```json
{
    "confTyp":"COM_CTRL",
    "method":"DELETE_PARA",
    "qryId" : "135776125701578973925",
    "para":{
        "sn":"C00320191022100719"
    }
}
```

返回：

```json
{
  "confTyp" : "COM_CTRL",
  "method" : "DELETE_PARA",
  "qryId" : "135776125701578973925",
  "status" : "success",
  "msg" : "删除SN为C00320191022100718的通用控制器及其子设备信息成功!!!",
  "para" : [ ]
}
```

### 3、查询通用控制器及其子设备参数

```json
{
    "confTyp":"COM_CTRL",
    "method":"QUERY_PARA",
    "qryId" : "135776125701578973925",
    "para":{
        "sn":"C00320191022100719"
    }xiaoyu
}
```

返回：

```json
{
    "confTyp":"COM_CTRL",
    "method":"SET_TYPE",
    "qryId":"000000000001578983708",
    "status":"success",
    "msg":"查询SN为C00320191022100718的通用控制器及其子设备信息成功!!!",
    "para":{
        "master":{
            "id":41,
            "f_actuator_sn":"C00320191022100718",
            "f_actuator_alias":"通用控制器2",
            "f_actuator_type":"Com_Ctrl",
            "f_actuator_type_id":"C003",
            "f_actuator_type_alias":"通用控制器",
            "f_actuator_addr":"0C144005",
            "f_actuator_group_id_array":"0100000000",
            "f_actuator_interval":0,
            "f_actuator_allocate":"00000000000000000000000000000000",
            "f_settings_para":0,
            "f_zone_id":1,
            "f_channel":1,
            "f_actuator_status":"设置参数成功",
            "f_last_value":"",
            "f_actuator_settings":"{"type":"Com_Roller","sn":"C00320191022100719","DO":16,"DI":16,"AO":16,"AI":16,"RS485":1}",
            "f_actuator_type_mac":"",
            "f_actuator_type_media":"LoRa",
            "f_actuator_power_source":"24V",
            "f_actuator_type_tag":"通用控制器",
            "f_actuator_vol":null,
            "f_vol_baseline":10,
            "f_actuator_rssi":38,
            "f_rssi_baseline":null,
            "f_actuator_csq":0,
            "f_csq_baseline":null,
            "f_pre_allocate1":"",
            "f_pre_allocate2":null,
            "f_pre_allocate3":null,
            "f_enable":1,
            "f_bindgroup_enable":1,
            "f_actuator_edit_time":"2020-01-02 10:09:19",
            "f_remark":"null"
        },
        "slaver":[
            {
                "id":42,
                "f_actuator_sn":"A00320191022100718",
                "f_actuator_alias":"C栋-南侧",
                "f_actuator_type":"Com_Roller",
                "f_actuator_type_id":"C001",
                "f_actuator_type_alias":"卷膜机",
                "f_actuator_addr":"0C144005",
                "f_actuator_group_id_array":"0103000000",
                "f_actuator_interval":0,
                "f_actuator_allocate":"1400500014000001F1115300000000",
                "f_settings_para":70,
                "f_zone_id":1,mergerParams
                "f_channel":1,
                "f_actuator_status":"卷膜完成",
                "f_last_value":null,
                "f_actuator_settings":null,
                "f_actuator_type_mac":null,
                "f_actuator_type_media":"LoRa",
                "f_actuator_power_source":"24V",
                "f_actuator_type_tag":"卷膜机下属第2路设备",
                "f_actuator_vol":null,
                "f_vol_baseline":null,
                "f_actuator_rssi":38,
                "f_rssi_baseline":null,
                "f_actuator_csq":0,
                "f_csq_baseline":null,
                "f_pre_allocate1":null,
                "f_pre_allocate2":null,
                "f_pre_allocate3":null,
                "f_enable":1,
                "f_bindgroup_enable":1,
                "f_actuator_edit_time":"2020-01-02 10:09:19",
                "f_remark":"null"
            }
        ]
    }
}
```

### 4、配置子设备参数

```json
{
    "confTyp":"COM_CTRL",
    "method":"SET_CHANNEL",
    "para":{
        "masterSn":"C00320191022100719",
        "slaverPara":[
            {
                "alias":"区域一顶1",
                "deviceType":"Com_Roller",
				"channel":3,
                "zoneId":1,
                "groupIdArray":"0102000000",
                "interface":{
                    "DO":[3,0,0,0,0,0,0,0],
                    "DI":[0,0,0,0,0,0,0,0],
                    "AI":[1,0],
                    "AO":null,
                    "ST485":1
                },
                "close":{
                    "DO":[1,0,0,0,0,0,0,0],
                    "DI":[0,0,0,0,0,0,0,0],
                    "AILow":500,
                    "AIHigh":1024,
                    "AO":null,
                    "ST485":null
                },
                "open":{
                    "DO":[2,0,0,0,0,0,0,0],
                    "DI":[0,0,0,0,0,0,0,0],
                    "AILow":500,
                    "AIHigh":1024,
                    "AO":null,
                    "ST485":null
                },
                "stop":{
                    "DO":0,
                    "DI":null,
                    "AILow":0,
                    "AIHigh":20,
                    "AO":null,
                    "ST485":null
                },
                "interval":10
            },
            {
                "alias":"拉幕电机",
                "deviceType":"Com_Curtain",
				"channel":4,
                "zoneId":1,
                "groupIdArray":"0102000000",
                "interface":{
                    "DO":[48,0,0,0,0,0,0,0],
                    "DI":[3,0,0,0,0,0,0,0],
                    "AI":[2,0],
                    "AO":null,
                    "ST485":1
                },
                "close":{
                    "DO":[16,0,0,0,0,0,0,0],
                    "DI":[1,0,0,0,0,0,0,0],
                    "AILow":500,
                    "AIHigh":1024,
                    "AO":null,
                    "ST485":null
                },
                "open":{
                    "DO":[32,0,0,0,0,0,0,0],
                    "DI":[2,0,0,0,0,0,0,0],
                    "AILow":500,
                    "AIHigh":1024,
                    "AO":null,
                    "ST485":null
                },
                "stop":{
                    "DO":[0,0,0,0,0,0,0,0],
                    "DI":[0,0,0,0,0,0,0,0],
                    "AILow":0,
                    "AIHigh":20,
                    "AO":null,
                    "ST485":null
                },
                "interval":10
            }
        ]
    }
}
```

字段解释：

* alias：子设备别名
* channel：子设备的路数
* DO：数字量输出所占路数，使用所占位数对应的二进制转换的十进制，输出最大64位，`3=0x03H=00000000  00000000 00000000 00000000 00000000 00000000 00000011b`，上述例子相当于1路占用了DO的低两位
* DI：数字量输入所占路数
* AO：模拟量输出所占路数
* AI：模拟量输入所占路数
* ST485：485硬件站号
* zoneId：区域id
* groupIdArray：组ID数组
* close：设备关闭条件
  * DO：DO状态
  * DI：DI状态
  * AILow：AI最小值
  * AIHigh：AI最大值
  * AO：AO设定值
* open：设备打开条件
  * DO：DO状态
  * DI：DI状态
  * AILow：AI最小值
  * AIHigh：AI最大值
  * AO：AO设定值
* stop：设备停止条件
  * DO：DO状态
  * DI：DI状态
  * AILow：AI最小值
  * AIHigh：AI最大值
  * AO：AO设定值
* interval：上报数据时间间隔

返回：

```json
{
  "confTyp" : "COM_CTRL",
  "method" : "SET_CHANNEL",
  "qryId" : "000000000001578987458",
  "status" : "success",
  "msg" : "修改SN为C00320191022100719的通用控制器第2参数成功!!!",
  "para" : [ ]
}
```

### 5、设置各输出接口默认状态

```json
{
    "confTyp":"COM_CTRL",
    "method":"SET_INIT_PARA",
    "para":{
        "sn":["C00320191022100719"],
         "DO":0,
         "AO":0,
         "RS485":"01050000FF008C3A",
         "timeOut":120
    }
}
```



### 一拖N卷膜机私有：

设置topic:`baseId/actuator/cmd`

反馈topic:`baseId/actuator/cmdack`

RedisChannel:`"ROLLER"`

### 3.1、重置LoRa卷膜机行程

**（前端--->服务器--->基地服务器）**--基地管理员

**重置某个区域下的某些组卷膜机行程**

```json
{
    "confTyp":"ROLLER",
    "method":"RESET_GROUP",
    "para":{
        "zoneId":1,
        "groupId":[2]
    }
}
```

**重置某些卷膜机的行程**

```json
{
    "confTyp":"ROLLER",
    "method":"RESET_SLAVER",
    "para":{
        "sn":["C00120190304150001","C00120190304150002"]
    }
}
```



### 3.2、 设置LoRa卷膜机开度

**（前端--->服务器--->基地服务器）**--基地管理员

**设置某个区域下的某些组**

```json
{
    "confTyp":"ROLLER",
    "method":"OPEN_GROUP",
    "para":{
        "zoneId":1,
        "groupId":[1,2],
        "open":[50,100]
    }
}
```

**设置某些卷膜机的开度**

```json
{   
 "confTyp":"ROLLER",
    "method":"OPEN_SLAVER",
    "para":{
        "sn":["C00120190304150001","C00120190304150002"],
        "open":[50,100]
    }
}
```

### 3.3、 设置LoRa卷膜机工作阈值

**（前端--->服务器--->基地服务器）**--基地管理员

**设置某个区域下的某些组卷膜机阈值**

```json
{
    "confTyp":"ROLLER",
    "method":"LIMIT_GROUP",
    "para":{
        "zoneId":1,
        "groupId":[1],
        "lowBatVolLimit":[1200],
		"maxMotorVolLimit":[700],
		"statusSendInterval":[1]
    }
}
```

**设置某些卷膜机卷膜机阈值**

```json
{
    "confTyp":"ROLLER",
    "method":"LIMIT_SLAVER",
    "para":{
        "sn":["C00120190304150001","C00120190304150002"],
        "lowBatVolLimit":[1200,0],
		"maxMotorVolLimit":[700,0],
		"statusSendInterval":[1,0]
    }
}
```

### 

拉幕电机，卷膜机，水泵，补光灯，

