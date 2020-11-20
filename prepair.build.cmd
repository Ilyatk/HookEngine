@mkdir ReleaseBundle\lib
@mkdir ReleaseBundle\include

@copy "!bin\HookEngine\Debug\x64\HookEngine-x64-md-d.lib" ReleaseBundle\lib\
@copy "!bin\HookEngine\Debug\x86\HookEngine-x86-md-d.lib" ReleaseBundle\lib\
@copy "!bin\HookEngine\Debug-MT\x64\HookEngine-x64-mt-d.lib" ReleaseBundle\lib\
@copy "!bin\HookEngine\Debug-MT\x86\HookEngine-x86-mt-d.lib" ReleaseBundle\lib\
@copy "!bin\HookEngine\Release\x64\HookEngine-x64-md.lib" ReleaseBundle\lib\
@copy "!bin\HookEngine\Release\x86\HookEngine-x86-md.lib" ReleaseBundle\lib\
@copy "!bin\HookEngine\Release-MT\x64\HookEngine-x64-mt.lib" ReleaseBundle\lib\
@copy "!bin\HookEngine\Release-MT\x86\HookEngine-x86-mt.lib" ReleaseBundle\lib\

@xcopy /S "HookEngine\include\HookEngine\*.h" "ReleaseBundle\include\HookEngine"
