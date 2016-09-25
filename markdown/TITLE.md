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
...

# まえがき {.unnumbered}
このドキュメントは、秋月電子で販売されている2.7インチモノクロメモリ液晶[^01]の
ブレークアウト基板の仕様とmbed(TM)向けライブラリを解説する本です。

シャープの2.7インチメモリ液晶は400x240ドットの解像度を持つ、反射型モノクロ液晶
です。5V単一電源で動かせて[^02]SPIバスに直結できるのが便利なうえ、他の一般的な
グラフィック液晶とは異なり、液晶が画素データを保持するのでマイクロコントローラ
側のプログラムが簡潔にできます。

直後に秋月電子もコントローラ基板を売りだした[^03]のですが、コントローラのPICマイコン
と液晶搭載部分が一体になっていました。

一方筆者も2014年にArduinoシールド形状の基板を試作[^04]し、OMMFで販売もしました
が、その後マルツ[^05]とスイッチサイエンス[^06]で通販を始めた際に現在の設計に
変更しました。

この本では、現在販売されている基板及びmbed用ライブラリ[^07]の設計についての解説
をします。いままでにこの基板を入手された方は特に _SRAMの存在価値について_ 疑問を
お持ちだと思いますので、そのあたりを重点的に。

`markdown/1.0_BoardDesign.md`{.include}
<!--  -->
[^01]: TODO: 秋月商品ページへのリンク
[^02]: 信号は3V系推奨らしいです
[^03]: TODO: 秋月商品ページへのリンク
[^04]: フレキを通すために削った部分の幅が狭くて加工が必要だった。
[^05]: TODO: マルツ商品ページヘのリンク
[^06]: TODO: スイッチサイエンス商品ページへのリンク
[^07]: TODO: mbedライブラリへのリンク
