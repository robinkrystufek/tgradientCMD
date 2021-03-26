#NoTrayIcon
#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Change2CUI=y
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****
#include <ComboConstants.au3>
#include <StructureConstants.au3>
#include <WinAPIConstants.au3>
#include <StatusBarConstants.au3>
#include <Constants.au3>
#include <GUIConstantsEx.au3>
#include <Array.au3>
#include <File.au3>
#include <String.au3>
#include <StaticConstants.au3>
#include <EditConstants.au3>
#include <GUIEdit.au3>
#include <ScrollBarConstants.au3>
#include <Date.au3>
#include <Math.au3>
#include <WindowsConstants.au3>
#include <ButtonConstants.au3>
#include <array.au3>
#include <MsgBoxConstants.au3>
#include <ColorConstants.au3>
#include <SliderConstants.au3>
#include <GDIPlus.au3>
#include <GuiStatusBar.au3>
#include <WinAPI.au3>
#include <AutoItConstants.au3>
#include <Misc.au3>
#include <GuiComboBox.au3>
#include <Timers.au3>
#include <GDIPlus.au3>
#include <ScrollBarsConstants.au3>
#include <FileConstants.au3>
#include <WinAPIFiles.au3>
#include <FrameConstants.au3>
#include <GuiMenu.au3>
#include <MenuConstants.au3>
#include <Crypt.au3>
#include <GDIPlus.au3>
#include <StringConstants.au3>
#include "tgradient_comm.au3"


;$path = "C:\Users\robin_000\Documents\IOCB\mRNA display\191204 tgradientcmd\protocols\test1.txt"

If $CmdLineRaw Then
	$path = $CmdLineRaw
Else
	Exit
EndIf


Global $stamplog = @YEAR & @MON & @MDAY & " " & @HOUR & @MIN & @SEC
While ProcessExists("iomgr.dll")
	ProcessClose("iomgr.dll")
	Sleep(100)
WEnd


Global $statuslogpth = @ScriptDir & "\log\" & $stamplog&"\"& $stamplog & " status.log"
Global $statuslogh
$statuslogh = Run(@ScriptDir & "\iomgr.dll "&$statuslogpth, @ScriptDir, @SW_HIDE, $STDIN_CHILD + $STDOUT_CHILD)
StdinWrite($statuslogh, @YEAR & "/" & @MON & "/" & @MDAY & " " & @HOUR & ":" & @MIN & ":" & @SEC & ": Initializing tgradientcmd")


_init()


_sendButtons($path)



_Exit()

Func _init()
	Local $comnum = IniRead ( @ScriptDir & "\default.cfg", "general", "comport", "1" )
	Local $sportSetError, $resOpen, $instr, $connectedflag = 0
	Local $conArr2[10]
	$conArr2 = _CommListPorts(2)
	_CommClosePort(True)
	statusUpdate("Establishing connection to instrument")
	$resOpen = _CommSetPort(Number($comnum), $sportSetError, "9600", "8", 0, "1", 0)
	If $resOpen = 0 Then
		statusUpdate("Unable to connect to client COM"&$comnum&": " & $sportSetError)
		_Exit()
	Else
		;sleep(1000)
		Local $returnstr = _CommGetLine(@CR, 0, 10000)
		_CommClearInputBuffer()
		statusUpdate("T-Gradient ready at COM" & Number($comnum)&", response: "&$returnstr)
	EndIf


EndFunc


Func _sendButtons($filepath)
	Local $hdlsave = FileOpen($filepath, 0)
	Local $movetxt = FileRead($hdlsave)
	FileClose($hdlsave)
	Local $readarr = StringSplit($movetxt, @CRLF, 3)
	Local $linesplit
	Local $k = 0
	While $k < UBound($readarr)
		If $readarr[$k] = "A" Then
			_CommSendString("PUSH1" & @LF, 1)
			statusUpdate("Push A")
		ElseIf $readarr[$k] = "B" Then
			_CommSendString("PUSH2" & @LF, 1)
			statusUpdate("Push B")
		ElseIf $readarr[$k] = "D" Then
			_CommSendString("PUSH3" & @LF, 1)
			statusUpdate("Push D")
		ElseIf $readarr[$k] = "8" Then
			_CommSendString("PUSH4" & @LF, 1)
			statusUpdate("Push up")
		ElseIf $readarr[$k] = "6" Then
			_CommSendString("PUSH5" & @LF, 1)
			statusUpdate("Push right")
		ElseIf $readarr[$k] = "2" Then
			_CommSendString("PUSH6" & @LF, 1)
			statusUpdate("Push down")
		Else
			statusUpdate("Unknown button '"&$readarr[$k]&"', unable to parse")
		EndIf
		$k += 1
		Sleep(500)
	WEnd
	statusUpdate("Transmission ended")
EndFunc

Func _Exit()
	statusUpdate("Exiting")
	While ProcessExists("iomgr.dll")
		ProcessClose("iomgr.dll")
		Sleep(100)
	WEnd

	Exit
EndFunc

Func statusUpdate($text, $dummy=0)
	StdinWrite($statuslogh, @CRLF & @YEAR & "/" & @MON & "/" & @MDAY & " " & @HOUR & ":" & @MIN & ":" & @SEC & ": "&$text)
EndFunc

