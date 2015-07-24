from subprocess import call

call("..\\Dependencies\\AStyle\\AStyle.exe --style=allman --recursive -n ..\\Library\\*.c ..\\Library\\*.h", shell=True)
