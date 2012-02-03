if [ "_$1" = "_-n" ]; then
    sh $0 |tr '\n' '$' |sed 's/\$/\\n/g';
    exit;
    fi

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
[ $GIT -eq 0 ] && git status |grep -q modified 2>/dev/null && /bin/echo -n ' (changed)' #is changed?
echo
echo -n 'local date: '
date
[ $GIT -eq 0 ] && echo -e "\ncommit info:"
[ $GIT -eq 0 ] && PAGER="" git log -1 --pretty="branch:    %d%ncommit:     %H%nauthor:     %an <%ae>%ndate:       %ai"

