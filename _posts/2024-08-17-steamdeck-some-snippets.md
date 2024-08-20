---
title: 스팀덱의 몇가지 스니펫
author: me
date: 2024-08-17 00:00:00 +0800
categories: [스팀덱,설정]
tags: [스팀덱팁]
---

## 출처   

[출처 : 도움이 되었으면 커피한잔 사달라고 합니다 5$](https://github.com/csrutil/steamdeck)

몇가지는 이미 구글링을 통해 얻었던 정보인데    

성능 향상을 위한 보안 설정 끄기는 또 여기서 먼저봐서 성능 향상이 느껴지면    

커피한잔 드려얍죠    

**이 설정으로 인해 발생하는 문제에 대해서는 책임지지 않습니다**

## 준비물 

데스크탑 모드에서 터미널을 수행합니다    

* 암호를 설정해둡니다 

```
passwd
```

* 읽기전용 모드를 제거합니다

```
sudo steamos-readonly disable
```

* 필요한 패키지를 설치합니다 

```
Install common packages pacman -S iperf3 tmux ethtool net-tools smemstat
```



## SSH 

* SSH 데몬을 수행합니다 

```
systemctl start sshd
systemctl enable sshd
```
* 인증파일을 복사합니다   

```
# copy pub key to ~/.ssh/authorized_keys
ssh-copy-id -i ~/.ssh/your_key_ed25519 deck@deck_ip
```

* 패스워드 인증을 제거하고 인증파일 인증을 사용합니다   
(반드시 필요한건 아닙니다 그냥 패스워드로 SSH 접속해서 사용해도 됩니다)  

```
PasswordAuthentication no
PubkeyAuthentication yes
```

## Journald logging

시스템의 저널링으로 인한 데이터를 50MB로 제한합니다 

확인해 보면 10시간 동안 약 22MB정도만 사용되므로 설정 제한을 둡니다    

```
# the size for the log, 10 hours
sudo du -sh /var/log/journal
22M /var/log/journal
``` 

* 저널 설정 편집

```
emacs /etc/systemd/journald.conf
```
```
[Journal]
#Storage=auto
#Compress=yes
#Seal=yes
#SplitMode=uid
#SyncIntervalSec=5m
#RateLimitIntervalSec=30s
#RateLimitBurst=10000
#SystemMaxUse=
SystemMaxUse=50M
Restart systemd-journald
```

* 저널데몬 재실행   
```
systemctl restart  systemd-journald
```

## Swap

기본적으로 100 으로 설정되어 있으나    
(제가 할땐 60)   
1로 설정해줍니다   

스왑사용을 줄여서 디스크 수명을 늘립니다 

```
cat /etc/sysctl.d/swappiness.conf
vm.swappiness=100
```
이라고 했지만, 이런 파일이 없어서    

```
sysctl vm.swappiness
```
수행으로 60이 설정되어있는것을 확인했고   

```
sudo sysctl vm.swappiness=10
```
로 10으로 설정했습니다   

```
sudo emacs /etc/sysctl.d/swappiness.conf
vm.swappiness=10
```

그래도 없길래 만들었는데, 스팀OS버전이 달라졌으니 꼭 필요한지는 모르겠네요   


## Spectre

[From https://meltdownattack.com/](https://meltdownattack.com)

다른 프로세스의 데이터를 탈취할 수 있는 문제를 보안해주는 기능이나   
게임만 하는 스팀덱에 무슨 상관?이라서 보안을 제거합니다 

원 게시물에 따르면 10% 정도의 성능 향상이 있다고 합니다    
(7zip 기준)   

#### GRUB_CMDLINE_LINUX_DEFAULT에 mitigations=off 설정 넣기 

```
nano /etc/default/grub

GRUB_CMDLINE_LINUX_DEFAULT="loglevel=3 quiet splash plymouth.ignore-serial-consoles module_blacklist=tpm amd_iommu=off amdgpu.gttsize=8128 spi_amd.speed_dev=1 audit=0 fbcon=vc:4-6 fbcon=rotate:1 mitigations=off nowatchdog nmi_watchdog=0"
```
해당 파일을 편집하면 초반에 관련 설정이 있고 

```mitigations=off nowatchdog nmi_watchdog=0"``` 이 부분을 추가해 줍니다    


## 다시 출처    

해당 설정을 빠르게 하는 쉘명령어는 출처에 있습니다     

[출처 : 도움이 되었으면 커피한잔 사달라고 합니다 5$](https://github.com/csrutil/steamdeck)
