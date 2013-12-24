#!/usr/bin/env bash

function get_version_info 
{
    LC_ALL=""
    LANG=""
    [ "_`git rev-parse --git-dir 2>/dev/null`" != "_" ];
    GIT=$?
    echo -n 'user:       '
    whoami
    echo -n 'hostname:   '
    hostname
    [ $GIT -eq 0 ] && echo -n 'branch:     '
    [ $GIT -eq 0 ] && git branch |grep '*' |sed 's/*//g;s/ //g' |tr -d '\n' #branch name
    [ $GIT -eq 0 ] && git status |grep -q modified 2>/dev/null && /bin/echo -n '*' #is changed?
    echo
    echo -n 'local date: '
    date
    [ $GIT -eq 0 ] && echo -e "\ncommit info:"
    [ $GIT -eq 0 ] && PAGER="" git log -1 --pretty="branch:    %d%ncommit:     %H%nauthor:     %an <%ae>%ndate:       %ai"
}

function get_commit_hash
{
    LC_ALL=""
    LANG=""
    [ "_`git rev-parse --git-dir 2>/dev/null`" != "_" ];
    GIT=$?
    [ $GIT -eq 0 ] && git status |grep -q modified 2>/dev/null && /bin/echo -n '*' #is changed?
    [ $GIT -eq 0 ] && PAGER="" git log -1 --pretty="%H";
}

function action 
{
    case $1 in
        INFO)
            get_version_info;
            ;;
        COMMIT_HASH)
            get_commit_hash;
            ;;
    esac
}

NL=1;
ACTION="INFO";
while getopts ":ncz" opt; do
    case $opt in
        n)
            NL="0";
            ;;
        c) 
            ACTION="COMMIT_HASH";
            ;;
        z)
            NL="-1";
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            exit 1
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            exit 1
            ;;
    esac
done

case $NL in
    0) 
        action $ACTION | tr '\n' '$' | sed 's/\$/\\n/g' | sed 's/"/\\"/g'; 
        ;;
    1)  
        action $ACTION;
        ;;
    -1) 
        action $ACTION | tr -d '\n';
        ;;
esac;
