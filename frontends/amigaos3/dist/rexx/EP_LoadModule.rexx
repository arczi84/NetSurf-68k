/* Start EaglePlayer if not allready started. Optional: load a given file */

PARSE ARG filename

ADDRESS COMMAND

EaglePlayer = 'SYS:Utilities/EaglePlayer/EaglePlayer'

/* Start EaglePlayer if not allready started */
IF SHOW('PORTS', 'rexx_EP') THEN DO
    /*'RequestChoice >NIL:' '"EaglePlayerStart Script" "EaglePlayer is already running!" "OK" PUBSCREEN=Workbench'*/
    'Run >NIL:' EaglePlayer Loadmodule filename
    EXIT
END
ELSE DO
    'Run >NIL:' EaglePlayer Loadmodule filename
  /*  DO 2 WHILE ~SHOW('PORTS', 'rexx_EP')   /* checks 2 times for rexx_EP REXX port */
        'waitforport rexx_EP'              /* waitforport waits 10 seconds. If SHOW rexx_EP REXX port then leave */
    EaglePlayer Loadmodule filename
    END */
    IF rc = 5 THEN SAY 'ERROR: Unable to load EaglePlayer!'
END
