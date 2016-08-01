/* Start JanoEditor if not allready started. Optional: load a given file */

PARSE ARG filename

ADDRESS COMMAND

JanoEditor = 'Tools:Text/Jano_v1.01/JanoEditor'

/* Start JanoEditor if not allready started */
IF SHOW('PORTS', 'JANO') THEN DO
    'RequestChoice >NIL:' '"JanoEditorStart Script" "JanoEditor is already running!" "OK" PUBSCREEN=Workbench'
    EXIT
END
ELSE DO
    'WBRun >NIL:' JanoEditor filename
    DO 2 WHILE ~SHOW('PORTS', 'JANO')   /* checks 2 times for JANO REXX port */
        'waitforport JANO'              /* waitforport waits 10 seconds. If SHOW JANO REXX port then leave */
    END
    IF rc = 5 THEN SAY 'ERROR: Unable to load JANO!'
END