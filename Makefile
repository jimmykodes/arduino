BASE ?= arduino:avr
BOARD ?= leonardo
FQBN = ${BASE}:${BOARD}
PORT ?= /dev/cu.usbmodem14301

compile:
	arduino-cli compile -b ${FQBN} ${PROGRAM}

upload:
	arduino-cli upload -b ${FQBN} -p ${PORT} ${PROGRAM}
