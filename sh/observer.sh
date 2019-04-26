#!/bin/sh
DEBUG=${DEBUG:-0}

# tags: ipc, socket, trap, setsid

# pause / resume process observer with on_state_change action support
# using simple socket poll ipc

trap 'fn_observer_cleanup' EXIT

declare -A observers

fn_observer() {
  declare observer_id
  declare action_pause
  declare action_resume
  declare log

  observer_id="$1" && shift
  [ -n "${observers["$observer_id"]}" ] && \
    echo "[error] observer id is not unique" && \
    observer_cleanup && exit 1
  action_pause="$1" && shift
  [ $# -gt 0 ] && action_resume="$1" && shift
  log='/tmp/nixTools.observer_.log'
  [ $# -gt 0 ] && log="$1" && shift

  declare observer_pid
  declare observer_heartbeat_pid
  declare observer
  declare observer_socket

  observer="$(mktemp -u)"
  observer_socket="$(mktemp -u)"
  echo '#!/bin/sh
DEBUG='${DEBUG:-0}'
log='$log'

trap "exec 3<&-" EXIT

mkfifo '"$observer_socket"'
# ensure access to socket regardless of write process state via local fd
exec 3<'"$observer_socket"'
[ $DEBUG -ge 2 ] && \
  ls -al /dev/fd/ > $log
live=0
last=$live
while true; do
  beat=""
  sleep 1
  read -t 5 beat <&3
  cat <&3 >/dev/null
  [ -n "$beat" ] && live=1 || live=0
  if [ $live -ne $last ]; then
    # state changed
    if [ $live -eq 1 ]; then
      '${action_resume:-"pass=\"\""}' >> $log  # resume
    else
      '${action_pause:-"pass=\"\""}' >> $log  # pause
    fi
  fi
  [ $DEBUG -ge 1 ] && \
    echo "[debug] beat: ${beat:-"dead0"}, state: $last -> $live" >> $log
  last=$live
done
' > "$observer"
  chmod +x "$observer"

  # save pipes
  exec 6<&0 7>&1 8>&2

  # start observer
  setsid "$observer" &
  observer_pid=$!
  [ $DEBUG -ge 1 ] && echo "[debug] observer running with pid '$observer_pid'" 1>&2
  # start heartbeat
  fn_observer_heartbeat "$observer_socket" &
  observer_heartbeat_pid=$!
  [ $DEBUG -ge 1 ] && echo "[debug] observer heartbeat running with pid '$observer_heartbeat_pid'" 1>&2
  observers[${#observers[@]}]="$observer_id|$observer_pid|$observer_heartbeat_pid|$observer|$observer_socket"
}

fn_observer_heartbeat() {
  heart="$1"
  [ $DEBUG -ge 1 ] && echo "[debug] beating: $heart"
  secs=0
  sleep 2
  while true; do
    sleep 1
    echo "live$secs" > "$heart"
    secs=$((secs + 1))
  done
}

fn_observer_cleanup() {
  declare observer_
  declare -a observer
  exec 6<&- 7>&- 8>&-
  for observer_ in "${observers[@]}"; do
    IFS="|"; observer=($(echo "$observer_")); IFS="$IFSORG"
    [ $DEBUG -ge 1 ] && echo "[debug] cleaning up '${observer[0]}' observer" 1>&2
    kill -TERM ${observer[1]} 2>/dev/null  # observer proc
    kill -TERM ${observer[2]} 2>/dev/null  # heartbeat proc
    [ -e "${observer[3]}" ] && rm "${observer[3]}"  # script
    [ -e "${observer[4]}" ] && rm "${observer[4]}"  # socket
  done
}

# initialise observer with actions
fn_observer 'simple' 'echo "[info] process paused"' \
                     'echo "[info] process resumed"'
declare -a observer
IFS="|"; observer=($(echo "${observers[$((${#observers[@]} - 1))]}")); IFS="$IFSORG"
echo "observer_pid: ${observer[1]}"
echo "observer_heartbeat_pid: ${observer[2]}"
echo "observer_script: ${observer[3]}"
echo "observer_socket: ${observer[4]}"

# prog
sleep 1
echo -n "[user] monsters are real? [mash]: "
read 2>/dev/null
