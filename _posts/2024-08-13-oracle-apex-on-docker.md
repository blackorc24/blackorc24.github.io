---
title: Docker로 오라클 Apex로컬 설치
author: me
date: 2024-08-13 00:00:00 +0800
categories: [Oracle,apex]
tags: []
---

## Docker에 OracleApex 23c 설치하기

[Using Oracle APEX with the 23c version of free Docker (링크포함)](https://pretius.com/blog/oracle-apex-docker-ords/)

원문에 나온 내용을 일부 따라하기 입니다만    
그대로 쓰면 SSL인증서가 만료되어 오류가 발생합니다    

해서 그 부분을 살짝 수정하고 진행하는 내용만 추가로 적습니다    


## 도커에 설치

맥, 리눅스 , 윈도우즈에서 사용가능하나    

DockerDesktop을 설치하지 않은 ARM맥의 경우 우선   
[도커엔진 대용 Colima를 설치를 하길 권장합니다(원문링크포함)](https://rafal.hashnode.dev/oracle-23c-free-apex-ords-on-docker-a-guide-for-macos-with-apple-m1)   

#### ARM 맥에서 docker 설치
1. Brew 설치   

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2. Colima 설치   

* 도커 엔진과는 겹치므로 도커 엔진이 존재하는지 확인 필요    
```docker into```  

```
brew install colima
```

3. Colima 실행

```
colima start --arch x86_64 --memory 4
```

이 설정으로 사양이 부족할 때는   
이렇게 사용합니다    
```
colima start --arch x86_64 --memory 6 --cpu 4
```

너무 많이 할당해주면 맥이 느려지므로 주의합니다   
아래 명령으로 전체 CPU갯수를 확인할 수 있습니다

```
sysctl -n hw.physicalcpu
```

#### 도커 컨테이너 생성 및 Apex 설치 파일 준비

도커 이미지 및 컨테이너를 생성하고  
Apex 설치에 필요한 스크립트를 다운받는 쉘명령어 입니다   

```shell
docker create -it --name 23cfree -p 8521:1521 -p 8500:5500 -p 8023:8080 -p 9043:8443 -p 9922:22 -e ORACLE_PWD=E container-registry.oracle.com/database/free:latest
curl -o unattended_apex_install_23c.sh https://raw.githubusercontent.com/Pretius/pretius-23cfree-unattended-apex-installer/main/src/unattended_apex_install_23c.sh
curl -o 00_start_apex_ords_installer.sh https://raw.githubusercontent.com/Pretius/pretius-23cfree-unattended-apex-installer/main/src/00_start_apex_ords_installer.sh
docker cp unattended_apex_install_23c.sh 23cfree:/home/oracle
docker cp 00_start_apex_ords_installer.sh 23cfree:/opt/oracle/scripts/startup
# 위에까지 다 실행하고 여기서 멈춰 docker start 23cfree
```

하지만, 이대로 실행하면 
```
curl -o apex-latest.zip https://download.oracle.com/otn_software/apex/apex-latest.zip

curl: (60) SSL certificate problem: self signed certificate in certificate chain
More details here: https://curl.haxx.se/docs/sslcerts.html

curl failed to verify the legitimacy of the server and therefore could not
establish a secure connection to it. To learn more about this situation and
how to fix it, please visit the web page mentioned above.
```
이런 오류를 만날 수 있는데요    
SSL 인증서가 만료됐기 때문에 SSL인증서를 갱신해주거나    
검증을 비활성화 해야합니다    

그래서 저는 자동스크립트에서 간단하게 해결하고자 SSL검증을 비활성화 했습니다    

설치 스크립트 내의 모두 세곳에 추가해주면 됩니다    

우선 "unattended_apex_install_23c.sh" 이 스크립트를 좋아하는 편집기로 열어줍니다  
  1) "# Get APEX" 위치를 찾습니다   

``` curl -k -o apex-latest.zip https://download.oracle.com/otn_software/apex/apex-latest.zip ```   

원래 있던 내용에 "-k" 만 추가합니다 SSL인증을 무시하는 옵션입니다    

   2) "# Install software" 위치를 찾습니다   
``` 
cat /dev/null > /etc/dnf/vars/ociregion

# 여기 두줄 추가
sed -i '/enabled=1/a sslverify=0' /etc/yum.repos.d/oraclelinux-developer-ol8.repo
sed -i '/enabled=1/a sslverify=0' /etc/yum.repos.d/oracle-linux-ol8.repo
### 

dnf update -y
dnf install sudo -y
dnf install nano -y
```

yum 레파지토리에서 다운받을때 SSL인증을 무시하는 옵션을 내용에 추가한것입니다    

   3) "# Install ORDS" 위치를 찾습니다   
```
su - <<EOF
yum-config-manager --add-repo=http://yum.oracle.com/repo/OracleLinux/OL8/oracle/software/x86_64
## 역시 한줄 추가 합니다
sed -i '/enabled=1/a sslverify=0' /etc/yum.repos.d/yum.oracle.com_repo_OracleLinux_OL8_oracle_software_x86_64.repo
## 
dnf install ords -y
EOF
```

ORDS를 다운받는 레파지토리를 지금 추가 하는 것이기 때문에     
새로 생성된 레파지토리 정보에 SSL인증을 무시하는 옵션을 추가한것입니다   

이제 앞서 중단 했던 
```
docker start 23cfree
```

도커를 실행한뒤 

```
docker exec -it 23cfree /bin/bash
tail -f /home/oracle/unattended_apex_install_23c.log
```

로 로그를 확인합니다 

로그파일 "unattended_apex_install_23c.log"이 조금 늦게 생성될 수도 있습니다   


오랜시간이 지난뒤   
```
### APEX INSTALLED ### 
```
의 내용이 보이면 드디어 끝난것입니다    

[http://localhost:8023/ords/apex](http://localhost:8023/ords/apex)
에 접속해서    

* User: ADMIN
* Password: OrclAPEX1999!

를 입력하면 다시 패스워드를 리셋하고 쭉 진행하면 끝   


설치중 문제가 생긴다면 원문  "(Original article) Manual setup" 에서 수동으로    
실행하는 부분을 따라 하시다 보면 문제가 생기는곳을 발견할 수 있습니다    

