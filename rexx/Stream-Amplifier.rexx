/*  $VER: AMPlifier MP3 Stream 1.0 (25.02.2001) */

OPTIONS RESULTS
OPTIONS FAILAT 21
PARSE ARG files


/* Enter here path and AREXX-Port from AMPlifier */
/* Hier den Pfad und AREXX-Port von AMPlifier eintragen */

amplifier_app  = "SYS:Utilities/AMPlifier/AMPlifier"
amplifier_port = "AMPLIFIER.1"


/* Start Amplifier */

IF ~SHOW("PORT", amplifier_port) THEN DO
  IF ~EXISTS(amplifier_app) THEN DO
    SAY "Couldn't find AMPlifier!"
    EXIT
  END
  ADDRESS COMMAND "RUN >NIL: "amplifier_app
  ADDRESS COMMAND "WaitForPort "amplifier_port
END

IF ~SHOW("PORT", amplifier_port) THEN DO
  SAY "Couldn't start AMPlifier!"
  EXIT
END


/* Add files and start playback */

ADDRESS VALUE amplifier_port

STOP
CLEAR
ADD files PLAY

