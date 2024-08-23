---
title: 스팀덱에 Wake-on-lan(WOL) 가능하게 설정하기
author: me
date: 2024-08-16 00:00:00 +0800
categories: [스팀덱,설정]
tags: [스팀덱팁]
---

## 스팀덱에 WOL설정해서 원격 플레이 하기

개인적으로 스팀덱은 집안에서 이동하기 혹은 다른 거점에서 사용하기 적합한 기기이지     
이동중에 사용하기에는 휴대성이나 배터리 관리가 폰에 비해 번거롭기 때문에    

편도 1시간30분이 걸리는 출근길에는 아이폰으로 원격 플레이를 하기로 했다    
검색해보니 스팀덱을 리모트 호스트로 쓰고자 하는 사람들이 이미 많았기에    
무리없이 바로 정보를 얻을 수 있었고, 담에 까먹을것 같아 정리한다    

스팀덱 무선랜으론 할 생각을 안해서 일단 고려대상이 아니고    
애플 USB 유선랜을 USB독에 연결해서 사용한다    

## WOL 설정하기   

### 읽기전용모드 비활성   

 데스크탑 모드로 진입해서 콘솔을 실행한다    
 ```
 sudo steamos-readonly disable
 ```

### 패키지 설치 
```
sudo pacman -S gnu-netcat 
```
하지만, 이것만 설치해서는 ethtool이 실행되지 않았다   
그래서 추가로 설치    
```
pacman -S iperf3 tmux ethtool net-tools smemstat
``` 
smemstat 는 없을 수도 있는데 제외하고 설치후 진행했지만 문제는 없었다   
ethtool만 설치해도 괜찮을지도?    

### 네트워크장치 정보 확인

이미 설치 끝나서    
참조한 링크 정보를 그대로 사용했다 

```
$ ip a
5: enp4sxxxxxx: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 00:e0:xx:xx:xx:xx brd ff:ff:ff:ff:ff:ff
    inet 163.xxx.xxx.xx/xx brd 163.xxx.xxx.255 scope global dynamic noprefixroute enp4s0f3u1u4
```
'ip a' 를 실행하면     
여러 네트워크 정보가 있는데    

이름이 wlan을 포함하지 않고 아이피를 가진 장치가 있다면 그것이 USB랜일것이니     
거기서 

* 장치인식아이디(enp4sxxxxxx 의 형식)
* 맥어드레스(00:e0:xx:xx:xx:xx 의 형식) 
* IP (192.168.1.18 의 형식) 

의 정보를 미리 갈무리해서 쓰기 좋게 준비해두자    

### 장치의 WOL설정 확인하기   

```
$ sudo ethtool enp4sxxxxxx | grep Wake-on

Supports Wake-on: pumbag
Wake-on: d

$ sudo ethtool -s enp4sxxxxxx wol g
``` 
'sudo ethtool enp4sxxxxxx | grep Wake-on' 를 실행하면(대소문자 구분)     
'Wake-on: d' 이렇게 현재 장치가 지원하는 정보가 나오는데     
내가 사용하는 애플USB이더넷은 'pg' 였다     
그래서 그냥 써도 되는거 아닌가 싶어 이 상태로 매직패킷을 보내봤지만        
아니였다    

WOL의 플래그는 
```
d: Wake-on-LAN 기능 비활성화 (Disabled)
p: 패턴 매칭 (Pattern match)
u: Unicast 패킷을 수신시 깨우는 설정
m: 멀티캐스트 패킷 (Multicast packet)
b: broadcast 패킷 (Broadcast packet)
a: ARP 요청 (ARP request)
g: Magic Packet을 감지했을 때 시스템을 깨우는 설정
```

이런식인데 어째든 ```sudo ethtool -s enp4sxxxxxx wol g``` 로 g 상태로 변경해준다    

### 테스트 

```
$ sudo systemctl stop firewalld
```
방화벽을 끄고 매직패킷 수신여부를 확인한다 안해도 된다 

```
$ sudo nc --udp --listen --local-port=9 --hexdump
```
이렇게 실행해두면 대기 상태가 되는데 

아이폰에 매직 패킷을 보낼 앱을 설치하고    

포트는 9(UDP) 로 설정한뒤 매직패킷을 보내면
```
�������Lh��Lh��Lh��Lh����Lh��Lh�.... Received 102 bytes from the socket
00000000  FF FF FF FF  FF FF 00 E0  4C 68 17 E3  00 E0 4C 68  ........Lh....Lh
00000010  17 E3 00 E0  4C 68 17 E3  00 E0 4C 68  17 E3 00 E0  ....Lh....Lh....
...
```
이런 데이터가 수신되는걸 확인할 수 있다     

테스트를 완료했으니 다시 방화벽을 실행한다    
```
$ sudo systemctl start firewalld
```

### 이더넷 장치 설정확인 

하지만, 여전히 절전모드에선 깨어나지 않는데       
USB이더넷 장치드라이버의 WOL설정을 추가해줘야 한다     

```
$ sudo lsusb

Bus 004 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 003 Device 003: ID 13d3:3553 IMC Networks Bluetooth Radio
Bus 003 Device 002: ID 28de:1205 Valve Software Steam Controller
Bus 003 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 002 Device 004: ID 0bda:8153 Realtek Semiconductor Corp. RTL8153 Gigabit Ethernet Adapter
Bus 002 Device 002: ID 0bda:0411 Realtek Semiconductor Corp. Hub
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 004: ID 3434:0311 Keychron Keychron V1
Bus 001 Device 003: ID 17ef:60e4 Lenovo Lenovo Legion M300 RGB Gaming Mouse
Bus 001 Device 002: ID 0bda:5411 Realtek Semiconductor Corp. RTS5411 Hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
```

