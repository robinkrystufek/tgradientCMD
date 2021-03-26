# tgradientCMD
<b>tgradientCMD controller for Biometra T-Gradient</b><br><br>
Autoit3 script that allows the direct control of Biometra T-Gradient PCR cycler, creation of button seqences and provides a communication overlay. Control over the instrument is established through button-injection with an Arduino module (tgradient_master.ino).<br>
Through the overlay, the button sequences can be called by external commands (command line only).<br><br>
This script uses the Serial port/COM port UDF by Martin Gibson (https://www.autoitscript.com/forum/topic/128546-serial-port-com-port-udf/). Required dll library commg.dll can be downloaded from Martin's website (https://www.mosaiccgl.co.uk/AutoItDownloads/confirm.php?get=COMMGvv2.zip). The file then needs to be placed in the same working directory with the script to work.
