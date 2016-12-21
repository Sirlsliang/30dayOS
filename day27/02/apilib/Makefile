OBJS_API =	api001.obj api002.obj api003.obj api004.obj api005.obj api006.obj \
			api007.obj api008.obj api009.obj api010.obj api011.obj api012.obj \
			api013.obj api014.obj api015.obj api016.obj api017.obj api018.obj \
			api019.obj api020.obj

# 添加默认命令
TOOL_PATH :=	../../../z_tools/
INCPATH		:=	$(TOOL_PATH)haribote/

CC1				:= wine	$(TOOL_PATH)cc1.exe -I $(INCPATH) -Os -Wall -quiet
GAS2NASK	:= wine	$(TOOL_PATH)gas2nask.exe -a
NASK			:= wine $(TOOL_PATH)nask.exe
OBJ2BIM		:= wine	$(TOOL_PATH)obj2bim.exe
RULEFILE	:= $(TOOL_PATH)haribote/haribote.rul
BIM2HRB		:= wine	$(TOOL_PATH)bim2hrb.exe
MAKEFONT	:= wine	$(TOOL_PATH)makefont.exe
BIN2OBJ		:= wine	$(TOOL_PATH)bin2obj.exe
GOLIB			:= wine $(TOOL_PATH)golib00.exe

default :
	make apilib.lib

apilib.lib : Makefile $(OBJS_API)
	$(GOLIB) $(OBJS_API) out:apilib.lib

%.obj :	%.nas	Makefile
	$(NASK)	$*.nas $*.obj	$*.lst

clean	: 
	-rm *.lst
	-rm *.obj

src :
	make clean
	-rm -f apilib.lib
