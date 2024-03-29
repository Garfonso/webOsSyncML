#g++ -O2 -I../cpp-sdk/include/funambol/common/ -I../cpp-sdk/include/funambol/posix/ -I/srv/preware/cross-compile/staging/armv7/usr/include/ -L../cpp-sdk/lib -L/srv/preware/cross-compile/staging/armv7/usr/lib -DENABLE_NAMESPACE -Wl,-Bstatic -lfunambol -Wl,-Bdynamic -lcurl -lssl -lcrypto -lz main.cpp WebOsCalendarSyncSource.cpp WebOsContactsSyncSource.cpp -o webOsSyncMLClient

JAVA_HOME="C:\Program Files\Java\jre6\bin"
PDKDIR=C:\Program Files (x86)\Palm\PDK

#PRE=1
#DEVICEOPTS=-mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp
#PIXI=0
DEVICEOPTS=-mcpu=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp
#DEBUG=0
#DEVICEOPTS+=-g

CC=arm-none-linux-gnueabi-g++
C=arm-none-linux-gnueabi-gcc
CFLAGS=-g -Icpp-sdk\include\common \
           -Icpp-sdk/include/posix \
	   -I"$(PDKDIR)\include" \
	   -I"$(PDKDIR)\include\SDL" \
           -DENABLE_NAMESPACE \
           -c
#CFLAGS += -DUSE_WCHAR
#dynamic case:
STATICLIBS=cpp-sdk/lib/libfunambol.a 
LIBS=-lcurl -lssl -lcrypto -lz -lpdl -lSDL -lGLESv2 -lm
#static case:
#STATICLIBS=cpp-sdk/lib/libfunambol.a \
           #cpp-sdk/lib/libcurl.a \
           #cpp-sdk/lib/libssl.a \
		   #cpp-sdk/lib/libz.a \
		   #cpp-sdk/lib/libcrypto.a \

#LIBS= -lcrypto -lpdl -lSDL -lGLESv2 -lm -lrt
LDFLAGS=-Lcpp-sdk/lib \
	-L"$(PDKDIR)/device/lib" \
	-Wl,--allow-shlib-undefined \
#        -L/srv/preware/cross-compile/staging/armv7/usr/lib \
#        -Wl,-Bdynamic \
#        -lcurl -lssl -lcrypto -lz \
#        -Wl,-Bstatic -lfunambol \
#        -Wl,-Bdynamic \

SOURCES=main.cpp Utils.cpp SysLog.cpp WebOsCalendarSyncSource.cpp WebOsContactsSyncSource.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=webOsSyncML_plugin

# 
all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(DEVICEOPTS) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS)  -Wl,-Bstatic $(STATICLIBS) -Wl,-Bdynamic $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(DEVICEOPTS) $< -o $@
	
kill:
	plink -P 10022 root@localhost -pw "" killall -9 webOsSyncML
	
run:
	plink -P 10022 root@localhost -pw "" "/media/internal/test/webOsSyncML"
	
package: all
	package-it.cmd
	
install-package: package
	palm-install info.mobo.webossyncml_1.0.0_all.ipk	
	
install: all
	pscp -scp -P 10022 -pw "" webOsSyncML root@localhost:/media/cryptofs/apps/usr/palm/applications/info.mobo.webossyncml
	
clean: 
	del $(OBJECTS) $(OBJECTS_C) $(EXECUTABLE)
