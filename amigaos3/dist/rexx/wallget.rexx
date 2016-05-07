/*
    Very lame WallGet download arexx script
*/

options RESULTS
parse arg downloaddir url

url=strip(url)

address command 'run <nil: >nil: c/wallget'	

DO 50000
	address command 'WaitForPort WALLGET'	
IF SHOW('P', WALLGET) THEN LEAVE
END

ADDRESS "WALLGET" 'ADDURL CONFIG="Download" SAVEDIR="'downloaddir'" URL="'url'"'

exit 0