이 중에 USB이더넷 장치와 허브를 사용한 경우    
혀브까지 적용해 줘야 하는데   
참조링크의 경우는 리얼텍 장치 3개에 모두 적용하는걸로 나오고    

내가 쓰는 허브는 허브관련 3개 애플 이더넷 1개로 총 4개를 해줬다   

```
tail /sys/bus/usb/devices/*/idVendor
tail /sys/bus/usb/devices/*/idProduct
```
이 명령어를 수행하면 각각    
```
...
==> /sys/bus/usb/devices/1-1/idVendor <==
0bda

==> /sys/bus/usb/devices/2-1.4/idVendor <==
0bda

==> /sys/bus/usb/devices/2-1/idVendor <==
0bda
...
```

```
...
==> /sys/bus/usb/devices/1-1/idProduct <==
5411

==> /sys/bus/usb/devices/2-1.4/idProduct <==
8153

==> /sys/bus/usb/devices/2-1/idProduct <==
0411
```

이런 내용들이 나오는데 둘다 할 필요는 없고 이더넷 장치로 확인되는 정보의    
```/sys/bus/usb/devices/2-1/``` 경로명을 갈무리해둔다


### 이더넷 장치의 설정 변경 

```
$ cat /sys/bus/usb/devices/1-1/power/wakeup
disabled
$ cat /sys/bus/usb/devices/2-1.4/power/wakeup
enabled
$ cat /sys/bus/usb/devices/2-1/power/wakeup
disabled
```
세개의 장치의 WOL설정을 확인한다   
'disabled' 되어 있으니 'enabled'로 바꾸준다 

```
$ sudo sh -c "echo enabled > /sys/bus/usb/devices/1-1/power/wakeup"

$ sudo sh -c "echo enabled > /sys/bus/usb/devices/2-1.4/power/wakeup"

$ sudo sh -c "echo enabled > /sys/bus/usb/devices/2-1/power/wakeup"
```

이제 스팀덱을 절전모드로 만든뒤 매직패킷을 보냅니다(내부네트워크에서 테스트함)    

### 설정 고정하기 

하지만, 리부팅하고 나면 설정이 사라질거라고 합니다    

'/etc/systemd/system/wol.service' 이 파일을 만들어 줍니다 

```
sudo emacs /etc/systemd/system/wol.service 
```
내용은 
```
[Unit]
Description=Enable WOL usb
Wants=network-pre.target
After=network-pre.target NetworkManager.service

[Service]
Type=oneshot
ExecStart=ethtool -s enp4sxxxxxx wol g ; sh -c "echo enabled > /sys/bus/usb/devices/2-1.4/power/wakeup" ; sh -c "echo enabled > /sys/bus/usb/devices/2-1/power/wakeup"
Restart=on-failure

[Install]
WantedBy=multi-user.target
```
로 채우고 

```
sudo systemctl enable wol.service
```

실행해준뒤  

```
systemctl status wol.service
```

제대로 로드 되는지 확인합니다.

저의 경우는 다중열 실행 오류가 나서 
```
[Unit]
Description=Enable WOL usb
Wants=network-pre.target
After=network-pre.target NetworkManager.service

[Service]
Type=oneshot
ExecStart=/bin/sh -c 'ethtool -s enp4s0f3u1u1 wol g && echo enabled > /sys/bus/usb/devices/1-1.1/power/wakeup && echo enabled > /sys/bus/usb/devices/2-1/power/wakeup && echo enabled > /sys/bus/usb/devices/1-1/power/wakeup && echo enabled > /sys/bus/usb/devices/1-1.5/power/wakeup'
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

이렇게 실행하는 곳을 다른 방법으로 이어서 사용했습니다    

그리고 마무리 합니다   
```
sudo steamos-readonly enable
```


## 하지만 안됨

참조한 링크초반에 "**JSAUX 6-in-1 docking station**" 라는 제품을 쎴다고 명시했는데    

내가 사용한 제품은 USB이더넷 장치가 연결되지 않았다며 WOL.service가 실패하고 있었다   

```
journalctl -u wol.service
```

USB이더넷 장치가 인식되는 시점이 상당히 느려 부팅시에는 제대로 서비스가 안되는 모양이였다    
포기... 

## 다른 장지로 해보니 됨

썬더볼트3 독으로 바꿔서 사용   
이전에 썼던건 USB독에 USB이더넷을 설치한건데    
USB장치라 시간이 지나면 절전 되어버린건지 안됐지만    

독 내장형 이더넷은 전원만 인가되어있으면 잠들지 않아서 사용가능    
스팀덱이 절전 중인 중간에 독의 전원을 끄거나 하면   
스팀덱을 다시 깨워서 장치를 인식시켜야 하나   

같은 로컬 환경에서도 스트리밍이 너무 느려서 포기    



## 참조링크   

[레딧에 누가 정리해 놔서 참조(배꼈)했습니다(링크포함)](https://www.reddit.com/r/SteamDeck/comments/1c1hujf/wakeonlan_your_steamdeck_a_full_guide_and_caveats/)


