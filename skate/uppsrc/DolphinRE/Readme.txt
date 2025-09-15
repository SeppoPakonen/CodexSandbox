https://ftp.aecx.cc/Programming/Dolphin%20OS%20Manual/a-z.html


Not implemented (known):
	Audio Interface
		AIInit
		AIReset
	ARAM user area
		ARGetBaseAddress
		ARGetDMAStatus
		ARInit
		ARReset
	ARAM DMA Task
		ARQFlushQueue
		ARQInit
		ARQPostRequest
		ARQReset
	DSP voice
		AXAcquireVoice
		AXFreeVoice
		AXInit
		AXQuit
		AXRegisterCallback
		AXSetCompressor(int)
		AXSetMode(int)
		AXSetVoiceAddr
		AXSetVoiceAdpcm
		AXSetVoiceAdpcmLoop
		AXSetVoiceMix
		AXSetVoiceSrc
		AXSetVoiceSrcRatio
		AXSetVoiceSrcType
		AXSetVoiceState
		AXSetVoiceType
		AXSetVoiceVe

	PS2 network code
		BucketIntOp(StatsGame*, char const*, char const*, int, char const*, int)
		BucketStringOp(StatsGame*, char const*, char const*, char const*, char const*, int)
		ChangeThreadPriority(int, int)
		CloseStatsConnection()
		CreateSema(SemaParam*)
		DI()
		DeleteSema(int)
		EI()
		FreeGame(StatsGame*)
		GenerateAuth(char*, char*, char*)
		GetChallenge(StatsGame*)
		GetThreadId()
		InitStatsConnection(int)
		IsStatsConnected()
		NewGame(int)
		NewPlayer(StatsGame*, int, char const*)
		PersistThink()
		ReferThreadStatus(int, ThreadParam*)
		RemovePlayer(StatsGame*, int)
		SendGameSnapShot(StatsGame*, void*, int)
		SignalSema(int)
		StartThread(int, void*)

	Memory Card
		CARDCheck
		CARDClose
		CARDCreate
		CARDDelete
		CARDFormat
		CARDFreeBlocks
		CARDGetEncoding
		CARDGetStatus
		CARDInit
		CARDMount
		CARDOpen
		CARDProbeEx
		CARDRead
		CARDSetStatus
		CARDWrite

	Track player
		DTKInit()
		DTKSetRepeatMode(int)

	DVD file system
		DVDCancel
		DVDClose
		DVDConvertPathToEntrynum
		DVDFastOpen
		DVDGetDriveStatus
		DVDInit
		DVDOpen
		DVDReadAsyncPrio
		DVDReadPrio

	Graphics device
		GDFlushCurrToMem
		GDInitGDLObj
		GDOverflowed
		GDPadCurr32
	
	USB interface
		HIOEnumDevices
		HIOInit
		HIOReadMailbox
		HIOWrite
		HIOWriteMailbox

	Cache
		LCDisable
		LCEnable

	Own TODO stuff
		PtrIndex_FindAdd(void*)

