PATH=%PATH%;%DDK%\bin\x86

signtool sign /v /a /n IDRIX /ac thawte_Primary_MS_Cross_Cert.cer /t http://timestamp.verisign.com/scripts/timestamp.dll "..\Release\Setup Files\veracrypt.sys"
signtool sign /v /a /n IDRIX /ac thawte_Primary_MS_Cross_Cert.cer /t http://timestamp.verisign.com/scripts/timestamp.dll "..\Release\Setup Files\veracrypt-x64.sys"

signtool sign /v /a /n IDRIX /ac Thawt_CodeSigning_CA.crt /t http://timestamp.verisign.com/scripts/timestamp.dll "..\Release\Setup Files\VeraCrypt.exe"
signtool sign /v /a /n IDRIX /ac Thawt_CodeSigning_CA.crt /t http://timestamp.verisign.com/scripts/timestamp.dll "..\Release\Setup Files\VeraCrypt Format.exe"
signtool sign /v /a /n IDRIX /ac Thawt_CodeSigning_CA.crt /t http://timestamp.verisign.com/scripts/timestamp.dll "..\Release\Setup Files\VeraCryptExpander.exe"

cd "..\Release\Setup Files\"

copy /V /Y ..\..\..\Translations\*.xml .

"VeraCrypt Setup.exe" /p

del *.xml

cd "..\..\Signing"

signtool sign /v /a /n IDRIX /ac Thawt_CodeSigning_CA.crt /t http://timestamp.verisign.com/scripts/timestamp.dll "..\Release\Setup Files\VeraCrypt Setup 1.0f-2.exe"

pause