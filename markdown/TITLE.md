---
title: 秋月メモリ液晶ブレークアウト基板の本
abstract: 秋月電子「メモリ液晶」のブレークアウト基板とmbed^TM^ライブラリの解説
circle: KY技研/みんなのラボ
author: "@_K4ZUKI_"
date: C91/2016-Dec
comiket: "C91"
year: "2016"
publisher: "秋葉原キンコーズで印刷製本予定"
docrevision: "1.0"
short-hash: (git short hash)

documentclass: book
papersize: b5paper
mainfont: RictyDiminished-Regular
sansfont: RictyDiminished-Regular
monofont: RictyDiminished-Regular
mainlang: Japanese
CJKoptions: BoldFont=RictyDiminished-Bold,
  ItalicFont=RictyDiminished-Oblique,
  BoldItalicFont=RictyDiminished-BoldOblique
CJKmainfont: RictyDiminished-Regular
CJKsansfont: RictyDiminished-Regular
CJKmonofont: RictyDiminished-Regular
geometry: top=30truemm,bottom=30truemm,left=20truemm,right=20truemm
keywords: keyword
secPrefix: Chapter
linkcolor: black
urlcolor: black
citecolor: black
chapter: true
listings: true
codeBlockCaptions: true
listingTitle: 'List'
listingTemplate: '---**$$listingTitle$$ $$i$$$$titleDelim$$ $$t$$**---'
---

# まえがき {.unnumbered}

![外観写真(*液晶は付属しません*)](images/TopView.jpg)

このドキュメントは、秋月電子で販売されている2.7インチモノクロメモリ液晶[^01]の
ブレークアウト基板の仕様とmbed(TM)向けライブラリを解説する本です。

シャープの2.7インチメモリ液晶は400x240ドットの解像度を持つ、反射型モノクロ液晶
です。5V単一電源で動かせて[^02]SPIバスに直結できるのが便利なうえ、他の一般的な
グラフィック液晶とは異なり、液晶が画素データを保持するのでマイクロコントローラ
側のプログラムが簡潔にできます。

直後に秋月電子もコントローラ基板を売りだした[^03]のですが、コントローラの
PICマイコンと液晶搭載部分が一体になっていました。しかしこれでは液晶の動作確認
はできますが応用・流用しづらいので筆者も2014年にArduinoシールド形状の基板を
試作[^04]し、2014年のOgakiMiniMakerFaireで販売もしました。その後
マルツさん[^05]とスイッチサイエンスさん[^06]で通販を始めた際に現在の設計に
変更しました。

![Ogaki Mini Maker Faire (2014) 出展時の写真](images/OMMF2014.jpg)

この本では、現在販売されている基板及びmbed用ライブラリ[^07]の設計についての解説
をします。いままでにこの基板を入手された方は特に _SRAMの存在価値について_ 疑問を
お持ちだと思いますので、そのあたりを重点的に。

## 注意事項

この文書で対象とする液晶モジュールは前述の秋月電子で販売中のものと、Mouserで
入手できるもの[^08]の2品種のみとします。最近スイッチサイエンスで販売しだした
カラータイプはどうやらピン互換のようですが電源電圧が異なるので基板側に改造が
必要になると思いますし、とりあえず対象外にします。[^09]

`markdown/1.0_BoardDesign.md`{.include}
`markdown/2.0_mbedAPI.md`{.include}

<!--  -->
[^01]: http://akizukidenshi.com/catalog/g/gP-04944/
[^02]: 信号は3V系推奨らしいです
[^03]: http://akizukidenshi.com/catalog/g/gK-06875/
[^04]: フレキを通すために削った部分の幅が狭くて加工が必要だった。
[^05]: Thttp://www.marutsu.co.jp/pc/i/259032/
[^06]: https://www.switch-science.com/catalog/2407/
[^07]: https://github.com/K4zuki/AkiSpiLcd
[^08]: http://www.mouser.com/ProductDetail/Sharp-Microelectronics/LS027B7DH01A/
[^09]: とか言ってたけど気になったので基板設計しちゃいました =>
 https://github.com/K4zuki/SsciSpiLcd です。mbedライブラリも拡張中です。
