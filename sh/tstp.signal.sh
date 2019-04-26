#!/bin/sh

# tag: signal

# only working due to read implementation

fn_trap() {
  echo 'TSTP trap'
  kill -STOP $$
}
trap 'fn_trap' TSTP

echo -n "interrupt me only [mash]: "
read 2>/dev/null

#l=0
# this child will suspend immediately and not return, leaving its
# parent in limbo!
#while true; do echo "$l"; sleep 1; l=$((l+1)); done
