ifeq ($(OS),Windows_NT)
HOME = C:/Users/$(USERNAME)
endif
PANSTYLES= $(HOME)/.pandoc
MISC= $(PANSTYLES)/pandoc_misc
include $(MISC)/Makefile.in

# MDDIR:= markdown
# DATADIR:= data
# TARGETDIR:= Out
# IMAGEDIR:= images
# WAVEDIR:= waves
# BITDIR:= bitfields
# BIT16DIR:= bitfield16
# MFILTDIR:= mermaid-filter

# INPUT:= TITLE.md
TARGET:= AkiSpiLcd

include $(MISC)/Makefile
