---
title: SQLDeveloper 사용
author: me
date: 2024-07-17 00:00:00 +0800
categories: [Oracle,SQLDeveloper]
tags: []
---

## PLSQL출력 결과가 안나올때 

```SQL
dbms_output.put_line('');
```
의 내용이 출력되지 않고,

```anonymous block completed ``` 라고 나온다면,

SQL*Plus명령어 입니다   

```SQL
set serveroutput on <format  wraped>
```
를 먼저 실행해두면 됩니다 


## 스크립트 실행중 에러가 발생하면 멈추기

SQLDeveloper 스크립트 전체 실행중 오류가 발생해도 해당 스크립트를 건너뛰게 설정되어있습니다  

```
whenever sqlerror exit
whenever oserror exit
```

이 설정을 먼저 해두면 오류 발생시 멈춥니다       

SQLPLUS에서 사용하는 설정들을 그대로 쓰는것들이라   
다양한 설정이 가능합니다      

```
WHENEVER SQLERROR {EXIT [SUCCESS | FAILURE | WARNING | n | variable  | :BindVariable] [COMMIT | ROLLBACK] | CONTINUE [COMMIT | ROLLBACK | NONE]}
```

[그래서 SQL*Plus Reference 에 다 있다(링크포함)](https://docs.oracle.com/en/database/oracle/oracle-database/19/sqpug/WHENEVER-SQLERROR.html)