Implemented (known)
	Matrix lib (remove C_)
		C_MTXConcat
		C_MTXCopy
		C_MTXFrustum
		C_MTXIdentity
		C_MTXInvXpose
		C_MTXInverse
		C_MTXLightOrtho
		C_MTXLookAt
		C_MTXMultVec
		C_MTXMultVecArray
		C_MTXOrtho
		C_MTXPerspective
		C_MTXQuat
		C_MTXRotAxisRad
		C_MTXRotRad
		C_MTXScale
		C_MTXTrans
		C_VECAdd
		C_VECCrossProduct
		C_VECDistance
		C_VECDotProduct
		C_VECMag
		C_VECNormalize
		C_VECReflect
		C_VECScale
		C_VECSubtract

	Stubs
		DCFlushRange
		DCStoreRange

	Graphics
		GXAbortFrame
		GXAdjustForOverscan
		GXBegin
		GXCallDisplayList
		GXClearGPMetric
		GXClearVtxDesc
		GXCopyDisp
		GXCopyTex
		GXDisableBreakPt
		GXDrawDone
		GXEnableBreakPt
		GXEnableTexOffsets
		GXEnd
		GXFlush
		GXGetCPUFifo
		GXGetFifoBase
		GXGetFifoLimits
		GXGetFifoPtrs
		GXGetFifoSize
		GXGetFifoStatus
		GXGetGPFifo
		GXGetGPStatus
		GXGetOverflowCount
		GXGetProjectionv
		GXGetScissor
		GXGetTexBufferSize
		GXGetViewportv
		GXInit
		GXInitLightAttn
		GXInitLightAttnA
		GXInitLightAttnK
		GXInitLightColor
		GXInitLightDir
		GXInitLightDistAttn
		GXInitLightPos
		GXInitLightSpot
		GXInitSpecularDir
		GXInitSpecularDirHA
		GXInitTexObj
		GXInitTexObjLOD
		GXInvalidateTexAll
		GXInvalidateVtxCache
		GXLoadLightObjImm
		GXLoadLightObjIndx
		GXLoadTexObj
		GXMpal480IntDf
		GXNtsc480IntDf
		GXPeekARGB
		GXPeekZ
		GXPixModeSync
		GXPokeARGB
		GXPokeAlphaUpdate
		GXPokeColorUpdate
		GXPokeZ
		GXPosition3s16
		GXProject
		GXReadDrawSync
		GXReadGPMetric
		GXReadXfRasMetric
		GXResetOverflowCount
		GXSetAlphaCompare
		GXSetAlphaUpdate
		GXSetBlendMode
		GXSetBreakPtCallback
		GXSetChanAmbColor
		GXSetChanCtrl
		GXSetChanMatColor
		GXSetClipMode
		GXSetColorUpdate
		GXSetCopyClear
		GXSetCopyFilter
		GXSetCullMode
		GXSetCurrentMtx
		GXSetDispCopyDst
		GXSetDispCopyGamma
		GXSetDispCopySrc
		GXSetDispCopyYScale
		GXSetDither
		GXSetDrawDone
		GXSetDrawDoneCallback
		GXSetDrawSync
		GXSetDrawSyncCallback
		GXSetDstAlpha
		GXSetFog
		GXSetFogRangeAdj
		GXSetGPMetric
		GXSetLineWidth
		GXSetNumChans
		GXSetNumTevStages
		GXSetNumTexGens
		GXSetPixelFmt
		GXSetPointSize
		GXSetProjection
		GXSetProjectionv
		GXSetScissor
		GXSetScissorBoxOffset
		GXSetTevKAlphaSel
		GXSetTevKColorSel
		GXSetTevOp
		GXSetTevOrder
		GXSetTevSwapMode
		GXSetTevSwapModeTable
		GXSetTexCoordGen
		GXSetTexCoordScaleManually
		GXSetTexCopyDst
		GXSetTexCopySrc
		GXSetVerifyLevel
		GXSetViewport
		GXSetViewportJitter
		GXSetVtxAttrFmt
		GXSetVtxDesc
		GXSetZCompLoc
		GXSetZMode
		GXTexCoord2u16
		GXWaitDrawDone

	Operating System
		OSCachedToPhysical
		OSCreateThread
		OSDisableInterrupts
		OSGetArenaHi
		OSGetArenaLo
		OSGetEuRgb60Mode
		OSGetLanguage
		OSGetProgressiveMode
		OSGetResetButtonState
		OSGetSoundMode
		OSGetTick
		OSGetTime
		OSInit
		OSInitStopwatch
		OSInitThreadQueue
		OSIsThreadSuspended
		OSPanic
		OSReport
		OSResetSystem
		OSRestoreInterrupts
		OSResumeThread
		OSSetAlarm
		OSSetArenaLo
		OSSetEuRgb60Mode
		OSSetProgressiveMode
		OSSetResetCallback
		OSSleepThread
		OSStartStopwatch
		OSStopStopwatch
		OSSuspendThread
		OSTicksToCalendarTime
		OSWakeupThread

	Controller Pad
		PADClamp
		PADControlMotor
		PADInit
		PADRead
		PADRecalibrate
		PADReset
		PADSetAnalogMode
	
	Video Device
		VIFlush
		VIGetDTVStatus
		VIGetNextField
		VIGetTvFormat
		VIInit
		VISetBlack
		VISetNextFrameBuffer
		VISetPostRetraceCallback
		VISetPreRetraceCallback
		VIWaitForRetrace
		VIConfigure


	
Other:
	SIO::LoadIRX(char const*, int, char*, bool)
	Script::PrintContents(Script::CStruct*, int)
	Thread::CreateThread(Thread::PerThreadStruct*)
	TransformAcc
	TransformDouble
	TransformSingle
	WaitSema(int)
	__GDCurrentDL
	__hwInitForEmu()
	bl_player
	bl_server
	bo_set
	g_netdb
	gcd_gamename
	gcd_secret_key
	kConnectionTypeDescs
	ne_noerror
	pd_public_ro
	sceCdDiskReady(int)
	sceCdInit(int)
	sceCdMmode(int)
	sceDNAS2AuthNetStart()
	sceDNAS2GetProxy(int*, char*, int, unsigned short*)
	sceDNAS2GetStatus(sceDNAS2Status_t*)
	sceDNAS2InitNoHDD(sceDNAS2TitleAuthInfo_t*, int, int, int, sceDNAS2TimeoutInfo_t*)
	sceDNAS2SetProxy(int, char const*, unsigned short)
	sceDNAS2Shutdown()
	snPause()
	sn_errno(int)
	sn_h_errno()
	std::OurPrintf(char const*)
	strupr(char*)

