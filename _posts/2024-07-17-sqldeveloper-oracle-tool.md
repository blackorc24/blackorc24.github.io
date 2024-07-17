---
title: SQLDeveloper에서 PL/SQL결과가 안보일때
author: me
date: 2024-07-17 00:00:00 +0800
categories: [Oracle,SQLDeveloper]
tags: []
---

## 문제   

```SQL
dbms_output.put_line('');
```
의 내용이 출력되지 않고,

```anonymous block completed ``` 라고 나온다면,

## 원인   

SQL*Plus명령어 입니다   

## 해결

```SQL
set serveroutput on <format  wraped>
```

를 먼저 실행해두면 됩니다 


