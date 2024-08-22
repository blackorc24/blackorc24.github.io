---
title: 자막없는 동영상 SRT 자막 만들기 그리고 번역하기
author: me
date: 2024-08-22 00:00:00 +0800
categories: [기타,공개툴활용]
tags: [영상자막]
---

## openai-whisper 로 동영상 음성을 자막으로

제공되는 Github을 가보면 파이쎤으로 실행하는 방법만 설명되어 있는데
(SRTranslator 까지 설치하려면 이쪽이 편하지만)

맥에서 진행하기 때문에 brew 로 설치       

```
brew install openai-whisper
```

끝. 아님.    
 

```
whisper <영상파일명> --model large --output_format srt
```
언어 옵션도 있는데 기본으론 자동설정이기 때문에 특별히 지정하진 않음    
번역옵션도 있지만 형편없기 때문에 다른 툴로 번역한다       

model 은 large로 했는데    
영어에선 필요없고,VRAM이 10GB가 필요하다고    

맥미니 M2Pro 램 16GB으로 24분짜리 영상을 사용하니 3시간 20분 정도 수행해서 자막 완성  

```
aiwhisper.zsh ./02no.mkv  # 매번 옵션 넣기 귀찮아서 만든 쉘스크립트  
/opt/homebrew/Cellar/openai-whisper/20231117_5/libexec/lib/python3.12/site-packages/whisper/transcribe.py:115: UserWarning: FP16 is not supported on CPU; using FP32 instead
  warnings.warn("FP16 is not supported on CPU; using FP32 instead")   # M2라서 아직 FP32 사용 
Detecting language using up to the first 30 seconds. Use `--language` to specify the language
Detected language: Japanese
[00:00.000 --> 00:08.320] いろいろと世話になったな
```

## SRTranslator 로 자막을 DeepL 번역하기    

하지만 DeepL은 유료잖아? 싶지만    
무료로 여러번 돌려서 자막을 생성해 주면 되는걸 누군가는 이미 생각했다    
ChatGPT 채팅형도 가능하지만(번역왕), 50줄씩만 번역해서 채팅으로 주기 때문에 귀찮다    

python으로 설치하면 되는데, 앞서 설치만 whisper 이 brew 설치 했기 때문에     

의존성 패키지중 certifi 의 버전이 맞지 않아서 오류가 발생한다    
그러니, python 가상 환경으로 설치한다    


```
python3 -m venv venv   # 디렉토리가 생성되므로 적절한 위치에서 실행합니다 
source venv/bin/activate   # python 가상환경을 적용 

pip install srtranslator
```

이렇게 설치하면 끝    

매번 실행할 때 마다 가상 환경을 적용해줘야 하므로 간편하게 쉘 스크립트 제작 

```
#!/bin/zsh
# Deepl 을 사용해  srt,ass의   자막 언어 변환

# 사용자로부터 입력받은 파일 경로
FILEPATH="$1"

# 가상환경 활성화
source ~/bin/srtenv/bin/activate

# srtranslator 명령어 실행 
python -m srtranslator "$FILEPATH" -o ko

# 가상환경 비활성화
deactivate
```

한국어로만 받을 거니까 -o ko 로 작성     
소스언어도 지정할 수 있는데 -i en   

입력안하면 자동이니까 난 자동     

전문가의 번역과는 비교가 안되기 때문에 그냥 아쉬운 영상 자막을 번역하는 정도로만 사용한다    


## 원문 링크 

[openai-whisper](https://github.com/openai/whisper/blob/main/README.md)   

[SRTranslator](https://github.com/sinedie/SRTranslator/blob/master/README.md)

