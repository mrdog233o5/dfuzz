#!/usr/bin/env bash

gainSudoAccess() {
    if [ $UID != 0 ]; then
        echo "Rerunning script with root user"
        sudo $0 $@
    fi
}

installStuff() {

    if [[ "$OSTYPE" == "darwin"* ]]; then
        mkdir -p /usr/local/bin
        install -v ./main /usr/local/bin/dfuzz
    else
        install -v ./main /usr/bin/dfuzz
    fi

    echo "installed dfuzz"

    mkdir -p /usr/local/share/man/man1
    install dfuzz.1 /usr/local/share/man/man1/dfuzz.1

    echo "installed man page of dfuzz"
}

main() {
    gainSudoAccess
    installStuff
    echo "dfuzz installed successfully"
}

main

