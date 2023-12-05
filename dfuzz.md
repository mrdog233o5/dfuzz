### NAME

    dfuzz - A generic simple password manager

### SYNOPSIS

	dfuzz [ all | word ] {options}

### USAGE

	all     check ALL of the posibilities, all characters and numbers, it will take a VERY long time
            syntax:
                dfuzz all <URL> <characters>
            example:
                # list all directory for google that is 2 characters long
                dfuzz all https://www.google.com/ 3

	word    go through a wordlist
            syntax:
                dfuzz word <URL> <wordlist directory>
            example:
                # fuzz https://www.google.com/ with the wordlist stored at /Users/coolguy/hack/wordlist/dir.txt
                dfuzz word https://www.google.com/ /Users/coolguy/hack/wordlist/dir.txt

