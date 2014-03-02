# komischer kram:
#LOCAL_PATH := $(call my-dir)
#include $(CLEAR_VARS)

CCFLAGS += -D ENABLE_FUNAMBOL_NAMESPACE
CCFLAGS += -D FUNAMBOL_BUILD_API
CCFLAGS += -D FUN_TRANSPORT_AGENT=FUN_CURL_TRANSPORT_AGENT
CCFLAGS += -g -O0
#LOCAL_CFLAGS += -O2
#LOCAL_CFLAGS += -D USE_WCHAR
CCFLAGS += 

INCLUDES+=-I../../src/include/common 
INCLUDES+=-I../../src/include/posix 
INCLUDES+=-I../../src/include/curl 
#INCLUDES+=-I/srv/preware/cross-compile/staging/armv7/usr/include
#INCLUDES+=-I/srv/preware/cross-compile/toolchain/arm-2009q1/lib/gcc/arm-none-linux-gnueabi/4.3.3/include
#INCLUDES+=-I/srv/preware/cross-compile/toolchain/arm-2009q1/lib/gcc/arm-none-linux-gnueabi/4.3.3/include-fixed/


#libc und lib/gcc irgendwie nicht gut.. hm..
#INCLUDES+=-I/srv/preware/cross-compile/toolchain/arm-2009q1/arm-none-linux-gnueabi/libc/usr/include
#INCLUDES+=-I/srv/preware/cross-compile/toolchain/arm-2009q1/arm-none-linux-gnueabi/libc/usr/include/linux

