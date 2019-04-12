#!/bin/sh
DEBUG=${DEBUG:-0}

# tags: ipc, socket, trap, setsid

# pause / resume process observer with on_state_change action support
# using simple socket poll ipc

declare observer_pid
declare observer_heartbeat_pid
declare observer
declare observer_socket

fn_observer() {
  declare pause
  declare resume
  declare log
  pause="$1" && shift
  [ $# -gt 0 ] && resume="$1" && shift
  log='/tmp/nixTools.observer_.log'
  [ $# -gt 0 ] && log="$1" && shift
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
      '${resume:-"pass=\"\""}' >> $log  # resume
    else
      '${pause:-"pass=\"\""}' >> $log  # pause
    fi
  fi
  [ $DEBUG -ge 1 ] && \
    echo "[debug] beat: ${beat:-"dead0"}, state: $last -> $live" >> $log
  last=$live
done
' > "$observer"
  chmod +x "$observer"

  # save pipes
  exec 6<&0
  exec 7>&1
  exec 8>&2

  setsid "$observer" &
  observer_pid=$!
  [ $DEBUG -ge 1 ] && echo "[debug] observer running with pid '$observer_pid'"
  # start heartbeat
  fn_observer_heartbeat "$observer_socket" &
  observer_heartbeat_pid=$!
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
  kill -KILL $observer_pid 2>/dev/null 1>&2
  kill -KILL $observer_heartbeat_pid 2>/dev/null 1>&2
  [ -e "$observer" ] && rm "$observer"
  [ -e "$observer_socket" ] && rm "$observer_socket"
}
trap 'fn_observer_cleanup' EXIT

# init observer with pause / resume actions
fn_observer 'echo "[info] process paused"' \
            'echo "[info] process resumed"'

# prog
sleep 1
echo -n "[user] monsters are real? [mash]: "
read 2>/dev/null
