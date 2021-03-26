#NoTrayIcon
#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Run_Tidy=y
#AutoIt3Wrapper_Tidy_Stop_OnError=n
#AutoIt3Wrapper_Run_Au3Stripper=y
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****
#include <GUIConstantsEx.au3>
#include <FileConstants.au3>
#include <MsgBoxConstants.au3>
#include <WinAPIFiles.au3>
#include <WindowsConstants.au3>
#include <Array.au3>
#include <WinAPIShPath.au3>

Global $path, $heap, $addition, $filehdl, $overwriteflag = 0, $split, $addition2, $dim

If $CmdLineRaw Then
	If StringLeft($CmdLineRaw, 1) = "#" Then
		$path = StringTrimLeft($CmdLineRaw, 1)
		$overwriteflag = 1
	Else
		$path = $CmdLineRaw
		$overwriteflag = 0
	EndIf
Else
	Exit
EndIf


If $overwriteflag = 0 Then $filehdl = FileOpen($path, $FO_OVERWRITE + $FO_CREATEPATH)


While True;ProcessExists("guioverlay.dll") <> 0
	$addition = ConsoleRead()
	If $addition <> "" Then
		If $overwriteflag = 0 Then
			FileWrite($filehdl, $addition)
			$heap &= $addition
		Else
			$split = StringSplit($addition, @CRLF)
			$dim = UBound($split)
			$addition2 = $split[$dim - 15] & @CRLF & $split[$dim - 13] & @CRLF & $split[$dim - 11] & @CRLF & $split[$dim - 9] & @CRLF & $split[$dim - 7] & @CRLF & $split[$dim - 5] & @CRLF & $split[$dim - 3] & @CRLF & $split[$dim - 1]
			$filehdl = FileOpen($path, $FO_OVERWRITE + $FO_CREATEPATH)
			FileWrite($filehdl, $addition2)
			FileClose($filehdl)
			$heap = $addition
		EndIf
	EndIf

	Sleep(250)
WEnd

If $overwriteflag = 0 Then FileClose($filehdl)