SRC=\
..\..\src\cpp\common\base\base64.cpp \
..\..\src\cpp\common\base\error.cpp \
..\..\src\cpp\common\base\md5.cpp \
..\..\src\cpp\common\base\quoted-printable.cpp \
..\..\src\cpp\common\base\util\ArrayElement.cpp \
..\..\src\cpp\common\base\util\ArrayList.cpp \
..\..\src\cpp\common\base\util\BasicTime.cpp \
..\..\src\cpp\common\base\util\baseutils.cpp \
..\..\src\cpp\common\base\util\EncodingHelper.cpp \
..\..\src\cpp\common\base\util\ItemContainer.cpp \
..\..\src\cpp\common\base\util\MemoryKeyValueStore.cpp \
..\..\src\cpp\common\base\util\PropertyFile.cpp \
..\..\src\cpp\common\base\util\StringBuffer.cpp \
..\..\src\cpp\common\base\util\StringMap.cpp \
..\..\src\cpp\common\base\util\WString.cpp \
..\..\src\cpp\common\base\util\XMLProcessor.cpp \
..\..\src\cpp\common\client\CacheSyncSource.cpp \
..\..\src\cpp\common\client\ConfigSyncSource.cpp \
..\..\src\cpp\common\client\DMTClientConfig.cpp \
..\..\src\cpp\common\client\FileLogAccessor.cpp \
..\..\src\cpp\common\client\FileSyncItem.cpp \
..\..\src\cpp\common\client\FileSyncSource.cpp \
..\..\src\cpp\common\client\MailSourceManagementNode.cpp \
..\..\src\cpp\common\client\OptionParser.cpp \
..\..\src\cpp\common\client\SyncClient.cpp \
..\..\src\cpp\common\inputStream\InputStream.cpp \
..\..\src\cpp\common\inputStream\BufferInputStream.cpp \
..\..\src\cpp\common\event\BaseEvent.cpp \
..\..\src\cpp\common\event\FireEvent.cpp \
..\..\src\cpp\common\event\ManageListener.cpp \
..\..\src\cpp\common\event\SetListener.cpp \
..\..\src\cpp\common\event\SyncEvent.cpp \
..\..\src\cpp\common\event\SyncItemEvent.cpp \
..\..\src\cpp\common\event\SyncSourceEvent.cpp \
..\..\src\cpp\common\event\SyncStatusEvent.cpp \
..\..\src\cpp\common\event\TransportEvent.cpp \
..\..\src\cpp\common\filter\AllClause.cpp \
..\..\src\cpp\common\filter\Clause.cpp \
..\..\src\cpp\common\filter\ClauseUtil.cpp \
..\..\src\cpp\common\filter\FieldClause.cpp \
..\..\src\cpp\common\filter\LogicalClause.cpp \
..\..\src\cpp\common\filter\SourceFilter.cpp \
..\..\src\cpp\common\filter\WhereClause.cpp \
..\..\src\cpp\common\http\HTTPHeader.cpp \
..\..\src\cpp\common\http\Proxy.cpp \
..\..\src\cpp\common\http\TransportAgent.cpp \
..\..\src\cpp\common\http\URL.cpp \
..\..\src\cpp\common\push\CTPConfig.cpp \
..\..\src\cpp\common\push\CTPMessage.cpp \
..\..\src\cpp\common\push\CTPParam.cpp \
..\..\src\cpp\common\push\CTPService.cpp \
..\..\src\cpp\common\push\CTPThreadPool.cpp \
..\..\src\cpp\common\spdm\DMTree.cpp \
..\..\src\cpp\common\spdm\ManagementNode.cpp \
..\..\src\cpp\common\spds\Chunk.cpp \
..\..\src\cpp\common\spds\AccessConfig.cpp \
..\..\src\cpp\common\spds\B64Decoder.cpp \
..\..\src\cpp\common\spds\B64Encoder.cpp \
..\..\src\cpp\common\spds\BodyPart.cpp \
..\..\src\cpp\common\spds\CredentialHandler.cpp \
..\..\src\cpp\common\spds\DataTransformer.cpp \
..\..\src\cpp\common\spds\DataTransformerFactory.cpp \
..\..\src\cpp\common\spds\DefaultConfigFactory.cpp \
..\..\src\cpp\common\spds\DeviceConfig.cpp \
..\..\src\cpp\common\spds\EmailData.cpp \
..\..\src\cpp\common\spds\FileData.cpp \
..\..\src\cpp\common\spds\FolderData.cpp \
..\..\src\cpp\common\spds\FolderExt.cpp \
..\..\src\cpp\common\spds\ItemReader.cpp \
..\..\src\cpp\common\spds\ItemReport.cpp \
..\..\src\cpp\common\spds\spdsutils.cpp \
..\..\src\cpp\common\spds\SyncItem.cpp \
..\..\src\cpp\common\spds\SyncItemStatus.cpp \
..\..\src\cpp\common\spds\SyncItemKeys.cpp \
..\..\src\cpp\common\spds\SyncManager.cpp \
..\..\src\cpp\common\spds\SyncManagerConfig.cpp \
..\..\src\cpp\common\spds\SyncMap.cpp \
..\..\src\cpp\common\spds\SyncMLBuilder.cpp \
..\..\src\cpp\common\spds\SyncMLProcessor.cpp \
..\..\src\cpp\common\spds\SyncReport.cpp \
..\..\src\cpp\common\spds\SyncSource.cpp \
..\..\src\cpp\common\spds\SyncSourceConfig.cpp \
..\..\src\cpp\common\spds\SyncSourceReport.cpp \
..\..\src\cpp\common\syncml\core\AbstractCommand.cpp \
..\..\src\cpp\common\syncml\core\Add.cpp \
..\..\src\cpp\common\syncml\core\Alert.cpp \
..\..\src\cpp\common\syncml\core\Anchor.cpp \
..\..\src\cpp\common\syncml\core\Atomic.cpp \
..\..\src\cpp\common\syncml\core\Authentication.cpp \
..\..\src\cpp\common\syncml\core\Chal.cpp \
..\..\src\cpp\common\syncml\core\CmdID.cpp \
..\..\src\cpp\common\syncml\core\ComplexData.cpp \
..\..\src\cpp\common\syncml\core\ContentTypeInfo.cpp \
..\..\src\cpp\common\syncml\core\ContentTypeParameter.cpp \
..\..\src\cpp\common\syncml\core\Copy.cpp \
..\..\src\cpp\common\syncml\core\Cred.cpp \
..\..\src\cpp\common\syncml\core\CTCap.cpp \
..\..\src\cpp\common\syncml\core\CTPropParam.cpp \
..\..\src\cpp\common\syncml\core\CTTypeSupported.cpp \
..\..\src\cpp\common\syncml\core\Data.cpp \
..\..\src\cpp\common\syncml\core\DataStore.cpp \
..\..\src\cpp\common\syncml\core\Delete.cpp \
..\..\src\cpp\common\syncml\core\DevInf.cpp \
..\..\src\cpp\common\syncml\core\DevInfData.cpp \
..\..\src\cpp\common\syncml\core\DevInfItem.cpp \
..\..\src\cpp\common\syncml\core\DSMem.cpp \
..\..\src\cpp\common\syncml\core\EMI.cpp \
..\..\src\cpp\common\syncml\core\Exec.cpp \
..\..\src\cpp\common\syncml\core\Ext.cpp \
..\..\src\cpp\common\syncml\core\Filter.cpp \
..\..\src\cpp\common\syncml\core\Get.cpp \
..\..\src\cpp\common\syncml\core\Item.cpp \
..\..\src\cpp\common\syncml\core\ItemizedCommand.cpp \
..\..\src\cpp\common\syncml\core\Map.cpp \
..\..\src\cpp\common\syncml\core\MapItem.cpp \
..\..\src\cpp\common\syncml\core\Mem.cpp \
..\..\src\cpp\common\syncml\core\Meta.cpp \
..\..\src\cpp\common\syncml\core\MetInf.cpp \
..\..\src\cpp\common\syncml\core\ModificationCommand.cpp \
..\..\src\cpp\common\syncml\core\NextNonce.cpp \
..\..\src\cpp\common\syncml\core\ObjectDel.cpp \
..\..\src\cpp\common\syncml\core\Property.cpp \
..\..\src\cpp\common\syncml\core\PropParam.cpp \
..\..\src\cpp\common\syncml\core\Put.cpp \
..\..\src\cpp\common\syncml\core\Replace.cpp \
..\..\src\cpp\common\syncml\core\ResponseCommand.cpp \
..\..\src\cpp\common\syncml\core\Results.cpp \
..\..\src\cpp\common\syncml\core\Search.cpp \
..\..\src\cpp\common\syncml\core\Sequence.cpp \
..\..\src\cpp\common\syncml\core\SessionID.cpp \
..\..\src\cpp\common\syncml\core\Source.cpp \
..\..\src\cpp\common\syncml\core\SourceArray.cpp \
..\..\src\cpp\common\syncml\core\SourceRef.cpp \
..\..\src\cpp\common\syncml\core\Status.cpp \
..\..\src\cpp\common\syncml\core\StringElement.cpp \
..\..\src\cpp\common\syncml\core\Sync.cpp \
..\..\src\cpp\common\syncml\core\SyncAlert.cpp \
..\..\src\cpp\common\syncml\core\SyncBody.cpp \
..\..\src\cpp\common\syncml\core\SyncCap.cpp \
..\..\src\cpp\common\syncml\core\SyncHdr.cpp \
..\..\src\cpp\common\syncml\core\SyncML.cpp \
..\..\src\cpp\common\syncml\core\SyncNotification.cpp \
..\..\src\cpp\common\syncml\core\SyncType.cpp \
..\..\src\cpp\common\syncml\core\SyncTypeArray.cpp \
..\..\src\cpp\common\syncml\core\Target.cpp \
..\..\src\cpp\common\syncml\core\TargetRef.cpp \
..\..\src\cpp\common\syncml\core\VerDTD.cpp \
..\..\src\cpp\common\syncml\core\VerProto.cpp \
..\..\src\cpp\common\syncml\formatter\Formatter.cpp \
..\..\src\cpp\common\syncml\parser\Parser.cpp \
..\..\src\cpp\common\vocl\VConverter.cpp \
..\..\src\cpp\common\vocl\VObject.cpp \
..\..\src\cpp\common\vocl\VObjectFactory.cpp \
..\..\src\cpp\common\vocl\VProperty.cpp \
..\..\src\cpp\common\mail\MailAccount.cpp \
..\..\src\cpp\common\mail\MailAccountManager.cpp \
..\..\src\cpp\common\mail\MailData.cpp \
..\..\src\cpp\common\mail\MailMessage.cpp \
..\..\src\cpp\common\mail\MailSyncSourceConfig.cpp \
..\..\src\cpp\posix\base\autotoolsadapter.cpp \
..\..\src\cpp\posix\base\Log.cpp \
..\..\src\cpp\posix\base\adapter\PlatformAdapter.cpp \
..\..\src\cpp\posix\http\MacTransportAgent.cpp \
..\..\src\cpp\posix\http\MozillaTransportAgent.cpp \
..\..\src\cpp\posix\http\CurlTransportAgent.cpp \
..\..\src\cpp\posix\http\TransportAgentFactory.cpp \
..\..\src\cpp\posix\push\FSocket.cpp \
..\..\src\cpp\posix\push\FThread.cpp \
..\..\src\cpp\posix\spdm\DeviceManagementNode.cpp \
..\..\src\cpp\posix\spdm\DMTreeFactory.cpp \
..\..\src\cpp\posix\spds\DESDecoder.cpp \
..\..\src\cpp\posix\spds\DESEncoder.cpp \

