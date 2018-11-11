
.PHONY: do_script

do_script: 
	    sh build.sh arg1 arg2 arg3

prerequisites: do_script

target: prerequisites 
