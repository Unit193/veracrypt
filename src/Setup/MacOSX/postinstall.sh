#!/bin/bash

if !([ -e "/usr/local/lib/libfuse.2.dylib" ])
then
	ln -s /usr/local/lib/libosxfuse.2.dylib /usr/local/lib/libfuse.2.dylib
fi

chmod -R go-w /Applications/VeraCrypt.app

exit 0