#nicht da.. warum???
#..\..\src\cpp\common\spds\MailMessage.cpp \
#..\..\src\cpp\common\spds\MailSyncSourceConfig.cpp \
#..\..\src\cpp\common\vocl\iCalendar\Calendar.cpp \
#..\..\src\cpp\common\vocl\iCalendar\Event.cpp \
#..\..\src\cpp\common\vocl\iCalendar\iCalConverter.cpp \
#..\..\src\cpp\common\vocl\iCalendar\iCalProperty.cpp \
#..\..\src\cpp\common\vocl\iCalendar\ToDo.cpp \
#..\..\src\cpp\common\vocl\vCard\Address.cpp \
#..\..\src\cpp\common\vocl\vCard\BusinessDetail.cpp \
#..\..\src\cpp\common\vocl\vCard\Contact.cpp \
#..\..\src\cpp\common\vocl\vCard\Contact30.cpp \
#..\..\src\cpp\common\vocl\vCard\ContactDetail.cpp \
#..\..\src\cpp\common\vocl\vCard\Email.cpp \
#..\..\src\cpp\common\vocl\vCard\Name.cpp \
#..\..\src\cpp\common\vocl\vCard\Note.cpp \
#..\..\src\cpp\common\vocl\vCard\PersonalDetail.cpp \
#..\..\src\cpp\common\vocl\vCard\Phone.cpp \
#..\..\src\cpp\common\vocl\vCard\Title.cpp \
#..\..\src\cpp\common\vocl\vCard\TypedProperty.cpp \
#..\..\src\cpp\common\vocl\vCard\vCardConverter.cpp \
#..\..\src\cpp\common\vocl\vCard\vCardProperty.cpp \
#..\..\src\cpp\common\vocl\vCard\WebPage.cpp \



OBJ=$(SRC:.cpp=.o)
LIBS=-lssl -lcrypto -lrt -lssl -lcrypto -ldl -lz
LDFLAGS=-L\srv\preware\cross-compile\staging\armv7\usr\lib
OUT=libfunambol.a

AR=arm-none-linux-gnueabi-ar
CCC=arm-none-linux-gnueabi-g++

default: $(OBJ) $(OUT)

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(AR) rcs $(OUT) $(OBJ)

#depend: dep

#dep:
#	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean: 
	del $(OBJ)
	del $(OUT) 
	del Makefile.bak


install:
	cp libfunambol.a ..\..\..\..\cpp-sdk\lib\

