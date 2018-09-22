#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

SPHXD=${SPHXD:-$BINDIR/sphxd}
SPHXCLI=${SPHXCLI:-$BINDIR/sphx-cli}
SPHXTX=${SPHXTX:-$BINDIR/sphx-tx}
SPHXQT=${SPHXQT:-$BINDIR/qt/sphx-qt}

[ ! -x $SPHXD ] && echo "$SPHXD not found or not executable." && exit 1

# The autodetected verssphx git tag can screw up manpage output a little bit
SPHXVER=($($SPHXCLI --verssphx | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for sphxd if --verssphx-string is not set,
# but has different outcomes for sphx-qt and sphx-cli.
echo "[COPYRIGHT]" > footer.h2m
$SPHXD --verssphx | sed -n '1!p' >> footer.h2m

for cmd in $SPHXD $SPHXCLI $SPHXTX $SPHXQT; do
  cmdname="${cmd##*/}"
  help2man -N --verssphx-string=${SPHXVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${SPHXVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
