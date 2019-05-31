#!/bin/sh

CONF_FILE=/etc/igmpproxy.conf

if [ ! -n "$2" ]; then
  echo "insufficient arguments!"
#  echo "Usage: $0 <Upstream_If> <Downstream_If> <Disabled_If>"
  echo "Usage: $0 <Upstream_If> <Downstream_If>"
  exit 0
fi

UPSTREAM_IF="$1"
DOWNSTREAM_IF="$2"
#DISABLED_IF="$3"

echo "##------------------------------------------------------"  > $CONF_FILE
echo "## Enable Quickleave mode (Sends Leave instantly)" >> $CONF_FILE
echo "##------------------------------------------------------" >> $CONF_FILE
echo "quickleave" >> $CONF_FILE

echo "##------------------------------------------------------" >> $CONF_FILE
echo "## Configuration for Upstream Interface"  >> $CONF_FILE
echo "##------------------------------------------------------" >> $CONF_FILE
echo "phyint $UPSTREAM_IF upstream  ratelimit 0  threshold 1" >> $CONF_FILE
echo "altnet 0.0.0.0/0" >> $CONF_FILE

echo "##------------------------------------------------------"  >> $CONF_FILE
echo "## Configuration for Downstream Interface" >> $CONF_FILE
echo "##------------------------------------------------------" >> $CONF_FILE
echo "phyint $DOWNSTREAM_IF downstream  ratelimit 0  threshold 1" >> $CONF_FILE

echo "##------------------------------------------------------" >> $CONF_FILE
echo "## Configuration for Disabled Interface" >> $CONF_FILE
echo "##------------------------------------------------------" >> $CONF_FILE
#echo "phyint $DISABLED_IF disabled" >> $CONF_FILE
echo "phyint eth2 disabled" >> $CONF_FILE
